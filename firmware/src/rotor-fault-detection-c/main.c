
/*
  rotor-fault-detection-c (rotor_fd)
  - C11, single-file build, no external libs.
  - Fault channels: outer-race (BPFO) primary; also checks BPFI/BSF/FTF.
  - Added: CLI flags, tach-based order tracking (equal-angle resampling),
           Q15 simulation hooks for headroom/overflow counters.

  Build:  make
  Run:    ./rotor_fd [flags]  (see --help)
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ------------------------- Config & types ------------------------- */

typedef struct {
  int   n;        // rolling elements
  double d;       // ball diameter [m]
  double D;       // pitch diameter [m]
  double beta;    // contact angle [rad]
  double rpm;     // nominal RPM
} BearingGeom;

typedef struct {
  double fs;           // Hz
  double duration_s;   // synthetic
  double band_lo, band_hi; // Hz
  int    nperseg;      // Welch
  int    enable_order; // 1 if order tracking
  int    q15simulate;  // 1 to enable headroom/overflow counters
  char   input[512];   // CSV path or ""
  char   out_json[512];// output JSON
  char   name[128];    // run label
} Config;

typedef struct { double re, im; } cplx;

/* ------------------------- Small utils ------------------------- */
static int is_power_of_two(int n){ return n>0 && (n & (n-1)) == 0; }
static int next_pow2(int n){ int p=1; while(p<n) p<<=1; return p; }
static double clamp(double x, double lo, double hi){ return x<lo?lo:(x>hi?hi:x); }

/* ------------------------- Geometry & predictions ------------------------- */
static double fr_hz(const BearingGeom *g){ return g->rpm / 60.0; }
static double bpfo_hz(const BearingGeom *g){
  return 0.5 * g->n * fr_hz(g) * (1.0 - (g->d/g->D) * cos(g->beta));
}
static double bpfi_hz(const BearingGeom *g){
  return 0.5 * g->n * fr_hz(g) * (1.0 + (g->d/g->D) * cos(g->beta));
}
static double bsf_hz(const BearingGeom *g){
  double ratio = (g->d/g->D) * cos(g->beta);
  return (g->D/(2.0*g->d)) * fr_hz(g) * (1.0 - ratio*ratio);
}
static double ftf_hz(const BearingGeom *g){
  return 0.5 * fr_hz(g) * (1.0 - (g->d/g->D) * cos(g->beta));
}

/* ------------------------- FFT (iterative radix-2) ------------------------- */
static inline cplx cadd(cplx a, cplx b){ cplx r={a.re+b.re, a.im+b.im}; return r; }
static inline cplx csub(cplx a, cplx b){ cplx r={a.re-b.re, a.im-b.im}; return r; }
static inline cplx cmul(cplx a, cplx b){ cplx r={a.re*b.re - a.im*b.im, a.re*b.im + a.im*b.re}; return r; }
static inline cplx cscale(cplx a, double s){ cplx r={a.re*s, a.im*s}; return r; }

static void fft(cplx *a, int N, int dir){
  int j=0;
  for(int i=1;i<N;i++){
    int bit=N>>1;
    for(; j & bit; bit>>=1) j^=bit;
    j^=bit;
    if(i<j){ cplx t=a[i]; a[i]=a[j]; a[j]=t; }
  }
  for(int len=2; len<=N; len<<=1){
    double ang = 2*M_PI/len * (dir>0 ? -1 : +1);
    cplx wlen = {cos(ang), sin(ang)};
    for(int i=0;i<N;i+=len){
      cplx w={1.0,0.0};
      for(int k=0;k<len/2;k++){
        cplx u=a[i+k], v=cmul(a[i+k+len/2], w);
        a[i+k]=cadd(u,v);
        a[i+k+len/2]=csub(u,v);
        w=cmul(w,wlen);
      }
    }
  }
  if(dir<0){
    double inv=1.0/N;
    for(int i=0;i<N;i++) a[i]=cscale(a[i], inv);
  }
}

/* ------------------------- FIR band-pass ------------------------- */
static void fir_bandpass(double *h, int taps, double fs, double f1, double f2){
  int M=taps-1;
  double fc1=f1/fs, fc2=f2/fs;
  for(int n=0;n<taps;n++){
    double m=n - M/2.0;
    double w=0.54 - 0.46*cos(2*M_PI*n/M);
    double h1 = (m==0.0)? 2*fc2 : sin(2*M_PI*fc2*m)/(M_PI*m);
    double h2 = (m==0.0)? 2*fc1 : sin(2*M_PI*fc1*m)/(M_PI*m);
    h[n]=w*(h1-h2);
  }
  double sum=0.0; for(int i=0;i<taps;i++) sum+=h[i];
  for(int i=0;i<taps;i++) h[i]/=(sum==0.0?1.0:sum);
}

static void conv_fir(const double *x, int N, const double *h, int L, double *y){
  for(int n=0;n<N;n++){
    double acc=0.0;
    for(int k=0;k<L;k++){
      int idx=n-k; if(idx>=0) acc+=h[k]*x[idx];
    }
    y[n]=acc;
  }
}

/* ------------------------- Hilbert envelope (FFT) ------------------------- */
static void envelope_fft(const double *x, int N, double *env){
  cplx *X=(cplx*)malloc(sizeof(cplx)*N);
  for(int i=0;i<N;i++){ X[i].re=x[i]; X[i].im=0.0; }
  fft(X,N,+1);
  for(int k=1;k<N/2;k++) X[k]=cscale(X[k], 2.0);
  for(int k=N/2+1;k<N;k++){ X[k].re=0; X[k].im=0; }
  fft(X,N,-1);
  for(int i=0;i<N;i++){
    double re=X[i].re, im=X[i].im;
    env[i]=sqrt(re*re + im*im);
  }
  free(X);
}

/* ------------------------- Welch PSD (Hann, 50%% overlap) ------------------------- */
static void hann_window(double *w, int N){
  for(int n=0;n<N;n++) w[n]=0.5*(1.0 - cos(2*M_PI*n/(N-1)));
}
static void welch_psd(const double *x, int N, int nperseg, int noverlap,
                      double fs, double *freqs, double *Pxx){
  int step=nperseg - noverlap;
  int segments=(N - noverlap)/step;
  if(segments<=0){ segments=1; }
  double *win=(double*)malloc(sizeof(double)*nperseg);
  hann_window(win,nperseg);
  double win_pow=0.0; for(int i=0;i<nperseg;i++) win_pow+=win[i]*win[i];
  cplx *buf=(cplx*)malloc(sizeof(cplx)*nperseg);
  for(int k=0;k<=nperseg/2;k++){ freqs[k]=(fs*k)/nperseg; Pxx[k]=0.0; }

  for(int s=0;s<segments;s++){
    int start=s*step;
    if(start+nperseg>N) break;
    for(int i=0;i<nperseg;i++){ double v=x[start+i]*win[i]; buf[i].re=v; buf[i].im=0.0; }
    fft(buf,nperseg,+1);
    for(int k=0;k<=nperseg/2;k++){
      double a=buf[k].re, b=buf[k].im;
      double power=(a*a + b*b);
      Pxx[k]+=power;
    }
  }
  for(int k=0;k<=nperseg/2;k++){
    Pxx[k]/=segments;
    Pxx[k]/=(win_pow);
    Pxx[k]*=2.0; // one-sided
    Pxx[k]/=fs;
  }
  Pxx[0]*=0.5; if(nperseg%2==0) Pxx[nperseg/2]*=0.5;
  free(win); free(buf);
}

/* ------------------------- Peak matching ------------------------- */
typedef struct {
  int found; int index; double freq; double snr_db; double df; int harmonic;
} PeakHit;

static double snr_db_ring(const double *Pxx, int N, int idx){
  int guard=3;
  int lo=idx-10*guard; if(lo<0) lo=0;
  int hi=idx+10*guard; if(hi>=N) hi=N-1;
  double sum=0; int cnt=0;
  for(int i=lo;i<=hi;i++){
    if(i>=idx-guard && i<=idx+guard) continue;
    sum+=Pxx[i]; cnt++;
  }
  double noise=(cnt>0? sum/cnt : 1e-20);
  if(noise<1e-20) noise=1e-20;
  return 10.0*log10(Pxx[idx]/noise);
}

static PeakHit nearest_peak(const double *freqs, const double *Pxx, int N, double target, double tol_rel, double min_snr_db){
  PeakHit h={0,0,0,0,0,0};
  if(target<=0) return h;
  double best_amp=-1; int best_idx=-1;
  for(int i=1;i<N-1;i++){
    if(!(Pxx[i]>Pxx[i-1] && Pxx[i]>Pxx[i+1])) continue;
    double rel=fabs(freqs[i]-target)/target;
    if(rel<=tol_rel){
      double amp=Pxx[i];
      if(amp>best_amp){ best_amp=amp; best_idx=i; }
    }
  }
  if(best_idx>=0){
    h.index=best_idx;
    h.freq=freqs[best_idx];
    h.snr_db=snr_db_ring(Pxx,N,best_idx);
    h.df=freqs[1]-freqs[0];
    h.harmonic=0;
    h.found=(h.snr_db>=min_snr_db);
  }
  return h;
}

/* ------------------------- CSV loader ------------------------- */
static int load_csv(const char *path, double **acc, double **tach, int *N){
  FILE *f=fopen(path,"rb");
  if(!f) return -1;
  int cap=1<<20, n=0; // pre-alloc
  double *a=(double*)malloc(sizeof(double)*cap);
  double *t=(double*)malloc(sizeof(double)*cap);
  int cols=0;
  while(1){
    double v1,v2; int r;
    long pos=ftell(f);
    r=fscanf(f," %lf , %lf",&v1,&v2);
    if(r==2){
      if(n>=cap){ cap*=2; a=(double*)realloc(a,sizeof(double)*cap); t=(double*)realloc(t,sizeof(double)*cap); }
      a[n]=v1; t[n]=v2; n++; cols=2; continue;
    }
    fseek(f,pos,SEEK_SET);
    r=fscanf(f," %lf",&v1);
    if(r==1){
      if(n>=cap){ cap*=2; a=(double*)realloc(a,sizeof(double)*cap); t=(double*)realloc(t,sizeof(double)*cap); }
      a[n]=v1; t[n]=NAN; n++; if(cols==0) cols=1; continue;
    }
    break;
  }
  fclose(f);
  *acc=a; *tach=(cols==2? t: NULL); *N=n; if(cols==1) free(t);
  return 0;
}

/* ------------------------- Equal-angle resampling (order tracking) ------------------------- */
static int detect_tach_edges(const double *tach, int N, double thresh, int *idx, int max_edges){
  int m=0;
  int prev_high = tach[0]>thresh;
  for(int i=1;i<N;i++){
    int high=tach[i]>thresh;
    if(!prev_high && high){
      if(m<max_edges){ idx[m++]=i; }
      else break;
    }
    prev_high=high;
  }
  return m;
}

// Linear sample env at arbitrary time index (continuous index into env array)
static inline double sample_linear(const double *env, int N, double idx){
  if(idx<0) return env[0];
  int i=(int)idx;
  if(i>=N-1) return env[N-1];
  double frac=idx - i;
  return env[i]*(1.0-frac) + env[i+1]*frac;
}

// Resample envelope to equal-angle samples: M samples per revolution
static double* resample_equal_angle(const double *env, int N, double fs,
                                    const int *edges, int E, int M, int *outN){
  if(E<2){ *outN=0; return NULL; }
  int total = (E-1)*M;
  double *y = (double*)malloc(sizeof(double)*total);
  int k=0;
  for(int e=0;e<E-1;e++){
    int i0=edges[e], i1=edges[e+1];
    double t0=i0/fs, t1=i1/fs;
    for(int m=0;m<M;m++){
      double alpha = (double)m / (double)M; // 0..1 within rev
      double t = t0 + alpha*(t1 - t0);
      double idx = t*fs;
      y[k++] = sample_linear(env, N, idx);
    }
  }
  *outN=total;
  return y;
}

/* ------------------------- Q15 simulation (diagnostics) ------------------------- */
typedef struct { int overflows; double peak_abs; } Q15Mon;
static void q15_track_array(Q15Mon *mon, const double *x, int N){
  for(int i=0;i<N;i++){
    double v = clamp(x[i], -1.999, 1.999);
    if(fabs(v) > mon->peak_abs) mon->peak_abs=fabs(v);
    if(v <= -1.0 || v >= 1.0) mon->overflows++;
  }
}

/* ------------------------- JSON writer ------------------------- */
static void write_json(const char *path,
  const char *name, const Config *cfg, const BearingGeom *g,
  double fr, double f_bpfo, double f_bpfi, double f_bsf, double f_ftf,
  PeakHit hz_bpfo, PeakHit hz_bpfi, PeakHit hz_bsf, PeakHit hz_ftf,
  int have_order, double mean_order_fs,
  PeakHit ord_bpfo, PeakHit ord_bpfi, PeakHit ord_bsf, PeakHit ord_ftf,
  const char *fault, double conf, const char *rationale,
  const Q15Mon *mon){

  FILE *f=fopen(path,"wb"); if(!f){ fprintf(stderr,"Cannot open %s\n", path); return; }
  time_t now=time(NULL);
  fprintf(f,"{\n");
  fprintf(f,"  \"run\": {\"timestamp\": %ld, \"name\": \"%s\"},\n", (long)now, name);
  fprintf(f,"  \"signal\": {\"fs_hz\": %.6f, \"duration_s\": %.6f, \"window\": \"hann\", \"nperseg\": %d, \"band_hz\": [%.1f, %.1f], \"order_tracking\": %s},\n",
    cfg->fs, cfg->duration_s, cfg->nperseg, cfg->band_lo, cfg->band_hi, have_order? "true":"false");
  fprintf(f,"  \"geometry\": {\"n\": %d, \"d\": %.6f, \"D\": %.6f, \"beta_deg\": %.2f, \"rpm\": %.2f},\n",
    g->n, g->d, g->D, g->beta*180.0/M_PI, g->rpm);
  fprintf(f,"  \"predictions_hz\": {\"fr\": %.6f, \"BPFO\": %.6f, \"BPFI\": %.6f, \"BSF\": %.6f, \"FTF\": %.6f},\n",
    fr, f_bpfo, f_bpfi, f_bsf, f_ftf);

  // helper macro to write peak
  #define WRITE_HZ(name,h,trail) do { \
    if((h).found) fprintf(f,"    \"" name "\": {\"freq\": %.6f, \"snr_db\": %.6f, \"df\": %.6f, \"harmonic\": %d}%s\n",(h).freq,(h).snr_db,(h).df,(h).harmonic, trail); \
    else fprintf(f,"    \"" name "\": null%s\n", trail); } while(0)

  fprintf(f,"  \"detections_hz\": {\n");
  WRITE_HZ("BPFO", hz_bpfo, ",");
  WRITE_HZ("BPFI", hz_bpfi, ",");
  WRITE_HZ("BSF",  hz_bsf,  ",");
  WRITE_HZ("FTF",  hz_ftf,  "");
  fprintf(f,"  },\n");

  if(have_order){
    fprintf(f,"  \"detections_order\": {\n");
    // In order domain, 'freq' is expressed in orders (cycles per rev), df in orders
    WRITE_HZ("BPFO", ord_bpfo, ",");
    WRITE_HZ("BPFI", ord_bpfi, ",");
    WRITE_HZ("BSF",  ord_bsf,  ",");
    WRITE_HZ("FTF",  ord_ftf,  "");
    fprintf(f,"  },\n");
  }

  if(mon){
    fprintf(f,"  \"q15\": {\"peak_abs\": %.6f, \"overflow_events\": %d},\n", mon->peak_abs, mon->overflows);
  }

  fprintf(f,"  \"decision\": {\"fault_class\": \"%s\", \"confidence\": %.3f, \"rationale\": \"%s\"}\n", fault, conf, rationale);
  fprintf(f,"}\n");
  fclose(f);
}

/* ------------------------- CLI ------------------------- */
static void defaults(Config *c, BearingGeom *g){
  c->fs=51200.0; c->duration_s=4.0; c->band_lo=4000.0; c->band_hi=8000.0;
  c->nperseg=65536; c->enable_order=0; c->q15simulate=0;
  c->input[0]='\0'; strcpy(c->out_json,"diagnostic.json"); strcpy(c->name,"run");
  g->n=8; g->d=0.010; g->D=0.050; g->beta=0.0; g->rpm=1800.0;
}
static void help(){
  printf("rotor_fd — options:\\n");
  printf("  --input <path>        CSV (acc or acc,tach)\\n");
  printf("  --fs <Hz>             sample rate (default 51200)\\n");
  printf("  --duration <s>        synthetic duration (default 4)\\n");
  printf("  --band <lo> <hi>      band-pass in Hz (default 4000 8000)\\n");
  printf("  --nperseg <N>         Welch segment length (pow2)\\n");
  printf("  --geom n=..,d=..,D=..,beta_deg=..,rpm=..\\n");
  printf("  --order               enable order tracking (needs tach)\\n");
  printf("  --q15simulate         track Q15 headroom/overflow\\n");
  printf("  --out <path>          output JSON (default diagnostic.json)\\n");
  printf("  --name <label>        run label\\n");
  printf("  --help\\n");
}
static void parse_geom(const char *s, BearingGeom *g){
  // crude parser: key=value pairs separated by commas
  char buf[256]; strncpy(buf,s,sizeof(buf)-1); buf[sizeof(buf)-1]='\0';
  char *tok = strtok(buf, ",");
  while(tok){
    char k[64]; double v=0.0; int ki=0, vi=0; int eq=-1;
    for(int i=0; tok[i]; ++i){ if(tok[i]=='='){ eq=i; break; } }
    if(eq>0){
      strncpy(k, tok, eq); k[eq]='\0';
      v = atof(tok+eq+1);
      if(strcmp(k,"n")==0) g->n=(int)v;
      else if(strcmp(k,"d")==0) g->d=v;
      else if(strcmp(k,"D")==0) g->D=v;
      else if(strcmp(k,"beta_deg")==0) g->beta = v * M_PI/180.0;
      else if(strcmp(k,"rpm")==0) g->rpm = v;
    }
    tok = strtok(NULL,",");
  }
}
static void parse_cli(int argc, char **argv, Config *c, BearingGeom *g){
  for(int i=1;i<argc;i++){
    if(strcmp(argv[i],"--help")==0){ help(); exit(0); }
    else if(strcmp(argv[i],"--input")==0 && i+1<argc){ strncpy(c->input, argv[++i], sizeof(c->input)-1); }
    else if(strcmp(argv[i],"--fs")==0 && i+1<argc){ c->fs=atof(argv[++i]); }
    else if(strcmp(argv[i],"--duration")==0 && i+1<argc){ c->duration_s=atof(argv[++i]); }
    else if(strcmp(argv[i],"--band")==0 && i+2<argc){ c->band_lo=atof(argv[++i]); c->band_hi=atof(argv[++i]); }
    else if(strcmp(argv[i],"--nperseg")==0 && i+1<argc){ c->nperseg=atoi(argv[++i]); }
    else if(strcmp(argv[i],"--geom")==0 && i+1<argc){ parse_geom(argv[++i], g); }
    else if(strcmp(argv[i],"--order")==0){ c->enable_order=1; }
    else if(strcmp(argv[i],"--q15simulate")==0){ c->q15simulate=1; }
    else if(strcmp(argv[i],"--out")==0 && i+1<argc){ strncpy(c->out_json, argv[++i], sizeof(c->out_json)-1); }
    else if(strcmp(argv[i],"--name")==0 && i+1<argc){ strncpy(c->name, argv[++i], sizeof(c->name)-1); }
  }
}

/* ------------------------- Main ------------------------- */
int main(int argc, char **argv){
  Config cfg; BearingGeom geom; defaults(&cfg,&geom); parse_cli(argc,argv,&cfg,&geom);

  // 1) Acquire signal
  int N = (int)(cfg.fs * cfg.duration_s);
  double *acc=NULL, *tach=NULL;
  if(cfg.input[0]){
    if(load_csv(cfg.input, &acc, &tach, &N)!=0){ fprintf(stderr,"Failed to read %s\\n", cfg.input); return 1; }
  } else {
    acc=(double*)malloc(sizeof(double)*N);
    tach=(double*)malloc(sizeof(double)*N);
    // Synthetic AM at 5 kHz, modulated by BPFO; tach 1 pulse/rev
    double fr = fr_hz(&geom);
    double f_bpfo = bpfo_hz(&geom);
    double fc=5000.0, m=0.5, snr_db=20.0;
    double sig_pow=0.0;
    srand(7);
    for(int i=0;i<N;i++){
      double t=i/cfg.fs;
      double s = (1.0 + m*sin(2*M_PI*f_bpfo*t)) * sin(2*M_PI*fc*t);
      acc[i]=s; sig_pow+=s*s;
      double phase = fmod(2*M_PI*fr*t, 2*M_PI);
      tach[i] = (phase < M_PI)? 0.0 : 1.0;
    }
    sig_pow/=N;
    double noise_pow = sig_pow / pow(10.0, snr_db/10.0);
    for(int i=0;i<N;i++){
      double u1=((double)rand()+1.0)/(RAND_MAX+1.0);
      double u2=((double)rand()+1.0)/(RAND_MAX+1.0);
      double z=sqrt(-2.0*log(u1))*cos(2*M_PI*u2);
      acc[i]+= z*sqrt(noise_pow);
    }
  }

  // Q15 monitor (diagnostics only)
  Q15Mon mon={0,0.0};
  if(cfg.q15simulate){
    for(int i=0;i<N;i++){ double v=acc[i]; if(fabs(v)>mon.peak_abs) mon.peak_abs=fabs(v); if(v<=-1.0 || v>=1.0) mon.overflows++; }
  }

  // 2) Band-pass -> 3) Envelope
  int taps=257;
  double *h=(double*)malloc(sizeof(double)*taps);
  double *y=(double*)malloc(sizeof(double)*N);
  fir_bandpass(h,taps,cfg.fs,cfg.band_lo,cfg.band_hi);
  conv_fir(acc,N,h,taps,y);

  // zero-pad to next pow2 for envelope FFT
  if(!is_power_of_two(N)){ int np=next_pow2(N); y=(double*)realloc(y,sizeof(double)*np); for(int i=N;i<np;i++) y[i]=0.0; N=np; }
  double *env=(double*)malloc(sizeof(double)*N);
  envelope_fft(y,N,env);

  // 4) Welch PSD (Hz)
  int nperseg=cfg.nperseg;
  if(nperseg>N){ nperseg=1; while(nperseg*2<=N) nperseg*=2; }
  int noverlap=nperseg/2;
  int K = nperseg/2 + 1;
  double *f_hz=(double*)malloc(sizeof(double)*K);
  double *P_hz=(double*)malloc(sizeof(double)*K);
  welch_psd(env,N,nperseg,noverlap,cfg.fs,f_hz,P_hz);

  // 5) Predictions (Hz)
  double fr = fr_hz(&geom);
  double f_bpfo = bpfo_hz(&geom);
  double f_bpfi = bpfi_hz(&geom);
  double f_bsf  = bsf_hz(&geom);
  double f_ftf  = ftf_hz(&geom);

  // 6) Peak matches (Hz)
  const double TOL_REL = 0.02, MIN_SNR_DB=6.0;
  PeakHit hz_bpfo = nearest_peak(f_hz,P_hz,K,f_bpfo,TOL_REL,MIN_SNR_DB);
  PeakHit hz_bpfi = nearest_peak(f_hz,P_hz,K,f_bpfi,TOL_REL,MIN_SNR_DB);
  PeakHit hz_bsf  = nearest_peak(f_hz,P_hz,K,f_bsf, TOL_REL,MIN_SNR_DB);
  PeakHit hz_ftf  = nearest_peak(f_hz,P_hz,K,f_ftf, TOL_REL,MIN_SNR_DB);
  if(!hz_bpfo.found){ for(int H=2;H<=3;H++){ PeakHit t=nearest_peak(f_hz,P_hz,K,H*f_bpfo,TOL_REL,MIN_SNR_DB); if(t.found){ t.harmonic=H; hz_bpfo=t; break; } } }
  if(!hz_bpfi.found){ for(int H=2;H<=3;H++){ PeakHit t=nearest_peak(f_hz,P_hz,K,H*f_bpfi,TOL_REL,MIN_SNR_DB); if(t.found){ t.harmonic=H; hz_bpfi=t; break; } } }

  // 7) Optional order tracking
  int have_order = 0;
  PeakHit ord_bpfo={0}, ord_bpfi={0}, ord_bsf={0}, ord_ftf={0};
  double mean_order_fs = 0.0; // samples per revolution
  if(cfg.enable_order && tach){
    // detect rising edges
    int maxE = N/10 + 10;
    int *edges=(int*)malloc(sizeof(int)*maxE);
    int E=detect_tach_edges(tach,N,0.5,edges,maxE);
    if(E>=2){
      have_order=1;
      // resample equal-angle: M samples/rev
      int M=1024, NA=0;
      double *env_ord = resample_equal_angle(env,N,cfg.fs,edges,E,M,&NA);
      if(env_ord && NA>2*M){
        // Welch in order domain: fs_orders = M (samples per revolution)
        int npo=cfg.nperseg; if(npo>NA){ npo=1; while(npo*2<=NA) npo*=2; }
        int K2=npo/2+1; double *f_ord=(double*)malloc(sizeof(double)*K2), *P_ord=(double*)malloc(sizeof(double)*K2);
        welch_psd(env_ord,NA,npo,npo/2,(double)M,f_ord,P_ord);
        // predicted orders
        double ord_bpfo_pred = f_bpfo / fr;
        double ord_bpfi_pred = f_bpfi / fr;
        double ord_bsf_pred  = f_bsf  / fr;
        double ord_ftf_pred  = f_ftf  / fr;
        ord_bpfo = nearest_peak(f_ord,P_ord,K2,ord_bpfo_pred,TOL_REL,MIN_SNR_DB);
        ord_bpfi = nearest_peak(f_ord,P_ord,K2,ord_bpfi_pred,TOL_REL,MIN_SNR_DB);
        ord_bsf  = nearest_peak(f_ord,P_ord,K2,ord_bsf_pred, TOL_REL,MIN_SNR_DB);
        ord_ftf  = nearest_peak(f_ord,P_ord,K2,ord_ftf_pred, TOL_REL,MIN_SNR_DB);
        if(!ord_bpfo.found){ for(int H=2;H<=3;H++){ PeakHit t=nearest_peak(f_ord,P_ord,K2,H*ord_bpfo_pred,TOL_REL,MIN_SNR_DB); if(t.found){ t.harmonic=H; ord_bpfo=t; break; } } }
        if(!ord_bpfi.found){ for(int H=2;H<=3;H++){ PeakHit t=nearest_peak(f_ord,P_ord,K2,H*ord_bpfi_pred,TOL_REL,MIN_SNR_DB); if(t.found){ t.harmonic=H; ord_bpfi=t; break; } } }
        mean_order_fs = (double)M;
        free(f_ord); free(P_ord);
      }
      if(env_ord) free(env_ord);
    }
    free(edges);
  }

  // 8) Decision (toy): favor BPFO if either Hz or Order shows strong hit
  const char *fault="unknown"; double conf=0.0; char rationale[128]="";
  double score = 0.0;
  if(hz_bpfo.found) score = fmax(score, (hz_bpfo.snr_db - 6.0)/12.0);
  if(ord_bpfo.found) score = fmax(score, (ord_bpfo.snr_db - 6.0)/12.0);
  score = clamp(score, 0.0, 1.0);
  if(score>0.0){ fault="outer_race"; conf=score; strcpy(rationale, "BPFO matched in spectrum"); }
  else strcpy(rationale, "no characteristic lines matched with SNR");

  // 9) JSON
  write_json(cfg.out_json, cfg.name, &cfg, &geom,
             fr, f_bpfo, f_bpfi, f_bsf, f_ftf,
             hz_bpfo, hz_bpfi, hz_bsf, hz_ftf,
             have_order, mean_order_fs,
             ord_bpfo, ord_bpfi, ord_bsf, ord_ftf,
             fault, conf, rationale,
             (cfg.q15simulate? &mon : NULL));

  fprintf(stderr,"Wrote %s (fault=%s, conf=%.2f)%s\n", cfg.out_json, fault, conf,
    have_order? " [order-tracking enabled]":"");
  free(acc); if(tach) free(tach); free(h); free(y); free(env); free(f_hz); free(P_hz);
  return 0;
}

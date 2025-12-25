unzip rotor-fault-detection-c.zip && cd rotor-fault-detection-c
make
# Synthetic demo
./rotor_fd --name rampA
# CSV with accel,tach @ 51.2 kHz
./rotor_fd --input my_signal.csv --fs 51200 --band 4000 8000 --nperseg 65536 \
  --geom n=8,d=0.010,D=0.050,beta_deg=0,rpm=1800 --order --name run1
# Output: diagnostic.json

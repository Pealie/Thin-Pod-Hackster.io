# Thin-Pod and a Possible Evolution Toward FPGA

## Executive summary

An evolution toward FPGA makes sense only if Thin-Pod begins to derive its value primarily from deterministic signal handling, parallel DSP pipelines, or unusually strict timing requirements. If the centre of gravity remains sensor control, modest feature extraction, networking, security, OTA, and gateway orchestration, an MCU or MPU-centred design remains the cleaner and lower-risk path.

For Thin-Pod, the most plausible role for FPGA is not at the battery-powered sensor node but at the gateway or instrument head, where power, thermal budget, and physical volume are less constrained and where a more substantial signal-processing pipeline can be justified.

## Framing the question

The key issue is not whether an FPGA is 'more powerful' than an MCU or MPU. The real question is whether the workload is better expressed as a **hardware dataflow pipeline** than as sequential software running on a processor.

MCUs and MPUs are generally the right tools when the design is dominated by:

* configuration and control logic
* networking and security stacks
* OTA and fleet management
* logging and storage policy
* UI and orchestration
* modest DSP workloads that fit comfortably inside software timing budgets

FPGAs become attractive when the design is dominated by:

* deterministic streaming pipelines
* fixed-latency processing
* high-rate or multi-channel synchronous acquisition
* custom interfaces or timing relationships
* heavy overlap between acquisition, conditioning, and feature extraction
* parallel DSP blocks that must operate continuously without scheduler jitter

## What an FPGA would most naturally do in Thin-Pod

### 1\. Deterministic acquisition front end

A gateway-class FPGA could handle the front end of the vibration pipeline with fixed timing and precise control over buffering and data movement. Likely candidates include:

* synchronous sampling and channel alignment
* timestamping and event capture
* deterministic buffering and framing
* encoder or tachometer capture for order-tracking style analysis
* trigger logic for burst capture or anomaly-driven acquisition

This is the point at which FPGA begins to look like instrumentation rather than general embedded compute.

### 2\. Streaming DSP

The strongest technical case for FPGA lies in continuous signal-path work such as:

* decimation and resampling
* FIR and IIR filtering
* windowing
* FFT preparation or spectral accumulation
* envelope extraction
* feature precomputation
* threshold or rule-based event generation

These workloads are inherently parallel and latency-sensitive. They suit hardware pipelines well, especially when several stages must run continuously and concurrently.

### 3\. Multi-channel scaling

If Thin-Pod were to evolve into a more instrument-like gateway with several channels, more sensors, or simultaneous streams, FPGA would offer an easier scaling path than repeatedly stretching CPU scheduling. Phase alignment and repeatable latency become materially easier to guarantee when the signal chain is implemented as hardware.

## What should remain in firmware or Linux software

Most of Thin-Pod's higher-level behaviour should remain outside the FPGA fabric.

### 1\. Node-side control and power management

At the sensor node, the dominant concerns remain:

* low power operation
* sensor standby duty cycling
* simple acquisition control
* transport over UWB or similar links
* basic health reporting
* fault handling

Those jobs are typically better served by MCU firmware. Unless the node acquires unusually demanding front-end timing or local DSP requirements, FPGA at the node is hard to justify.

### 2\. Networking, security, and lifecycle management

The following are far better left in software:

* wireless session management
* authentication and key handling
* OTA and rollback logic
* logging, diagnostics, and metrics
* storage and file handling
* cloud or dashboard integration
* configuration and remote management

These domains change frequently, benefit from software tooling, and gain little from hardware implementation.

### 3\. Inference and model lifecycle

The machine-learning model itself should usually remain in MCU or MPU software at first. The more valuable early use of FPGA is to improve the signal presented to the classifier rather than to force the classifier itself into programmable logic.

A sensible division would be:

* FPGA for acquisition, conditioning, and feature generation
* processor software for inference, threshold tuning, deployment, retraining workflow, and telemetry

Only if inference later becomes highly repetitive, latency-critical, or computationally dominant would hardware acceleration for parts of that path deserve serious consideration.

## DE10-Nano as a development step

The DE10-Nano remains a credible exploratory platform because it offers a real SoC-FPGA environment rather than a pure FPGA board. It combines FPGA fabric with a hard processor system, which makes it suitable for partitioning work between programmable logic and software.

That makes it useful for proving whether Thin-Pod would genuinely benefit from:

* deterministic capture pipelines
* fixed-latency DSP stages
* hardware-assisted feature extraction
* multi-stream time alignment

As a learning and prototyping platform, that is a respectable role.

## Limits of a DE10-Nano-class path

The DE10-Nano is, however, an older platform. An older SoC FPGA can still prove an architectural point, but it should not automatically be mistaken for the right long-term production destination.

The main constraints are:

* older fabric generation
* older processor subsystem
* older toolchain expectations
* less alignment with newer edge-AI or industrial adaptive-platform ecosystems

If an FPGA path were validated experimentally, a later move to a newer SoC-FPGA or adaptive-compute platform would likely be the more realistic product direction.

## Benefits of an FPGA evolution

### Determinism

Latency, jitter, and scheduling variance are reduced when critical parts of the signal path are expressed directly in hardware.

### Parallelism

Several DSP stages can run concurrently without context switching or scheduler overhead.

### Instrument-grade behaviour

Synchronous acquisition, repeatable timing, and phase relationships are easier to guarantee.

### Pipeline cleanliness

The signal chain can be partitioned more cleanly into acquisition, conditioning, feature generation, and higher-level interpretation.

### Growth path for richer sensing

If Thin-Pod later expands toward multi-channel monitoring, tighter timing constraints, or more advanced spectral techniques, FPGA creates headroom that software-only approaches may struggle to maintain elegantly.

## Disadvantages of an FPGA evolution

### Development burden

FPGA introduces a much larger engineering surface:

* hardware description and IP integration
* clocking and timing closure
* verification complexity
* host-software integration
* toolchain complexity
* board-level design considerations

### Longer iteration cycles

Simple software changes become slower and more expensive when functionality migrates into logic.

### Higher integration risk

Power, clocks, reset sequencing, memory interfaces, and debug workflows become more demanding.

### Talent and maintenance cost

The design becomes harder to staff, maintain, and transfer than a conventional MCU or MPU system.

### Risk of solving the wrong problem

If the actual bottleneck is not deterministic signal plumbing, the FPGA effort may add complexity without delivering proportionate value.

## Recommended partition for Thin-Pod

### Sensor node

Remain MCU-first.

Use MCU firmware for:

* sensor control
* duty cycling
* local health checks
* basic thresholds
* transport handling
* power management

Avoid FPGA at the node unless the sensing ambition changes substantially.

### Gateway without FPGA

Retain the current model if the DSP load remains manageable in software.

Processor-side software handles:

* ingest
* feature extraction
* inference
* storage
* networking
* security
* updates

This remains the simplest and most maintainable baseline.

### Gateway with FPGA

Introduce FPGA only for the deterministic front end.

FPGA handles:

* synchronous ingest
* buffering
* time alignment
* DSP front-end stages
* trigger logic
* feature precomputation

Processor software handles:

* orchestration
* inference
* user-facing logic
* networking
* storage
* configuration
* OTA and lifecycle management

This is the most credible place for FPGA in the Thin-Pod roadmap.

## Overall judgement

Thin-Pod should not evolve wholesale toward FPGA. A selective gateway-side evolution is the more defensible path.

The most sensible sequence is:

1. retain the current MCU or MPU-centred architecture as the baseline
2. profile the software pipeline to identify genuine timing or throughput bottlenecks
3. prototype only the painful deterministic DSP stages on an FPGA platform such as DE10-Nano
4. evaluate whether the measured gain is large enough to justify added design complexity
5. consider a newer SoC-FPGA or adaptive platform only if the experiment clearly validates the architecture

## Conclusion

For Thin-Pod, FPGA is best viewed as a **precision instrument technology**, not as a default upgrade path. It becomes valuable when the project's identity shifts toward deterministic acquisition and hardware-defined signal processing. Until then, the cleaner architecture remains one in which the node stays MCU-first and the gateway remains software-led, with FPGA introduced only where the signal path truly demands it.

## References

1. Terasic, **DE10-Nano Kit**. Official product page.
2. Altera, **Agilex 5 FPGAs and SoC FPGAs**. Official product family information.
3. AMD, **Kria KD240 Drives Starter Kit**. Official product page.


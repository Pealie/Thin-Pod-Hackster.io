# TinyML and the ThinPod Project

**Date:** 2 May 2026  
**Project:** ThinPod vibration sensing node and gateway architecture  
**Focus:** TinyML, vibration analytics, edge inference, and practical development path

## Executive summary

TinyML fits the ThinPod project well, but its best role is not as a decorative AI layer. It should become a disciplined decision layer above a trustworthy vibration measurement chain.

The recommended direction is:

1. build ThinPod first as a reliable vibration measurement instrument;
2. extract classical vibration features on the Pod;
3. perform early anomaly detection on the Gateway;
4. use the STM32N6 Gateway for heavier TinyML inference;
5. only move small, robust inference back onto the Pod once the dataset and feature pipeline are proven.

The strongest near-term architecture remains:

> \*\*features on Pod, model on Gateway\*\*

This keeps the wireless node simple, power-aware, and testable, while giving the STM32N6 Gateway enough responsibility to justify its edge-AI capability.

## 1\. Why TinyML is relevant to ThinPod

ThinPod is a compact vibration-sensing system for rotating machinery. Its task is not merely to collect acceleration data, but to determine whether a machine is behaving normally, drifting from baseline, or showing signs of mechanical fault development.

That is a natural TinyML problem because the system has several embedded constraints:

* limited power on the sensing node;
* limited useful radio airtime over the UWB link;
* high-bandwidth vibration data from the accelerometer;
* a need for low-latency local interpretation;
* a preference for local intelligence rather than cloud dependence;
* a gateway architecture capable of more sophisticated inference.

TensorFlow Lite for Microcontrollers is designed for running machine-learning inference on microcontrollers, DSPs, and other devices with limited memory. It is inference-focused rather than training-focused, which suits embedded deployment after a model has been trained and validated elsewhere. \[1]

For ThinPod, the immediate value of TinyML is not that it can say ‘AI’. The value is **radio discipline**. Raw vibration windows can be large. Feature packets and anomaly scores can be tiny. A system that transmits only meaningful summaries or triggered raw windows will be more power-efficient and more scalable than one that streams everything continuously.

## 2\. Sensor and hardware context

The ADXL1005 is an appropriate accelerometer choice for this project because it is explicitly positioned for bearing fault detection and diagnostics. Analog Devices describes it as a low-noise, wide-bandwidth MEMS accelerometer with a typical noise density of 75 µg/√Hz across its linear frequency range. \[2]

That matters because ThinPod is not using a low-grade motion sensor for gesture recognition. It is using a serious analogue vibration sensor. The firmware, filtering, sampling, packetisation, and model pipeline should therefore preserve mechanically meaningful information rather than reducing the signal too early to a crude vibration threshold.

The DWM3001C module is also relevant architecturally. Qorvo describes the DWM3001C as a fully integrated UWB module based on the DW3110 UWB IC and including a Nordic nRF52833 BLE SoC, antenna, accelerometer, power management, and crystal. \[3]

The nRF52833 itself is capable enough for acquisition, feature extraction, and small inference. Nordic lists it as a 64 MHz Arm Cortex-M4 with FPU, 512 KB flash, 128 KB RAM, and a 12-bit ADC capable of 200 ksps. \[4]

However, that does not mean the Pod should carry the full TinyML burden at the beginning. The Pod also has to handle sampling, buffering, UWB communications, power control, diagnostics, and firmware reliability. The more intelligent first move is to use the Pod for clean sensing and feature generation, then use the Gateway for model experimentation.

## 3\. Why the Gateway is the natural first home for TinyML

The STM32N6 Gateway is far more suitable for experimental inference than the Pod. ST describes the STM32N6 series as including the Neural-ART Accelerator, with up to 600 GOPS for edge AI inference. \[5]

ST also provides the STM32N6-AI ecosystem and X-CUBE-AI tooling for development and deployment of AI models on STM32N6 devices. X-CUBE-AI is described as a tool for evaluating, optimising, and compiling edge-AI models for STM32 microcontrollers and the Neural-ART Accelerator. \[6] \[7]

This makes the Gateway the right place for:

* model comparison;
* model versioning;
* per-machine baselining;
* fault classification experiments;
* long-window inference;
* spectrogram or envelope-domain models;
* logging and visualisation;
* later integration with PC or cloud tooling.

The Gateway can receive raw windows or features from the Pod, then decide whether a vibration event is normal, suspicious, or critical.

A concise architectural statement would be:

> \*\*Pod:\*\* ‘I felt something worth reporting.’  
> \*\*Gateway:\*\* ‘This looks like a developing fault, not ordinary variation.’

## 4\. TinyML should follow classical vibration DSP

TinyML should not replace classical vibration analysis in ThinPod. It should sit above it.

The first useful layer is a DSP and feature extraction layer, including:

* RMS acceleration;
* peak and peak-to-peak acceleration;
* crest factor;
* kurtosis;
* skewness;
* zero-crossing rate;
* spectral centroid;
* dominant frequency;
* band power;
* envelope spectrum peaks;
* harmonics around shaft speed, where RPM is known.

These features are interpretable. They provide engineering evidence, not just classification outputs. Many rotating-machine faults already have a meaningful relationship to spectral content, impulses, modulation, harmonics, and envelope behaviour.

TinyML becomes useful when these features interact in ways that are difficult to threshold manually, when each machine has its own normal baseline, or when the system needs to distinguish unusual-but-harmless behaviour from mechanically significant behaviour.

Arm CMSIS-DSP is relevant for the feature-extraction layer because it provides common signal-processing functions for Cortex-M and Cortex-A processors. \[8]

Arm CMSIS-NN is relevant for later embedded neural inference because it provides efficient neural-network kernels intended to maximise performance and minimise memory footprint on Arm Cortex-M processors. \[9]

## 5\. Recommended development progression

### Stage 1: Measurement-first baseline

The first objective should be a reliable vibration measurement pipeline.

ThinPod should be able to produce:

* raw waveform plots;
* FFT plots;
* envelope-domain plots;
* RMS trend plots;
* crest factor trend plots;
* band-power trend plots;
* repeatable recordings from the same physical setup.

This establishes that the signal chain is real, clean, and meaningful.

At this stage, the project should avoid overclaiming machine learning. The milestone is not ‘classification’. The milestone is ‘trustworthy vibration evidence’.

### Stage 2: Feature packets

The Pod should next generate compact feature packets from vibration windows.

Example feature packet contents:

```text
sample\_rate\_hz
window\_length\_samples
rms\_acceleration
peak\_acceleration
crest\_factor
kurtosis
band\_power\_low
band\_power\_mid
band\_power\_high
dominant\_frequency\_hz
battery\_voltage
firmware\_version
board\_revision
sensor\_orientation
packet\_sequence\_number
```

This packet format gives the Gateway enough data to trend machine behaviour without requiring continuous raw streaming.

### Stage 3: Gateway-side anomaly detection

The first TinyML model should probably be anomaly detection, not supervised fault classification.

A practical first model could learn normal behaviour from feature vectors, then flag windows that sit far from the normal cluster. Edge Impulse documents K-means anomaly detection as a block that clusters normal data and identifies points distant from the nearest cluster centre as anomalous. \[10]

Edge Impulse also documents spectral analysis workflows for motor anomaly detection, using spectral features to generate inputs for an anomaly detection model. \[11]

For ThinPod, this suggests a strong early experiment:

* collect normal vibration data from a bench motor, fan, pump, or bearing rig;
* extract DSP features;
* train an anomaly detector on normal data;
* introduce controlled changes such as imbalance, loosened mounting, rubbing, or changed load;
* evaluate whether the anomaly score rises in a repeatable way.

The first useful result is not ‘bearing fault detected’. It is:

> ‘This vibration window no longer resembles the learned normal state.’

That is a more defensible early claim.

### Stage 4: Gateway-side classification

Supervised classification should come later, once labelled fault data exists.

Potential classes might include:

* normal;
* imbalance;
* looseness;
* rubbing;
* bearing defect;
* misalignment;
* unknown or unclassified anomaly.

This stage requires particular care. A classifier trained on weak labels will produce weak conclusions. Adjacent windows from the same recording should not be casually split into training and test sets, because that can produce misleadingly high accuracy. A stronger validation method is to split by run, day, machine, mounting condition, or physical fault configuration.

### Stage 5: Pod-side event gating

Only after the feature and anomaly pipeline is proven should the Pod gain a small model or decision gate.

A Pod-side model should be deliberately small:

* input: engineered features rather than raw waveforms;
* model type: K-means, small dense network, or tiny autoencoder;
* output: anomaly score, event flag, or ‘send raw window now’ trigger;
* deployment: quantised, deterministic, and easy to test.

This would let the Pod conserve energy by transmitting compact feature summaries during normal operation, but send raw windows when the vibration state becomes interesting.

## 6\. Proposed ThinPod operating modes

The rev 0.1 firmware should support at least three modes.

### Raw window mode

The Pod transmits ADC samples from a captured vibration window.

Purpose:

* signal-chain validation;
* dataset collection;
* FFT and envelope analysis;
* debugging analogue noise, clipping, aliasing, and mounting artefacts.

### Feature mode

The Pod transmits a compact feature vector per vibration window.

Purpose:

* low-bandwidth monitoring;
* Gateway-side anomaly detection;
* long-term trends;
* battery-efficient operation.

### Event mode

The Pod transmits only when thresholds or anomaly indicators justify it.

Purpose:

* low-duty-cycle operation;
* practical field deployment;
* raw capture around unusual events;
* reduced UWB airtime.

This three-mode design is important because it prevents TinyML from obscuring basic engineering evidence. Raw data remains available when needed.

## 7\. Hardware implications for rev 0.2

TinyML increases the importance of analogue quality. A model can accidentally learn board artefacts, regulator noise, mounting resonance, ADC clipping, aliasing, loose wiring, or inconsistent test conditions.

For rev 0.2, the ADXL1005 signal path should be treated as part of the model pipeline.

Priority areas:

* short, clean routing from ADXL1005 output to the RC network and ADC node;
* careful ground return around the analogue path;
* local decoupling close to the accelerometer;
* test points for ACC\_VOUT and ADC\_NODE;
* documented sample rates and filter cutoffs;
* avoidance of switching-regulator noise coupling into the analogue path;
* mechanically repeatable mounting.

The existing 8 kΩ / 680 pF RC network gives a cutoff around 29 kHz. That can make sense only if the sampling strategy supports the intended bandwidth. If the ADC sampling rate is much lower, aliasing becomes a serious risk.

This affects the earlier rev 0.2 placement question. From a TinyML perspective, the sensor-conditioning passives should be close to the sensor and ADC path. The switching regulator should not be moved closer to the sensor merely for physical neatness. It should be placed for good power integrity, short switching-current loops, clean grounding, and minimal coupling into ACC\_VOUT and ADC\_NODE.

## 8\. Dataset strategy

The dataset is the make-or-break part of TinyML for ThinPod.

A useful dataset should include:

* normal running at different speeds;
* normal running under different loads;
* different mounting tightnesses;
* different surfaces or fixtures;
* intentional imbalance;
* loosened mounting;
* light rubbing or contact events, if safe;
* bearing-like roughness, if safely simulated;
* non-fault disturbances such as tapping the enclosure or moving cables.

Each captured window should include metadata:

```text
date\_time
machine\_type
machine\_identifier
sample\_rate\_hz
window\_length\_samples
sensor\_orientation
mounting\_method
board\_revision
firmware\_version
supply\_voltage
load\_condition
rpm\_if\_known
fault\_label
fault\_severity
notes
```

The validation split should avoid leakage. Training and testing on adjacent windows from the same continuous run can produce unrealistically good results. A stronger approach is to train on one run and test on another, train on one mounting condition and test on another, or train on one day and test on another.

## 9\. What success should look like

A credible early ThinPod TinyML result would look like this:

1. The Pod records repeatable vibration windows.
2. The Gateway plots raw waveform, FFT, and feature trends.
3. Normal behaviour forms a stable feature-space cluster.
4. Controlled mechanical changes move feature vectors away from that baseline.
5. Anomaly score rises repeatably under abnormal conditions.
6. Raw windows can be captured on event triggers.
7. The entire pipeline is documented and reproducible.

A weaker result would be a classifier with impressive accuracy but unclear data collection, weak labels, leakage between training and test data, and no signal-chain validation. That should be avoided.

## 10\. Relevance to open hardware and Hackster-style presentation

TinyML strengthens the ThinPod story if it is presented as an engineering architecture, not just as an AI claim.

A strong project description would be:

> ThinPod is an open vibration-sensing node for rotating machinery that performs local feature extraction and edge anomaly detection. It reduces raw vibration streams to meaningful condition indicators, using a gateway architecture that enables more powerful TinyML inference without overburdening the battery node.

This gives the project a clear technical identity:

* open hardware sensing node;
* meaningful analogue front end;
* UWB backhaul;
* compact embedded feature extraction;
* STM32N6 edge-AI gateway;
* reproducible dataset and model pipeline.

For open hardware release, the TinyML work should be documented alongside the hardware:

* schematic;
* PCB layout;
* bill of materials;
* sensor signal-chain notes;
* sampling firmware;
* feature extraction code;
* training notebook;
* model export procedure;
* validation report;
* example dataset;
* Gateway inference code.

That would make ThinPod much more than a board. It would become a reproducible edge condition-monitoring platform.

## 11\. Recommended immediate next steps

The best immediate move is to make ThinPod TinyML-ready, not TinyML-heavy.

For rev 0.1:

1. implement raw window capture;
2. implement feature extraction on the Pod or Gateway;
3. define the packet format;
4. add metadata to every recording;
5. build a small bench dataset;
6. plot waveform, FFT, and feature trends;
7. train a simple anomaly detector on normal data;
8. test controlled abnormal conditions;
9. document the whole process in a bring-up and verification report.

For rev 0.2:

1. improve analogue layout around the ADXL1005 and ADC node;
2. keep sensor-conditioning passives close to the signal path;
3. manage the switching regulator placement carefully;
4. add or preserve useful test points;
5. design the board so dataset quality is not compromised by avoidable electrical artefacts.

## 12\. Verdict

TinyML is highly relevant to ThinPod, but the first win is not a neural network.

The first win is a clean measurement pipeline.

The second win is trustworthy feature extraction.

The third win is feature-based anomaly detection.

The fourth win is Gateway-side classification.

Pod-side inference comes later, once the data proves what the model should be learning.

In short:

> ThinPod should begin as a vibration instrument, mature into an edge analytics instrument, and only then become a TinyML product.

## References

\[1] TensorFlow Lite for Microcontrollers, TensorFlow GitHub repository. https://github.com/tensorflow/tflite-micro

\[2] Analog Devices, ADXL1005 datasheet and product information. https://www.analog.com/media/en/technical-documentation/data-sheets/adxl1005.pdf

\[3] Qorvo, DWM3001C product page. https://www.qorvo.com/products/p/DWM3001C

\[4] Nordic Semiconductor, nRF52833 Product Specification, key features. https://docs.nordicsemi.com/

\[5] STMicroelectronics, STM32N6 series product page. https://www.st.com/en/microcontrollers-microprocessors/stm32n6-series.html

\[6] STMicroelectronics, STM32N6-AI software ecosystem. https://www.st.com/en/development-tools/stm32n6-ai.html

\[7] STMicroelectronics, X-CUBE-AI product page. https://www.st.com/en/embedded-software/x-cube-ai.html

\[8] Arm, CMSIS-DSP software library documentation. https://arm-software.github.io/CMSIS\_5/DSP/html/index.html

\[9] Arm, CMSIS-NN software library documentation. https://arm-software.github.io/CMSIS-NN/latest/

\[10] Edge Impulse, Anomaly Detection K-means documentation. https://docs.edgeimpulse.com/studio/projects/learning-blocks/blocks/anomaly-detection-k-means

\[11] Edge Impulse, Brushless DC motor anomaly detection project. https://docs.edgeimpulse.com/projects/expert-network/brushless-dc-motor-anomaly-detection


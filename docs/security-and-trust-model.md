# Thin-Pod Security and Trust Model

## Purpose

Thin-Pod treats security as part of measurement integrity. A vibration alert is only meaningful if the sensor, firmware, calibration state, packet path, gateway processing version, model version and decision record can be traced and trusted.

This document defines the security and trust posture for the Thin-Pod platform. It is intentionally short and practical. It is a starting point for rev 0.1 and should evolve as the hardware, firmware and gateway architecture mature.

## Security posture

Thin-Pod should be described as **secure-ready** or **designed for secure lifecycle management** until secure boot, signed firmware, provisioning, authenticated updates and key handling have been implemented and tested.

The project goal is not only to protect data. It is to preserve confidence in the complete evidence chain:

```text
physical vibration
→ sensor and analogue front end
→ ADC window capture
→ packetisation and metadata
→ UWB transport
→ gateway ingest
→ DSP / ML processing
→ diagnostic statement
→ maintenance decision
```

## Core trust principles

1. **Measurement provenance matters.** Each captured vibration window should be traceable to a known pod, hardware revision, firmware version, calibration state, sample rate, timestamp and gateway processing version.
2. **Open hardware and security are compatible.** Schematics, KiCad files, firmware, BOMs, calibration procedures and datasets can be public. Signing keys, provisioning secrets, private credentials and deployment tokens must not be public.
3. **The gateway is the first practical trust anchor.** In early revisions, the gateway should verify incoming packets, track device identity, record firmware/model versions, maintain audit logs and reject untrusted data as diagnostic evidence.
4. **Security claims must be evidence-led.** Thin-Pod should not be described as secure until the relevant mechanisms are implemented, documented and tested.
5. **ML depends on trusted data.** A model trained on unknown, corrupted or poorly labelled vibration windows is not trustworthy, even if the model architecture is sophisticated.

## Threats and failure modes

The main threats and trust failures include:

* spoofed or misidentified pod data;
* replayed vibration packets;
* corrupted packet payloads;
* untracked firmware changes;
* unauthorised or downgraded firmware;
* exposed debug interfaces in deployment;
* missing calibration records;
* sensor remounting without metadata update;
* model drift or untracked model replacement;
* training data poisoning or mislabelling;
* gateway accepting poor-quality data as valid diagnostic evidence;
* false confidence caused by packet loss, clipping, timing jitter or unsupported operating regimes.

## Node-level requirements

Future Thin-Pod nodes should support, where practical:

* unique device identity;
* signed firmware images;
* anti-rollback protection;
* documented firmware release hashes;
* controlled debug access for deployment;
* calibration metadata linked to hardware revision;
* packet counters or sequence numbers;
* basic payload integrity checking;
* power and signal-quality metadata where available.

For rev 0.1, these may begin as documented requirements rather than fully implemented features.

## Gateway-level requirements

The gateway should become the main trust and evidence-management point. It should:

* verify pod identity before accepting measurement data;
* check packet integrity and sequence continuity;
* record packet loss, retransmission and timing statistics;
* track pod firmware version and hardware revision;
* track gateway firmware, DSP pipeline and ML model versions;
* preserve audit logs for diagnostic decisions;
* distinguish trusted data, degraded data and rejected data;
* prevent poor-quality captures from producing overconfident alerts.

## Telemetry trust

UWB transport should be treated as a measurable telemetry path, not as an assumed guarantee of trust. Thin-Pod should distinguish:

* packet integrity;
* device identity;
* timestamp confidence;
* replay resistance;
* ranging integrity, if used;
* confidentiality, if required;
* robustness under interference, multipath and NLOS conditions.

A valid packet is not automatically a trusted diagnostic record. Origin, timing, context and signal quality must also be considered.

## Dataset and ML provenance

Every dataset should include metadata sufficient to reproduce or challenge the result:

* pod ID;
* gateway ID;
* hardware revision;
* firmware version;
* sensor type and analogue front-end revision;
* calibration state;
* mounting method and location;
* sample rate and window length;
* packet statistics;
* operating regime or RPM, where available;
* fault or maintenance label, if known;
* DSP feature pipeline version;
* ML model version;
* train/test split definition.

Every deployed model should have a model card describing training data, intended use, known limits, latency, memory footprint, quantisation state, failure modes and confidence behaviour.

## Secure update direction

A mature Thin-Pod system should support:

* signed firmware releases;
* version checks and anti-rollback;
* safe recovery from failed updates;
* documented build provenance;
* SBOMs for firmware and gateway software;
* private key storage outside the public repository;
* a clear separation between open design artefacts and deployment secrets.

## Open-hardware boundary

Public:

* schematics;
* PCB source files;
* firmware source;
* BOM;
* mechanical drawings;
* calibration workflows;
* test procedures;
* benchmark datasets;
* documentation.

Private or controlled:

* signing keys;
* provisioning secrets;
* deployment credentials;
* private cloud tokens;
* per-site operational secrets;
* any sensitive customer or asset data.

## Current rev 0.1 status

Thin-Pod rev 0.1 should be treated as a research and bring-up platform. Rev 0.2 security task is to define the evidence chain and metadata model before hard security claims are made.

Recommended rev 0.2 actions:

1. Add hardware and firmware revision identifiers to packet metadata.
2. Add sequence numbers and CRC checks to vibration-window payloads.
3. Log packet loss and timing irregularities at the gateway.
4. Record calibration and mounting metadata with each dataset.
5. Track DSP and ML pipeline versions.
6. Add a simple diagnostic audit log.
7. Keep all signing keys and credentials out of the repository.
8. Document which security features are implemented, planned or not yet supported.

## Pitch language

Preferred:

> Thin-Pod is secure-ready open hardware for trusted vibration telemetry, edge DSP and machine-learning-based condition monitoring.

Avoid until implemented and tested:

> Thin-Pod is secure.

## Summary

Thin-Pod’s security model is not only about protecting communications. It is about making machine-health evidence trustworthy. The system should be able to answer: which device measured this vibration, under what calibration and firmware state, how it was transmitted, how it was processed, which model interpreted it, and how confident the gateway should be in the resulting diagnostic statement.


# Neoverse, Helium, and the Arm Cortex Direction

## Overview

Arm now has three fairly distinct compute lanes.

**Neoverse** is the infrastructure-class family for cloud, hyperscale, networking, AI datacentres, and high-performance edge infrastructure. It belongs to the A-profile world and is designed around many-core scale, performance per watt, virtualization, memory coherency, and accelerator-heavy systems. ([Arm Neoverse overview](https://newsroom.arm.com/blog/what-is-arm-neoverse), [Arm datacentre products](https://www.arm.com/products/cloud-datacenter))

**Cortex-A** is the application-processor line for rich operating systems and more capable embedded or edge platforms. It also belongs to the A-profile world, but is aimed more at client devices, embedded Linux systems, and increasingly AI-capable edge compute. ([Cortex-A320 introduction](https://newsroom.arm.com/blog/introducing-arm-cortex-a320-cpu))

**Cortex-M** is the microcontroller-class family for low-power embedded systems, control, sensing, DSP, and small-scale machine learning. It belongs to the M-profile world and is much closer to bare-metal and RTOS development. ([Arm Helium](https://www.arm.com/technologies/helium))

## What Neoverse is

Neoverse is best understood as Arm’s server and infrastructure CPU strategy. It is no longer just a catalogue of cores. Arm presents it as a broader platform for cloud and AI infrastructure, including cores, interconnect, system IP, software support, and compute subsystem products intended to shorten silicon development cycles. Public Arm material highlights Neoverse V3, N3, N2, and Neoverse CSS offerings for cloud and datacentre deployment. ([Arm Neoverse overview](https://newsroom.arm.com/blog/what-is-arm-neoverse), [Arm Neoverse CSS](https://www.arm.com/products/cloud-datacenter/neoverse-compute-subsystems))

Architecturally, Neoverse sits in the Armv9-A direction rather than the microcontroller domain. The design priorities are high sustained throughput, power-efficient scaling across many cores, support for Linux and virtualization, coherent memory systems, and close coupling to accelerators used in AI and networking. That makes it the natural Arm line for hyperscale servers, AI host CPUs, smart NICs, telecom infrastructure, and other large-scale compute deployments. ([Arm Neoverse CSS](https://www.arm.com/products/cloud-datacenter/neoverse-compute-subsystems))

## How Helium relates to Neoverse

Helium is **not** a Neoverse technology.

Helium is Arm’s name for the **M-Profile Vector Extension**, or **MVE**, and it is associated with the **Cortex-M** family. Arm describes it as a vector extension added to Armv8.1-M to accelerate DSP and machine-learning workloads on low-power microcontrollers. ([Arm Helium](https://www.arm.com/technologies/helium))

The relationship between Neoverse and Helium is therefore only indirect. Both are part of Arm’s wider response to increasing demand for AI and data-parallel computation, but they live in different processor profiles and solve different classes of problem.

* **Helium** serves microcontrollers doing compact local DSP and ML.
* **Neoverse** serves infrastructure CPUs running cloud, network, or AI-heavy platforms.
* The closer A-profile analogues to Helium are technologies such as **Neon**, **SVE**, and **SME**, not Helium itself. ([Arm Helium](https://www.arm.com/technologies/helium))

So the clean answer is that Neoverse does not 'use Helium'. Helium belongs to Cortex-M. Neoverse belongs to the A-profile infrastructure family.

## Compact comparison table

|Line / term|What it is|Target domain|ISA/profile|Main performance feature|Typical software stack|Relationship to Helium|
|-|-|-|-|-|-|-|
|**Neoverse**|Arm’s infrastructure CPU family and platform|Cloud, hyperscale, AI datacentres, networking, 5G, infrastructure edge|**A-profile**, now centred on Armv9-A|High core counts, performance per watt, virtualization, large caches, coherent interconnect, accelerator attachment|Linux, hypervisors, cloud-native stacks, datacentre firmware|Not Helium; separate architectural family ([Arm Neoverse overview](https://newsroom.arm.com/blog/what-is-arm-neoverse))|
|**Cortex-A**|Application processors for rich OS devices and embedded Linux systems|Phones, tablets, consumer devices, some edge and industrial Linux systems|**A-profile**|64-bit application compute, stronger security and AI support in newer generations|Linux, Android, hybrid RTOS-rich OS stacks|Uses A-profile SIMD and vector technologies, not Helium ([Cortex-A320 introduction](https://newsroom.arm.com/blog/introducing-arm-cortex-a320-cpu))|
|**Cortex-M**|Microcontroller-class processors|MCUs, low-power embedded, control, sensing, DSP, tinyML|**M-profile**|Deterministic low-power embedded compute, optional DSP and ML acceleration|Bare metal, RTOS, lightweight embedded stacks|Helium belongs here ([Arm Helium](https://www.arm.com/technologies/helium))|
|**Helium / MVE**|M-Profile Vector Extension|DSP and ML on microcontrollers|Extension to **Armv8.1-M**|Vector-style acceleration for compact DSP and ML workloads|CMSIS, embedded DSP and ML libraries, bare metal or RTOS|This is the Cortex-M vector extension, not a Neoverse feature ([Arm Helium](https://www.arm.com/technologies/helium))|
|**Neon / SVE / SME**|A-profile SIMD, vector, and matrix technologies|Cortex-A, Neoverse, infrastructure and higher-end application compute|**A-profile** extensions|Wider data-parallel and matrix-oriented compute for richer systems|Linux, Android, server software stacks|These are closer A-profile analogues to Helium, but they are distinct technologies ([Arm Helium](https://www.arm.com/technologies/helium))|

## Is there a roadmap for Arm Cortex microprocessors?

There is a public direction, but not a fully disclosed long-range roadmap with dates for every future core.

For **Cortex-M**, the public product direction is reasonably clear. Arm’s current family includes **Cortex-M55**, **Cortex-M85**, and **Cortex-M52** as Helium-capable parts. That suggests a deliberate spread of Helium across performance tiers, from smaller embedded implementations up to the highest-performance Cortex-M line. ([Arm CPU products](https://www.arm.com/products/silicon-ip-cpu), [Cortex-M52 release](https://newsroom.arm.com/news/arm-cortex-m52))

For **Cortex-A at the embedded and edge end**, the public signal is **Cortex-A320**, introduced in February 2025 as Arm’s first ultra-efficient Armv9 Cortex-A core for IoT and edge AI. Arm pairs it with the Ethos-U85 NPU as an Armv9 edge-AI platform, which implies a continued move toward more capable and more secure application-class processors at the edge. ([Cortex-A320 introduction](https://newsroom.arm.com/blog/introducing-arm-cortex-a320-cpu))

The larger pattern is that Arm’s public roadmap is less about publishing a simple year-by-year list of future Cortex cores and more about signalling product direction:

1. **Cortex-M** is becoming more AI-capable through Helium and related embedded-DSP support.
2. **Cortex-A** is becoming more edge-AI-oriented through Armv9 features and closer pairing with NPUs.
3. **Neoverse** is increasingly its own infrastructure platform, rather than simply another branch of the broader Cortex story. ([Arm CPU products](https://www.arm.com/products/silicon-ip-cpu), [Arm datacentre products](https://www.arm.com/products/cloud-datacenter))

## Practical synthesis

At a high level, Arm’s strategy now separates cleanly by scale.

* **Cortex-M plus Helium** addresses small, real-time, power-constrained embedded intelligence.
* **Cortex-A** addresses richer edge and client systems that need operating-system-class compute and increasing AI capability.
* **Neoverse** addresses servers, cloud, networking, and AI infrastructure.

That means Neoverse and Helium are related only in the broad strategic sense that both support Arm’s wider move toward AI-era computing. Technically, they belong to different processor families, instruction-set profiles, and deployment scales.


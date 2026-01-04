### 5 V and GND pin numbers (from UM3417)

**ARDUINO Power header (CN5)**

* **+5 V:** CN5 **pin 5** 
* **GND:** CN5 **pin 6** and **pin 7** 

**ST morpho headers (CN3 and CN15)**

* **+5 V:** CN3 **pin 6** and **pin 18** 

* **GND:** CN3 **pin 8**, **pin 19**, **pin 20**, **pin 22** 

* **+5 V (STLINK rail):** CN15 **pin 8 = 5V_STLK** 

* **GND:** CN15 **pin 9** and **pin 20** 

* **Analogue ground:** CN15 **pin 32 = AGND** 

**CN15 vs CN5**

* **CN5** is the ARDUINO power header and is the cleanest place to pick up **board +5 V** and **GND** for add-on regulators. 
* **CN15** also exposes power pins, but note **5V_STLK** is specifically the STLINK-sourced 5 V rail and is called out separately in the pin naming. 

---

### Engineering Log, high-level summary

* UWB bring-up remained in a ‘known-good’ state, with stable ranging output between the two DWM3001C-CDKs.
* ADXL1005 analogue output sanity-checked at approximately mid-rail with DC measurement, with tapping producing only very small, fast excursions at the node as currently probed.
* FNIRSI scope configuration progressed far enough to show transient activity during tapping, highlighting a measurement setup issue rather than a flat sensor rail.
* Gateway wiring plan tightened using official NUCLEO-N657X0-Q connector tables: confirmed **CN3 pin 17 = PC0** mapping, and enumerated reliable **+5 V** and **GND** pickup points on **CN5** and ST morpho headers for powering external buck modules cleanly.

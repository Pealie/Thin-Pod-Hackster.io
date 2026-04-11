# Thinpod USB-C Power-Entry ASCII Circuit Diagram

This document contains the ASCII circuit diagram for the **USB-C power-entry section of Thinpod**, with **reverse-voltage protection** added ahead of the **S7V8F3**.

## ASCII circuit diagram

```text
                              THINPOD USB-C POWER ENTRY

        USB-C RECEPTACLE
        'sink only, power only'
        -------------------------
        VBUS (A4,A9,B4,B9) --------------------o----\[ F1 ]----|>|----o-------> RAW\_PROT -----> U1 VIN
                                               |              D1            |
                                               |                            |
                                               |                           \[TP1]
                                               |                            |
                                              \[TVS]                         |
                                               |                            |
        GND  (A1,A12,B1,B12) ------------------o----------------------------o------------------> GND
                                               |
                                              \[TP3]
                                               |

        CC1 (A5) ----------------------------\[ R1 5k1 ]----------------------+
                                                                              |
        CC2 (B5) ----------------------------\[ R2 5k1 ]----------------------+
                                                                              |
                                                                             GND

        D+   (A6,B6)   NC
        D-   (A7,B7)   NC
        SBU1 (A8)      NC
        SBU2 (B8)      NC
        SHIELD         chassis / shield policy, or GND by EMC policy


                              REGULATOR STAGE
                              ----------------

        RAW\_PROT -----------------------------------------------> U1 VIN
        GND ----------------------------------------------------> U1 GND
        RAW\_PROT -----------------------------------------------> U1 SHDN    'always on'
        U1 VOUT ------------------------------------------------> 3v3+       -----> \[TP2]


                              S7V8F3
                         +----------------+
        RAW\_PROT ------->| VIN        VOUT|-------> 3v3+
        GND ------------>| GND        SHDN|<------- RAW\_PROT
                         +----------------+
```

## Net summary

```text
USB-C VBUS  -> VBUS\_IN -> F1 -> D1 -> RAW\_PROT -> S7V8F3 VIN
USB-C GND   -> GND
CC1         -> R1 5k1 -> GND
CC2         -> R2 5k1 -> GND
TVS         -> from VBUS\_IN to GND
S7V8F3 SHDN -> RAW\_PROT
S7V8F3 VOUT -> 3v3+
```

## Notes

* USB-C plug reversal does **not** reverse the polarity of VBUS and GND.
* `D1` is the reverse-voltage protection diode ahead of the regulator input.
* `F1` is the input fuse.
* `TVS` is the transient suppression diode from input rail to ground.
* `R1` and `R2` are the USB-C sink pull-down resistors on `CC1` and `CC2`.
* `RAW\_PROT` is the protected input rail after the fuse and diode.


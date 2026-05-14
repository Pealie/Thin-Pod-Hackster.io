# 72 Engineering Log.md

**Date:** 12 May 2026  
**Project:** Thin-Pod  
**Board:** Thin-Pod rev 0.1  
**Status:** PCB received, initial power-path investigation underway

## High-level overview

This evening’s work focused on the first practical power investigation after the arrival of the Thin-Pod rev 0.1 PCB. The board had already passed an initial visual inspection, so attention moved to whether the DWM3001C-CDK could be powered through the Thin-Pod carrier board and associated supply path.

A bench supply was connected to the Thin-Pod PCB input at J1. Power was confirmed at the PCB input connector, with approximately 3.34 V measured between J1 positive and negative. This confirmed that the external supply connection to the Thin-Pod PCB was present and correctly applied.

Further checks indicated that the Thin-Pod board itself was producing the expected 3.3 V rail at TP4, and that voltage was also present at the intended CDK power point. This was an important early result: it suggested that the Thin-Pod PCB power-entry and regulated 3.3 V path were not obviously faulty.

The DWM3001C-CDK, however, did not power up when mounted on the Thin-Pod PCB. The CDK was then removed from the Thin-Pod board and tested separately. When powered from its micro-USB connectors, the CDK powered normally from a laptop. When powered directly from the bench supply at 3.3 V across the CDK’s own J1 positive and negative external-power input, no LED indication was observed.

At this stage, the fault could have been interpreted in several ways: a possible misunderstanding of the CDK external-power input, a CDK-side power-path issue, poor contact at the J1 input pads, or a damaged/altered input-power route on that particular CDK.

To resolve this, a spare new DWM3001C-CDK was tested under the same direct bench-supply conditions. The bench supply was connected across the spare CDK’s J1 positive and negative input at 3.3 V. The red LED illuminated. This confirmed that the DWM3001C-CDK can accept 3.3 V through its own J1 external-power input, and that the bench-supply method, polarity and voltage were valid.

## Main finding

The key result from this evening is that the original CDK appears not to accept power through its J1 external-power input, while a spare new CDK does accept power through the same route.

This strongly suggests that the earlier failure to power the CDK from the Thin-Pod PCB is not primarily a Thin-Pod PCB fault. The evidence now points towards a CDK-side issue with the original board, most likely involving its J1 external-power path, contact condition, protection circuitry, solder/jumper configuration, or local input-power circuitry.

The Thin-Pod PCB power route has not yet been fully cleared at system level, but the most important early distinction has been made: the board is generating 3.3 V as expected, and a known-good CDK has now demonstrated that 3.3 V at CDK J1 is a valid power condition.

## Evidence chain

```text
Thin-Pod PCB received and visually inspected             PASS
3.34 V measured at Thin-Pod J1 input                     PASS
3.3 V rail present at TP4                                PASS
3.3 V present at intended CDK power point                PASS
Original CDK powers from USB when removed from PCB       PASS
Original CDK powered directly at CDK J1 with 3.3 V       FAIL, no LED
Spare new CDK powered directly at CDK J1 with 3.3 V      PASS, red LED illuminated
```

## Interpretation

The evening’s work shifted the diagnosis away from a broad PCB fault and towards a much narrower CDK power-input issue.

The original CDK is not necessarily dead, because it still powers from USB. More likely, the USB power path and J1 external-power path behave differently, and the external-power path on the original CDK may be open, damaged, unlinked, misconfigured or otherwise unavailable.

The spare CDK result is especially useful because it acts as a control test. It confirms that the direct J1 power test is meaningful and that 3.3 V is sufficient to produce a visible power indication on a working CDK.

## Significance

This was a valuable first bring-up session because it produced a clear diagnostic separation.

Instead of assuming that the Thin-Pod PCB had failed because the CDK did not power on, the investigation followed the power path, checked the relevant rails, isolated the CDK from the carrier board, and compared behaviour against a spare known-good CDK. That made the result much stronger.

This is exactly the sort of evidence that the rev 0.1 bring-up process is intended to capture: not only whether something works, but where the failure boundary appears to be.

The Thin-Pod PCB remains credible at this stage. The power rail appears to exist where expected, and the unresolved issue now concerns whether the carrier-board route will correctly power a known-good CDK when tested tomorrow.

## Next steps

The next test is to mount the spare working DWM3001C-CDK on the Thin-Pod PCB and power the Thin-Pod board from J1 using the bench supply.

The expected result is that the spare CDK red power LED should illuminate if the Thin-Pod carrier power path is correctly reaching the CDK’s usable external-power input route.

During that test, the following should be recorded:

```text
Bench supply voltage
Bench supply current limit
Actual current draw
Voltage at Thin-Pod J1
Voltage at TP4
Voltage at the CDK power input
Whether the spare CDK red LED illuminates
Any voltage sag or current limiting
Any abnormal heating
```

If the spare CDK powers successfully from the Thin-Pod PCB, then Thin-Pod rev 0.1 will have passed a significant early system-level power-path test.

If the spare CDK does not power when mounted on Thin-Pod, despite powering directly from its own J1, the remaining issue will be the exact carrier-to-CDK power routing, contact alignment or connector/interface mapping.

## Closing note

Tonight’s work did not complete the power bring-up, but it produced an important diagnostic result. The Thin-Pod PCB is not currently implicated as the primary fault. The original CDK appears to have a problem on its external J1 power-input path, while the spare CDK confirms that 3.3 V external powering through J1 is valid.

The next session can now proceed with a known-good CDK and a much clearer test hypothesis.


# BSPD Onboarding Research — Gavin Barron

Oct 12, 2026 · @Gavin Barron (transfered to markdown / uploaded)

## Overview

The BSPD (Brake System Plausibility Device) cuts power when the driver brakes hard while the motor still draws more than 5 kW for over 0.5 s. That combination means the car is fighting itself, for example from a stuck throttle or a software fault.

Because software could be the cause, the BSPD must be standalone, nonprogrammable analog hardware. It backs up the VCU's software checks by watching the actual battery current, not the pedal position.

## Rule requirements

The BSPD must open the shutdown circuit on (hard braking AND >5 kW for over 0.5 s) OR an open or short on either sensor input (brake pressure sensor or HASS 300-S current sensor). Summarized from the 2027 FSAE rules draft (v0.0, July 2026).

| Rule | Requirement (paraphrased) | What it means for the design |
| --- | --- | --- |
| EV.8.7.1 | Standalone, nonprogrammable circuit; in addition to the VCU's APPS/brake check (EV.4.7) | Analog comparators and logic gates only, no microcontroller |
| EV.8.7.2 | Open the shutdown circuit when hard braking and 5 kW at nominal battery voltage coexist for more than 0.5 s | AND of two comparators, then a 0.5 s timer |
| EV.8.7.3 | Open the shutdown circuit on an open or short in any sensor input | Window comparator + pull-down on both inputs: brake pressure sensor and HASS 300-S current sensor |
| EV.8.7.4–6 | Inspection test: inject current via a separate wire from an external supply while pressing the brake; CS+/CS− 4 mm shrouded banana jacks, not red or black | Test wire/coil through the HASS 300-S current sensor; jacks on the car or a breakout harness |
| EV.8.2.3 | After a BMS, IMD or BSPD trip, stay disabled until a person at the car resets it; the driver cannot reset it | A latch is needed somewhere|
| T.4.3.4 | A brake pressure sensor (BSE) signal outside its normal range (e.g. <0.5 V or >4.5 V) is a failure; pull-up/down resistors must make open circuits detectable | Method for meeting EV.8.7.3; the same method is applied to the HASS 300-S current sensor |

EV.4.6 ("hard braking") only points to the brake pressure sensor (BSE) section and gives no number, so the threshold is a design choice.

## Key design numbers

Hard braking is 350 psi (1.2 V) and the 5 kW point is about 46 A; the current comparator trips early at about 40 A for margin.

| Quantity | Value | Source |
| --- | --- | --- |
| Hard-braking pressure | 350 psi | Luca |
| Brake threshold voltage | 0.5 + 0.002 × 350 = 1.2 V | Brake pressure sensor transfer function |
| Battery pack | 30s (5 modules × 6 cells) | FS-4 Grafana, confirmed by Luca |
| Nominal pack voltage | 30 × 3.6 V ≈ 108 V (≈114 V full, ≈96 V under load) | Calculated |
| 5 kW current | 5000 W ÷ 108 V ≈ 46 A | Calculated |
| Design trip current | ≈40 A (covers ±7A HASS 300-S current sensor offset) | Design choice |
| HASS 300-S current sensor output at 40 A | U_ref + ≈83 mV | HASS 300-S transfer function |
| Diff amp output at 40 A (gain ≈10) | ≈0.83 V | Calculated; gain is a placeholder |
| Peak pack current seen | ≈570 A (≈62 kW) | FS-4 Grafana, warm-up run Oct 4, 2026 |
| Timer | ≈0.4–0.45 s, so total with filters lands at or under 0.5 s | Design choice |

## Calculations

Every number in this doc comes from one of the derivations below.

### 1. Brake pressure sensor transfer function

Given (AiM datasheet + Summit range): 0 psi gives 0.5 V, 2000 psi gives 4.5 V, linear within ±0.5% of full scale.

- Slope = (4.5 V − 0.5 V) ÷ (2000 psi − 0 psi) = 4.0 V ÷ 2000 psi = 0.002 V/psi (2 mV/psi)
- Line through (0 psi, 0.5 V): V = 0.5 V + 0.002 V/psi × P
- Check at full scale: 0.5 + 0.002 × 2000 = 0.5 + 4.0 = 4.5 V
- Accuracy: 0.5% × 2000 psi = ±10 psi; ±10 psi × 2 mV/psi = ±20 mV

### 2. Hard-braking threshold voltage

- V = 0.5 + 0.002 × 350 = 0.5 + 0.7 = 1.2 V

### 3. Pack voltage

- Cells in series = 5 modules × 6 cells = 30
- Nominal: 30 × 3.6 V = 108 V
- Full charge: 30 × 3.8 V = 114 V
- Sag under load: 30 × 3.2 V = 96 V

### 4. 5 kW threshold current

- P = V × I, so I = P ÷ V
- I = 5000 W ÷ 108 V = 46.3 A (rounded to 46 A)

### 5. Peak power check

- P = 108 V × 570 A = 61,560 W ≈ 62 kW, a realistic FSAE power level, which supports the 30s pack count

### 6. HASS 300-S sensitivity

Given (LEM datasheet): U\_out = U\_ref + 0.625 V × I ÷ I\_PN, with I\_PN = 300 A.

- Sensitivity = 0.625 V ÷ 300 A = 0.002083 V/A ≈ 2.08 mV/A
- So U\_out = 2.5 V + 0.002083 V/A × I

### 7. HASS output range and fault window

- At +900 A: 2.5 + 0.002083 × 900 = 2.5 + 1.875 = 4.375 V
- At −900 A: 2.5 − 1.875 = 0.625 V
- At ±300 A (nominal): 2.5 ± 0.625 = 1.875 V to 3.125 V
- At 570 A (measured peak): 2.5 + 0.002083 × 570 = 2.5 + 1.19 = 3.69 V
- 3.69 V is outside the nominal range but inside 0.625–4.375 V, so the HASS 300-S fault window is based on ±900 A, widened to ≈0.5–4.5 V for margin

### 8. HASS 300-S current sensor offset in amps

- Offset in amps = ±15 mV ÷ 2.083 mV/A = ±7.2 A (≈ ±7 A)
- Share of the threshold: 7.2 A ÷ 46.3 A = 0.156 ≈ 15%

### 9. HASS 300-S current sensor output at the current thresholds

- At 46.3 A: 0.002083 × 46.3 = 0.0965 V ≈ 96 mV above U\_ref
- At 40 A: 0.002083 × 40 = 0.0833 V ≈ 83 mV above U\_ref

### 10. Difference amplifier output (gain G = 10, placeholder)

- V\_amp = (U\_out − U\_ref) × G
- At 0 A: (U\_ref − U\_ref) × 10 = 0 V
- At 40 A: 0.0833 V × 10 = 0.833 V ≈ 0.83 V (the comparator reference)
- At 46.3 A: 0.0965 V × 10 = 0.965 V ≈ 0.96 V
- At 570 A: 1.19 V × 10 = 11.9 V, above the op-amp supply, so the output saturates high
- Offset after the amp: ±15 mV × 10 = ±150 mV; ±150 mV ÷ (2.083 mV/A × 10) = ±7.2 A, the same error in amps

### 11. Pull-down resistors in parallel

- R\_total = (R1 × R2) ÷ (R1 + R2)
- Two 10 kΩ (BMS + BSPD): (10 kΩ × 10 kΩ) ÷ 20 kΩ = 5 kΩ
- 5 kΩ is above the datasheet's 2 kΩ minimum load, so sharing the HASS 300-S Output pin between the BMS and the BSPD is acceptable

## Brake pressure sensor

The AiM 0–2000 psi sensor (Summit AAG-S0050, AiM X05PSA02000P18) outputs 0.5–4.5 V linearly: V = 0.5 + 0.002 × psi.

| Parameter | Value |
| --- | --- |
| Pressure range | 0–2000 psi |
| Output | 500 mV at 0 psi, 4500 mV at 2000 psi |
| Sensitivity | 2 mV/psi |
| Accuracy | < ±0.5% FS, non-linearity + hysteresis (≈ ±20 mV, ±10 psi) |
| Supply | 8–16 V (runs from the 12 V LV system) |
| Consumption | < 10 mA |
| Pinout | 1 signal (white), 2 GND (black), 3 +Vb (red), 4 not connected |

- The transfer function is derived from the datasheet's start/end-of-scale values and Summit's 0–2000 psi range; the accuracy spec supports treating it as linear.
- The brake pressure sensor's ground (black wire) and the BSPD ground must be the same reference.

## Current sensor (LEM HASS 300-S)

The car uses a HASS 300-S (found in the FS-4 BMS firmware), giving about 2.08 mV per amp around a 2.5 V zero: U_out = U_ref + 0.625 × I ÷ 300.

| Parameter | Symbol | Value |
| --- | --- | --- |
| Supply | U_C | 5 V ±5% (absolute max < 6.5 V) |
| Consumption | I_C | 19 mA typical, 25 mA max |
| Nominal current | I_PN | 300 A |
| Measuring range | I_PM | ±900 A (output ≈0.625–4.375 V) |
| Reference | U_ref | 2.5 V ± 25 mV |
| Output at 0 A | U_OE | U_ref ± 15 mV |
| Sensitivity | S_Th | 0.625 V per I_PN ≈ 2.08 mV/A |
| Accuracy | ε | ≤ ±1% at I_PN; linearity ≤ ±1% over full range |
| Noise | U_no | < 40 mVpp |
| Load | R_L | ≥ 2 kΩ; datasheet values assume 10 kΩ Output to 0 V |
| Pinout |  | 1 U_ref, 2 Output, 3 0 V, 4 +5 V; positive current = arrow direction |

**Firmware findings** (fs-4-firmware, tractive-battery/BMS.cpp):

- The BMS reads the HASS 300-S through an instrumentation amplifier: gain 1.69, zero point ≈0.2 V. Old commented values.
- It reads discharge ("Output") and charge ("Input") as separate signals.
- In the Grafana bus-current data, discharge reads positive and current never went negative in the warm-up run.

## Circuit architecture

Six comparators feed one OR gate: faults trip immediately, while hard braking plus high current must last 0.5 s.

Fault checks read the raw brake pressure sensor and HASS 300-S signals; only the high-current comparator reads the amplified HASS 300-S signal, which saturates at high current.

| Comparator | Input | Reference | HIGH means |
| --- | --- | --- | --- |
| Hard braking | Filtered brake pressure sensor signal | 1.2 V (350 psi) | Braking hard |
| Brake too low | Filtered brake pressure sensor signal | 0.4 V | Open or short to ground |
| Brake too high | Filtered brake pressure sensor signal | 4.6 V | Short to power |
| High current | Filtered diff amp output (from HASS 300-S) | ≈0.83 V (≈40 A) | Above 5 kW, with margin |
| Current too low | Filtered raw HASS 300-S output | ≈0.5 V | Open or short to ground |
| Current too high | Filtered raw HASS 300-S output | ≈4.5 V | Short to power |

All references come from voltage dividers; every comparator gets a small hysteresis resistor.

## Current-path error budget

The ≈83 mV signal at 40 A is detectable: subtracting U_ref and filtering removes most error, and the remaining ±15 mV HASS 300-S offset is covered by tripping at 40 A instead of 46 A.

| Error | Size | Behavior | Fix |
| --- | --- | --- | --- |
| Reference shift | ±25 mV | Output and U_ref move together | Subtract U_ref (diff amp) |
| Offset | ±15 mV ≈ ±7 A | Constant mismatch between Output and U_ref | Margin: trip at ≈40 A |
| HASS 300-S noise | < 40 mVpp | Fast, random, averages to zero | RC low-pass filter |


## Design decisions

Each decision below is a working choice that can change once the open questions are answered.

| Decision | Rationale |
| --- | --- |
| Output BSPD_OK: high = OK, 0 V = fault | Fail-safe: a broken wire or dead board reads as a fault. A relay driver or latch can be added after it without changing anything upstream. |
| Tap the raw HASS signal (Output + U\_ref) and build my own diff amp | Keeps the BSPD standalone from the BMS amplifier.
| Fault checks read the raw HASS signal; only CURRENT_HIGH reads the amplified one | The ×10 signal saturates near ≈570 A peaks and would look like a short-to-power fault. The raw signal stays ≈0.6–4.4 V across ±900 A. |
| Current fault window ≈0.5–4.5 V | Based on I_PM (±900 A → 0.625–4.375 V) plus margin, since real peaks (≈570 A) exceed I_PN (300 A). |
| Brake fault window 0.4–4.6 V | Just outside the brake pressure sensor's 0.5–4.5 V range, per T.4.3.4. |
| Trip current ≈40 A, not 46 A | Tripping early keeps the BSPD compliant on a worst-case HASS 300-S unit |
| RC filter on each input, plus hysteresis on every comparator | The filter removes fast noise; hysteresis prevents chatter as a slow, filtered signal crosses the threshold. |
| Separate 0.5 s timer after the AND, diode to reset it | Measures "both conditions together, continuously" as the rule states
| Diff amp: one op-amp with large resistors, or an instrumentation amp | The HASS 300-S Output pin may be shared with the BMS, so loading must keep R_L ≥ 2 kΩ. Two 10 kΩ pull-downs (BMS + BSPD) in parallel make 5 kΩ, still above 2 kΩ. |

## Open questions

Two answers are in (350 psi hard braking, 30s/108 V pack); these remain.

- [ ] Is there a shared latch for BMS/IMD/BSPD faults? Should the BSPD output a signal (0 V = fault) or drive a relay in the shutdown loop?
**seperate**
- [ ] Can the BSPD tap the raw HASS 300-S signal (Output + U_ref) with its own diff amp, powered entirely from the BMS side? Does that count as "standalone" or do we need another current sensor?
**Doesn't Matter**
- [ ] Which way is the HASS installed (discharge positive)?
**positive**
- [ ] What LV supply is available for the BSPD board (12 V)?
**12V**


## Sources

- [FSAE 2027 Rules draft](https://fsaeonline.com/cdsweb/gen/DownloadDocument.aspx?DocumentID=9574fa97-c13e-4b90-9c75-044e30348cab) (EV.8.7, EV.8.2, EV.4.6–4.7, T.4.3)
- [AiM car/bike pressure sensors datasheet](https://aim-sportline.com/download/doc/eng/sensors/pressure/Datasheet_CarBike_PressureSensors_eng.pdf)
- [Summit Racing AAG-S0050](https://www.summitracing.com/parts/aag-s0050) (0–2000 psi range)
- [LEM HASS 50…600-S datasheet](https://www.lem.com/sites/default/files/products_datasheets/hass-50_600-s-v22.pdf)
- fs-4-firmware, tractive-battery/BMS.cpp (HASS 300-S constants)
- FS-4 Grafana: warm-up run Oct 4, 2026 (bus current, cell voltages)
- [LV onboarding README](https://github.com/formulaslug/fs-electrical-onboarding/blob/main/lv/README.md)

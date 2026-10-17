# BSPD — Brake System Plausibility Device

**Author:** Isaac Hashimi
**Project:** Formula Slug LV onboarding
**Status:** Schematic complete (all functional blocks). Connectors and final ERC cleanup remaining.

## What this circuit does

The BSPD is a standalone, non-programmable safety circuit. It opens the shutdown circuit (SDC) if the driver is braking hard **and** the motor is drawing high power at the same time for longer than a short delay. It also trips if either sensor signal is lost or out of range, or if the BSPD itself loses power. Once tripped, it stays tripped until power is cycled.

Signal path:

```
Brake sensor  ─► Brake comparator ───┐
                                     ├─► Wired-AND ─► ~400 ms delay ─► Latch ─► Fail-safe relay ─► SDC
Current sensor ─► Current comparator ┘                                   ▲
                                                                         │
Both sensors ─► Window comparators (0.3–4.7 V) ─► Sensor fault ──────────┘
```

## Assumptions (to verify)

I didn't have the team's exact sensor part numbers or the final 2027 rule values while designing, so I used typical values. Every threshold below can be changed by swapping one or two resistor values.

| Parameter | Assumed value | Source to verify |
|---|---|---|
| Supply rail | 5 V | Team LV design (likely regulated from ~12 V GLV) |
| Brake sensor output | 0.5–4.5 V over 0–100 bar | Brake pressure sensor datasheet |
| Hard-braking threshold | 30 bar | FSAE 2027 rules, EV8.7 |
| Power threshold | 5 kW | FSAE 2027 rules, EV8.7 |
| Max pack voltage | 600 V | Accumulator spec |
| Current sensor output | 2.5 V at 0 A, 26.7 mV/A | Current sensor datasheet |
| Trip delay | must trip within 500 ms | FSAE 2027 rules, EV8.7 |

## Blocks

### 1. Brake comparator (U1A)

Brake pressure goes to the + input. A divider (R1, R2) sets the threshold on the − input.

- Sensor voltage at 30 bar: V = 0.5 + 4 × (30/100) = **1.70 V**
- Divider: V_ref = 5 × R2 / (R1 + R2). With R2 = 10k, R1 = 19.4k → nearest standard value **19.6k** → V_ref = **1.69 V**

### 2. Current comparator (U1B)

Same structure, using R4 and R5.

- Current at 5 kW: I = 5000 W / 600 V = **8.33 A**. I used the *max* pack voltage because higher voltage means less current for the same power, so this is the lowest current that can still represent 5 kW.
- Sensor voltage: 2.5 + 8.33 × 0.0267 = **2.722 V**
- Divider: R5 = 10k, R4 = 8.37k ideal.
- **Rounding direction matters.** 8.45k gives V_ref = 2.71 V, which trips at about 7.9 A (≈4.7 kW), so it's compliant. 8.2k would trip at about 9.3 A (≈5.6 kW), which is over the limit. I chose **R4 = 8.45k**.

### 3. Wired-AND

The LMV393 has open-collector outputs: each output can only pull *down* to ground or let go. Tying U1A and U1B's outputs together with one pull-up (R6, 10k) makes a free AND gate. The line only goes high when **both** comparators let go, meaning hard braking **and** high current. If either one is still pulling low, the line stays low. Net: `Plausibility_Fault`.

### 4. Delay (~400 ms)

`Plausibility_Fault` charges C2 through R3. U2A compares the capacitor voltage against 2.5 V (R7/R8 divider, net `Ref_2V5`).

- Time to reach half the supply: t = RC × ln(2) ≈ 0.693 × RC
- I targeted **~400 ms**, not 500 ms, because ceramic capacitors can be ±10% and the rules require tripping *within* 500 ms.
- RC = 0.4 / 0.693 ≈ 0.577 s. With C2 = 1 µF, total R ≈ 577k.
- Charging current also passes through the pull-up R6 (10k), so R3 = 567k → standard value **562k**.
- Check: (562k + 10k) × 1 µF × 0.693 ≈ **396 ms**. At +10% capacitance it's ≈436 ms, still under 500 ms.
- If the fault clears early, the comparators pull `Plausibility_Fault` low and C2 discharges back through R3, so the timer resets on its own.

### 5. Latch (U2B)

Once tripped, the BSPD must stay tripped until power is cycled.

- At power-up, R10 (100k) pulls U2B's + input to 0 V, below the 2.5 V reference, so the output starts low.
- When `Delayed_Fault` goes high, it pushes the latch node up through D1. The output goes high (`BSPD_Trip`, pulled up by R11).
- R12 (10k) feeds the output back into the + input and holds it at ≈5 × 100k / 120k = **4.2 V**, positive feedback.
- When the fault clears, D1 blocks `Delayed_Fault` from pulling the latch node back down, so the latch holds.
- The only reset is removing power.

### 6. Fail-safe output stage

The relay must hold the SDC **closed** only while everything is OK *and* powered, so that losing BSPD power also opens the SDC.

| State | BSPD_Trip | Q1 | Q2 | Relay K1 | SDC |
|---|---|---|---|---|---|
| Normal | Low | Off | On (gate pulled up by R13) | Energized | Closed |
| Tripped | High | On | Off (gate pulled low) | Released | **Open** |
| BSPD loses power | — | — | Off | Released | **Open** |

- Q1 and Q2 (2N7002) invert the logic so the relay is energized in the normal state.
- K1 (G5V-1) uses its **normally-open** contact: `SDC_IN` on COM, `SDC_OUT` on NO. The NC contact is unused (no-connect flag).
- D2 (1N4148) is a flyback diode across the coil, protecting Q2 from the voltage spike when the coil turns off.

### 7. Sensor fault detection (U3, U4)

A disconnected or broken sensor wire reads ≈0 V, and a short to the supply reads ≈5 V. In normal operation the sensors stay between 0.5 and 4.5 V, so I check each sensor against a **0.3–4.7 V window**.

- References: R16/R17 (158k/10k) → 5 × 10/168 = **0.30 V**. R18/R19 (10k/158k) → 5 × 158/168 = **4.70 V**.
- U3A/U4A: is the sensor **above** 0.3 V? (sensor on +). U3B/U4B: is the sensor **below** 4.7 V? (sensor on −, inputs swapped).
- All four outputs are wired-AND into `Sensors_OK` (pull-up R14). It's high only when both sensors are in range.
- Q3 inverts this into `Sensor_Fault` (pull-up R15), which pushes the latch node up through D3. D3 works like D1, so the two trip sources can't interfere with each other.

### Decoupling

Each comparator IC has a 100 nF capacitor across its supply pins (C1, C3, C4, C5).

## Known issues and open questions

1. **Startup glitch.** At power-up, the LMV393 takes a moment to become active, and U2B's output could float high through R11 and latch the BSPD by accident. Proposed fix: a ~100 nF capacitor from the latch node to GND to hold the + input low for a few milliseconds while the supply comes up.
2. **No hysteresis on the comparators.** If a sensor sits right at a threshold, noise could make an output chatter. The delay block filters most of this for the plausibility path, but adding a little hysteresis (a large feedback resistor to + input) would make the comparators more robust.
3. **Current sensor resolution.** If the BSPD shares a current sensor sized for full motor current (e.g. ±500 A), the 5 kW threshold is only ~33 mV above zero, which is easy for noise to swamp. A sensor with a separate low-range channel would help. Which sensor does the team use for the BSPD?
4. **Relay ratings.** Need to confirm the SDC current and voltage are within K1's contact rating. Also, the real SDC relay coil probably runs off the ~12 V GLV rail instead of 5 V. The driver circuit works the same either way.
5. **Sensor fault timing.** Should a sensor fault trip instantly, or after a short filter (older rules used ~100 ms) to avoid false trips from noise?
6. **Supply.** The schematic assumes a 5 V rail. A regulator from the GLV supply and the required power-input connector still need to be added.
7. **Inspection access.** The rules require a way to test the BSPD at tech inspection (disconnecting each sensor and power individually). This should be reflected in the connector design.

## Remaining work

- [ ] Add connectors (brake sensor, current sensor, power input, SDC in/out) to replace the off-sheet labels
- [ ] Clean ERC run
- [ ] Verify relay pin 5 is NO on the G5V-1 symbol
- [ ] Update thresholds with real sensor datasheets and 2027 rule values



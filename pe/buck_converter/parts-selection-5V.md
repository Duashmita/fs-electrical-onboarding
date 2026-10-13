# TPS54331 5 V / 2 A component selection

Prepared 2026-10-12. Input 10-15 V, nominal 13.5 V. This is a component selection and reference-design starting point, not a validated board or approval to order. The existing schematic has not been edited.

Selection follows supplied LV standards: exposed IC leads, resistor/capacitor sizes >=0603, hand solder footprints where provided, 1% resistors, ceramic capacitors, and >=50% voltage/power rating margin. The larger input/output capacitors retain practical capacitance and are permitted by the standards.

## Bill of materials

Prices are listed one-piece USD unit prices from DigiKey pages, rather than quantity-break optimized prices. Taxes, shipping, tariff and PCB excluded. Select Cut Tape, not Digi-Reel, and the direct DigiKey listing rather than marketplace substitutes. Stock is time-sensitive; some indexed pages are weeks/months old.

| References | Qty | Value | MFG | MFG PN / DigiKey link | Digi PN | Unit USD | Size and rating |
|---|---:|---|---|---|---|---:|---|
| U1 | 1 | TPS54331DR | Texas Instruments | [TPS54331DR](https://www.digikey.com/en/products/detail/texas-instruments/TPS54331DR/2047963) | 296-26991-1-ND | 1.62 | SOIC-8 |
| L1 | 1 | 6.8 uH | Bourns | [SRN8040-6R8Y](https://www.digikey.com/en/products/detail/bourns-inc/SRN8040-6R8Y/2756134) | SRN8040-6R8YCT-ND | 0.55 | 8 x 8 x 4 mm; 3.9 A rated, 4.2 A saturation |
| D1 | 1 | B340A-13-F | Diodes Incorporated | [B340A-13-F](https://www.digikey.com/en/products/detail/diodes-incorporated/B340A-13-F/725015) | B340A-FDICT-ND | 0.54 | SMA; 40 V, 3 A |
| C1,C2 | 2 | 4.7 uF | TDK | [C3216X7R1E475K160AC](https://www.digikey.com/en/products/detail/tdk-corporation/C3216X7R1E475K160AC/634397) | 445-1606-1-ND | 0.31 | 1206; 25 V X7R +/-10% |
| C3,C5 | 2 | 10 nF | KEMET | [C0603C103K5RACTU](https://www.digikey.com/en/products/detail/kemet/C0603C103K5RACTU/411366) | 399-C0603C103K5RACTUCT-ND | 0.12 | 0603; 50 V X7R +/-10% |
| C4 | 1 | 100 nF | KEMET | [C0603C104K5RACTU](https://www.digikey.com/en/products/detail/kemet/C0603C104K5RACTU/1465623) | 399-C0603C104K5RACTUCT-ND | 0.13 | 0603; 50 V X7R +/-10% |
| C6 | 1 | 4.7 nF | TDK | [C1608C0G1H472J080AA](https://www.digikey.com/en/products/detail/tdk-corporation/C1608C0G1H472J080AA/2732835) | 445-7400-1-ND | 0.21 | 0603; 50 V C0G +/-5% |
| C7 | 1 | 39 pF | KEMET | [C0603C390J5GACTU](https://www.digikey.com/en/products/detail/kemet/C0603C390J5GACTU/3471634) | 399-C0603C390J5GACTUCT-ND | 0.10 | 0603; 50 V C0G +/-5% |
| C8,C9 | 2 | 33 uF | TDK | [C3225X5R1A336M200AC](https://www.digikey.com/en/products/detail/tdk/C3225X5R1A336M200AC/2733168) | 445-7733-1-ND | 1.11 | 1210; 10 V X5R +/-20% |
| R1 | 1 | 332 kOhm | YAGEO | [RC0603FR-07332KL](https://www.digikey.com/en/products/detail/yageo/RC0603FR-07332KL/727165) | 311-332KHRCT-ND | 0.10 | 0603; 0.1 W +/-1% |
| R2 | 1 | 68.1 kOhm | YAGEO | [RC0603FR-0768K1L](https://www.digikey.com/en/products/detail/yageo/RC0603FR-0768K1L/727354) | 311-68.1KHRCT-ND | 0.10 | 0603; 0.1 W +/-1% |
| R3 | 1 | 49.9 kOhm | YAGEO | [RC0603FR-0749K9L](https://www.digikey.com/en/products/detail/yageo/RC0603FR-0749K9L/730212) | 311-49.9KHRCT-ND | 0.10 | 0603; 0.1 W +/-1% |
| R4 | 1 | 0 Ohm | YAGEO | [RC0603JR-070RL](https://www.digikey.com/en/products/detail/yageo/RC0603JR-070RL/729622) | 311-0.0GRCT-ND | 0.10 | 0603; feedback jumper |
| R5 | 1 | 10 kOhm | YAGEO | [AC0603FR-0710KL](https://www.digikey.com/en/products/detail/yageo/AC0603FR-0710KL/2828135) | 311-10KLDCT-ND | 0.11 | 0603; 0.1 W +/-1% |
| R6 | 1 | 1.91 kOhm | YAGEO | [RC0603FR-071K91L](https://www.digikey.com/en/products/detail/yageo/RC0603FR-071K91L/726877) | 311-1.91KHRCT-ND | 0.10 | 0603; 0.1 W +/-1% |
| J1,J2 (bench option) | 2 | 2-pin power header | Sullins | [PEC02SAAN](https://www.digikey.com/en/products/detail/sullins-connector-solutions/PEC02SAAN/859155) | S1012E-02-ND | 0.16 | 2.54 mm pitch; 3 A/contact; mating connection must also support 2 A |

Core components total: $6.84 using one-piece unit prices. Two optional bench headers bring total to $7.16. Matching cables are not included. R5 uses AC-series because the equivalent RC0603FR-0710KL is currently out of stock.

## KiCad assignments verified against installed KiCad 10 footprint files

| References | Symbol | Footprint |
|---|---|---|
| U1 | Existing imported TPS54331DR | Package_SO:SOIC-8_3.9x4.9mm_P1.27mm |
| L1 | Device:L | Inductor_SMD:L_Bourns-SRN8040_8x8.15mm |
| D1 | Device:D_Schottky | Diode_SMD:D_SMA_Handsoldering |
| C1,C2 | Device:C | Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder |
| C3,C5 | Device:C | Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder |
| C4 | Device:C | Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder |
| C6 | Device:C | Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder |
| C7 | Device:C | Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder |
| C8,C9 | Device:C | Capacitor_SMD:C_1210_3225Metric_Pad1.33x2.70mm_HandSolder |
| R1 | Device:R | Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder |
| R2 | Device:R | Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder |
| R3 | Device:R | Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder |
| R4 | Device:R | Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder |
| R5 | Device:R | Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder |
| R6 | Device:R | Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder |
| J1,J2 (bench option) | Connector_Generic:Conn_01x02 | Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical |

U1's symbol is already embedded/imported in your project; it is not a standard installed TPS54331 symbol. Use that existing symbol after checking its pin numbers against TI. Standard footprints above exist locally. The inductor footprint references the actual Bourns SRN8040 datasheet. Verify diode symbol pin 1/cathode matches footprint pad 1. Passive generic symbols work with these purchased parts; individual manufacturer symbols are unnecessary.

## Changes relative to the supplied screenshot

- R5: 19.2 kOhm -> 10 kOhm; R6: 3.25 kOhm -> 1.91 kOhm. Vout=0.8*(1+10/1.91)=4.988 V nominal. The original divider produces approximately 5.526 V.
- R3: 29.4 kOhm -> 49.9 kOhm; C6: 1 nF -> 4.7 nF; C7: 47 pF -> 39 pF.
- C8,C9: 47 uF -> 33 uF each, in parallel. These values align with TI Table 7-1's ceramic-output 12 V-to-5 V design.
- Keep R1,R2,C3,C4,C5 and L1 values. R1/R2 give undervoltage enable thresholds below the 10 V operating minimum; they are not protection that enforces a 10 V minimum.
- Replace C6/C7 electrolytic footprints with 0603 ceramic footprints. Replace L1's 0805 footprint with SRN8040, D1's DO-35 footprint with SMA, and U1's unqualified D8 assignment with the standard library-qualified SOIC footprint.
- Replace the 1.00 mm input header footprint if using the selected 2.54 mm header. J2 is an optional output connector to access 5 V/GND for bench testing.

## Calculated checks and limits

At 15 V input, 5 V output, 6.8 uH, 570 kHz: inductor ripple approximately 0.86 A p-p, peak at 2 A load approximately 2.43 A. With -30% inductance and 456 kHz minimum switching frequency, estimated ripple is 1.54 A and peak 2.77 A, below the listed 4.2 A saturation rating. This checks normal 2 A operation, not fault or 3 A operation. Datasheet frequency is 570 kHz nominal but 456 kHz minimum: satisfies the project's nominal frequency interpretation only, not a strict guaranteed >=500 kHz criterion.

All capacitors have >=50% voltage-rating margin: input 25 V >=1.5*15 V, output 10 V >=1.5*5 V. Resistor divider worst-case dissipation is a few mW, far below the 0.1 W ratings; do not run the 2 A load current through them. R4 carries feedback current only.

Output ripple target <=50 mV p-p (1% of 5 V), with 250 mV p-p the project's 5% upper limit. Do not assume two labeled 33 uF capacitors provide 66 uF in circuit. TDK DC-bias curves at 5 V, temperature/tolerance, ESR and actual effective capacitance need final checking and compensation/stability recalculation over the 10-15 V input range. The manufacturer's interactive graph did not supply numeric DC-bias data through the text retrieval. For example, with worst-case estimated ripple 1.54 A, effective combined C=30 uF and combined ESR=5 mOhm, switching ripple approximation is about 22 mV p-p; those capacitance/ESR numbers are conditions to verify, not claimed measurements of the selected parts. Validate load transients, light-load Eco-mode ripple and IC/diode temperature on the finished board.

## Source documents and datasheets

- LV standards supplied by user: C:/Users/ashto/.codex/attachments/e6ae1d1c-ef38-41b7-8578-3f46eddbe9df/Pasted text.txt
- Onboarding requirements supplied by user: C:/Users/ashto/.codex/attachments/4d3f93e0-98e3-44a0-9514-a1e69a48ac88/Pasted text.txt
- [TI TPS54331 datasheet, Table 7-1 and layout guidelines](https://www.ti.com/lit/ds/symlink/tps54331.pdf)
- [Bourns SRN8040 datasheet](https://www.bourns.com/docs/Product-Datasheets/SRN8040.pdf)
- [TDK selected output capacitor and DC-bias tools](https://product.tdk.com/en/search/capacitor/ceramic/mlcc/info?part_no=C3225X5R1A336M200AC)
- Manufacturer datasheets are linked from each DigiKey product page. Add the specific datasheet link, Description, MFG PN, MFG, Digi PN and Digi URL to each schematic symbol as the LV standards require.



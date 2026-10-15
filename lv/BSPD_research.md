BSPD circuit(Brake Sensor Plausibility Device):

AiM Sports Pressure Sensors X05PSA02000P18
Current Transducer HASS 50 ... 600-S

Rules
- simultaneous braking and high power output
- BSPD must open shut down circuit if:
  - Demand for hard braking for more than .5 sec
  - tractive system current delivers 5kW of power in the DC circuit delivered to motors at at nominal battery voltage (nom batt V: 108 V)
    for more than 0.5 sec
  - if there is an open circuit or short circuit in any sensor input
- demonstration of BSPD circuit 
  - prove function of bspd in vehicle + current sensor 
  - not send power to motors of vehicule
how to test: induce a current from seperte wire from external power supply to simulate tractive system current 
             (pressing brake pedal)
            - vehicule must have wire or coil to simulate tractive system current to bspd current sesor 
            - two connection points for external power supply 
              - connection points: two 4mm shroouded banana jacks (not red or black), labeled CS+ & CS-,
                be accessible for active inspection located on vehicle or on adaptor harness/breakout box

Sensors:
- Brake sensor
  - potentiometer like sensor: moves as the brake pedal is pressed which increases the voltage on the sensor output
  - range of 0.5 V - 1.4 V
- Current Transducer
  - Uses a hall effect sensor that creates a  proportional to the current on the high  connectors
  - multiply current and  together to make a proportional power

What is hall effect:
- creation of a measurable  across an electrical conductor or semi conductor when an electric current flows through
  it and a magnetic field is applies perpendicular to the current

Op-Amp Comparator:
- compares an analog inpput voltage with another analog input voltage and determines which is highest of the two voltages.

op-amp vs opamp comparator:
- op-amp: closed loop w neg feedback
- op-amp as comparator: open-loop w no feedback

<img width="820" height="546" alt="ECE 171 L 6" src="https://github.com/user-attachments/assets/b9b4fac8-320f-45b7-a60f-f0e089b36c10" />

- choice of resistors:
-   R_L > 2k
-   R_o < 5
-   input impedance of (-) branch should be abt R1

Overall circuit:

one comparator for hard brake
- (250-300 PSI) -> (4500 V - 500 V)/(2000 PSI) = 2 V/PSI
- (250 PSI * 2V/PSI) = 500 V
- 300 PSI -> 600 V

- V ref = 550 V

one comparator for accelerator
- current seniors outputs a DC signal (two wires intertwined to minimize noise)
- take the DC signal use a differential amplifier to convert to a single ended signal
<img width="424" height="297" alt="Screenshot 2026-10-15 at 2 49 04 PM" src="https://github.com/user-attachments/assets/de3e1bd6-eba8-410e-b1fb-77427542ec9b" />
- Vo from differential amp has to be compared to Vref = 2.5 V in a comparator circuit pre AND gate
Both comparators connected in an and gate
- InAmp 

if output 1, then open shut down

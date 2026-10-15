BSPD circuit(Brake Sensor Plausability Device):

AiM Sports Pressure Sensors X05PSA02000P18
Current Transducer HASS 50 ... 600-S

Rules
- simultaneous braking and high power output
- BSPD must open shut down circuit if:
  - Demand for hard brakeng for more than .5 sec
  - tractive system current delivers 5kW of power in the DC circuit delivered to motors at at nominal battery voltage
    for more than 0.5 sec
  - if there is an open circuit or short circuit in any sensor input
- demonstration of BSPD circuit 
  - proove function of bspd in vehicule + current sensor 
  - not send power to motors of vehicule
how to test: induce a current from seperte wire from external power supply to simulate tractive system current 
             (pressing brake pedal)
            - vehicule must have wire or coil to simulate tractive system current to bspd current sesor 
            - two connection points for external power supply 
              - connection points: two 4mm shroouded banana jacks (not red or black), labeled CS+ & CS-,
                be acsessible for active inspection located on vehicule or on adaptor harness/breakout box

Sensors:
- Brake sensor
  - potentiometer like sensor: moves as the brake pedal is pressed which increses the voltage on the sensor output
  - range of 0.5 V - 1.4 V
- Current Transducer
  - Uses a hall effect sensor that creates a voltage prooportional to the current on the high voltage connectors
  - multiply current and voltage together to make a proportional power

What is hall effect:
- creation of a measurable voltage across an electrical conductor or semi conductor when an electric current flows through
  it and a magnetic field is applies perpendicular to the current

Op-Amp Comparator:
- compares an analog inpput voltage with another analog input voltage and determines which is highest of the two voltages.

op-amp vs opamp comaparator:
- op-amp: closed loop w neg feedback
- op-amp as comparator: open-loop w no feedback

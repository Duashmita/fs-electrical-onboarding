These will be the associated notes for the strain gauge project. I meant to start this one earlier but Physics 5C's midterm got in the way for a little so it had to be postponed.

# Project Goal Notes
- Research how other FSAE teams have implemented strain gauges
- Design my own strain gauge (using flexible PCBs)
- Need to "compute the necessary length of minimum width copper trace needed to equal 350 ohms of resistance"
    - Minimum width according to JLC: 
    12μm copper trace: 3/3 mil
    18μm copper trace: 3.5/3.5 mil
    35μm copper trace: 4/4 mil
    Thickness of trace shouldn't be too large considering this is for measurement, not too much current (I think)
- Also need to determine the size of the rectangle needed to fit those traces, based on JLC minimums

# FSAE Predecessor Research
- https://dewesoft.com/blog/measuring-wishbone-deformations-on-formula
    - This team measured wishbone deformation using strain gauges
    - Used resistive strain gauges
    - Article includes equations to relate different factors of deformation, may be useful later

# FPC Advantages and Drawbacks
- Can be bent without causing significant damage
- Typically weigh less (although for this case the difference would be minimal)
- Good heat dissipation (kind of irrelevant)
- High initial costs
- Not repairable



# Computation Notes
- Equation I found for trace resistance:
R = ρ * (L/(T*W)) * [1 + α * (temp-25)]

ρ - resistivity (1.7 * 10^-6 ohm-cm for copper)
L - length (goal, since resistance is in spec)
W - trace width (specified by JLC, going with 3 mil)
T - trace height (12μm for 3 mil width from JLC)
α - temperature coefficient (3.9 * 10^-3 ohm/ohm/C for copper)
- Since for this consideration I'm going to assume STP (there's no consistent operating temperature in this environment regardless, so the resistance variability cannot be accounted for in this approximation)
- Temperature portion then cancels out (25C -25 = 0)
- Did the math on a notepad the way Dhar intended
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
- Did the math on a notepad
- Initial result 0.188m, informed that this was off by 2 orders of magnitude
- Actual result: 18.8m, mistake was calculator input-based

# Gauge Shape Consideration
- According to Luca, the diameter of the rods for the suspension are 16mm. From that, the circumference can be calculated
- An additional note is that unlike other FSAE strain gauge implementations, FS instead wants to have the sensor wrap around the entire rod rather than a portion for increased data accuracy
- The next step is to determine the actual dimensions of the sensor. To do that, I'm going to get the circumference, subtract 2x the board edge spacing (0.3mm for JLC), and then divide the result by 6 mil to accomodate space between traces. It's worth noting I don't intend on slightly overlapping the sensor in order to compensate for the slight amount of edge spacing dead space as that overlap could introduce stiffness or other abnormalities that could jeopardize the integrity of the data
- Effective width of strip = 0.049665m
- Divided by 6 mil, I got approximately 325 lines back and forth, which evens out to 324
- 162 back and forth line pairs at the smallest size
- Now the actual length of the strip needs to be determined from the trace length of 18.8m (18.8m/324 runs back and forth)
    - This yielded an unfortunate 0.058m of length, going any longer would introduce excessive resistance
    - Did a little more research, and from what I've read the distances between the traces do matter with respect to having as many traces for data averaging as possible. That being said, most strain gauges are rather small and have around 20 traces or 10 back and forth trace pairs
    - With this in mind, I'm going to increase the trace spacing to accomodate a longer sensor length
    - It is worth noting this may result in high temperature variability between traces , but I'm hoping that the excessive amount of trace lines will help compensate for this by providing a larger surface area to account for temperature fluctuations
    - I also considered increasing the size of the trace alongside the trace width to maintain the 1:1 trace to spacing ratio, but that (according to the equation above) should decrease the resistance by length, nullifying the benefit of wider traces
    - With 15 mil per trace (3 mil trace plus 12 mil spacing), the length of the entire sensor ends up at 0.144m for the sensor, which is almost half a foot long
    - For the sake of this exercise, I'll treat that as sufficient, as I do not know what the average length of a suspension member is
    

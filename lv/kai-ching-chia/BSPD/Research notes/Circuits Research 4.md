### Hysteresis and noise immunity
### The comparator's stability problem
1. ex) the input is ~2.5 V and the reference signal is a stable 2.5 V
	- if the input signal fluctuates, the comparator keeps getting toggled
	- called "output chatter"
### Hysteresis
1. Meaning: using 2 switching thresholds instead of one
	- instead of switching H and L at the same V, use:
		1. upper threshold (L to H) and lower threshold (H to L)
		2. creates a region where small fluctuations do not matter, basically a safe zone
			- this is called the hysteresis width
			- Vh = Vupper - Vlower
2. implementation
	- basically eats its output (output also leads into input)
	- "positive feedback loop" - helps the system ignore some noise
### Other methods
1. The problem of rapid switching by fluctuations can also be solved by
	1. fault detection (determines if condition is outside limits)
	2. latching (shutdown until reset condition is fulfilled)
	3. timing circuits (cooldown before response, aka debounce in roblox)
2. Hysteresis needs to be selected carefully
### Latches
1. A latch can retain a state with 2 stable states
	1. Set: shutdown
	2. Reset: return to normal functions
	- this will be extremely useful for the BSPD
2. What exactly is a latch?
	1. circuit that can retain a logical state (1/0)
		- even if the trigger input changes, it will retain the shutdown state until the reset is triggered
3. SR latches - set reset latches
#### Circuit memory
1. latches do not require software
	- you can make it with 2 nor gates (combination of not and or gate)
	- wont change states unless reset is toggled (even if set is toggled, this is for SR latch though idk about the other ones)
	- forbidden input: set and reset toggled, causes unpredictable outcome
2. how the nor gate memory works
	- nor gate only outputs 1 when both inputs are 0
	- if either input is 1, output is still 0
	- SR latch connects 2 nor gates so that the output feeds into other gates input
		- called cross-coupled feedback
- remember, latches are NOT timers
- for the bspd, the latch will need to remember the state after removing power (by default it doesnt, so somehow gotta set it up)
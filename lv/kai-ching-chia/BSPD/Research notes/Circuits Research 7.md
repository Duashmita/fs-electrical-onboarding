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
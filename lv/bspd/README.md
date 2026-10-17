NOTE: ignore the william-kwok kicad document. its INC

Explanation of BSPD block diagram

Looking at the FS-4 VCU BSPD schematic
	i.    "Current + Brake Sensor: Buffers + Filters"
		a. hass sensor Vref and Vout
			i.  Vref: a zero point?
			ii. Vout: the actual current sensor signal
		b. brake sensors. one for front and one for back wheels
			i. 0.5-4.5v signal input
		c. all 4 signals are filtered to varying degrees. 
			i.  current sensors are not filtered much.
			ii. brake sensors are filtered much much more
				a. probably bc brakes are more affected by vibrations and stuff like that?
		d. Buffers exist because there are a ton of resistive loads downstream of these filtered signals, will cause Vdroop without
	ii.    "BSPD Sensor Short/Open"
		a. answers the question, "are these values I am getting physically possible?" faults trigger when 
		b. thresholds are voltage dividers that put the thresholds where in the region of both sensors's impossible output range (0-5v) under their normal op conditions
			i. thresholds are set not set right at 0 and 5v because 0 and 5v cannot reliably be reached even for rail-to-rail op-amps. 
				a. i also assume that the thresholds are under the reliable output peak and ceilings for these op-amps too
	iii.   "BSPD High Current + Hard Braking"
		a.  Current Sensor Portion
			i.    difference amplifer compares current sensor output to its reference sensor. amplifies by 20 times
				a. 20 times bc raw current difference is small
			ii.   output goes into the  comparator that is set at the cutoff
			iii.  that output is basically a binary, "am i drawing enough current to be evaluated by the BSPD?"
			iiii. and finally, that becomes one of the two inputs to the big BSPD AND logic gate
		b.  Brake Sensor
			i.  the two filtered and buffered brake sensor inputs fed into their respective comparators evaluated against an adjustable voltage (a pot?)
			ii. these get ORed and then get fed into the 2nd leg of the big BSPD AND logic gate
	iiii.   "BSPD Latching Relay"
		a.  the impausibility fault is OR'ed with the three open/short logic lines from each of the sensors
		b.	the output of that is fed into an RC timer, a comparator op-amp with a set threshold waits for the cap to reach a certain voltage
		c.	then the op amp rails high, triggering a relay driver that triggers a bistable latching relay
		d.	that triggers a monostable relay, breaking the shutdown loop -> cuts HV

# What I Learned on the FSLV Onboarding: BSPD

1. Learned abt analog comparators
	a. logic 2in 1out -> inputs are analogue, outputs are digital
		i.   input  0: non-inverting (+)
		ii.  input  1: inverting (-)
		iii. output 0: 
	b. two supply voltages that out0 swings between
	c. 
		i.   if in0 > in1, then out0 = 1
		ii.  if in0 < in1, then out0 = 0
		iii. BSPD should trip high when implausibiltiy condition is met -> V_brake_signal on (+) and V_brake_threshold on (-)
	
2. Learned abt op-amps
	a. basically the same in and out as comparator
		i.  usually cant get to the ceiling and floor supply voltages, but they get close
		ii. rail-to-rail get close?
	b. open loop
		i. op-amps can be used as a comparator if open loop (out does not loop back to in)
	c. closed loop
		i.   feedback. (out goes back to into to inverting input)
			a. positive feedback (output hooked to V+)
				1. a rise in v+ results in a rise in v out -> leads to a rise v+ -> slams the + rail
				2. works in the opposite direction if v+ drops
				3. can cause latching (good?)
				4. tuning positive feedback controls how hard it "sticks" 
			b. negative feedback (output hooked to v-)
				1. if v- > v+, neg feedback pulls v_out down and brings v- back to v+
				2. opposite behavior if v- < v+
		ii.  op-amp self-corrects and wants both inputs to be equal
				a. resister at output determines what it equiliberates to

	d. buffer
		i.   one input, v_out feeds back to v-
		ii.  only non-inverting amp
		iii. push-pull buffer. isolates source from load and increases drive strength
		
	e. non-inverting amplifier
		i. finite input impedance (affects the source)
	f. summing amplifier
		i. multiple inputs to a single input with voltages weighed by resistors can allow a single op-amp to make a discrete output based on multiple analog inputs
		
3. Learned abt voltage dividers
	i . two resistors between supply and ground. tap into the node in the middle
	ii. voltage at that node is a ratio of the input voltage R1 / (R1 + R2)
	iii. anything that is connected to that node draws current and pulls Vout down.
		a. comparators and op amps are fine here because their inputs draw neglibile amounts of current.
		b. same reason why sensors are buffered before they are used, to prevent this Vdroop.
	iiii. small resistors waste a lot of current (v=ir) -> resistors heat up, but output is pretty stuff
	iiiii. large resistors waste less current, but the output is soft.
	iiiiii. 10s kO is generally ok.
	iiiiiii. every part has some slop.

4. Learned about RC timing
	i. a resistor in series with a capacitor
	ii. reisstor limits how fast the capaactiro fills up
	iii. a larger resistor or larger capacitance increases how long it takes to fills
	iiii. its a simple way to time something. a comparator / op amp can then read the voltage of the capacitor and decide what to do with it.

5. Learned about latching
	i.  is using a positive feedback op-amp good for latching the bsdp?
		a. silent failures
	ii. SR latch
6. Learned active high wired AND 
	i.    https://en.wikipedia.org/wiki/Wired_logic_connection#The_wired_AND_connection
		a. works bc open drain exists
		b. diode trick can guarantee that it works regardless of open drain or push-pull op-amps
	ii.   https://en.wikipedia.org/wiki/Diode
		a. unidirectional current flow
	iii.  this is a bad idea for BSPD bc its a safety circuit
	iiii. would require a buffer or comparator 

7. From the Data sheest
	i. 5kw -> 46A @108v
	ii. that is roughly 96mV on the HASS current sensor
	iii. about 1.93V after gain 20.
	iiii. HE sensor HASS 300-S
		a. vref = 2.5v
		b. 0.6 to 4.4v
	iiiii. Brake sensor
		a. 0.5 to 4.5v
		
8. Rules Requirements
	i.    BSPD should open shutdown circuit when hard braking and 5kW or more power is drawn from the battery.
	ii.	  has to be tripped within 0.5s
	iii.  non-programable
	
9. Summing Op Amp
	i.	 looked into this as an option for summing all the outputs of the open/short output block
	ii. 	the output is really complicated. inverted output
	iii.	tighter detection margin, more parts, more work to make working

10. Differential Op Amp
	i.	 why use one?
		a. the hall effect sensor that FS4/FS5 uses has lines for both current output and the current sensor's Vref
		b. allows the output to be relative to itself. cancels noise or offsets picked up through transmission
		c. Vout=(R2/R1) * (Vcurrent - Vref)
		d. basically, Vcurrent - Vref. Then blow up that difference by a factor (gain)
	ii.  inputs
		a. Vcurrent on the non-inverting input
		b. Vref on the inverting input
	iii. resistors	
		a. R1, R2, R3, R4 where R1 = R3, R2 = R4
		b. R1 Vcurrent to non-inverting input
		c. R2 Vcurrent to GND
		d. R3 Vref to inverting input
		e. R4 Output to inverting input
		f. ratio sets differential gain
		g.  Not sure abt why the
		h. I would assume that tight resistors are used here. 
		
11. Schmidtt trigger / Schmidtt input
	i. turns a slow signal or a noisy signal into a clean discrete one.
	ii. its hystersis as a device
	iii. op amps can be a used as a schmidtt trigger with a positive feedback loop.
	
12. Threshold
	i. rules requires trip at 5kW. that's 46.3A for us.
	ii. doing something like 4.5kW threshold would be good. 42A @ 108V.
		a. this would definitely be below 5kW
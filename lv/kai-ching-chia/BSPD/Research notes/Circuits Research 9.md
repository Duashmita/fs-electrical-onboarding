### Power supplies
1. literally in the name again
	- every electronic component needs power to work
	- however, they might require different voltages
		- go too high and it will fry the component
		- go too low and the component wont even work
2. voltage regulators
	- changes input voltage into regulated output voltage
		- ex) 12 V into 5 V
		- two types:
			1. linear regulator - conv voltage into heat, very simple
				- equation for power dissipated:
				Ploss = (Vin - Vout) * I
			2. switching regulator - uses switching circuitry to convert voltage, but can introduce electrical noise
				- used for larger voltage drops
3. Supply voltage is not the same as signal voltage
	- signal voltage carries information
	- supply voltage is just power
		1. however, if supply voltage is unstable, a bunch of things stop working
	- reference voltage is just a comparison point
### Output drivers
1. core idea: comparator/logic circuitry outputs low current signal
	- relay coils might need a bunch of current
	- Enter output drivers: (basically amplifier i think)
	- MOFSET
#### What is a MOFSET?
1. transistor that can act as electronically controlled switch
2. main parts are:
	1. Gate (G): control
	2. Drain (D): one side of switched current path
	3. Source (S): other side of switched current path
3. basically, train track switcher
### Pull up and pull down resistors
1. Floating inputs - disconnected inputs (not high nor low)
	- solution: use resistor to give input a default state (look below)
2. Pull up resistor - gives default high state
3. Pull down resistor - gives default low state\
- why use resistors instead of connecting to ground? 
	- 
### Relays
1. Is an electrically controlled switch. many use coils
	- the coil is inductive
	- when current in it is interrupted, magnetic field collapses
		this generates a voltage spike and can damage transistors or MOFSET
2. Flyback diodes
	- does not conduct when power is on
	- this means that when the power turns off suddenly (voltage spike), it starts conducting and lowkey absorbs that shi
### Signal protection methods
1. decoupling capacitor - placed near component's power supply pins to keep supply voltage stable
	- changes in the current dont effect the supply voltage too quickly
2. reverse polarity protection - prevents damage if ps connected backwards
3. transient protection - protect against voltage spikes
4. current and voltage ratings - every component will be rated for what they can handle
5. grounding - components need the right connections for reference and current return
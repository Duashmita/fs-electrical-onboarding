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
### Relays
1. Is an electrically controlled switch. many use coils
	- the coil is inductive
	- when current in it is interrupted, magnetic field collapses
		this generates a voltage spike and can damage transistors or MOFSET
2. Flyback diodes
	- does not conduct when power is on
	- this means that when the power turns off suddenly (voltage spike), it starts conducting and lowkey absorbs that shi

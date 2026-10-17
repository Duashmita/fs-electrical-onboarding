### Voltage reduction
- imagine you have 12 V supply but some of your LV components need like 5 V 
1. Linear regulator - reduces voltage by converting energy into heat
	- simple, cheap, low noise
	- unfortunately might get too hot and inefficient
2. Buck converter - turns electrical energy on and off quickly
	- smoother output voltage
	- more efficient
- we want to waste less energy, so we choose a buck converter
### Understanding the core components
1. MOSFET - transistor (connects to input supply when on, reverse if off)
2. Diode - when MOSFET turns off, current goes through here (alternate route)
3. Inductor - stores energy in magnetic field when current runs through
	- resists rapid changes in current
	- if input turns off, releases stored energy
	- smoothing fluctuations in energy
4. Output capacitor - stores energy in electric field
	- supplies current when load needs extra energy
	- absorbs extra current
	- output voltage stabilizer
### How it functions
1. State A: on
	- MOSFET is on, current flows in
	- inductor magnetic field grows as it stores energy
	- inductor current increases
2. State B: off
	- MOSFET is off, disconnects
	- inductor magnetic field collapses, releasing energy
	- inductor generates the voltage to maintain current, current slowly goes down
- repeat this very fast (many times per second)
### Key terms
1. Duty cycle (D) : D = Ton / (Ton + Toff)
	- 50% means switch is on 50% of the time
	- usually evaluates to fraction between 1 and 0
	- Vout = D * Vin, D = Vout/Vin : use these to find out what the duty cycle is w voltage
2. Switching frequency : Tsw = 1 / Fsw
	- tells you how many on/off cycles per second
	- Fsw - measured in hz, Tsw - duration of one switching cycle
	- higher Fsw allows for a smaller inductor
3. Voltage ripple : small repeated varaition in output voltage around ave value
	- amplitude of the function
	- since inductor current isn't always constant, it might look like a sin wave
	- usually, +output capacitance reduces ripple
	- delta Vout ~ delta IL / (8Fsw * Cout)
		- delta Vout = peak to peak voltage ripple
		- delta IL = peak to peak inductor current ripple
		- Cout = output capacitance
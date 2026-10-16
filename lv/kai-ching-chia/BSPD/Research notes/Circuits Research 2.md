### Voltage divider
1. literally in the name LMAO
	- produces lower voltage from higher input voltage with 2+ components
		- usually resistors
	- used when component/circuit needs a smaller voltage range
	- ex) sensor makes 0-5V signal, but another circuit needs 0-2V signal
2. has 4 important parts
	- Vin = input voltage applied across both (or however many there are) resistors
	- R1 = resistor between input V and output node
	- R2 = resistor between output node and GND
	- Vout = voltage between the two resistors and GND
- note: output is measured across r2
- this relies heavily on Ohm's Law and Kirchhoff's Law
- basically splits voltage into 2 different voltages which you can use to ur likin
### Voltage divider equation
Vout = Vin (R2 / (R1 + R2))
V in volts, R in ohms
- you can conv VDE to current asw with ohms law 
1. How does this work (resistors in series)
	- Rtotal = R1 + R1
		I = V / R = Vin / Rtotal
		I = Vin / (R1 + R2)
		Vout = IR2
		Vout = (Vin * R2) / (R1 + R2)
- ex) reducing 12V to 4V
	Vin = 12V, R1 = 2kohms, R2 = 1kohms
	1. I = 12 / 3kohms
	2. Vout = (12 * 1) / (2 + 1) = 4V
		- this means that the input got cut down into 1/3
- what happens if the resistors change? (example scenarios)
	1. +R2 while =R1 : increases output V
	2. -R2 while R1 : decreases output V
	3. -R1 while R2 : decreases output V
	4. if R2 > R1 output approaches input V
	5. if R2 < R1 output approaches GND
	6. if R2 = R1 output = 1/2 input V
#### Currents
1. remember how you can calculate current from this equation?
	- I = Vin / (R1 + R2)
	- ex) same figures as last example
		I = 12 / 3000 = 0.004 A = 4 mA
### How sensor circuits work
1. detection (transducer)
	- physical element changes electrical property in response to env
		ex) thermoresistor changes resistance based on temp
	- however, these changes are usually hella small (microvolts n shi)
		- also can have high impedence (current is almost 0)
		- very susceptible to noise affecting it
2. signal conditioning
	- voltage dividers
		- basically, the divider chops down the volts
		- small percent change -> becomes bigger percent change
	- wheatstone bridge (?)
	- amplifiers/filters
3. output
### Electrical power
1. P = VI
	- p = power in watts
	- more volts/current = more power
### RC circuits
1. RC circuit = circuit with resistor and capacitor
	1. resistor - limits current
	2. capacitor - stores charge and energy
	- when connected, makes voltage change gradually instead of immediately
2. this can act like a timer (how long it takes for the signal to fade)
### Capacitors
1. When a capacitor fills (assuming that theres a resistor), the voltage rises quickly at first, then it slows down as it approaches supply voltage
ex) 5V supply, resistor, & uncharged capacitor. What happens when you connect the supply?
	1. current flows thru resistor into capacitor
	2. as capacitor voltage increases, less voltage remains across the resistor, so current decreases
2. the equation: Vc(t) = Vs (1-e^ -t / (RC)) ^b0b0cb
	1. Vc(t) = capacitor voltage
	2. Vs = supply voltage
	3. R = resistance
	4. C = capacitance (farads)
	5. t = elapsed time in seconds
### The time constant
1. represented by greek letter tau, describes how fast RC circuit responds
	- tau = RC, in seconds
	- this is because ohms/farads = seconds
2. as time increases, charging rate decreases. similar to sqrt (x) equation
	now, what happens when you change R or C
	1. +R or C : charging takes longer
	2. - R or C : charging takes less time
	3. linear relationship (increase R by 2 makes time 2x longer)
### Capacitor charge and discharge
1. Since we know it takes time to fill the capacitor in an RC circuit, use it
	1. Connect capacitor output to comparator
	2. Until the correct time has passed (for the capacitor to fill), it won't discharge the correct voltage to trigger the comparator
	equation for calculating delay (derived from the equation above [[Circuits Research 5#^b0b0cb|this thing]])
	t = - RC ln (1 - Vth / Vs)
2. What about the discharge rate?
	1. voltage decreases exponentially
	2. equation: Vc(t) = V0e^ -t / RC
		V0 is the initial charge
remember, this is not always a 100% spot on accurate timer. there will be some variability
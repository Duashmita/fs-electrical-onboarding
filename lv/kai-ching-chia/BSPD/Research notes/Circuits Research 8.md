### Sensors
1. circuitry that makes the sensor's output usable by the rest of the system
	- sensor does not necessarily make decision, it can just provide information
2. different sensors can make different outputs
	1. voltage
	2. current
	3. resistance
### Window comparators
1. make upper and lower bounds
	- checks if sensor voltage is above 4.5 V
	- checks if sensor voltage is below 0.5 V
- makes a range of acceptable voltages
### Signal conditioning
1. a sensor's raw output is not always ready to be used
	- we need to
	1. scale voltage into good range
	2. filter out noise
	3. buffer signal so next circuit doesnt load sensor
	4. protect input from voltages outside safe limits
### Faults (not good)
1. Electrical range fault
	- voltage goes outside sensor's expected electrical range
2. plausibility fault
	- voltage is valid but measurement doesnt make sense (bug)
	- ex) sensor stays at 2.5 V 
- general hardware signal path
	1. Sensor (produces voltage)
	2. Signal conditioning (scales, filters, protects signal)
	3. Fault detection (check if within bounds or plausibility stuff)
	4. shutdown (if fault detected then no)\
### Additional concepts
1. Sensor loading: if circuit connected to sensor draws too much current, alters sensors output voltage
2. Noise filtering: if there is too much electrical noise, the signal might be briefly pushed in/out of wanted threshold
3. input protection: sensor interface might need to protect IC inputs from excess voltage or transients (spike of current/voltage)
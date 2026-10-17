### Comparators in more depth
1. component that compares 2 voltages, outputs indicating which is higher (like mc)
	- digital output (binary)
2. three important connections
	1. non inverting input + (one voltage)
	2. inverting input - (other voltage)
	3. output (indication of result)
	- usually works like
	+>- then high V
### Reference voltage
1. ref voltage is voltage chosen as comparison threshold
	- ex) ground (GND)
2. usually use this when you want to detect a certain voltage threshold 
	- ex) ref V = 3V and you use a comparator to see if sensor shoots out something higher than 3V. the ref V will be an input\
3. creating your ref V
	- Vref = Vsupply (R2 / (R1 + R2))
	- ex) you have 5V supply and want 2.5V ref
		make it so that both resistors are 10kohms
		Vref = 5 (10 / (10 + 10)) = 2.5 V
### LM393 (comparator IC)
1. dual comparator (2 seperate comparators)
	- open collector output
		- basically a one way switch: can turn output low, but cant turn it back up to high
		- disconnects if at certain value (from my understanding)
	- pull up resistor
		- helps it reconnect (like a helpful rubberband)
	- im wondering now what are the advantages
		basically, this makes it so that instead of being like a regular comparator (which can only output 2 specific voltages), it can either do min or max voltage
2. For the bspd, since we need a range of 0.5 V to 4.5 V, this dual comparator tech is very nice muy bien stuff
	- set one comparator to bottom of range and the other to the top of the range
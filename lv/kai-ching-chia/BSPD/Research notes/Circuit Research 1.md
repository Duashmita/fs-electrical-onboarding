### What is it?
1. connected path
2. made of four things
	1. power supply (battery)
	2. resistor (limits current)
	3. LED (makes light when current flows)
	4. GND (reference voltage)
### Nodes & comparators
1. node = group of directly connected points that share same electrical potential
	- electrical potential = voltage
	- in short a node is the intersections of wires that are connected to components
2. some rules about nodes
	1. components connected by wires with no components in between belong to the same node
	2. two wires crossing on a schematic dont mean they are connected. look for connection dot
	3. voltage is always measured between 2 points. if a node is 2V, it means its 2V relative to designated gnd
3. comparator = component used to find which of two voltages/currrents is greater
	 - compares analog current/voltages
	 - outputs a digital signal (binary)
### Ground & power rails
1.  (GND) is voltage ref - usually assigned 0V
	- voltage is all relative, therefore, if something is 5V, it means its 5V more than GND
2. power rail - something that provides a constant supply voltage and current to component
	- low resistance
	- can be wire, line, pcb layer
### Series vs Parallel connections
1. Series: components are connected in the same path
	- same current in each component
	- their voltage drops (each component makes V drop, see [[]]
2. Parallel: components connect across the same node
	- each branch has same voltage
	- current divides between branches
examples of why the difference is important:
	1. two resistors in series make voltage divider
	2. sensor outputs connected in parallel can interfere with one another
### Guide for analyzing circuit
1. identify power supply and ground
2. identify components & connections
3. find nodes
4. find relevant voltages
5. apply circuit laws (ohms, etc)
example:
	- 5V supply, 1 komh resistor, and an LED w 2V, in a series
	- V(of resistor) = 5-2 = 3V
	- use ohm's law
		I=V/R=3/1000 = 0.003 amps
		the current is 3 milliamps (mA)

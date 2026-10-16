### Digital stuff explained
1. Binary states
	1. 0/low voltage
	2. 1/high voltage
	- this usually depends on the voltage range of the circuit
2. AND gate
	1. only produces a 1 when all inputs are 1
	2. Y = A /\ B or Y = A * B
3. OR gate
	1. only produces a 1 if at least 1 input is 1
	2. Y = A \\/ B or Y = A + B
4. NOT gate (inverter)
	1. reverses logical state (ex. 1 to 0)
	2. Y = -| A 
### Combination of gates 
1. many circuits will use many gates to make a specific requirement
ex) F1 = first sensor fault, F2 = 2nd sensor fault, E = enable system
F's are the triggers, E is the system starting. we want this system to become active if either sensors report a fault, but only if a system enable signal is active

it would probably be something like:
Y = E /\ (F1 \\/ F2)
### Truth tables
1. shows every possible output of every combination of inputs
### Activity
1. Active - means is happening
2. Inactive - means is not happening
- only tells you if signal is active or not, interpretation of fault or not is based on what you make

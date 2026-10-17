## Purpose: 
##### Open the shutdown circuit if these two conditions occur for 0.5 seconds:
1. Hard braking
2. Motor current reaches >5kW during nominal battery voltage
### Requirements
1. open circuit = fault, thus open shutdown circuit within 100 msec
2. once bspd trips, shutdown circuit stays open. only can be manually reset (not from cockpit)
3. circuit must include ext test pts
## Planning:
##### idea: break it into systems until individual logic (gates, not actual ones)
gate 0 - shut down (SR latch)
	gate 0.1 - if gate 1 & 2 are active then return true (or gate)
		gate 1 - two conditions for 0.5s (maybe use capacitor + comparator)
			gate 1.1 - hard braking + high current
				gate 1.11 - braking pressure signal
					if signal is at threshold (use comparator)
						if pressure is at threshold (use pressure transducer)
				gate 1.12 - current >5kW
					node with battery power is >5kW (use comparator)
		gate 2 - is circuit open? (double comparator)
			gate 2.1 - if sensor is outputting below bottom threshold
			gate 2.2 - if sensor is outputting above top threshold
Activity 2: Component Selection





Resistors and capacitors are both 0603. (Do not go under but can go up).



Parameters:

|Input Voltage|\~10-15V|
|-|-|
|Output Voltage|5V|
|Output Current|I\_out >= 2A|
|Switching Frequency|Fsw >= 500kHz|
|Feedback Control|\~|
|Ripple Voltage|1-5%|





Check inductors' saturation current. Make sure it does not oversaturate the output current, creating an extremely large output ripple.



\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_





LV Standards:



* Signals flow left -> right and power top to bottom.
* Signal: something we can measure and transform.
* Power: drives the signal transformations.
* Bypass capacitor: low-impedance capacitor placed between an IC power pin and ground to shunt high-frequency noise away from the supply rail.
* Bypass capacitors should always be oriented vertically.
* Label wires and use them to connect signals.
* Pins must have unique names that match the datasheet and should be placed on a 100mil grid with 50mil increments. They should also faciliate signal flow with input pins on the left, and output pins on the right.
* Make sure that resistors and capacitors are rated for high enough power and voltage with 50% margin.
* Use 1% tolerance resistors.
* Have reference designators clearly visible for all components, aligned next to the component that it designates.



\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_





Buck IC selection:





Immediate check-off:

* Make sure its input voltage is 10-15V
* Output current is at least 2A
* Switching Frequency is at least 500kHz.



Typed in for voltage regulators, particularly DC DC Switching regulators and 27,000+ results came up so I tried to narrow down the search by first selecting the topology as Buck, this narrowed down the search to around 25,000 items. Next I selected precisely 500kHz for the switching frequency which brought my selection down to 910 results! I then put in for the input voltage max value at 15 Volts and that narrowed the selection down to 51 items. I then selected for in-stock only items and it narrowed my search down to 7:



LT1506CS8-3.3#PBF, LT1506IS8-3.3#PBF, LT1506CR-3.3#PBF, LT1506CS8#PBF, LT1506CR#PBF, LT1506CR-SYNC#PBF, and LT1506IR-SYNC#PBF.

However, all of these options when I looked at their data had their voltage input minimums at 4V. So their voltage ranges were all 4-15V. Although this satisfies a 15V max that we want, the minimum is not 10V so none fit what I'm looking for. Upon further inspection the price points were not great either, with many going between $18-20, being very pricey.



Thus, I made some adjustments to my search. First I selected a higher voltage input max range just to view more options. After setting my voltage input max to 18V, I got 51 options to select from instead, and looked at voltage ranges to narrow down my options.



My choice selection went down to 6: MPQ28261DL-LF-Z, ISL85003FRZ, ISL85003AFRZ, RT6224AHGJ6F, ISL85003AFRZ-T7A, and ISL85003FRZ-T7A.



From these 6, I concluded that the first one the MPQ2861DL-LF-Z is the best Buck IC to select.



The MPQ28261DL-LF-Z I chose because:

* It meets all essential requirements with a fixed frequency switching of 500kHz, its voltage output can max out at 18V meaning it can output our desired 5V, its input voltage minimum is 4.5V to 21V which gives us a good range of input voltage where it can be the 10-15V range, and its current output exceeds our minimum of 2A because the current output it has is 3A.
* The price a great range with a quantity of 1 having a unit price of $3!
* There is more than 1,000+ in stock so there is plenty of time to select and buy





\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_





Inductor selection:



When looking for an inductor I needed to make sure the inductor I select does not oversaturate at the output current I want.

In order to make my selection I went with a safer route and underneath the buck IC I chose was a list of associated products. These listing are more likely to show items that are compatible with the particular buck IC I chose, which is what I prioritize when making my selection. 

The data sheet recommends an inductance between 1-10 microH, and of the ones listed below I found some that were around 3.3 microH inductors which made for a promising choice. 15 miliOhms would make better efficiency for my board, looking at the listing options and the MPL-AL6050-3R3 inductor had a very close DC resistance of 11.5 miliOhms, making it the most promising candidate. In addition, the saturation current is 12A and the inductance is 3 microHenries. The price-point is also very reasonable at $1.73





\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_





Capacitor selection:



Immediate check-off:

* Capacitors must be 0603 or up.
* Get an output capacitor that can help filter/reduce noise.
* Input capacitor with X5R or X7R dielectrics.
* Output capacitor: give 5V output approximately, and meets the voltage ripple range between 1-5%.





To look for compatible capacitors I went back to my buck IC and looked back at the MPQ28261DL-LF-Z datasheet to find any information.

In the last few pages it had a section for selecting the input capacitor (C1), saying that the required input capacitor needs to supply the AC current to the step-down converter while maintaining the DC inout voltage and it recommends ceramic capacitors with X5R or X7R dielectrics with small temperature coefficients and low ESR. Most applications it says to use a 22microFarads.



Therefore, first thing I looked up on DigiKey is for a ceramic capacitor. Thousands of options came up but I sorted my options down to a couple hundred by selecting In-stock, 22 microFarads for capacitance, mounting type as surface mount, and from the selection that was created I found a capacitance meeting all the following criteria including having a X5R dielectric/temperature coefficient. The model for input Capacitance I selected is: CL31A226KAHNNNE.





For the output capacitor, given that the current going through the system is 3A, the datasheet provides the equation for a ceramic capacitor and I plugged in the known values to find the change in V\_out to see if its below the 1-5% margin. Going through the steps I calculated that the percentage is below by tinkering with values and a 47 microFarad output capacitor is a safe value to go with that meets all the requirements. 

On DigiKey I typed up ceramic capacitors then filtered it out by putting 47microFarads for the capacitance, in stock, and mounting type as surface mount which narrowed it down to 30 capacitors to choose from. From the 30 listed, C5750X7R1E476M230KB felt like the stronger candidate of the bunch because it exceeded 10V rated voltage which was 25V, it's capacitance is exactly what we wanted it to be at which is 47 microFarads, and the dielectric type matches what we want since its an X7R and the price point is slightly more expensive, but it has a capacitance tolerance of +- 20% and there is relatively little capacitance loss at the 5V mark looking at the datasheet for the capacitor.



\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_







Final Table:





|Component|Cost|Link|Units|Size|
|-|-|-|-|-|
|Buck IC|$3.00|https://www.digikey.com/en/products/detail/monolithic-power-systems-inc/MPQ28261DL-LF-Z/5292984|1|14-QFN (3x4mm)|
|Inductor|$1.73|https://www.digikey.com/en/products/detail/monolithic-power-systems-inc/MPL-AL6050-3R3/12418177|1|6.60x6.40mm|
|Capacitor|Input capacitor: $0.45<br /><br /><br /><br /><br /><br />Output capacitor: $4.81|https://www.digikey.com/en/products/detail/samsung-electro-mechanics/CL31A226KAHNNNE/3888705<br /><br /><br />https://www.digikey.com/en/products/detail/tdk/C5750X7R1E476M230KB/15930204|1<br /><br /><br /><br /><br /><br />1|3.20x1.60mm<br /><br /><br /><br /><br /><br />5.70x5.00mm|
|Total Cost|$9.99||||

















\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_


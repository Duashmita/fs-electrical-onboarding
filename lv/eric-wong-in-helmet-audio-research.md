# In-Helmet Audio Research

## Project goals

- Find a speaker and microphone that can make the driver's voice clear over the radio
- Find parts that can fit inside a helmet without compromising ergonomics and safety
- Find a connector that stays connected during use but can come apart quickly for emergencies
- Look into what electrical specs we would need to compare when choosing speaker parts, such as power and connections. 

## Sample Speaker
### Speaker: CSS-66668N
DigiKey part number: 102-3861-ND

### Why it might work
Its listed range is 160Hz-15kHz, which covers most of the speech range; however, it doesn't prove voices would be audible inside a noisy helmet. 

### Issues
It's listed at 66 x 66 x 29 mm. This is a headphone-style speaker, but it's still fairly large for a helmet. I'd need to check whether it fits in the helmet's ear area without pressing on the driver's ear or getting in the way of the padding.

### What to check next
The speaker is rated at 3W and 8Ω impedance. I'd need to find an amplifier that supports an 8Ω speaker and check how much power it can deliver at 8Ω. Then compare that with the speaker's 3W rating to see whether the pairing is suitable. 

## Speaker options
### Speaker: SP-1511S-1
DigiKey part number: 433-1125-ND

### Why it might work
It is much smaller than the sample speaker at 15 x 11 x 3.3 mm, so it may fit near the helmet's ear area without taking up as much space. It's also thin, so it won't press into the driver's ear as much. Its 400Hz-5kHz frequency range includes most of the range used for speech. The datasheet measures it at 83 ±3 dB SPL, using a 1V signal at 10 cm in a small test box. 

### Issues
The 83 dB rating comes from the specific test setup, so it doesn't show exactly how loud the speaker will sound inside a helmet. The datasheet also says the speaker has no IP rating, so you should consider moisture protection.

### What to check next
Check that the amplifier supports an 8Ω speaker and that its output stays within the speaker's 0.7 W rated power. Then check that the speaker fits comfortably near the driver's ear without pressing on the ear or interfering with the padding. Also test whether speech is clear in the helmet.


### Speaker: AS01508AO-WP-R
DigiKey part number: 668-1515-ND

### Why it might work
It's a similar size to the SP-1511S-1 (15 x 11 x 3mm), so it may fit near the driver's ear without taking up much space. It's rated for 8Ω and 0.7W, matching the SP-1511S-1's impedance and rated power. Its listed frequency range is 500Hz-14kHz, and its face meets IP65 requirements when properly mounted. 

### Issues
The datasheet's 94±3 dBa rating was measured at 0.7W and 10cm under specific conditions. The SP1511S-1 was tested differently, so you can't directly compare the dB ratings to prove the speaker is louder. The IP65 rating applies to the speaker's face when properly mounted, not the whole helmet setup. 

### What to check next
Check that the amplifier supports an 8Ω speaker and can supply the speaker's rated power. Also test its fit and speech clarity in the helmet. 

## Microphone options
### Microphone: CMR-4015-44-SP
DigiKey part number: 2223-CMR-4015-44-SP-ND

### Why it might work
It's a small noise-canceling electret condenser microphone. It is 4mm across and about 1.5-1.8 mm thick, so you can place it near the driver's mouth without taking up much room. Its 100 Hz-10kHz frequency range includes the range needed to pick up speech. 

### Issues
The microphone outputs an analog signal, so the system needs electronics to convert it to digital telemetry. The datasheet's example circuit uses a 2V supply and a 2.2kΩ resistor, although the mic is rated for supplies from 1-5V. The mic is labeled "noise canceling," which could help in a loud setting, but you would still need to test it inside a helmet while driving. 

### What to check next
Check how the microphone's analog signal would connect to the team's radio. Look for a comfortable way to position it near the driver's mouth, protect it from wind, and test how well it picks up speech with loud background noise.

## Packaging
I would place the speaker near the driver's ear, using an existing helmet mount or a removable mount if one is available. It should sit flat and close enough to hear clearly without pressing against the ear or blocking the speaker opening. You could mount the microphone near the driver's mouth on a small, flexible support, with a light foam cover to reduce wind noise.

The wires should follow the helmet's lining and be secured so they don't rub against the driver or get caught during entry and exit. The wires should connect to the car with a plug that stays connected while driving but disconnects quickly if the driver needs to exit. I would avoid cutting, drilling, or removing helmet padding in the protective material. The final placement would need to be checked for comfort, clear speech, and quick exit from the car. 

The microphone also needs a support circuit because it uses an analog signal. I would place the circuit in the car with the radio. The helmet would contain the microphone, speaker, and connecting wires. The microphone's analog signal would travel through the cable to the circuit, which would power the microphone and connect its audio to the radio. 

<img src="helmet-audio-block-diagram (1200 x 500 px).png" alt="In-helmet audio block diagram" width="900">

## Questions for Luca
What radio unit will the helmet connect to, and does it accept an analog microphone signal? Also, can the radio drive an 8Ω speaker, or would it need a separate amplifier?
## Sources
SP-1511S-1
https://www.digikey.com/en/products/detail/soberton-inc/SP-1511S-1/6099099?s=N4IgTCBcDaIMoAUC0BGArCldUgLoF8g

AS01508AO-WP-R
https://www.digikey.com/en/products/detail/pui-audio-inc/AS01508AO-WP-R/5958413?s=N4IgTCBcDaIIIGUAMBGArEgHHA8gWgHUAFPAJRAF0BfIA

CMR-4015-44-SP
https://www.digikey.com/en/products/detail/same-sky-formerly-cui-devices/CMR-4015-44-SP/16716043?s=N4IgTCBcDaIMIFkBKBaALABgIwFZ1pQGUAFEAXQF8g

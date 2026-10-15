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
DigiKey part number: 2223-CMR-4015-44-SP-ND

### Why it might work
It is much smaller than the sample speaker at 15 x 11 x 3.3 mm, so it may fit near the helmet's ear area without taking up as much space. It's also thin, so it won't press into the driver's ear as much. Its 400Hz-5kHz frequency range includes most of the range used for speech. The datasheet measures it at 83 ±3 dB SPL, using a 1V signal at 10 cm in a small test box. 

### Issues
The 83 dB rating comes from the specific test setup, so it doesn't show exactly how loud the speaker will sound inside a helmet. The datasheet also says the speaker has no IP rating, so you should consider moisture protection.

### What to check next
Check that the amplifier supports an 8Ω speaker and that its output stays within the speaker's 0.7 W rated power. Then check that the speaker fits comfortably near the driver's ear without pressing on the ear or interfering with the padding. Also test whether speech is clear in the helmet.

## Microphone options
### Microphone: CMR-4015-44-SP
DigiKey part number: 2223-CMR-4015-44-SP-ND

### Why it might work
It's a small noise-canceling electret condenser microphone. It is 4mm across and about 1.5-1.8 mm thick, so you can place it near the driver's mouth without taking up much room. Its 100 Hz-10kHz frequency range includes the range needed to pick up speech. 

### Issues
The microphone outputs an analog signal, so the system needs electronics to convert it to digital telemetry. The datasheet's example circuit uses a 2V supply and a 2.2kΩ resistor, although the mic is rated for supplies from 1-5V. The mic is labeled "noise canceling," which could help in a loud setting, but it would still need to be tested inside a helmet while driving. 

### What to check next
Check how the microphone's analog signal would connect ot he team's telemetry electronics. Look for a comfortable way to position it near the driver's mouth, protect it from wind, and test how well it picks up speech with loud background noise.

## Connector options

## Questions for Luca

## Sources

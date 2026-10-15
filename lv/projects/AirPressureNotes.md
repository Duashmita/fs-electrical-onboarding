Sensor example: https://www.digikey.com/en/products/detail/bosch-sensortec/BMP180/2688259

# Specifications (from DigiKey and datasheet from Bosch)
    - BMP180
    - Pressure range: 4.35 - 15.95PSI
    - Supply voltage: 1.8V - 3.6V (VDD), 1.62V - 3.6V (VDDIO)
    - Dimensions: 3.6mm x 3.8mm x 0.93mm
    - Two wire I2C interface
    - Board mount
    - Absolute pressure type
    - Accuracy: +-2%
    - Termination style: SMD (SMT) tab

# BMP 180 Considerations
    Pros
    - MEMS (micro-electro mechanical system), used to measure pressure
    - Commonly used with Arduinos, so there is a lot of support software-wise
    - I2C compatibility
    Cons
    - Out of production, difficult to source
    - "Decent" (mediocre?) resolution
    - Difficult to interface multiple units with a single microcontroller due to a lack of address pins for I2C

    Goals
    - Resolution equal to or better than the original
    - Communication or output interface that allows it to be banked with more pressure sensors
    - Adequete software support
    - Still in stock and manufactured
    - Absolute pressure type, board mount to mirror previous application capacity and ability to be connected in an array

# Communication notes
- I2C is the communication method from the exmaple, which has the benefit of being a bus-based system, but requires extra pins to set addresses. Without addresses, the maximum amount of pressure sensors on the same bus is two, and that's only if one of the address bits can be changed (which is not always the case)
- SPI is another common form of the communication, and has the benefit of having no theoretical limit (no set clockspeed, depends on the sensors and relevant electronics used). Unfortunatly it does not have a bus system that uses addresses, preventing the straightforward implementation of multiple sensors without using a significant amount of pins.
- Analog sensors send continuous data and lack the capacity for bidirectional communication required of bus-based implementation. While there are very technical ways to connect multiple of them in series, it would likely compromise the integrity of the communication and would instead requiring a copious amount of GPIO on the microcontroller to have an array of analog sensors.
- OneWire is another solution that provides a means for communicating with multiple devices on the same communication line. It has an incredibly high capacity for multiple devices in the same network, but with the pretty significant downside of not being a commmon native output interface, making it difficult to find an applicable sensor within specification.
    - One possible sub-solution would be to have communication intermediaries like the DS2482S-800, which is a micro-board that translates between OneWire and I2C, providing 8 unique sensor connections to a single I2C bus. The downside is the added point of failure and complexity of translating between outputs, alongside the presence of an additional board.
- SENT is the common sensor communication method for vehicles which is a significant positive considering the nature of Formula Slug, however, its focus is on data resolution and has no support for multiple sensors on a single communication line. 

# Candidates
Following filters:
- Pressure type: Absolute
- Accuracy: >=2%
- Applications: Board mount
- Product status: Active

MS561101BA03-50
- Initially promising because of its significantly better pressure range, compact size, and accuracy of the sensor
- It also utilized SMD tab termination and the same voltage supply range, allowing its integration to be similar to its predecessor 
- While it does have I2C support, it does not have the capacity to change addresses due to a lack of the relevant pins (according to its datasheet, it can only be used with other sensors on the same bus if they have different addresses, which would not be the case for duplicate sensors)
- It does have SPI communication, although despite its higher pin count, does not natively support address-based communication and would require more GPIO from the microcontroller to support each additional sensor
- https://www.digikey.com/en/products/detail/te-connectivity-measurement-specialties/MS561101BA03-50/5277445

BMP390
- A successor to the BMP180, it shares the same implementation style and a moderately improved
- While it is still actively produced, it was out of stock as of 10/12. May or may not be an issue
- It has both I2C and SPI communication, the former of which has a total of... two possible addresses, depending on the SDO connection, making it not ideal for an array of pressure sensors
- https://www.digikey.com/en/products/detail/bosch-sensortec/BMP390/16164575

Following these failures, I went back to research I2C and found some interesting information. While I2C does have support for custom addresses in the form of address setting pins (A0-2 typically), most pressure sensors are in fact too small to have this be an option. Instead, they have what I observed with the BMP390, which is having the ability to change a single bit in the address, allowing for two of the same sensor in the same I2C bus. From this approach, comes a few intruiging options:

- Have multiple buses, each one with two sensors. The upside is flexibility with sensor choices, although the rather severe downside of requiring one pin for each sensor (although it is two sensors per bus, as far as I understand you would still need two pins per bus, resulting in 1:1 pin to sensor ratio for the microcontroller). This is also likely a hassle on the software side
- Use multiple different sensor with their own addresses all on the I2C bus. While technically possible, processing the data from each one would be nightmareish as they would have their own respective ranges and accuracies, introducing too much variability to the end result of the information
- I2C multiplixers are something that came up during this research, which suggested the possibility of having a secondary board connected to the microcontroller that would allow for multiple I2C sensors to be connected in the same bus, they would just need to be wired to the multiplixer, and the multiplexer would then be wired to the original microcontroller. The benefit is a significantly larger array of the same sensor on the same bus, but with the downside that there is now an additional board that needs to be accounted for in the layout of wherever the microcontroller is located. The most popular example is the TCA9548A (https://www.digikey.com/en/products/detail/adafruit-industries-llc/2717/5604376), which has 8 interfacing channels. These channels would then allow for 8 sensors with the same address to be connected to one microcontroller. With two-addresssolutions like the aforementioned BMP390, this opens up the possibility for 16 individual pressure sensors on the same I2C bus, which can be further expanded by either having multiple I2C buses or wiring multiplexers in series

HSPPAD143C
- Building off of the theory that I2C is viable with the introduction of multiplexing, this sensor is a promising candidate for several reasons
- Aside from the benefits and drawbacks of sticking to I2C, the pure specifications make it ideal including a 0.2% accuracy rating for an increased operating pressure range (4.35 - 29.01PSI range)
- Similar connection design makes it ideal for implementation, given its surface board mount design
- https://www.digikey.com/en/products/detail/alps-alpine/HSPPAD143C/21721511

# Considering Luca's Feedback
- "With spi all devices share sck,pico,poci and you just need one extra cs per device"
    - sck, pico, and poci are all SPI pins that are shared, with PICO and POCI being the bidirection communication method for devices
        - PICO: Peripheral in / controller out
        - POCI: Peripheral out / controller in
        - sck is just clock
    - CS, or chip select is what needs to be duplicated, as it's the pin that determines which peripheral device (each sensor) is receiving from PICO
        - As Luca said, it's only one additional CS pin per device as each CS pin is held high until the peripheral needs to be addressed, in which case it's set to low
    - https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi/chip-select-cs - Source for SPI pinout information, alongside a few other articles for clarification
    - "Which is feasible with a demultiplexer or without depending on the number of sensors"
        - Makes sense as this would provide as a means to interact with more devices by controlling a larger set of CS pins (thanks CSE-12 for making this easy to understand)
- "With i2c you can also use an i2c controlled switch in a tree or linear"
    - As far I as understand, it's another approach to creating multiple I2C buses, like the aforementioned multiplexing approach I proposed. The MUX allows for using multiple I2C buses at the same time, but the switcher is preferable in terms of variable voltage delivery. That being said, variable voltage shouldn't be required if I'm using the same kind of sensor along all the buses as they would all have the same voltage draw.
    - Tree versus linear is how the different switches or MUXs are connected, in this case I imagine tree would work well with MUX implementation for ease of implementation (less switch controlling design I think)
- "There are also address shifters I think"
    - This is something that I actually considered but omitted because as far as I understand, it would require a second board for each and every sensor before interfacing with the I2C bus in order to appropriately configure the bus. I consider 
- "So combining a sensor with address pins and a switch or address shifters is viable as well"
    - I couldn't find a viable sensor with address pins based on the specifications of the original, so out of all of these I think the multiplexer or switch would provide the most viable connection scheme for I2C implementation

# Final Selection
Given the additional information and previous considerations, the ideal implementation would include either I2C or SPI integration in order to have as many sensors as possible, as well as a high quality and range to the data being recorded.

While the BMP390 is an excellent candidate and is still being produced, the fact that it continues to be out of stock at DigiKey hints at the fact that is still being produced may not be accurate or up to date. Instead, the following sensor provides the most flexibility in that it offers both SPI and I2C as integration options, allowing for flexibility with how it is implemented.

PSD0603130
- A high quality sensor with an accuracy of +-0.35% (as opposed to the original 2%), its pressure range on the low end is the same but is still functional in high pressure environment, something which may be useful for racing applications. What makes it stand out is the aforementioned dual support for I2C and SPI, meaning that it can be initially implemented with SPI, but can be switched over to I2C if more GPIO pins are needed for other applications or if the amount of sensors required exceeds the amount of GPIO pins required (also worth noting that demultiplexing can also expand SPI sensor count, but just like I2C multiplexing it requires an additional component).
- https://www.digikey.com/en/products/detail/pui-audio-inc/PSD0603130/21778456

# Conclusion
With all of that being said, all of the aforementioned components can be appropriately utilized as they fall within spec and have a means for interconnectivity for arrays of pressure sensors without using too many GPIO pins. It is worth noting that I didn't seriously consider price as that was not a part of the original specifications, as well as the fact that almost all pressure sensors on DigiKey that fall within spec cost between $5-10.

Final ranking:
1. PSD0603130
    - Most flexible with I2C and SPI support
    - Still in stock
2. MS561101BA03-50
    - SPI support means it can be implemented using either one additional GPIO pin per sensor or the inclusion of a demultiplexer for having significantly more sensors
3. HSPPAD143C
    - Ranked below the SPI option because although a very high amount of I2C sensors can be implemented using multiplexers and switchers in either a tree or linear setup, the fact that a smaller use case (with no additional hardware) would require two pins per sensor across separate buses makes it less flexible
4. BMP390
    - While I imagine this is the most easily implemented as it follows very similar design considerations to the BMP180, just with better specs, the fact that it remains out of stock throughout the duration of this research project makes it a dubious choice
    - Otherwise, its dual support of I2C and SPI make it a great choice comperable to the current first choice
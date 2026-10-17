## Part 1: Air pressure and humidity sensors

Similar sensors to the BME 180:

1. Link to BME 280:  https://www.digikey.com/en/products/detail/bosch-sensortec/BME280/6136306

Source DigiKey
Protocols: I2C, SPI
Accuracy: ±3%
Humidity Range: 0 to 100% Realtive Humidity
Temperature Range: -40°C to 85°C

-> the sensor, BME 280 is also attached to the esp32c3 for temperature and humidity sensors.
-> I2C uses SDA and SCL and GND
-> used in humidifiers, dehumidifiers, and IoT environments

2. Link to BMP280: https://www.digikey.com/en/products/detail/bosch-sensortec/BMP280/6136307

 
Source: DigiKey
Protocols: I2C, SPI
Accuracy for Pressure: ±0.015PSI (±0.1kPa)
Pressure: 4.35PSI ~ 15.95PSI (30kPa ~ 110kPa)
Temperature: -40°C to 85°C

-> used to monitor weather
-> calculate changes in velocity for GPS tracking

3. 	DHT20 - AHT20 PIN MODULE - I2C T
Protocols: I2C
Humidity: 0 to 100% Realitive Humidity
Accuracy: +3% or - 3% Realitve Humidity
Temperature: -40°C to 80°

Link to DHT20 - AHT20 PIN MODULE -> https://www.digikey.com/en/products/detail/adafruit-industries-llc/5183/15204087

-> can work well with the esp32c3, arduino, and rasberry pi. 
->Can be used to create to monitor temperature and humidity for an EV and can track weather for an IOT weather station.

Aerodynamic Picture

Range: From -2.50e^2 Pascals  to 2.50*10^2 Pascals so it around 500 Pascals

Resolution: 500/50 = 10 
 - Each interval is around 50 Pascals so I did 500/50 = 10

Precision: 1% 
- The precision should be 1% because it should be as accurate as possible. 






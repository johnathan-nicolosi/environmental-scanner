# environmental-scanner
Arduino sketch for a DIY environmental sensor array

This comprehensive scanner uses an Arduino Uno, Ethernet Shield (connected to your local network), light sensor, temp and humidity sensor, water sensor, gas sensor, smoke sensor and magnetic field sensor and displays the sensor data to a website, which can be accessed from any phone, tablet or computer connected to the same network as the Arduino. A piezo (active) buzzer is used as a built-in alarm, which will sound under certain conditions.

This sketch was based on the WebServer sketch, created by David Mellis and modified by Tom Igoe and Arturo Guadalupi, so a special thanks goes out to them. You will also need the following libraries installed: SPI (serialperipheral interface) library, Ethernet library, Adafruit Sensor library, and DHT sensor library.

<h1> Environmental Scanner</h1>
This is the code repository for an environmentally aware robot, that uses an Arduino Uno to sense the environment using a variety of analog and digital sensors. This data is then sent to a Raspberry Pi, using the serial (USB) connection.

<h3>Arduino sketch for a DIY environmental sensor array</h3>

This comprehensive scanner uses an Arduino Uno, light sensor, water sensor, gas sensor, smoke sensor and hall sensor and to detect light, water, gas, smoke and electromagnetic fields and then sends an alert over USB to a Raspberry Pi. Currently the Raspberry Pi will only display the current status of each sensor, but future releases will allow the Raspberry Pi to use this information to make intelligent decisions based on each sensor.

Upload the arduino-raspberry.iso to your Arduino Uno. You will need the following sensors for this project:
<ul>
  <li>Hall Sensor (KY-003)</li>
  <li>Gas and Alcohol Sensor (MQ3)</li>
  <li>Light Sensor (KY-018)</li>
  <li>Smoke Sensor (MQ2)</li>
  <li>Water Level Sensor</li>
</ul>

Upload the raspberry-arduino.py script to your Raspberry Pi. In your Raspberry Pi interface, be sure to enable Serial and I2C in PiConfig. Restart your Raspberry Pi and execute the following commands:

sudo apt-get install python-serial
sudo pip install pyserial

Next, connect your Arduino to your Raspberry Pi then execute:

ls /dev/tty*

Look for a line with /dev/ttyACMO or something similar (an ACM with any number 0, 1, 2, etc.).

Open the raspberry-arduino.py script and update the <strong>ser=serial.Serial("dev/ttyACM0",9600)</strong> to the ACM number you found. Next, run the raspberry-arduino.py script in Pyhton3. You will see a the status of each sensor in your Python terminal.

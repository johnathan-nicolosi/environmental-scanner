
/*
 Arduino Environmental Scanner

 A modified web server that displays the results of an array of environmental sensors
 using an Arduino Uno and Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 25 June 2016
 by Johnathan Nicolosi
 */

#include <SPI.h>
#include <Ethernet.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//DHT sensor output for Arduino digital D2 pin 
#define DHTPIN 2 

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

const int smokePin = A0; //Smoke sensor (smoke or flammable gas concentration level) output pin to Arduino analog A0 pin
const int lightPin = A1; //Light sensor output pin to Arduino analog A1 pin
const int gasPin = A2; // MQ-3 Alcohol Gas sensor (LPG, i-butane, methane, alcohol, Hydrogen, smoke, etc.) output to Arduino analog A2 pin
const int hallSensor = A3; //Digital Hall sensor to detect magnetic fields
int sensorValue = 0; //define numeric variables val
const int waterSensor = A4; //Water sensor on Arduino analog A4 pin
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 138); //Change IP to one available on your local network

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

// for the alarm buzzer
int piezoPin = 3;

// for communicating with Raspberry
int tempOut = 4;
int smokeOut = 5;
int waterOut = 6;
int magnetOut = 7;


void setup() {
  // Send digital signal to raspberry GPIO Pins
  pinMode(tempOut, OUTPUT);
  pinMode(smokeOut, OUTPUT);
  pinMode(waterOut, OUTPUT);
  pinMode(magnetOut, OUTPUT);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
   // wait for serial port to connect. Needed for native USB port only
  }
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  //the rest of setup is dedicated to the environmental sensors
  Serial.println("Scanning environmental variables....");
  delay(2000);
  Serial.println("scan in progress...please wait!");
  delay(2000);
  dht.begin();
}

void loop() { 
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
      
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
        
          // Output for environmental sensors
          // Wait a one or more seconds between measurements.
          // delay(1000); //print values every 1 second
          // Reading temperature or humidity takes about 250 milliseconds!
          // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
          float h = dht.readHumidity();
          // Read temperature as Celsius (the default)
          float t = dht.readTemperature();
          // Read temperature as Fahrenheit (isFahrenheit = true)
          float f = dht.readTemperature(true);
          //convert temp to kelvin
          float k = t+273.15;
         
          // Check if any reads failed and exit early (to try again).
          if (isnan(h) || isnan(t) || isnan(f)) {
            client.println("Failed to read from DHT sensor!");
            return;
          }
          

          // Compute heat index in Fahrenheit (the default)
          float hif = dht.computeHeatIndex(f, h);
          // Compute heat index in Celsius (isFahreheit = false)
          float hic = dht.computeHeatIndex(t, h, false);
          
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<title>Environmental Scanner</title>");
          client.println("<body>");
          client.println("<center>");
          
          //Alert if tempterature is too high
          if (dht.readTemperature(f) > 85){
          client.println("<h2 style=\"");
          client.println("color: #ff0000;\"");
          client.println(">ALERT: High Temperature!</h2>");     
          tone(piezoPin, 1000, 900);   
          digitalWrite(tempOut, HIGH);   // send signal to raspberry (HIGH is the voltage level)
            
          }
          //Alert if tempterature is too low
          if (dht.readTemperature(f) < 45){
          client.println("<h2 style=\"");
          client.println("color: #ff0000;\"");
          client.println(">ALERT: Low Temperature!</h2>");      
          tone(piezoPin, 1000, 900); 
          digitalWrite(tempOut, HIGH);   // send signal to raspberry (HIGH is the voltage level)        
          }
                             
          //Alert if smoke is detected
          if (analogRead(smokePin) > 480){
          client.println("<h2 style=\"");
          client.println("color: #ff0000;\"");
          client.println(">ALERT: Smoke Detected!</h2>");
          }        
          
          //Alert if smoke is detected
          if (analogRead(smokePin) > 550){
          /*tone(piezoPin, 1000, 900);   */
          digitalWrite(smokeOut, HIGH);   // send signal to raspberry (HIGH is the voltage level)           
          }      
          
           //Alert if gas is detected
          if (analogRead(gasPin) > 300){
          client.println("<h2 style=\"");
          client.println("color: #ff0000;\"");
          client.println(">ALERT: Gas Detected!</h2>");   
          /*tone(piezoPin, 1000, 900);       */
          digitalWrite(smokeOut, HIGH);   // send signal to raspberry (HIGH is the voltage level)         
          
          }

          //Alert if light is detected
          if (analogRead(lightPin) < 995){
          client.println("<h2 style=\"");
          client.println("color: #ff0000;\"");
          client.println(">ALERT: Light Detected!</h2>");     
          }
          //else {
          //client.println("<h2>No Light Detected</h2>");
          //}

          //Alert if magnetic field is detected
          sensorValue = analogRead(hallSensor); //read sensor line
          if (sensorValue > 499) //when the Hall sensor detects a magnetic field
          {
          client.println("<h2 style=\"");
          client.println("color: #ff0000;\"");
          client.println(">ALERT: Magnetic Field Detected!</h2>");
          digitalWrite(magnetOut, HIGH);   // send signal to raspberry (HIGH is the voltage level)

          }
          //Alert if magnetic field is detected
          sensorValue = analogRead(hallSensor); //read sensor line
          if (sensorValue < 495) //when the Hall sensor detects a magnetic field
          {
          client.println("<h2 style=\"");
          client.println("color: #ff0000;\"");
          client.println(">ALERT: Magnetic Field Detected!</h2>");
          digitalWrite(magnetOut, HIGH);   // send signal to raspberry (HIGH is the voltage level)
          }
          else{
          digitalWrite(magnetOut, LOW);
          }
          
          //Alert if water is detected
          if (analogRead(waterSensor) > 10){
          client.println("<h2 style=\"");
          client.println("color: #ff0000;\"");
          client.println(">ALERT: Water Detected!</h2>");  
          /*tone(piezoPin, 1000, 900);  */
          digitalWrite(waterOut, HIGH);   // send signal to raspberry (HIGH is the voltage level)          
          }         
          client.println("</center>");
          client.println("<hr /><br />");
          client.println("<h3>CURRENT CONDITIONS</h3>");

          //display results of sensors to webserver 
          client.println("<p>");
          client.print(" Humidity: ");
          client.print(h);
          client.print(" % ");
          client.println("<br />");
          client.print(" Temp: ");
          client.print(t);
          client.print(" C; ");
          client.print(f);
          client.print(" F; ");
          client.print(k);
          client.print(" K ");
          client.println("<br />");
          client.print(" Heat Index: ");
          client.print(hic);
          client.print(" *C; ");
          client.print(hif);
          client.print(" *F ");
          client.println("</p>");
          client.println("<hr />");
          client.println("<br />");
          client.println("<h3>SENSOR DATA</h3>");
          client.println("<p>");
          client.println("  Smoke Level: ");
          client.print(analogRead(smokePin));
          client.println("<br />");
          client.print("  Gas Level: ");
          client.println(analogRead(gasPin));
          client.println("<br />");         
          client.print("  Luminance (Light < 995 < Dark): ");
          client.print(analogRead(lightPin));
          client.println("<br />");
          client.print("  Magnetic Field: ");
          client.println(analogRead(hallSensor));
          client.println("<br />");
          client.print("  Moisture: ");
          client.println(analogRead(waterSensor));
          client.print("</p>");
          client.println("<br />");
          client.println("</body>");
          client.println("</html>");
          break;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}


const int smokePin = A0; //Smoke sensor (smoke or flammable gas concentration level) output pin to Arduino analog A0 pin
const int lightPin = A1; //Light sensor output pin to Arduino analog A2 pin
const int gasPin = A2; // MQ-3 Alcohol Gas sensor (LPG, i-butane, methane, alcohol, Hydrogen, smoke, etc.) output to Arduino analog A2 pin
const int hallSensor = A3; //Digital Hall sensor to detect magnetic fields
const int waterSensor = A4; //Water sensor on Arduino analog A4 pin

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (analogRead(smokePin) > 500){
    Serial.println("Alert: Smoke Detected!");
  }
  else{
    Serial.println("No Smoke Detected");
  }

  if (analogRead(lightPin) < 995){
    Serial.println("Alert: Light Detected!");
  }
  else{
    Serial.println("No Light Detected");
  }

  if (analogRead(gasPin) > 400){
    Serial.println("Alert: Gas Detected!");
  }
  else{
    Serial.println("No Gas Detected");
  }

  if (analogRead(hallSensor) > 499){
    Serial.println("Alert: Magnetic Field Detected!");
  }
  else{
    Serial.println("No Magnetic Field Detected");
  }

  if (analogRead(waterSensor) > 20){
    Serial.println("Alert: Water Detected!");
  }
  else{
    Serial.println("No Water Detected");
  }

  delay(1000);
}

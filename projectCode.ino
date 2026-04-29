#include "Sensor.h"

// Solenoid Pin Definitions
const int pinPressure = 6; 
const int pinVacuum   = 4; 

// Sensors
Sensor tap(A0);
Sensor flexy1(A1);
Sensor flexy2(A2);
Sensor flexy3(A3);

// Status variables
double pPower = 0.0; 
double vPower = 0.0; 
String currentAction = "HOLD";

// Timer variables for high responsiveness
unsigned long lastSendTime = 0;
const int sendInterval = 50; // Send data every 50ms

void setup() {
  pinMode(pinPressure, OUTPUT);
  pinMode(pinVacuum, OUTPUT);
  
  digitalWrite(pinPressure, LOW);
  digitalWrite(pinVacuum, LOW);
  
  Serial.begin(9600);
}

void loop() { 
  // 1. LISTEN for commands (Immediate priority)
  if (Serial.available() > 0) {
    char cmd = Serial.read(); 

    switch (cmd) {
      case 'O': // FILL
        digitalWrite(pinPressure, HIGH);
        digitalWrite(pinVacuum, LOW);
        pPower = 1.0; vPower = 0.0;
        currentAction = "FILL";
        break;

      case 'V': // VACUUM
        digitalWrite(pinPressure, LOW);
        digitalWrite(pinVacuum, HIGH);
        pPower = 0.0; vPower = 1.0;
        currentAction = "VACUUM";
        break;

      case 'S': // HOLD
        digitalWrite(pinPressure, LOW);
        digitalWrite(pinVacuum, LOW);
        pPower = 0.0; vPower = 0.0;
        currentAction = "HOLD";
        break;
    }
  }

  // 2. BROADCAST data (Timer-based, doesn't clog the buffer)
  if (millis() - lastSendTime >= sendInterval) {
    Serial.print(tap.read());
    Serial.print(",");
    Serial.print(flexy1.read());
    Serial.print(",");
    Serial.print(flexy2.read());
    Serial.print(",");
    Serial.print(flexy3.read());
    Serial.print(",");
    Serial.print(pPower);
    Serial.print(",");
    Serial.print(vPower);
    Serial.print(",");
    Serial.print(currentAction);
    Serial.println(","); // Ending with comma per your format
    
    lastSendTime = millis();
  }
}
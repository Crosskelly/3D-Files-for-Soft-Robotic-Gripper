// Sensor methods 
#include "Sensor.h"

Sensor::Sensor(int pin) { 
  readings[10];
  readIndex = 0;
  total =0;
  average = 0;
  sensorPin   = pin;
  for (int thisReading = 0; thisReading < 10; thisReading++){
    readings[thisReading]=0;
  }
  pinMode(pin, INPUT);
}

int Sensor::read(){
  // // subtract the last reading:
  // total = total - readings[readIndex];
  // // read from the sensor:
  // readings[readIndex] = analogRead(sensorPin);
  // // add the reading to the total:
  // total = total + readings[readIndex];
  // // advance to the next position in the array:
  // readIndex = readIndex + 1;

  // // if we're at the end of the array...
  // if (readIndex >= 10) {
  //   // ...wrap around to the beginning:
  //   readIndex = 0;
  // }

  // // calculate the average:
  // average = total / 10;
  return analogRead(sensorPin);
}

String Sensor::advancedRead(){
  int i = read();
  if (i<1000 && i>900){
    Serial.println("Light Press");
  }
  else if (i<900 && i>800){
    Serial.println("Medium Press");
  }
  else if (i<800){
    Serial.println("Hard Press");
  }
  else{
      Serial.println("No contact");
  }
}


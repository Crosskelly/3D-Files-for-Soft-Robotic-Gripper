// Sensor main class

#ifndef Sensor_h
#define Sensor_h

#include <Arduino.h>
// const int numReadings = 10; 
// int readings[numReadings];
// int readIndex = 0;
// int total =0;
// int average = 0;

class Sensor {
  private:
    int sensorPin; 
    int readings[10];
    int readIndex;
    int total;
    int average;

  public:
    Sensor(int pin);
    int read();
    String advancedRead();
    //methods here 
};

#endif
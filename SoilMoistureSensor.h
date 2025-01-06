// SoilMoistureSensor.h
#ifndef SOILMOISTURESENSOR_H
#define SOILMOISTURESENSOR_H

#include <Arduino.h>

class SoilMoistureSensor {
private:
    int pin;

public:
    SoilMoistureSensor(int pin);
    void setup();
    float readMoisturePercentage();
};

#endif

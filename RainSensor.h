// RainSensor.h
#ifndef RAINSENSOR_H
#define RAINSENSOR_H

#include <Arduino.h>

class RainSensor {
private:
    int pin;

public:
    RainSensor(int pin);
    void setup();
    float readRainPercentage();
};

#endif

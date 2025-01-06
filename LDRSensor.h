// LDRSensor.h
#ifndef LDRSENSOR_H
#define LDRSENSOR_H

#include <Arduino.h>

class LDRSensor {
private:
    int pin;

public:
    LDRSensor(int pin);
    void setup();
    bool isLightDetected();
};

#endif

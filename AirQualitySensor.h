// AirQualitySensor.h
#ifndef AIRQUALITYSENSOR_H
#define AIRQUALITYSENSOR_H

#include <Arduino.h>

class AirQualitySensor {
private:
    int pin;

public:
    AirQualitySensor(int pin);
    void setup();
    int readAirQuality();
};

#endif

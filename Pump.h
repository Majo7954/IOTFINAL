#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>

class Pump {
private:
    int pin;

public:
    Pump(int pin);    // Constructor
    void setup();     // Configura el pin de la bomba
    void turnOn();    // Enciende la bomba
    void turnOff();   // Apaga la bomba
};

#endif

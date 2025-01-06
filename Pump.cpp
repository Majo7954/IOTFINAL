#include "Pump.h"

Pump::Pump(int pin) : pin(pin) {}

void Pump::setup() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW); // Asegura que esté apagada inicialmente
}

void Pump::turnOn() {
    digitalWrite(pin, HIGH);
}

void Pump::turnOff() {
    digitalWrite(pin, LOW);
}

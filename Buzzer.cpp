#include "Buzzer.h"
#include <Arduino.h>

Buzzer::Buzzer(int pin)
    : pin(pin), isActive(false), frequency(2000) {}

void Buzzer::init() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Buzzer::setFrequency(int freq) {
    frequency = freq;
}

void Buzzer::turnOn() {
    if (!isActive) {
        isActive = true;
        tone(pin, frequency);
    }
}

void Buzzer::turnOff() {
    if (isActive) {
        isActive = false;
        noTone(pin);
    }
}

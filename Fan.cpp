#include "Fan.h"
#include <Arduino.h>

Fan::Fan(int pin1, int pin2) : pin1(pin1), pin2(pin2) {}

void Fan::init() {
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    Serial.println("[Fan] Initialized.");
}

void Fan::turnOn() {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
    Serial.println("[Fan] Turned ON.");
}

void Fan::turnOff() {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    Serial.println("[Fan] Turned OFF.");
}

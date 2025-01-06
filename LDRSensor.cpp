// LDRSensor.cpp
#include "LDRSensor.h"

LDRSensor::LDRSensor(int pin) : pin(pin) {}

void LDRSensor::setup() {
    pinMode(pin, INPUT);
    Serial.print("LDRSensor setup completed on pin: ");
    Serial.println(pin);
}

bool LDRSensor::isLightDetected() {
    int state = digitalRead(pin);
    Serial.print("LDRSensor state: ");
    Serial.println(state == LOW ? "Light detected" : "Darkness detected");
    return state == LOW;
}

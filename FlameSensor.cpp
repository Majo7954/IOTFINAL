#include "FlameSensor.h"
#include <Arduino.h>

FlameSensor::FlameSensor(int pin) : pin(pin), detected(false) {}

void FlameSensor::init() {
    pinMode(pin, INPUT);
    Serial.println("[FlameSensor] Initialized.");
}

void FlameSensor::read() {
    detected = digitalRead(pin) == LOW;
    Serial.print("[FlameSensor] Flame detected: ");
    Serial.println(detected ? "YES" : "NO");
}

bool FlameSensor::isFlameDetected() const {
    return detected;
}

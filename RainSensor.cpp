// RainSensor.cpp
#include "RainSensor.h"

RainSensor::RainSensor(int pin) : pin(pin) {}

void RainSensor::setup() {
    pinMode(pin, INPUT);
    Serial.print("RainSensor setup completed on pin: ");
    Serial.println(pin);
}

float RainSensor::readRainPercentage() {
    int value = analogRead(pin);
    float percentage = map(value, 3500, 2500, 0, 100);
    percentage = constrain(percentage, 0, 100);
    Serial.print("RainSensor reading: ");
    Serial.print(value);
    Serial.print(" - Rain Level: ");
    Serial.print(percentage);
    Serial.println("%");
    return percentage;
}

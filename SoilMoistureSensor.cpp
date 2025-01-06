// SoilMoistureSensor.cpp
#include "SoilMoistureSensor.h"

SoilMoistureSensor::SoilMoistureSensor(int pin) : pin(pin) {}

void SoilMoistureSensor::setup() {
    pinMode(pin, INPUT);
    Serial.print("SoilMoistureSensor setup completed on pin: ");
    Serial.println(pin);
}

float SoilMoistureSensor::readMoisturePercentage() {
    int value = analogRead(pin);
    float percentage = map(value, 3410, 400, 0, 100);
    percentage = constrain(percentage, 0, 100);
    Serial.print("SoilMoistureSensor reading: ");
    Serial.print(value);
    Serial.print(" - Moisture: ");
    Serial.print(percentage);
    Serial.println("%");
    return percentage;
}

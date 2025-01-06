#include "AirQualitySensor.h"

AirQualitySensor::AirQualitySensor(int pin) : pin(pin) {}

void AirQualitySensor::setup() {
    pinMode(pin, INPUT);  // Configura el pin como entrada
    Serial.print("AirQualitySensor setup completed on pin: ");
    Serial.println(pin);
}

int AirQualitySensor::readAirQuality() {
  int sensorValue = analogRead(pin);  // Read the analog value from the sensor
  delayMicroseconds(10); // Optional delay for stable reading
  Serial.print("AirQualitySensor reading: ");
  Serial.println(sensorValue);  // Show the analog value
  return sensorValue;  // Return the analog value
}

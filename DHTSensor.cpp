#include "DHTSensor.h"
#include <Arduino.h>

DHTSensor::DHTSensor(int pin, int type) 
    : pin(pin), dht(pin, type), temperature(0), humidity(0) {}

void DHTSensor::init() {
    dht.begin();
    Serial.println("[DHTSensor] Initialized.");
}

void DHTSensor::read() {
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("[DHTSensor] Error reading data!");
        humidity = 0;
        temperature = 0;
    } else {
        Serial.print("[DHTSensor] Temperature: ");
        Serial.print(temperature);
        Serial.print("°C, Humidity: ");
        Serial.print(humidity);
        Serial.println("%");
    }
}

float DHTSensor::getTemperature() const {
    return temperature;
}

float DHTSensor::getHumidity() const {
    return humidity;
}

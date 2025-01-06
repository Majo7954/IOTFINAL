#ifndef GREENHOUSE_H
#define GREENHOUSE_H

#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "RainSensor.h"
#include "LDRSensor.h"
#include "AirQualitySensor.h"
#include "SoilMoistureSensor.h"
#include "DHTSensor.h"
#include "FlameSensor.h"
#include "Pump.h"
#include "Fan.h"
#include "ServoMotor.h"
#include "LED.h"
#include "Buzzer.h"
#include "WiFiManager.h"

class Greenhouse {
public:
    // Constructor que acepta cliente WiFi y credenciales
    Greenhouse(WiFiClientSecure* client, const char* ssid, const char* password);

    void setup();
    void loop();
    void configureMQTT(const char* broker, int port, const char* clientId, const char* updateTopic, const char* deltaTopic);

    bool isConnected();
    void reconnect(const char* clientId);

private:
    // Gestión de WiFi y MQTT
    WiFiManager wifiManager;  // Ahora recibe credenciales desde el constructor
    WiFiClientSecure* client;
    PubSubClient* mqttClient;

    // Sensores
    RainSensor rainSensor;
    LDRSensor ldrSensor;
    AirQualitySensor airQualitySensor;
    SoilMoistureSensor soilMoistureSensor;
    DHTSensor dhtSensor;
    FlameSensor flameSensor;

    // Actuadores
    Pump pump;
    Fan fan;
    ServoMotor servo;
    LED yellowLED1, yellowLED2, redLED;
    Buzzer buzzer;

    // MQTT Topics
    String updateTopic;
    String deltaTopic;

    // Variables de sensores y estado
    float soilMoisture;
    float temperature;
    float humidity;
    float rainPercentage;
    bool lightDetected;
    bool pumpState;
    bool fanState;
    bool yellowLEDsState;
    String servoState;

    // Valores anteriores de sensores
    float lastSoilMoisture;
    float lastTemperature;
    float lastHumidity;

    // Temporizador para leer DHT
    unsigned long lastDHTReadTime;
    const unsigned long dhtInterval = 2000;

    // Métodos auxiliares
    void checkSensors();
    void controlActuators();
    void publishShadow();
    void handleDelta(const char* payload);

    // Handlers para actuadores
    void handlePumpState(bool state);
    void handleFanState(bool state);
    void handleServoState(const String& state);
    void handleYellowLEDsState(bool state);

    // Sensores específicos
    void readDHTSensor();
    bool hasSensorValuesChanged() const;
    void storeLastSensorValues();
};

#endif // GREENHOUSE_H

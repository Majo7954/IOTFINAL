#include <ArduinoJson.h>
#include "Greenhouse.h"

Greenhouse::Greenhouse(WiFiClientSecure* client, const char* ssid, const char* password)
    : client(client),
      wifiManager(ssid, password),
      rainSensor(35),
      ldrSensor(2),
      airQualitySensor(4),
      soilMoistureSensor(34),
      dhtSensor(15, DHT11),
      flameSensor(18),
      pump(12),
      fan(26, 27),
      servo(13),
      yellowLED1(21),
      yellowLED2(33),
      redLED(23),
      buzzer(5),
      lastDHTReadTime(0),
      pumpState(false),
      fanState(false),
      servoState("closed"),
      yellowLEDsState(false),
      lastSoilMoisture(-1),
      lastTemperature(-1),
      lastHumidity(-1) {}

void Greenhouse::setup() {
    Serial.begin(115200);
    Serial.println("[Greenhouse] Initializing system...");

    wifiManager.connect();

    rainSensor.setup();
    ldrSensor.setup();
    airQualitySensor.setup();
    soilMoistureSensor.setup();
    dhtSensor.init();
    flameSensor.init();

    pump.setup();
    fan.init();
    servo.init();
    yellowLED1.init();
    yellowLED2.init();
    redLED.init();
    buzzer.init();

    Serial.println("[Greenhouse] Setup complete.");
}

void Greenhouse::configureMQTT(const char* broker, int port, const char* clientId, const char* updateTopic, const char* deltaTopic) {
    mqttClient = new PubSubClient(*client);
    mqttClient->setServer(broker, port);
    this->updateTopic = updateTopic;
    this->deltaTopic = deltaTopic;

    mqttClient->setCallback([this](char* topic, byte* payload, unsigned int length) {
        payload[length] = '\0'; // Convertir a cadena válida
        this->handleDelta((const char*)payload);
    });
}

bool Greenhouse::isConnected() {
    return mqttClient->connected();
}

void Greenhouse::reconnect(const char* clientId) {
    while (!mqttClient->connected()) {
        Serial.print("[MQTT] Attempting to connect...");
        if (mqttClient->connect(clientId)) {
            Serial.println("[MQTT] Connected");
        } else {
            Serial.printf("[MQTT] Failed, rc=%d. Retrying...\n", mqttClient->state());
            delay(5000);
        }
    }
}

void Greenhouse::loop() {
    if (!mqttClient->loop()) {
        reconnect("GreenhouseClient");
    }

    checkSensors();
    publishState();
    delay(5000);
}

void Greenhouse::checkSensors() {
    soilMoisture = soilMoistureSensor.readMoisturePercentage();
    readDHTSensor();
    flameSensor.read();
    rainPercentage = rainSensor.readRainPercentage();
    lightDetected = ldrSensor.isLightDetected();

    Serial.printf("[Sensors] Soil Moisture: %.2f%%, Temperature: %.2f°C, Humidity: %.2f%%\n",
                  soilMoisture, temperature, humidity);

    Serial.printf("[Sensors] Rain Level: %.2f%%, Light: %s\n",
                  rainPercentage, lightDetected ? "Detected" : "Not detected");

    if (flameSensor.isFlameDetected()) {
        Serial.println("[Sensors] Flame detected!");
        redLED.blink();
        buzzer.turnOn();
    } else {
        redLED.turnOff();
        buzzer.turnOff();
    }
}

void Greenhouse::readDHTSensor() {
    unsigned long currentTime = millis();
    if (currentTime - lastDHTReadTime >= dhtInterval) {
        temperature = dhtSensor.getTemperature();
        humidity = dhtSensor.getHumidity();
        lastDHTReadTime = currentTime;
    }
}

void Greenhouse::publishState() {
    if (!mqttClient || !mqttClient->connected()) {
        Serial.println("[Greenhouse] MQTT client not connected. Skipping state publication.");
        return;
    }

    DynamicJsonDocument doc(1024);

    doc["state"]["reported"]["sensors"]["soilMoisture"] = soilMoisture;
    doc["state"]["reported"]["sensors"]["temperature"] = temperature;
    doc["state"]["reported"]["sensors"]["humidity"] = humidity;
    doc["state"]["reported"]["sensors"]["rainPercentage"] = rainPercentage;
    doc["state"]["reported"]["sensors"]["lightDetected"] = lightDetected;

    doc["state"]["reported"]["actuators"]["pumpState"] = pumpState;
    doc["state"]["reported"]["actuators"]["fanState"] = fanState;
    doc["state"]["reported"]["actuators"]["servoState"] = servoState;
    doc["state"]["reported"]["actuators"]["yellowLEDsState"] = yellowLEDsState;

    String json;
    serializeJson(doc, json);

    if (mqttClient->publish(updateTopic, json.c_str())) {
        Serial.println("[Greenhouse] State published successfully.");
    } else {
        Serial.println("[Greenhouse] Failed to publish state.");
    }
}

void Greenhouse::handleDelta(const char* payload) {
    Serial.printf("[MQTT] Delta message received: %s\n", payload);

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
        Serial.printf("[MQTT] JSON Parse Error: %s\n", error.f_str());
        return;
    }

    if (doc.containsKey("pump")) {
        pumpState = doc["pump"];
        pumpState ? pump.turnOn() : pump.turnOff();
    }

    if (doc.containsKey("fan")) {
        fanState = doc["fan"];
        fanState ? fan.turnOn() : fan.turnOff();
    }

    if (doc.containsKey("servo")) {
        servoState = doc["servo"].as<String>();
        if (servoState == "open") {
            servo.open();
        } else if (servoState == "closed") {
            servo.close();
        }
    }

    if (doc.containsKey("yellowLEDs")) {
        yellowLEDsState = doc["yellowLEDs"];
        yellowLEDsState ? yellowLED1.turnOn() : yellowLED1.turnOff();
        yellowLEDsState ? yellowLED2.turnOn() : yellowLED2.turnOff();
    }

    // Publicar el estado actualizado
    publishState();
}


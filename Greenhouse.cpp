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

    wifiManager.connect(); // Conexión a WiFi

    rainSensor.setup();
    ldrSensor.setup();
    airQualitySensor.setup();
    soilMoistureSensor.setup();
    dhtSensor.init();
    flameSensor.init();

    pump.setup();
    fan.init();
    servo.init(); // Inicializa el servo
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
        payload[length] = '\0'; // Asegurarse de que el payload sea un string válido
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
            Serial.print("[MQTT] Failed, rc=");
            Serial.print(mqttClient->state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void Greenhouse::loop() {
    if (!mqttClient->loop()) {
        reconnect("GreenhouseClient");
    }
    checkSensors();
    controlActuators();
    delay(5000); // Intervalo entre lecturas y acciones
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
        Serial.println("[Sensors] No flame detected.");
        redLED.turnOff();
        buzzer.turnOff();
    }
}

void Greenhouse::controlActuators() {
    if (ldrSensor.isLightDetected()) {
        yellowLED1.turnOff();
        yellowLED2.turnOff();
        Serial.println("[Lighting] Natural light detected, yellow LEDs turned off.");
    } else {
        yellowLED1.turnOn();
        yellowLED2.turnOn();
        Serial.println("[Lighting] No natural light, yellow LEDs turned on.");
    }

    if (soilMoisture < 30.0) {
        pump.turnOn();
        Serial.println("[Irrigation] Pump turned on for watering.");
        delay(3000);
        pump.turnOff();
        Serial.println("[Irrigation] Pump turned off.");
    }

    if (temperature > 30.0) {
        fan.turnOn();
        Serial.println("[Ventilation] Fan turned on to reduce temperature.");
        delay(3000);
        fan.turnOff();
        Serial.println("[Ventilation] Fan turned off.");
    }

    if (servoState == "closed") {
        servo.open();
        servoState = "open";
        Serial.println("[Servo] Tank opened.");
    } else {
        servo.close();
        servoState = "closed";
        Serial.println("[Servo] Tank closed.");
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

void Greenhouse::handleDelta(const char* payload) {
    Serial.print("[MQTT] Delta message received: ");
    Serial.println(payload);

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
        Serial.print("[MQTT] Failed to parse JSON: ");
        Serial.println(error.f_str());
        return;
    }

    // Procesar el mensaje delta
    if (doc.containsKey("pump")) {
        bool pumpCommand = doc["pump"];
        pump.setState(pumpCommand);
        Serial.printf("[Actuators] Pump state changed to: %s\n", pumpCommand ? "ON" : "OFF");
    }
}

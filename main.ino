#include "WiFiManager.h"
#include "Greenhouse.h"

// WiFi y MQTT configuración
const char* WIFI_SSID = "Mi 9T Pro";
const char* WIFI_PASSWORD = "boquitapapa";
const char* MQTT_BROKER = "a2xkr2m0uy0ejv-ats.iot.us-east-2.amazonaws.com";
const int MQTT_PORT = 8883;
const char* CLIENT_ID = "ESP32_Greenhouse";

const char* UPDATE_TOPIC = "$aws/things/Greenhouse/shadow/update";
const char* DELTA_TOPIC = "$aws/things/Greenhouse/shadow/update/delta";

// Certificados AWS IoT
const char AMAZON_ROOT_CA1[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
<Contenido del certificado>
-----END CERTIFICATE-----
)EOF";

const char CERTIFICATE[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
<Contenido del certificado>
-----END CERTIFICATE-----
)KEY";

const char PRIVATE_KEY[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
<Contenido de la llave privada>
-----END RSA PRIVATE KEY-----
)KEY";

// Instancia de WiFiManager
WiFiManager wifi(WIFI_SSID, WIFI_PASSWORD);

// Puntero para Greenhouse
Greenhouse* greenhouse;

void setup() {
    Serial.begin(115200);

    // Conectar a WiFi
    Serial.println("[Setup] Connecting to WiFi...");
    wifi.connect();

    // Configurar certificados para AWS IoT
    Serial.println("[Setup] Setting up AWS IoT certificates...");
    wifi.setupAWSCertificates(AMAZON_ROOT_CA1, CERTIFICATE, PRIVATE_KEY);

    // Crear instancia de Greenhouse
    Serial.println("[Setup] Initializing Greenhouse...");
    WiFiClientSecure* client = wifi.getClient();
    greenhouse = new Greenhouse(client, WIFI_SSID, WIFI_PASSWORD); // Pasa credenciales WiFi
    greenhouse->configureMQTT(MQTT_BROKER, MQTT_PORT, CLIENT_ID, UPDATE_TOPIC, DELTA_TOPIC);

    // Configurar el sistema del invernadero
    greenhouse->setup();

    Serial.println("[Setup] Greenhouse is ready.");
}

void loop() {
    // Verificar conexión WiFi
    if (!wifi.isConnected()) {
        Serial.println("[Loop] Reconnecting to WiFi...");
        wifi.connect();
    }

    // Verificar conexión MQTT
    if (!greenhouse->isConnected()) {
        Serial.println("[Loop] Reconnecting to MQTT...");
        greenhouse->reconnect(CLIENT_ID);
    }

    // Ejecutar ciclo principal del invernadero
    greenhouse->loop();
}

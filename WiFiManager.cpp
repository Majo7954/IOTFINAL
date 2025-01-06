#include "WiFiManager.h"
#include <Arduino.h>

WiFiManager::WiFiManager(const char* ssid, const char* password)
    : ssid(ssid), password(password) {}

void WiFiManager::connect() {
    Serial.print("[WiFiManager] Connecting to WiFi: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("[WiFiManager] Connected to WiFi!");
    Serial.print("[WiFiManager] IP Address: ");
    Serial.println(WiFi.localIP());
}

void WiFiManager::setupAWSCertificates(const char* rootCA, const char* certificate, const char* privateKey) {
    client.setCACert(rootCA);
    client.setCertificate(certificate);
    client.setPrivateKey(privateKey);
    Serial.println("[WiFiManager] AWS certificates configured.");
}

WiFiClientSecure* WiFiManager::getClient() {
    return &client;
}

bool WiFiManager::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getIPAddress() const {
    return WiFi.localIP().toString();
}

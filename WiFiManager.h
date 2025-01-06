#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <WiFiClientSecure.h>

class WiFiManager {
private:
    const char* ssid;
    const char* password;
    WiFiClientSecure client;

public:
    WiFiManager(const char* ssid, const char* password);

    void connect();
    void setupAWSCertificates(const char* rootCA, const char* certificate, const char* privateKey);
    WiFiClientSecure* getClient();
    bool isConnected() const;
    String getIPAddress() const;
};

#endif // WIFIMANAGER_H

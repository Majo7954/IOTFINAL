#include "WiFiManager.h"
#include "Greenhouse.h"

// WiFi y MQTT configuración
const char* WIFI_SSID = "Mi 9T Pro";
const char* WIFI_PASSWORD = "boquitapapa";
const char* MQTT_BROKER = "a2xkr2m0uy0ejv-ats.iot.us-east-2.amazonaws.com";
const int MQTT_PORT = 8883;
const char* CLIENT_ID = "ESP32_Greenhouse";

const char* UPDATE_TOPIC = "$aws/things/ESP32/shadow/update";
const char* DELTA_TOPIC = "$aws/things/ESP32/shadow/update/delta";

// Certificados AWS IoT
const char AMAZON_ROOT_CA1[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

const char CERTIFICATE[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAKuAXProiFElPjXBaDNI4SnYwqvzMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDExMDYyMjEy
NDJaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC+YhqxhdUfrrHnmXbo
/mGPEVQt3ieqsvEWEeLv8bJ7EzRP4K44zQlqnk9OO7q4+PtJZkdns0fiAZ1bf0rD
tGTLN1ui49CP6MGe3md2hUrSK8/hQtATyJ5S0T/6nAk/s8ynEnLCo2EIuye7F+O8
979Nd7wOB0YzN6ymByDmyM5ZEP0YW44WO5VayMZg2LBFDNYSLvVbcbgq2tiwykIk
9S+wC3WYdLLHFn3RUeBmDxVxsiG7f9Yh0p0g5h1swXUyoWenN2enzYeSXk9Mqsw6
oicbNd4ocidZVoaFIPSOBDndmWjR3KstgYtR3ZuaI/m56f/qOr0udiWJWU4sqRKy
/qJlAgMBAAGjYDBeMB8GA1UdIwQYMBaAFO2izxdUoFHPFA9L/lzOWW0srS7EMB0G
A1UdDgQWBBSdMWw8Z824vQHKin1xqVlGz/skwDAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAuubhkX2JDyq2IaLI/BWB//Lk
WouVpnJY508ZGyqJYgOeaIMOtb7vGcFqTnkyVOTmNonkxUSqgVZZARJD2bZ43f9D
3sdjGEKBSQF8tG2ahhfpfRDVFlVpTMz2SCjBgTGUS9CY+q/Jv1Y6xu6mkD5/7G6l
C5Wfc0gItOLgdDMuqm4GkHJJ0BLtKGV0OxnCs/mLAak9F229PJC7yGbzB/e3d2y9
FcJ1/+wYapkiIFjB5GBLSVjXiYo2J/BiClAtUzQ9Rw5nIc7qtIaTDCH8IZJK8CG0
nD1H9njWd5HtuoDyY2dpl6aO39r74w4e9tLxjCtO5GVPvOZq+khdlY0m+A+bUQ==
-----END CERTIFICATE-----
)KEY";

const char PRIVATE_KEY[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAvmIasYXVH66x55l26P5hjxFULd4nqrLxFhHi7/GyexM0T+Cu
OM0Jap5PTju6uPj7SWZHZ7NH4gGdW39Kw7RkyzdbouPQj+jBnt5ndoVK0ivP4ULQ
E8ieUtE/+pwJP7PMpxJywqNhCLsnuxfjvPe/TXe8DgdGMzespgcg5sjOWRD9GFuO
FjuVWsjGYNiwRQzWEi71W3G4KtrYsMpCJPUvsAt1mHSyxxZ90VHgZg8VcbIhu3/W
IdKdIOYdbMF1MqFnpzdnp82Hkl5PTKrMOqInGzXeKHInWVaGhSD0jgQ53Zlo0dyr
LYGLUd2bmiP5uen/6jq9LnYliVlOLKkSsv6iZQIDAQABAoIBAQCxAju5VCpsc5tb
LtddF66TO1Rd9RBCaG84ytaJqtn8DkX0An1ZuGEJ7TjlDymzc2MHwn2fUBQEcOXi
PT7BKMcSHl0n3NJ8CreCw1vE2XdQY4W69cjKrBtv+reWhV8gCEa+C6wBha/hU0lE
bZjDiEEogKurC15cnmX6U+RmTst0CHnsmd7vQpC+E6lx+vSwPCwxEW3KfU38OXvO
OGSvWFFHsaqSSgxEKKslbevsOYYVwOIKYRko7J3zhY/GaGojg8T+nY+Q2STDMZBz
wS+KlQuqTHdEZcsnyn21cOV00iya3eTpeN23Y9D0zkEUXb+Ux1DO+DzVU2uas802
Ng1G+xqhAoGBAOLYOvO1ZMukEq/Lt3YQtdFP6+veX5Ee2FmAWaFJH0IrlpywPEF2
o5k02l+AeukVQCB9HHLVagfIY8s0OJdEiON056i5aBRQn0JF0FFao0C2jutYhwv6
2iysZsDDFpID3GOr9piOpVYYqAdasVTu8usHSALh+oysfBoqWFhdnNKtAoGBANba
Mxlp3IOsvFEMwcdSg7XqYcuu3Ew9Nr2Ff9ZsM7LxPNaSRFam1kHtOX+rXPu1XvYI
j2UFW0HcQ9eEScKzlYqz6Xe9+RDX41fwf+kZFuJlcQJlQBsjxxuA1TaQA7pKP39r
ANN4HPyiELrK4Nc9gbGcqNxU16cjbH6JV0nOB72ZAoGAEkMOG+iSgMJorrZbwql9
uagReh+g2fH9wNbQYGeemJkhuuOFNk/yw7a5jc6uFm2biAIKdEQyEQZU4lsF40el
LYqPy8mBU4FaEN/F8Ya2ddM8lj2+QXyka8xFSWdrPb4J1AxYAdgZ/2BkxpLKi+fg
roMbjP/cR87q/sBB9Hs00NECgYBBfBoNCeYLQC/ZqEC7xtucK8cI9M/ycMnpc/Il
4a7UQJDheY1Yt9sgFvTqg8mAcqRx6uQBz2MkLMWEv65XqLLQpstnEsECMDr7x7LE
0j/WW0LZjxC99Oav7FGQIs7wDcdSx8NlsPtsyNy+gi2LjRSllPEmzrk4GSTtYGD5
ftF/KQKBgFbkDK1QghPXIebvy9F95utcOcMrytC7NPI/bskxMeAcbVvdCw3o9A4m
YzzWS9Yiz7ivCE9PnukFS7cgr1tGuZ8zhLUAeneuGaXUowstke7vjmjsPoPJLero
5gCogMLdiH9g9vVBCSJKoSWxf/xqnijce45wIx+/3PzCwHd60JAA
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

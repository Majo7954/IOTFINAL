#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include <DHT.h>

class DHTSensor {
private:
    int pin;
    DHT dht;
    float temperature;
    float humidity;

public:
    DHTSensor(int pin, int type);
    void init();
    void read();
    float getTemperature() const;
    float getHumidity() const;
};

#endif // DHT_SENSOR_H

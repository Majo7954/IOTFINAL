#ifndef FLAME_SENSOR_H
#define FLAME_SENSOR_H

class FlameSensor {
private:
    int pin;
    bool detected;

public:
    FlameSensor(int pin);
    void init();
    void read();
    bool isFlameDetected() const;
};

#endif // FLAME_SENSOR_H

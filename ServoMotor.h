#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include <ESP32Servo.h>

class ServoMotor {
public:
    ServoMotor(int pin);
    void init();
    void open();
    void close();
    void stop();
    bool isOpened() const;

private:
    int pin;
    bool isOpen; // Indica si el servo está en posición abierta
    Servo servo;
};

#endif

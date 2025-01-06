#ifndef LED_H
#define LED_H

#include <Arduino.h>

class LED {
private:
    const int pin;
    bool state;
    unsigned long lastBlinkTime;
    const unsigned long blinkInterval;
    bool isBlinking;

public:
    LED(int pin);
    void init();
    void turnOn();
    void turnOff();
    void blink();
    void update();
    bool getState() const { return state; }
    bool isBlinkActive() const { return isBlinking; }
};

#endif
#include "LED.h"
#include <Arduino.h>

LED::LED(int pin) 
    : pin(pin), state(false), lastBlinkTime(0), blinkInterval(500) {}

void LED::init() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void LED::turnOn() {
    digitalWrite(pin, HIGH);
    state = true;
}

void LED::turnOff() {
    digitalWrite(pin, LOW);
    state = false;
}

void LED::blink() {
    if (millis() - lastBlinkTime >= blinkInterval) {
        state = !state;
        digitalWrite(pin, state);
        lastBlinkTime = millis();
    }
}

void LED::update() {
    if (state) {
        blink();
    }
}
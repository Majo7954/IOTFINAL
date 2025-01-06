#ifndef BUZZER_H
#define BUZZER_H

class Buzzer {
private:
    int pin;
    bool isActive;
    int frequency;

public:
    Buzzer(int pin);
    void init();
    void setFrequency(int freq);
    void turnOn();
    void turnOff();
    bool getState() const { return isActive; }
};

#endif

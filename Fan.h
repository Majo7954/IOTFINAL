#ifndef FAN_H
#define FAN_H

class Fan {
private:
    int pin1;
    int pin2;

public:
    Fan(int pin1, int pin2);
    void init();
    void turnOn();
    void turnOff();
};

#endif // FAN_H

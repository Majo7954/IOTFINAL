#include "ServoMotor.h"
#include <Arduino.h>

ServoMotor::ServoMotor(int pin) : pin(pin), isOpen(false) {}

void ServoMotor::init() {
    servo.attach(pin);
    stop(); // Detener el servo inicialmente
    Serial.println("[ServoMotor] Initialized.");
}

void ServoMotor::open() {
    servo.writeMicroseconds(1700); // Gira en sentido horario
    delay(500); // Ajustar tiempo según sea necesario
    stop(); // Detener el servo
    isOpen = true;
    Serial.println("[ServoMotor] Opened.");
}

void ServoMotor::close() {
    servo.writeMicroseconds(1300); // Gira en sentido antihorario
    delay(500); // Ajustar tiempo según sea necesario
    stop(); // Detener el servo
    isOpen = false;
    Serial.println("[ServoMotor] Closed.");
}

void ServoMotor::stop() {
    servo.writeMicroseconds(1500); // Detiene el servo
    Serial.println("[ServoMotor] Stopped.");
}

bool ServoMotor::isOpened() const {
    return isOpen;
}

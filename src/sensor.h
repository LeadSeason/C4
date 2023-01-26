#pragma once
#include <Arduino.h>

class Sensor
{
    public:
        Sensor();
        Sensor(uint8_t sensorPin, int pinThreshold);
        bool getState();
        bool getStateLoop();
        int sensorLoop();
        int getValue();
        int getNewValue();
        int getMaxValue();
        int getMinValue();
        int getCurrentValue();
    private:
        bool state = 0;
        int previousValueMax;
        int previousValueMin;
        int previousValue;
        int sensorNewValue;
        int pin;
        int threshold;
};
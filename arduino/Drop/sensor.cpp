#include "sensor.h"

Sensor::Sensor()
{

}

Sensor::Sensor(uint8_t sensorPin, int pinThreshold)
{
    pin = sensorPin;
    threshold = pinThreshold;

    pinMode(pin, INPUT);

    previousValue = analogRead(pin);
    for (size_t i = 1; i < 20; i++)
    {
        previousValue = ((previousValue * (i - 1)) + analogRead(pin)) / i;
        delay(20);
    }

    previousValueMax = previousValue + (threshold >> 1);
    previousValueMin = previousValue - (threshold >> 1);
}

int Sensor::sensorLoop()
{
    sensorNewValue = analogRead(pin);
    if (sensorNewValue >= previousValueMax)
    {
        state = false;
        return 1;
    } 
    if (sensorNewValue <= previousValueMin)
    {
        state = true;
        return -1;
    }
    state = false;
    return 0;
}

bool Sensor::getStateLoop()
{
    sensorLoop();
    return state;
}

bool Sensor::getState()
{
    return state;
}

int Sensor::getNewValue()
{
    return sensorNewValue;
}

int Sensor::getValue()
{
    return previousValue;
}

int Sensor::getCurrentValue()
{
    return analogRead(pin);
}

int Sensor::getMaxValue()
{
    return previousValueMax;
}

int Sensor::getMinValue()
{
    return previousValueMin;
}
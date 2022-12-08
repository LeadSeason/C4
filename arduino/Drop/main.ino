#include <ezButton.h>
#include <Servo.h>
#include <AccelStepper.h>
#include <Arduino.h>
#include "dropper.h"

// Define stepper motor connections and steps per revolution:
#define pinDir 8
#define pinStep 9
#define pinServo 10

const int piceLocations[8] = {0, 891, 1040, 1248, 1560, 2080, 3120, 6240};
c4Dropper dropper;

void setup()
{
    dropper = c4Dropper(pinStep, pinDir);
}

void loop()
{
    for (int i = 1; i < 8; i++)
    {
        dropper.dropPice(-piceLocations[i]);
    }
}
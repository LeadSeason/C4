#ifndef dropper_h
#define dropper_h
#include <Servo.h>
#include <AccelStepper.h>
#include <ezButton.h>
#include <Arduino.h>

class c4Dropper {
    public:
        c4Dropper();
        c4Dropper(int step, int pin);
        void goToHome();
        void dropPice(int pos);
        void servodrop();
    private:
        AccelStepper motor;
        Servo dropperServo;
        ezButton limitSwitch;
};

#endif
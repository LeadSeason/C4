#include "dropper.h"

c4Dropper::c4Dropper()
: limitSwitch(A0)
{

}

c4Dropper::c4Dropper(int step, int dir)
: motor(AccelStepper::DRIVER, step, dir), limitSwitch(A0)
{
    dropperServo.attach(10);
    dropperServo.write(0);

    limitSwitch.setDebounceTime(50);
    motor.setMaxSpeed(8000.0);
    motor.setAcceleration(16000.0);

    while (!limitSwitch.getState())
    {
        motor.setSpeed(400);
        motor.run();
        limitSwitch.loop();
    }
    motor.stop();
    motor.setCurrentPosition(0);
}

void c4Dropper::goToHome()
{
    /*
     * ✅ Move to position 0
     * ✅ Dont drop a pice at location 0
     *    Check if any sensors are triggered
     */
    motor.moveTo(0);
    while (motor.distanceToGo() != 0)
    {
        motor.run();
    }
}

void c4Dropper::dropPice(int pos)
{
    motor.moveTo(pos);
    while (motor.distanceToGo() != 0)
    {
        motor.run();
    }
    servodrop();
    goToHome();
}

void c4Dropper::servodrop()
{
    dropperServo.write(100);
    delay(300);
    dropperServo.write(0);
}
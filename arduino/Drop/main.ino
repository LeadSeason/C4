#include <Arduino.h>
#include <ezButton.h>
#include <Servo.h>
#include <AccelStepper.h>
#include "dropper.h"
#include "utils.hpp"
#include "sensor.h"
#include "game.h"

// Define stepper motor connections and steps per revolution:
#define PINDIR 8
#define PINSTEP 9
#define PINSERVO 10
#define THRESHOLD 80

const int piceLocations[8] = {0, 891, 1782, 2673, 3564, 4455, 5346, 6237};
const uint8_t sensorPins[] = {62, 63, 64, 65, 66, 67, 68};
c4Dropper dropper;
Sensor sensors[sizeof(sensorPins)];
Game game;

void setup()
{
    Serial.begin(115200);
    utils::Print("Serial init ... OK\n");

    utils::Print("Dropper init ... ");
    dropper = c4Dropper(PINSTEP, PINDIR);
    utils::Print("OK\n");

    utils::Print("Sensors init ... ");
    for (int i = 0; i < sizeof(sensorPins); i++)
    {
        pinMode(sensorPins[i], INPUT);
        sensors[i] = Sensor(sensorPins[i], THRESHOLD);
        utils::Print("ping: %d\n", sensorPins[i]);
    }
    utils::Print("OK\n");

    utils::Print("Game logic init ... ");
    game = Game();
    utils::Print("OK\n");
    game.displayBoard();

}

void loop()
{
    unsigned int colunm = 0;
    for (Sensor& sensor : sensors)
    {
        int sens_loop = sensor.sensorLoop();
        if (sensor.getState())
        {
            // clear terminal.
            utils::Print("\033[2J\033[3J\033[1;1H");
            // Print info about puck detection results.
            utils::Print("%d \033[38;5;7mLed (%d)\033[38;5;4m %d \033[38;5;%dm%d\033[38;5;7m, (%d - %d - %d)\n",
            sens_loop, colunm, sensor.getNewValue(), sensor.getState(), sensor.getState(), sensor.getMinValue(), sensor.getValue(), sensor.getMaxValue());

            game.dropPiece(colunm + 1, 1);
            game.displayBoard();

            while (sensor.getStateLoop()) {}

            checkGameOver(game.checkWinners());
        }
        colunm++;
    }
    delay(20);
}

void checkGameOver(int gameover)
{
	if (gameover != 0)
	{
		if (gameover == -1)
		{
			utils::Print("No body won Sad🙁");
		}
		else if (gameover == 1)
		{
			utils::Print("Yellow Player Won🎉");
		}
		else if (gameover == 2)
		{
			utils::Print("Red Player Won🎉");
		}
		else
		{
			utils::Print("Player %d Won🎉", gameover);
		}
		utils::Print("\n");

        utils::Print("Gameover. Clear board and press reset to restart game");
        while (true) {}
	}
}
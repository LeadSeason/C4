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

const int piceLocations[8] = {0, 7400, 6412, 5400, 4350, 3306, 2254, 1252};
const uint8_t sensorPins[] = {62, 63, 64, 65, 66, 67, 68};
c4Dropper dropper;
Sensor sensors[sizeof(sensorPins)];
Game game;

void setup()
{
    Serial.begin(115200);
    utils::Print("Serial init     ... \033[38;5;2mOK\033[38;5;7m\n");

    utils::Print("Dropper init    ... \n");
    dropper = c4Dropper(PINSTEP, PINDIR);
    utils::Print("Dropper init    ... \033[38;5;2mOK\033[38;5;7m\n");

    utils::Print("Sensors init    ... \n");
    for (int i = 0; i < sizeof(sensorPins); i++)
    {
        pinMode(sensorPins[i], INPUT);
        sensors[i] = Sensor(sensorPins[i], THRESHOLD);
        utils::Print("\033[38;5;7mSensor (%d)\033[38;5;4m %d \033[38;5;%dm%d\033[38;5;7m, (%d - %d - %d)\n",
        i, sensors[i].getNewValue(), sensors[i].getState(), sensors[i].getState(), sensors[i].getMinValue(), sensors[i].getValue(), sensors[i].getMaxValue());
    }
    utils::Print("Sensors init    ... \033[38;5;2mOK\033[38;5;7m\n");

    utils::Print("Game logic init ... ");
    game = Game();
    utils::Print("\033[38;5;2mOK\033[38;5;7m\n");
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
            // Print info about puck detection results.
            utils::Print("%d \033[38;5;7mLed (%d)\033[38;5;4m %d \033[38;5;%dm%d\033[38;5;7m, (%d - %d - %d)\n",
            sens_loop, colunm, sensor.getNewValue(), sensor.getState(), sensor.getState(), sensor.getMinValue(), sensor.getValue(), sensor.getMaxValue());

            game.dropPiece(colunm + 1);
            utils::Print("Gameover State: %d\n", game.checkWinners());
            game.displayBoard();
            checkGameOver(game.checkWinners());
            utils::Print("\n");

            int aiMove = game.aiPlayMove();

            while (sensor.getStateLoop()) {}

            utils::Print("🤔Ai Playing move: %d. Dropping\n", aiMove);
            utils::Print("Gameover State: %d\n", game.checkWinners());
            game.displayBoard();

            dropper.dropPice(piceLocations[aiMove]);

            checkGameOver(game.checkWinners());
            utils::Print("\n");
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
			utils::Print("No body won Sad🙁\n");
		}
		else if (gameover == 1)
		{
			utils::Print("Orage Player Won🎉\n");
		}
		else if (gameover == 2)
		{
			utils::Print("Yellow Player Won🎉\n");
		}
		else
		{
			utils::Print("Player %d Won🎉\n", gameover);
		}
		utils::Print("\n");

        utils::Print("Gameover. Clear board and press reset to restart game\n");
        while (true) {}
	}
}
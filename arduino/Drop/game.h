#pragma once
#include <Arduino.h>
#include "utils.hpp"

// rows columns
#define C_ROWS 6			// row
#define C_COLUMNS 7			// column

class Game
{
    public:
        Game();
        Game(int depth);
        void displayBoard();
        int dropPiece(int col, int piece);
        int checkWinners();
    private:
        void initBoard();
        int maxDepth;
        int board[C_ROWS][C_COLUMNS];
        int isPlaceValid(int col);
};


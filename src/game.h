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
        int checkWinners();
        int dropPiece(int col);
        int aiPlayMove();
    private:
        void initBoard();
        int isPlaceValid(int col);
        int isPlaceValid(int board[C_ROWS][C_COLUMNS], int col);
        int dropPiece(int col, int piece);
        int dropPiece(int board[C_ROWS][C_COLUMNS], int col, int piece);
        int maxDepth;
        int g_bestScore, g_bestMove;
        int MAX_INT = 10000;
        int MIN_INT = -10000;
        int board[C_ROWS][C_COLUMNS];
        void copyBoard(int board[C_ROWS][C_COLUMNS], int Coard[C_ROWS][C_COLUMNS]);
        int checkWinners(int board[C_ROWS][C_COLUMNS]);
        int getMaxConnected(int board[C_ROWS][C_COLUMNS], int player);
        int evaluateBoard(int board[C_ROWS][C_COLUMNS], int depth);
        int minmax(int board[C_ROWS][C_COLUMNS], int depth, bool maximizing);
        int getBestMove(int board[C_ROWS][C_COLUMNS], bool maximizing);

};


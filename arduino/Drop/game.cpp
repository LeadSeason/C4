#include "game.h"

Game::Game()
: Game(4)
{
}

Game::Game(int depth)
{
    maxDepth = depth;
    initBoard();
}

void Game::initBoard()
{
	for (int x = 0; x < C_ROWS; x++)
		for (int y = 0; y < C_COLUMNS; y++)
			board[x][y] = 0;
}

int Game::isPlaceValid(int col)
{
	/*
	 * Used to check if Pieces can be dropped
	 * Return 0 if valid
	 * Return player if not
	 * Return -1 if outofbounds
	 */

	if (C_COLUMNS < col || col < 1)
		return -1;

	return board[C_ROWS - 1][col - 1];
}

int Game::dropPiece(int col, int piece)
{
	if (isPlaceValid(col) == 0)
	{
		for (int y = 0; y < C_ROWS; y++)
		{
			if (board[y][col - 1] == 0)
			{
				return board[y][col - 1] = piece;
			}
		}
	}

	return -1;
}


int Game::checkWinners()
{
	int i, j;

	// check for all players
	for (int p = 1; p <= 2; p++)
	{
		// check horizontals
		for (i = 0; i < C_COLUMNS; i++)
			for (j = 0; j <= C_COLUMNS - 4; j++)
			{
				if (board[i][j] == p && board[i][j + 1] == p && board[i][j + 2] == p &&
					board[i][j + 3] == p)
					return p;
			}

		// check verticals
		for (i = 0; i <= C_ROWS - 4; i++)
			for (j = 0; j < C_COLUMNS; j++)
			{
				if (board[i][j] == p && board[i + 1][j] == p && board[i + 2][j] == p &&
					board[i + 3][j] == p)
					return p;
			}

		// check main diagonals (\)
		for (i = C_ROWS - 1; i >= 4 - 1; i--)
			for (j = 0; j <= C_COLUMNS - 4; j++)
			{
				if (board[i][j] == p && board[i - 1][j + 1] == p && board[i - 2][j + 2] == p &&
					board[i - 3][j + 3] == p)
					return p;
			}

		// check other diagonals (/)
		for (i = 0; i <= C_ROWS - 4; i++)
			for (j = 0; j <= C_COLUMNS - 4; j++)
			{
				if (board[i][j] == p && board[i + 1][j + 1] == p && board[i + 2][j + 2] == p &&
					board[i + 3][j + 3] == p)
					return p;
			}
	}

	// check if stalement
	for (i = 0; i < C_COLUMNS; i++)
		if (board[C_ROWS - 1][i] == 0)
			return 0;

	return -1;
}

void Game::displayBoard()
{
	int i, j, k, m;
	const char vert_line = '|';
	const char horz_line_segment[] = "---+";
	const char horz_line_segment_prefix = '+';
	const char playerColor1[] = "\x1b[30;43m";
	const char playerColor2[] = "\x1b[30;41m";
	const char resetColor[] = "\x1b[0m";

	utils::Print("\n ");
	for (i = 0; i < C_COLUMNS; i++)
		utils::Print(" %d  ", i + 1);
	utils::Print("\n");

	for (i = C_ROWS - 1; i >= 0; i--)
	{
		utils::Print("%c", vert_line);
		for (j = 0; j < C_COLUMNS; j++)
		{
			switch (board[i][j])
			{
			case 1:
				utils::Print(" %s%d%s %c", playerColor1, board[i][j], resetColor, vert_line);
				break;
			case 2:
				utils::Print(" %s%d%s %c", playerColor2, board[i][j], resetColor, vert_line);
				break;
			default:
				utils::Print(" %d %c", board[i][j], vert_line);
			}
		}
		utils::Print("\n");
		utils::Print("%c", horz_line_segment_prefix);
		for (k = 0; k < C_COLUMNS; k++)
			utils::Print("%s", horz_line_segment);

		utils::Print("\n");
	}
	utils::Print("\n");
}
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

int Game::dropPiece(int col)
{
	return dropPiece(col, 1);
}


int Game::checkWinners()
{
	int i, j, p;

	utils::Print("Checking winning moves\r");
	// check for all players
	for (p = 1; p <= 2; p++)
	{
		utils::Print("Checking winning move fro player %d\r", p);
		// check horizontals (-)
		for (i = 0; i < C_COLUMNS; i++)
			for (j = 0; j <= C_COLUMNS - 4; j++)
			{
				utils::Print("Checking winning move pices at (%d %d), (%d %d), (%d %d), (%d %d)\r", i, j,i, j + 1,i, j + 2,i, j + 3);
				if (board[i][j] == p && board[i][j + 1] == p && board[i][j + 2] == p &&
					board[i][j + 3] == p)
				{
					utils::Print("✅Found winning move pices at (%d %d), (%d %d), (%d %d), (%d %d)\r", i, j,i, j + 1,i, j + 2,i, j + 3);
					return p;
				}
			}

		// check verticals (|)
		for (i = 0; i <= C_ROWS - 4; i++)
			for (j = 0; j < C_COLUMNS; j++)
			{
				utils::Print("Checking winning move pices at (%d %d), (%d %d), (%d %d), (%d %d)\r", i, j, i + 1, j, i + 2, j, i + 3, j);
				if (board[i][j] == p && board[i + 1][j] == p && board[i + 2][j] == p &&
					board[i + 3][j] == p)
				{
					utils::Print("✅Found winning move pices at (%d %d), (%d %d), (%d %d), (%d %d)\r", i, j, i + 1, j, i + 2, j, i + 3, j);
					return p;
				}
			}

		// check main diagonals (\)
		for (i = C_ROWS - 1; i >= 4 - 1; i--)
			for (j = 0; j <= C_COLUMNS - 4; j++)
			{
				utils::Print("Checking winning move pices at (%d %d), (%d %d), (%d %d), (%d %d)\r", i, j, i - 1, j + 1,i - 2, j + 2,i - 3, j + 3);
				if (board[i][j] == p && board[i - 1][j + 1] == p && board[i - 2][j + 2] == p &&
					board[i - 3][j + 3] == p)
				{
					utils::Print("✅Found winning move pices at (%d %d), (%d %d), (%d %d), (%d %d)\r", i, j, i - 1, j + 1,i - 2, j + 2,i - 3, j + 3);
					return p;
				}
			}

		// check other diagonals (/)
		for (i = 0; i <= C_ROWS - 4; i++)
			for (j = 0; j <= C_COLUMNS - 4; j++)
			{
				utils::Print("Checking winning move pices at (%d %d), (%d %d), (%d %d), (%d %d)\r", i, j, i + 1, j + 1, i + 2, j + 2, i + 3, j + 3);
				if (board[i][j] == p && board[i + 1][j + 1] == p && board[i + 2][j + 2] == p &&
					board[i + 3][j + 3] == p)
				{
					utils::Print("✅Found winning move pices at (%d %d), (%d %d), (%d %d), (%d %d)\r", i, j, i + 1, j + 1, i + 2, j + 2, i + 3, j + 3);
					return p;
				}
			}
	}
	utils::Print("No winning moves for any player\r");

	// check if stalement
	for (i = 0; i < C_COLUMNS; i++)
		if (board[C_ROWS - 1][i] == 0)
		{
			utils::Print("No stale mate continue\r");
			return 0;
		}

	utils::Print("stale mate\r");
	return -1;
}



void Game::displayBoard()
{
	int i, j, k, m;
	const char vert_line = '|';
	const char horz_line_segment[] = "---+";
	const char horz_line_segment_prefix = '+';
	const char playerColor1[] = "\x1b[30;41m";
	const char playerColor2[] = "\x1b[30;43m";
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

int Game::aiPlayMove()
{
	int aiMove = getBestMove(board, false);
	dropPiece(aiMove, 2);
	return  aiMove;
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

int Game::getMaxConnected(int board[C_ROWS][C_COLUMNS], int player)
{
	auto getMaxConnectedCol = [&](int col)
	{
		int connected = 0;
		int current = 0;

		for (int i = 0; i < C_ROWS; i++)
		{
			if (board[i][col] == player)
			{
				current++;
				connected = utils::Max(current, connected);
			}
			else
			{
				current = 0;
			}
		}

		return connected;
	};

	auto getMaxConnectedRow = [&](int row)
	{
		int connected = 0;
		int current = 0;

		for (int i = 0; i < C_ROWS; i++)
		{
			if (board[row][i] == player)
			{
				current++;
				connected = utils::Max(current, connected);
			}
			else
			{
				current = 0;
			}
		}

		return connected;
	};

	int maxConnected = 0;

	for (int i = 0; i < C_COLUMNS; i++)
	{
		maxConnected = utils::Max(maxConnected, getMaxConnectedCol(i));
	}

	for (int i = 0; i < C_ROWS; i++)
	{
		maxConnected = utils::Max(maxConnected, getMaxConnectedRow(i));
	}

	/// @todo: Check diagonals
	return maxConnected;
}

void Game::copyBoard(int board[C_ROWS][C_COLUMNS], int Coard[C_ROWS][C_COLUMNS])
{
	int x, y;
	for (x = 0; x < C_COLUMNS; x++)
		for (y = 0; y < C_ROWS; y++)
			Coard[y][x] = board[y][x];
}

// -1 = stalemate, 0 = continue, 1=player, 2=player2 ... etc
int Game::checkWinners(int s[C_ROWS][C_COLUMNS])
{
	int i, j;

	// check for all players
	for (int p = 1; p <= 2; p++)
	{
		// check horizontals
		for (i = 0; i < C_COLUMNS; i++)
			for (j = 0; j <= C_COLUMNS - 4; j++)
			{
				if (s[i][j] == p && s[i][j + 1] == p && s[i][j + 2] == p &&
					s[i][j + 3] == p)
					return p;
			}

		// check verticals
		for (i = 0; i <= C_ROWS - 4; i++)
			for (j = 0; j < C_COLUMNS; j++)
			{
				if (s[i][j] == p && s[i + 1][j] == p && s[i + 2][j] == p &&
					s[i + 3][j] == p)
					return p;
			}

		// check main diagonals (\)
		for (i = C_ROWS - 1; i >= 4 - 1; i--)
			for (j = 0; j <= C_COLUMNS - 4; j++)
			{
				if (s[i][j] == p && s[i - 1][j + 1] == p && s[i - 2][j + 2] == p &&
					s[i - 3][j + 3] == p)
					return p;
			}

		// check other diagonals (/)
		for (i = 0; i <= C_ROWS - 4; i++)
			for (j = 0; j <= C_COLUMNS - 4; j++)
			{
				if (s[i][j] == p && s[i + 1][j + 1] == p && s[i + 2][j + 2] == p &&
					s[i + 3][j + 3] == p)
					return p;
			}
	}

	// check if stalement
	for (i = 0; i < C_COLUMNS; i++)
		if (s[C_ROWS - 1][i] == 0)
			return 0;

	return -1;
}

int Game::isPlaceValid(int board[C_ROWS][C_COLUMNS], int col)
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

int Game::dropPiece(int board[C_ROWS][C_COLUMNS], int col, int piece)
{
	if (isPlaceValid(board, col) == 0)
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

int Game::evaluateBoard(int board[C_ROWS][C_COLUMNS], int depth)
{
	int result = checkWinners(board);

	switch (result)
	{
	case 1:
		return 100 - depth;
	case 2:
		return depth - 100;

	default:
		break;
	}

	return getMaxConnected(board, 1) - getMaxConnected(board, 2);
}

int Game::minmax(int board[C_ROWS][C_COLUMNS], int depth, bool maximizing)
{
	if (depth >= maxDepth | checkWinners(board) != 0)
	{
		return evaluateBoard(board, depth);
	}

	int bestScore = maximizing ? MIN_INT : MAX_INT;

	for (int i = 1; i <= C_COLUMNS; i++)
	{
		if (isPlaceValid(board, i) == 0)
		{
			int tempBoard[C_ROWS][C_COLUMNS];
			copyBoard(board, tempBoard);

			dropPiece(tempBoard, i, maximizing ? 1 : 2);
			int score = minmax(tempBoard, depth + 1, !maximizing);

			if (maximizing)
				bestScore = utils::Max(score, bestScore);
			else
				bestScore = utils::Min(score, bestScore);
		}
	}

	return bestScore;
}

int Game::getBestMove(int board[C_ROWS][C_COLUMNS], bool maximizing)
{
	int bestMove = 1;

	int bestScore = maximizing ? MIN_INT : MAX_INT;

	for (int i = 1; i <= C_COLUMNS; i++)
	{
		int tempBoard[C_ROWS][C_COLUMNS];
		copyBoard(board, tempBoard);

		if (isPlaceValid(tempBoard, i) == 0)
		{
			dropPiece(tempBoard, i, maximizing ? 1 : 2);
			int score = minmax(tempBoard, 0, !maximizing);

			if (maximizing)
			{
				if (score > bestScore)
				{
					bestMove = i;
					bestScore = score;
				}
			}
			else
			{
				if (score < bestScore)
				{
					bestMove = i;
					bestScore = score;
				}
			}
		}
	}

	g_bestMove = bestMove;
	g_bestScore = bestScore;

	return bestMove;
}

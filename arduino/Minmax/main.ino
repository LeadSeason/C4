
// vim:set tabstop=2
// vim:set shiftwith=2

#include <Arduino.h>
#include <stdio.h>

// #include "type_traits.h"
#define C_row 6			// row
#define C_col 7			// column
#define C_max_players 2 // Max player ammount
#define MAX_DEPTH 4		// Max depth of minmax()

#define MAX_INT 10000
#define MIN_INT (-MAX_INT)

namespace Util
{
	template <typename T>
	inline T Min(T a, T b)
	{
		if (a > b)
			return b;
		else
			return a;
	}

	template <typename T>
	inline T Max(T a, T b)
	{
		if (a < b)
			return b;
		else
			return a;
	}
}

template <typename... Args>
void Print(const char* str, Args... args)
{
	#if defined(Arduino_h)
		char buffer[128];
		sprintf(buffer, str, args...);

		Serial.print(buffer);
	#else
		printf(str, args...);
	#endif
}

void Print(const char* str)
{
	#if defined(Arduino_h)
		Serial.print(str);
	#else
		printf(str);
	#endif
}

int getUserInt()
{	
	#if defined(Arduino_h)
	while (!Serial.available())
	{
	}

	return Serial.parseInt();
	#else
		int pos;
		scanf("%d", &pos);
		return pos;
	#endif
}
#if defined(Arduino_h)
void setup()
{
	Serial.begin(115200);
	Serial.println("Serial started");
}

void loop()
{
	myMain();
}
#endif

void initBoard(int board[C_row][C_col])
{
	for (int x = 0; x < C_row; x++)
		for (int y = 0; y < C_col; y++)
			board[x][y] = 0;
}

// -1 = stalemate, 0 = continue, 1=player, 2=player2 ... etc
int checkWinners(int s[C_row][C_col])
{
	int i, j;

	// check for all players
	for (int p = 1; p <= C_max_players; p++)
	{
		// check horizontals
		for (i = 0; i < C_col; i++)
			for (j = 0; j <= C_col - 4; j++)
			{
				if (s[i][j] == p && s[i][j + 1] == p && s[i][j + 2] == p &&
					s[i][j + 3] == p)
					return p;
			}

		// check verticals
		for (i = 0; i <= C_row - 4; i++)
			for (j = 0; j < C_col; j++)
			{
				if (s[i][j] == p && s[i + 1][j] == p && s[i + 2][j] == p &&
					s[i + 3][j] == p)
					return p;
			}

		// check main diagonals (\)
		for (i = C_row - 1; i >= 4 - 1; i--)
			for (j = 0; j <= C_col - 4; j++)
			{
				if (s[i][j] == p && s[i - 1][j + 1] == p && s[i - 2][j + 2] == p &&
					s[i - 3][j + 3] == p)
					return p;
			}

		// check other diagonals (/)
		for (i = 0; i <= C_row - 4; i++)
			for (j = 0; j <= C_col - 4; j++)
			{
				if (s[i][j] == p && s[i + 1][j + 1] == p && s[i + 2][j + 2] == p &&
					s[i + 3][j + 3] == p)
					return p;
			}
	}

	// check if stalement
	for (i = 0; i < C_col; i++)
		if (s[C_row - 1][i] == 0)
			return 0;

	return -1;
}

int g_bestMove, g_bestScore;

void displayBoard(int board[][C_col])
{

	int i, j, k, m;
	const char vert_line = '|';
	const char horz_line_segment[] = "---+";
	const char horz_line_segment_prefix = '+';
	const char playerColor1[] = "\x1b[30;43m";
	const char playerColor2[] = "\x1b[30;41m";
	const char playerColor3[] = "\x1b[30;45m";
	const char resetColor[] = "\x1b[0m";

	// system("clear");

	Print("\n ");
	for (i = 0; i < C_col; i++)
		Print(" %d  ", i + 1);
	Print("\n");

	for (i = C_row - 1; i >= 0; i--)
	{
		Print("%c", vert_line);
		for (j = 0; j < C_col; j++)
		{
			switch (board[i][j])
			{
			case 1:
				Print(" %s%d%s %c", playerColor1, board[i][j], resetColor, vert_line);
				break;
			case 2:
				Print(" %s%d%s %c", playerColor2, board[i][j], resetColor, vert_line);
				break;
			case 3:
				Print(" %s%d%s %c", playerColor3, board[i][j], resetColor, vert_line);
				break;
			default:
				Print(" %d %c", board[i][j], vert_line);
			}
		}
		Print("\n");
		Print("%c", horz_line_segment_prefix);
		for (k = 0; k < C_col; k++)
			Print("%s", horz_line_segment);

		Print("\n");
	}
	//	for (m = 1; m <= C_col; m++) {
	//		(m > 9) ? printf(" ") : printf("  ");
	//	}
	Print("\n");

	// std::cout << "Move: " << g_bestMove << " with score of: " << g_bestScore << std::endl;
}

// ai stuff

void copyBoard(int board[C_row][C_col], int Coard[C_row][C_col])
{
	int x, y;
	for (x = 0; x < C_col; x++)
		for (y = 0; y < C_row; y++)
			Coard[y][x] = board[y][x];
}

int isPlaceValid(int board[C_row][C_col], int col)
{
	/*
	 * Used to check if Pieces can be dropped
	 * Return 0 if valid
	 * Return player if not
	 * Return -1 if outofbounds
	 */

	if (C_col < col || col < 1)
		return -1;

	return board[C_row - 1][col - 1];
}

int dropPiece(int board[C_row][C_col], int col, int piece)
{
	if (isPlaceValid(board, col) == 0)
	{
		for (int y = 0; y < C_row; y++)
		{
			if (board[y][col - 1] == 0)
			{
				return board[y][col - 1] = piece;
			}
		}
	}

	return -1;
}

int getMaxConnected(int board[C_row][C_col], int player)
{
	auto getMaxConnectedCol = [&](int col)
	{
		int connected = 0;
		int current = 0;

		for (int i = 0; i < C_row; i++)
		{
			if (board[i][col] == player)
			{
				current++;
				connected = Util::Max(current, connected);
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

		for (int i = 0; i < C_row; i++)
		{
			if (board[row][i] == player)
			{
				current++;
				connected = Util::Max(current, connected);
			}
			else
			{
				current = 0;
			}
		}

		return connected;
	};

	int maxConnected = 0;

	for (int i = 0; i < C_col; i++)
	{
		maxConnected = Util::Max(maxConnected, getMaxConnectedCol(i));
	}

	for (int i = 0; i < C_row; i++)
	{
		maxConnected = Util::Max(maxConnected, getMaxConnectedRow(i));
	}

	/// @todo: Check diagonals
	return maxConnected;
}

int evaluateBoard(int board[C_row][C_col], int depth)
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

int minmax(int board[C_row][C_col], int depth, bool maximizing)
{
	if (depth >= MAX_DEPTH || checkWinners(board) != 0)
	{
		return evaluateBoard(board, depth);
	}

	int bestScore = maximizing ? MIN_INT : MAX_INT;

	for (int i = 1; i <= C_col; i++)
	{
		if (isPlaceValid(board, i) == 0)
		{
			int tempBoard[C_row][C_col];
			copyBoard(board, tempBoard);

			dropPiece(tempBoard, i, maximizing ? 1 : 2);
			int score = minmax(tempBoard, depth + 1, !maximizing);

			if (maximizing)
				bestScore = Util::Max(score, bestScore);
			else
				bestScore = Util::Min(score, bestScore);
		}
	}

	return bestScore;
}

int getBestMove(int board[C_row][C_col], bool maximizing)
{
	int bestMove = 1;

	int bestScore = maximizing ? MIN_INT : MAX_INT;

	for (int i = 1; i <= C_col; i++)
	{
		int tempBoard[C_row][C_col];
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

bool checkGameOver(int board[C_row][C_col])
{
	int gameover = checkWinners(board);
	if (gameover != 0)
	{
		if (gameover == -1)
		{
			Print("No body won Sad🙁");
		}
		else if (gameover == 1)
		{
			Print("Yellow Player Won🎉");
		}
		else if (gameover == 2)
		{
			Print("Red Player Won🎉");
		}
		else
		{
			Print("Player %d Won🎉", gameover);
		}
		Print("\n");
		return true;
	}

	return false;
}

#if defined(Arduino_h)
	int myMain()
#else
	int main()
#endif
{
	int board[C_row][C_col];
	initBoard(board);
	displayBoard(board);

	int possible = -1;
	while (true)
	{
		int pos;

		Print("Drop piece:");

		// get a valid player input
		do
		{
			pos = getUserInt();
			possible = dropPiece(board, pos, 1);

			if (checkGameOver(board))
			{
				return 0;
			}
		} while (possible == -1);

		dropPiece(board, getBestMove(board, false), 2);
		displayBoard(board);

		if (checkGameOver(board))
		{
			break;
		}
	}

	return 0;
}

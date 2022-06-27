#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <stdint.h>

#define GAME_FLAG_ALIVE			1
#define GAME_FLAG_VICTORY		2

#define SQUARE_FLAG_OPEN		1
#define SQUARE_FLAG_MINE		2
#define SQUARE_FLAG_FLAGGED		4

#define ERROR_MINE_COUNT	2
#define ERROR_MEM			1
#define OK					0

#define OPEN_RESULT_CLEAR			0
#define OPEN_RESULT_MINE			1
#define OPEN_RESULT_ALREADY_OPEN	2
#define OPEN_RESULT_DEAD			3
#define OPEN_RESULT_FLAGGED			4
#define OPEN_RESULT_VICTORY			5

#define RESULT_FLAG_TOGGLED			6

typedef enum Result_ {
	CLEAR,
	MINE,
	ALREADY_OPEN,
	DEAD,
	CANT_OPEN_FLAGGED,
	VICTORY,
	FLAG_TOGGLED,
	CANT_FLAG
} Result;

typedef struct Minesweeper_ 
{
	uint8_t*	squares;
	uint16_t	width;
	uint16_t	height;
	uint16_t	mines;
	uint8_t		flags;
} Minesweeper;

typedef struct Neighbor_
{
	uint8_t*		square;
	uint16_t		x;
	uint16_t		y;
} Neighbor;

typedef struct Neighbors_
{
	union
	{
		Neighbor squares[8];
		struct
		{
			Neighbor top_left;
			Neighbor top_middle;
			Neighbor top_right;
			Neighbor left;
			Neighbor right;
			Neighbor bottom_left;
			Neighbor bottom_middle;
			Neighbor bottom_right;
		};
	};
} Neighbors;

int initMinesweeper(Minesweeper* game, uint16_t width, uint16_t height, uint16_t mine_count);

void printMinesweeper(Minesweeper* game);

Result openSquare(Minesweeper* game, uint16_t x, uint16_t y);

Result toggleFlag(Minesweeper* game, uint16_t x, uint16_t y);

void deleteMinesweeper(Minesweeper* game);
#endif
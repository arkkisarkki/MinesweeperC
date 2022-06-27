#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <stdint.h>

#define GAME_FLAG_ALIVE			1
#define GAME_FLAG_VICTORY		2

#define SQUARE_FLAG_OPEN		1
#define SQUARE_FLAG_MINE		2
#define SQUARE_FLAG_FLAGGED		4

typedef enum InitResult_ {
	ERROR_MINE_COUNT,
	ERROR_MEM,
	OK
} InitResult;

typedef enum ActionResult_ {
	// Open
	CLEAR,
	MINE,
	ALREADY_OPEN,
	DEAD,
	CANT_OPEN_FLAGGED,
	VICTORY,
	
	// Open adjacent
	NOT_OPEN,
	BAD_FLAG_COUNT,

	// Flag
	FLAG_TOGGLED,
	CANT_FLAG
} ActionResult;

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

InitResult initMinesweeper(Minesweeper* game, uint16_t width, uint16_t height, uint16_t mine_count);

void printMinesweeper(Minesweeper* game);

ActionResult openSquare(Minesweeper* game, uint16_t x, uint16_t y);

ActionResult toggleFlag(Minesweeper* game, uint16_t x, uint16_t y);

ActionResult openAllAdjacent(Minesweeper* game, uint16_t x, uint16_t y);

void deleteMinesweeper(Minesweeper* game);
#endif
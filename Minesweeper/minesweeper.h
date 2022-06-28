#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <stdint.h>

#define GAME_FLAG_ALIVE			1
#define GAME_FLAG_VICTORY		2

#define SQUARE_FLAG_OPEN		1
#define SQUARE_FLAG_MINE		2
#define SQUARE_FLAG_FLAGGED		4

#define coordsToIndex(width, x, y) ((width) * (y) + (x))

typedef enum InitResult_
{
	/** All good. */
	OK,
	/** Mine count exceeds square count. */
	ERROR_MINE_COUNT,
	/** Memory allocation failure. */
	ERROR_MEM
} InitResult;

typedef enum ActionResult_
{
	// Open
	/** Square did not contain a mine.*/
	CLEAR,
	/** Square contained a mine. */
	MINE,
	/** The square was already open. */
	ALREADY_OPEN,
	/** Player was already dead. */
	DEAD,
	/** Trying to open a flagged square. */
	CANT_OPEN_FLAGGED,
	/** All mines found. */
	VICTORY,

	// Open adjacent
	/** Square needs to be open before adjacent can be opened. */
	NOT_OPEN,
	/** Can't open adjacent if neighboring flag count does not equal the number in the square. */
	BAD_FLAG_COUNT,

	// Flag
	/** Toggled a flag in a square. */
	FLAG_TOGGLED,
	/** Can't flag an open square. */
	CANT_FLAG
} ActionResult;

typedef struct Minesweeper_
{
	uint8_t* squares;
	uint16_t	width;
	uint16_t	height;
	uint16_t	mines;
	uint8_t		flags;
} Minesweeper;

typedef struct Neighbor_
{
	uint8_t* square;
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
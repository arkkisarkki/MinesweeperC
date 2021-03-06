#include "minesweeper.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/**
 * Create a new minesweeper game.
 *
 * \param game Minesweeper struct to initialize.
 * \param width Width of the game.
 * \param height Height of the game.
 * \param mine_count Number of mines to place.
 * \return InitResult describing error or successful init.
 */
InitResult initMinesweeper(Minesweeper* game, uint16_t width, uint16_t height, uint16_t mine_count)
{
	game->width = width;
	game->height = height;
	game->flags = GAME_FLAG_ALIVE;

	size_t size = (size_t) width * height;
	game->squares = calloc(size, sizeof(uint8_t));

	if (!game->squares)
	{
		return ERROR_MEM;
	}
	if (mine_count > size)
	{
		return ERROR_MINE_COUNT;
	}

	srand((unsigned int) time(NULL));
	game->mines = 0;
	while (game->mines < mine_count)
	{
		unsigned int i = rand() % (size - 1);
		if (!game->squares[i])
		{
			game->squares[i] = SQUARE_FLAG_MINE;
			game->mines++;
		}
	}

	return OK;
}

/**
 * Get neighboring blocks for given coordinates.
 *
 * \param game
 * \param x
 * \param y
 * \return
 */
static Neighbors getNeighbors(Minesweeper* game, uint16_t x, uint16_t y)
{
	Neighbors retval = { 0 };
	uint16_t width = game->width;

	if (y > 0)
	{
		if (x > 0)
		{
			retval.top_left.square = &game->squares[coordsToIndex(width, x - 1, y - 1)];
			retval.top_left.x = x - 1;
			retval.top_left.y = y - 1;
		}
		if (x < game->width - 1)
		{
			retval.top_right.square = &game->squares[coordsToIndex(width, x + 1, y - 1)];
			retval.top_right.x = x + 1;
			retval.top_right.y = y - 1;
		}
		retval.top_middle.square = &game->squares[coordsToIndex(width, x, y - 1)];
		retval.top_middle.x = x;
		retval.top_middle.y = y - 1;
	}
	if (x > 0)
	{	
		retval.left.square = &game->squares[coordsToIndex(width, x - 1, y)];
		retval.left.x = x - 1;
		retval.left.y = y;
	}
	if (x < game->width - 1)
	{
		retval.right.square = &game->squares[coordsToIndex(width, x + 1, y)];
		retval.right.x = x + 1;
		retval.right.y = y;
	}
	if (y < game->height - 1)
	{
		if (x > 0)
		{	
			retval.bottom_left.square = &game->squares[coordsToIndex(width, x - 1, y + 1)];
			retval.bottom_left.x = x - 1;
			retval.bottom_left.y = y + 1;
		}
		if (x < game->width - 1)
		{
			retval.bottom_right.square = &game->squares[coordsToIndex(width, x + 1, y + 1)];
			retval.bottom_right.x = x + 1;
			retval.bottom_right.y = y + 1;
		}
		retval.bottom_middle.square = &game->squares[coordsToIndex(width, x, y + 1)];
		retval.bottom_middle.x = x;
		retval.bottom_middle.y = y + 1;
	}

	return retval;
}

/**
 * Count mines in neighboring squares for given coordinates.
 *
 * \param game
 * \param x
 * \param y
 * \return Count of mines in neighbors.
 */
static unsigned int countAdjacentMines(Minesweeper* game, uint16_t x, uint16_t y)
{
	Neighbors neighbors = getNeighbors(game, x, y);
	unsigned int retval = 0;
	for (unsigned int i = 0; i < 8; i++)
	{
		if (neighbors.squares[i].square && *neighbors.squares[i].square & SQUARE_FLAG_MINE)
		{
			retval++;
		}
	}

	return retval;
}

/**
 * Print game state to stdout.
 *
 * \param game
 */
void printMinesweeper(Minesweeper* game)
{
	for (unsigned int row = 0; row < game->height; row++)
	{
		for (unsigned int column = 0; column < game->width; column++)
		{
			uint8_t current = game->squares[coordsToIndex(game->width, column, row)];

			if (current & SQUARE_FLAG_OPEN)
			{
				if (current & SQUARE_FLAG_MINE)
				{
					printf("X ");
				}
				else
				{
					unsigned int count = countAdjacentMines(game, column, row);
					if (count == 0)
					{
						printf(". ");
					}
					else
					{
						printf("%d ", count);
					}
				}
			}
			else // Closed
			{
				if (current & SQUARE_FLAG_FLAGGED)
				{
					printf("F ");
				}
				else
				{
					printf("%c ", 0xFE);
				}
			}
		}
		printf("\n");
	}
}

/**
 * Check if game is solved.
 *
 * \param game
 * \return
 */
static int checkVictory(Minesweeper* game)
{
	unsigned int open_count = 0;
	for (int i = 0; i < game->width * game->height; i++)
	{
		if (game->squares[i] & SQUARE_FLAG_OPEN)
		{
			open_count++;
		}
	}

	return game->width * game->height - open_count == game->mines;
}

/**
 * Attempt to open a square with given coordinates.
 *
 * \param game
 * \param x
 * \param y
 * \return
 */
ActionResult openSquare(Minesweeper* game, uint16_t x, uint16_t y)
{
	if (!(game->flags & GAME_FLAG_ALIVE))
	{
		// Already dead, can't open more squares.
		return DEAD;
	}

	uint8_t* target = &game->squares[coordsToIndex(game->width, x, y)];
	if (*target & SQUARE_FLAG_OPEN)
	{
		return ALREADY_OPEN;
	}
	if (*target & SQUARE_FLAG_FLAGGED)
	{
		return CANT_OPEN_FLAGGED;
	}

	*target |= SQUARE_FLAG_OPEN;
	*target &= ~SQUARE_FLAG_FLAGGED;
	if (*target & SQUARE_FLAG_MINE)
	{
		game->flags &= ~GAME_FLAG_ALIVE;
		return MINE;
	}

	if (countAdjacentMines(game, x, y) == 0)
	{
		Neighbors neighbors = getNeighbors(game, x, y);
		for (unsigned int i = 0; i < 8; i++)
		{
			if (neighbors.squares[i].square)
			{
				openSquare(game, neighbors.squares[i].x, neighbors.squares[i].y);
			}
		}
	}

	if (checkVictory(game))
	{
		game->flags |= GAME_FLAG_VICTORY;
		return VICTORY;
	}

	return CLEAR;
}

/**
 * Attempt to flag a square with given coordinates.
 *
 * \param game
 * \param x
 * \param y
 * \return
 */
ActionResult toggleFlag(Minesweeper* game, uint16_t x, uint16_t y)
{
	if (!(game->squares[coordsToIndex(game->width, x, y)] & SQUARE_FLAG_OPEN))
	{
		game->squares[coordsToIndex(game->width, x, y)] ^= SQUARE_FLAG_FLAGGED;
		return FLAG_TOGGLED;
	}
	return CANT_FLAG;
}

/**
 * Attempt to open adjacent non-flagged squares for given coordinates.
 *
 * \param game
 * \param x
 * \param y
 * \return
 */
ActionResult openAllAdjacent(Minesweeper* game, uint16_t x, uint16_t y)
{
	if (!(game->squares[coordsToIndex(game->width, x, y)] & SQUARE_FLAG_OPEN))
	{
		// Square needs to be open to attempt opening adjacent.
		return NOT_OPEN;
	}

	Neighbors neighbors = getNeighbors(game, x, y);
	int flag_count = 0;
	for (int i = 0; i < 8; i++)
	{
		if (neighbors.squares[i].square && *neighbors.squares[i].square & SQUARE_FLAG_FLAGGED)
		{
			flag_count++;
		}
	}

	if (flag_count != countAdjacentMines(game, x, y))
	{
		return BAD_FLAG_COUNT;
	}

	for (int i = 0; i < 8; i++)
	{
		if (neighbors.squares[i].square)
		{
			ActionResult result = openSquare(game, neighbors.squares[i].x, neighbors.squares[i].y);
			if (result == MINE)
			{
				return MINE;
			}
			else if (result == VICTORY)
			{
				return VICTORY;
			}
		}
	}

	return CLEAR;
}

/**
 * Release allocated memory.
 *
 * \param game
 */
void deleteMinesweeper(Minesweeper* game)
{
	free(game->squares);
}
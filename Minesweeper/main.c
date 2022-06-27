#include "minesweeper.h"
#include <stdio.h>

int main()
{
	Minesweeper game = { 0 };
	if (initMinesweeper(&game, 10, 10, 5) != OK)
	{
		return 1;
	}
	
	printMinesweeper(&game);
	
	while (game.flags & GAME_FLAG_ALIVE && !(game.flags & GAME_FLAG_VICTORY))
	{	
		char command;
		unsigned int x;
		unsigned int y;
		ActionResult result;
		
		int read = scanf_s(" %c %u %u", &command, 1, &x, &y);
		if (command == 'o')
		{
			result = openSquare(&game, x - 1, y - 1);	
		}
		else if (command == 'f')
		{
			result = toggleFlag(&game, x - 1, y - 1);
		}

		printMinesweeper(&game);

		switch (result)
		{
		case MINE:
		{
			printf("BOOM!\n");
		}break;
		case VICTORY:
		{
			printf("You won!\n");
		}break;
		case FLAG_TOGGLED:
		{
			printf("Flag toggled\n");
		}break;
		}
	}
	
	deleteMinesweeper(&game);

	return 0;
}
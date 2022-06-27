#include "minesweeper.h"
#include <stdio.h>

int main()
{
	int run = 1;
	while (run)
	{
		unsigned int width;
		unsigned int height;
		unsigned int mine_count;
		
		printf("Select width: ");
		scanf_s(" %u", &width);

		printf("Select height: ");
		scanf_s(" %u", &height);

		printf("Select mine count: ");
		scanf_s(" %u", &mine_count);

		Minesweeper game = { 0 };
		if (initMinesweeper(&game, width, height, mine_count) != OK)
		{
			return 1;
		}

		printMinesweeper(&game);

		// Game running
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
			else if (command == 'a')
			{
				result = openAllAdjacent(&game, x - 1, y - 1);
			}
			else
			{
				printf("Unknown command\n");
				continue;
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
			case ALREADY_OPEN:
			{
				printf("Square already open\n");
			} break;
			case FLAG_TOGGLED:
			{
				printf("Flag toggled\n");
			}break;
			case CANT_FLAG:
			{
				printf("Can't flag open square\n");
			} break;
			case BAD_FLAG_COUNT:
			{
				printf("Bad flag count\n");
			}break;
			case NOT_OPEN:
			{
				printf("Square needs to be open first\n");
			}break;
			}
		}

		deleteMinesweeper(&game);

		printf("Play again? ");
		char resp;
		
		restart:
		scanf_s(" %c", &resp, 1);
		if (resp == 'y' || resp == 'Y')
		{
			run = 1;
		}
		else if (resp == 'n' || resp == 'N')
		{
			run = 0;
		}
		else
		{
			goto restart;
		}
	}

	return 0;
}
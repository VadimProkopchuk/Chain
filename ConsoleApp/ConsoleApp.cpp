#include "pch.h"
#include "GameBuilderFactory.h"

int main()
{
	int EXIT = 2;
	int choice;
	utils::set_console_title(); 

	do
	{
		utils::resize_window(300, 300);
		utils::clearScreen();
		choice = utils::get_menu();

		switch (choice)
		{
			case 0: {
				utils::resize_window(700, 700);
				utils::clearScreen();

				GameBuilderFactory::CreateGameBuilder()
					->useYellowPlayerName(utils::readString("Enter YELLOW player name: "))
					->useBlackPlayerName(utils::readString("Enter BLACK player name: "))
					->useBoardSize(utils::readWithCheck("Enter board size: ", 5))
					->build()
					->run();

				getchar();
				break;
			}
			case 1: {
				utils::clearScreen();
				Game::author();
				getchar();
				break;
			}
		}
	} while (choice != EXIT);

	return EXIT_SUCCESS;
}
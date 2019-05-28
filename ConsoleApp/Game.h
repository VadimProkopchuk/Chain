#include "Player.h"
#include "Board.h"

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

class Game
{
private:
	Player* _yellowPlayer;
	Player* _blackPlayer;
	Board* _board;

	bool play(Player* active, Player* passive) {
		utils::clearScreen();

		if (!_board->make_move(active)) {

			cout << "Player " << active->getName() << " finished the game." << endl;
			return true;
		}

		if (_board->check_way(active)) {
			_board->render();
			cout << "Player " << _yellowPlayer->getName() << " is WIN!" << endl;
			return true;
		}

		return false;
	}



public:
	Game(string yellowPlayerName, string blackPlayerName, int boardSize) {
		_yellowPlayer = new Player(yellowPlayerName, YELLOW);
		_blackPlayer = new Player(blackPlayerName, BLACK);
		_board = new Board(boardSize);
	}

	~Game() {
		delete _yellowPlayer;
		delete _blackPlayer;
		delete _board;
	}

	void run() {
		Player* active = _blackPlayer;
		Player* passive = _yellowPlayer;

		do {
			Player* buffer = active;
			
			active = passive;
			passive = buffer;
		} while (!play(active, passive));
	}

	static void author() {
		cout << "Created by Vadim Prokopchuk." << endl;
	}
};

#endif

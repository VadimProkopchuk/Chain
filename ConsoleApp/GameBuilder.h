#include "Game.h"

#ifndef GAME_BUILDER_INCLUDED
#define GAME_BUILDER_INCLUDED

class GameBuilder
{
private:
	string _yellowPlayerName;
	string _blackPlayerName;
	int _boardSize;

public:
	GameBuilder(string yellowPlayerName, string blackPlayerName, int boardSize)
		: _yellowPlayerName(yellowPlayerName), _blackPlayerName(blackPlayerName),
		_boardSize(boardSize) {

	}

	GameBuilder* useYellowPlayerName(string name) {
		_yellowPlayerName = name;
		return this;
	}

	GameBuilder* useBlackPlayerName(string name) {
		_blackPlayerName = name;
		return this;
	}

	GameBuilder* useBoardSize(int boardSize) {
		_boardSize = boardSize;
		return this;
	}

	Game* build() {
		return new Game(_yellowPlayerName, _blackPlayerName, _boardSize);
	}

	~GameBuilder() {

	}
};

#endif
#include "CellType.h"

#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED
class Player
{
private:
	string _name;
	CellType _cellType;

public:
	Player(string name, CellType cellType)
		: _name(name), _cellType(cellType) {

	}

	~Player() {

	}

	string getName() {
		return this->_name;
	}

	CellType getCellType() {
		return this->_cellType;
	}
};

#endif
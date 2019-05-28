#include "CellType.h"
#include <list>

#ifndef BOARD_INCLUDED
#define BOARD_INCLUDED

class Board
{
private:
	class Point {
	public:
		int x;
		int y;

		Point(int x, int y) : x(x), y(y)
		{
		}
	};

	int _size;
	int _pointI;
	int _pointJ;
	CellType** _board;
	list<Point*>* _startPos;
	list<Point*>* _endPos;

	void init() {
		_board = new CellType*[_size];

		for (int i = 0; i < _size; i++) {
			_board[i] = new CellType[_size];

			for (int j = 0; j < _size; j++) {
				_board[i][j] = WHITE;
			}
		}
	}

	string get_board_symbol(CellType type) {
		switch (type)
		{
			case WHITE: return "   ";
			case YELLOW: return " Y ";
			case BLACK: return " B ";
			case WHITE_POINT: return " * ";
			case YELLOW_POINT: return "Y/*";
			case BLACK_POINT: return "B/*";
			default:return "   ";
		}
	}

	int get_index(int current, int step) {
		int index = current + step;

		if (index < 0) {
			index = _size - 1;
		}

		return index % _size;
	}

	bool handle_key_events() {
		DWORD w;
		HANDLE keyboard = GetStdHandle(STD_INPUT_HANDLE);
		INPUT_RECORD input[1];

		ReadConsoleInput(keyboard, input, 1, &w);

		if (input[0].EventType == KEY_EVENT && input[0].Event.KeyEvent.bKeyDown)
		{
			switch (input[0].Event.KeyEvent.wVirtualScanCode) {
			case 72: // up
				_pointI = get_index(_pointI, -1);
				break;

			case 75: // left 
				_pointJ = get_index(_pointJ, -1);
				break;

			case 77: // right
				_pointJ = get_index(_pointJ, 1);
				break;

			case 80: // down
				_pointI = get_index(_pointI, 1);
				break;

			case 28:
				return true;
			}
		}

		return false;
	}

	void set_point(int i, int j) {
		CellType cell = _board[i][j];

		switch (cell)
		{
			case WHITE: 
				cell = WHITE_POINT;
				break;
			case YELLOW:
				cell = YELLOW_POINT;
				break;
			case BLACK:
				cell = BLACK_POINT;
				break;
		}

		_board[i][j] = cell;
	}

	void revert_point(int i, int j) {
		CellType cell = _board[i][j];

		switch (cell)
		{
		case WHITE_POINT:
			cell = WHITE;
			break;
		case YELLOW_POINT:
			cell = YELLOW;
			break;
		case BLACK_POINT:
			cell = BLACK;
			break;
		}

		_board[i][j] = cell;
	}

	void revert_cells() {
		for (int i = 0; i < _size; i++) {
			for (int j = 0; j < _size; j++) {
				revert_point(i, j);
			}
		}
	}

	bool has_available() {
		return _board[_pointI][_pointJ] == WHITE_POINT;
	}

	void move(Player* player) {
		do {
			utils::clearScreen();
			this->revert_cells();
			this->set_point(_pointI, _pointJ);
			this->render_board();
			this->active_player(player);
		} while (!handle_key_events());
	}

	void commit(CellType type) {
		_board[_pointI][_pointJ] = type;
	}

	bool find_cell() {
		for (int i = 0; i < _size; i++) {
			for (int j = 0; j < _size; j++) {
				if (_board[i][j] == WHITE) {
					_pointI = i;
					_pointJ = j;
					return true;
				}
			}
		}

		return false;
	}


	bool available_for_changes() {
		for (int i = 0; i < _size; i++) {
			for (int j = 0; j < _size; j++) {
				if (_board[i][j] == WHITE) {
					return true;
				}
			}
		}

		return false;
	}

	void render_board() {
		cout << endl << "    ";
		for (int i = 0; i < _size; i++) {
			cout << "  Y ";
		}

		cout << endl << "    +";
		for (int i = 0; i < _size; i++) {
			cout << "---+";
		}

		for (int i = 0; i < _size; i++) {

			cout << endl << "   B|";

			for (int j = 0; j < _size; j++) {
				cout << get_board_symbol(_board[i][j]) << "|";
			}

			cout << "B" << endl << "    +";

			for (int j = 0; j < _size; j++) {
				cout << "---+";
			}
		}

		cout << endl << "    ";
		for (int i = 0; i < _size; i++) {
			cout << "  Y ";
		}
	}

	void active_player(Player* player) {
		cout << endl << endl 
			<< "Active player: " << player->getName() 
			<< " (" << get_board_symbol(player->getCellType()) << ")" 
			<< endl;
	}

	void find_positions(CellType type) {
		if (type == YELLOW) {
			_startPos = new list<Point*>();
			_endPos = new list<Point*>();

			for (int i = 0; i < _size; i++) {
				if (_board[0][i] == YELLOW) {
					_startPos->push_back(new Point(0, i));
				}

				if (_board[_size - 1][i] == YELLOW) {
					_endPos->push_back(new Point(_size - 1, i));
				}
			}
		}

		if (type == BLACK) {
			_startPos = new list<Point*>();
			_endPos = new list<Point*>();

			for (int i = 0; i < _size; i++) {
				if (_board[i][0] == BLACK) {
					_startPos->push_back(new Point(i, 0));
				}

				if (_board[i][_size - 1] == BLACK) {
					_endPos->push_back(new Point(i, _size - 1));
				}
			}
		}
	}

	bool hasEnd(int i, int j) {
		for (auto iter = _endPos->begin(); iter != _endPos->end(); iter++) {
			if ((*iter)->x == i && (*iter)->y == j) {
				return true;
			}
		}

		return false;
	}

	bool find(int i, int j, CellType type, bool** visited) {
		if (_board[i][j] != type || visited[i][j] == true) {
			visited[i][j] = true;
			return false;
		}

		visited[i][j] = true;

		if (hasEnd(i, j)) {
			return true;
		}

		return (i > 0 && find(i - 1, j, type, visited)) ||		// up
			(j < _size - 1 && find(i, j + 1, type, visited)) ||	// right
			(i < _size - 1 && find(i + 1, j, type, visited)) || // down
			(j > 0 && find(i, j - 1, type, visited));			// left
	}

	bool find(CellType type) {
		Point* initial = _startPos->front();
		bool** visited = new bool*[_size];

		for (int i = 0; i < _size; i++) {
			visited[i] = new bool[_size];
		}

		return find(initial->x, initial->y, type, visited);
	}

public:
	Board(int size) {
		_size = size;
		init();
	}

	~Board() {
		for (int i = 0; i < _size; i++) {
			delete[] _board[i];
		}

		delete[] _board;
	}

	bool make_move(Player* player) {
		if (find_cell()) {
			do { 
				move(player); 
			} while (!has_available());
			commit(player->getCellType());
		}

		return available_for_changes();
	}

	bool check_way(Player* player) {
		find_positions(player->getCellType());

		if (_startPos->size() > 0 && _endPos->size() > 0) {
			return find(player->getCellType());
		}
		
		return false;
	}

	void render() {
		utils::clearScreen();
		this->render_board();
		cout << endl << endl;
	}
};

#endif
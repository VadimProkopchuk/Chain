#include "pch.h"
#include <tchar.h>

class utils
{
private:
	static string prefix(int selected, int menu) {
		return selected % 3 == menu ? ">> " : "";
	}

	static string suffix(int selected, int menu) {
		return selected % 3 == menu ? " <<" : "";
	}

	static void menu_item(string text, int selected, int number) {
		cout << "\t    " << prefix(selected, number) << text << suffix(selected, number) << endl;
	}

	static void refresh_menu(int selected) {
		clearScreen();
		cout << endl << endl << endl << endl;
		cout << "\t\tMENU" << endl << endl;
		menu_item("New game", selected, 0);
		menu_item("Author ", selected, 1);
		menu_item("Exit  ", selected, 2);
	}

	static bool handle_keys(int& selectedMenu) {
		DWORD w;
		HANDLE keyboard = GetStdHandle(STD_INPUT_HANDLE);
		INPUT_RECORD input[1];

		ReadConsoleInput(keyboard, input, 1, &w);

		if (input[0].EventType == KEY_EVENT && input[0].Event.KeyEvent.bKeyDown)
		{
			switch (input[0].Event.KeyEvent.wVirtualScanCode) {
			case 72:
				selectedMenu = abs(selectedMenu - 1);
				break;

			case 80:
				selectedMenu++;
				break;

			case 28:
				return true;
			}
		}

		return false;
	}

public:
	static int abs(int val) {
		return val > 0 ? val : val * -1;
	}

	static int get_menu() {
		int selectedMenu = 0;
		char key;

		do {
			refresh_menu(selectedMenu);
		} while (!handle_keys(selectedMenu));

		return selectedMenu % 3;
	}

	static void set_console_title() {
		TCHAR title[MAX_PATH];

		StringCchPrintf(title, MAX_PATH, TEXT("Chain - Console APP. Powered by Vadim Prokopchuk."));
		SetConsoleTitle(title);
	}

	static void resize_window(int width, int height)
	{
		HWND console = GetConsoleWindow();
		RECT r;
		GetWindowRect(console, &r);
		MoveWindow(console, r.left, r.top, width, height, TRUE);
	}

	static void clearScreen()
	{
		HANDLE hStdOut;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD count;
		DWORD cellCount;
		COORD homeCoords = { 0, 0 };

		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

		if (hStdOut == INVALID_HANDLE_VALUE) {
			return;
		}

		if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
			return;
		}

		cellCount = csbi.dwSize.X *csbi.dwSize.Y;

		if (!FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellCount, homeCoords, &count)) {
			return;
		}

		if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count)) {
			return;
		}

		SetConsoleCursorPosition(hStdOut, homeCoords);
	}

	static std::string readString(std::string msg) {
		std::cout << msg;
		std::string value;
		std::getline(std::cin, value);

		return value;
	}

	template<class T>
	static T readValue(std::string msg) {
		T value;

		std::cout << msg;

		while ((!(std::cin >> value)) || (std::cin.peek() != '\n')) {
			std::cout << "Invalid value. Try again: ";
			std::cin.clear();
			std::cin.ignore(5, '\n');
		}

		return value;
	}

	template <class T>
	static T readWithCheck(std::string msg, T min) {
		T res = readValue<T>(msg);

		while (res < min)
		{
			std::cout << "Enter value greather " << min << std::endl;
			res = readValue<T>(msg);
		}

		cin.ignore();

		return res;
	}
};
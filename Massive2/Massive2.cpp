#include <iostream>
#include <windows.h> // COORD HANDLE SetConsoleTextAttribute SetConsoleCursorPosition
#include <conio.h> // _getch
using namespace std;

enum GameObject : short { HALL, WALL, COIN, ENEMY, PILL };
enum Color : short {
	BLACK, DARKBLUE, DARKGREEN, TURQUOISE, DARKRED,
	PURPLE, DARKYELLOW, GREY, DARKGREY, BLUE, GREEN,
	CYAN, RED, PINK, YELLOW, WHITE
};
enum Key : short {
	LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80,
	ENTER = 13, SPACE = 32, ESCAPE = 27, BACKSPACE = 8
};

int main() {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	srand(time(0));
	rand();
	system("title Bomberman");

	// cursor hide
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = false;
	cursor.dwSize = 100; // 1-100
	SetConsoleCursorInfo(h, &cursor);

	const int HEIGHT = 25;
	const int WIDTH = 65;
	int map[HEIGHT][WIDTH] = {};
	int coins = 0;
	int health = 100;
	// ASCII Table
	//for (int i = 0; i < 256; i++)
	//{
	//	cout << (char)i << " - " << i << "\n";
	//}

	// MODEL (логіка заповнення масиву)
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			map[y][x] = rand() % 5; // 0 1 2 3

			// рамка по краях
			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
				map[y][x] = GameObject::WALL;

			// отвори для входу і виходу
			if (x == 0 && y == 2 ||
				x == 1 && y == 2 ||
				x == 2 && y == 2 ||

				x == WIDTH - 1 && y == HEIGHT - 3 ||
				x == WIDTH - 2 && y == HEIGHT - 3 ||
				x == WIDTH - 3 && y == HEIGHT - 3)
				map[y][x] = GameObject::HALL;

			if (map[y][x] == GameObject::ENEMY) {
				int r = rand() % 10; // 0 1 2 3 4 5 6 7 8 9
				if (r != 0) {
					map[y][x] = GameObject::HALL;
				}
			}
			else if (map[y][x] == GameObject::PILL) {
				int r = rand() % 20; // 0 1 2 3 4 5 6 7 8 9
				if (r != 0) {
					map[y][x] = GameObject::HALL;
				}
			}
		}
	}

	///////////////////////////////////////////////////////////

	// VIEW (подання - як буде виглядати ігрова локація)
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			switch (map[y][x]) {
			case GameObject::HALL: // 0
				// як показати коридор
				SetConsoleTextAttribute(h, Color::BLACK);
				cout << " ";
				break;
			case GameObject::WALL: // 1
				SetConsoleTextAttribute(h, Color::DARKGREEN);
				// cout << "#";
				cout << (char)178;
				break;
			case GameObject::COIN:
				SetConsoleTextAttribute(h, Color::YELLOW);
				coins++;
				cout << ".";
				break;
			case GameObject::ENEMY:
				SetConsoleTextAttribute(h, Color::RED);
				cout << "O";
				break;
				// case GameObject::SMTH_ELSE:
					// як показати щось інше
				//	break;
			case GameObject::PILL:
				SetConsoleTextAttribute(h, Color::GREEN);
				cout << "+";
				break;
			}
		cout << "\n";
	}

	////////////////////////////////////////////////////////////
	// показ ГГ
	COORD hero;
	hero.X = 0;
	hero.Y = 2;
	SetConsoleCursorPosition(h, hero);
	SetConsoleTextAttribute(h, Color::BLUE);
	cout << "O";

	int coins_collected = 0;
	COORD coins_info;
	coins_info.X = WIDTH + 1;
	coins_info.Y = 0;
	SetConsoleCursorPosition(h, coins_info);
	SetConsoleTextAttribute(h, Color::DARKYELLOW);
	cout << "COINS: ";
	SetConsoleTextAttribute(h, Color::YELLOW);
	cout << coins_collected;
	COORD xp;
	xp.X = WIDTH + 1;
	xp.Y = 1;
	SetConsoleCursorPosition(h, xp);
	SetConsoleTextAttribute(h, Color::DARKGREEN);
	cout << "HEALTH: ";
	COORD point;
	point.X = WIDTH + 9;
	point.Y = 1;
	SetConsoleCursorPosition(h, point);
	SetConsoleTextAttribute(h, Color::GREEN);
	cout << health;


	// ігровий двигунчик
	while (true) {
		int code = _getch(); // get character
		if (code == 224) {
			code = _getch();
		}

		COORD old_position = hero;
		COORD oldpoint = point;
		bool has_been_moved = false; // false - пересування не було

		// cout << code << "\n";
		switch (code) {
		case Key::LEFT:
			if (hero.X > 0 && map[hero.Y][hero.X - 1] != GameObject::WALL) {
				has_been_moved = true;
				hero.X--;
			}
			break;
		case Key::RIGHT:
			if (map[hero.Y][hero.X + 1] != GameObject::WALL) {
				has_been_moved = true;
				hero.X++;
			}
			break;
		case Key::UP:
			if (map[hero.Y - 1][hero.X] != GameObject::WALL) {
				has_been_moved = true;
				hero.Y--;
			}
			break;
		case Key::DOWN:
			if (map[hero.Y + 1][hero.X] != GameObject::WALL) {
				has_been_moved = true;
				hero.Y++;
			}
			break;
		}

		if (has_been_moved) {
			// стирання ГГ в "старих" координатах
			SetConsoleCursorPosition(h, old_position);
			SetConsoleTextAttribute(h, Color::BLACK);
			cout << " ";
			// перемальовка ГГ в нових координатах
			SetConsoleCursorPosition(h, hero);
			SetConsoleTextAttribute(h, Color::BLUE);
			cout << "O";
			if (hero.X == WIDTH - 1 && hero.Y == HEIGHT - 3 ||
				hero.X == WIDTH - 2 && hero.Y == HEIGHT - 3 ||
				hero.X == WIDTH - 3 && hero.Y == HEIGHT - 3){ 
				COORD win;
				win.X = WIDTH + 1;
				win.Y = 2;
				SetConsoleCursorPosition(h, win);
				SetConsoleTextAttribute(h, Color::RED);
				cout << "YOU WIN!!! ";
				break;
			}
		}

		// приклад перевірки на перетинання з якимось об'єктом в лабірінті
		if (map[hero.Y][hero.X] == GameObject::COIN) {
			// cout << "COIN";
			coins--;
			coins_collected++;
			map[hero.Y][hero.X] = GameObject::HALL;
			COORD coins_info;
			coins_info.X = WIDTH + 1;
			coins_info.Y = 0;
			SetConsoleCursorPosition(h, coins_info);
			SetConsoleTextAttribute(h, Color::DARKYELLOW);
			cout << "COINS: ";
			SetConsoleTextAttribute(h, Color::YELLOW);
			cout << coins_collected;
			if (coins == 0) {
				COORD win;
				win.X = WIDTH + 1;
				win.Y = 2;
				SetConsoleCursorPosition(h, win);
				SetConsoleTextAttribute(h, Color::RED);
				cout << "YOU WIN!!!(All coins collected) ";
				break;
			}
		}

		if (map[hero.Y][hero.X] == GameObject::ENEMY) {
			health = health - 25;
			map[hero.Y][hero.X] = GameObject::HALL;
			COORD xp;
			xp.X = WIDTH + 1;
			xp.Y = 1;
			SetConsoleCursorPosition(h, oldpoint);
			SetConsoleTextAttribute(h, Color::BLACK);
			cout << "    ";
			SetConsoleCursorPosition(h, xp);
			SetConsoleTextAttribute(h, Color::DARKGREEN);
			cout << "HEALTH: ";
			SetConsoleTextAttribute(h, Color::GREEN);
			cout << health;
			COORD point;
			point.X = WIDTH + 9;
			point.Y = 1;
			SetConsoleCursorPosition(h, point);
			SetConsoleTextAttribute(h, Color::GREEN);
			cout << health;
			if (health == 0) {
				COORD loose;
				loose.X = WIDTH + 1;
				loose.Y = 2;
				SetConsoleCursorPosition(h, loose);
				SetConsoleTextAttribute(h, Color::RED);
				cout << "YOU LOSE! ";
				break;
			}
		}
		if (map[hero.Y][hero.X] == GameObject::PILL) {
			map[hero.Y][hero.X] = GameObject::HALL;
			if (health < 100) {
				health = health + 25;
				COORD xp;
				xp.X = WIDTH + 1;
				xp.Y = 1;
				SetConsoleCursorPosition(h, oldpoint);
				SetConsoleTextAttribute(h, Color::BLACK);
				cout << "    ";
				SetConsoleCursorPosition(h, xp);
				SetConsoleTextAttribute(h, Color::DARKGREEN);
				cout << "HEALTH: ";
				SetConsoleTextAttribute(h, Color::GREEN);
				cout << health;
				COORD point;
				point.X = WIDTH + 9;
				point.Y = 1;
				SetConsoleCursorPosition(h, point);
				SetConsoleTextAttribute(h, Color::GREEN);
				cout << health;
			}
			else {
				COORD xp;
				xp.X = WIDTH + 1;
				xp.Y = 1;
				SetConsoleCursorPosition(h, oldpoint);
				SetConsoleTextAttribute(h, Color::BLACK);
				cout << "    ";
				SetConsoleCursorPosition(h, xp);
				SetConsoleTextAttribute(h, Color::DARKGREEN);
				cout << "HEALTH: ";
				SetConsoleTextAttribute(h, Color::GREEN);
				cout << health;
				COORD point;
				point.X = WIDTH + 9;
				point.Y = 1;
				SetConsoleCursorPosition(h, point);
				SetConsoleTextAttribute(h, Color::GREEN);
				cout << health;
			}
		}

	}


	Sleep(INFINITE);
}


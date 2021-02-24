#pragma once
#include "list.h"

class Exit
{
	int rows; // количество строк лабиринта
	int cols; // количество столбцов лабиринта
	wchar_t **labyrinth; // указатель на двумерный динамический массив
	List way; // сохраняет правильный путь ведущий к выходу (в отличии от стека в генераторе лабиринта данный стек сохраняет путь каждой клетки)
	int speed;
	bool animation;
	bool rec(Coord); // принимает координату и начинает искать выход, обладает способностью самозапуска, если столкнется с разветлением
	bool find_door(); // поиск входа в лабиринт
	bool chek_up(Coord); // проверяет что находится сверху от текущей позиции
	bool chek_right(Coord); // проверяет что находится справа от текущей позиции
	bool chek_down(Coord); // проверяет что находится снизу от текущей позиции
	bool chek_left(Coord); // проверяет что находится слева от текущей позиции
public:
	Exit(wchar_t **, int, int); // констурктор с параметрами
	~Exit(); // деструктор
	bool search_out(); // подготавливает программу к запуску функции rec
	void print(); // печать выхода
	void print_coord(); // печать координат
	void update(wchar_t **, int, int);
	void log(wchar_t file_name[]);
	void animationOnOff();
	void setAnimationSpeed(int);
	bool getAnimationStatus();
};
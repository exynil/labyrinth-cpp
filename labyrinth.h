#pragma once
#include "list.h"

class Labyrinth
{
	Coord begin; // координаты входа в лабиринт
	Coord end; // координата выхода из лабиринта
	List way; // хранит координаты пройденного путя
	bool record; // включатель и выключатель записи (обрисовка пошагового строения лабиринта в текстовом документе)
	int rows; // количесвто строк
	int cols; // количество столбцов
	wchar_t **labyrinth; // холст для лабиринта
	bool exception[4]; // временное сохранение запрещенных направлений в на определенной позиции
	bool build_up(int length); // строит проход на вверх от текущей позиции
	bool build_right(int length); // строит проход на право от текущей позиции
	bool build_down(int length); // строит проход вниз от текущей позиции
	bool build_left(int length); // строит проход на лево от текущей позиции
	void reset_exception(); // открывает пути на все направления
	bool chek(); // проверяет не достигла ли текущая позиция области выхода из лабиринта
	void fill(wchar_t);
public:
	Labyrinth(int r = 50, int c = 50, int b_r = 0, int b_c = 1);
	~Labyrinth();
	bool build(); // генерирует лабиринт
	void print(); // печатает лабиринт
	void recordOnOff(); // включает/отключает покадровую запись
	void update(int, int);
	wchar_t **getArray(); // передает указатель на лабиринт из массива
	void log(wchar_t file_name[]); // записывает каждый построенных проход
	int getRows()
	{
		return rows;
	}
	int getCols()
	{
		return cols;
	}
	bool getRecordStatus()
	{
		return record;
	}
};
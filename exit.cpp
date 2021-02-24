#pragma once
#include <iostream>
#include "exit.h"
#include <windows.h>
#include <fstream>
#include <codecvt> // не знаю что это, но это позволяет записывать в файл unicode символы

using namespace std;

Exit::Exit(wchar_t **lab, int r, int c) // КОНСТРУКТОР С ПАРАМЕТРАМИ
{
	rows = r; // устанавливаем количество строк
	cols = c; // устанавливаем количество столбцов
	labyrinth = new wchar_t *[rows]; // выделяем память для строк
	speed = 20;
	animation = false;
	for (int i = 0; i < rows; i++)
	{
		labyrinth[i] = new wchar_t[cols]; // выделяем память для столбцов
	}
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			labyrinth[i][j] = lab[i][j]; // копирую исходный двумерный лабиринт для сканирования
		}
	}
}

Exit::~Exit() // ДЕСТРУКТОР
{

	for (int i = 0; i < rows; i++)
	{
		delete[] labyrinth[i];
	}
	delete[] labyrinth;
}

bool Exit::search_out() // ПОДГОТОВКА К ПОИСКУ ВЫХОДА
{
	if (find_door()) // находим вход в лабиринт
	{
		if (animation)
		{
			system("cls");
			print();
		}
		if (rec(way.pop()))
			return true;
	}
	return false;
}

bool Exit::rec(Coord pos) // РЕКУРСИВНАЯ ФУНКЦИЯ ПРОБЕГАЕТ ПО ПРОХОДАМ И ИЩЕТ ВЫХОД
{
	COORD position = { 0, 0 }; //позиция x и y
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


	Coord temp;
	int count; // счетчик количества пустых блоков вокруг текущей позиции
	way.push(pos);
	while (true) // бесконечный цикл
	{
		count = 0; // обнуляем счетчик

		if (chek_up(pos)) // проверяем есть ли проход на верх, если да увеличиваем счетчик
			count++;
		if (chek_right(pos)) // проверяем есть ли проход на право, если да увеличиваем счетчик
			count++;
		if (chek_down(pos)) // проверяем есть ли проход вниз, если да увеличиваем счетчик
			count++;
		if (chek_left(pos)) // проверяем есть ли проход на лево, если да увеличиваем счетчик
			count++;

		if (count == 1) // если насчитанных проходов единица, выполняем следующие действия
		{
			if (chek_up(pos)) // если есть проход на верх
			{
				if (animation)
				{
					position = { short(pos.col), short(pos.row) };
					SetConsoleCursorPosition(hConsole, position);
					wcout << wchar_t(8593);
					Sleep(speed);
				}

				labyrinth[pos.row][pos.col] = wchar_t(8593); // нашу текущую позицию отмечаем как пройденной меткой "стрелка"
				if (labyrinth[pos.row - 1][pos.col] == wchar_t(9829)) // если обнаружен символ выхода из лабиринта wchar_t(9829) возвращаем true
				{
					if (animation)
					{
						position = { 0, short(pos.row + 1) };
						SetConsoleCursorPosition(hConsole, position);
					}
					pos.row = pos.row - 1;
					way.push(pos);
					return true;
				}
				pos.row = pos.row - 1;
				way.push(pos);
				continue; // продолжаем бесконечный цикл пропуская текущую итерацию
			}
			if (chek_right(pos)) // [ниже анологичные действия см. строки 68 - 75]
			{
				if (animation)
				{
					position = { short(pos.col), short(pos.row) };
					SetConsoleCursorPosition(hConsole, position);
					wcout << wchar_t(8594);
					Sleep(speed);
				}

				labyrinth[pos.row][pos.col] = wchar_t(8594);
				if (labyrinth[pos.row][pos.col + 1] == wchar_t(9829))
				{
					if (animation)
					{
						position = { 0, short(pos.row + 3) };
						SetConsoleCursorPosition(hConsole, position);
					}
					pos.col = pos.col + 1;
					way.push(pos);
					return true;
				}
				pos.col = pos.col + 1;
				way.push(pos);
				continue;
			}
			if (chek_down(pos)) // [ниже анологичные действия см. строки 68 - 75]
			{
				if (animation)
				{
					position = { short(pos.col), short(pos.row) };
					SetConsoleCursorPosition(hConsole, position);
					wcout << wchar_t(8595);
					Sleep(speed);
				}
				labyrinth[pos.row][pos.col] = wchar_t(8595);
				if (labyrinth[pos.row + 1][pos.col] == wchar_t(9829))
				{
					if (animation)
					{
						position = { 0, short(pos.row + 3) };
						SetConsoleCursorPosition(hConsole, position);
					}
					pos.row = pos.row + 1;
					way.push(pos);
					return true;
				}
				pos.row = pos.row + 1;
				way.push(pos);
				continue;
			}
			if (chek_left(pos)) // [ниже анологичные действия см. строки 68 - 75]
			{
				if (animation)
				{
					position = { short(pos.col), short(pos.row) };
					SetConsoleCursorPosition(hConsole, position);
					wcout << wchar_t(8592);
					Sleep(speed);
				}
				labyrinth[pos.row][pos.col] = wchar_t(8592);
				if (labyrinth[pos.row][pos.col - 1] == wchar_t(9829))
				{
					if (animation)
					{
						position = { 0, short(pos.row + 3) };
						SetConsoleCursorPosition(hConsole, position);
					}
					pos.col = pos.col - 1;
					way.push(pos);
					return true;
				}
				pos.col = pos.col - 1;
				way.push(pos);
				continue;
			}
		}
		if (count == 0 || count == 2 || count == 3) // если подсчитанных свободных проходов 0 или больше 1 выходим из безконечного цикла
		{
			break;
		}
	}
	if (count > 1 && count < 4) // если проходов больше 1 и меньше 4, мы находимся на разветлении, запускаем рекурсию на все направления кроме той откуда мы сами пришли
	{
		if (animation)
		{
			position = { short(pos.col), short(pos.row) };
			SetConsoleCursorPosition(hConsole, position);
			wcout << wchar_t(9830);
			Sleep(speed);
		}
		if (chek_up(pos)) // если есть проход на верх
		{
			labyrinth[pos.row][pos.col] = wchar_t(9830); // на текущую позицию ставлю метку разветления пути "S"
			if (labyrinth[pos.row - 1][pos.col] == wchar_t(9829))
			{
				pos.row = pos.row - 1;
				way.push(pos);
				return true;
			}
			temp.row = pos.row - 1;
			temp.col = pos.col;
			if (rec(temp)) // запускаю рекурсивную функцию для иследования разветленного прохода
				return true;
		}
		if (chek_right(pos)) // [ниже анологичные действия см. строки 114 - 121]
		{
			labyrinth[pos.row][pos.col] = wchar_t(9830);
			if (labyrinth[pos.row][pos.col + 1] == wchar_t(9829))
			{
				pos.col = pos.col + 1;
				way.push(pos);
				return true;
			}
			temp.row = pos.row;
			temp.col = pos.col + 1;
			if (rec(temp))
				return true;
		}
		if (chek_down(pos)) // [ниже анологичные действия см. строки 114 - 121]
		{
			labyrinth[pos.row][pos.col] = wchar_t(9830);
			if (labyrinth[pos.row + 1][pos.col] == wchar_t(9829))
			{
				pos.row = pos.row + 1;
				way.push(pos);
				return true;
			}
			temp.row = pos.row + 1;
			temp.col = pos.col;
			if (rec(temp))
				return true;
		}
		if (chek_left(pos)) // [ниже анологичные действия см. строки 114 - 121]
		{
			labyrinth[pos.row][pos.col] = wchar_t(9830);
			if (labyrinth[pos.row][pos.col - 1] == wchar_t(9829))
			{
				pos.col = pos.col - 1;
				way.push(pos);
				return true;
			}
			temp.row = pos.row;
			temp.col = pos.col - 1;
			if (rec(temp))
				return true;
		}
	}
	if (count == 0) // если количество проходов вокруг текущей позиции 0, мы в тупике, выполнеям действия для возвращения назад
	{
		while (true) // бесконечный цикл
		{
			if (pos.row == -1)
			{
				wcout << "COUNT = 0, RW = -1, EXIT IS NOT FOUND!\n";
				exit(0);
			}
			// если метка на текущей позиции не равна метке разветления "wchar_t(9830)" или если метка является меткой разветления но вокруг нету проходов.
			if ((labyrinth[pos.row][pos.col] != wchar_t(9830)) || (labyrinth[pos.row][pos.col] == wchar_t(9830) && !chek_up(pos) && !chek_right(pos) && !chek_down(pos) && !chek_left(pos)))
			{
				if (animation)
				{
					position = { short(pos.col), short(pos.row) };
					SetConsoleCursorPosition(hConsole, position);
					wcout << wchar_t(9618);
					Sleep(speed);
				}
				labyrinth[pos.row][pos.col] = wchar_t(9618); // отмечаем текущую позицию как тупиковый проход
			}
			if (labyrinth[pos.row][pos.col] == wchar_t(9830)) // если мы дошли до позиции с меткой разветления "wchar_t(9830)"
			{
				way.push(pos);
				break; // выходим из бесконечного цикла
			}
			pos = way.pop();
		}
	}
	return false;
}


bool Exit::chek_up(Coord pos) // ФУНКЦИЯ ПРОВЕРЯЕТ ЧТО НАХОДИТСЯ СВЕРХУ ОТ ТЕКУЩЕЙ ПОЗИЦИИ (ПРИНИМАЕТ КООРДИНАТЫ ТЕКУЩЕЙ ПОЗИЦИИ)
{
	if (pos.row != 0) // если текущая строка не является нулевой (не является краем лабиринта)
		if (labyrinth[pos.row - 1][pos.col] == ' ' || labyrinth[pos.row - 1][pos.col] == wchar_t(9829))  // если проход есть или же если достугнут символ выхода из лабиринта
			return true;
	return false;
}

bool Exit::chek_right(Coord pos)  // [ниже анологичные действия см. строки 170 - 174]
{
	if (pos.col != cols - 1)
		if (labyrinth[pos.row][pos.col + 1] == ' ' || labyrinth[pos.row][pos.col + 1] == wchar_t(9829))
			return true;
	return false;
}

bool Exit::chek_down(Coord pos)  // [ниже анологичные действия см. строки 170 - 174]
{
	if (pos.row != rows - 1)
		if (labyrinth[pos.row + 1][pos.col] == ' ' || labyrinth[pos.row + 1][pos.col] == wchar_t(9829))
			return true;
	return false;
}

bool Exit::chek_left(Coord pos)  // [ниже анологичные действия см. строки 170 - 174]
{
	if (pos.col != 0)
		if (labyrinth[pos.row][pos.col - 1] == ' ' || labyrinth[pos.row][pos.col - 1] == wchar_t(9829))
			return true;
	return false;
}

bool Exit::find_door() // ФУНКЦИЯ ИЩЕТ ВХОД В ЛАБИРИНТ ПРОБЕГАЯ ПО КРАЯМ ДВУМЕРНОГО МАССИВА
{
	Coord pos;
	for (int i = 0; i < rows; i++)
	{
		if (labyrinth[i][0] == ' ') // проверка левого края лабиринта
		{
			pos.row = i;
			pos.col = 0;
			way.push(pos);
			return true;
		}
		else if (labyrinth[i][cols - 1] == ' ') // проверка правого края лабиринта
		{
			pos.row = i;
			pos.col = cols - 1;
			way.push(pos);
			return true;
		}
	}
	for (int i = 0; i < cols; i++)
	{
		if (labyrinth[0][i] == ' ') // проверка верхнего края лабиринта
		{
			pos.row = 0;
			pos.col = i;
			way.push(pos);
			return true;
		}
		else if (labyrinth[rows - 1][i] == ' ') // проверка нижнего края лабиринта
		{
			pos.row = rows - 1;
			pos.col = i;
			way.push(pos);
			return true;
		}
	}
	return false; // вход не найден, возвращаем false
}

void Exit::print() // ФУНКЦИЯ ПЕЧАТАЕТ МАССИВ
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			wcout << labyrinth[i][j];
		}
		wcout << endl;
	}
}

void Exit::print_coord() // ФУНКЦИЯ ПЕЧАТАЕТ КООРДИНАТЫ
{
	way.printBack();
}

void Exit::update(wchar_t **lab, int r, int c) // обновление лабиринта
{
	for (int i = 0; i < rows; i++)
	{
		delete[] labyrinth[i];
	}
	delete[] labyrinth;
	rows = r;
	cols = c;
	labyrinth = new wchar_t *[rows]; // выделяем память для строк
	for (int i = 0; i < rows; i++)
	{
		labyrinth[i] = new wchar_t[cols]; // выделяем память для столбцов
	}
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			labyrinth[i][j] = lab[i][j]; // копирую исходный двумерный лабиринт для сканирования
		}
	}
}

void Exit::log(wchar_t file_name[])
{
	wofstream log(file_name, ios::app);
	log.imbue(locale(locale("C"), new codecvt_utf8<wchar_t, 0x10ffff, generate_header>()));
	if (!log)
	{
		wcout << L"Сould not open file!\n";
		exit(0);
	}
	log << endl;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			log << labyrinth[i][j];
		}
		log << endl;
	}
	log.close();
}

void Exit::animationOnOff()
{
	if (animation)
		animation = false;
	else
		animation = true;
}

void Exit::setAnimationSpeed(int value)
{
	speed = value;
}

bool Exit::getAnimationStatus()
{
	return animation;
}
#pragma once
#include <iostream>
#include <ctime>
#include <fstream>
#include "labyrinth.h"
#include <codecvt> // не знаю что это, но это позволяет записывать в файл unicode символы

using namespace std;

Labyrinth::Labyrinth(int r, int c, int begin_row, int begin_col) // констурктор с параметрами
{
	srand(unsigned int(time(NULL)));
	begin.row = begin_row; // установка координат начала лабиринта (строка)
	begin.col = begin_col; // установка координат начала лабиринта (столбец)
	end.row = r - 1; // установка координат конца лабиринта (строка)
	end.col = c - 1; // установка координат конца лабиринта (столбец)
	record = false; // отключение записи
	rows = r; // количество строк в лабиринте
	cols = c; // количество столбцов в лабиринте
	labyrinth = new wchar_t *[rows]; // выделение памяти для строк
	for (int i = 0; i < rows; i++)
	{
		labyrinth[i] = new wchar_t[cols]; // выделение памяти для столбцов
	}
	fill('+');
}

Labyrinth::~Labyrinth() // деструктор
{
	for (int i = 0; i < rows; i++)
	{
		delete[] labyrinth[i]; // особождение памяти от столбцов
	}
	delete[] labyrinth; // освобождение памяти от строк
}

bool Labyrinth::build() // генератор лабиринта
{
	if (record) // если запись включена очищаем файл от прошлых записей и добавляем заголовок
	{
		wofstream addTitle(L"log.txt", ios::binary);
		addTitle.imbue(locale(locale("C"), new codecvt_utf8<wchar_t, 0x10ffff, generate_header>()));
		if (!addTitle)
		{
			wcout << L"Сould not open file!\n";
			exit(0);
		}
		addTitle << L"LABYRINTH\n\n";
		addTitle.close();
	}
	way.clear(); // очищаем путь
	Coord temp; // создаем переменную для временного хранения координат
	way.push(begin); // добавляем в пройденный путь координты начала лабиринта
	reset_exception(); // открываем пути на все направления (массив сохраняет в себе направления для текущей позиции, в которые запрещается двигаться из-за наличия стен)
	int rand_length; // сохраняет рандомное значение длины генерируемого прохода
	int rand_direction; // сохраняет рандомное направление лабиринта 1 - вверх, 2 - впарво, 3 - вниз, 4 - влево
	int rand_value; // случайное число (в определенный момент делается откат по пройденному пути назад и начать генерировать маршрут с другой позиции, чтобы создать тупиковый проход)
	int count = 0; // счетчик построенных тунелей
	fill(wchar_t(9608)); // заполняем лабиринт стенами
	labyrinth[begin.row][begin.col] = ' '; // рисуем пробел на координате входа в лабиринт
	while (true) // бесконечный цикл
	{
		if (chek()) // проверка: не достигла ли текущая позиция выхода из лабиринта
		{
			break;
		}
		if (count > 30) // если созданных проходов построено более чем 30, выполняем следующие действия
		{
			rand_value = rand() % 15; // генерируем случайное число
			for (int i = 0; i < rand_value; i++)
			{
				temp = way.pop(); // возвращаемся назад по пройденному пути
				if (temp.row == -1) // если извлекаемая координа равна -1 (исключительная и редкая ситуация когда лабиринт генерируясь перекрывает себе путь к выходу)
				{
					build(); // повторно запускаем постройку лабиринта
				}
			}
			count = 0; // обнуляем счетчик
			continue; // пропускаем итерацию
		}
		rand_length = rand() % 2 + 4; // генерируем случайную длину прохода от 2 до 5
		rand_direction = rand() % 4 + 1; // генерируем случайное направления постройки прохода
		if (exception[rand_direction - 1] == 0) // если такое напрвление было ранее заблокировано, пропускаем итерацию
			continue;
		switch (rand_direction)
		{
		case 1:
			if (build_up(rand_length)) // запускаем функцию которая строит проход вверх от текущей позиции
			{
				reset_exception(); // открываем пути на все направления
				count++; // учеличиваем счетчик количества построенных тунелей с момента сброса счетчика
			}
			else // если проход построить не удалось устанавливаем запрет на текущее направление
				exception[0] = 0;
			break;
		case 2:
			if (build_right(rand_length)) // [ниже анологичные действия см. строки 86 - 93]
			{
				reset_exception();
				count++;
			}
			else
				exception[1] = 0;
			break;
		case 3:
			if (build_down(rand_length)) // [ниже анологичные действия см. строки 86 - 93]
			{
				reset_exception();
				count++;
			}
			else
				exception[2] = 0;
			break;
		case 4:
			if (build_left(rand_length)) // [ниже анологичные действия см. строки 86 - 93]
			{
				reset_exception();
				count++;
			}
			else
				exception[3] = 0;
			break;
		}
		if (!exception[0] && !exception[1] && !exception[2] && !exception[3]) // если все навпарвления заблокированы, возвращаемся назад по пройденному пути
		{
			if (count > 30) // если построено больше 15 проходов, количество шагов назад генерируется случайно от 3 до 10
			{
				rand_value = rand() % 15 + 1;
				for (int i = 0; i < rand_value; i++)
				{
					temp = way.pop();
					if (temp.row == -1)
					{
						build();
					}
				}
				count = 0; // сбрасываем счетчик построенных проходов
			}
			else // если построено меньше 15 проходов, количество шагов назад 1
			{
				temp = way.pop();
				if (temp.row == -1) // если извлекаемая координа равна -1 (исключительная и редкая ситуация когда лабиринт генерируясь перекрывает себе путь к выходу)
				{
					build(); // если извлекаемая координа равна -1 (исключительная и редкая ситуация когда лабиринт генерируясь перекрывает себе путь к выходу)
				}
			}
			reset_exception(); // открываем пути на все направления
		}
	}
	return true;
}

bool Labyrinth::build_up(int length) // постройка прохода вверх
{
	Coord pos; // сохраняет текущую позицию
	pos = way.pop(); // извлекаем из стека наше местоположение в лабиринте
	if (pos.row == -1) // если извлекаемая координа равна -1 (исключительная и редкая ситуация когда лабиринт генерируясь перекрывает себе путь к выходу)
		return false;
	way.push(pos); // заносим извлеченную координату обартно в стек
	if (pos.col != 0 && pos.col != cols - 1 && pos.row > 1) // если текущая позиция по столбцу не равна 0, не равна краю и по строке больше единицы
	{
		while (pos.row - length < 1) // уменьшаем длину пока конец будущего прохода на верх не будет находится выше 0 (продотвращаем выход прохода за границы лабиринта)
			length--;
		for (int i = pos.row - 1; i > pos.row - length - 1 && length > 2; i--) // проверяем будущий проход 
			if (labyrinth[i][pos.col] == ' ' || labyrinth[i][pos.col - 1] == ' ' || labyrinth[i][pos.col + 1] == ' ') // если на пути слева, справа или на текущей позиции находятся другие проходы
			{
				length--; // уменьшаем длину
				i = pos.row; // устанавливаем итератор на первоначальное значение
			}
		if (length > 2) // если длина больше 2, можно строить проход вверх
		{
			for (int i = pos.row - 1; i > pos.row - length; i--)
				labyrinth[i][pos.col] = ' '; // строим проход на верх
			pos.row = pos.row - length + 1; // текущую позицию устанавливаем на конец прохода
			way.push(pos); // текущую позицию заносим в стек
			if (record) // если включена запись
				log(L"log.txt"); // записываем текущее состояние лабиринта в текстовый файл
			return true;
		}
	}
	return false;
}

bool Labyrinth::build_right(int length) // [ниже анологичные действия см. строки 150 - 179]
{
	Coord pos;
	pos = way.pop();
	if (pos.row == -1)
	{
		return false;
	}
	way.push(pos);
	if (pos.row != 0 && pos.row != rows - 1 && pos.col < cols - 2)
	{
		while (pos.col + length > cols - 1)
			length--;
		for (int i = pos.col + 1; i < pos.col + length + 1; i++)
			if (labyrinth[pos.row][i] == ' ' || labyrinth[pos.row - 1][i] == ' ' || labyrinth[pos.row + 1][i] == ' ')
			{
				length--;
				i = pos.col;
			}
		if (length > 2)
		{
			for (int i = pos.col + 1; i < pos.col + length; i++)
				labyrinth[pos.row][i] = ' ';
			pos.col = pos.col + length - 1;
			way.push(pos);
			if (record)
				log(L"log.txt");
			return true;
		}
	}
	return false;
}

bool Labyrinth::build_down(int length) // [ниже анологичные действия см. строки 150 - 179]
{
	Coord pos;
	pos = way.pop();
	if (pos.row == -1)
	{
		return false;
	}
	way.push(pos);
	if (pos.col != 0 && pos.col != cols - 1 && pos.row < rows - 2)
	{
		while (pos.row + length > rows - 1)
			length--;
		for (int i = pos.row + 1; i < pos.row + length + 1; i++)
			if (labyrinth[i][pos.col] == ' ' || labyrinth[i][pos.col - 1] == ' ' || labyrinth[i][pos.col + 1] == ' ')
			{
				length--;
				i = pos.row;
			}
		if (length > 2)
		{
			for (int i = pos.row + 1; i < pos.row + length; i++)
			{
				labyrinth[i][pos.col] = ' ';
			}
			pos.row = pos.row + length - 1;
			way.push(pos);
			if (record)
				log(L"log.txt");
			return true;
		}
	}
	return false;
}

bool Labyrinth::build_left(int length) // [ниже анологичные действия см. строки 150 - 179]
{
	Coord pos;
	pos = way.pop();
	if (pos.row == -1)
	{
		return false;
	}
	way.push(pos);
	if (pos.row != 0 && pos.row != rows - 1 && pos.col > 1)
	{
		while (pos.col - length < 1)
			length--;
		for (int i = pos.col - 1; i > pos.col - length - 1; i--)
			if (labyrinth[pos.row][i] == ' ' || labyrinth[pos.row - 1][i] == ' ' || labyrinth[pos.row + 1][i] == ' ')
			{
				length--;
				i = pos.col;
			}
		if (length > 2)
		{
			for (int i = pos.col - 1; i > pos.col - length; i--)
			{
				labyrinth[pos.row][i] = ' ';
			}
			pos.col = pos.col - length + 1;
			way.push(pos);
			if (record)
				log(L"log.txt");
			return true;
		}
	}
	return false;
}

bool Labyrinth::chek()
{
	Coord pos;
	pos = way.pop();
	way.push(pos);
	if (pos.row > end.row - 8 && pos.row < end.row && pos.col > end.col - 8 && pos.col < end.col)
	{
		while (pos.row != rows)
		{
			pos.row++;
			if (pos.row == rows - 1)
			{
				labyrinth[pos.row][pos.col] = wchar_t(9829); // '#'
				if (record)
					log(L"log.txt");
				return true;
			}
			else
			{
				labyrinth[pos.row][pos.col] = ' ';
				if (record)
					log(L"log.txt");
			}
			way.push(pos);
		}
	}
	return false;
}

void Labyrinth::log(wchar_t file_name[])
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

void Labyrinth::print() // печать лабиринта
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

void Labyrinth::recordOnOff()
{
	if (record)
		record = false;
	else
		record = true;
}

void Labyrinth::update(int _rows, int _cols)
{
	for (int i = 0; i < rows; i++)
	{
		delete[] labyrinth[i];
	}
	delete[] labyrinth;
	rows = _rows;
	cols = _cols;
	end.row = rows - 1; // установка координат конца лабиринта (строка)
	end.col = cols - 1; // установка координат конца лабиринта (столбец)
	labyrinth = new wchar_t *[rows]; // выделение памяти для строк
	for (int i = 0; i < rows; i++)
	{
		labyrinth[i] = new wchar_t[cols]; // выделяем память для столбцов
	}
}

wchar_t ** Labyrinth::getArray()
{
	return labyrinth;
}

void Labyrinth::reset_exception() // сброс запрещенных направлений для постройки прохода в лабиринте
{
	for (int i = 0; i < 4; i++)
		exception[i] = 1;
}

void Labyrinth::fill(wchar_t symb)
{
	for (int i = 0; i < rows; i++) // заполняем двумерный массив стенами (символ стен wchar_t(219))
	{
		for (int j = 0; j < cols; j++)
		{
			labyrinth[i][j] = symb; // char(219)
		}
	}
}
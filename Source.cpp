#include <iostream>

#include "labyrinth.h" // генератор лабиринтов
#include "exit.h" // взломщик лабиринтов
#include <fstream>
#include <io.h> // подключено для использования Unicode
#include <fcntl.h> // подключено для использования Unicode
#include <codecvt> // не знаю что это, но это позволяет записывать в файл unicode символы
//#include <windows.h>
using namespace std;

int main()
{
	setlocale(0, "");
	_setmode(_fileno(stdout), _O_U16TEXT);
	int rows = 65, cols = 150;
	int choice;
	int count = 0;
	Labyrinth lab(rows, cols, 0, 1);
	Exit ex(lab.getArray(), lab.getRows(), lab.getCols());
	wofstream result;
	do
	{
		wcout << L"Выберите действие:\n";
		wcout << L"1  - сгенерировать лабиринт.\n";
		wcout << L"2  - вывод лабиринта.\n";
		wcout << L"3  - вывод решенного лабиринта.\n";
		wcout << L"4  - показать анимацию поиска выхода.\n";
		wcout << L"5  - изменить скорость анимации поиска выхода.\n";
		wcout << L"6  - распечатать координаты от входа до выхода.\n";
		wcout << L"7  - изменить параметры лабиринта.\n";
		wcout << L"8  - включить/отключить покадровую запись генерации лабиринта в файл log.txt.\n";
		wcout << L"9  - сохранить исходный и решенный лабиринт в файл save.txt.\n";
		wcout << L"10 - создатель.\n";
		wcout << L"0  - выход из программы.\n";
		wcout << L"\nВвод: ";
		cin >> choice;
		switch (choice)
		{
		case 0:
			exit(0);
			break;
		case 1:
			wcout << L"\nЛабиринт №" << ++count << ".\n\n";
			if (lab.getRecordStatus())
			{
				wcout << L"\nНе забудьте отключить покадровую запись для дальнейшей быстрой генерации...\n";
				wcout << L"\nВыполняется покадровая запись генерации лабиринта...\n\n";
			}
			lab.build(); // генерация лабиринта
			lab.print(); // печать лабиринта
			if (lab.getRecordStatus())
			{
				wcout << L"\n====================================================================================\n";
				wcout << L"\nПокадровая запись завершена. Запись располагается в папке с проектом.\n";
				wcout << L"\n====================================================================================\n\n";
			}
			break;
		case 2:
			wcout << L"\nЛабиринт №" << count << ".\n\n";
			lab.print(); // печать лабиринта
			break;
		case 3:
			wcout << L"\nРешенный лабиринт №" << count << ".\n\n";
			ex.update(lab.getArray(), lab.getRows(), lab.getCols()); // обновление лабиринта в объекте
			ex.search_out(); // поиск выхода
			ex.print(); // печать решенного лабиринта
			break;
		case 4:
			wcout << L"\nРешенный лабиринт №" << count << ".\n\n";
			ex.update(lab.getArray(), lab.getRows(), lab.getCols()); // обновление лабиринта в объекте
			ex.animationOnOff();
			ex.search_out(); // поиск выхода
			ex.animationOnOff();
			break;
		case 5:
			int speed;
			do
			{
				wcout << L"Введите скорость анимации (0 - 5000): ";
				cin >> speed;
			} while (speed < 0 && speed > 5000);
			ex.setAnimationSpeed(speed);
			wcout << L"Скорость анимации изменена на " << speed << ".\n\n";
			break;
		case 6:
			ex.update(lab.getArray(), lab.getRows(), lab.getCols()); // обновление лабиринта в объекте
			ex.search_out(); // поиск выхода
			wcout << L"\nКоординаты от входа до выхода.\n\n";
			ex.print_coord(); // печать координат
			break;
		case 7:
			wcout << L"\nПредупреждение: Чтобы предотвратить искажение лабиринта,\n";
			wcout << L"размер лабиринта должен быть меньше текущего размера окна.\n";
			wcout << L"В случае искажения укажите повторно размеры по меньше или измените размеры окна.\n\n";
			wcout << L"Введите высоту: ";
			cin >> rows;
			wcout << L"Введите ширину: ";
			cin >> cols;
			lab.update(rows, cols);
			wcout << L"\nИзменение параметров лабиринта...\n\n";
			wcout << L"\n====================================================================================\n";
			wcout << L"\nПараметры внесены.\n";
			wcout << L"\n====================================================================================\n\n";
			break;
		case 8:
			wcout << L"\nПредупреждение: Время записи зависит от размеров лабиринта и\n";
			wcout << L"мощности вашей системы.\n";
			lab.recordOnOff();
			wcout << L"Покадровая запись строения лабиринта включена.\n\n";
			break;
		case 9:
			wcout << L"Сохранение исходнго и решенного лабиринта в файл...\n";
			result.open(L"save.txt", ios::out);
			result << L"Labyrinth.\n\n";
			result.close();
			lab.log(L"save.txt");
			ex.update(lab.getArray(), lab.getRows(), lab.getCols()); // обновление лабиринта в объекте
			ex.search_out(); // поиск выхода
			ex.log(L"save.txt");
			wcout << L"\n====================================================================================\n";
			wcout << L"\nИсходный и решенный лабиринт сохранен в файл save.txt.\n";
			wcout << L"\n====================================================================================\n\n";
			break;
		case 10:
			wcout << L"\n====================================================================================\n";
			wcout << L"\nАвтор: Ким Максим\n";
			wcout << L"\nДата: 31.01.2018\n";
			wcout << L"\nВерсия: 0.2\n";
			wcout << L"\n====================================================================================\n\n";
			break;
		}
	} while (choice != 0);
	return 0;
}
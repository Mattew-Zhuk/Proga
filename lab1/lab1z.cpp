#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct MeterReading
{
	string resource;
	int year, month, day;
	double value;
	string color;
};

bool is_date (const string& s)
{
	int dots = 0;
	for (char c : s)
	{
		if (c == '.') dots++;
		else if (!isdigit (c)) return false;
	}
	if (dots != 2) return false;

	size_t p1 = s.find ('.');
	size_t p2 = s.find ('.', p1 + 1);
	if (p1 == string::npos || p2 == string::npos) return false;

	string y = s.substr (0, p1);
	string m = s.substr (p1 + 1, p2 - p1 - 1);
	string d = s.substr (p2 + 1);

	return !y.empty () && !m.empty () && !d.empty ();
}

bool is_number (const string& s)
{
	if (s.empty ()) return false;
	bool dot = false;
	for (char c : s)
	{
		if (c == '.')
		{
			if (dot) return false;
			dot = true;
		}
		else if (!isdigit (c))
		{
			return false;
		}
	}
	return s != ".";
}

vector<pair<string, bool>> parse_tokens (const string& line)
{
	vector<pair<string, bool>> tokens;
	size_t i = 0;
	size_t n = line.length ();

	while (i < n)
	{
		while (i < n && isspace (line [i])) ++i;
		if (i >= n) break;

		if (line [i] == '"')
		{
			++i;
			string token;
			while (i < n && line [i] != '"')
			{
				token += line [i];
				++i;
			}
			if (i < n && line [i] == '"') ++i;
			tokens.emplace_back (token, true);
		}
		else
		{
			string token;
			while (i < n && !isspace (line [i]))
			{
				token += line [i];
				++i;
			}
			tokens.emplace_back (token, false);
		}
	}
	return tokens;
}

void parse_date (const string& date_str, int& year, int& month, int& day)
{
	size_t p1 = date_str.find ('.');
	size_t p2 = date_str.find ('.', p1 + 1);
	year = stoi (date_str.substr (0, p1));
	month = stoi (date_str.substr (p1 + 1, p2 - p1 - 1));
	day = stoi (date_str.substr (p2 + 1));
}

void print_reading (const MeterReading& r, int index)
{
	cout << "Запись #" << index << ":\n";
	cout << "  Ресурс: " << r.resource << "\n";
	cout << "  Дата: " << r.year << "-" << r.month << "-" << r.day << "\n";
	cout << "  Значение: " << r.value << "\n";
	cout << "  Цвет: " << r.color << "\n";
}

bool add_reading (const string& line, vector<MeterReading>& readings)
{
	auto tokens = parse_tokens (line);
	if (tokens.empty ())
	{
		cout << "Пустая строка.\n";
		return false;
	}

	size_t first_prop = 0;
	while (first_prop < tokens.size ())
	{
		const auto& tok = tokens [first_prop];
		if (tok.second || is_date (tok.first) || is_number (tok.first))
			break;
		++first_prop;
	}

	if (first_prop == 0)
	{
		cout << "Не удалось выделить тип объекта.\n";
		return false;
	}

	if (tokens.size () - first_prop != 4)
	{
		cout << "Ожидалось 4 свойства, получено " << tokens.size () - first_prop << ".\n";
		return false;
	}

	const auto& prop_resource = tokens [first_prop];
	const auto& prop_date = tokens [first_prop + 1];
	const auto& prop_value = tokens [first_prop + 2];
	const auto& prop_color = tokens [first_prop + 3];

	if (!prop_resource.second)
		cout << "Предупреждение: ресурс должен быть в кавычках.\n";
	if (!is_date (prop_date.first))
	{
		cout << "Ошибка: некорректный формат даты.\n";
		return false;
	}
	if (!is_number (prop_value.first))
	{
		cout << "Ошибка: некорректное число.\n";
		return false;
	}

	MeterReading reading;
	reading.resource = prop_resource.first;
	parse_date (prop_date.first, reading.year, reading.month, reading.day);
	reading.value = stod (prop_value.first);
	reading.color = prop_color.first;

	readings.push_back (reading);
	cout << "Запись успешно добавлена.\n";
	return true;
}

void show_menu ()
{
	cout << "\n--- Меню ---\n";
	cout << "1. Добавить показания\n";
	cout << "2. Вывести список всех показаний\n";
	cout << "3. Выход\n";
	cout << "Выберите действие: ";
}

int main ()
{
	setlocale (LC_ALL, "ru");

	vector<MeterReading> readings;
	int choice = 0;

	while (true)
	{
		show_menu ();
		cin >> choice;
		cin.ignore ();

		if (choice == 1)
		{
			cout << "Введите строку с показаниями (формат: тип_объекта \"ресурс\" гггг.мм.дд значение цвет):\n";
			string line;
			getline (cin, line);
			add_reading (line, readings);
		}
		else if (choice == 2)
		{
			if (readings.empty ())
			{
				cout << "Список пуст.\n";
			}
			else
			{
				cout << "\nСписок всех показаний:\n";
				for (size_t i = 0; i < readings.size (); ++i)
				{
					print_reading (readings [i], i + 1);
				}
			}
		}
		else if (choice == 3)
		{
			cout << "Выход из программы.\n";
			break;
		}
		else
		{
			cout << "Неверный выбор. Попробуйте снова.\n";
		}
	}

	return 0;
}
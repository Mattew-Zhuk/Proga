#include <QCoreApplication>
#include <QTextStream>
#include <vector>
#include "meterreading.h"

#ifdef _WIN32
#include <windows.h>
#endif

void printMenu() {
	QTextStream out(stdout);
	out << "\n=== МЕНЮ ===\n";
	out << "1. Добавить показание\n";
	out << "2. Вывести все показания\n";
	out << "3. Выход\n";
	out << "Ваш выбор: ";
	out.flush();
}

void addReading(std::vector<MeterReading>& readings, QTextStream& out, QTextStream& in) {
	out << "Введите строку в формате: \"Тип ресурса\" ГГГГ.ММ.ДД Значение\n";
	out << "Пример: \"Электричество\" 2024.01.15 150.5\n";
	out << "> ";
	out.flush();

	QString line = in.readLine();
	if (line.isNull() || line.trimmed().isEmpty()) {
		out << "Строка пуста, добавление отменено.\n";
		out.flush();
		return;
	}

	try {
		MeterReading reading = MeterReading::fromString(line.trimmed());
		readings.push_back(reading);
		out << "Показание успешно добавлено.\n";
		out.flush();
	} catch (const std::exception& e) {
		out << "Ошибка при разборе строки: " << e.what() << "\n";
		out.flush();
	}
}

void showAllReadings(const std::vector<MeterReading>& readings, QTextStream& out) {
	if (readings.empty()) {
		out << "Список показаний пуст.\n";
		out.flush();
		return;
	}
	out << "\n=== ВСЕ ПОКАЗАНИЯ ===\n";
	for (size_t i = 0; i < readings.size(); ++i) {
		out << i+1 << ". " << readings[i].toString() << "\n";
	}
	out << "======================\n";
	out.flush();
}

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);

#ifdef _WIN32
	system("chcp 65001 > nul");
#endif

	QTextStream in(stdin);
	QTextStream out(stdout);

	std::vector<MeterReading> readings;

	while (true) {
		printMenu();
		QString choiceStr = in.readLine();
		if (choiceStr.isNull()) break;
		int choice = choiceStr.trimmed().toInt();

		switch (choice) {
		case 1:
			addReading(readings, out, in);
			break;
		case 2:
			showAllReadings(readings, out);
			break;
		case 3:
			out << "До свидания!\n";
			return 0;
		default:
			out << "Неверный выбор, попробуйте снова.\n";
			out.flush();
			break;
		}
	}
	return 0;
}

#include <QtTest>
#include <QApplication>
#include <QPushButton>
#include <QTableView>
#include <QLineEdit>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QTextBrowser>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QSignalSpy>
#include <QTimer>
#include <QMessageBox>
#include <QElapsedTimer>

#include "mainwindow.h"
#include "firstwindow.h"
#include "helpwindow.h"
#include "meterreadingmodel.h"

class TestUI : public QObject {
	Q_OBJECT

private slots:
	// Инициализация и очистка
	void initTestCase();
	void cleanupTestCase();

	// ==================== ТЕСТЫ ГЛАВНОГО ОКНА ====================
	void test_MainWindow_InitialState();
	void test_MainWindow_AllButtonsExist();
	void test_MainWindow_FirstButton_OpensFirstWindow();
	void test_MainWindow_HelpButton_OpensHelpWindow();
	void test_MainWindow_ExitButton_QuitsApplication();

	// ==================== ТЕСТЫ ОКНА ПОКАЗАНИЙ ====================
	void test_FirstWindow_InitialState();
	void test_FirstWindow_AddValidReading();
	void test_FirstWindow_AddEmptyResource_ShouldNotAdd();
	void test_FirstWindow_AddMultipleReadings();
	void test_FirstWindow_DeleteSelectedReading();
	void test_FirstWindow_DeleteWithoutSelection_NoCrash();
	void test_FirstWindow_TableModelUpdatesCorrectly();
	void test_FirstWindow_BackButton_ClosesWindow();
	void test_FirstWindow_LoadFromFileButton_Exists();

	// ==================== ТЕСТЫ ОКНА СПРАВКИ ====================
	void test_HelpWindow_InitialState();
	void test_HelpWindow_ContentLoaded();
	void test_HelpWindow_BackButton_ClosesWindow();
	void test_HelpWindow_ImagePathDisplayed();

	// ==================== КОМПЛЕКСНЫЕ ТЕСТЫ ====================
	void test_Integration_OpenFirstWindow_AddReadings_Close();
	void test_Integration_OpenHelpWindow_ThenFirstWindow();
	void test_Stress_AddManyReadings_Performance();

private:
	// Вспомогательные методы - раздельные для разных типов
	QPushButton* findButton(QWidget* parent, const QString& text);
	QLineEdit* findLineEdit(QWidget* parent);
	QDateEdit* findDateEdit(QWidget* parent);
	QDoubleSpinBox* findDoubleSpinBox(QWidget* parent);
	QTableView* findTableView(QWidget* parent);
	QTextBrowser* findTextBrowser(QWidget* parent);

	void simulateClick(QAbstractButton* button);
	void waitForWindow(QWidget* window, bool visible = true, int timeout = 1000);
	void addTestReading(FirstWindow* window, const QString& resource, double value);
	int getTableRowCount(FirstWindow* window);
	FirstWindow* findFirstWindow();
	HelpWindow* findHelpWindow();

	MainWindow* mainWindow;
	QString tempDir;
};

// ==================== ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ ====================

QPushButton* TestUI::findButton(QWidget* parent, const QString& text) {
	if (!parent) return nullptr;
	const auto buttons = parent->findChildren<QPushButton*>();
	for (QPushButton* btn : buttons) {
		if (btn->text() == text) {
			return btn;
		}
	}
	return nullptr;
}

QLineEdit* TestUI::findLineEdit(QWidget* parent) {
	if (!parent) return nullptr;
	const auto edits = parent->findChildren<QLineEdit*>();
	return edits.isEmpty() ? nullptr : edits.first();
}

QDateEdit* TestUI::findDateEdit(QWidget* parent) {
	if (!parent) return nullptr;
	const auto edits = parent->findChildren<QDateEdit*>();
	return edits.isEmpty() ? nullptr : edits.first();
}

QDoubleSpinBox* TestUI::findDoubleSpinBox(QWidget* parent) {
	if (!parent) return nullptr;
	const auto spins = parent->findChildren<QDoubleSpinBox*>();
	return spins.isEmpty() ? nullptr : spins.first();
}

QTableView* TestUI::findTableView(QWidget* parent) {
	if (!parent) return nullptr;
	const auto tables = parent->findChildren<QTableView*>();
	return tables.isEmpty() ? nullptr : tables.first();
}

QTextBrowser* TestUI::findTextBrowser(QWidget* parent) {
	if (!parent) return nullptr;
	const auto browsers = parent->findChildren<QTextBrowser*>();
	return browsers.isEmpty() ? nullptr : browsers.first();
}

void TestUI::simulateClick(QAbstractButton* button) {
	QVERIFY2(button != nullptr, "Кнопка не найдена");
	QTest::mouseClick(button, Qt::LeftButton);
	QCoreApplication::processEvents();
	QTest::qWait(50);
}

void TestUI::waitForWindow(QWidget* window, bool visible, int timeout) {
	QElapsedTimer timer;
	timer.start();
	while (timer.elapsed() < timeout) {
		if (window && window->isVisible() == visible) {
			QCoreApplication::processEvents();
			return;
		}
		QTest::qWait(10);
	}
}

void TestUI::addTestReading(FirstWindow* window, const QString& resource, double value) {
	QLineEdit* resourceEdit = findLineEdit(window);
	QDoubleSpinBox* valueSpin = findDoubleSpinBox(window);
	QPushButton* addButton = findButton(window, "Добавить");

	QVERIFY(resourceEdit);
	QVERIFY(valueSpin);
	QVERIFY(addButton);

	resourceEdit->setText(resource);
	valueSpin->setValue(value);
	simulateClick(addButton);
}

int TestUI::getTableRowCount(FirstWindow* window) {
	QTableView* tableView = findTableView(window);
	if (!tableView) return -1;

	MeterReadingModel* model = qobject_cast<MeterReadingModel*>(tableView->model());
	return model ? model->getReadingCount() : -1;
}

FirstWindow* TestUI::findFirstWindow() {
	const auto topWidgets = qApp->topLevelWidgets();
	for (QWidget* w : topWidgets) {
		FirstWindow* firstWindow = qobject_cast<FirstWindow*>(w);
		if (firstWindow && firstWindow->isVisible()) {
			return firstWindow;
		}
	}
	return nullptr;
}

HelpWindow* TestUI::findHelpWindow() {
	const auto topWidgets = qApp->topLevelWidgets();
	for (QWidget* w : topWidgets) {
		HelpWindow* helpWindow = qobject_cast<HelpWindow*>(w);
		if (helpWindow && helpWindow->isVisible()) {
			return helpWindow;
		}
	}
	return nullptr;
}

// ==================== ИНИЦИАЛИЗАЦИЯ ====================

void TestUI::initTestCase() {
	qDebug() << "\n========================================";
	qDebug() << "ЗАПУСК АВТОМАТИЗИРОВАННЫХ UI ТЕСТОВ";
	qDebug() << "========================================\n";

	tempDir = QDir::temp().absoluteFilePath("ui_test_");
	tempDir += QString::number(QDateTime::currentMSecsSinceEpoch());
	QDir().mkpath(tempDir);

	qputenv("QT_FATAL_WARNINGS", "0");
}

void TestUI::cleanupTestCase() {
	QDir(tempDir).removeRecursively();

	qDebug() << "\n========================================";
	qDebug() << "ВСЕ UI ТЕСТЫ ЗАВЕРШЕНЫ";
	qDebug() << "========================================\n";
}

// ==================== ТЕСТЫ ГЛАВНОГО ОКНА ====================

void TestUI::test_MainWindow_InitialState() {
	MainWindow window;
	window.show();
	waitForWindow(&window);

	QVERIFY2(window.isVisible(), "Главное окно должно быть видимым");
	QCOMPARE(window.windowTitle(), QString("Главное окно"));

	window.close();
}

void TestUI::test_MainWindow_AllButtonsExist() {
	MainWindow window;
	window.show();
	waitForWindow(&window);

	QPushButton* firstBtn = findButton(&window, "Первая");
	QPushButton* helpBtn = findButton(&window, "Справка");
	QPushButton* exitBtn = findButton(&window, "Выход");

	QVERIFY2(firstBtn, "Кнопка 'Первая' должна существовать");
	QVERIFY2(helpBtn, "Кнопка 'Справка' должна существовать");
	QVERIFY2(exitBtn, "Кнопка 'Выход' должна существовать");

	QVERIFY2(firstBtn->isEnabled(), "Кнопка 'Первая' должна быть активна");
	QVERIFY2(helpBtn->isEnabled(), "Кнопка 'Справка' должна быть активна");
	QVERIFY2(exitBtn->isEnabled(), "Кнопка 'Выход' должна быть активна");

	window.close();
}

void TestUI::test_MainWindow_FirstButton_OpensFirstWindow() {
	MainWindow window;
	window.show();
	waitForWindow(&window);

	QPushButton* firstBtn = findButton(&window, "Первая");
	QVERIFY(firstBtn);

	simulateClick(firstBtn);
	QTest::qWait(100);

	FirstWindow* firstWindow = findFirstWindow();

	QVERIFY2(firstWindow, "Должно открыться окно FirstWindow");
	QVERIFY2(firstWindow->isVisible(), "Окно FirstWindow должно быть видимым");
	QCOMPARE(firstWindow->windowTitle(), QString("Показания счетчиков"));

	firstWindow->accept();
	waitForWindow(firstWindow, false);

	window.close();
}

void TestUI::test_MainWindow_HelpButton_OpensHelpWindow() {
	MainWindow window;
	window.show();
	waitForWindow(&window);

	QPushButton* helpBtn = findButton(&window, "Справка");
	QVERIFY(helpBtn);

	simulateClick(helpBtn);
	QTest::qWait(100);

	HelpWindow* helpWindow = findHelpWindow();

	QVERIFY2(helpWindow, "Должно открыться окно HelpWindow");
	QVERIFY2(helpWindow->isVisible(), "Окно HelpWindow должно быть видимым");
	QCOMPARE(helpWindow->windowTitle(), QString("Справка"));

	helpWindow->accept();
	waitForWindow(helpWindow, false);

	window.close();
}

void TestUI::test_MainWindow_ExitButton_QuitsApplication() {
	MainWindow window;
	window.show();
	waitForWindow(&window);

	QPushButton* exitBtn = findButton(&window, "Выход");
	QVERIFY2(exitBtn, "Кнопка 'Выход' должна существовать");
	QVERIFY2(exitBtn->isEnabled(), "Кнопка 'Выход' должна быть активна");
	QCOMPARE(exitBtn->text(), QString("Выход"));

	window.close();
}

// ==================== ТЕСТЫ ОКНА ПОКАЗАНИЙ ====================

void TestUI::test_FirstWindow_InitialState() {
	FirstWindow window;
	window.show();
	waitForWindow(&window);

	QVERIFY(window.isVisible());
	QCOMPARE(window.windowTitle(), QString("Показания счетчиков"));

	QTableView* tableView = findTableView(&window);
	QLineEdit* resourceEdit = findLineEdit(&window);
	QDateEdit* dateEdit = findDateEdit(&window);
	QDoubleSpinBox* valueSpin = findDoubleSpinBox(&window);
	QPushButton* addBtn = findButton(&window, "Добавить");
	QPushButton* deleteBtn = findButton(&window, "Удалить выделенное");
	QPushButton* loadBtn = findButton(&window, "Загрузить из файла");
	QPushButton* backBtn = findButton(&window, "Назад");

	QVERIFY2(tableView, "Должен быть QTableView");
	QVERIFY2(resourceEdit, "Должно быть поле ввода типа ресурса");
	QVERIFY2(dateEdit, "Должен быть QDateEdit");
	QVERIFY2(valueSpin, "Должен быть QDoubleSpinBox");
	QVERIFY2(addBtn, "Должна быть кнопка 'Добавить'");
	QVERIFY2(deleteBtn, "Должна быть кнопка 'Удалить выделенное'");
	QVERIFY2(loadBtn, "Должна быть кнопка 'Загрузить из файла'");
	QVERIFY2(backBtn, "Должна быть кнопка 'Назад'");

	MeterReadingModel* model = qobject_cast<MeterReadingModel*>(tableView->model());
	QVERIFY2(model, "Модель должна быть установлена");
	QCOMPARE(model->getReadingCount(), 0);

	window.accept();
}

void TestUI::test_FirstWindow_AddValidReading() {
	FirstWindow window;
	window.show();
	waitForWindow(&window);

	addTestReading(&window, "Электричество", 150.75);

	QCOMPARE(getTableRowCount(&window), 1);

	QTableView* tableView = findTableView(&window);
	MeterReadingModel* model = qobject_cast<MeterReadingModel*>(tableView->model());
	MeterReading reading = model->getReadingAt(0);

	QCOMPARE(reading.getResourceType(), QString("Электричество"));
	QCOMPARE(reading.getValue(), 150.75);
	QCOMPARE(reading.getDate(), QDate::currentDate());

	QLineEdit* resourceEdit = findLineEdit(&window);
	QDoubleSpinBox* valueSpin = findDoubleSpinBox(&window);

	QVERIFY(resourceEdit->text().isEmpty());
	QCOMPARE(valueSpin->value(), 0.0);

	window.accept();
}

void TestUI::test_FirstWindow_AddEmptyResource_ShouldNotAdd() {
	FirstWindow window;
	window.show();
	waitForWindow(&window);

	QLineEdit* resourceEdit = findLineEdit(&window);
	QDoubleSpinBox* valueSpin = findDoubleSpinBox(&window);
	QPushButton* addBtn = findButton(&window, "Добавить");

	resourceEdit->clear();
	valueSpin->setValue(100.0);

	simulateClick(addBtn);

	QCOMPARE(getTableRowCount(&window), 0);

	window.accept();
}

void TestUI::test_FirstWindow_AddMultipleReadings() {
	FirstWindow window;
	window.show();
	waitForWindow(&window);

	QVector<QPair<QString, double>> readings = {
		{"Электричество", 150.5},
		{"Вода", 75.3},
		{"Газ", 200.0},
		{"Отопление", 500.75},
		{"Интернет", 1000.0}
	};

	for (const auto& reading : readings) {
		addTestReading(&window, reading.first, reading.second);
	}

	QCOMPARE(getTableRowCount(&window), readings.size());

	QTableView* tableView = findTableView(&window);
	MeterReadingModel* model = qobject_cast<MeterReadingModel*>(tableView->model());

	for (int i = 0; i < readings.size(); i++) {
		MeterReading reading = model->getReadingAt(i);
		QCOMPARE(reading.getResourceType(), readings[i].first);
		QCOMPARE(reading.getValue(), readings[i].second);
	}

	window.accept();
}

void TestUI::test_FirstWindow_DeleteSelectedReading() {
	FirstWindow window;
	window.show();
	waitForWindow(&window);

	addTestReading(&window, "Первый", 100.0);
	addTestReading(&window, "Второй", 200.0);
	addTestReading(&window, "Третий", 300.0);

	QCOMPARE(getTableRowCount(&window), 3);

	QTableView* tableView = findTableView(&window);
	tableView->selectRow(1);

	QPushButton* deleteBtn = findButton(&window, "Удалить выделенное");
	simulateClick(deleteBtn);

	QCOMPARE(getTableRowCount(&window), 2);

	MeterReadingModel* model = qobject_cast<MeterReadingModel*>(tableView->model());
	QCOMPARE(model->getReadingAt(0).getResourceType(), QString("Первый"));
	QCOMPARE(model->getReadingAt(1).getResourceType(), QString("Третий"));

	window.accept();
}

void TestUI::test_FirstWindow_DeleteWithoutSelection_NoCrash() {
	FirstWindow window;
	window.show();
	waitForWindow(&window);

	addTestReading(&window, "Тест", 100.0);

	QPushButton* deleteBtn = findButton(&window, "Удалить выделенное");

	simulateClick(deleteBtn);

	QCOMPARE(getTableRowCount(&window), 1);

	window.accept();
}

void TestUI::test_FirstWindow_TableModelUpdatesCorrectly() {
	FirstWindow window;
	window.show();
	waitForWindow(&window);

	QTableView* tableView = findTableView(&window);
	MeterReadingModel* model = qobject_cast<MeterReadingModel*>(tableView->model());

	QSignalSpy rowsInsertedSpy(model, &MeterReadingModel::rowsInserted);
	addTestReading(&window, "Сигнал", 500.0);
	QCOMPARE(rowsInsertedSpy.count(), 1);

	QSignalSpy rowsRemovedSpy(model, &MeterReadingModel::rowsRemoved);
	tableView->selectRow(0);
	QPushButton* deleteBtn = findButton(&window, "Удалить выделенное");
	simulateClick(deleteBtn);
	QCOMPARE(rowsRemovedSpy.count(), 1);

	window.accept();
}

void TestUI::test_FirstWindow_BackButton_ClosesWindow() {
	FirstWindow window;
	window.show();
	waitForWindow(&window);

	QPushButton* backBtn = findButton(&window, "Назад");
	QVERIFY(backBtn);

	simulateClick(backBtn);
	waitForWindow(&window, false, 500);

	QVERIFY2(!window.isVisible(), "Окно должно быть закрыто после нажатия 'Назад'");
}

void TestUI::test_FirstWindow_LoadFromFileButton_Exists() {
	FirstWindow window;
	window.show();
	waitForWindow(&window);

	QPushButton* loadBtn = findButton(&window, "Загрузить из файла");
	QVERIFY2(loadBtn, "Кнопка 'Загрузить из файла' должна существовать");
	QVERIFY(loadBtn->isEnabled());
	QCOMPARE(loadBtn->text(), QString("Загрузить из файла"));

	window.accept();
}

// ==================== ТЕСТЫ ОКНА СПРАВКИ ====================

void TestUI::test_HelpWindow_InitialState() {
	HelpWindow window;
	window.show();
	waitForWindow(&window);

	QVERIFY(window.isVisible());
	QCOMPARE(window.windowTitle(), QString("Справка"));
	QVERIFY(window.isModal());

	QTextBrowser* textBrowser = findTextBrowser(&window);
	QPushButton* backBtn = findButton(&window, "Назад");

	QVERIFY2(textBrowser, "Должен быть QTextBrowser");
	QVERIFY2(backBtn, "Должна быть кнопка 'Назад'");
	QVERIFY(backBtn->isEnabled());

	window.accept();
}

void TestUI::test_HelpWindow_ContentLoaded() {
	HelpWindow window;
	window.show();
	waitForWindow(&window);

	QTextBrowser* textBrowser = findTextBrowser(&window);
	QVERIFY(textBrowser);

	QString html = textBrowser->toHtml();

	QVERIFY2(html.contains("Справка по программе"),
			 "Должен быть заголовок 'Справка по программе'");
	QVERIFY2(html.contains("Функции программы"),
			 "Должен быть раздел 'Функции программы'");
	QVERIFY2(html.contains("Первая"),
			 "Должно быть упоминание о функции 'Первая'");
	QVERIFY2(html.contains("Справка"),
			 "Должно быть упоминание о функции 'Справка'");
	QVERIFY2(html.contains("Выход"),
			 "Должно быть упоминание о функции 'Выход'");

	window.accept();
}

void TestUI::test_HelpWindow_BackButton_ClosesWindow() {
	HelpWindow window;
	window.show();
	waitForWindow(&window);

	QPushButton* backBtn = findButton(&window, "Назад");
	QVERIFY(backBtn);

	simulateClick(backBtn);
	waitForWindow(&window, false, 500);

	QVERIFY2(!window.isVisible(), "Окно должно быть закрыто после нажатия 'Назад'");
}

void TestUI::test_HelpWindow_ImagePathDisplayed() {
	HelpWindow window;
	window.show();
	waitForWindow(&window);

	QTextBrowser* textBrowser = findTextBrowser(&window);
	QString html = textBrowser->toHtml();

	QVERIFY2(html.contains("image.jpg") || html.contains("Путь к изображению"),
			 "Должна быть информация о пути к изображению");

	window.accept();
}

// ==================== КОМПЛЕКСНЫЕ ТЕСТЫ ====================

void TestUI::test_Integration_OpenFirstWindow_AddReadings_Close() {
	MainWindow mainWin;
	mainWin.show();
	waitForWindow(&mainWin);

	QPushButton* firstBtn = findButton(&mainWin, "Первая");
	simulateClick(firstBtn);
	QTest::qWait(100);

	FirstWindow* firstWindow = findFirstWindow();
	QVERIFY(firstWindow);

	addTestReading(firstWindow, "Интеграция", 999.99);
	QCOMPARE(getTableRowCount(firstWindow), 1);

	QPushButton* backBtn = findButton(firstWindow, "Назад");
	simulateClick(backBtn);
	waitForWindow(firstWindow, false);

	mainWin.close();
}

void TestUI::test_Integration_OpenHelpWindow_ThenFirstWindow() {
	MainWindow mainWin;
	mainWin.show();
	waitForWindow(&mainWin);

	QPushButton* helpBtn = findButton(&mainWin, "Справка");
	simulateClick(helpBtn);
	QTest::qWait(100);

	HelpWindow* helpWindow = findHelpWindow();
	QVERIFY(helpWindow);

	helpWindow->accept();
	waitForWindow(helpWindow, false);

	QPushButton* firstBtn = findButton(&mainWin, "Первая");
	simulateClick(firstBtn);
	QTest::qWait(100);

	FirstWindow* firstWindow = findFirstWindow();
	QVERIFY(firstWindow);
	firstWindow->accept();

	mainWin.close();
}

void TestUI::test_Stress_AddManyReadings_Performance() {
	QBENCHMARK {
		FirstWindow window;
		window.show();
		waitForWindow(&window);

		for (int i = 0; i < 100; i++) {
			addTestReading(&window, QString("Ресурс_%1").arg(i), i * 10.0);
		}

		QCOMPARE(getTableRowCount(&window), 100);
		window.accept();
	}
}

// Запуск тестов
QTEST_MAIN(TestUI)

#include "test_ui.moc"

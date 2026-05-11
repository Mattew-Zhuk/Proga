#include <QtTest>
#include "commandprocessor.h"
#include "meterreadingmodel.h"
#include <QTemporaryFile>
#include <QTextStream>

class TestCommandProcessor : public QObject {
	Q_OBJECT
private slots:
	void testAddCommand();
	void testRemoveCommand();
	void testSaveCommand();
	void testMultipleCommands();
	void testInvalidCommand();
};

void TestCommandProcessor::testAddCommand() {
	MeterReadingModel model;
	CommandProcessor proc(&model);
	QTemporaryFile cmdFile;
	QVERIFY(cmdFile.open());
	QTextStream(&cmdFile) << "ADD Электричество; 2024.01.15; 150.5\n";
	cmdFile.close();

	QString err;
	QVERIFY(proc.processCommandsFromFile(cmdFile.fileName(), err));
	QCOMPARE(model.getReadingCount(), 1);
	QCOMPARE(model.getReadingAt(0).getResourceType(), QString("Электричество"));
}

void TestCommandProcessor::testRemoveCommand() {
	MeterReadingModel model;
	model.addReading(MeterReading("Вода", QDate(2024,1,1), 100));
	model.addReading(MeterReading("Газ", QDate(2024,1,2), 200));
	model.addReading(MeterReading("Вода", QDate(2024,1,3), 50));
	CommandProcessor proc(&model);
	QTemporaryFile cmdFile;
	QVERIFY(cmdFile.open());
	QTextStream(&cmdFile) << "REM value > 150\n";
	cmdFile.close();

	QString err;
	QVERIFY(proc.processCommandsFromFile(cmdFile.fileName(), err));
	QCOMPARE(model.getReadingCount(), 2);
	QCOMPARE(model.getReadingAt(0).getResourceType(), QString("Вода"));
	QCOMPARE(model.getReadingAt(1).getResourceType(), QString("Вода"));
}

void TestCommandProcessor::testSaveCommand() {
	MeterReadingModel model;
	model.addReading(MeterReading("Тест", QDate(2024,1,1), 123.45));
	CommandProcessor proc(&model);
	QTemporaryFile outFile;
	QVERIFY(outFile.open());
	outFile.close();

	QTemporaryFile cmdFile;
	QVERIFY(cmdFile.open());
	QTextStream(&cmdFile) << "SAVE " << outFile.fileName() << "\n";
	cmdFile.close();

	QString err;
	QVERIFY(proc.processCommandsFromFile(cmdFile.fileName(), err));
	QFile verify(outFile.fileName());
	QVERIFY(verify.open(QIODevice::ReadOnly));
	QString content = QTextStream(&verify).readAll();
	QVERIFY(content.contains("\"Тест\""));
	QVERIFY(content.contains("123.45"));
}

void TestCommandProcessor::testMultipleCommands() {
	MeterReadingModel model;
	CommandProcessor proc(&model);
	QTemporaryFile cmdFile;
	QVERIFY(cmdFile.open());
	QTextStream out(&cmdFile);
	out << "ADD Вода; 2024.01.01; 50\n"
		<< "ADD Газ; 2024.01.02; 200\n"
		<< "REM value > 100\n"
		<< "ADD Электричество; 2024.01.03; 300\n"
		<< "SAVE /tmp/test_save.txt\n";
	cmdFile.close();

	QString err;
	QVERIFY(proc.processCommandsFromFile(cmdFile.fileName(), err));
	QCOMPARE(model.getReadingCount(), 2);
	QCOMPARE(model.getReadingAt(0).getResourceType(), QString("Вода"));
	QCOMPARE(model.getReadingAt(1).getResourceType(), QString("Электричество"));
}

void TestCommandProcessor::testInvalidCommand() {
	MeterReadingModel model;
	CommandProcessor proc(&model);
	QTemporaryFile cmdFile;
	QVERIFY(cmdFile.open());
	QTextStream(&cmdFile) << "INVALID command\n";
	cmdFile.close();

	QString err;
	QVERIFY(!proc.processCommandsFromFile(cmdFile.fileName(), err));
	QVERIFY(err.contains("ошибок: 1"));
}

QTEST_APPLESS_MAIN(TestCommandProcessor)
#include "test_commandprocessor.moc"

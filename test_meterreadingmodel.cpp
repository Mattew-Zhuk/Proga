#include <QtTest>
#include "meterreadingmodel.h"
#include "logger.h"
#include <QFile>
#include <QTextStream>

class TestMeterReadingModel : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testAddReading();
    void testRemoveReading();
    void testLoadFromFileValid();
    void testLoadFromFileInvalid();
    void testLoadFromFileMixed();
    void testClear();
    void testFromStringValid();
    void testFromStringInvalid();
    void testTryFromString();

private:
    void createTestFile(const QString& filename, const QStringList& lines);
};

void TestMeterReadingModel::initTestCase() {
    Logger::instance().setLogFile("test_errors.log");
}

void TestMeterReadingModel::cleanupTestCase() {
    QFile::remove("test_data.txt");
    QFile::remove("test_errors.log");
}

void TestMeterReadingModel::createTestFile(const QString& filename, const QStringList& lines) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const QString& line : lines) {
            out << line << "\n";
        }
        file.close();
    }
}

void TestMeterReadingModel::testAddReading() {
    MeterReadingModel model;
    QCOMPARE(model.getReadingCount(), 0);

    MeterReading reading("Электричество", QDate(2024, 1, 15), 150.5);
    model.addReading(reading);

    QCOMPARE(model.getReadingCount(), 1);
    QCOMPARE(model.getReadingAt(0).getResourceType(), QString("Электричество"));
    QCOMPARE(model.getReadingAt(0).getValue(), 150.5);
}

void TestMeterReadingModel::testRemoveReading() {
    MeterReadingModel model;
    model.addReading(MeterReading("Вода", QDate(2024, 1, 15), 50.0));
    model.addReading(MeterReading("Газ", QDate(2024, 1, 15), 200.0));

    QCOMPARE(model.getReadingCount(), 2);

    model.removeReading(0);
    QCOMPARE(model.getReadingCount(), 1);
    QCOMPARE(model.getReadingAt(0).getResourceType(), QString("Газ"));
}

void TestMeterReadingModel::testLoadFromFileValid() {
    QStringList lines;
    lines << "\"Электричество\" 2024.01.15 150.5"
          << "\"Вода\" 2024.01.16 75.3"
          << "\"Газ\" 2024.01.17 200.0";

    createTestFile("test_data.txt", lines);

    MeterReadingModel model;
    model.loadFromFile("test_data.txt");

    QCOMPARE(model.getReadingCount(), 3);
    QCOMPARE(model.getReadingAt(0).getResourceType(), QString("Электричество"));
    QCOMPARE(model.getReadingAt(1).getResourceType(), QString("Вода"));
    QCOMPARE(model.getReadingAt(2).getResourceType(), QString("Газ"));
}

void TestMeterReadingModel::testLoadFromFileInvalid() {
    QStringList lines;
    lines << "\"Электричество\" 2024.01.15 150.5"      // valid
          << "некорректная строка"                     // invalid
          << "\"Вода\" 2024.13.45 75.3"                // invalid date
          << "\"\" 2024.01.16 50.0"                    // empty resource
          << "\"Газ\" 2024.01.17 -100.0";              // negative value

    createTestFile("test_data.txt", lines);

    MeterReadingModel model;
    model.loadFromFile("test_data.txt");

    // Только первая строка должна загрузиться успешно
    QCOMPARE(model.getReadingCount(), 1);
    QCOMPARE(model.getReadingAt(0).getResourceType(), QString("Электричество"));
}

void TestMeterReadingModel::testLoadFromFileMixed() {
    QStringList lines;
    lines << "\"Электричество\" 2024.01.15 150.5"
          << ""
          << "\"Вода\" 2024.01.16 75.3"
          << "   "
          << "\"Газ\" 2024.01.17 200.0";

    createTestFile("test_data.txt", lines);

    MeterReadingModel model;
    model.loadFromFile("test_data.txt");

    QCOMPARE(model.getReadingCount(), 3);
}

void TestMeterReadingModel::testClear() {
    MeterReadingModel model;
    model.addReading(MeterReading("Электричество", QDate(2024, 1, 15), 150.5));
    model.addReading(MeterReading("Вода", QDate(2024, 1, 16), 75.3));

    QCOMPARE(model.getReadingCount(), 2);

    model.clear();
    QCOMPARE(model.getReadingCount(), 0);
}

void TestMeterReadingModel::testFromStringValid() {
    MeterReading reading = MeterReading::fromString("\"Электричество\" 2024.01.15 150.5");

    QCOMPARE(reading.getResourceType(), QString("Электричество"));
    QCOMPARE(reading.getDate(), QDate(2024, 1, 15));
    QCOMPARE(reading.getValue(), 150.5);
}

void TestMeterReadingModel::testFromStringInvalid() {
    bool exceptionCaught = false;

    try {
        MeterReading::fromString("invalid string");
    } catch (const ParseException& e) {
        exceptionCaught = true;
    }
    QVERIFY(exceptionCaught);

    exceptionCaught = false;
    try {
        MeterReading::fromString("\"\" 2024.01.15 150.5");
    } catch (const ParseException& e) {
        exceptionCaught = true;
    }
    QVERIFY(exceptionCaught);

    exceptionCaught = false;
    try {
        MeterReading::fromString("\"Электричество\" 2024.13.45 150.5");
    } catch (const ParseException& e) {
        exceptionCaught = true;
    }
    QVERIFY(exceptionCaught);
}

void TestMeterReadingModel::testTryFromString() {
    MeterReading reading;
    QString error;

    bool result = MeterReading::tryFromString("\"Электричество\" 2024.01.15 150.5", reading, error);
    QVERIFY(result);
    QCOMPARE(reading.getResourceType(), QString("Электричество"));

    result = MeterReading::tryFromString("invalid", reading, error);
    QVERIFY(!result);
    QVERIFY(!error.isEmpty());
}

QTEST_APPLESS_MAIN(TestMeterReadingModel)

#include "test_meterreadingmodel.moc"

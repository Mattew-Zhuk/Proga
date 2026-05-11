#include <QtTest>
#include "conditionparser.h"
#include "meterreading.h"

class TestConditionParser : public QObject {
	Q_OBJECT
private slots:
	void testResourceType();
	void testValueComparison();
	void testDateComparison();
	void testInvalidCondition();
};

void TestConditionParser::testResourceType() {
	MeterReading r("Вода", QDate(2024,1,1), 100.0);
	QString err;
	QVERIFY(ConditionParser::matches(r, "resourceType == \"Вода\"", err));
	QVERIFY(!ConditionParser::matches(r, "resourceType == \"Газ\"", err));
	QVERIFY(ConditionParser::matches(r, "resourceType != \"Газ\"", err));
	QVERIFY(!ConditionParser::matches(r, "resourceType < \"Вода\"", err)); // неверный оператор
}

void TestConditionParser::testValueComparison() {
	MeterReading r("Электричество", QDate(2024,1,1), 150.5);
	QString err;
	QVERIFY(ConditionParser::matches(r, "value > 100", err));
	QVERIFY(ConditionParser::matches(r, "value < 200", err));
	QVERIFY(ConditionParser::matches(r, "value == 150.5", err));
	QVERIFY(ConditionParser::matches(r, "value != 0", err));
	QVERIFY(!ConditionParser::matches(r, "value == 0", err));
}

void TestConditionParser::testDateComparison() {
	MeterReading r("Газ", QDate(2024,5,10), 200.0);
	QString err;
	QVERIFY(ConditionParser::matches(r, "date > 2024.01.01", err));
	QVERIFY(ConditionParser::matches(r, "date < 2024.12.31", err));
	QVERIFY(ConditionParser::matches(r, "date == 2024.05.10", err));
	QVERIFY(ConditionParser::matches(r, "date != 2024.01.01", err));
	QVERIFY(!ConditionParser::matches(r, "date == 2023.12.31", err));
}

void TestConditionParser::testInvalidCondition() {
	MeterReading r("А", QDate::currentDate(), 1.0);
	QString err;
	QVERIFY(!ConditionParser::matches(r, "unknownField == 5", err));
	QVERIFY(!err.isEmpty());
	QVERIFY(!ConditionParser::matches(r, "value xxx 10", err));
	QVERIFY(!err.isEmpty());
}

QTEST_APPLESS_MAIN(TestConditionParser)
#include "test_conditionparser.moc"

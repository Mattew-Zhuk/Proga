#include "conditionparser.h"
#include <QRegularExpression>
#include <QDate>
#include <QDebug>

bool ConditionParser::matches(const MeterReading& reading, const QString& condition, QString& errorMessage) {
	QString cond = condition.trimmed();
	QRegularExpression regex(R"(^(\w+)\s+([<>]=?|==|!=)\s+(.+)$)");
	QRegularExpressionMatch match = regex.match(cond);
	if (!match.hasMatch()) {
		errorMessage = QString("Неверный формат условия: %1").arg(cond);
		return false;
	}

	QString field = match.captured(1);
	QString op = match.captured(2);
	QString valueStr = match.captured(3).trimmed();

	if (field == "resourceType") {
		if ((valueStr.startsWith('"') && valueStr.endsWith('"')) ||
			(valueStr.startsWith('\'') && valueStr.endsWith('\''))) {
			valueStr = valueStr.mid(1, valueStr.length() - 2);
		}
		if (op == "==") {
			return reading.getResourceType() == valueStr;
		} else if (op == "!=") {
			return reading.getResourceType() != valueStr;
		} else {
			errorMessage = "Для resourceType поддерживаются только операторы == и !=";
			return false;
		}
	}
	else if (field == "date") {
		QDate date = QDate::fromString(valueStr, "yyyy.MM.dd");
		if (!date.isValid()) {
			errorMessage = QString("Неверный формат даты: %1 (ожидается yyyy.MM.dd)").arg(valueStr);
			return false;
		}
		QDate readingDate = reading.getDate();
		if (op == "<") return readingDate < date;
		if (op == "<=") return readingDate <= date;
		if (op == ">") return readingDate > date;
		if (op == ">=") return readingDate >= date;
		if (op == "==") return readingDate == date;
		if (op == "!=") return readingDate != date;
		errorMessage = QString("Неподдерживаемый оператор для даты: %1").arg(op);
		return false;
	}
	else if (field == "value") {
		bool ok;
		double val = valueStr.toDouble(&ok);
		if (!ok) {
			errorMessage = QString("Неверное числовое значение: %1").arg(valueStr);
			return false;
		}
		double readingVal = reading.getValue();
		if (op == "<") return readingVal < val;
		if (op == "<=") return readingVal <= val;
		if (op == ">") return readingVal > val;
		if (op == ">=") return readingVal >= val;
		if (op == "==") return qFabs(readingVal - val) < 1e-9;
		if (op == "!=") return qFabs(readingVal - val) >= 1e-9;
		errorMessage = QString("Неподдерживаемый оператор для значения: %1").arg(op);
		return false;
	}
	else {
		errorMessage = QString("Неизвестное поле '%1' в условии").arg(field);
		return false;
	}
}

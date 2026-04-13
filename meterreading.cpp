#include "meterreading.h"
#include <QStringList>
#include <QRegularExpression>

MeterReading::MeterReading() : value_(0.0) {}

MeterReading::MeterReading(const QString& resourceType, const QDate& date, double value)
	: resource_type_(resourceType), date_(date), value_(value) {}

QString MeterReading::getResourceType() const {
	return resource_type_;
}

QDate MeterReading::getDate() const {
	return date_;
}

double MeterReading::getValue() const {
	return value_;
}

void MeterReading::setResourceType(const QString& resourceType) {
	resource_type_ = resourceType;
}

void MeterReading::setDate(const QDate& date) {
	date_ = date;
}

void MeterReading::setValue(double value) {
	value_ = value;
}

QString MeterReading::toString() const {
	return QString("\"%1\" %2 %3").arg(resource_type_).arg(date_.toString("yyyy.MM.dd")).arg(value_);
}

MeterReading MeterReading::fromString(const QString& str) {
	MeterReading result;
	QString error;
	if (!tryFromString(str, result, error)) {
		throw ParseException(error);
	}
	return result;
}

bool MeterReading::tryFromString(const QString& str, MeterReading& result, QString& errorMessage) {
	QRegularExpression regex("\"([^\"]+)\"\\s+(\\d{4}\\.\\d{2}\\.\\d{2})\\s+(\\d+\\.?\\d*)");
	QRegularExpressionMatch match = regex.match(str);

	if (!match.hasMatch()) {
		errorMessage = QString("Неверный формат строки: %1").arg(str);
		return false;
	}

	QString resourceType = match.captured(1);
	if (resourceType.isEmpty()) {
		errorMessage = "Тип ресурса не может быть пустым";
		return false;
	}

	QDate date = QDate::fromString(match.captured(2), "yyyy.MM.dd");
	if (!date.isValid()) {
		errorMessage = QString("Неверная дата: %1").arg(match.captured(2));
		return false;
	}

	bool ok;
	double value = match.captured(3).toDouble(&ok);
	if (!ok || value < 0) {
		errorMessage = QString("Неверное значение: %1").arg(match.captured(3));
		return false;
	}

	result = MeterReading(resourceType, date, value);
	return true;
}

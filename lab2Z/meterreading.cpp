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
	QRegularExpression regex("\"([^\"]+)\"\\s+(\\d{4}\\.\\d{2}\\.\\d{2})\\s+(\\d+\\.?\\d*)");
	QRegularExpressionMatch match = regex.match(str);

	if (match.hasMatch()) {
		QString resourceType = match.captured(1);
		QDate date = QDate::fromString(match.captured(2), "yyyy.MM.dd");
		double value = match.captured(3).toDouble();
		return MeterReading(resourceType, date, value);
	}

	return MeterReading();
}

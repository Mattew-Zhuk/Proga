#ifndef METERREADING_H
#define METERREADING_H

#include <QString>
#include <QDate>
#include <QMetaType>
#include <stdexcept>

class ParseException : public std::runtime_error {
public:
	explicit ParseException(const QString& message): std::runtime_error(message.toStdString()), message_(message) {}

	QString getMessage() const { return message_; }

private:
	QString message_;
};

class MeterReading {
public:
	MeterReading();
	MeterReading(const QString& resourceType, const QDate& date, double value);

	QString getResourceType() const;
	QDate getDate() const;
	double getValue() const;

	void setResourceType(const QString& resourceType);
	void setDate(const QDate& date);
	void setValue(double value);

	QString toString() const;
	static MeterReading fromString(const QString& str);
	static bool tryFromString(const QString& str, MeterReading& result, QString& errorMessage);

private:
	QString resource_type_;
	QDate date_;
	double value_;
};

Q_DECLARE_METATYPE(MeterReading)

#endif // METERREADING_H

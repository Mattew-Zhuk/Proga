#ifndef METERREADING_H
#define METERREADING_H

#include <QString>
#include <QDate>
#include <QMetaType>

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

private:
	QString resource_type_;
	QDate date_;
	double value_;
};

Q_DECLARE_METATYPE(MeterReading)

#endif // METERREADING_H

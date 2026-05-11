#ifndef CONDITIONPARSER_H
#define CONDITIONPARSER_H

#include <QString>
#include "meterreading.h"

class ConditionParser
{
public:
	static bool matches(const MeterReading& reading, const QString& condition, QString& errorMessage);
};

#endif // CONDITIONPARSER_H

#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <QString>
#include "meterreadingmodel.h"

class CommandProcessor {
public:
	explicit CommandProcessor(MeterReadingModel* model);

	bool processCommandsFromFile(const QString& fileName, QString& errorSummary);

private:
	bool processCommand(const QString& line, QString& error);

	MeterReadingModel* model_;
};

#endif // COMMANDPROCESSOR_H

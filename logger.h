#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMutex>

class Logger {
public:
	static Logger& instance();
	void logError(const QString& message);
	void logWarning(const QString& message);
	void logInfo(const QString& message);
	void setLogFile(const QString& filename);

private:
	Logger();
	~Logger();
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	QFile logFile_;
	QMutex mutex_;
};

#endif // LOGGER_H

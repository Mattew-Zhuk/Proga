#include "logger.h"
#include <QDateTime>

Logger::Logger() {
	setLogFile("errors.log");
}

Logger::~Logger() {
	if (logFile_.isOpen()) {
		logFile_.close();
	}
}

Logger& Logger::instance() {
	static Logger instance;
	return instance;
}

void Logger::setLogFile(const QString& filename) {
	QMutexLocker locker(&mutex_);
	if (logFile_.isOpen()) {
		logFile_.close();
	}
	logFile_.setFileName(filename);
	logFile_.open(QIODevice::Append | QIODevice::Text);
}

void Logger::logError(const QString& message) {
	QMutexLocker locker(&mutex_);
	if (logFile_.isOpen()) {
		QTextStream out(&logFile_);
		out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " [ERROR] " << message << "\n";
	}
}

void Logger::logWarning(const QString& message) {
	QMutexLocker locker(&mutex_);
	if (logFile_.isOpen()) {
		QTextStream out(&logFile_);
		out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " [WARNING] " << message << "\n";
	}
}

void Logger::logInfo(const QString& message) {
	QMutexLocker locker(&mutex_);
	if (logFile_.isOpen()) {
		QTextStream out(&logFile_);
		out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " [INFO] " << message << "\n";
	}
}

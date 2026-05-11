#include "commandprocessor.h"
#include "conditionparser.h"
#include "logger.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDate>

CommandProcessor::CommandProcessor(MeterReadingModel* model) : model_(model) {}

bool CommandProcessor::processCommandsFromFile(const QString& fileName, QString& errorSummary) {
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		errorSummary = QString("Не удалось открыть файл команд: %1").arg(fileName);
		Logger::instance().logError(errorSummary);
		return false;
	}

	QTextStream in(&file);
	int lineNum = 0;
	int successCount = 0;
	int errorCount = 0;
	QStringList errors;

	while (!in.atEnd()) {
		lineNum++;
		QString line = in.readLine().trimmed();
		if (line.isEmpty()) continue;

		QString cmdError;
		if (processCommand(line, cmdError)) {
			successCount++;
		} else {
			errorCount++;
			QString errMsg = QString("Строка %1: %2").arg(lineNum).arg(cmdError);
			errors << errMsg;
			Logger::instance().logError(errMsg);
		}
	}
	file.close();

	errorSummary = QString("Обработано команд: всего %1, успешно: %2, ошибок: %3")
					   .arg(successCount + errorCount).arg(successCount).arg(errorCount);
	if (!errors.isEmpty()) {
		errorSummary += "\nДетали ошибок:\n" + errors.join("\n");
	}

	return errorCount == 0;
}

bool CommandProcessor::processCommand(const QString& line, QString& error) {
	QRegularExpression regex(R"(^(\w+)\s+(.*)$)");
	QRegularExpressionMatch match = regex.match(line);
	if (!match.hasMatch()) {
		error = "Неизвестный формат команды";
		return false;
	}

	QString cmd = match.captured(1).toUpper();
	QString args = match.captured(2).trimmed();

	if (cmd == "ADD") {
		QStringList fields = args.split(';');
		if (fields.size() < 3) {
			error = "Недостаточно полей для ADD (требуется: тип; дата; значение)";
			return false;
		}
		QString resourceType = fields[0].trimmed();
		if (resourceType.isEmpty()) {
			error = "Тип ресурса не может быть пустым";
			return false;
		}
		QDate date = QDate::fromString(fields[1].trimmed(), "yyyy.MM.dd");
		if (!date.isValid()) {
			error = QString("Неверная дата: %1").arg(fields[1].trimmed());
			return false;
		}
		bool ok;
		double value = fields[2].trimmed().toDouble(&ok);
		if (!ok || value < 0) {
			error = QString("Неверное значение: %1").arg(fields[2].trimmed());
			return false;
		}
		model_->addReading(MeterReading(resourceType, date, value));
		return true;
	}
	else if (cmd == "REM") {
		QString condition = args;
		int removed = 0;
		for (int i = model_->getReadingCount() - 1; i >= 0; --i) {
			QString condError;
			if (ConditionParser::matches(model_->getReadingAt(i), condition, condError)) {
				model_->removeReading(i);
				removed++;
			} else if (!condError.isEmpty()) {
				error = QString("Ошибка в условии REM: %1").arg(condError);
				return false;
			}
		}
		return true;
	}
	else if (cmd == "SAVE") {
		if (args.isEmpty()) {
			error = "Не указано имя файла для сохранения";
			return false;
		}
		model_->saveToFile(args);
		return true;
	}
	else {
		error = QString("Неизвестная команда: %1").arg(cmd);
		return false;
	}
}

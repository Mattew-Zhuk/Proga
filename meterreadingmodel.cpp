#include "meterreadingmodel.h"
#include "logger.h"
#include <QFile>
#include <QTextStream>

MeterReadingModel::MeterReadingModel(QObject *parent) : QAbstractTableModel(parent) {}

int MeterReadingModel::rowCount(const QModelIndex &parent) const {
	if (parent.isValid()) return 0;
	return readings_.size();
}

int MeterReadingModel::columnCount(const QModelIndex &parent) const {
	if (parent.isValid()) return 0;
	return 3;
}

QVariant MeterReadingModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid() || index.row() >= readings_.size())
		return QVariant();

	if (role == Qt::DisplayRole) {
		const MeterReading& reading = readings_[index.row()];
		switch (index.column()) {
			case 0: return reading.getResourceType();
			case 1: return reading.getDate().toString("yyyy.MM.dd");
			case 2: return reading.getValue();
		}
	}

	return QVariant();
}

QVariant MeterReadingModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		switch (section) {
			case 0: return "Тип ресурса";
			case 1: return "Дата";
			case 2: return "Значение";
		}
	}
	return QVariant();
}

void MeterReadingModel::addReading(const MeterReading& reading) {
	beginInsertRows(QModelIndex(), readings_.size(), readings_.size());
	readings_.push_back(reading);
	endInsertRows();
}

void MeterReadingModel::removeReading(int row) {
	if (row >= 0 && row < readings_.size()) {
		beginRemoveRows(QModelIndex(), row, row);
		readings_.erase(readings_.begin() + row);
		endRemoveRows();
	}
}

void MeterReadingModel::loadFromFile(const QString& fileName) {
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		Logger::instance().logError(QString("Не удалось открыть файл: %1").arg(fileName));
		return;
	}

	beginResetModel();

	int validCount = 0;
	int errorCount = 0;
	int lineNumber = 0;

	QTextStream in(&file);
	while (!in.atEnd()) {
		lineNumber++;
		QString line = in.readLine().trimmed();

		if (line.isEmpty()) {
			continue;
		}

		MeterReading reading;
		QString errorMessage;

		if (MeterReading::tryFromString(line, reading, errorMessage)) {
			readings_.push_back(reading);
			validCount++;
		} else {
			errorCount++;
			Logger::instance().logError(QString("Строка %1: %2").arg(lineNumber).arg(errorMessage));
		}
	}

	file.close();

	Logger::instance().logInfo(QString("Загрузка завершена. Загружено: %1, Ошибок: %2").arg(validCount).arg(errorCount));

	endResetModel();
}

void MeterReadingModel::clear() {
	beginResetModel();
	readings_.clear();
	endResetModel();
}

int MeterReadingModel::getReadingCount() const {
	return readings_.size();
}

MeterReading MeterReadingModel::getReadingAt(int index) const {
	if (index >= 0 && index < readings_.size()) {
		return readings_[index];
	}
	return MeterReading();
}

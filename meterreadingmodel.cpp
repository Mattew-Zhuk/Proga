<<<<<<< HEAD
#include "meterreadingmodel.h"
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
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	beginResetModel();
	readings_.clear();

	QTextStream in(&file);
	while (!in.atEnd()) {
		QString line = in.readLine().trimmed();
		if (!line.isEmpty()) {
			MeterReading reading = MeterReading::fromString(line);
			readings_.push_back(reading);
		}
	}

	endResetModel();
	file.close();
}

void MeterReadingModel::clear() {
	beginResetModel();
	readings_.clear();
	endResetModel();
}
=======
#include "meterreadingmodel.h"
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
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	beginResetModel();
	readings_.clear();

	QTextStream in(&file);
	while (!in.atEnd()) {
		QString line = in.readLine().trimmed();
		if (!line.isEmpty()) {
			MeterReading reading = MeterReading::fromString(line);
			readings_.push_back(reading);
		}
	}

	endResetModel();
	file.close();
}

void MeterReadingModel::clear() {
	beginResetModel();
	readings_.clear();
	endResetModel();
}
>>>>>>> 7df45ef5055a00e4588a8006f8529f2845126fbb

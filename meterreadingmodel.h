#ifndef METERREADINGMODEL_H
#define METERREADINGMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include "meterreading.h"

class MeterReadingModel : public QAbstractTableModel {
	Q_OBJECT

public:
	explicit MeterReadingModel(QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	void addReading(const MeterReading& reading);
	void removeReading(int row);
	void loadFromFile(const QString& fileName);
	void clear();

private:
	std::vector<MeterReading> readings_;
};

#endif // METERREADINGMODEL_H

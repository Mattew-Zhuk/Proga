<<<<<<< HEAD
#ifndef FIRSTWINDOW_H
#define FIRSTWINDOW_H

#include <QDialog>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGroupBox>
#include "meterreadingmodel.h"

class FirstWindow : public QDialog
{
	Q_OBJECT

public:
	explicit FirstWindow(QWidget *parent = nullptr);

private slots:
	void addReading();
	void deleteSelectedReading();
	void loadFromFile();
	void onBackButtonClicked();

private:
	void setupUi();
	QLayout* createInputGroup();
	QLayout* createButtons();
	void createTableView();

	MeterReadingModel* model_;
	QTableView* tableView_;
	QLineEdit* resourceTypeEdit_;
	QDateEdit* dateEdit_;
	QDoubleSpinBox* valueSpinBox_;
	QPushButton* addButton_;
	QPushButton* deleteButton_;
	QPushButton* loadButton_;
	QPushButton *m_backButton;
};

#endif // FIRSTWINDOW_H
=======
#ifndef FIRSTWINDOW_H
#define FIRSTWINDOW_H

#include <QDialog>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGroupBox>
#include "meterreadingmodel.h"

class FirstWindow : public QDialog
{
	Q_OBJECT

public:
	explicit FirstWindow(QWidget *parent = nullptr);

private slots:
	void addReading();
	void deleteSelectedReading();
	void loadFromFile();
	void onBackButtonClicked();

private:
	void setupUi();
	QLayout* createInputGroup();
	QLayout* createButtons();
	void createTableView();

	MeterReadingModel* model_;
	QTableView* tableView_;
	QLineEdit* resourceTypeEdit_;
	QDateEdit* dateEdit_;
	QDoubleSpinBox* valueSpinBox_;
	QPushButton* addButton_;
	QPushButton* deleteButton_;
	QPushButton* loadButton_;
	QPushButton *m_backButton;
};

#endif // FIRSTWINDOW_H
>>>>>>> 7df45ef5055a00e4588a8006f8529f2845126fbb

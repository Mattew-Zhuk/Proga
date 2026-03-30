#include "firstwindow.h"
#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>

FirstWindow::FirstWindow(QWidget *parent) : QDialog(parent) {
	model_ = new MeterReadingModel(this);
	setupUi();

	loadFromFile();

	connect(m_backButton, &QPushButton::clicked, this, &FirstWindow::onBackButtonClicked);
}

void FirstWindow::setupUi() {
	QWidget* centralWidget = new QWidget(this);
	QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);


	m_backButton = new QPushButton("Назад", this);

	createTableView();
	mainLayout->addWidget(tableView_);
	mainLayout->addLayout(createInputGroup());
	mainLayout->addLayout(createButtons());
	mainLayout->addWidget(m_backButton);



	setModal (true);
	setWindowTitle("Показания счетчиков");
	resize(800, 600);
}
void FirstWindow::onBackButtonClicked()
{
	accept();
}
QLayout* FirstWindow::createInputGroup() {
	QHBoxLayout* layout = new QHBoxLayout();

	QGroupBox* inputGroup = new QGroupBox("Добавление показания");
	QHBoxLayout* groupLayout = new QHBoxLayout(inputGroup);

	QLabel* resourceLabel = new QLabel("Тип ресурса:");
	resourceTypeEdit_ = new QLineEdit();
	resourceTypeEdit_->setPlaceholderText("Например: Электричество");

	QLabel* dateLabel = new QLabel("Дата:");
	dateEdit_ = new QDateEdit();
	dateEdit_->setDate(QDate::currentDate());
	dateEdit_->setCalendarPopup(true);
	dateEdit_->setDisplayFormat("yyyy.MM.dd");

	QLabel* valueLabel = new QLabel("Значение:");
	valueSpinBox_ = new QDoubleSpinBox();
	valueSpinBox_->setRange(0, 1000000);
	valueSpinBox_->setDecimals(2);

	addButton_ = new QPushButton("Добавить");
	connect(addButton_, &QPushButton::clicked, this, &FirstWindow::addReading);

	groupLayout->addWidget(resourceLabel);
	groupLayout->addWidget(resourceTypeEdit_);
	groupLayout->addWidget(dateLabel);
	groupLayout->addWidget(dateEdit_);
	groupLayout->addWidget(valueLabel);
	groupLayout->addWidget(valueSpinBox_);
	groupLayout->addWidget(addButton_);

	layout->addWidget(inputGroup);
	return layout;
}

QLayout* FirstWindow::createButtons() {
	QHBoxLayout* layout = new QHBoxLayout();

	deleteButton_ = new QPushButton("Удалить выделенное");
	connect(deleteButton_, &QPushButton::clicked, this, &FirstWindow::deleteSelectedReading);

	loadButton_ = new QPushButton("Загрузить из файла");
	connect(loadButton_, &QPushButton::clicked, this, &FirstWindow::loadFromFile);

	layout->addWidget(deleteButton_);
	layout->addWidget(loadButton_);
	layout->addStretch();

	return layout;
}

void FirstWindow::createTableView() {
	tableView_ = new QTableView();
	tableView_->setModel(model_);
	tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableView_->setSelectionMode(QAbstractItemView::SingleSelection);
	tableView_->horizontalHeader()->setStretchLastSection(true);
	tableView_->resizeColumnsToContents();
}

void FirstWindow::addReading() {
	QString resourceType = resourceTypeEdit_->text().trimmed();
	if (resourceType.isEmpty()) {
		QMessageBox::warning(this, "Предупреждение", "Введите тип ресурса");
		return;
	}

	MeterReading reading(resourceType, dateEdit_->date(), valueSpinBox_->value());
	model_->addReading(reading);

	resourceTypeEdit_->clear();
	dateEdit_->setDate(QDate::currentDate());
	valueSpinBox_->setValue(0);

	tableView_->resizeColumnsToContents();
}

void FirstWindow::deleteSelectedReading() {
	QModelIndex currentIndex = tableView_->currentIndex();
	if (currentIndex.isValid()) {
		model_->removeReading(currentIndex.row());
	}
}

void FirstWindow::loadFromFile() {
	QString fileName = "data.txt";
	//QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл", "", "Текстовые файлы (*.txt)");

	model_->loadFromFile(fileName);
	tableView_->resizeColumnsToContents();
}

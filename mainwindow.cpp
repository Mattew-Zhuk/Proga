#include "mainwindow.h"
#include "firstwindow.h"
#include "helpwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setupUI();

	connect(m_firstButton, &QPushButton::clicked, this, &MainWindow::onFirstButtonClicked);
	connect(m_helpButton, &QPushButton::clicked, this, &MainWindow::onHelpButtonClicked);
	connect(m_exitButton, &QPushButton::clicked, this, &MainWindow::onExitButtonClicked);
}

void MainWindow::setupUI()
{
	setWindowTitle("Главное окно");
	resize(300, 200);

	QWidget *centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	QVBoxLayout *layout = new QVBoxLayout(centralWidget);

	m_firstButton = new QPushButton("Первая", this);
	m_helpButton = new QPushButton("Справка", this);
	m_exitButton = new QPushButton("Выход", this);

	layout->addWidget(m_firstButton);
	layout->addWidget(m_helpButton);
	layout->addWidget(m_exitButton);
	layout->addStretch();
}

void MainWindow::onFirstButtonClicked()
{
	FirstWindow *firstWindow = new FirstWindow(this);
	firstWindow->exec();
}

void MainWindow::onHelpButtonClicked()
{
	HelpWindow *helpWindow = new HelpWindow(this);
	helpWindow->exec();
}

void MainWindow::onExitButtonClicked()
{
	QApplication::quit();
}

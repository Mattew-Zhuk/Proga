#include "firstwindow.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

FirstWindow::FirstWindow(QWidget *parent) : QDialog(parent)
{
	setupUI();

	connect(m_backButton, &QPushButton::clicked, this, &FirstWindow::onBackButtonClicked);
}

void FirstWindow::setupUI()
{
	setWindowTitle("Первое окно");
	setModal(true);
	resize(300, 200);

	QVBoxLayout *layout = new QVBoxLayout(this);

	QLabel *label = new QLabel("Это первое окно", this);
	label->setAlignment(Qt::AlignCenter);

	m_backButton = new QPushButton("Назад", this);

	layout->addWidget(label);
	layout->addWidget(m_backButton);
}

void FirstWindow::onBackButtonClicked()
{
	accept();
}

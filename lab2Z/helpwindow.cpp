#include "helpwindow.h"
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QFile>
#include <QUrl>

HelpWindow::HelpWindow(QWidget *parent) : QDialog(parent)
{
	setupUI();
	loadHelpContent();

	connect(m_backButton, &QPushButton::clicked, this, &HelpWindow::onBackButtonClicked);
}

void HelpWindow::setupUI()
{
	setWindowTitle("Справка");
	setModal(true);
	resize(400, 300);

	QVBoxLayout *layout = new QVBoxLayout(this);

	m_textBrowser = new QTextBrowser(this);

	m_backButton = new QPushButton("Назад", this);

	layout->addWidget(m_textBrowser);
	layout->addWidget(m_backButton);
}

void HelpWindow::loadHelpContent()
{
	QString htmlContent = R"(
	<!DOCTYPE html>
	<html>
	<head>
		<style>
			body {
				font-family: Arial, sans-serif;
				margin: 20px;
				background-color: #000000;
			}
			h1 {
				color: #333;
				text-align: center;
			}
			p {
				color: #666;
				line-height: 1.6;
			}
			.info {
				background-color: #e7f3ff;
				border-left: 4px solid #2196F3;
				padding: 10px;
				margin: 10px 0;
			}
		</style>
	</head>
	<body>
		<h1>Справка по программе</h1>

		<div class="info">
			<h2>О программе</h2>
		</div>

		<h2>Функции программы:</h2>
		<ul>
			<li><b>Первая</b> - открывает первое окно</li>
			<li><b>Справка</b> - показывает эту справку</li>
			<li><b>Выход</b> - завершает работу программы</li>
		</ul>

		<h2>Навигация:</h2>
		<p>В каждом окне есть кнопка "Назад" для возврата в главное окно.</p>

	</body>
	</html>
	)";

	m_textBrowser->setHtml(htmlContent);
}

void HelpWindow::onBackButtonClicked()
{
	accept();
}

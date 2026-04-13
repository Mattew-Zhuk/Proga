<<<<<<< HEAD
#include "helpwindow.h"
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QFile>
#include <QUrl>
#include <QDir>

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
	resize(500, 400);

	QVBoxLayout *layout = new QVBoxLayout(this);

	m_textBrowser = new QTextBrowser(this);

	m_backButton = new QPushButton("Назад", this);

	layout->addWidget(m_textBrowser);
	layout->addWidget(m_backButton);
}

void HelpWindow::loadHelpContent()
{
	QString imagePath = QDir::currentPath() + "/image.jpg";

	QUrl imageUrl = QUrl::fromLocalFile(imagePath);

	QString htmlContent = QString(R"(
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
				color: #FFFFFF;
				text-align: center;
			}
			.image-container {
				text-align: center;
				margin: 20px 0;
				padding: 10px;
				background-color: black;
				border: 1px solid #ddd;
				border-radius: 5px;
			}
			img {
				max-width: 100%%;
				height: auto;
				border-radius: 3px;
			}
			.info {
				background-color: #FFFFFF;
				border-left: 4px solid #2196F3;
				padding: 10px;
				margin: 10px 0;
			}
		</style>
	</head>
	<body>
		<h1>Справка по программе</h1>

		<div class="image-container">
			<h3>Пример изображения:</h3>
			<img src="%1" alt="Изображение не найдено">
			<p><i>Путь к изображению: %2</i></p>
		</div>

		<h2>Функции программы:</h2>
		<ul>
			<li><b>Первая</b> - открывает первое окно</li>
			<li><b>Справка</b> - показывает эту справку с изображением</li>
			<li><b>Выход</b> - завершает работу программы</li>
		</ul>

	</body>
	</html>
	)").arg(imageUrl.toString()).arg(imagePath);

	m_textBrowser->setHtml(htmlContent);
}

void HelpWindow::onBackButtonClicked()
{
	accept();
}
=======
#include "helpwindow.h"
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QFile>
#include <QUrl>
#include <QDir>

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
	resize(500, 400);

	QVBoxLayout *layout = new QVBoxLayout(this);

	m_textBrowser = new QTextBrowser(this);

	m_backButton = new QPushButton("Назад", this);

	layout->addWidget(m_textBrowser);
	layout->addWidget(m_backButton);
}

void HelpWindow::loadHelpContent()
{
	QString imagePath = QDir::currentPath() + "/image.jpg";

	QUrl imageUrl = QUrl::fromLocalFile(imagePath);

	QString htmlContent = QString(R"(
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
				color: #FFFFFF;
				text-align: center;
			}
			.image-container {
				text-align: center;
				margin: 20px 0;
				padding: 10px;
				background-color: black;
				border: 1px solid #ddd;
				border-radius: 5px;
			}
			img {
				max-width: 100%%;
				height: auto;
				border-radius: 3px;
			}
			.info {
				background-color: #FFFFFF;
				border-left: 4px solid #2196F3;
				padding: 10px;
				margin: 10px 0;
			}
		</style>
	</head>
	<body>
		<h1>Справка по программе</h1>

		<div class="image-container">
			<h3>Пример изображения:</h3>
			<img src="%1" alt="Изображение не найдено">
			<p><i>Путь к изображению: %2</i></p>
		</div>

		<h2>Функции программы:</h2>
		<ul>
			<li><b>Первая</b> - открывает первое окно</li>
			<li><b>Справка</b> - показывает эту справку с изображением</li>
			<li><b>Выход</b> - завершает работу программы</li>
		</ul>

	</body>
	</html>
	)").arg(imageUrl.toString()).arg(imagePath);

	m_textBrowser->setHtml(htmlContent);
}

void HelpWindow::onBackButtonClicked()
{
	accept();
}
>>>>>>> 7df45ef5055a00e4588a8006f8529f2845126fbb

<<<<<<< HEAD
#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>

class QPushButton;
class QTextBrowser;

class HelpWindow : public QDialog
{
	Q_OBJECT
public:
	explicit HelpWindow(QWidget *parent = nullptr);

private slots:
	void onBackButtonClicked();

private:
	void setupUI();
	void loadHelpContent();

	QPushButton *m_backButton;
	QTextBrowser *m_textBrowser;
};

#endif // HELPWINDOW_H
=======
#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>

class QPushButton;
class QTextBrowser;

class HelpWindow : public QDialog
{
	Q_OBJECT
public:
	explicit HelpWindow(QWidget *parent = nullptr);

private slots:
	void onBackButtonClicked();

private:
	void setupUI();
	void loadHelpContent();

	QPushButton *m_backButton;
	QTextBrowser *m_textBrowser;
};

#endif // HELPWINDOW_H
>>>>>>> 7df45ef5055a00e4588a8006f8529f2845126fbb

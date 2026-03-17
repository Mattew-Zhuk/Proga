#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPushButton;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = nullptr);

private slots:
	void onFirstButtonClicked();
	void onHelpButtonClicked();
	void onExitButtonClicked();

private:
	void setupUI();

	QPushButton *m_firstButton;
	QPushButton *m_helpButton;
	QPushButton *m_exitButton;
};

#endif // MAINWINDOW_H

#ifndef FIRSTWINDOW_H
#define FIRSTWINDOW_H

#include <QDialog>

class QPushButton;

class FirstWindow : public QDialog
{
	Q_OBJECT
public:
	explicit FirstWindow(QWidget *parent = nullptr);

private slots:
	void onBackButtonClicked();

private:
	void setupUI();

	QPushButton *m_backButton;
};

#endif // FIRSTWINDOW_H

#ifndef ABOUTTEXTDIALOG_H
#define ABOUTTEXTDIALOG_H


#include <QDialog>
#include "ui_abouttextdialog.h"
//#include <QtGui>


class AboutTextDialog : public QDialog
{
	Q_OBJECT

public:
	AboutTextDialog(QWidget *parent = 0);
	~AboutTextDialog();

private:
	Ui::aboutText ui;


	private slots :

	void showText(void);
	void closeText(void);
	void addText();
};

#endif // ABOUTTEXTDIALOG_H

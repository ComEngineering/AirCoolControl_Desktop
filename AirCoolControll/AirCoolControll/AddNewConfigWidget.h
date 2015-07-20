#ifndef ADDNEWCONFIGWIDGET_H
#define ADDNEWCONFIGWIDGET_H

#include <QDialog>
#include "ui_AddNewConfigWidget.h"

class AddNewConfigWidget : public QDialog
{
    Q_OBJECT

public:
    AddNewConfigWidget(QWidget *parent = 0);
    ~AddNewConfigWidget();

private:
    Ui::AddNewConfigWidget ui;
};

#endif // ADDNEWCONFIGWIDGET_H

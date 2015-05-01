#ifndef COOLERSTATEWIDGET_H
#define COOLERSTATEWIDGET_H

#include <QWidget>
#include "ui_coolerstatewidget.h"
#include <qstandarditemmodel.h>


class CoolerStateWidget : public QWidget
{
    Q_OBJECT

public:
    CoolerStateWidget(QWidget *parent = 0);
    ~CoolerStateWidget();

    void setParameterList(const std::vector<std::pair<std::string,std::string>>& list,bool isInput);
    
    void updateParameter(int n, int value, bool isInput);

private slots:
    void registerSet(QStandardItem *);

signals:
    void newRegisterValue(QString&, int);

private:
    Ui::CoolerStateWidget ui;    
};

#endif // COOLERSTATEWIDGET_H

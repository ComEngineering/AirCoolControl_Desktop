#ifndef COOLERSTATEWIDGET_H
#define COOLERSTATEWIDGET_H

#include <QWidget>
#include "ui_coolerstatewidget.h"
#include <qstandarditemmodel.h>
#include "ConfigMap.h"

class CoolerStateWidget : public QWidget
{
    Q_OBJECT

public:
    CoolerStateWidget(QWidget *parent = 0);
    ~CoolerStateWidget();

    void setParameterList(const std::vector<std::pair<std::string, std::string>>& list, ConfigMap::RegisterType type);
    
    void updateParameter(int n, int value, ConfigMap::RegisterType type);

    void clear();

private slots:
    void registerSet(QTableWidgetItem *);

signals:
    void newRegisterValue(QString&, int);

private:
    Ui::CoolerStateWidget ui;    
    QTableWidget*         m_tables[ConfigMap::REGISTER_PULL_COUNT];
};

#endif // COOLERSTATEWIDGET_H

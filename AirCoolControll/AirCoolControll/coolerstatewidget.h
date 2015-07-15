#ifndef COOLERSTATEWIDGET_H
#define COOLERSTATEWIDGET_H

#include <QWidget>
#include "ui_coolerstatewidget.h"
#include <qstandarditemmodel.h>
#include "ConfigMap.h"
#include <qmutex.h>
#include <qstring.h>
#include <map>

class DeviceExplorer;

class CoolerStateWidget : public QWidget
{
    Q_OBJECT

public:
    CoolerStateWidget(DeviceExplorer *parent);
    ~CoolerStateWidget();

    void setParameterList(const std::vector<std::pair<std::string, std::string>>& list, ConfigMap::RegisterType type);
    
    void updateParameter(int n, QVariant value, ConfigMap::RegisterType type);
private:
    void initPlotter(void);

private slots:
    void onCoilChanged();
    void registerSet(int);
    void onPlotCheckChanged();

signals:
    void newRegisterValue(int,QString&, int);

protected:
    void timerEvent(QTimerEvent *event);

private:
    Ui::CoolerStateWidget        ui;    
    QTableWidget*                m_tables[ConfigMap::REGISTER_PULL_COUNT];
    mutable  QMutex              m_updateMutex;
    std::map<QString,QCPGraph*>  m_plotList;
    DeviceExplorer *             m_parent;
    bool                         m_needSetTimeRange;
    int                          m_colorEnumerator;
};

#endif // COOLERSTATEWIDGET_H

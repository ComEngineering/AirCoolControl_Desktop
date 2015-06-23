#ifndef DeviceExplorer_H
#define DeviceExplorer_H

#include <QObject>
#include "ConfigMap.h"
#include <memory>
#include "ModbusDriver.h"
#include "PullerReadCoilTask.h"
#include "coolerstatewidget.h"
#include "MdiSubWindowPermanent.h"
#include "RegestryHistory.h"
#include "connectionlog.h"

class DeviceExplorer : public QObject
{
    Q_OBJECT

public:
    DeviceExplorer(const ConfigMapShared config, ModbusDriverShared modbus, DeviceInfoShared info, QObject *parent = NULL);
    ~DeviceExplorer();

    bool  getRegisterValue(const std::string & key,int& value);
    void  setRegisterValue(const std::string & key,int value);
    void  setCoilState(const std::string & key, bool state);

    QString errorString();
    ConfigMapShared getCurrentConfig(){ return m_currentMap; }
    void  stopTasks();

    void updateStateWidget(void);

    void activateView(QMdiArea * area);

    void somethingChanged();

    void setListView(ConnectionLog* view);

private slots:
    void sendValueToDevice(int, QString&, int);
    void viewStateChanged(Qt::WindowStates , Qt::WindowStates);

private:
    DeviceInfo                m_info;
    QString                   m_errorString;

    ConfigMapShared           m_currentMap;
    ModbusDriverShared        m_modbus;
    std::vector<quint16>      m_localPull[ConfigMap::REGISTER_PULL_COUNT];
    
    PullerReadTaskShared      m_registers[ConfigMap::REGISTER_PULL_COUNT];

    CoolerStateWidget*        m_view;
    MdiSubWindowPermanent*    m_mdi;
    ConnectionLog*            m_listView;

    RegestryHistory           m_history;
};

typedef std::shared_ptr<DeviceExplorer> DeviceExplorerShared;

#endif // DeviceExplorer_H

#ifndef DeviceExplorer_H
#define DeviceExplorer_H

#include <QObject>
#include "ConfigMap.h"
#include <memory>
#include "ModbusDriver.h"
#include "PullerReadCoilTask.h"
#include "coolerstatewidget.h"
#include "MdiSubWindowPermanent.h"

class DeviceExplorer : public QObject
{
    Q_OBJECT

public:
    enum State{
        Ready = 0,
        DeviceNotReady,
        ConfigNotFound
    };

    DeviceExplorer(const ConfigMapShared config, ModbusDriverShared modbus, DeviceInfoShared info, QObject *parent = NULL);
    ~DeviceExplorer();

    bool  getRegisterValue(const std::string & key,int& value);
    void  setRegisterValue(const std::string & key,int value);
    void  setCoilState(const std::string & key, bool state);

    QString errorString();
    ConfigMapShared getCurrentConfig(){ return m_currentMap; }
    State getState(){ return m_state; }
    void  stopTasks();

    void updateStateWidget(void);

    void activateView(QMdiArea * area);

private slots:
    void sendValueToDevice(int, QString&, int);

private:
    State                     m_state;
    DeviceInfoShared          m_deviceInfo;
    QString                   m_errorString;
    ConfigMapShared           m_currentMap;
    ModbusDriverShared        m_modbus;
    QVector<quint16>          m_localPull[ConfigMap::REGISTER_PULL_COUNT];
    
    PullerReadTaskShared      m_registers[ConfigMap::REGISTER_PULL_COUNT];

    CoolerStateWidget*        m_view;
    MdiSubWindowPermanent*    m_mdi;
};

typedef std::shared_ptr<DeviceExplorer> DeviceExplorerShared;

#endif // DeviceExplorer_H

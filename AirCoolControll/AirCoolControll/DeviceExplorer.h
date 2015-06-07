#ifndef DeviceExplorer_H
#define DeviceExplorer_H

#include <QObject>
#include "ConfigMap.h"
#include <memory>
#include "ModbusDriver.h"
#include "PullerReadCoilTask.h"

class DeviceExplorer : public QObject
{
    Q_OBJECT

public:
    enum State{
        Ready = 0,
        DeviceNotReady,
        ConfigNotFound
    };

    DeviceExplorer(const ConfigMapShared config, ModbusDriverShared modbus, int id, QObject *parent = NULL);
    ~DeviceExplorer();

    bool  getRegisterValue(const std::string & key,int& value);
    void  setRegisterValue(const std::string & key,int value);
    void  setCoilState(const std::string & key, bool state);

    QString errorString();
    ConfigMapShared getCurrentConfig(){ return m_currentMap; }
    State getState(){ return m_state; }
    void  stopTasks();

private:
    

private:
    State                     m_state;
    int                       m_deviceID;
    QString                   m_errorString;
    ConfigMapShared           m_currentMap;
    ModbusDriverShared        m_modbus;
    QVector<quint16>          m_localPull[ConfigMap::REGISTER_PULL_COUNT];
    
    PullerReadTaskShared      m_registers[ConfigMap::REGISTER_PULL_COUNT];
};

typedef std::shared_ptr<DeviceExplorer> DeviceExplorerShared;

#endif // DeviceExplorer_H

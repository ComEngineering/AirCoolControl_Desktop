#ifndef DeviceExplorer_H
#define DeviceExplorer_H

#include <QObject>
#include "ConfigMap.h"
#include <memory>
#include "ModbusDriver.h"

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
    bool  setRegisterValue(const std::string & key,int value);
    QString errorString();
    ConfigMapShared getCurrentConfig(){ return m_currentMap; }
    State getState(){ return m_state; }
    void  stopTasks();

private:
    

private:
    State                   m_state;
    int                     m_deviceID;
    QString                 m_errorString;
    ConfigMapShared         m_currentMap;
    ModbusDriverShared      m_modbus;
    QVector<quint16>        m_localInPull;
    QVector<quint16>        m_localOutPull;
    PullerReadTaskShared    m_inRegisters;
    PullerReadTaskShared    m_outRegisters;
};

typedef std::shared_ptr<DeviceExplorer> DeviceExplorerShared;

#endif // DeviceExplorer_H

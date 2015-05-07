#ifndef COOLLEREXPLORER_H
#define COOLLEREXPLORER_H

#include <QObject>
#include "ConfigMap.h"
#include <memory>
#include "PullerReadTask.h"
#include "ModbusDriver.h"

class CoollerExplorer : public QObject
{
    Q_OBJECT

public:
    enum State{Ready = 0,DeviceNotReady,ConfigNotFound};

    CoollerExplorer(const ConfigList& configs,ModbusDriver& modbus,int id,QObject *parent);
    ~CoollerExplorer();

    bool  getRegisterValue(const std::string & key,int& value);
    bool  setRegisterValue(const std::string & key,int value);
    QString errorString();
    ConfigMapShared getCurrentConfig(){ return m_currentMap; }
    State getState(){ return m_state; }
    void  stopTasks();

private:
    

private:
    State             m_state;
    QString           m_errorString;
    const ConfigList& m_configs;
    ConfigMapShared   m_currentMap;
    ModbusDriver&     m_modbus;
    QVector<quint16>  m_localInPull;
    QVector<quint16>  m_localOutPull;
    PullerReadTaskShared  m_inRegisters;
    PullerReadTaskShared  m_outRegisters;
    int               m_currentDeviceID;
};

typedef std::shared_ptr<CoollerExplorer> CoollerExplorerShared;
#endif // COOLLEREXPLORER_H

#ifndef MODBUSDRIVER_H
#define MODBUSDRIVER_H

#include <QObject>
#include "PullerReadTask.h"
#include "VersionStorage.h"
#include "modbusuart_impl.h"
#include "ModbusRegisterPuller.h"
#include <qmutex.h>
#include <map>

class ModbusDriver : public QObject
{
    Q_OBJECT

public:
    ModbusDriver(QObject *parent);
    ~ModbusDriver();

    bool setPortName(const QString& name);
    void addPullerReadTask(PullerReadTaskShared a_task);
    bool readDeviceInfo(quint16 id, DeviceInfo& info);
    bool writeRegister(quint16 id, quint16 regNumber, quint16 value);
    void getDeviceList(DeviceInfoMap& map) const;
    bool readyToWork() const;
    void removeTaskWithID(int id);

signals:
    void deviceListUpdated(void);

private slots:
    void UARTfail(void);

private:
    ModBusUART_ImplShared m_modbus;
    ModbusRegisterPuller  m_puller;
    QString               m_currentPortName;
    mutable QMutex *      m_mutex;
};

#endif // MODBUSDRIVER_H

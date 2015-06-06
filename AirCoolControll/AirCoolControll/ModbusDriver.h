#ifndef MODBUSDRIVER_H
#define MODBUSDRIVER_H

#include <QObject>
#include <qmutex.h>
#include <map>
#include <memory>
#include <functional>
#include "VersionStorage.h"
#include "modbusuart_impl.h"
#include "ModbusPuller.h"
#include "DeviceInfo.h"

class ModbusDriver : public QObject
{
    Q_OBJECT

public:
    typedef std::function<void(DeviceInfoShared)> detectionCallback;

    ModbusDriver(const QString& name,QObject *parent = NULL);
    ~ModbusDriver();

    void addPullerReadTask(PullerTaskShared a_task);
    void requestDeviceAproval(quint16 id);
    void writeRegister(quint16 id, quint16 regNumber, quint16 value);
    bool readyToWork() const;
    void removeTaskWithID(int id);
    void onDeviceDetected(DeviceInfoShared a_info);
    const QString& getName(void) const { return m_currentPortName; }

signals:
    void deviceDetected(DeviceInfoShared);
    void connectionFail();

private slots:
    void UARTfail(void);

private:
    ModBusUART_Impl*      m_modbus;
    ModbusPuller          m_puller;
    QString               m_currentPortName;
    mutable QMutex *      m_mutex;
};

typedef std::shared_ptr<ModbusDriver> ModbusDriverShared;

#include "DeviceInfo.h"

#endif // MODBUSDRIVER_H

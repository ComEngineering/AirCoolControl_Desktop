#ifndef ModbusRegisterPuller_H
#define ModbusRegisterPuller_H

#include <QThread>
#include "IPullerTask.h"
#include <qlist.h>
#include "modbusuart_impl.h"
#include <qsemaphore.h>

class ModbusRegisterPuller : public QThread
{
    Q_OBJECT

public:
    ModbusRegisterPuller(QObject *parent);
    ~ModbusRegisterPuller();

    void clearTaskList();
    void removeTaskWithID(int id);
    void addTask(PullerTaskShared a_task);
    void startPulling(ModBusUART_ImplShared modbus);
    void stopPulling();
    void getDeviceInfoList(DeviceInfoMap& to_map) const;

protected:
    void run(void);

signals:
    void deviceListUpdated(void);

private:
    QList<PullerTaskShared> m_tasks;
    ModBusUART_ImplShared   m_modbus;
    bool                    m_isStoped;
    DeviceInfoMap           m_deviceInfoList;
    mutable QMutex          m_infoMapMutex;
    mutable QMutex          m_taskMutex;
    QSemaphore              m_endProcessingSemaphore;
    bool                    m_continueProcessing;
};

#endif // ModbusRegisterPuller_H

#ifndef ModbusPuller_H
#define ModbusPuller_H

#include <QThread>
#include "PullerTaskBase.h"
#include <qlist.h>
#include "modbusuart_impl.h"
#include <qsemaphore.h>

class ModbusPuller : public QThread
{
    Q_OBJECT

public:
    ModbusPuller(QObject *parent);
    ~ModbusPuller();

    void clearTaskList();
    void removeTaskWithID(int id);
    void addTask(PullerTaskShared a_task);
    void startPulling(ModBusUART_ImplShared modbus);
    void stopPulling();

protected:
    void run(void);

signals:

private:
    QList<PullerTaskShared> m_tasks;
    ModBusUART_ImplShared   m_modbus;
    bool                    m_isStoped;
    mutable QMutex          m_infoMapMutex;
    mutable QMutex          m_taskMutex;
    QSemaphore              m_endProcessingSemaphore;
    bool                    m_continueProcessing;
};

#endif // ModbusPuller_H

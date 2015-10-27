#ifndef ModbusPuller_H
#define ModbusPuller_H

#include "PullerTaskBase.h"
#include <qlist.h>
#include "modbusuart_impl.h"
#include <qmutex.h>
#include <qtimer.h>

class ModbusPuller : QObject
{
    Q_OBJECT

public:
    ModbusPuller(QObject *parent);
    ~ModbusPuller();

    void clearTaskList();
    void removeTaskWithID(int id);
    void addTask(PullerTaskShared a_task);
    void startPulling(const QString& uartName);
    void stopPulling();
    void taskFinished(bool remove);

private slots:
    void process();

private:
    void schedule();

    QList<PullerTaskShared> m_tasks;
    ModBusUART_Impl*        m_modbus;
    bool                    m_isStoped;
    mutable QMutex          m_taskMutex;
    /// variable that indicates if some kind of task has to be removed from task list.
    // 0 - nothing
    // -1 - all
    // id - for all requests with apropriate device id 
    QTimer                  m_timer;
    int                     m_currentTaskIndex;
    bool                    m_modbusBusy;
};

#endif // ModbusPuller_H

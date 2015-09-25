#ifndef ModbusPuller_H
#define ModbusPuller_H

#include <QThread>
#include "PullerTaskBase.h"
#include <qlist.h>
#include "modbusuart_impl.h"
#include <qmutex.h>
#include <qeventloop.h>
#include <qtimer.h>

class ModbusPuller : public QThread
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

protected:
    void run(void);

private slots:
    void process();

private:
    QList<PullerTaskShared> m_tasks;
    QList<PullerTaskShared> m_newTasks;
    ModBusUART_Impl*        m_modbus;
    bool                    m_isStoped;
    mutable QMutex          m_taskMutex;
    /// variable that indicates if some kind of task has to be removed from task list.
    // 0 - nothing
    // -1 - all
    // id - for all requests with apropriate device id 
    volatile int            m_removeIndex;
    QEventLoop              m_eventLoop;
    QTimer                  m_timer;
};

#endif // ModbusPuller_H

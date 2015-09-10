#include "ModbusPuller.h"
#include <qwaitcondition.h>

ModbusPuller::ModbusPuller(QObject *parent)
    : QThread(parent),
    m_modbus(NULL),
    m_isStoped(true),
    m_removeIndex(0)
{
   ;
}

ModbusPuller::~ModbusPuller()
{
    stopPulling();
}

void ModbusPuller::clearTaskList()
{
    QMutexLocker lock(&m_taskMutex);
    m_removeIndex = -1;
}

void ModbusPuller::removeTaskWithID(int id)
{
    QMutexLocker lock(&m_taskMutex);
    m_removeIndex = id;
}

void ModbusPuller::addTask(PullerTaskShared a_task)
{
    QMutexLocker lock(&m_taskMutex);
    m_newTasks.append(a_task);
    m_timer.start(0);
}

void ModbusPuller::process(void)
{
    if (!m_modbus)
        return;
    
    bool highestPriorityTaskDone = true;
    
    for (PullerTaskBase::Priority p = PullerTaskBase::Priority::High; highestPriorityTaskDone && p < PullerTaskBase::Priority::Count; PullerTaskBase::NEXT(p))
    {
        for (QList<PullerTaskShared>::iterator task = m_tasks.begin(); task != m_tasks.end(); task++)
        {
            if ((*task)->priority() == p && (*task)->isItTimeToDo())
            {
                if (true == (*task)->proceed(m_modbus))
                {
                    m_tasks.erase(task);
                    break;
                }
                highestPriorityTaskDone = false;
            }
        }
    }

    {
        QMutexLocker lock(&m_taskMutex);
        if (m_newTasks.size() != 0)
        {
            for (PullerTaskShared a_task : m_newTasks)
                m_tasks.push_back(a_task);
            
            m_newTasks.clear();
        }

        if (0 != m_removeIndex)
        {
            if (-1 == m_removeIndex)
                m_tasks.clear();
            else
            {
                bool finished = true;
                while (finished && m_tasks.size())
                {
                    finished = false;
                    for (QList<PullerTaskShared>::iterator it = m_tasks.begin(); it != m_tasks.end(); it++)
                    {
                        if (m_removeIndex == (*it)->getID())
                        {
                            m_tasks.erase(it);
                            finished = true;
                            break;
                        }
                    }
                }
            }
            m_removeIndex = 0;
        }
    }
        
    if (m_tasks.size() != 0)
    {
        int pause = (*m_tasks.begin())->millisecondsToCall();
        for (QList<PullerTaskShared>::iterator task = m_tasks.begin() + 1; task != m_tasks.end(); task++)
        {
            int a_pause = (*task)->millisecondsToCall();
            if (a_pause < pause)
                pause = a_pause;
        }
        if (pause < 0)
            pause = 0;
        m_timer.start(pause);
    }
}

void ModbusPuller::run(void)
{
    int currentScanningID = 1;

    m_timer.setSingleShot(true);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(process()));
    
    exec();
    
    if (m_modbus)
        delete m_modbus;
}

void ModbusPuller::startPulling(const QString& uartName)
{
    if (m_modbus)
        delete m_modbus;

    m_modbus = new ModBusUART_Impl(uartName);
}

void ModbusPuller::stopPulling()
{
    exit();
}

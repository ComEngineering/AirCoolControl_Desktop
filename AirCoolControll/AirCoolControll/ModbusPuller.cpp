#include "ModbusPuller.h"

ModbusPuller::ModbusPuller(QObject *parent)
    : QThread(parent),
    m_modbus(NULL),
    m_isStoped(true),
    m_continueProcessing(true),
    m_endProcessingSemaphore(1),
    m_removeIndex(0)
{
   
}

ModbusPuller::~ModbusPuller()
{
    if(m_continueProcessing)
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
    m_isStoped = false;
}

void ModbusPuller::run(void)
{
    m_endProcessingSemaphore.acquire(1);
    int currentScanningID = 1;
    QString vendor, product, version;
    bool highestPriorityTaskDone;
    while (m_continueProcessing)
    {
        if (m_isStoped)
        {
            QThread::yieldCurrentThread();
            continue;
        }

        if (!m_modbus)
            m_modbus = new ModBusUART_Impl(m_uartName);

        highestPriorityTaskDone = true;
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
            if (pause > 0)
                QThread::msleep(pause);
        }
        else
        {
            m_isStoped = true;
        }
    }
    
    if (m_modbus)
        delete m_modbus;

    m_endProcessingSemaphore.release(1);
}

void ModbusPuller::startPulling(const QString& uartName)
{
    m_uartName = uartName;
    m_isStoped = false;
}

void ModbusPuller::stopPulling()
{
    m_continueProcessing = false;
    if (false == m_isStoped)
        m_endProcessingSemaphore.acquire(0);
}

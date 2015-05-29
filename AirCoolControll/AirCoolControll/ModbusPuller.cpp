#include "ModbusPuller.h"

ModbusPuller::ModbusPuller(QObject *parent)
    : QThread(parent),
    m_modbus(NULL),
    m_isStoped(true),
    m_continueProcessing(true)
{

}

ModbusPuller::~ModbusPuller()
{
    m_continueProcessing = false;
    if(false == m_isStoped)
        m_endProcessingSemaphore.acquire();
}

void ModbusPuller::clearTaskList()
{
    QMutexLocker lock(&m_taskMutex);
    m_tasks.clear();
}

void ModbusPuller::removeTaskWithID(int id)
{
    bool finished = true;
    while (finished && m_tasks.size())
    {
        finished = false;
        for (QList<PullerTaskShared>::iterator it = m_tasks.begin(); it != m_tasks.end(); it++)
        {
            if (id == (*it)->getID())
            {
                QMutexLocker lock(&m_taskMutex);
                m_tasks.erase(it);
                finished = true;
                break;
            }
        }
    }
}

void ModbusPuller::addTask(PullerTaskShared a_task)
{
    QMutexLocker lock(&m_taskMutex);
    m_tasks.append(a_task);
}

void ModbusPuller::run(void)
{
    int currentScanningID = 1;
    QString vendor, product, version;
    while (m_continueProcessing)
    {
        if (m_isStoped)
            QThread::yieldCurrentThread();

        {
            QMutexLocker lock(&m_taskMutex);
            for (QList<PullerTaskShared>::iterator task = m_tasks.begin(); task != m_tasks.end(); )
            {
                if (true == (*task)->proceed(m_modbus))
                {
                    task = m_tasks.erase(task);
                }
                else
                {
                    task++;
                }
            }
        }
    }
    m_endProcessingSemaphore.release();
}

void ModbusPuller::startPulling(ModBusUART_ImplShared modbus)
{
    m_modbus = modbus;
    m_isStoped = false;
}

void ModbusPuller::stopPulling()
{
    m_isStoped = true;
}

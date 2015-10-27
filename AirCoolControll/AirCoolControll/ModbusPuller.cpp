#include "ModbusPuller.h"
#include <qwaitcondition.h>

ModbusPuller::ModbusPuller(QObject *parent)
    : m_modbus(nullptr)
    , m_isStoped(true)
{
}

ModbusPuller::~ModbusPuller()
{
    stopPulling();
}

void ModbusPuller::stopPulling()
{
    m_timer.stop();
    if (m_modbus)
    {
        delete m_modbus;
        m_modbus = nullptr;
    }
}

void ModbusPuller::clearTaskList()
{
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
    m_tasks.push_back(a_task);
    if (!m_modbusBusy)
        schedule();
}

void ModbusPuller::process(void)
{
    if (!m_modbus)
        return;

    for (PullerTaskBase::Priority p = PullerTaskBase::Priority::High; p < PullerTaskBase::Priority::Count; PullerTaskBase::NEXT(p))
    {
        m_currentTaskIndex = 0;
        for (QList<PullerTaskShared>::iterator currentTask = m_tasks.begin(); currentTask != m_tasks.end(); currentTask++, m_currentTaskIndex++)
        {
            if ((*currentTask)->priority() == p && (*currentTask)->isItTimeToDo())
            {
                m_modbusBusy = true;
                (*currentTask)->proceed(m_modbus);
                return;
            }
        }
    }
}

void ModbusPuller::taskFinished(bool remove)
{
    if (remove)
        m_tasks.removeAt(m_currentTaskIndex);
    m_modbusBusy = false;
    schedule();
}

void ModbusPuller::schedule()
{
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

void ModbusPuller::startPulling(const QString& uartName)
{
    m_modbusBusy = false;
    m_timer.setSingleShot(true);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(process()));

    m_modbus = new ModBusUART_Impl(uartName, this);
}
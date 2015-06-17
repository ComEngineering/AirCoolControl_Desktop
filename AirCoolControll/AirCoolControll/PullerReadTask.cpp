#include "PullerReadTask.h"
#include <QMutexLocker>
#include "modbusuart_impl.h"
#include "Configurator.h"
#include "DeviceExplorer.h"


PullerReadTask::PullerReadTask(int id,int speed,Interval& range) :
    PullerTaskBase(id,speed),
    m_range(range),
    m_mutex(new QMutex()),
    m_listener(NULL)
{
    m_pull.resize(range.second - range.first + 1);
}


PullerReadTask::~PullerReadTask()
{
    delete m_mutex;
}

bool PullerReadTask::isContentChanged()
{
    return m_isUpdated;
}

void PullerReadTask::getContent(QVector<quint16>& list)
{
    QMutexLocker lock(m_mutex);
    m_isUpdated = false;
    list = m_pull;
}

void PullerReadTask::setContent(const QVector<quint16>& list)
{
    if (list.size() == m_pull.size())
    {
        for (QVector<quint16>::const_iterator it = list.begin(), it2 = m_pull.begin(); it != list.end(); it++, it2++)
        {
            if (*it != *it2)
            {
                {
                    QMutexLocker lock(m_mutex);
                    m_isUpdated = true;
                    m_pull = list;
                }
                if (m_listener)
                    m_listener->somethingChanged();
                break;
            }
        }
    }
}

bool PullerReadTask::proceed(ModBusUART_Impl* modbus)
{
    QVector<quint16> res;
    if (modbus->readRegisterPool(getID(), getSpeed(), m_range.first, m_range.second - m_range.first + 1, res))
    {
        setContent(res);
        m_lastSuccessfullAttemptTime = boost::posix_time::microsec_clock::local_time();
        m_failCounter = 0;
    }
    else
    {
        m_failCounter++;
    }
    
    return false;
}

bool PullerReadTask::isItTimeToDo(void) const
{
    boost::posix_time::time_duration diff = boost::posix_time::microsec_clock::local_time() - m_lastSuccessfullAttemptTime;
    if (diff.total_milliseconds() > Configurator::getPullInterval()) 
        return true;
    return false;
}

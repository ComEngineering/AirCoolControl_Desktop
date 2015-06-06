#include "PullerReadCoilTask.h"
#include <QMutexLocker>


PullerReadCoilTask::PullerReadCoilTask(int id, Interval& range) :
    PullerTaskBase(id),
    m_range(range),
    m_mutex(new QMutex())
{
    m_pull.resize(range.second - range.first + 1);
}


PullerReadCoilTask::~PullerReadCoilTask()
{
    delete m_mutex;
}

bool PullerReadCoilTask::isContentChanged()
{
    return m_isUpdated;
}

void PullerReadCoilTask::getContent(QVector<bool>& list)
{
    QMutexLocker lock(m_mutex);
    m_isUpdated = false;
    list = m_pull;
}

void PullerReadCoilTask::setContent(const QVector<bool>& list)
{
    if (list.size() == m_pull.size())
    {
        QMutexLocker lock(m_mutex);
        m_isUpdated = true;
        m_pull = list;
    }
}

bool PullerReadCoilTask::proceed(ModBusUART_Impl* modbus)
{
    QVector<bool> res;
    if (modbus->readCoilPool(getID(), m_range.first, m_range.second - m_range.first + 1, res))
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

bool PullerReadCoilTask::isItTimeToDo(void) const
{
    boost::posix_time::time_duration diff = boost::posix_time::microsec_clock::local_time() - m_lastSuccessfullAttemptTime;
    if (diff.total_milliseconds() > 1000) /// TO DO take from settings
        return true;
    return false;
}
#include "PullerTask.h"
#include <QMutexLocker>


PullerTask::PullerTask(int id,Interval& range) :
    m_id(id),
    m_range(range),
    m_mutex(new QMutex())
{
    m_pull.resize(range.second - range.first + 1);
}


PullerTask::~PullerTask()
{
    delete m_mutex;
}

bool PullerTask::isContentChanged()
{
    return m_isUpdated;
}

void PullerTask::getContent(QVector<quint16>& list)
{
    QMutexLocker lock(m_mutex);
    m_isUpdated = false;
    list = m_pull;
}

void PullerTask::setContent(const QVector<quint16>& list)
{
    if (list.size() == m_pull.size())
    {
        QMutexLocker lock(m_mutex);
        m_isUpdated = true;
        m_pull = list;
    }
}

int  PullerTask::getID() const
{
    return m_id;
}

const Interval& PullerTask::getRange() const
{
    return m_range;
}

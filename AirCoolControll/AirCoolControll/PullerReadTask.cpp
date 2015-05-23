#include "PullerReadTask.h"
#include <QMutexLocker>


PullerReadTask::PullerReadTask(int id,Interval& range) :
    PullerTaskBase(id),
    m_range(range),
    m_mutex(new QMutex()),
	m_id(id)
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
        QMutexLocker lock(m_mutex);
        m_isUpdated = true;
        m_pull = list;
    }
}

const Interval& PullerReadTask::getRange() const
{
    return m_range;
}

bool PullerReadTask::proceed(ModBusUART_ImplShared modbus)
{
    QVector<quint16> res;
    if (modbus->readRegisterPool(m_id, m_range.first, m_range.second - m_range.first + 1, res))
    {
        setContent(res);
    }
    
    return false;
}

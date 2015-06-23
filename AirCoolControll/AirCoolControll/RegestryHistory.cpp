#include "RegestryHistory.h"
#include "Configurator.h"


RegestryHistory::RegestryHistory() :
    m_snapshortSize(-1),
    m_startTime(boost::posix_time::second_clock::local_time()),
    m_snapshorts(k_preallocSize)
{
}


RegestryHistory::~RegestryHistory()
{

}


void RegestryHistory::setSnapshortSize(int snapshortSize)
{
    m_snapshortSize = snapshortSize;
    m_storage.reserve(k_preallocSize * snapshortSize);
}


void RegestryHistory::addSnapshort(std::vector<quint16>& snapshort)
{
    assert(m_snapshortSize != -1);

    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_duration diff = boost::posix_time::second_clock::local_time() - m_startTime;
    boost::posix_time::seconds limit(Configurator::getHistoryLength());
    if (diff > limit)
    {
        shiftStorage(now - limit);
        diff = boost::posix_time::second_clock::local_time() - m_startTime;
    }
    m_snapshorts.push_back(SnapshortInfo(boost::posix_time::seconds(diff.seconds()), static_cast<int>(m_storage.size() )));
    assert(snapshort.size() == m_snapshortSize);
    m_storage.insert(m_storage.end(),snapshort.begin(), snapshort.end());
}


void RegestryHistory::shiftStorage(boost::posix_time::ptime fromTime)
{
    int i;
    for (i = 0; i < m_snapshorts.size(); i++)
    {
        if (m_startTime + m_snapshorts[i].m_timeFromStart > fromTime)
            break;
    }

    if (i == m_snapshorts.size())
    {
        m_startTime = boost::posix_time::second_clock::local_time();
        m_snapshorts.clear();
    }
    m_snapshorts.erase(m_snapshorts.begin(), m_snapshorts.begin() + i);
    m_storage.erase(m_storage.begin(), m_storage.begin() + i*m_snapshortSize);

    boost::posix_time::seconds offset = m_snapshorts[0].m_timeFromStart;
    m_startTime += offset;
    for (auto it : m_snapshorts)
    {
        it.m_timeFromStart -= offset;
        it.m_offset -= i * m_snapshortSize;
    }
}
#include "RegestryHistory.h"
#include "Configurator.h"
#include <algorithm>
#include <QMutexLocker>


RegestryHistory::RegestryHistory() :
    m_snapshortSize(-1),
    m_startTime(boost::posix_time::second_clock::local_time())
{
}


RegestryHistory::~RegestryHistory()
{

}


void RegestryHistory::setSnapshortSize(int snapshortSize)
{
    m_snapshortSize = snapshortSize;
    m_all_history.resize(snapshortSize);
}


void RegestryHistory::addSnapshort(std::vector<quint16>& snapshort)
{
    assert(m_snapshortSize != -1);
    assert(snapshort.size() == m_snapshortSize);

    boost::posix_time::time_duration diff = boost::posix_time::second_clock::local_time() - m_startTime;
    boost::posix_time::time_duration limit = boost::posix_time::seconds(Configurator::getHistoryLength());//boost::posix_time::seconds (100);// 

    QMutexLocker locker(&m_accessMutex);

    for (int i = 0; i < snapshort.size(); i++)
    {
        if (m_all_history[i].size() == 0)
        {
            m_all_history[i].push_back(SnapshortInfo(diff, snapshort[i]));
        }
        else 
        {
            if (((m_all_history[i].end() - 1)->m_timeFromStart == diff) ||
                (m_all_history[i].size() > 1 && ((m_all_history[i].end() - 1)->m_value == snapshort[i]) && ((m_all_history[i].end() - 2)->m_value == snapshort[i])))
            {
                (m_all_history[i].end() - 1)->m_value = snapshort[i];
                (m_all_history[i].end() - 1)->m_timeFromStart = diff;
            }
            else
            {
                m_all_history[i].push_back(SnapshortInfo(diff, snapshort[i]));
            }
        }
    }
    
    if (diff > limit)
    {
        shiftStorage(diff - limit);
        m_startTime = boost::posix_time::second_clock::local_time();
    }
}


void RegestryHistory::shiftStorage(boost::posix_time::time_duration fromTime)
{
    for (auto a_history : m_all_history)
    {
        if (a_history.empty())
            continue;
        ValueHistory::iterator f = std::find_if(a_history.begin(), a_history.end(), [fromTime](const SnapshortInfo& a_value){return a_value.m_timeFromStart > fromTime; });
        assert(f != a_history.begin() || f != a_history.end());
        
        ValueHistory::iterator before = f - 1;
        quint16 valueDiff = f->m_value - before->m_value;
        long timeDiff = (f->m_timeFromStart - before->m_timeFromStart).seconds();
        float aspect = valueDiff / timeDiff;
        before->m_value += aspect * (fromTime - before->m_timeFromStart).seconds();
        a_history.erase(a_history.begin(), before);
        std::for_each(a_history.begin(), a_history.end(), [fromTime](SnapshortInfo& a_value){a_value.m_timeFromStart -= fromTime; });
    }
}

void RegestryHistory::getOneHistory(int n, QVector<qreal>& timeLabels, QVector<qreal>& values) const
{
    assert(n < m_all_history.size() && n >= 0);

    QMutexLocker locker(&m_accessMutex);

    time_t startTime = to_time_t(m_startTime);

    ValueHistory a_history = m_all_history[n];
    for (const auto& i : a_history)
    {
        double i_time = startTime + i.m_timeFromStart.total_seconds();
        timeLabels.push_back(i_time);
        values.push_back(i.m_value);
    }
    
    if (values.size())
    {
        timeLabels.push_back(to_time_t(boost::posix_time::second_clock::local_time()));
        values.push_back(*(values.end() - 1));
    }
}

time_t RegestryHistory::to_time_t(boost::posix_time::ptime t)
{
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
    boost::posix_time::time_duration::sec_type x = (t - epoch).total_seconds();

    return time_t(x);
}
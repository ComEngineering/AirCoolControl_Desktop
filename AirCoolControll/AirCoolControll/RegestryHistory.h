#ifndef __REGESTRYHISTORY__
#define __REGESTRYHISTORY__

#include <vector>
#include "boost/date_time/posix_time/posix_time.hpp" 
#include <qglobal.h>

class RegestryHistory
{
public:
    RegestryHistory();
    ~RegestryHistory();

    void setSnapshortSize(int snapshortSize);
    void addSnapshort(std::vector<quint16>& snapshort);

private:
    void shiftStorage(boost::posix_time::time_duration fromTime);

private:
    int              m_snapshortSize;

    struct SnapshortInfo
    {
        boost::posix_time::time_duration m_timeFromStart;
        quint16                    m_value;

        SnapshortInfo(boost::posix_time::seconds timeFromStart, quint16 value) :
            m_timeFromStart(timeFromStart),
            m_value(value) {}
        SnapshortInfo()  {}
    };

    typedef std::vector<SnapshortInfo> ValueHistory;
    
    boost::posix_time::ptime   m_startTime;
    std::vector<ValueHistory>  m_all_history;
};

#endif //__REGESTRYHISTORY__
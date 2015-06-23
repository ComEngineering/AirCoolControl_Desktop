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
    void shiftStorage(boost::posix_time::ptime fromTime);

private:
    static const int k_preallocSize = 1000; // TODO optimize
    int              m_snapshortSize;

    struct SnapshortInfo
    {
        boost::posix_time::seconds m_timeFromStart;
        int                        m_offset;
        SnapshortInfo(boost::posix_time::seconds timeFromStart, int offset) :
            m_timeFromStart(timeFromStart),
            m_offset(offset) {}
        SnapshortInfo() : m_timeFromStart(0){}
    };

    boost::posix_time::ptime   m_startTime;
    std::vector<SnapshortInfo> m_snapshorts;
    std::vector<quint16>       m_storage;
};

#endif //__REGESTRYHISTORY__
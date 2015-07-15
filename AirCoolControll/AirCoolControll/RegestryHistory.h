#ifndef __REGESTRYHISTORY__
#define __REGESTRYHISTORY__

#include <vector>
#include <map>
#include "boost/date_time/posix_time/posix_time.hpp" 
#include <qglobal.h>
#include <qmutex.h>
#include <qvector.h>

class RegestryHistory
{
public:
    RegestryHistory();
    ~RegestryHistory();

    struct SnapshortInfo
    {
        boost::posix_time::time_duration m_timeFromStart;
        qint16                          m_value;

        SnapshortInfo(boost::posix_time::time_duration timeFromStart, quint16 value) :
            m_timeFromStart(timeFromStart),
            m_value(value) {}
        SnapshortInfo()  {}
    };
    
    typedef std::vector<SnapshortInfo> ValueHistory;
    
    void addValue(const QString& name,qint16 value);
    void getOneHistory(const QString& name, QVector<qreal>& timeLabels, QVector<qreal>& values) ;

private:
    void shiftStorage(boost::posix_time::time_duration fromTime);
    static time_t to_time_t(boost::posix_time::ptime t);
  
private:
    mutable QMutex                  m_accessMutex;
    boost::posix_time::ptime        m_startTime;
    std::map<QString,ValueHistory>  m_all_history;
};

#endif //__REGESTRYHISTORY__
#ifndef __PullerReadTask__
#define __PullerReadTask__

#include  <QVector>
#include  <QMutex>
#include  <memory>
#include  "Interval.h"
#include  "PullerTaskBase.h"
#include  "boost/date_time/posix_time/posix_time.hpp" 

class PullerReadTask : public PullerTaskBase
{

public:
    PullerReadTask(int id, Interval& range);
    virtual ~PullerReadTask();

    bool proceed(ModBusUART_Impl* modbus);
    virtual bool isItTimeToDo(void) const;

    bool isContentChanged();
    void getContent(QVector<quint16>& list);
    void setContent(const QVector<quint16>& list);
    
    const Interval& getRange() const;

private:
    QVector<quint16>         m_pull;
    Interval                 m_range;
    bool                     m_isUpdated;
    mutable QMutex *         m_mutex;
    boost::posix_time::ptime m_lastSuccessfullAttemptTime;
};

typedef std::shared_ptr<PullerReadTask> PullerReadTaskShared;

#endif //__PullerReadTask__
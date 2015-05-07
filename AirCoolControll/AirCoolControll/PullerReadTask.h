#ifndef __PullerReadTask__
#define __PullerReadTask__

#include  <QVector>
#include  <QMutex>
#include  <memory>
#include  "Interval.h"
#include  "IPullerTask.h"

class PullerReadTask : public IPullerTask
{

public:
    PullerReadTask(int id, Interval& range);
    virtual ~PullerReadTask();

    bool proceed(ModBusUART_ImplShared modbus);
    int  getID() const;

    bool isContentChanged();
    void getContent(QVector<quint16>& list);
    void setContent(const QVector<quint16>& list);
    
    const Interval& getRange() const;

private:
    QVector<quint16>    m_pull;
    int                 m_id;
    Interval            m_range;
    bool                m_isUpdated;
    mutable QMutex *    m_mutex;
};

typedef std::shared_ptr<PullerReadTask> PullerReadTaskShared;

#endif //__PullerReadTask__
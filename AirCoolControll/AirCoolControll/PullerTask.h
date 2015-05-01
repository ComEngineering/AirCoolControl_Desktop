#ifndef __PULLERTASK__
#define __PULLERTASK__

#include  <QVector>
#include  <QMutex>
#include  <memory>
#include  "Interval.h"

class PullerTask 
{

public:
    PullerTask(int id, Interval& range);
    ~PullerTask();

    bool isContentChanged();
    void getContent(QVector<quint16>& list);
    void setContent(const QVector<quint16>& list);
    int  getID() const;
    const Interval& getRange() const;

private:
    QVector<quint16>    m_pull;
    int                 m_id;
    Interval            m_range;
    bool                m_isUpdated;
    mutable QMutex *    m_mutex;
};

typedef std::shared_ptr<PullerTask> PullerTaskShared;

#endif //__PULLERTASK__
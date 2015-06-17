#ifndef __PullerReadTask__
#define __PullerReadTask__

#include  <QVector>
#include  <QMutex>
#include  <memory>
#include  "Interval.h"
#include  "PullerTaskBase.h"

class DeviceExplorer;

class PullerReadTask : public PullerTaskBase
{

public:
    PullerReadTask(int id, int speed, Interval& range);
    virtual ~PullerReadTask();

    bool proceed(ModBusUART_Impl* modbus);
    virtual bool isItTimeToDo(void) const;

    bool isContentChanged();
    void getContent(QVector<quint16>& list);
    void setContent(const QVector<quint16>& list);

    void setListener(DeviceExplorer* listener) { m_listener = listener; }

protected:
    Interval                 m_range;
private:
    QVector<quint16>         m_pull;
    bool                     m_isUpdated;
    mutable QMutex *         m_mutex;
    DeviceExplorer *         m_listener;
};

typedef std::shared_ptr<PullerReadTask> PullerReadTaskShared;

#endif //__PullerReadTask__
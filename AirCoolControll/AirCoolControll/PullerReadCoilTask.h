#ifndef __PullerReadCoilTask__
#define __PullerReadCoilTask__

#include "PullerTaskBase.h"
#include "Interval.h"

class PullerReadCoilTask :
    public PullerTaskBase
{
public:
    PullerReadCoilTask(int id, Interval& range);
    ~PullerReadCoilTask();

    bool proceed(ModBusUART_Impl* modbus);
    virtual bool isItTimeToDo(void) const;

    bool isContentChanged();
    void getContent(QVector<bool>& list);
    void setContent(const QVector<bool>& list);

private:
    QVector<bool>            m_pull;
    Interval                 m_range;
    bool                     m_isUpdated;
    mutable QMutex *         m_mutex;
};

#endif //__PullerReadCoilTask__
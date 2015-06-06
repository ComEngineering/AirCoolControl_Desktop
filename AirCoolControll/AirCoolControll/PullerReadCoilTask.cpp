#include "PullerReadCoilTask.h"
#include "modbusuart_impl.h"


PullerReadCoilTask::PullerReadCoilTask(int id, Interval& range) :
    PullerReadTask(id,range)
{
}


PullerReadCoilTask::~PullerReadCoilTask()
{

}

bool PullerReadCoilTask::proceed(ModBusUART_Impl* modbus)
{
    QVector<quint16> res;
    if (modbus->readCoilPool(getID(), m_range.first, m_range.second - m_range.first + 1, res))
    {
        setContent(res);
        m_lastSuccessfullAttemptTime = boost::posix_time::microsec_clock::local_time();
        m_failCounter = 0;
    }
    else
    {
        m_failCounter++;
    }

    return false;
}


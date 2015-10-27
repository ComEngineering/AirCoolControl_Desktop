#include "PullerReadCoilTask.h"
#include "modbusuart_impl.h"


PullerReadCoilTask::PullerReadCoilTask(int id, int speed, Interval& range) :
    PullerReadTask(id,speed,range)
{
}


PullerReadCoilTask::~PullerReadCoilTask()
{

}

void PullerReadCoilTask::proceed(ModBusUART_Impl* modbus)
{
    modbus->readCoilPool(this, getID(), getSpeed(), m_range.first, m_range.second - m_range.first + 1);
}
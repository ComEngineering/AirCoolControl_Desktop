#include "PullerWriteTask.h"


PullerWriteTask::PullerWriteTask(int id, int regNumber, int value) :
    PullerTaskBase(id),
    m_regNumber(regNumber),
    m_value(value),
    m_tryCounter(0)
{
}


PullerWriteTask::~PullerWriteTask()
{
}


bool PullerWriteTask::proceed(ModBusUART_ImplShared modbus)
{
    bool rc = true;

    if (!modbus->writeRegister(getID(), m_regNumber, m_value))
    {
        if (m_tryCounter++ < 3)  /// TO DO read from settings
            rc = false;
    }

    return rc;
}
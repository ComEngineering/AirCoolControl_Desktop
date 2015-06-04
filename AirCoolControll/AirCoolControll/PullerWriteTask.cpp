#include "PullerWriteTask.h"


PullerWriteTask::PullerWriteTask(int id, int regNumber, int value) :
    PullerTaskBase(id),
    m_regNumber(regNumber),
    m_value(value)
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
        if (m_failCounter++ < 3)  /// TO DO read from settings
            rc = false;
       /* else
            emit transmitionError();*/
    }

    return rc;
}
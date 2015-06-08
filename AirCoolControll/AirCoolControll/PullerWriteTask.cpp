#include "PullerWriteTask.h"
#include "modbusuart_impl.h"
#include "Configurator.h"


PullerWriteTask::PullerWriteTask(int id, int regNumber, int value) :
    PullerTaskBase(id),
    m_regNumber(regNumber),
    m_value(value)
{
}


PullerWriteTask::~PullerWriteTask()
{
}


bool PullerWriteTask::proceed(ModBusUART_Impl* modbus)
{
    bool rc = true;

    if (!modbus->writeRegister(getID(), m_regNumber, m_value))
    {
        if (m_failCounter++ <  Configurator::getRetryCount())  
            rc = false;
       /* else
            emit transmitionError();*/
    }

    return rc;
}
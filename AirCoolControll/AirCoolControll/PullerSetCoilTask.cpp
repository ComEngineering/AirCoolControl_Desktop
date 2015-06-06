#include "PullerSetCoilTask.h"
#include "modbusuart_impl.h"

PullerSetCoilTask::PullerSetCoilTask(int id, int regNumber, bool value) :
    PullerTaskBase(id),
    m_regNumber(regNumber),
    m_value(value)
{
}


PullerSetCoilTask::~PullerSetCoilTask()
{
}


bool PullerSetCoilTask::proceed(ModBusUART_Impl* modbus)
{
    bool rc = true;

    if (!modbus->writeCoil(getID(), m_regNumber, m_value))
    {
        if (m_failCounter++ < 3)  /// TO DO read from settings
            rc = false;
        /* else
        emit transmitionError();*/ // TO DO fire callback
    }

    return rc;
}
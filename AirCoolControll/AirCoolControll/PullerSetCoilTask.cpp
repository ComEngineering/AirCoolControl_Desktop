#include "PullerSetCoilTask.h"
#include "modbusuart_impl.h"
#include "Configurator.h"

PullerSetCoilTask::PullerSetCoilTask(int id, int speed, int regNumber, bool value) :
    PullerTaskBase(id,speed),
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

    if (!modbus->writeCoil(getID(), getSpeed(), m_regNumber, m_value))
    {
        if (m_failCounter++ < Configurator::getRetryCount())  /// TO DO read from settings
            rc = false;
        /* else
        emit transmitionError();*/ // TO DO fire callback
    }

    return rc;
}
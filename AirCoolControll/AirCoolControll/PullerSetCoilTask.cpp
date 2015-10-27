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


void PullerSetCoilTask::proceed(ModBusUART_Impl* modbus)
{
    modbus->writeCoil(nullptr, getID(), getSpeed(), m_regNumber, m_value);
}

void PullerSetCoilTask::succesCall(bool rc)
{

}
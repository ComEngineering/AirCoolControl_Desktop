#include "PullerWriteTask.h"
#include "modbusuart_impl.h"
#include "Configurator.h"


PullerWriteTask::PullerWriteTask(int id, int speed, int regNumber, int value) :
    PullerTaskBase(id,speed),
    m_regNumber(regNumber),
    m_value(value)
{
}


PullerWriteTask::~PullerWriteTask()
{
}


void PullerWriteTask::proceed(ModBusUART_Impl* modbus)
{
    modbus->writeRegister(this, getID(), getSpeed(), m_regNumber, m_value);
}

void PullerWriteTask::succesCall(bool result)
{
    //if (m_failCounter++ <  Configurator::getRetryCount())

}
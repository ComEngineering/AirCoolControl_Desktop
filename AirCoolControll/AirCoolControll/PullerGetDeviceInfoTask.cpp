#include "PullerGetDeviceInfoTask.h"
#include "Configurator.h"
#include "ModbusDriver.h"


PullerGetDeviceInfoTask::PullerGetDeviceInfoTask(int id, int speed, const QString& uartName, ModbusDriver::detectionCallback cb) :
    PullerTaskBase(id, speed),
    m_uartName(uartName),
    m_cb(cb)
{
}

PullerGetDeviceInfoTask::~PullerGetDeviceInfoTask()
{
}

void PullerGetDeviceInfoTask::proceed(ModBusUART_Impl* modbus)
{
    modbus->readDeviceInfo(this, getID(), getSpeed());
}

void PullerGetDeviceInfoTask::succesCall(const QString& vendor, const QString& product, const QString& version)
{
    m_cb(std::make_shared<DeviceInfo>(m_uartName, getID(), getSpeed(), vendor, product, version));
}

bool PullerGetDeviceInfoTask::failCall()
{
    if (++m_failCounter >= Configurator::getRetryCount())
    {
        m_cb(std::make_shared<DeviceInfo>(m_uartName, getID()));
        return true;
    }

    return false;
}
#include "PullerGetDeviceInfoTask.h"
#include "Configurator.h"


PullerGetDeviceInfoTask::PullerGetDeviceInfoTask(int id, int speed, const QString& uartName, ModbusDriver::detectionCallback cb) :
    PullerTaskBase(id,speed),
    m_uartName(uartName),
    m_cb(cb)
{
}


PullerGetDeviceInfoTask::~PullerGetDeviceInfoTask()
{
}

bool PullerGetDeviceInfoTask::proceed(ModBusUART_Impl* modbus)
{
    bool rc = true;

    QString vendor,product,version;

    if ( ! modbus->readDeviceInfo(getID(), getSpeed(), vendor, product, version))
    {
        if (++m_failCounter < Configurator::getRetryCount()) 
            rc = false;
        else
            m_cb(std::make_shared<DeviceInfo>(m_uartName, getID()));
    }
    else
    {
        m_cb(std::make_shared<DeviceInfo>(m_uartName, getID(), getSpeed(), vendor, product, version));
    }

    return rc;
}
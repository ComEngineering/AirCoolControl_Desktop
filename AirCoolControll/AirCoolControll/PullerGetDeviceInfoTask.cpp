#include "PullerGetDeviceInfoTask.h"


PullerGetDeviceInfoTask::PullerGetDeviceInfoTask(int id):
    PullerTaskBase(id),
    m_tryCounter(0)
{
}


PullerGetDeviceInfoTask::~PullerGetDeviceInfoTask()
{
}

bool PullerGetDeviceInfoTask::proceed(ModBusUART_ImplShared modbus)
{
    bool rc = true;

    if (!modbus->readDeviceInfo(getID(), m_vendor, m_product, m_version))
    {
        if (m_tryCounter++ < 3)  /// TO DO read from settings
            rc = false;
        else
            emit deviceNotFound();
    }
    else
    {
        emit deviceDetected();
    }

    return rc;
}
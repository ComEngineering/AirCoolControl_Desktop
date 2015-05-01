#include "ModbusRegisterPuller.h"

ModbusRegisterPuller::ModbusRegisterPuller(QObject *parent)
    : QThread(parent),
    m_modbus(NULL),
    m_isStoped(true),
    m_continueProcessing(true)
{

}

ModbusRegisterPuller::~ModbusRegisterPuller()
{
    m_continueProcessing = false;
    m_endProcessingSemaphore.acquire();
    //m_deviceInfoList.clear();
}

void ModbusRegisterPuller::clearTaskList()
{
    m_tasks.clear();
}

void ModbusRegisterPuller::removeTaskWithID(int id)
{
    bool finished = true;
    while (finished && m_tasks.size())
    {
        finished = false;
        for (QList<PullerTaskShared>::iterator it = m_tasks.begin(); it != m_tasks.end(); it++)
        {
            if (id == (*it)->getID())
            {
                m_tasks.erase(it);
                finished = true;
                break;
            }
        }
    }
}

void ModbusRegisterPuller::addTask(PullerTaskShared a_task)
{
    QMutexLocker lock(&m_taskMutex);
    m_tasks.append(a_task);
}

void ModbusRegisterPuller::run(void)
{
    int currentScanningID = 1;
    QString vendor, product, version;
    while (m_continueProcessing)
    {
        if (m_isStoped)
            QThread::yieldCurrentThread();

        DeviceInfoMap::iterator ci = m_deviceInfoList.find(currentScanningID);
        if (m_modbus->readDeviceInfo(currentScanningID, vendor, product, version))
        {
            if (! (ci != m_deviceInfoList.end() && (vendor == ci->second->m_vendor || product == ci->second->m_product || ci->second->m_version == version)))
            {
                DeviceInfoShared a_info = std::make_shared<DeviceInfo>(vendor, product, version);
                {
                    QMutexLocker lock(&m_infoMapMutex);
                    m_deviceInfoList[currentScanningID] = a_info;
                }
                emit deviceListUpdated();
            }
        }
        else
        {
            if (ci != m_deviceInfoList.end())
            {
                //if (!m_modbus->readDeviceInfo(currentScanningID, vendor, product, version))
                {
                    QMutexLocker lock(&m_infoMapMutex);
                    m_deviceInfoList.erase(currentScanningID);
                }
                emit deviceListUpdated();
            }
        }

        if (++currentScanningID > 10)
            currentScanningID = 1;

        {
            QMutexLocker lock(&m_taskMutex);
            for (PullerTaskShared task : m_tasks)
            {
                QVector<quint16> res;
                if (m_modbus->readRegisterPool(task->getID(), task->getRange().first, task->getRange().second - task->getRange().first + 1, res))
                {
                    task->setContent(res);
                }
            }
        }
    }
    m_endProcessingSemaphore.release();
}

void ModbusRegisterPuller::startPulling(ModBusUART_ImplShared modbus)
{
    m_modbus = modbus;
    m_isStoped = false;
}

void ModbusRegisterPuller::stopPulling()
{
    m_isStoped = true;
}


void ModbusRegisterPuller::getDeviceInfoList(DeviceInfoMap& to_map) const
{
    QMutexLocker lock(&m_infoMapMutex);
    to_map = m_deviceInfoList;
}
#include "ConnectedDeviceStorage.h"

ConnectedDeviceStorage::ConnectedDeviceStorage(UART_DeviceStorage& drivers, QObject *parent)
    : QObject(parent),
    m_configs(NULL),
    m_drivers(drivers),
    m_currentIndex(-1)
{

}

ConnectedDeviceStorage::~ConnectedDeviceStorage()
{

}

bool ConnectedDeviceStorage::addDevice(DeviceInfoShared a_info)
{
    ModbusDriverShared driver = m_drivers.getDriver(a_info->getUART());
    if (NULL == driver)
        return false;
    
    ConfigMapShared  currentMap;
    bool rc = false;
    
    for (std::shared_ptr<ConfigMap> a_map : *m_configs)
    {
        if (a_map->isSupport(a_info))
        {
            currentMap = a_map;
            rc = true;
            break;
        }
    }
    
    if (rc)
    {
        a_info->setExplorer(std::make_shared<DeviceExplorer>(currentMap, driver, a_info->getID()));
    }

    push_back(a_info);

    if (m_currentIndex == -1)
    {
        m_currentIndex = 0;
        emit activeChanged();
    }
    
    return true;
}

void ConnectedDeviceStorage::setActiveIndex(int index)
{
    if (index < size())
        m_currentIndex = index;
    else
        m_currentIndex = -1;

    emit activeChanged();
}

DeviceInfoShared ConnectedDeviceStorage::getActiveDevice(void)
{
    DeviceInfoShared rc;
    if (-1 != m_currentIndex)
    {
        int counter = m_currentIndex;
        for (std::list<DeviceInfoShared>::iterator it = begin(); it != end(); it++,counter--)
            if (0 == counter)
            {
                rc = *it;
                break;
            }
    }

    return rc;
}

void ConnectedDeviceStorage::removeDevicesWithUART(const QString& uartName)
{
    int counter = 0;
    int newCurrentIndex = m_currentIndex;
    for (std::list<DeviceInfoShared>::iterator it = begin(); it != end(); counter++)
    {
        QString deviceUART_Name = (*it)->getUART();
        if (deviceUART_Name == uartName)
        {
            it = erase(it);
            if (m_currentIndex == counter)
            {
                newCurrentIndex = 0;
            }
            else if (counter < m_currentIndex)
                newCurrentIndex--;
        }
        else it++;
    }

    if(size() == 0) 
        newCurrentIndex = -1;
    if (newCurrentIndex != m_currentIndex)
    {
        m_currentIndex = newCurrentIndex;
        emit activeChanged();
    }
}

void ConnectedDeviceStorage::removeDeviceFromList(int n)
{
    if (DISCONNECT_ALL == n)
    {
        clear();
        m_currentIndex = -1;
    }
    else
    {
        if (DISCONNECT_CURRENT == n)
            n = m_currentIndex;
        if (n >= size())
            return;
        int counter = 0;
        for (std::list<DeviceInfoShared>::iterator it = begin(); it != end(); it++)
            if (counter++ == n)
            {
                erase(it);
                break;
            }

        if (m_currentIndex == n)
            m_currentIndex = size() == 0 ? -1 : 0;
        else if (m_currentIndex > n)
            m_currentIndex--;
    }

    emit activeChanged();
}
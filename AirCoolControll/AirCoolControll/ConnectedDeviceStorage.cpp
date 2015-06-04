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
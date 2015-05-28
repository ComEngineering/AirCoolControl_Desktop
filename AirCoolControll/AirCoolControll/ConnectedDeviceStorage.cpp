#include "ConnectedDeviceStorage.h"

ConnectedDeviceStorage::ConnectedDeviceStorage(UART_DeviceStorage& drivers, QObject *parent)
    : QObject(parent),
    m_configs(NULL),
    m_drivers(drivers)
{

}

ConnectedDeviceStorage::~ConnectedDeviceStorage()
{

}

bool ConnectedDeviceStorage::addDevice(DeviceInfoShared a_info)
{
    ModbusDriverShared driver = m_drivers.getDriverWithName(a_info->m_uart);
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
        a_info->m_explorer = std::make_shared<DeviceExplorer>(currentMap, driver, a_info->m_id);
    }

    push_back(a_info);
    
    return true;
}
#include "ConnectedDeviceStorage.h"
#include "UART_DeviceStorage.h"

ConnectedDeviceStorage::ConnectedDeviceStorage(UART_DeviceStorage& drivers, QObject *parent)
    : QObject(parent),
    m_configs(NULL),
    m_drivers(drivers),
    m_mdiArea(NULL)
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
        a_info->setExplorer(std::make_shared<DeviceExplorer>(currentMap, driver, a_info));
    }

    push_back(a_info);

    return true;
}

void ConnectedDeviceStorage::removeDevicesWithUART(const QString& uartName)
{
    int counter = 0;

    for (std::list<DeviceInfoShared>::iterator it = begin(); it != end(); counter++)
    {
        QString deviceUART_Name = (*it)->getUART();
        if (deviceUART_Name == uartName)
        {
            it = erase(it);
        }
        else it++;
    }
}

void ConnectedDeviceStorage::removeDeviceFromList(int n)
{
    if (DISCONNECT_ALL == n)
    {
        clear();
    }
    else
    {
        if (n >= size())
            return;
        int counter = 0;
        for (std::list<DeviceInfoShared>::iterator it = begin(); it != end(); it++)
            if (counter++ == n)
            {
                erase(it);
                break;
            }
    }
}

void ConnectedDeviceStorage::updateDeviceTick()
{
    for (auto it = begin(); it != end(); it++)
        (*it)->getExplorer()->updateStateWidget();
}

void ConnectedDeviceStorage::setActiveIndex(int n)
{
    if (n >= size() || NULL == m_mdiArea)
        return;

    int counter = 0;
    for (std::list<DeviceInfoShared>::iterator it = begin(); it != end(); it++)
        if (counter++ == n)
        {
            (*it)->getExplorer()->activateView(m_mdiArea);
            break;
        }
}
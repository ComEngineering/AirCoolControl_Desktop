#include "ConnectedDeviceStorage.h"
#include "UART_DeviceStorage.h"
#include <algorithm>

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
        DeviceExplorerShared d = std::make_shared<DeviceExplorer>(currentMap, driver, a_info);
        d->setListView(m_listView);
        a_info->setExplorer(d);
    }

    push_back(a_info);
    setActiveIndex(size() - 1);

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

bool ConnectedDeviceStorage::setActiveIndex(int n)
{
    bool rc = false;
    
    if (n < size() && NULL != m_mdiArea)
    {
        int counter = 0;
        for (std::list<DeviceInfoShared>::iterator it = begin(); it != end(); it++)
            if (counter++ == n)
            {
                DeviceExplorerShared a_device = (*it)->getExplorer();
                if (a_device)
                {
                    a_device->activateView(m_mdiArea);
                    rc = true;
                }
                break;
            }
    }
    return rc;
}

void ConnectedDeviceStorage::getDevicesConnectedToDriver(const QString& name, std::vector<QString>& vector) const
{
    for (auto i = begin(); i != end(); i++)
        if (name == (*i)->getUART())
            vector.push_back((*i)->getDescription());
}

int  ConnectedDeviceStorage::findDeviceIndex(const DeviceExplorer* device) const
{
    int i = 0;
    int rc = -1;
    for (auto it : *this)
    {
        if (it->getExplorer().get() == device)
        {
            rc = i;
            break;
        }
        i++;
    }
    return rc;
}
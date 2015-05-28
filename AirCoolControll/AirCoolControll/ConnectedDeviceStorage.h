#ifndef CONNECTEDDEVICESTORAGE_H
#define CONNECTEDDEVICESTORAGE_H

#include <QObject>
#include "ConfigMap.h"
#include <list>
#include "DeviceInfo.h"
#include "DeviceExplorer.h"
#include "UART_DeviceStorage.h"


class ConnectedDeviceStorage : public QObject, public std::list<DeviceInfoShared>
{
    Q_OBJECT

public:
    ConnectedDeviceStorage(UART_DeviceStorage& drivers,QObject *parent);
    ~ConnectedDeviceStorage();

    void setConfigList(const ConfigList * configs) { m_configs = configs; }
    bool addDevice(DeviceInfoShared a_info);

private:
    const ConfigList *  m_configs;
    UART_DeviceStorage& m_drivers;
};

#endif // CONNECTEDDEVICESTORAGE_H

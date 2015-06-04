#ifndef CONNECTEDDEVICESTORAGE_H
#define CONNECTEDDEVICESTORAGE_H

#include <QObject>
#include <list>
#include <QString>
#include "ConfigMap.h"

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

    void removeDeviceWithUART(const QString& uartName);

    void setActiveIndex(int index);
    int  getActiveIndex(void) const{ return m_currentIndex; }
    
    DeviceInfoShared getActiveDevice(void);

signals:
    void activeChanged();

private:
    int                 m_currentIndex;
    const ConfigList *  m_configs;
    UART_DeviceStorage& m_drivers;
};

#endif // CONNECTEDDEVICESTORAGE_H

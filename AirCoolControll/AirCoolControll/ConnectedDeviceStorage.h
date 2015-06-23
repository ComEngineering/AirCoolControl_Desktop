#ifndef CONNECTEDDEVICESTORAGE_H
#define CONNECTEDDEVICESTORAGE_H

#include <QObject>
#include <list>
#include <QString>
#include <qmdiarea.h>

#include "DeviceInfo.h"
#include "DeviceExplorer.h"

class UART_DeviceStorage;

class ConnectedDeviceStorage : public QObject, public std::list<DeviceInfoShared>
{
    Q_OBJECT

public:
    ConnectedDeviceStorage(UART_DeviceStorage& drivers,QObject *parent);
    ~ConnectedDeviceStorage();

    void setConfigList(const ConfigList * configs) { m_configs = configs; }
    bool addDevice(DeviceInfoShared a_info);
    void getDevicesConnectedToDriver(const QString& name,std::vector<QString>& vector) const;

    void setMdiArea(QMdiArea * area) { m_mdiArea = area; }

    void removeDeviceFromList(int n);

    void updateDeviceTick();

    bool setActiveIndex(int n);

    static const int DISCONNECT_ALL = -1;
    static const int DISCONNECT_CURRENT = -2;

public slots:
    void removeDevicesWithUART(const QString& uartName);

signals:
    void activeChanged();

private:
    const ConfigList *  m_configs;
    UART_DeviceStorage& m_drivers;
    QMdiArea*           m_mdiArea;
};

#endif // CONNECTEDDEVICESTORAGE_H

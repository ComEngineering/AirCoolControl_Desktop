#ifndef CONNECTEDDEVICESTORAGE_H
#define CONNECTEDDEVICESTORAGE_H

#include <QObject>
#include <list>
#include <QString>
#include <qmdiarea.h>

#include "DeviceInfo.h"
#include "DeviceExplorer.h"
#include "ConfigStorage.h"

class UART_DeviceStorage;
class ConnectionLog;

class ConnectedDeviceStorage : public QObject, public std::list<DeviceInfoShared>
{
    Q_OBJECT

public:
    ConnectedDeviceStorage(UART_DeviceStorage& drivers,QObject *parent);
    ~ConnectedDeviceStorage();

    void setConfigList(const ConfigStorage * configs) { m_configs = configs; }
    bool addDevice(DeviceInfoShared a_info);
    void getDevicesConnectedToDriver(const QString& name,std::vector<QString>& vector) const;

    void setMdiArea(QMdiArea * area) { m_mdiArea = area; }
    void setListView(ConnectionLog* view) { m_listView = view; }

    void removeDeviceFromList(int n);
    int  findDeviceIndex(const DeviceExplorer* device) const;

    bool setActiveIndex(int n);

    static const int DISCONNECT_ALL = -1;
    static const int DISCONNECT_CURRENT = -2;

public slots:
    void removeDevicesWithUART(const QString& uartName);

signals:
    void activeChanged();

private:
    const ConfigStorage*m_configs;
    UART_DeviceStorage& m_drivers;
    QMdiArea*           m_mdiArea;
    ConnectionLog*      m_listView;
};

#endif // CONNECTEDDEVICESTORAGE_H

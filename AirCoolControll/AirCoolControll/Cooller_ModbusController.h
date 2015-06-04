#ifndef __Cooller_ModBusController__
#define __Cooller_ModBusController__

#include "coolerstatewidget.h"
#include "modbusdialog.h"
#include "externalcontrollmanager.h"
#include <QtSerialPort\qserialportinfo.h>
#include <QtSerialPort\qserialport.h>
#include <qtimer.h>
#include <memory>
#include "DeviceExplorer.h"
#include "ModbusDriver.h"
#include "UART_DeviceStorage.h"
#include "ConnectedDeviceStorage.h"

class Cooller_ModBusController : public QObject
{
    Q_OBJECT
public:
    Cooller_ModBusController(CoolerStateWidget *view, ModBusDialog *config);
    ~Cooller_ModBusController();

private:
    void checkConnectionState(void);
    static bool equalPredicat(QSerialPortInfo& first,QSerialPortInfo& second);
    bool readXMLConfig(const QString& path);
    void updateStateWidget(void);
    void allertError(QString errorDescription);

private slots:
    void updateState(void);
    void newDevice(int);
    void sendConfiguration(void);
    void externalStateChanged(void);
    void externalListChanged(void);
    void addDevice(DeviceInfoShared info);
    void setActiveDevice(void);

signals:
    void newStatus(const QString&);

private:
    CoolerStateWidget *     m_view;
    ModBusDialog *          m_configDialog;

    UART_DeviceStorage      m_info;
    QTimer   *              m_recheckTimer;
    bool                    m_available;
    ExternalConnector       m_connector;
    ExternalControllManager m_externalManager;
    ConfigList              m_configs;
    
    ConnectedDeviceStorage  m_explorers;
    DeviceExplorerShared    m_currentDevice;
};

#endif // __Cooller_ModBusController__

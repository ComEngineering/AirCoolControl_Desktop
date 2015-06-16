#ifndef __Cooller_ModBusController__
#define __Cooller_ModBusController__


#include <QtSerialPort\qserialportinfo.h>
#include <QtSerialPort\qserialport.h>
#include <qtimer.h>
#include <memory>

#include "externalcontrollmanager.h"
#include "externalconnector.h"
#include "DeviceExplorer.h"
#include "ModbusDriver.h"
#include "UART_DeviceStorage.h"
#include "ConnectedDeviceStorage.h"

class AirCoolControll;

class Cooller_ModBusController : public QObject
{
    Q_OBJECT
public:
    Cooller_ModBusController(AirCoolControll* mainWindow);
    ~Cooller_ModBusController();

    void performConnection(int uart_number, int deviceIndex, int speedIndex);

private:
    void checkConnectionState(void);
    static bool equalPredicat(QSerialPortInfo& first,QSerialPortInfo& second);
    bool readXMLConfig(const QString& path);
    void allertError(QString errorDescription);
    void newDevice();
    int  speedIndexToSpeed(int speedIndex);

private slots:
    void updateState(void);
    void sendConfiguration(void);
    void externalStateChanged(void);
    void externalListChanged(void);
    void addDevice(DeviceInfoShared info);

signals:
    void newStatus(const QString&);

private:
    AirCoolControll*        m_mainWindow;
    UART_DeviceStorage      m_info;
    QTimer   *              m_recheckTimer;
    bool                    m_available;
    ExternalConnector       m_connector;
    ExternalControllManager m_externalManager;
    ConfigList              m_configs;
    
    ConnectedDeviceStorage  m_explorers;
};

#endif // __Cooller_ModBusController__

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

private slots:
    void updateState(void);
    void newSpeed(int);
    void newPort(int);
    void newDevice(int);
    void sendConfiguration(void);
    void externalStateChanged(void);
    void externalListChanged(void);

signals:
    void newStatus(const QString&);

private:
    CoolerStateWidget *     m_view;
    ModBusDialog *          m_configDialog;
    QList<QSerialPortInfo>  m_info;
    QTimer   *              m_recheckTimer;
    bool                    m_available;
    int                     m_comunicationSpeedIndex;
    int                     m_currentDeviceID;
    ExternalConnector       m_connector;
    ExternalControllManager m_externalManager;
    ConfigList              m_configs;
    DeviceExplorerShared   m_explorer;
    ModbusDriver            m_modbus;

    ConfigMap::ParameterList m_inParameters;
    ConfigMap::ParameterList m_outParameters;
};

#endif // __Cooller_ModBusController__

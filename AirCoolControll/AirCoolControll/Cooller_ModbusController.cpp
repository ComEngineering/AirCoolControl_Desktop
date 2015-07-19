#include <QtSerialPort\qserialportinfo.h>
#include <qstring>
#include <qobject>
#include <algorithm>
#include <qmessagebox.h>
#include "Logger.h"
#include "Cooller_ModbusController.h"
#include "aircoolcontroll.h"

Cooller_ModBusController::Cooller_ModBusController(AirCoolControll* mainWindow) :
    m_mainWindow(mainWindow),
    m_recheckTimer(new QTimer(this)),
    m_available(false),
    m_externalManager(this),
    m_info(this),
    m_explorers(m_info,this),
    m_connector(this)
{
    connect(m_recheckTimer, SIGNAL(timeout()), this, SLOT(updateState()));
    m_recheckTimer->setInterval(500);
    m_recheckTimer->start();
//    config->setExternalCommunicator(&m_connector);

    //connect(&m_connector, SIGNAL(connectionEstablished()), config, SLOT(connectionEstablished()));
    //connect(&m_connector, SIGNAL(connectionEstablished()), this, SLOT(sendConfiguration()));
    //connect(&m_connector, SIGNAL(connectionBroken()), config, SLOT(connectionBroken()));
    //connect(&m_connector, SIGNAL(connectionErrorOccured(QString)), config, SLOT(connectionErrorOccured(QString)));
    //
    connect(&m_externalManager, SIGNAL(stateChanged()), this, SLOT(externalStateChanged()));
    connect(&m_externalManager, SIGNAL(listChanged()), this, SLOT(externalListChanged()));

    qRegisterMetaType<DeviceInfoShared>("DeviceInfoShared");
    qRegisterMetaType<QSerialPort::SerialPortError>("QSerialPort::SerialPortError");
    qRegisterMetaType<QVector<int>>("QVector<int>");

    connect(&m_info, SIGNAL(deviceDetected(DeviceInfoShared)), this, SLOT(addDevice(DeviceInfoShared)));
    
    connect(&m_info, SIGNAL(uartDisconnected(const QString&)), &m_explorers, SLOT(removeDevicesWithUART(const QString&)));

    m_explorers.setConfigList(&m_configs);
    
    m_mainWindow->getConnectionLog()->setDeviceList(&m_explorers);
    m_explorers.setMdiArea(mainWindow->getMdiArea());
    m_explorers.setListView(m_mainWindow->getConnectionLog());
}


Cooller_ModBusController::~Cooller_ModBusController()
{
    m_recheckTimer->stop();
    m_explorers.clear();
}

void Cooller_ModBusController::updateState(void)
{
    checkConnectionState();
}

void Cooller_ModBusController::allertError(QString errorDescription)
{
    QMessageBox allertBox(QMessageBox::Icon::Critical,tr("Connection error"),errorDescription,QMessageBox::StandardButton::Ok);
    allertBox.exec();
}

void Cooller_ModBusController::addDevice(DeviceInfoShared info)
{
    if (info->empty())
        allertError(tr("Device hasn't been found. At %1, id = %2").arg(info->getUART(), QString::number(info->getID())));
    else
    {
        m_explorers.addDevice(info);
    }
}

void Cooller_ModBusController::checkConnectionState(void)
{
    bool uart_list_changed = m_info.update(QSerialPortInfo::availablePorts());

    if (m_info.empty())
    {
        m_mainWindow->getUART_Configurator()->setError(QObject::tr("COM ports aren't available"), true);
        m_mainWindow->getUART_Configurator()->setCOMindex(-1);
        m_available = false;
        return;
    }
    
    if (uart_list_changed)
    {
        m_mainWindow->getUART_Configurator()->setCOMlist(m_info.getPortList());
        int n = m_mainWindow->getUART_Configurator()->getCOMindex();
        if (-1 != n)
        {
            QString currentPortName = m_info.portName(n);
            n = -1;
            for (int i = 0; i < m_info.size(); i++)
            {
                if (m_info.portName(i) == currentPortName)
                {
                    n = i;
                    break;
                }
            }
        }
       
        if (!m_available)
        {
            m_available = true;
            m_mainWindow->getUART_Configurator()->clearError();
        }
        
        n = (-1 != n) ? n : 0;
        m_mainWindow->getUART_Configurator()->setCOMindex(n);

        if (m_connector.isActive())
            sendConfiguration();
    }
}

int  Cooller_ModBusController::speedIndexToSpeed(int speedIndex)
{
    const static int convertStorage[] = {
        110,
        300,
        600,
        1200,
        2400,
        4800,
        9600,
        14400,
        19200,
        38400,
        56000,
        57600,
        115200,
        128000,
        256000
    };

    assert(speedIndex >= 0 && speedIndex < sizeof(convertStorage) / sizeof(int));
    
    return convertStorage[speedIndex]; 
}

void Cooller_ModBusController::performConnection(int uart_number, int deviceIndex, int speedIndex)
{
    if (-1 == deviceIndex)
        return;

    int speed = speedIndexToSpeed(speedIndex);

    ModbusDriverShared modbus = m_info.getDriver(uart_number);
    modbus->requestDeviceAproval(deviceIndex,speed);
}

void Cooller_ModBusController::sendConfiguration(void)
{
    QList<QString> ports;

    for (auto info : m_info.getPortList())
    {
        ports.push_back(info);
    }
    m_connector.sendPortList(ports);
}

void Cooller_ModBusController::externalStateChanged(void)
{
    if (m_externalManager.isActiveConnection())
    {
//        m_configDialog->setExternalConnection(m_externalManager.getExternalIP());
    }
    else
    {
//        m_configDialog->dropExternalConnection();
    }
}

void Cooller_ModBusController::externalListChanged(void)
{
    m_mainWindow->getUART_Configurator()->setExternalPorts(m_externalManager.getExternalPortsList());
}

std::vector<std::pair<QString, bool>> Cooller_ModBusController::getDriverList(void) const
{
    return m_info.getDriverList();
}

void Cooller_ModBusController::releaseDriverWithName(const QString& driverName)
{
    m_explorers.removeDevicesWithUART(driverName);
    m_info.releaseDriver(driverName);
    m_mainWindow->getConnectionLog()->updateContent();
}

void Cooller_ModBusController::getDevicesConnectedToDriver(const QString& name, std::vector<QString>& vector) const
{
    m_explorers.getDevicesConnectedToDriver(name, vector);
}
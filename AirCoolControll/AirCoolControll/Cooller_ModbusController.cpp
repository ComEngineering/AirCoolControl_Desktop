
#ifdef _WIN32
    #pragma warning(disable:4996)
    #define  _D_SCL_SECURE_NO_WARNINGS
#endif


#include <QtSerialPort\qserialportinfo.h>
#include <qstring>
#include <qobject>
#include <algorithm>
#include "Configurator.h"
#include <qdiriterator.h>
#include <qmainwindow.h>
#include <qmessagebox.h>

#include "modbusuart_impl.h"
#include "Logger.h"
#include "Cooller_ModbusController.h"
#include "ConfigMap.h"
#include "aircoolcontroll.h"

Cooller_ModBusController::Cooller_ModBusController(AirCoolControll* mainWindow) :
    m_mainWindow(mainWindow),
    m_recheckTimer(new QTimer(this)),
    m_available(false),
    //m_comunicationSpeedIndex(6), //9600
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
    connect(&m_info, SIGNAL(deviceDetected(DeviceInfoShared)), this, SLOT(addDevice(DeviceInfoShared)));
    connect(&m_explorers, SIGNAL(activeChanged(void)), this, SLOT(setActiveDevice(void)));

 //   connect(m_view, SIGNAL(newRegisterValue(int,QString&, int)), this, SLOT(sendValueToDevice(int,QString&, int)));

    connect(&m_info, SIGNAL(uartDisconnected(const QString&)), &m_explorers, SLOT(removeDevicesWithUART(const QString&)));

    QString configsPath = Configurator::getConfigFilesPath();
    QDirIterator iter(configsPath, QStringList() << "*.xml", QDir::Files | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    int configsRed = 0;
    while (iter.hasNext())
    {
        iter.next();
        QString xmlFilePath = iter.filePath();
        if (readXMLConfig(xmlFilePath))
            configsRed++;
    }
    if (0 == configsRed )
    {
        emit newStatus(tr("No config files were found"));
    }
    else
    {
        m_explorers.setConfigList(&m_configs);
    }

  //  m_configDialog->setDeviceList(&m_explorers);
}


Cooller_ModBusController::~Cooller_ModBusController()
{
    m_recheckTimer->stop();
    m_explorers.clear();
}

void Cooller_ModBusController::updateState(void)
{
    checkConnectionState();

    updateStateWidget();
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
        //m_configDialog->refreshDeviceList();
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

void Cooller_ModBusController::newDevice()
{
    int com_index = m_mainWindow->getUART_Configurator()->getCOMindex();
    if (-1 == com_index)
        return;

    ModbusDriverShared modbus = m_info.getDriver(com_index);
    if (modbus->readyToWork())
    {
        modbus->requestDeviceAproval(com_index);
    }
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

bool Cooller_ModBusController::readXMLConfig(const QString& path)
{
    boost::property_tree::ptree tree;
    try
    {
        boost::property_tree::read_xml(path.toStdString(), tree, boost::property_tree::xml_parser::no_comments);
        std::string vendor = tree.get<std::string>("Config.Vendor");
        std::string product = tree.get<std::string>("Config.Product");
        std::string versionMin = tree.get<std::string>("Config.Version.min");
        std::string versionMax = tree.get<std::string>("Config.Version.max");
        std::shared_ptr<ConfigMap> a_map = std::make_shared<ConfigMap>(vendor, product, versionMin, versionMax);

        std::string uiType = tree.get<std::string>("Config.UI.<xmlattr>.type", "none");
        if ("none" != uiType)
        {
            std::string ui_path = tree.get<std::string>("Config.UI");
            a_map->setUI_Config(uiType, ui_path);
        }

        boost::property_tree::ptree values[ConfigMap::REGISTER_PULL_COUNT] = { 
            tree.get_child("Config.InputValues", boost::property_tree::ptree()), 
            tree.get_child("Config.OutValues", boost::property_tree::ptree()),
            tree.get_child("Config.Coils",  boost::property_tree::ptree())
        };

        for (int i = 0; i < ConfigMap::REGISTER_PULL_COUNT; i++)
        {
            boost::property_tree::ptree& a_value = values[i];
            for (const std::pair<std::string, boost::property_tree::ptree> &p : a_value)
            {
                std::string name = p.first;
                ConfigMap::Parameter a_parameter;
                a_parameter.m_description = p.second.get_value<std::string>();
                a_parameter.m_registerNumber = p.second.get<int>("<xmlattr>.R");
                a_map->getInterval(i).add(a_parameter.m_registerNumber);
                a_parameter.m_type = (ConfigMap::RegisterType)i;
                int b = p.second.get<int>("<xmlattr>.B", -1);
                a_parameter.m_decodeMethod = p.second.get<std::string>("<xmlattr>.D", "");
                if (b != -1)
                {
                    a_parameter.m_isBool = true;
                    a_parameter.m_bitNumber = b;
                }
                else
                {
                    a_parameter.m_isBool = false;
                }
                a_map->addVariable(i,name, a_parameter);
            }
        }

        m_configs.push_back(a_map);
    }
    catch (boost::property_tree::xml_parser_error& err)
    {
        Logger::log(err.message(), Logger::Error);
        return false;
    }
    catch (boost::property_tree::ptree_bad_data& err)
    {
        Logger::log(err.what(), Logger::Error);
        return false;
    }

    return true;
}

void Cooller_ModBusController::updateStateWidget()
{
    if (m_currentDevice)
    {
        for (ConfigMap::RegisterType it = ConfigMap::REGISTER_PULL_FIRST; it < ConfigMap::REGISTER_PULL_COUNT; ConfigMap::NEXT(it))
        {
            const ConfigMap::ParameterList& parameters = m_currentDevice->getCurrentConfig()->getParametersList(it);
            
            for (int i = 0; i < parameters.size(); i++)
            {
                int value;
                if (m_currentDevice->getRegisterValue(parameters[i].first, value))
                {
//                    m_view->updateParameter(i, value, it);
                }
            }
        }
    }
}

void Cooller_ModBusController::setActiveDevice(void)
{
    //DeviceInfoShared info = m_explorers.getActiveDevice();
    //m_currentDevice = info ? info->getExplorer() : NULL;
    //if (m_currentDevice)
    //{
    //    for (ConfigMap::RegisterType i = ConfigMap::REGISTER_PULL_FIRST; i < ConfigMap::REGISTER_PULL_COUNT; ConfigMap::NEXT(i))
    //        m_view->setParameterList(m_currentDevice->getCurrentConfig()->getParametersList(i), i);
    //}
    //else
    //{
    //    m_view->clear();
    //}
    //m_configDialog->refreshDeviceList();
}

void Cooller_ModBusController::sendValueToDevice(int registerType,QString& name, int v)
{
    if (m_currentDevice)
    {
        switch (static_cast<ConfigMap::RegisterType>(registerType))
        {
        case ConfigMap::OUTPUT_REGISTER :
            m_currentDevice->setRegisterValue(name.toStdString(), v);
            break;
        case ConfigMap::COIL :
            m_currentDevice->setCoilState(name.toStdString(), static_cast<bool>(v));
            break;
        }
        
    }
}

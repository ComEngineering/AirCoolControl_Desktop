
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

Cooller_ModBusController::Cooller_ModBusController(CoolerStateWidget *view, ModBusDialog *config) :
    m_view(view), 
    m_configDialog(config),
    m_recheckTimer(new QTimer(this)),
    m_available(false),
    //m_comunicationSpeedIndex(6), //9600
    m_connector(this),
    m_externalManager(this),
    m_info(this),
    m_explorers(m_info,this)
{
    connect(m_recheckTimer, SIGNAL(timeout()), this, SLOT(updateState()));
    m_recheckTimer->setInterval(500);
    m_recheckTimer->start();
    config->setExternalCommunicator(&m_connector);

    connect(m_configDialog, SIGNAL(deviceIDChanged(int)), this, SLOT(newDevice(int)));

    connect(&m_connector, SIGNAL(connectionEstablished()), config, SLOT(connectionEstablished()));
    connect(&m_connector, SIGNAL(connectionEstablished()), this, SLOT(sendConfiguration()));
    connect(&m_connector, SIGNAL(connectionBroken()), config, SLOT(connectionBroken()));
    connect(&m_connector, SIGNAL(connectionErrorOccured(QString)), config, SLOT(connectionErrorOccured(QString)));
    
    connect(&m_externalManager, SIGNAL(stateChanged()), this, SLOT(externalStateChanged()));
    connect(&m_externalManager, SIGNAL(listChanged()), this, SLOT(externalListChanged()));

    qRegisterMetaType<DeviceInfoShared>("DeviceInfoShared");
    connect(&m_info, SIGNAL(deviceDetected(DeviceInfoShared)), this, SLOT(addDevice(DeviceInfoShared)));

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
}


Cooller_ModBusController::~Cooller_ModBusController()
{
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
        allertError(tr("Device's been found. At %1, id = %2").arg(info->m_uart, info->m_id));
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
        m_configDialog->setError(QObject::tr("COM ports aren't available"), true);
        m_configDialog->setCOMindex(-1);
        m_available = false;
        return;
    }
    
    if (uart_list_changed)
    {
        int n = m_configDialog->getCOMindex();
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
            m_configDialog->clearError();
        }

        m_configDialog->setCOMlist(m_info.getPortList());
        
        n = (-1 != n) ? n : 0;
        m_configDialog->setCOMindex(n);

        if (m_connector.isActive())
            sendConfiguration();
    }
}

void Cooller_ModBusController::newDevice(int n)
{
    int com_index = m_configDialog->getCOMindex();
    if (-1 == com_index)
        return;

    ModbusDriverShared modbus = m_info.getDriver(com_index);
    if (modbus->readyToWork())
    {
        modbus->requestDeviceAproval(n);
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
        m_configDialog->setExternalConnection(m_externalManager.getExternalIP());
    }
    else
    {
        m_configDialog->dropExternalConnection();
    }
}

void Cooller_ModBusController::externalListChanged(void)
{
    m_configDialog->setExternalPorts(m_externalManager.getExternalPortsList());
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

        boost::property_tree::ptree values[2] = { tree.get_child("Config.InputValues"), tree.get_child("Config.OutValues") };

        for (int i = 0; i < 2; i++)
        {
            boost::property_tree::ptree& a_value = values[i];
            for (const std::pair<std::string, boost::property_tree::ptree> &p : a_value)
            {
                std::string name = p.first;
                ConfigMap::Parameter a_parameter;
                a_parameter.m_description = p.second.get_value<std::string>();
                a_parameter.m_registerNumber = p.second.get<int>("<xmlattr>.R");
                a_map->getInputInterval().add(a_parameter.m_registerNumber);
                a_parameter.m_isWriteble = (bool)i;
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
                a_map->addVariable(name, a_parameter);
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
   /* for (int i = 0; i < m_inParameters.size();i++)
    {
        int value;
        if (m_explorer->getRegisterValue(m_inParameters[i].first, value))
        {
            m_view->updateParameter(i, value,true);
        }
    }

    for (int i = 0; i < m_outParameters.size(); i++)
    {
        int value;
        if (m_explorer->getRegisterValue(m_outParameters[i].first, value))
        {
            m_view->updateParameter(i, value,false);
        }
    }*/
}

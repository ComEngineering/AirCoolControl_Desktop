
#ifdef _WIN32
    #pragma warning(disable:4996)
    #define  _D_SCL_SECURE_NO_WARNINGS
#endif

#include "Cooller_ModbusController.h"
#include <QtSerialPort\qserialportinfo.h>
#include <qstring>
#include <qobject>
#include <algorithm>
#include "modbusuart_impl.h"
#include "Configurator.h"
#include <qdiriterator.h>
#include "Logger.h"
#include <qmainwindow.h>

Cooller_ModBusController::Cooller_ModBusController(CoolerStateWidget *view, ModBusDialog *config) :
    m_view(view), 
    m_configDialog(config),
    m_recheckTimer(new QTimer(this)),
    m_available(false),
    m_comunicationSpeedIndex(6), //9600
    m_currentDeviceID(1),
    m_connector(this),
    m_externalManager(this),
    m_modbus(this)
{
    connect(m_recheckTimer, SIGNAL(timeout()), this, SLOT(updateState()));
    m_recheckTimer->setInterval(500);
    m_recheckTimer->start();
    config->setExternalCommunicator(&m_connector);

    connect(m_configDialog, SIGNAL(speedChanged(int)), this, SLOT(newSpeed(int)));
    connect(m_configDialog, SIGNAL(portChanged(int)), this, SLOT(newPort(int)));
    connect(m_configDialog, SIGNAL(deviceIDChanged(int)), this, SLOT(newDevice(int)));
    connect(&m_connector, SIGNAL(connectionEstablished()), config, SLOT(connectionEstablished()));
    connect(&m_connector, SIGNAL(connectionEstablished()), this, SLOT(sendConfiguration()));
    connect(&m_connector, SIGNAL(connectionBroken()), config, SLOT(connectionBroken()));
    connect(&m_connector, SIGNAL(connectionErrorOccured(QString)), config, SLOT(connectionErrorOccured(QString)));
    connect(&m_externalManager, SIGNAL(stateChanged()), this, SLOT(externalStateChanged()));
    connect(&m_externalManager, SIGNAL(listChanged()), this, SLOT(externalListChanged()));

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
}


Cooller_ModBusController::~Cooller_ModBusController()
{
}

void Cooller_ModBusController::updateState(void)
{
    checkConnectionState();

    if (m_explorer && m_explorer->getState() == DeviceExplorer::Ready)
    {
        updateStateWidget();
    }
}

bool Cooller_ModBusController::equalPredicat(QSerialPortInfo& first, QSerialPortInfo& second)
{
    return first.portName() == second.portName();
}

void Cooller_ModBusController::checkConnectionState(void)
{
    QList<QSerialPortInfo> a_info = QSerialPortInfo::availablePorts();

    if (a_info.empty())
    {
        m_configDialog->setError(QObject::tr("COM ports aren't available"), true);
        m_info.clear();
        m_configDialog->setCOMindex(-1);
        m_available = false;
        return;
    }
    
    if (a_info.size() != m_info.size() || ! std::equal(a_info.begin(), a_info.end(), m_info.begin(), equalPredicat))
    {
        int n = m_configDialog->getCOMindex();
        if (-1 != n)
        {
            QString currentPortName = m_info[n].portName();
            n = -1;
            for (int i = 0; i < a_info.size(); i++)
            {
                if (a_info[i].portName() == currentPortName)
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

        m_info = a_info;
        m_configDialog->setCOMlist(m_info);
        
        n = (-1 != n) ? n : 0;
        m_configDialog->setCOMindex(n);

        if (m_connector.isActive())
            sendConfiguration();
    }
}

void Cooller_ModBusController::newSpeed(int n)
{
    m_comunicationSpeedIndex = n;
}

void Cooller_ModBusController::newPort(int n)
{
    const static qint32 rates[] = {0};
    if (-1 != n)
    {
        QSerialPortInfo a_info = m_info[n];
        QString name = a_info.portName();
        m_modbus.setPortName(name);
        m_configDialog->setDeviceIndex(1);
    }
}

void Cooller_ModBusController::newDevice(int n)
{
    m_currentDeviceID = n;
    if (m_modbus.readyToWork())
    {
        if (m_explorer)
            m_explorer->stopTasks();
        m_explorer = std::make_shared<DeviceExplorer>(m_configs, m_modbus, m_currentDeviceID, this);
        if (m_explorer->getState() == DeviceExplorer::Ready)
        {
            m_inParameters = m_explorer->getCurrentConfig()->getInputParametersList();
            m_outParameters = m_explorer->getCurrentConfig()->getOutputParametersList();
            m_view->setParameterList(m_inParameters,true);
            m_view->setParameterList(m_outParameters,false);
        }
    }
}


void Cooller_ModBusController::sendConfiguration(void)
{
    QList<QString> ports;

    for (auto info : m_info)
    {
        ports.push_back(info.description() + ' ' + info.portName());
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

        boost::property_tree::ptree values = tree.get_child("Config.InputValues");

        for (const std::pair<std::string, boost::property_tree::ptree> &p : values)
        {
            std::string name = p.first;
            ConfigMap::Parameter a_parameter;
            a_parameter.m_description = p.second.get_value<std::string>();
            a_parameter.m_registerNumber = p.second.get<int>("<xmlattr>.R");
            a_map->getInputInterval().add(a_parameter.m_registerNumber);
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

        values = tree.get_child("Config.OutValues");
        for (const std::pair<std::string, boost::property_tree::ptree> &p : values)
        {
            std::string name = p.first;
            ConfigMap::Parameter a_parameter;
            a_parameter.m_description = p.second.get_value<std::string>();
            a_parameter.m_registerNumber = p.second.get<int>("<xmlattr>.R");
            a_map->getOutputInterval().add(a_parameter.m_registerNumber);
            a_parameter.m_maxValue = p.second.get<float>("<xmlattr>.max", FLT_MAX);
            a_parameter.m_minValue = p.second.get<float>("<xmlattr>.max", FLT_MIN);
            a_parameter.m_isWriteble = true;
            a_map->addVariable(name, a_parameter);
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
    for (int i = 0; i < m_inParameters.size();i++)
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
    }
}

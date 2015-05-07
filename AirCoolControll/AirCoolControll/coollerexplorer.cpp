#include "coollerexplorer.h"
#include "Logger.h"
#include "ConfigMap.h"
#include <boost/property_tree/xml_parser.hpp>
#include <cfloat>

CoollerExplorer::CoollerExplorer(const ConfigList& configs, ModbusDriver& modbus, int id, QObject *parent)
    : QObject(parent),
    m_state(Ready),
    m_currentDeviceID(0),
    m_configs(configs),
    m_modbus(modbus)
{
    m_currentDeviceID = id;
    DeviceInfo info;
    if (!m_modbus.readDeviceInfo(id, info))
    {
        m_errorString = tr("Can't read device identification information");
        m_state = DeviceNotReady;
        return;
    }

    bool rc = false;
    for (std::shared_ptr<ConfigMap> a_map : m_configs)
    {
        if (a_map->isSupport(info))
        {
            m_currentMap = a_map;
            rc = true;
            break;
        }
    }
    
    if (!rc)
    {
        m_errorString = tr("Can't find suitable config file for this device");
        m_state = ConfigNotFound;
    } 
    else
    {
        Interval i_int = m_currentMap->getInputInterval();
        Interval o_int = m_currentMap->getOutputInterval();
        m_inRegisters = std::make_shared<PullerReadTask>(m_currentDeviceID,i_int);
        m_outRegisters = std::make_shared<PullerReadTask>(m_currentDeviceID,o_int);
        m_modbus.addPullerReadTask(m_inRegisters);
        m_modbus.addPullerReadTask(m_outRegisters);
        m_localInPull.resize(i_int.second - i_int.first + 1);
        m_localOutPull.resize(o_int.second - o_int.first + 1);
    }
}

CoollerExplorer::~CoollerExplorer()
{
    
}

void CoollerExplorer::stopTasks()
{
    m_modbus.removeTaskWithID(m_currentDeviceID);
}

bool  CoollerExplorer::getRegisterValue(const std::string & key,int& value)
{
    if (m_state != Ready || ! m_currentMap->haveVariableWithName(key))
        return false;

    if (m_currentMap->isVariableOut(key))
    {
        if (m_outRegisters->isContentChanged())
            m_outRegisters->getContent(m_localOutPull);
        value = m_currentMap->getValue(key, m_localOutPull);
    }
    else
    {
        if (m_inRegisters->isContentChanged())
            m_inRegisters->getContent(m_localInPull);
        value = m_currentMap->getValue(key, m_localInPull);
    }

    return true;
}

bool CoollerExplorer::setRegisterValue(const std::string & key,int value)
{
    if (m_currentDeviceID != 0 && m_currentMap->haveVariableWithName(key))
    {
        return m_modbus.writeRegister(m_currentDeviceID, m_currentMap->getRegisterNumber(key), value);
    }
    return false;
}

QString CoollerExplorer::errorString()
{
    return m_errorString;
}


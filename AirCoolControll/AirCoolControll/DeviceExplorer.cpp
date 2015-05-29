#include "Logger.h"
#include <boost/property_tree/xml_parser.hpp>
#include <cfloat>
#include "PullerReadTask.h"
#include "DeviceExplorer.h"

DeviceExplorer::DeviceExplorer(const ConfigMapShared config, ModbusDriverShared modbus, int id, QObject *parent)
    : QObject(parent),
    m_state(Ready),
    m_deviceID(id),
    m_modbus(modbus),
    m_currentMap(config),
    m_errorString(tr("Working"))
{
    Interval i_int = m_currentMap->getInterval(ConfigMap::INPUT_REGISTERS_PULL);
    Interval o_int = m_currentMap->getInterval(ConfigMap::OUTPUT_REGISTERS_PULL);
    m_inRegisters = std::make_shared<PullerReadTask>(m_deviceID,i_int);
    m_outRegisters = std::make_shared<PullerReadTask>(m_deviceID, o_int);
    m_modbus->addPullerReadTask(m_inRegisters);
    m_modbus->addPullerReadTask(m_outRegisters);
    m_localInPull.resize(i_int.second - i_int.first + 1);
    m_localOutPull.resize(o_int.second - o_int.first + 1);
}

DeviceExplorer::~DeviceExplorer()
{
    stopTasks();
}

void DeviceExplorer::stopTasks()
{
    m_modbus->removeTaskWithID(m_deviceID);
}

bool  DeviceExplorer::getRegisterValue(const std::string & key,int& value)
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

bool DeviceExplorer::setRegisterValue(const std::string & key,int value)
{
    if ( m_currentMap->haveVariableWithName(key))
    {
        int bitNumber;
        if (m_currentMap->isVariableBool(key, bitNumber))
        {
            int currentValue;
            getRegisterValue(key, currentValue);
            value = (bool)value ? currentValue | (1 << bitNumber) : currentValue & ~(1 << bitNumber);
        }
        m_modbus->writeRegister(m_deviceID, m_currentMap->getRegisterNumber(key), value);
    }
    return true; // TO DO need asynchronious solution
}

QString DeviceExplorer::errorString()
{
    return m_errorString;
}


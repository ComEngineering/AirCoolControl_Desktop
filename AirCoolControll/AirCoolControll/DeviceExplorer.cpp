#include "Logger.h"
#include <cfloat>
#include "DeviceExplorer.h"

DeviceExplorer::DeviceExplorer(const ConfigMapShared config, ModbusDriverShared modbus, int id, QObject *parent)
    : QObject(parent),
    m_state(Ready),
    m_deviceID(id),
    m_modbus(modbus),
    m_currentMap(config),
    m_errorString(tr("Working"))
{
    for (int i = ConfigMap::INPUT_REGISTER; i < ConfigMap::REGISTER_PULL_COUNT; i++)
    {
        Interval a_int = m_currentMap->getInterval(i);
        if (!a_int.empty())
        {
            m_registers[i] = i != ConfigMap::COIL ? std::make_shared<PullerReadTask>(m_deviceID, a_int) :
                std::make_shared<PullerReadCoilTask>(m_deviceID, a_int);
            m_modbus->addPullerReadTask(m_registers[i]);
            m_localPull[i].resize(a_int.second - a_int.first + 1);
        }
    }
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

    ConfigMap::RegisterType a_type = m_currentMap->getVariableType(key);
    if (m_registers[a_type] == NULL)
        return false;

    if (m_registers[a_type]->isContentChanged())
        m_registers[a_type]->getContent(m_localPull[a_type]);
    value = m_currentMap->getValue(key, m_localPull[a_type]);
    
    return true;
}

void DeviceExplorer::setRegisterValue(const std::string & key,int value)
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
}

QString DeviceExplorer::errorString()
{
    return m_errorString;
}


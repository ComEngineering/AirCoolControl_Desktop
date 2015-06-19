#include "Logger.h"
#include <cfloat>
#include <qmdiarea.h>
#include "DeviceExplorer.h"

DeviceExplorer::DeviceExplorer(const ConfigMapShared config, ModbusDriverShared modbus, DeviceInfoShared info, QObject *parent)
    : QObject(parent),
    m_state(Ready),
    m_id(info->getID()),
    m_speed(info->getSpeed()),
    m_modbus(modbus),
    m_currentMap(config),
    m_errorString(tr("Working")),
    m_mdi(NULL)
{
    m_view = new CoolerStateWidget();
    
    for (ConfigMap::RegisterType i = ConfigMap::REGISTER_PULL_FIRST; i < ConfigMap::REGISTER_PULL_COUNT; ConfigMap::NEXT(i))
    {
        Interval a_int = m_currentMap->getInterval(i);
        if (!a_int.empty())
        {
            m_registers[i] = i != ConfigMap::COIL ? std::make_shared<PullerReadTask>(m_id, m_speed, a_int) :
                std::make_shared<PullerReadCoilTask>(m_id, m_speed, a_int);
            m_modbus->addPullerReadTask(m_registers[i]);
            m_localPull[i].resize(a_int.second - a_int.first + 1);
            m_view->setParameterList(m_currentMap->getParametersList(i), i);
            m_registers[i]->setListener(this);
        }
    }
    connect(m_view, SIGNAL(newRegisterValue(int, QString&, int)), this, SLOT(sendValueToDevice(int, QString&, int)));
}

DeviceExplorer::~DeviceExplorer()
{
    stopTasks();
    if (m_mdi)
        delete m_mdi;
    else
        delete m_view;
}

void DeviceExplorer::stopTasks()
{
    m_modbus->removeTaskWithID(m_id);
}

bool  DeviceExplorer::getRegisterValue(const std::string & key,int& value)
{
    if (m_state != Ready || ! m_currentMap->haveVariableWithName(key))
        return false;

    ConfigMap::RegisterType a_type = m_currentMap->getVariableType(key);
    if (m_registers[a_type] == NULL)
        return false;

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
        m_modbus->writeRegister(m_id, m_speed, m_currentMap->getRegisterNumber(key), value);
    }
}

void  DeviceExplorer::setCoilState(const std::string & key, bool state)
{
    if (m_currentMap->haveVariableWithName(key))
    {
        m_modbus->setCoil(m_id, m_speed, m_currentMap->getRegisterNumber(key), state);
    }
}

QString DeviceExplorer::errorString()
{
    return m_errorString;
}


void DeviceExplorer::sendValueToDevice(int registerType, QString& name, int v)
{
    switch (static_cast<ConfigMap::RegisterType>(registerType))
    {
    case ConfigMap::OUTPUT_REGISTER:
        setRegisterValue(name.toStdString(), v);
        break;
    case ConfigMap::COIL:
        setCoilState(name.toStdString(), static_cast<bool>(v));
        break;
    }
}

void DeviceExplorer::updateStateWidget()
{
    for (ConfigMap::RegisterType it = ConfigMap::REGISTER_PULL_FIRST; it < ConfigMap::REGISTER_PULL_COUNT; ConfigMap::NEXT(it))
    {
        const ConfigMap::ParameterList& parameters = m_currentMap->getParametersList(it);

        for (int i = 0; i < parameters.size(); i++)
        {
            int value;
            if (getRegisterValue(parameters[i].first, value))
            {
                m_view->updateParameter(i, value, it);
            }
        }
    }
}

void DeviceExplorer::activateView(QMdiArea * area)
{
    if (NULL == m_mdi)
    {
        m_mdi = new MdiSubWindowPermanent(m_view,area);
        area->addSubWindow(m_mdi);
    }
    m_mdi->show();
    m_mdi->setFocus();
    area->setActiveSubWindow(m_mdi);
}

void DeviceExplorer::somethingChanged()
{
    for (ConfigMap::RegisterType i = ConfigMap::REGISTER_PULL_FIRST; i < ConfigMap::REGISTER_PULL_COUNT; ConfigMap::NEXT(i))
    {
        if (m_registers[i]->isContentChanged())
        {
            m_registers[i]->getContent(m_localPull[i]);
            break;
        }
    }
}
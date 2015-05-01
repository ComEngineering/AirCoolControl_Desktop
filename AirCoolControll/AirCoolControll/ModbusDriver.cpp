#include "ModbusDriver.h"
#include <QMutexLocker>

ModbusDriver::ModbusDriver(QObject *parent)
    : QObject(parent),
    m_mutex(new QMutex()),
    m_puller(this)
{
    connect(&m_puller, SIGNAL(deviceListUpdated()), this, SIGNAL(deviceListUpdated()));   
}

ModbusDriver::~ModbusDriver()
{
   // m_puller.terminate();
    delete m_mutex;
}

bool ModbusDriver::setPortName(const QString& name)
{
    if (name == m_currentPortName)
        return true;
    
    m_puller.terminate();
    m_puller.clearTaskList();
    
    if (m_modbus)
    {
        disconnect(m_modbus.get(), SIGNAL(fatalError()), this, SLOT(UARTfail()));
    }
    m_modbus = std::make_shared<ModBusUART_Impl>(name,this);
    connect(m_modbus.get(), SIGNAL(fatalError()), this, SLOT(UARTfail()));

    if (m_modbus->isOpen())
    {
        m_currentPortName = name;
        m_puller.startPulling(m_modbus);
        m_puller.start();
        return true;
    }
    return false;
}

void ModbusDriver::UARTfail()
{
    m_puller.terminate();
}

void ModbusDriver::addPullerTask(PullerTaskShared a_task)
{
    m_puller.addTask(a_task);
}

bool ModbusDriver::readDeviceInfo(quint16 id, DeviceInfo& info)
{
    QString versionString;
    if (m_modbus->readDeviceInfo(id, info.m_vendor, info.m_product, versionString))
    {
        VersionStorage v(versionString);
        if (v.isLegal())
        {
            info.m_version = v;
            return true;
        }
    }
    return false;
}

bool ModbusDriver::writeRegister(quint16 id, quint16 regNumber, quint16 value)
{
    return m_modbus->writeRegister(id, regNumber, value);
}

void ModbusDriver::getDeviceList(DeviceInfoMap& map) const
{
    m_puller.getDeviceInfoList(map);
}

void ModbusDriver::removeTaskWithID(int id)
{
    m_puller.removeTaskWithID(id);
}

bool ModbusDriver::readyToWork() const
{
    return  (m_modbus.use_count() > 0 && m_modbus->isOpen());
}
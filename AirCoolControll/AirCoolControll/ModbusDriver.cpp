#include "ModbusDriver.h"
#include <QMutexLocker>
#include "PullerGetDeviceInfoTask.h"
#include "PullerWriteTask.h"
#include "DeviceInfo.h"

ModbusDriver::ModbusDriver(const QString& name, QObject *parent)
    : QObject(parent),
    m_puller(this)
{ 
    m_modbus = std::make_shared<ModBusUART_Impl>(name,this);
    connect(m_modbus.get(), SIGNAL(fatalError()), this, SLOT(UARTfail()));
    
    if (m_modbus->isOpen())
    {
        m_currentPortName = name;
        m_puller.startPulling(m_modbus);
        m_puller.start();
    }
}

ModbusDriver::~ModbusDriver()
{
   // m_puller.terminate();

}

void ModbusDriver::UARTfail()
{
    m_puller.terminate();
    emit connectionFail();
}

void ModbusDriver::addPullerReadTask(PullerReadTaskShared a_task)
{
    m_puller.addTask(a_task);
}

void ModbusDriver::requestDeviceAproval(quint16 id)
{
    detectionCallback cb = std::bind(&ModbusDriver::onDeviceDetected, this, std::placeholders::_1);
    PullerGetDeviceInfoTaskShared a_task = std::make_shared<PullerGetDeviceInfoTask>(id,m_currentPortName,cb);
    m_puller.addTask(a_task);
}

void ModbusDriver::onDeviceDetected(DeviceInfoShared a_info)
{
    emit deviceDetected(a_info);
}

void ModbusDriver::writeRegister(quint16 id, quint16 regNumber, quint16 value)
{
    PullerWriteTaskShared a_task = std::make_shared<PullerWriteTask>(id, regNumber, value);
    m_puller.addTask(a_task);
}

void ModbusDriver::removeTaskWithID(int id)
{
    m_puller.removeTaskWithID(id);
}

bool ModbusDriver::readyToWork() const
{
    return  (m_modbus.use_count() > 0 && m_modbus->isOpen());
}
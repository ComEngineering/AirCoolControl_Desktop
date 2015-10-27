#include "ModbusDriver.h"
#include <QMutexLocker>
#include "PullerWriteTask.h"
#include "PullerSetCoilTask.h"
#include "DeviceInfo.h"

ModbusDriver::ModbusDriver(const QString& name, QObject *parent)
    : QObject(parent),
    m_puller(this)
{ 
    m_currentPortName = name;
    m_puller.startPulling(name);
}

ModbusDriver::~ModbusDriver()
{
}

void ModbusDriver::UARTfail()
{
    m_puller.stopPulling();
    emit connectionFail();
}

void ModbusDriver::addPullerReadTask(PullerTaskShared a_task)
{
    m_puller.addTask(a_task);
}

void ModbusDriver::requestDeviceAproval(quint16 id, int speed)
{
    detectionCallback cb = std::bind(&ModbusDriver::onDeviceDetected, this, std::placeholders::_1);
    PullerGetDeviceInfoTaskShared a_task = std::make_shared<PullerGetDeviceInfoTask>(id, speed, m_currentPortName, cb);
    m_puller.addTask(a_task);
}

void ModbusDriver::onDeviceDetected(DeviceInfoShared a_info)
{
    emit deviceDetected(a_info);
}

void ModbusDriver::writeRegister(quint16 id, int speed, quint16 regNumber, quint16 value)
{
    PullerWriteTaskShared a_task = std::make_shared<PullerWriteTask>(id, speed, regNumber, value);
    m_puller.addTask(a_task);
}

void ModbusDriver::setCoil(quint16 id, int speed, quint16 regNumber, bool state)
{
    PullerSetCoilTaskShared a_task = std::make_shared<PullerSetCoilTask>(id, speed, regNumber, state);
    m_puller.addTask(a_task);
}

void ModbusDriver::removeTaskWithID(int id)
{
    m_puller.removeTaskWithID(id);
}

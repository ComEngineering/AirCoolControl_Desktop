#include "UART_DeviceStorage.h"
#include <QtSerialPort\qserialportinfo.h>
#include <assert.h>

UART_DeviceStorage::UART_DeviceStorage(QObject *parent)
    : QObject(parent)
{

}

UART_DeviceStorage::~UART_DeviceStorage()
{

}

bool UART_DeviceStorage::update(const QList<QSerialPortInfo>& info)
{
    QList<QString> names;
    QString name;
    bool rc = false;

    for (QList<QSerialPortInfo>::const_iterator a_info = info.begin(); a_info != info.end(); a_info++)
    {
        name = a_info->description() + ' ' + a_info->portName();
        if (m_storage.find(name) == m_storage.end())
        {
            m_storage[name] = std::pair<QString, ModbusDriverShared>(a_info->portName(), NULL);
            rc = true;
        }
        names.push_back(name);
    }

    for (std::map<QString, std::pair<QString, ModbusDriverShared>>::iterator it = m_storage.begin(); it != m_storage.end(); )
    {
        name = it->first;
        if (!names.contains(name))
        {
            QString removedUART_systemName = it->second.first;
            it = m_storage.erase(it);
            rc = true;
            emit uartDisconnected(removedUART_systemName);
        }
        else
            it++;
    }

    if (rc)
    {
        m_syncNames.clear();
        for (auto i : m_storage)
        {
            m_syncNames.append(i.first);
        }
    }
    return rc;
}

bool UART_DeviceStorage::empty(void) const
{
    return m_storage.empty();
}

int  UART_DeviceStorage::size(void) const
{
    return m_storage.size();
}

QString UART_DeviceStorage::portName(int i) const
{
    return m_syncNames[i];
}

QList<QString> UART_DeviceStorage::getPortList(void) const
{
    return m_syncNames;
}

ModbusDriverShared UART_DeviceStorage::getDriverWithName(const QString& name) 
{
    ModbusDriverShared rc;
    std::map<QString, std::pair<QString, ModbusDriverShared>>::const_iterator f = m_storage.find(name);
    if (m_storage.end() != f)
    {
        std::pair<QString, ModbusDriverShared> driverInfo = f->second;
        rc = driverInfo.second;
        if (!rc)
        {
            rc = std::make_shared<ModbusDriver>(driverInfo.first, this);
            m_storage[name].second = rc;

            connect(rc.get(), SIGNAL(deviceDetected(DeviceInfoShared)), this, SIGNAL(deviceDetected(DeviceInfoShared)));
        }
    }

    return rc;
}

ModbusDriverShared UART_DeviceStorage::getDriver(int index) 
{
    assert(m_storage.size() > index);

    QString name = m_syncNames[index];
 
    return getDriverWithName(name);
}

ModbusDriverShared UART_DeviceStorage::getDriver(const QString& name)
{
    ModbusDriverShared rc;
    for (auto i : m_storage)
    {
        rc = i.second.second;
        if (rc && rc->getName() == name)
            break;    
    }

    return rc;
}

void UART_DeviceStorage::releaseDriver(const QString& name)
{
    for (auto& i : m_storage)
    {
        if (i.second.first == name)
        {
            i.second.second = NULL;
            break;
        }
    }
}

std::vector<std::pair<QString, bool>> UART_DeviceStorage::getDriverList(void) const
{
    std::vector<std::pair<QString, bool>> rc;
    for (auto i : m_storage)
        rc.push_back(std::pair<QString, bool>(i.second.first,static_cast<bool>(i.second.second)));

    return rc;
}
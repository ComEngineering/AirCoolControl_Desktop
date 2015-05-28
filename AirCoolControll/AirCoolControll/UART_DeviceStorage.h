#ifndef UART_DEVICESTORAGE_H
#define UART_DEVICESTORAGE_H

#include <QObject>
#include "ModbusDriver.h"
#include <map>

class UART_DeviceStorage : public QObject
{
    Q_OBJECT

public:
    UART_DeviceStorage(QObject *parent);
    ~UART_DeviceStorage();

    bool    update(const QList<QSerialPortInfo>& a_info);
    bool    empty(void) const;
    int     size(void) const;
    QString portName(int i) const;
    QList<QString> getPortList(void) const;
    ModbusDriverShared getDriver(int com_index);
    ModbusDriverShared getDriverWithName(const QString& name);

signals:
    void uartDisconnected(const QString& name);
    void detectedError(const QString errorDescription);
    void deviceDetected(DeviceInfoShared info);

private:
    template<typename T>
    ModbusDriverShared getDriver(T arg1);

private:
    std::map<QString,std::pair<QString, ModbusDriverShared>> m_storage;
    QList<QString>                                           m_syncNames;
};

#endif // UART_DEVICESTORAGE_H

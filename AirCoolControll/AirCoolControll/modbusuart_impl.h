#ifndef MODBUSUART_IMPL_H
#define MODBUSUART_IMPL_H

#include <QObject>
#include <QtSerialPort\qserialport.h>
#include <qvector.h>
#include <qmutex.h>
#include <memory>
#include "VersionStorage.h"
#include <unordered_map>


struct DeviceInfo
{
    DeviceInfo(const QString& ven, const QString& p, const QString& ver) : m_vendor(ven), m_product(p), m_version(ver){}
    DeviceInfo(){} 
    QString        m_vendor;
    QString        m_product;
    VersionStorage m_version;
};

typedef std::shared_ptr<DeviceInfo> DeviceInfoShared;

typedef std::unordered_map<int, DeviceInfoShared> DeviceInfoMap;

class ModBusUART_Impl : public QObject
{
    Q_OBJECT

public:
    ModBusUART_Impl(const QString& info,QObject *parent);
    ~ModBusUART_Impl();

    void   setTimeOut(int t);
    void   setSpeed(int speed);
    bool   readRegisterPool(quint16 id, quint16 regNumber,quint16 regCount,QVector<quint16>& list);
    bool   writeRegister(quint16 id, quint16 regNumber, quint16 value);
    bool   readDeviceInfo(quint16 id, QString& vendor, QString& product, QString& version);
    bool   isOpen(){ return m_isOpen; }

private slots:
    void communicationError(QSerialPort::SerialPortError);

signals:
    void fatalError(void);

private:
    ModBusUART_Impl(ModBusUART_Impl&);
    ModBusUART_Impl& operator =(ModBusUART_Impl&);

    static quint16 crc16(const char* buffer, int length);
    static QByteArray ModBusUART_Impl::makeRTUFrame(int slave, int function, const QByteArray & data);

    bool checkCRC(const QByteArray& data) const;

private:
    QSerialPort     m_port;
    int             m_timeOut;
    mutable QMutex  m_mutex;
    bool            m_isOpen;
};

typedef std::shared_ptr<ModBusUART_Impl> ModBusUART_ImplShared;
#endif // MODBUSUART_IMPL_H

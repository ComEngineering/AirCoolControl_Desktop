#ifndef MODBUSUART_IMPL_H
#define MODBUSUART_IMPL_H

#include <QObject>
#include <QtSerialPort\qserialport.h>
#include <qvector.h>
#include <qmutex.h>
#include <memory>

class ModBusUART_Impl : public QObject
{
    Q_OBJECT

public:
    ModBusUART_Impl(const QString& info,QObject *parent = NULL);
    ~ModBusUART_Impl();

    void   setSpeed(int speed);
    
    bool   readRegisterPool(quint16 id, int speed, quint16 regNumber,quint16 regCount,std::vector<quint16>& list);
    bool   writeRegister(quint16 id, int speed, quint16 regNumber, quint16 value);

    bool   readCoilPool(quint16 id, int speed, quint16 regNumber, quint16 regCount, std::vector<quint16>& list);
    bool   writeCoil(quint16 id, int speed, quint16 regNumber, bool state);

    bool   readDeviceInfo(quint16 id, int speed, QString& vendor, QString& product, QString& version);
    bool   isOpen(){ return m_isOpen; }

private slots:
    void communicationError(QSerialPort::SerialPortError);

signals:
    void fatalError(void);

private:
    ModBusUART_Impl(ModBusUART_Impl&);
    ModBusUART_Impl& operator =(ModBusUART_Impl&);

    bool startRequest(int speed);
    void stopRequest(void);

    static quint16 crc16(const char* buffer, int length);
    static QByteArray ModBusUART_Impl::makeRTUFrame(int slave, int function, const QByteArray & data);

    bool checkCRC(const QByteArray& data) const;

private:
    QSerialPort     m_port;
    mutable QMutex  m_mutex;
    bool            m_isOpen;
};

typedef std::shared_ptr<ModBusUART_Impl> ModBusUART_ImplShared;

#endif // MODBUSUART_IMPL_H

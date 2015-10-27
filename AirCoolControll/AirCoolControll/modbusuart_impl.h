#ifndef MODBUSUART_IMPL_H
#define MODBUSUART_IMPL_H

#include <QObject>
#include <QtSerialPort\qserialport.h>
#include <qtimer.h>
#include <qmutex.h>
#include <memory>

class PullerTaskBase;
class ModbusPuller;

class ModBusUART_Impl : public QObject
{
    Q_OBJECT

public:
    ModBusUART_Impl(const QString& info, ModbusPuller *parent);
    ~ModBusUART_Impl();

    void   setSpeed(int speed);
    
    bool   readRegisterPool(PullerTaskBase* owner, quint16 id, int speed, quint16 regNumber, quint16 regCount);
    bool   writeRegister(PullerTaskBase* owner, quint16 id, int speed, quint16 regNumber, quint16 value);

    bool   readCoilPool(PullerTaskBase* owner, quint16 id, int speed, quint16 regNumber, quint16 regCount);
    bool   writeCoil(PullerTaskBase* owner, quint16 id, int speed, quint16 regNumber, bool state);

    bool   readDeviceInfo(PullerTaskBase* owner, quint16 id, int speed);
    bool   isOpen(){ return m_isOpen; }

private slots:
    void communicationError(QSerialPort::SerialPortError);
    void writeFinished(qint64);
    void readFinished(void);
    void readTimeElapsed(void);

signals:
    void fatalError(void);

private:
    enum RequestType {
        READ_REGISTER_POOL,
        WRITE_REGISTER,
        READ_COIL_POOL,
        WRITE_COIL,
        READ_DEVICE_INFO
    };
    ModBusUART_Impl(ModBusUART_Impl&);
    ModBusUART_Impl& operator =(ModBusUART_Impl&);

    bool startRequest(int speed);
    void stopRequest(bool remove);

    static quint16 crc16(const char* buffer, int length);
    static QByteArray ModBusUART_Impl::makeRTUFrame(int slave, int function, const QByteArray & data);

    bool checkCRC(const QByteArray& data) const;

    void handleReadDeviceInfo();
    void handleWriteCoil();
    void handleWriteRegister();
    void handleReadCoilPool();
    void handleReadRegisterPool();

private:
    QSerialPort     m_port;
    bool            m_isOpen;
    RequestType     m_currentRequest;
    int             m_currentRequestLength;
    int             m_currentResponseLength;
    int             m_regNumber;
    quint16         m_id;
    PullerTaskBase* m_owner;
    QTimer          m_readWaitTimer;
    QByteArray      m_buffer;
    ModbusPuller*   m_puller;
};

typedef std::shared_ptr<ModBusUART_Impl> ModBusUART_ImplShared;

#endif // MODBUSUART_IMPL_H

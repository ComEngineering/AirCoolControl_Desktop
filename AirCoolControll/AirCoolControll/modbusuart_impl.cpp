#include "modbusuart_impl.h"
#include <qdatastream.h>
#include <QtEndian> 
#include <QMutexLocker>

ModBusUART_Impl::ModBusUART_Impl(const QString& name, QObject *parent)
    : QObject(parent),
    m_timeOut(50) // default value
{
    connect(&m_port, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(communicationError(QSerialPort::SerialPortError)));

    m_port.setPortName(name);
}

ModBusUART_Impl::~ModBusUART_Impl()
{
    QMutexLocker locker(&m_mutex);
}

bool ModBusUART_Impl::startRequest(void)
{
    if (m_port.open(QIODevice::ReadWrite)) 
    {
        return  
            m_port.setDataBits(QSerialPort::Data8) &&
            m_port.setFlowControl(QSerialPort::NoFlowControl) &&
            m_port.setStopBits(QSerialPort::OneStop) &&
            m_port.setParity(QSerialPort::NoParity) &&
            m_port.setBaudRate(QSerialPort::Baud9600);
    }
   
    return false;
}

void ModBusUART_Impl::stopRequest(void)
{
    m_port.close();
}

void ModBusUART_Impl::setTimeOut(int t)
{
    m_timeOut = t;
}

void ModBusUART_Impl::setSpeed(int speed)
{
    m_port.setBaudRate(speed);
}

bool ModBusUART_Impl::readRegisterPool(quint16 id, quint16 regNumber, quint16 regCount,QVector<quint16>& o_list)
{
    if (!startRequest())
        return false;

    bool rc = false;

    do
    {
        quint16 regNumberBig = qToBigEndian<quint16>(regNumber);
        quint16 regCountBig = qToBigEndian<quint16>(regCount);

        QByteArray data = QByteArray::fromRawData((const char*)& regNumberBig, sizeof(quint16));
        data += QByteArray::fromRawData((const char*)& regCountBig, sizeof(quint16));
        QByteArray req = makeRTUFrame(id, 3, data);

        QMutexLocker locker(&m_mutex);

        m_port.write(req);
        if (!m_port.waitForBytesWritten(m_timeOut))
        {
            break;
        }

        int responseLengthMust = 1 + 1 + 1 + 2 * regCount + 2;

        if (m_port.waitForReadyRead(m_timeOut))
        {
            QByteArray responseData = m_port.readAll();
            while (m_port.waitForReadyRead(20))  // TO DO from settings
                responseData += m_port.readAll();

            if (!checkCRC(responseData))
                break;

            if (responseData.size() != responseLengthMust || responseData[1] != char(3) || responseData[0] != char(id))
                break;

            o_list.clear();
            const uchar* d = (const uchar *)responseData.data() + 3;
            for (int i = 0; i < regCount; i++)
            {
                quint16 v = qFromBigEndian<quint16>(d + i * 2);
                o_list.push_back(v);
            }

            rc = true;
        }
    } while (false);

    stopRequest();
    return rc;
}

bool ModBusUART_Impl::writeRegister(quint16 id, quint16 regNumber, quint16 value)
{
    if (!startRequest())
        return false;

    bool rc = false;

    do
    {
        quint16 regNumberBig = qToBigEndian<quint16>(regNumber);
        quint16 valueBig = qToBigEndian<quint16>(value);
        QByteArray data = QByteArray::fromRawData((const char*)& regNumberBig, sizeof(quint16));
        data.append(QByteArray::fromRawData((const char*)&valueBig, sizeof(quint16)));
        QByteArray req = makeRTUFrame(id, 6, data);

        QMutexLocker locker(&m_mutex);

        m_port.write(req);

        if (!m_port.waitForBytesWritten(m_timeOut))
            break;

        if (m_port.waitForReadyRead(m_timeOut))
        {
            QByteArray responseData = m_port.readAll();
            while (m_port.waitForReadyRead(20)) // TO DO from settings
                responseData += m_port.readAll();

            if (!checkCRC(responseData))
                break;

            if (responseData.size() == 8 && responseData[1] != char(6) || responseData[0] != char(id))
            {
                rc = true;
            }
        }
    }
    while (false);
    
    stopRequest();
    return rc;
}

bool ModBusUART_Impl::readCoilPool(quint16 id, quint16 regNumber, quint16 coilCount, QVector<bool>& o_list)
{
    if (!startRequest())
        return false;

    bool rc = false;

    do
    {
        quint16 regNumberBig = qToBigEndian<quint16>(regNumber);
        quint16 regCountBig = qToBigEndian<quint16>(coilCount);

        QByteArray data = QByteArray::fromRawData((const char*)& regNumberBig, sizeof(quint16));
        data += QByteArray::fromRawData((const char*)& regCountBig, sizeof(quint16));
        QByteArray req = makeRTUFrame(id, 1, data);

        QMutexLocker locker(&m_mutex);

        m_port.write(req);
        if (!m_port.waitForBytesWritten(m_timeOut))
        {
            break;
        }

        char byteMust = (coilCount + 7) / 8;
        int responseLengthMust = 1 + 1 + 1 + byteMust + 2;

        if (m_port.waitForReadyRead(m_timeOut))
        {
            QByteArray responseData = m_port.readAll();
            while (m_port.waitForReadyRead(20))  // TO DO from settings
                responseData += m_port.readAll();

            if (!checkCRC(responseData))
                break;

            if (responseData.size() != responseLengthMust || responseData[1] != char(1) || responseData[0] != char(id) || responseData[2] != byteMust)
                break;

            o_list.clear();
            const uchar* d = (const uchar *)responseData.data() + 3;
            for (int i = 0; i < coilCount; i++)
            {
                quint8 v = qFromBigEndian<quint8>(d + i / 8);
                o_list.push_back(v & (0x1 << (i % 8)) );
            }

            rc = true;
        }
    } while (false);

    stopRequest();
    return rc;
}

bool ModBusUART_Impl::writeCoil(quint16 id, quint16 regNumber, bool state)
{
    if (!startRequest())
        return false;

    bool rc = false;

    do
    {
        quint16 regNumberBig = qToBigEndian<quint16>(regNumber);
        quint16 valueBig = (state) ? 0xff00 : 0x0;
        QByteArray data = QByteArray::fromRawData((const char*)& regNumberBig, sizeof(quint16));
        data.append(QByteArray::fromRawData((const char*)&valueBig, sizeof(quint16)));
        QByteArray req = makeRTUFrame(id, 5, data);

        QMutexLocker locker(&m_mutex);

        m_port.write(req);

        if (!m_port.waitForBytesWritten(m_timeOut))
            break;

        if (m_port.waitForReadyRead(m_timeOut))
        {
            QByteArray responseData = m_port.readAll();
            while (m_port.waitForReadyRead(20)) // TO DO from settings
                responseData += m_port.readAll();

            if (!checkCRC(responseData))
                break;

            if (responseData.size() == 8 && responseData[1] != char(5) || responseData[0] != char(id))
            {
                rc = true;
            }
        }
    } while (false);

    stopRequest();
    return rc;
}

bool ModBusUART_Impl::readDeviceInfo(quint16 id, QString& vendor, QString& product, QString& version)
{
    if (!startRequest())
        return false;

    bool rc = false;

    do
    {
        const char reqBody[] = { char(0x0E), char(1), char(0) };
        QByteArray reqBodyArray(reqBody);
        reqBodyArray.append(char(0));
        QByteArray req = makeRTUFrame(id, 43, reqBodyArray);

        QMutexLocker locker(&m_mutex);

        if (m_port.write(req) != req.size())
            break;

        if (!m_port.waitForBytesWritten(m_timeOut))
            break;

        if (m_port.waitForReadyRead(m_timeOut))
        {
            QByteArray responseData = m_port.readAll();
            while (m_port.waitForReadyRead(20)) // TO DO from settings
                responseData += m_port.readAll();

            if (!checkCRC(responseData))
                break;

            if (responseData[1] != char(43) || responseData[0] != char(id) || responseData[2] != char(0x0e) || responseData[3] != char(1))
                break;
            try
            {
                int numberOfObjects = responseData[7];
                if (numberOfObjects < 3)
                    break;
                
                int i = 0;
                for (int startIndex = 8; i < numberOfObjects; i++)
                {
                    if (i != responseData[startIndex])
                        break;
                    int len = responseData[startIndex + 1];
                    std::string a_string(responseData.data() + startIndex + 2, len);
                    QString* target;
                    switch (i)
                    {
                    case 0:
                        target = &vendor;
                        break;
                    case 1:
                        target = &product;
                        break;
                    case 2:
                        target = &version;
                        break;
                    }
                    (*target) = QString::fromStdString(a_string);
                    startIndex += len + 2;
                }
                if (i == numberOfObjects)
                    rc = true;
            }
            catch (...)
            {

            }
        }
    } while (false);

    stopRequest();
    return rc;
}

bool ModBusUART_Impl::checkCRC(const QByteArray& data) const
{
    int len = data.size();
    if (len < 4)
        return false;

    quint16 crc = crc16(data.data(), len - 2);
    quint16 crcInResponse = qFromLittleEndian<quint16>((const uchar *)data.data() + len - 2);
    return (crc == crcInResponse);
}

void ModBusUART_Impl::communicationError(QSerialPort::SerialPortError err)
{
    QString errDescription = m_port.errorString();
    switch (err)
    {
    case QSerialPort::UnknownError :
        m_port.clearError();
        /*m_port.close();
        if (m_port.open(QIODevice::ReadWrite))
        {
            m_isOpen = true;
        }
        else
        {
            m_isOpen = false;
        }*/
        break;
    case QSerialPort::DeviceNotFoundError:
    case QSerialPort::PermissionError:
    case QSerialPort::NotOpenError:
        m_isOpen = false;
        if (m_port.isOpen())
        {
            m_port.close();
        }
        emit fatalError();
        break;
    }
}

quint16 ModBusUART_Impl::crc16(const char* buffer, int length)
{
    static const unsigned int wCRCTable[] = {
        0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
        0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
        0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
        0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
        0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
        0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
        0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
        0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
        0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
        0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
        0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
        0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
        0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
        0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
        0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
        0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
        0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
        0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
        0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
        0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
        0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
        0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
        0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
        0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
        0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
        0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
        0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
        0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
        0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
        0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
        0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
        0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

    unsigned char temp;
    unsigned int  word = 0xFFFF;

    while (length--)
    {
        temp = *buffer++ ^ word;
        word >>= 8;
        word ^= wCRCTable[temp];
    }
    return word;

}

QByteArray ModBusUART_Impl::makeRTUFrame(int slave, int function, const QByteArray & data)
{
    QByteArray frame;
    QDataStream ds(&frame, QIODevice::WriteOnly);
    ds.setByteOrder(QDataStream::LittleEndian);
    ds << quint8(slave) << quint8(function);
    ds.writeRawData(data.constData(), data.size());
    int const crc = crc16(frame.constData(), frame.size());
    ds << quint16(crc);
    return frame;
}

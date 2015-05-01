#ifndef EXTERNALCONNECTOR_H
#define EXTERNALCONNECTOR_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <boost/property_tree/xml_parser.hpp>

class ExternalConnector : public QObject
{
    Q_OBJECT

public:
    ExternalConnector(QObject *parent);
    ~ExternalConnector();

    void connectToRemote(const QString& ip);
    void disconnectFromRemote();
    bool isActive();
    void sendPortList(const QList<QString>& list);

    const static int COMUNICATION_PORT = 11223;//// TO DO configure a port number

private:
    void sendHandShake();
    void sendMessage(std::string& msg);
    void handshakeProc(boost::property_tree::ptree*);

private slots:
    void socketConnected();
    void socketDisconnected();
    void connectionError(QAbstractSocket::SocketError);
    void readData();

signals:
    void connectionEstablished();
    void connectionBroken();
    void connectionErrorOccured(QString err);

private:
    QTcpSocket      m_socket;
    bool            m_active;
    bool            m_isHeaderRed;
    int             m_bytesLeftInMessage;
    QByteArray      m_data;
};

#endif // EXTERNALCONNECTOR_H

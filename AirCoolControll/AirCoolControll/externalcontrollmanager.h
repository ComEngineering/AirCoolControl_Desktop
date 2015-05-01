#ifndef EXTERNALCONTROLLMANAGER_H
#define EXTERNALCONTROLLMANAGER_H

#include <QObject>
#include <QtNetwork/qtcpserver.h>
#include <boost/property_tree/xml_parser.hpp>
#include <qmutex.h>

class ExternalControllManager : public QObject
{
    Q_OBJECT

public:
    ExternalControllManager(QObject *parent);
    ~ExternalControllManager();

    QString& getErrorDescription(void) const;
    bool     isActiveConnection(void) const;
    QString  getExternalIP(void) const;
    const QList<QString>& getExternalPortsList(void) const;

public slots:
    void newConnection(void);
    void readClient(void);
    void clientDisconnected(void);
    void connectionError(QAbstractSocket::SocketError);

signals:
    void stateChanged(void);
    void listChanged(void);

private:
    void runServer();
    void sendMessage(std::string& msg);
    bool checkVersion(std::string& version);
    void handshakeProc(boost::property_tree::ptree*);
    void portsProc(boost::property_tree::ptree*);

private:
    QTcpServer      m_server;
    bool            m_server_status;
    QString         m_errorDescription;
    QTcpSocket*     m_clientSocket;
    bool            m_isHeaderRed;
    int             m_bytesLeftInMessage;
    QByteArray      m_data;
    QList<QString>  m_portsInfo;
    mutable QMutex  m_mutex;
};

#endif // EXTERNALCONTROLLMANAGER_H

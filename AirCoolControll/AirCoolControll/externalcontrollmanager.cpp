#include "externalcontrollmanager.h"
#include "externalconnector.h"
#include <string>
#include <map>
#include <sstream>
#include "Logger.h"
#include "RequestRouter.h"
#include <QMutexLocker>

ExternalControllManager::ExternalControllManager(QObject *parent)
    : QObject(parent),
    m_server(this),
    m_server_status(false),
    m_isHeaderRed(false),
    m_clientSocket(NULL)
{
    connect(&m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    runServer();
}

ExternalControllManager::~ExternalControllManager()
{

}

void ExternalControllManager::runServer()
{
    if (!m_server.listen(QHostAddress::Any, ExternalConnector::COMUNICATION_PORT) && !m_server_status) {
        m_errorDescription = QObject::tr("Unable to start the server: %1.").arg(m_server.errorString());
    }
    else {
        m_server_status = true;
    }
}

void ExternalControllManager::sendMessage(std::string& msg)
{
    int messageLength = msg.size();
    m_clientSocket->write((char*)&messageLength, sizeof(int));
    m_clientSocket->write(msg.c_str());
}

void ExternalControllManager::newConnection(void)
{
    if (m_server_status)
    {
        QMutexLocker locker(&m_mutex);

        m_clientSocket = m_server.nextPendingConnection();
        QString clientAdress = m_clientSocket->peerAddress().toString();
        
        connect(m_clientSocket, SIGNAL(readyRead()), this, SLOT(readClient()));
        connect(m_clientSocket, SIGNAL(disconnected(void)), this, SLOT(clientDisconnected()));
        connect(m_clientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
        m_server.close();

        emit stateChanged();
    }
}

void ExternalControllManager::readClient(void)
{
    const static RequestRouter::RouterMap routerMap = {
        RequestRouter::RouterPair(std::string("handshake"), std::bind(&ExternalControllManager::handshakeProc, this, std::placeholders::_1)),
        RequestRouter::RouterPair(std::string("ports"), std::bind(&ExternalControllManager::portsProc, this, std::placeholders::_1))
    };

    const static RequestRouter router(routerMap);

    QMutexLocker locker(&m_mutex); 

    while (m_clientSocket->bytesAvailable())
    {
        if (!m_isHeaderRed)
        {
            QByteArray header = m_clientSocket->read(sizeof(int));
            m_bytesLeftInMessage = *(int*)header.data();
            m_isHeaderRed = true;
        }

        m_data.append(m_clientSocket->read(m_bytesLeftInMessage));
        m_bytesLeftInMessage -= m_data.length();
        if (0 == m_bytesLeftInMessage)
        {
            std::istringstream ss(m_data.data());
            m_data.clear();
            m_isHeaderRed = false;
            router.routeRequest(ss, "command");
        }
    }
}

void ExternalControllManager::handshakeProc(boost::property_tree::ptree* params)
{
    std::string versionString = params->get<std::string>("version");
    boost::property_tree::ptree response;
    response.add("command", "handshake");
    if (checkVersion(versionString))
    {
        response.add("params.success", "true");
    }
    else
    {
        response.add("params.success", "false");
    }
    std::ostringstream ss;
    boost::property_tree::write_xml(ss,response);
    sendMessage(ss.str());
}

void ExternalControllManager::clientDisconnected(void)
{
    {
        QMutexLocker locker(&m_mutex);

        m_clientSocket = NULL;
    }
    emit stateChanged();

    runServer();
}

void ExternalControllManager::connectionError(QAbstractSocket::SocketError err)
{
    Logger::log(m_clientSocket->errorString().toUtf8().constData(), Logger::Error);
}

bool ExternalControllManager::checkVersion(std::string& version)
{
    return true;
}

void ExternalControllManager::portsProc(boost::property_tree::ptree* params)
{
    boost::property_tree::ptree list = params->get_child("ports");
    QList<QString>  portsInfo;
    for (const std::pair<std::string, boost::property_tree::ptree> &p : list)
    { 
        portsInfo.push_back(QString::fromStdString(p.second.get_value<std::string>()));
    }
    
    if (m_portsInfo != portsInfo)
    {
        m_portsInfo = portsInfo;
        emit listChanged();
    }
}

bool ExternalControllManager::isActiveConnection(void) const
{
    return NULL != m_clientSocket;
}

QString ExternalControllManager::getExternalIP(void) const
{
    if (!m_server_status || NULL == m_clientSocket)
        return QString();

    return m_clientSocket->peerAddress().toString();
}

const QList<QString>& ExternalControllManager::getExternalPortsList(void) const
{
    return m_portsInfo;
}

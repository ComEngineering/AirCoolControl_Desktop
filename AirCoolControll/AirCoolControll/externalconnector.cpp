#include "externalconnector.h"
#include <boost/property_tree/ptree.hpp>
#include <sstream>
#include <boost/property_tree/xml_parser.hpp>
#include "RequestRouter.h"

ExternalConnector::ExternalConnector(QObject *parent)
    : QObject(parent),
    m_socket(this),
    m_active(false),
    m_isHeaderRed(false)
{
    connect(&m_socket, SIGNAL(connected(void)), this, SLOT(socketConnected()));
    connect(&m_socket, SIGNAL(disconnected(void)), this, SLOT(socketDisconnected()));
    connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(&m_socket, SIGNAL(readyRead(void)), this, SLOT(readData()));
}

ExternalConnector::~ExternalConnector()
{

}

bool ExternalConnector::isActive()
{
    return m_active;
}

void ExternalConnector::connectToRemote(const QString& ip)
{
    m_socket.connectToHost(ip, COMUNICATION_PORT);
}

void ExternalConnector::disconnectFromRemote()
{
    m_socket.disconnectFromHost();
}

void ExternalConnector::socketConnected()
{
    sendHandShake();
}

void ExternalConnector::socketDisconnected()
{
    emit connectionBroken();
    m_active = false;
}

void ExternalConnector::connectionError(QAbstractSocket::SocketError err)
{
    m_active = false;
    QString errorDescription = m_socket.errorString();
    emit connectionErrorOccured(errorDescription);
}

void ExternalConnector::sendMessage(std::string& msg)
{
    int messageLength = msg.size();
    m_socket.write((char*)&messageLength, sizeof(int));
    m_socket.write(msg.c_str());
}

void ExternalConnector::readData()
{
    const static RequestRouter::RouterMap routerMap = {
        RequestRouter::RouterPair(std::string("handshake"), std::bind(&ExternalConnector::handshakeProc, this, std::placeholders::_1))
    };

    const static RequestRouter router(routerMap);

    while (m_socket.bytesAvailable())
    {
        if (!m_isHeaderRed)
        {
            QByteArray header = m_socket.read(sizeof(int));
            m_bytesLeftInMessage = *(int*)header.data();
            m_isHeaderRed = true;
        }

        m_data.append(m_socket.read(m_bytesLeftInMessage));
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

void ExternalConnector::sendHandShake()
{
    boost::property_tree::ptree a_tree;
    a_tree.put("command", "handshake");
    a_tree.put("params.version", "0");
    std::ostringstream buffer;
    write_xml(buffer, a_tree);
    sendMessage(buffer.str());
}

void ExternalConnector::handshakeProc(boost::property_tree::ptree* params)
{
    bool isSuccess = params->get<bool>("success");
    if (isSuccess)
    { 
        emit connectionEstablished();
        m_active = true;
    }
    else
    {
        emit connectionErrorOccured(tr("Version is not supported"));
        m_socket.close();
    }
}

void ExternalConnector::sendPortList(const QList<QString>& list)
{
    boost::property_tree::ptree a_tree;
    a_tree.put("command", "ports");
    boost::property_tree::ptree& portlist = a_tree.add_child("params.ports", boost::property_tree::ptree{});
    for (QString a_port : list)
    {
        portlist.add("port", a_port.toStdString());
    }
    std::ostringstream buffer;
    write_xml(buffer, a_tree);
    sendMessage(buffer.str());
}
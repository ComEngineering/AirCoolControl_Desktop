#include "ExternalConnectionWindow.h"
#include "Configurator.h"
#include "externalconnector.h"

ExternalConnectionWindow::ExternalConnectionWindow(QWidget *parent)
    : QWidget(parent),
    m_connector(NULL)
{
    ui.setupUi(this);

    ui.ipEd0->setText(Configurator::getSettings("IP0").toString());
    ui.ipEd1->setText(Configurator::getSettings("IP1").toString());
    ui.ipEd2->setText(Configurator::getSettings("IP2").toString());
    ui.ipEd3->setText(Configurator::getSettings("IP3").toString());

    ui.disconnectButton->setEnabled(false);
    ui.externalStatusLabel->setText(tr("Disconnected"));

    connect(ui.connectButton, SIGNAL(clicked()), this, SLOT(externalConnect()));
    connect(ui.disconnectButton, SIGNAL(clicked()), this, SLOT(externalDisconnect()));
}

ExternalConnectionWindow::~ExternalConnectionWindow()
{

}

void ExternalConnectionWindow::setExternalComunicator(ExternalConnector * comunicator)
{
    m_connector = comunicator;
    connect(m_connector, SIGNAL(connectionEstablished()), this, SLOT(externalConnected()));
    connect(m_connector, SIGNAL(connectionBroken()), this, SLOT(externalDisconnected()));
}

void ExternalConnectionWindow::externalConnect()
{
    if (NULL == m_connector)
        return;

    QString ip;
    ip = ui.ipEd0->text() + '.' + ui.ipEd1->text() + '.' + ui.ipEd2->text() + '.' + ui.ipEd3->text();
    /* if (ip == "127.0.0.1")
    {
    setExternalStatus(tr("Lockalhost isn't available"),true);
    return;
    }*/
    m_connector->connectToRemote(ip);
    Configurator::setSettings("IP0", ui.ipEd0->text());
    Configurator::setSettings("IP1", ui.ipEd1->text());
    Configurator::setSettings("IP2", ui.ipEd2->text());
    Configurator::setSettings("IP3", ui.ipEd3->text());
}

void ExternalConnectionWindow::externalDisconnect()
{
    if (NULL != m_connector)
        m_connector->disconnectFromRemote();
}

void ExternalConnectionWindow::externalConnected()
{
    
}

void ExternalConnectionWindow::externalDisconnected()
{
    
}

void ExternalConnectionWindow::setExternalStatus(const QString& status, bool isError)
{
    if (isError)
    {
        ui.externalStatusLabel->setStyleSheet("QLabel { background-grey : white; color : red; }");
    }
    else
    {
        ui.externalStatusLabel->setStyleSheet("QLabel { background-grey : white; color : black; }");
    }
    ui.externalStatusLabel->setText(status);
}

void ExternalConnectionWindow::externalConnection(bool isConnected)
{
    ui.connectButton->setDisabled(isConnected);
    ui.disconnectButton->setEnabled(isConnected);
    setExternalStatus(isConnected ? tr("Connected") : tr("Disonnected"));
}

void ExternalConnectionWindow::externalConnection(QString err)
{
    setExternalStatus(tr("Connection error : ") + err, true);
}

void ExternalConnectionWindow::setExternalConnection(const QString& adress)
{

}

void ExternalConnectionWindow::dropExternalConnection()
{

}


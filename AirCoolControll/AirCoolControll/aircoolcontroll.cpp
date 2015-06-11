#include "aircoolcontroll.h"

AirCoolControll::AirCoolControll(QWidget *parent)
    : QMainWindow(parent),
    m_preferences(NULL)
{
    ui.setupUi(this);
    
    m_uartConnector = new UART_ConnectionWindow(this);
    m_uartConnectorMdi = new MdiSubWindowPermanent(m_uartConnector,this);
    
    (m_internetConnector = new ExternalConnectionWindow(this))->hide();

    m_comunicator = new Cooller_ModBusController(this);
    connect(m_comunicator, SIGNAL(newStatus(const QString&)), ui.statusBar, SLOT(showMessage(const QString&, int)));
  
   ///  Toolbox actions  ///////////////////////////////////////////////////////////////

    connect(ui.actionPreferences, SIGNAL(triggered(void)), this, SLOT(showPreferencesDialog()));
    connect(ui.actionConnect_to_device, SIGNAL(triggered(void)), this, SLOT(showConnectDialog()));
    connect(ui.actionDisconnect_from_UART, SIGNAL(triggered(void)), this, SLOT(showDisconnectDialog()));
    connect(ui.actionConnect_to_host, SIGNAL(triggered(void)), this, SLOT(showConnectToHostDialog()));

   /////////////////////////////////////////////////////////////////////////////////////
}

AirCoolControll::~AirCoolControll()
{
   
}

UART_ConnectionWindow * AirCoolControll::getUART_Configurator(void)
{
    return m_uartConnector;
}

void AirCoolControll::showPreferencesDialog()
{
    if (NULL == m_preferences)
    {
        PreferencesWindow*  preferences = new PreferencesWindow(this);
        m_preferences = ui.mdiArea->addSubWindow(preferences);
        m_preferences->setWindowFlags(Qt::WindowTitleHint);
        m_preferences->setWindowIcon(QPixmap(":/Images/preferences.png"));
        connect(preferences, SIGNAL(finished(void)), this, SLOT(hidePreferences()));
    }

    m_preferences->show();
}

void AirCoolControll::hidePreferences(void)
{
    m_preferences->hide();
}

void AirCoolControll::showConnectDialog()
{
    ui.mdiArea->addSubWindow(m_uartConnectorMdi);
    m_uartConnectorMdi->setWindowFlags(Qt::WindowCancelButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint);
    m_uartConnectorMdi->setWindowIcon(QPixmap(":/Images/connect.png"));
    m_uartConnectorMdi->show();
}

void AirCoolControll::showDisconnectDialog()
{

}

void AirCoolControll::showConnectToHostDialog()
{
    ui.mdiArea->addSubWindow(m_internetConnector);
    m_internetConnector->setWindowFlags(Qt::WindowCancelButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint);
    m_internetConnector->setWindowIcon(QPixmap(":/Images/connect.png"));
    m_internetConnector->show();
}

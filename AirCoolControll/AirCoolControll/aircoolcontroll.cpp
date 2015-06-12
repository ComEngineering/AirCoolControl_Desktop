#include "aircoolcontroll.h"

AirCoolControll::AirCoolControll(QWidget *parent)
    : QMainWindow(parent),
    m_preferences(NULL),
    m_uartConnector(this),
    m_internetConnector(this)
{
    ui.setupUi(this);

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
    return m_uartConnector.widget();
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
    ui.mdiArea->addSubWindow(m_uartConnector);
    m_uartConnector.operator QMdiSubWindow*()->setWindowIcon(QPixmap(":/Images/connect.png"));
    m_uartConnector.operator QMdiSubWindow*()->show();
}

void AirCoolControll::showDisconnectDialog()
{

}

void AirCoolControll::showConnectToHostDialog()
{
    ui.mdiArea->addSubWindow(m_internetConnector);
    m_internetConnector.operator QMdiSubWindow*()->setWindowIcon(QPixmap(":/Images/connect_to_host.png"));
    m_internetConnector.operator QMdiSubWindow*()->show();
}

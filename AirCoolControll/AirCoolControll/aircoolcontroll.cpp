#include "aircoolcontroll.h"
#include "Cooller_ModbusController.h"

AirCoolControll::AirCoolControll(QWidget *parent)
    : QMainWindow(parent),
    m_preferences(NULL),
    m_uartConnector(this),
    m_internetConnector(this)
{
    ui.setupUi(this);

    setWindowState(windowState() ^ Qt::WindowMaximized);

    m_comunicator = new Cooller_ModBusController(this);
    m_uartConnector.widget<UART_ConnectionWindow>()->setController(m_comunicator);
    connect(m_comunicator, SIGNAL(newStatus(const QString&)), ui.statusBar, SLOT(showMessage(const QString&, int)));

    m_uartConnector.container<UART_ConnectionWindow>()->setWindowIcon(QPixmap(":/Images/connect.png"));
    ui.mdiArea->addSubWindow(m_uartConnector.container<UART_ConnectionWindow>());

    m_internetConnector.container<ExternalConnectionWindow>()->setWindowIcon(QPixmap(":/Images/connect_to_host.png"));
    ui.mdiArea->addSubWindow(m_internetConnector.container<ExternalConnectionWindow>());

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

QMdiArea*    AirCoolControll::getMdiArea(void) const
{
    return ui.mdiArea;
}

UART_ConnectionWindow * AirCoolControll::getUART_Configurator(void) const
{
    return m_uartConnector.widget<UART_ConnectionWindow>();
}

ConnectionLog * AirCoolControll::getConnectionLog(void) const
{
    return ui.deviceExplorerContent;
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
    QSize mainWindowSize = ui.mdiArea->frameSize();
    QMdiSubWindow* container = m_uartConnector.container<UART_ConnectionWindow>();
    QSize connectorSize = container->frameSize();
    container->move(mainWindowSize.width() / 2 - connectorSize.width() / 2, mainWindowSize.height() / 2 - connectorSize.height() / 2);
    m_uartConnector.activate();
}

void AirCoolControll::showDisconnectDialog()
{

}

void AirCoolControll::showConnectToHostDialog()
{
    m_internetConnector.activate();
}

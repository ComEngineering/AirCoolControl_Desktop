#ifndef AIRCOOLCONTROLL_H
#define AIRCOOLCONTROLL_H

#include <QtWidgets/QMainWindow>
#include <qmdisubwindow.h>
#include "ui_aircoolcontroll.h"
#include "coolerstatewidget.h"
#include "PreferencesWindow.h"
#include "ExternalConnectionWindow.h"
#include "UART_ConnectionWindow.h"
#include "connectionlog.h"
#include "MdiContainer.h"

class Cooller_ModBusController;

class AirCoolControll : public QMainWindow
{
    Q_OBJECT

public:
    AirCoolControll(QWidget *parent = 0);
    ~AirCoolControll();

    UART_ConnectionWindow * getUART_Configurator(void) const;
    ConnectionLog * getConnectionLog(void) const;

public slots:
    void showDeviceLog();

private slots:
    void showPreferencesDialog();
    void showConnectDialog();
    void showDisconnectDialog();
    void showConnectToHostDialog();
    void hidePreferences();

private:
    Ui::AirCoolControllClass               ui;
    Cooller_ModBusController*              m_comunicator; 
    QMdiSubWindow*                         m_preferences;

    MdiContainer<UART_ConnectionWindow>    m_uartConnector;
    MdiContainer<ExternalConnectionWindow> m_internetConnector;
    MdiContainer<ConnectionLog>            m_connectionLog;
};

#endif // AIRCOOLCONTROLL_H

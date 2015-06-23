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
#include "UART_DisconnectWindow.h"

class Cooller_ModBusController;

class AirCoolControll : public QMainWindow
{
    Q_OBJECT

public:
    AirCoolControll(QWidget *parent = 0);
    ~AirCoolControll();

    UART_ConnectionWindow * getUART_Configurator(void) const;
    ConnectionLog * getConnectionLog(void) const;
    QMdiArea*    getMdiArea(void) const;

public slots:

private slots:
    void showPreferencesDialog();
    void showConnectDialog();
    void showDisconnectDialog();
    void showConnectToHostDialog();
    void newActiveWindow(QMdiSubWindow* w);

private:
    Ui::AirCoolControllClass   ui;
    Cooller_ModBusController*  m_comunicator; 
    PreferencesWindow*         m_preferences;

    UART_ConnectionWindow*     m_uartConnector;
    UART_DisconnectWindow*     m_uartDisconnector;
    ExternalConnectionWindow*  m_internetConnector;
};

#endif // AIRCOOLCONTROLL_H

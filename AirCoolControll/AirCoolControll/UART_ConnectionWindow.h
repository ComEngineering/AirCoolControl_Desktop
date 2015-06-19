#ifndef UART_CONNECTIONWINDOW_H
#define UART_CONNECTIONWINDOW_H

#include <QDialog>
#include "ui_UART_ConnectionWindow.h"
#include <QtSerialPort\qserialportinfo.h>
#include "Cooller_ModbusController.h"

class ExternalConnector;

class UART_ConnectionWindow : public QDialog
{
    Q_OBJECT

public:
    UART_ConnectionWindow(QWidget *parent = 0);
    ~UART_ConnectionWindow();

    void setController(Cooller_ModBusController * controller){
        m_controller = controller;
    }

    int  getCOMindex();
    void setCOMindex(int n);
    void setCOMlist(QList<QString>& list);

    void setError(const QString & errorString, bool critical = false);
    void clearError(void);

    void setExternalPorts(const QList<QString>& list);
    
private slots:
    void connectPressed(void);

private:
    Ui::UART_ConnectionWindow ui;
    Cooller_ModBusController * m_controller;
};

#endif // UART_CONNECTIONWINDOW_H

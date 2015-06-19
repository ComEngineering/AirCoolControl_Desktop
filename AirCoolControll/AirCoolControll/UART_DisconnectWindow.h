#ifndef UART_DISCONNECTWINDOW_H
#define UART_DISCONNECTWINDOW_H

#include <QDialog>
#include "ui_UART_DisconnectWindow.h"
#include "Cooller_ModbusController.h"

class UART_DisconnectWindow : public QDialog
{
    Q_OBJECT

public:
    UART_DisconnectWindow(Cooller_ModBusController* controller, QWidget *parent = 0);
    ~UART_DisconnectWindow();

private slots:
    void doRelease(void);

protected:
    void showEvent(QShowEvent * event);

private:
    Ui::UART_DisconnectWindow ui;
    Cooller_ModBusController* m_controller;
};

#endif // UART_DISCONNECTWINDOW_H

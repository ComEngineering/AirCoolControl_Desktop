#ifndef UART_CONNECTIONWINDOW_H
#define UART_CONNECTIONWINDOW_H

#include <QWidget>
#include "ui_UART_ConnectionWindow.h"
#include <QtSerialPort\qserialportinfo.h>


class ExternalConnector;

class UART_ConnectionWindow : public QWidget
{
    Q_OBJECT

public:
    UART_ConnectionWindow(QWidget *parent = 0);
    ~UART_ConnectionWindow();

    int  getCOMindex(void);
    void setCOMindex(int n);
    void setCOMlist(QList<QString>& list);

    int  getSpeed();
    int  getDeviceIndex();
    void setDeviceIndex(int n);

    void setError(const QString & errorString, bool critical = false);
    void clearError(void);

    void setExternalPorts(const QList<QString>& list);

signals:
    void speedChanged(int n);
    void portChanged(int n);
    void performConnection(int);

    private slots:
    void speedValueChanged(int);
    void portValueChanged(int);
    void connectPressed(void);

private:
    Ui::UART_ConnectionWindow ui;

};

#endif // UART_CONNECTIONWINDOW_H

#ifndef CONNECTIONLOG_H
#define CONNECTIONLOG_H

#include <QWidget>
#include <qbrush.h>
#include "ui_connectionlog.h"
#include "modbusuart_impl.h"

class ConnectedDeviceStorage;

class ConnectionLog : public QWidget
{
    Q_OBJECT

public:
    ConnectionLog(QWidget *parent = 0);
    ~ConnectionLog();

    void setDeviceList(ConnectedDeviceStorage* devices);
    void updateContent(void);

    void activateDevice(const QString& uart_name, int id);

private slots:
    void cellSelected(int row, int column);
    void removeConnection(void);
    void removeAllConnection(void);

private:
    void sellectionChanged(int n,const QBrush& brush);

private:
    Ui::ConnectionLog ui;
    ConnectedDeviceStorage* m_devices;
    int                     m_currentIndex;

    static const QBrush  s_selected; 
    static const QBrush  s_free; 
    static const QBrush  s_unsuported;
};

#endif // CONNECTIONLOG_H

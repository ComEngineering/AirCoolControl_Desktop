#ifndef CONNECTIONLOG_H
#define CONNECTIONLOG_H

#include <QWidget>
#include <qbrush.h>
#include "ui_connectionlog.h"
#include "modbusuart_impl.h"
#include "ConnectedDeviceStorage.h"

class ConnectionLog : public QWidget
{
    Q_OBJECT

public:
    ConnectionLog(QWidget *parent = 0);
    ~ConnectionLog();

    void setDeviceList(ConnectedDeviceStorage* devices);
    void updateContent(void);

private slots:
    void cellSelected(int row, int column);
    void removeConnection(void);
    void removeAllConnection(void);

private:
    void sellectionChanged(int n);

private:
    Ui::ConnectionLog ui;
    ConnectedDeviceStorage* m_devices;
    int                     m_currentIndex;

    static const QBrush  s_selected; 
    static const QBrush  s_free; 
};

#endif // CONNECTIONLOG_H

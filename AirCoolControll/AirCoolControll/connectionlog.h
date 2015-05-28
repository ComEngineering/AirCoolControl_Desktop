#ifndef CONNECTIONLOG_H
#define CONNECTIONLOG_H

#include <QWidget>
#include "ui_connectionlog.h"
#include "modbusuart_impl.h"
#include "ConnectedDeviceStorage.h"

class ConnectionLog : public QWidget
{
    Q_OBJECT

public:
    ConnectionLog(QWidget *parent = 0);
    ~ConnectionLog();

    void setDeviceList(const ConnectedDeviceStorage* devices);
    void updateContent(void);

public:
    Ui::ConnectionLog ui;
    const ConnectedDeviceStorage* m_devices;
};

#endif // CONNECTIONLOG_H

#ifndef CONNECTIONLOG_H
#define CONNECTIONLOG_H

#include <QWidget>
#include "ui_connectionlog.h"
#include "modbusuart_impl.h"

class ConnectionLog : public QWidget
{
    Q_OBJECT

public:
    ConnectionLog(QWidget *parent = 0);
    ~ConnectionLog();

    void setDeviceList(const DeviceInfoMap& map);

public:
    Ui::ConnectionLog ui;
};

#endif // CONNECTIONLOG_H

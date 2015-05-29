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

private slots:
    void cellSelected(int row, int column);

private:
    void sellectionChanged();

private:
    Ui::ConnectionLog ui;
    const ConnectedDeviceStorage* m_devices;
    int m_currentSellectedRaw;
};

#endif // CONNECTIONLOG_H

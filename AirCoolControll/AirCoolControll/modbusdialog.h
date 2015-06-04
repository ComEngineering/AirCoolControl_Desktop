#ifndef MODBUSDIALOG_H
#define MODBUSDIALOG_H

#include <QWidget>
#include "ui_modbusdialog.h"
#include <QtSerialPort\qserialportinfo.h>
#include "externalconnector.h"
#include "modbusuart_impl.h"

class ModBusDialog : public QWidget
{
    Q_OBJECT

public:
    ModBusDialog(QWidget *parent = 0);
    ~ModBusDialog();

    int  getCOMindex(void);
    void setCOMindex(int n);
    void setCOMlist(QList<QString>& list);

    void setExternalCommunicator(ExternalConnector* connector);
    void setExternalConnection(const QString& adress);
    void dropExternalConnection();
    void setExternalPorts(const QList<QString>& list);

    int  getSpeed();
    int  getDeviceIndex();
    void setDeviceIndex(int n);

    void setDeviceList(ConnectedDeviceStorage *storage);
    void refreshDeviceList(void);

    void setError(QString & errorString, bool critical = false);
    void clearError(void);

signals:
    void speedChanged(int n);
    void portChanged(int n);
    void deviceIDChanged(int n);

public slots:
    void connectionEstablished();
    void connectionBroken();
    void connectionErrorOccured(QString);

private slots:
    void newSpeed(int);
    void newPort(int);
    void newDevice(int);

private:
    Ui::ModBusDialog ui;

private:
    bool    m_isError;
};

#endif // MODBUSDIALOG_H

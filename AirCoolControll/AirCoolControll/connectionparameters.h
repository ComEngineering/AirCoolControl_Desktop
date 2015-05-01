#ifndef CONNECTIONPARAMETERS_H
#define CONNECTIONPARAMETERS_H

#include "ui_connectionparameters.h"
#include <QWidget>
#include <QtSerialPort\qserialportinfo.h>
#include "externalconnector.h"

class ExternalConnector;

class ConnectionParameters : public QWidget
{
    Q_OBJECT

public:
    ConnectionParameters(QWidget *parent = 0);
    ~ConnectionParameters();

    int getCOMindex(void);
    void setCOMindex(int n);
    void setCOMlist(QList<QSerialPortInfo>& list);

    void setExternalComunicator(ExternalConnector * comunicator);
    void externalConnection(bool isConnected);
    void externalConnection(QString err);

    void setExternalConnection(const QString& adress);
    void dropExternalConnection();
    void setExternalPorts(const QList<QString>& list);

    int getSpeed();
    int getDeviceIndex();
    void setDeviceIndex(int n);

    void setError(const QString & errorString,bool critical = false);
    void clearError(void);

    void setExternalStatus(const QString& status, bool isError = false);

signals:
    void speedChanged(int n);
    void portChanged(int n);
    void deviceIDChanged(int n);

private slots:
    void speedValueChanged(int);
    void portValueChanged(int);
    void IDValueChanged(int);
    void controllTabChanged(int);
    
    void externalConnect();
    void externalConnected();
    
    void externalDisconnect();
    void externalDisconnected();
    
private:
    Ui::ConnectionParameters ui;
    ExternalConnector *      m_connector;
};

#endif // CONNECTIONPARAMETERS_H

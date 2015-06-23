#ifndef EXTERNALCONNECTIONWINDOW_H
#define EXTERNALCONNECTIONWINDOW_H

#include <QDialog>
#include "ui_ExternalConnectionWindow.h"


class ExternalConnector;

class ExternalConnectionWindow : public QDialog
{
    Q_OBJECT

public:
    ExternalConnectionWindow(QWidget *parent = 0);
    ~ExternalConnectionWindow();

    void setExternalComunicator(ExternalConnector * comunicator);
    void externalConnection(bool isConnected);
    void externalConnection(QString err);

    void setExternalConnection(const QString& adress);
    void dropExternalConnection();

    void setExternalStatus(const QString& status, bool isError = false);

private slots:
    void externalConnect();
    void externalConnected();

    void externalDisconnect();
    void externalDisconnected();

private:
    Ui::ExternalConnectionWindow ui;
    ExternalConnector *      m_connector;
};

#endif // EXTERNALCONNECTIONWINDOW_H

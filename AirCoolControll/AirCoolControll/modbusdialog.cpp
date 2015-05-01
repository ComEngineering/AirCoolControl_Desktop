#include "modbusdialog.h"

ModBusDialog::ModBusDialog(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    connect(ui.parameters, SIGNAL(speedChanged(int)), this, SLOT(newSpeed(int)));
    connect(ui.parameters, SIGNAL(portChanged(int)), this, SLOT(newPort(int)));
    connect(ui.parameters, SIGNAL(deviceIDChanged(int)), this, SLOT(newDevice(int)));
}

ModBusDialog::~ModBusDialog()
{

}

int ModBusDialog::getCOMindex(void)
{
    return ui.parameters->getCOMindex();
}

void ModBusDialog::setCOMindex(int n)
{
    ui.parameters->setCOMindex(n);
}

void ModBusDialog::setCOMlist(QList<QSerialPortInfo>& list)
{
    ui.parameters->setCOMlist(list);
}

int ModBusDialog::getSpeed()
{
    return ui.parameters->getSpeed();
}

int ModBusDialog::getDeviceIndex()
{
    return ui.parameters->getDeviceIndex();
}

void ModBusDialog::setDeviceIndex(int n)
{
    return ui.parameters->setDeviceIndex(n);
}

void ModBusDialog::setError(QString & errorString,bool critical)
{
    ui.parameters->setError(errorString,critical);
}

void ModBusDialog::clearError(void)
{
    ui.parameters->clearError();
}

void ModBusDialog::newSpeed(int n)
{
    emit speedChanged(n);
}

void ModBusDialog::newPort(int n)
{
    emit portChanged(n);
}

void ModBusDialog::newDevice(int n)
{
    emit deviceIDChanged(n);
}

void ModBusDialog::setExternalCommunicator(ExternalConnector* connector)
{
    ui.parameters->setExternalComunicator(connector);
}

void ModBusDialog::connectionEstablished()
{
    ui.parameters->externalConnection(true);
}

void ModBusDialog::connectionBroken()
{
    ui.parameters->externalConnection(false);
}

void ModBusDialog::connectionErrorOccured(QString err)
{
    ui.parameters->externalConnection(err);
}

void ModBusDialog::setExternalConnection(const QString& adress)
{
    ui.parameters->setExternalConnection(adress);
}

void ModBusDialog::dropExternalConnection()
{
    ui.parameters->dropExternalConnection();
}

void ModBusDialog::setExternalPorts(const QList<QString>& list)
{
    ui.parameters->setExternalPorts(list);
}

void ModBusDialog::setDeviceList(const DeviceInfoMap& map)
{
    ui.log->setDeviceList(map);
}
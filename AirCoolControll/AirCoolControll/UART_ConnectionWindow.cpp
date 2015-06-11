#include "UART_ConnectionWindow.h"
#include "externalconnector.h"
#include "Configurator.h"

UART_ConnectionWindow::UART_ConnectionWindow(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    QList<qint32> boudsRates = QSerialPortInfo::standardBaudRates();
    for (int n : boudsRates)
    {
        ui.comboBoxSpeed->addItem(QString::number(n));
    }
    ui.comboBoxSpeed->setCurrentIndex(6); // 9600 bouds

    connect(ui.comboBoxSpeed, SIGNAL(currentIndexChanged(int)), this, SLOT(speedValueChanged(int)));
    connect(ui.UARTconnectButton, SIGNAL(clicked(void)), this, SLOT(connectPressed(void)));
    connect(ui.comboBoxCOM, SIGNAL(currentIndexChanged(int)), this, SLOT(portValueChanged(int)));
}

UART_ConnectionWindow::~UART_ConnectionWindow()
{

}

int UART_ConnectionWindow::getCOMindex(void)
{
    return  ui.comboBoxCOM->currentIndex();
}

void UART_ConnectionWindow::setCOMindex(int n)
{
    if (n < ui.comboBoxCOM->count())
        ui.comboBoxCOM->setCurrentIndex(n);
}

void UART_ConnectionWindow::setCOMlist(QList<QString>& list)
{
    ui.comboBoxCOM->clear();
    for (auto info : list)
    {
        ui.comboBoxCOM->addItem(info);
    }
}


int UART_ConnectionWindow::getSpeed()
{
    return ui.comboBoxSpeed->currentIndex();
}

int UART_ConnectionWindow::getDeviceIndex()
{
    return ui.spinBoxID->value();
}

void UART_ConnectionWindow::setError(const QString & errorString, bool critical)
{
    ui.comboBoxCOM->setDisabled(critical);
    ui.comboBoxSpeed->setDisabled(critical);
    ui.spinBoxID->setDisabled(critical);

    ui.communicationErrorLabel->setText(errorString);
    ui.communicationErrorLabel->setStyleSheet("QLabel { background-grey : white; color : red; }");
    ui.communicationErrorLabel->setHidden(false);
}

void UART_ConnectionWindow::clearError(void)
{
    ui.comboBoxCOM->setEnabled(true);
    ui.comboBoxSpeed->setEnabled(true);
    ui.spinBoxID->setEnabled(true);
    ui.communicationErrorLabel->setHidden(true);
}

void UART_ConnectionWindow::speedValueChanged(int n)
{
    emit speedChanged(n);
}

void UART_ConnectionWindow::connectPressed(void)
{
    emit performConnection(ui.spinBoxID->value());
}

void UART_ConnectionWindow::portValueChanged(int n)
{
    emit portChanged(n);
}

void UART_ConnectionWindow::setDeviceIndex(int n)
{
    ui.spinBoxID->setValue(n);
}

void UART_ConnectionWindow::setExternalPorts(const QList<QString>& list)
{

}


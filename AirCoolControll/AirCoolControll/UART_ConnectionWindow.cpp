#include "UART_ConnectionWindow.h"
#include "Configurator.h"
#include "aircoolcontroll.h"

UART_ConnectionWindow::UART_ConnectionWindow(QWidget *parent)
    : QWidget(parent),
    m_controller(NULL)
{
    ui.setupUi(this);

    QList<qint32> boudsRates = QSerialPortInfo::standardBaudRates();
    for (int n : boudsRates)
    {
        ui.comboBoxSpeed->addItem(QString::number(n));
    }
    ui.comboBoxSpeed->setCurrentIndex(6); // 9600 bouds

    connect(ui.UARTconnectButton, SIGNAL(clicked(void)), this, SLOT(connectPressed(void)));
}

UART_ConnectionWindow::~UART_ConnectionWindow()
{

}

int UART_ConnectionWindow::getCOMindex()
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
    if (ui.comboBoxCOM->count() > 0)
        ui.UARTconnectButton->setEnabled(true);
}

void UART_ConnectionWindow::setError(const QString & errorString, bool critical)
{
    ui.comboBoxCOM->setDisabled(critical);
    ui.comboBoxSpeed->setDisabled(critical);
    ui.spinBoxID->setDisabled(critical);
    ui.UARTconnectButton->setDisabled(critical);

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

void UART_ConnectionWindow::connectPressed(void)
{
    if (m_controller)
        m_controller->performConnection(ui.comboBoxCOM->currentIndex(), ui.spinBoxID->value(), ui.comboBoxSpeed->currentIndex());
    parentWidget()->close();
}

void UART_ConnectionWindow::setExternalPorts(const QList<QString>& list)
{

}


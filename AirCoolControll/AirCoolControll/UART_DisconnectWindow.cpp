#include "UART_DisconnectWindow.h"

UART_DisconnectWindow::UART_DisconnectWindow(Cooller_ModBusController* controller,QWidget *parent)
    : QDialog(parent),
    m_controller(controller)
{
    ui.setupUi(this);

    connect(ui.releaseButton, SIGNAL(clicked()), this, SLOT(doRelease()));

    std::vector<std::pair<QString, bool>> list = m_controller->getDriverList();
    for (auto i : list)
    {
        ui.portList->addItem(i.first);
    }
}

UART_DisconnectWindow::~UART_DisconnectWindow()
{

}

void UART_DisconnectWindow::showEvent(QShowEvent * event)
{
    std::vector<std::pair<QString, bool>> list = m_controller->getDriverList();
    for (auto i : list)
    {
        ui.portList->addItem(i.first);
    }
}

void UART_DisconnectWindow::doRelease(void)
{
    int currentIndex = ui.portList->currentIndex();
    if (currentIndex == -1)
        return;

    m_controller->releaseDriverWithName(ui.portList->itemText(currentIndex));
}
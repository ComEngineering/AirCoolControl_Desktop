#include "UART_DisconnectWindow.h"
#include <qmessagebox.h>

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
    ui.portList->clear();
    for (auto i : list)
    {
        ui.portList->addItem(i.first);
    }
}

void UART_DisconnectWindow::doRelease(void)
{
    close();
    int currentIndex = ui.portList->currentIndex();
    if (currentIndex == -1)
        return;

    std::vector<QString> list;
    m_controller->getDevicesConnectedToDriver(ui.portList->currentText(), list);
    
    if (!list.empty())
    {
        QMessageBox allert;
        allert.setText(tr("This action is going to break down following connections:"));
        QString visualList;
        for (auto i : list)
            visualList += i + '\n';
        visualList.truncate(visualList.length() - 1);
        allert.setInformativeText(visualList);
        allert.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        allert.setDefaultButton(QMessageBox::Cancel);

        int ret = allert.exec();

        if (ret == QMessageBox::Cancel)
        {
            return;
        }  
    }
    
    m_controller->releaseDriverWithName(ui.portList->itemText(currentIndex));
}
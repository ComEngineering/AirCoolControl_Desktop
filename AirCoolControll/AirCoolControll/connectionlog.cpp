#include "connectionlog.h"

ConnectionLog::ConnectionLog(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

ConnectionLog::~ConnectionLog()
{

}

void ConnectionLog::setDeviceList(const ConnectedDeviceStorage* devices)
{
    m_devices = devices;
}

void ConnectionLog::updateContent(void)
{
    /*ui.tableWidget->setRowCount(m_devices.size());
    int currentRow = 0;
    for (std::pair<int,DeviceInfoShared> info : map)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(QString::number(info.first));
        newItem->setFlags(Qt::ItemIsEnabled);
        ui.tableWidget->setItem(currentRow, 0, newItem);

        newItem = new QTableWidgetItem(info.second->m_vendor);
        newItem->setFlags(Qt::ItemIsEnabled);
        ui.tableWidget->setItem(currentRow, 1, newItem);

        newItem = new QTableWidgetItem(info.second->m_product);
        newItem->setFlags(Qt::ItemIsEnabled);
        ui.tableWidget->setItem(currentRow, 2, newItem);

        newItem = new QTableWidgetItem(info.second->m_version);
        newItem->setFlags(Qt::ItemIsEnabled);
        ui.tableWidget->setItem(currentRow, 3, newItem);

        currentRow++;
    }*/
}
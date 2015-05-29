#include "connectionlog.h"

ConnectionLog::ConnectionLog(QWidget *parent)
    : QWidget(parent),
    m_currentSellectedRaw(-1)
{
    ui.setupUi(this);

   // ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableWidget->resizeColumnsToContents();

    connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(cellSelected(int, int)));
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
    ui.tableWidget->setRowCount(m_devices->size());
    int currentRow = 0;

    for (DeviceInfoShared info : *m_devices)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(info->getUART() + ':' + QString::number(info->getID()));
        newItem->setFlags(Qt::ItemIsEnabled);
        ui.tableWidget->setItem(currentRow, 0, newItem);

        newItem = new QTableWidgetItem(info->getVendor());
        newItem->setFlags(Qt::ItemIsEnabled);
        ui.tableWidget->setItem(currentRow, 1, newItem);

        newItem = new QTableWidgetItem(info->getProduct());
        newItem->setFlags(Qt::ItemIsEnabled);
        ui.tableWidget->setItem(currentRow, 2, newItem);

        newItem = new QTableWidgetItem(QString(info->getVersion()));
        newItem->setFlags(Qt::ItemIsEnabled);
        ui.tableWidget->setItem(currentRow, 3, newItem);

        newItem = new QTableWidgetItem(); 
        if (!info->getExplorer())
        {
            newItem->setText(tr("Unsupported device"));
        }
        else
        {
            QString status = info->getExplorer()->errorString();
            newItem->setText(status);
        }
        newItem->setFlags( Qt::ItemIsEnabled);
        ui.tableWidget->setItem(currentRow, 4, newItem);
       
        if (info->isActive())
            m_currentSellectedRaw = currentRow;

        currentRow++;
    }
    if (m_currentSellectedRaw < 0 || m_currentSellectedRaw >= currentRow)
        m_currentSellectedRaw = 0;
}

void ConnectionLog::cellSelected(int row, int column)
{
    m_currentSellectedRaw = row;
    sellectionChanged();
}

void ConnectionLog::sellectionChanged()
{
    int n = 0;
    for (std::list<DeviceInfoShared>::const_iterator it = m_devices->begin(); it != m_devices->end(); it++, n++)
    {
        (*it)->setActive(n == m_currentSellectedRaw);
    }
}
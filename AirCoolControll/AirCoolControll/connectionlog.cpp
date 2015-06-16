#include "connectionlog.h"

const QBrush ConnectionLog::s_selected = QBrush(Qt::yellow);
const QBrush ConnectionLog::s_free = QBrush(Qt::white);

ConnectionLog::ConnectionLog(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

   // ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableWidget->resizeColumnsToContents();

    connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(cellSelected(int, int)));
    connect(ui.disconnectButton, SIGNAL(clicked()), this, SLOT(removeConnection()));
    connect(ui.clearButton, SIGNAL(clicked()), this, SLOT(removeAllConnection()));
}

ConnectionLog::~ConnectionLog()
{

}

void ConnectionLog::setDeviceList(ConnectedDeviceStorage* devices)
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

        currentRow++;
    }
   // sellectionChanged();
}

void ConnectionLog::cellSelected(int row, int column)
{
    m_devices->setActiveIndex(row);
    sellectionChanged(row);
}

void ConnectionLog::sellectionChanged(int n)
{
    int nsel = n;
    int nc = ui.tableWidget->rowCount();
    if (nsel == -1 || nsel >= nc)
        return;

    for (int y = 0; y < nc; y++)
    {
        for (int x = 0; x < ui.tableWidget->columnCount(); x++)
        {
            QTableWidgetItem* a_item = ui.tableWidget->item(y, x);
            if (y == nsel)
                a_item->setBackground(s_selected);
            else
                a_item->setBackground(s_free);
        }
    }
}

void ConnectionLog::removeConnection(void)
{
    if (m_devices)
        m_devices->removeDeviceFromList(ConnectedDeviceStorage::DISCONNECT_CURRENT);
}

void ConnectionLog::removeAllConnection(void)
{
    if (m_devices)
        m_devices->removeDeviceFromList(ConnectedDeviceStorage::DISCONNECT_ALL);
}
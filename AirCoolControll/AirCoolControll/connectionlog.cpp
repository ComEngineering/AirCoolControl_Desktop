#include "connectionlog.h"
#include "ConnectedDeviceStorage.h"

const QBrush ConnectionLog::s_selected = QBrush(Qt::yellow);
const QBrush ConnectionLog::s_free = QBrush(Qt::white);
const QBrush ConnectionLog::s_unsuported = QBrush(Qt::gray);

ConnectionLog::ConnectionLog(QWidget *parent)
    : QWidget(parent),
    m_currentIndex(-1)
{
    ui.setupUi(this);

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
        DeviceExplorerShared a_device = info->getExplorer();
        if (!a_device)
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

        if (!a_device)
            sellectionChanged(currentRow, s_unsuported);

        currentRow++;
    }
}

void ConnectionLog::cellSelected(int row, int column)
{
    if (m_devices->setActiveIndex(row))
        sellectionChanged(row, s_selected);
}

void ConnectionLog::sellectionChanged(int n, const QBrush& brush)
{
    int nc = ui.tableWidget->rowCount();
    if (n == -1 || n >= nc)
        return;

    m_currentIndex = n;
    for (int y = 0; y < nc; y++)
    {
        for (int x = 0; x < ui.tableWidget->columnCount(); x++)
        {
            QTableWidgetItem* a_item = ui.tableWidget->item(y, x);
            if (y == n)
                a_item->setBackground(brush);
            else
                a_item->setBackground(s_free);
        }
    }
}

void ConnectionLog::removeConnection(void)
{
    if (m_devices)
        m_devices->removeDeviceFromList(m_currentIndex);
    updateContent();
}

void ConnectionLog::removeAllConnection(void)
{
    if (m_devices)
        m_devices->removeDeviceFromList(ConnectedDeviceStorage::DISCONNECT_ALL);
    updateContent();
}

void ConnectionLog::activateDevice(const QString& uart_name, int id)
{
    int n = m_devices->findDeviceIndex(uart_name, id);
    if (-1 != n)
        sellectionChanged(n, s_selected);
}
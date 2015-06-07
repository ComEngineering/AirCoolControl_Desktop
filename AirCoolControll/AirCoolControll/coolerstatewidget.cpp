#include "coolerstatewidget.h"

CoolerStateWidget::CoolerStateWidget(QWidget *parent)
    : QWidget(parent)
{ 
    ui.setupUi(this);

    ui.inputParametersTable->setSortingEnabled(false);

    m_tables[ConfigMap::INPUT_REGISTER] = ui.inputParametersTable;
    m_tables[ConfigMap::OUTPUT_REGISTER] = ui.outputParametersTable;
    m_tables[ConfigMap::COIL] = ui.coilsTable;
    connect(ui.outputParametersTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(registerSet(QTableWidgetItem *)));
    connect(ui.coilsTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(registerSet(QTableWidgetItem *)));
}

CoolerStateWidget::~CoolerStateWidget()
{

}

void CoolerStateWidget::setParameterList(const std::vector<std::pair<std::string, std::string>>& list, ConfigMap::RegisterType type)
{
    Qt::ItemFlags f = Qt::ItemIsEnabled;
    
    switch (type)
    {
    case ConfigMap::INPUT_REGISTER:
        break;
    case ConfigMap::OUTPUT_REGISTER:
        f |= Qt::ItemIsEditable;
        break;
    case ConfigMap::COIL:
        f |= Qt::ItemIsEditable;
        break;
    }

    m_tables[type]->setRowCount(list.size());

    int currentRow = 0;
    for (std::pair<std::string, std::string> a_line : list)
    {
        QTableWidgetItem* newItem = new QTableWidgetItem(QString::fromStdString(a_line.second));
        newItem->setFlags(Qt::ItemIsEnabled);
        m_tables[type]->setItem(currentRow, 1, newItem);

        newItem = new QTableWidgetItem(QString());
        newItem->setFlags(f);
        newItem->setData(Qt::UserRole,QVariant(QString::fromStdString(a_line.first)));
        newItem->setData(Qt::UserRole + 1, QVariant(static_cast<quint16>(type)));
        m_tables[type]->setItem(currentRow, 0, newItem);
        currentRow++;
    }
    
}

void CoolerStateWidget::updateParameter(int n, int value, ConfigMap::RegisterType type)
{
    m_tables[type]->blockSignals(true);
    
    QTableWidgetItem *aItem = m_tables[type]->item(n, 0);
    if(NULL != aItem)
        aItem->setText(QString::number(value));
    m_tables[type]->blockSignals(false);
}

void CoolerStateWidget::registerSet(QTableWidgetItem *item)
{
    QString text = item->data(Qt::DisplayRole).toString();
    bool ok;
    int d = text.toInt(&ok);
    if (ok)
    {
        QString name = item->data(Qt::UserRole).toString();
        int registerType  = item->data(Qt::UserRole + 1).toInt();
        emit newRegisterValue(registerType, name, d);
    }
}

void CoolerStateWidget::clear()
{
    for (QTableWidget* a_widget : m_tables)
        while ( a_widget->rowCount())
            a_widget->removeRow(0);
}
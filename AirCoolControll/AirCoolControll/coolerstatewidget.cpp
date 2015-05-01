#include "coolerstatewidget.h"


CoolerStateWidget::CoolerStateWidget(QWidget *parent)
    : QWidget(parent)
{ 
    ui.setupUi(this);

    ui.inputParametersTable->setSortingEnabled(false);
    connect(ui.outputParametersTable, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(registerSet(QStandardItem *)));
}

CoolerStateWidget::~CoolerStateWidget()
{

}

void CoolerStateWidget::setParameterList(const std::vector<std::pair<std::string, std::string>>& list,bool isInput)
{
    QTableWidget * widget = isInput ? ui.inputParametersTable : ui.outputParametersTable;

    Qt::ItemFlags f;
    if (isInput)
        f = Qt::ItemIsEnabled;
    else
        f = Qt::ItemIsEnabled | Qt::ItemIsEditable;

    widget->setRowCount(list.size());
    int currentRow = 0;
    for (std::pair<std::string, std::string> a_line : list)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(a_line.first));
        newItem->setFlags(Qt::ItemIsEnabled);
        widget->setItem(currentRow, 0, newItem);
        
        newItem = new QTableWidgetItem(QString::fromStdString(a_line.second));
        newItem->setFlags(Qt::ItemIsEnabled);
        widget->setItem(currentRow, 2, newItem);

        newItem = new QTableWidgetItem(QString());
        newItem->setFlags(f);
        widget->setItem(currentRow, 1, newItem);
        currentRow++;
    }
}

void CoolerStateWidget::updateParameter(int n, int value, bool isInput)
{
    QTableWidget * widget = isInput ? ui.inputParametersTable : ui.outputParametersTable;
    QTableWidgetItem *aItem = widget->item(n, 1);
    if(NULL != aItem)
        aItem->setText(QString::number(value));
}

void CoolerStateWidget::registerSet(QStandardItem *item)
{
    QString text = item->data(Qt::UserRole).toString();
    bool ok;
    int d = text.toInt(&ok);
    if (ok)
    {
        QString name = ui.outputParametersTable->item(0, 0)->data(Qt::UserRole).toString();
        emit newRegisterValue(name, d);
    }
}

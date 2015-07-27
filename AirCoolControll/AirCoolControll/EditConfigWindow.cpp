#include "EditConfigWindow.h"

EditConfigWindow::EditConfigWindow(ConfigMapShared a_map, QWidget *parent)
    : QDialog(parent),
    m_config(a_map)
{
    ui.setupUi(this);

    connect(ui.button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui.button_save, SIGNAL(clicked()), this, SLOT(saveConfig()));

    connect(ui.button_add_coils, SIGNAL(clicked()), this, SLOT(addCoil()));
    connect(ui.button_add_input, SIGNAL(clicked()), this, SLOT(addInput()));
    connect(ui.button_add_output, SIGNAL(clicked()), this, SLOT(addOutput()));

    connect(ui.button_edit_coils, SIGNAL(clicked()), this, SLOT(editCoil()));
    connect(ui.button_edit_input, SIGNAL(clicked()), this, SLOT(editInput()));
    connect(ui.button_edit_output, SIGNAL(clicked()), this, SLOT(editOutput()));

    connect(ui.button_delete_coils, SIGNAL(clicked()), this, SLOT(deleteCoil()));
    connect(ui.button_delete_input, SIGNAL(clicked()), this, SLOT(deleteInput()));
    connect(ui.button_delete_output, SIGNAL(clicked()), this, SLOT(deleteOutput()));

    connect(ui.table_input, SIGNAL(cellClicked(int, int)), this, SLOT(cellSelectedInput(int, int)));
    connect(ui.table_output, SIGNAL(cellClicked(int, int)), this, SLOT(cellSelectedOutput(int, int)));
    connect(ui.table_coils, SIGNAL(cellClicked(int, int)), this, SLOT(cellSelectedCoils(int, int)));

    m_map = a_map->m_map;
    m_tables[ConfigMap::INPUT_REGISTER] = ui.table_input;
    m_tables[ConfigMap::OUTPUT_REGISTER] = ui.table_output;
    m_tables[ConfigMap::COIL] = ui.table_coils;

    for (int& a : m_currentIndex)
        a = 0;

    initTab(ConfigMap::INPUT_REGISTER);
    initTab(ConfigMap::OUTPUT_REGISTER);
    initTab(ConfigMap::COIL);
}

EditConfigWindow::~EditConfigWindow()
{

}

void EditConfigWindow::initTab(ConfigMap::RegisterType type)
{
    int rowCount = 0;
    for (const auto& it : m_map)
    {
        if (it.second.m_type == type)
        {
            rowCount++;
        }
    }
    m_tables[type]->setRowCount(rowCount);
    
    int currentRow = 0;
    for (const auto& it : m_map)
    {
        if (it.second.m_type == type)
        {
            QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(it.first));
            newItem->setFlags(Qt::ItemIsEnabled);
            m_tables[type]->setItem(currentRow, 0, newItem);

            newItem = new QTableWidgetItem(QString::fromStdString(it.second.m_description));
            newItem->setFlags(Qt::ItemIsEnabled);
            m_tables[type]->setItem(currentRow++, 1, newItem);
        }
    }
    int selectedItem = m_currentIndex[type];
    sellectionChanged(selectedItem, selectedItem, m_tables[type]);
}

void EditConfigWindow::cellSelectedInput(int row, int collumn)
{
    sellectionChanged(m_currentIndex[ConfigMap::INPUT_REGISTER],row, ui.table_input);
    m_currentIndex[ConfigMap::INPUT_REGISTER] = row;
}

void EditConfigWindow::cellSelectedOutput(int row, int collumn)
{
    sellectionChanged(m_currentIndex[ConfigMap::OUTPUT_REGISTER], row, ui.table_output);
    m_currentIndex[ConfigMap::OUTPUT_REGISTER] = row;
}

void EditConfigWindow::cellSelectedCoils(int row, int collumn)
{
    sellectionChanged(m_currentIndex[ConfigMap::COIL], row, ui.table_coils);
    m_currentIndex[ConfigMap::COIL] = row;
}

void EditConfigWindow::sellectionChanged(int oldIndex, int newIndex, QTableWidget* table)
{
    int nc = table->rowCount();
    if (newIndex == -1 || newIndex >= nc)
        return;

    for (int x = 0; x < table->columnCount(); x++)
    {
        QTableWidgetItem* a_item = table->item(oldIndex, x);
        a_item->setBackground(QBrush(Qt::white));
        a_item = table->item(newIndex, x);
        a_item->setBackground(QBrush(Qt::yellow));
    }
    table->update();
}

void EditConfigWindow::saveConfig()
{
    m_config->m_map = m_map;
    done(1);
}

void EditConfigWindow::addCoil()
{

}

void EditConfigWindow::addInput()
{

}

void EditConfigWindow::addOutput()
{

}

void EditConfigWindow::editCoil()
{

}

void EditConfigWindow::editInput()
{

}

void EditConfigWindow::editOutput()
{

}

void EditConfigWindow::deleteCoil()
{

}

void EditConfigWindow::deleteInput()
{

}

void EditConfigWindow::deleteOutput()
{

}
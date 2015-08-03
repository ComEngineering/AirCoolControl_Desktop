#include "EditConfigWindow.h"
#include "NewParameterNameDialog.h"
#include "EditParameterDialog.h"

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

    connect(ui.table_input, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(cellEditInput(int, int)));
    connect(ui.table_output, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(cellEditOutput(int, int)));
    connect(ui.table_coils, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(cellEditCoils(int, int)));

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

void EditConfigWindow::cellEditInput(int row, int collumn)
{
    cellSelectedInput(row, collumn);
    editInput();
}

void EditConfigWindow::cellEditOutput(int row, int collumn)
{
    cellSelectedOutput(row, collumn);
    editOutput();
}

void EditConfigWindow::cellEditCoils(int row, int collumn)
{
    cellSelectedCoils(row, collumn);
    editCoil();
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
    m_config->save();
    done(1);
}

void EditConfigWindow::addNewParameter(ConfigMap::RegisterType type)
{
    ConfigMap::Parameter newParameter;
    newParameter.m_type = type;
    QString newName;
    if (!getNewName(newName))
        return;

    if (editParameter(newParameter))
    {
        m_config->setNewParameter(newName.toStdString(), newParameter);
        m_tables[type]->clearContents();
        m_map = m_config->m_map;
        initTab(type);
    }
}

void EditConfigWindow::addCoil()
{
    addNewParameter(ConfigMap::COIL);
}

void EditConfigWindow::addInput()
{
    addNewParameter(ConfigMap::INPUT_REGISTER);
}

void EditConfigWindow::addOutput()
{
    addNewParameter(ConfigMap::OUTPUT_REGISTER);
}

void EditConfigWindow::editCoil()
{
    editParameter(ConfigMap::COIL);
}

void EditConfigWindow::editInput()
{
    editParameter(ConfigMap::INPUT_REGISTER);
}

void EditConfigWindow::editOutput()
{
    editParameter(ConfigMap::OUTPUT_REGISTER);
}

void EditConfigWindow::deleteCoil()
{
    deleteParameter(ConfigMap::COIL);
}

void EditConfigWindow::deleteInput()
{
    deleteParameter(ConfigMap::INPUT_REGISTER);
}

void EditConfigWindow::deleteOutput()
{
    deleteParameter(ConfigMap::OUTPUT_REGISTER);
}

void EditConfigWindow::editParameter(ConfigMap::RegisterType type)
{
    QTableWidgetItem * ti = m_tables[type]->item(m_currentIndex[type], 0);
    QString id = ti->text();
    ConfigMap::Parameter newParameter = m_config->findParameter(id.toStdString())->second;
    if (editParameter(newParameter))
    {
        m_config->setNewParameter(id.toStdString(), newParameter);
    }
}

void EditConfigWindow::deleteParameter(ConfigMap::RegisterType type)
{
    QTableWidgetItem * ti = m_tables[type]->item(m_currentIndex[type], 0);
    QString id = ti->text();
    m_config->deleteParameterWithName(id.toStdString());
}

bool EditConfigWindow::editParameter(ConfigMap::Parameter& parameter)
{
    EditParameterDialog dialog(parameter, this);

    return dialog.exec() == 1;
}

bool EditConfigWindow::getNewName(QString& newName)
{
    bool rc = false;
    std::set<QString> existingNames;
    for (const auto& it : m_config->m_map)
        existingNames.insert(QString::fromStdString(it.first));
    
    NewParameterNameDialog nameDialog(existingNames, this);
    if (nameDialog.exec() == 1)
    {
        newName = nameDialog.getNewName();
        rc = true;
    }

    return rc;
}
#include "EditParameterDialog.h"

EditParameterDialog::EditParameterDialog(ConfigMap::Parameter& parameter,QDialog *parent)
    : QDialog(parent),
    m_editedParameter(parameter),
    m_enumCurrentIndex(-1),
    m_errorCurrentIndex(-1)
{
    ui.setupUi(this);

    ui.lineEdit_description->setText(QString::fromStdString(m_editedParameter.m_description));
    
    ui.spinBox_register->setValue(m_editedParameter.m_registerNumber);
    
    ui.checkBox->setChecked(m_editedParameter.m_isBool);
    connect(ui.checkBox, SIGNAL(stateChanged(int)), this, SLOT(newBitState(int)));
    ui.spinBox_bitNumber->setEnabled(m_editedParameter.m_isBool);

    ui.spinBox_bitNumber->setValue(m_editedParameter.m_bitNumber);
   
    ui.spinBox_minValue->setValue(m_editedParameter.m_minValue);
   
    ui.spinBox_maxValue->setValue(m_editedParameter.m_maxValue);

    ui.comboBox_decodeMethod->addItems(QString("None,AT").split(','));
    
    connect(ui.button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui.button_ok, SIGNAL(clicked()), this, SLOT(okClicked()));
    connect(ui.button_addErrorDetector, SIGNAL(clicked()), this, SLOT(addError()));
    connect(ui.button_addEnum, SIGNAL(clicked()), this, SLOT(addEnum()));
    connect(ui.button_deleteErrorDetector, SIGNAL(clicked()), this, SLOT(deleteError()));
    connect(ui.button_deleteEnum, SIGNAL(clicked()), this, SLOT(deleteEnum()));
    connect(ui.table_enum, SIGNAL(cellClicked(int, int)), this, SLOT(cellSelectedEnum(int, int)));
    connect(ui.table_error, SIGNAL(cellClicked(int, int)), this, SLOT(cellSelectedError(int, int)));
        
    initTables();
}

EditParameterDialog::~EditParameterDialog()
{

}


void EditParameterDialog::newBitState(int n)
{
    bool is_bool = (bool)n;  
    ui.spinBox_bitNumber->setEnabled(is_bool);
}

void EditParameterDialog::okClicked()
{
    const static char* methods[] = { "", "AT" };
    
    m_editedParameter.m_description = ui.lineEdit_description->text().toStdString();
    m_editedParameter.m_registerNumber = ui.spinBox_register->value();
    m_editedParameter.m_bitNumber = ui.spinBox_bitNumber->value();
    m_editedParameter.m_minValue = ui.spinBox_minValue->value();
    m_editedParameter.m_maxValue = ui.spinBox_maxValue->value();
    m_editedParameter.m_isBool = ui.checkBox->isChecked();
    
    int n = ui.comboBox_decodeMethod->currentIndex();
    m_editedParameter.m_decodeMethod = methods[n < 0 ? 0 : n];

    int tableSize = ui.table_enum->rowCount();
    if (0 != tableSize)
    {
        m_editedParameter.m_enumeration.clear();
        for (int i = 0; i < tableSize; i++)
        {
            QString name = ui.table_enum->item(i, 1)->text();
            if (QString() == name)
                continue;
            QSpinBox* enumSpin = qobject_cast<QSpinBox*>(ui.table_enum->cellWidget(i, 0));
            m_editedParameter.m_enumeration.emplace_back(std::pair<std::string, int>(name.toStdString(), enumSpin->value()));
        }
    }

    tableSize = ui.table_error->rowCount();
    if (0 != tableSize)
    {
        m_editedParameter.m_errorDetector.clear();
        for (int i = 0; i < tableSize; i++)
        {
            QString name = ui.table_error->item(i, 2)->text();
            if (QString() == name)
                continue;
            QSpinBox* spin = qobject_cast<QSpinBox*>(ui.table_error->cellWidget(i, 1));
            QComboBox* combo = qobject_cast<QComboBox*>(ui.table_error->cellWidget(i, 0));
            const static char* enumDescriptions[] = { "EQ", "GT", "LT", "GTE", "LTE", "AND", "XOR" };
            m_editedParameter.m_errorDetector.addError( enumDescriptions[combo->currentIndex()], name.toStdString(), spin->value());
        }
    }

    done(1);
}

void EditParameterDialog::initTables(void)
{
    ui.button_deleteEnum->setEnabled(false);
    ui.button_deleteErrorDetector->setEnabled(false);

    ui.table_error->setSelectionMode(QTableWidget::NoSelection);
    ui.table_enum->setSelectionMode(QTableWidget::NoSelection);

    for (const auto& it : m_editedParameter.m_enumeration)
    {
        addEnumLine(it.first, it.second);
    }

    for (int i = 0; i < m_editedParameter.m_errorDetector.size(); i++)
    {
        addErrorLine(m_editedParameter.m_errorDetector[i]);
    }
}

void EditParameterDialog::addErrorLine(const ConfigMap::ErrorDetector::Error& error)
{
    static const QString detectionTypes("==,>,<,>=,<=,AND,XOR");

    int currentRow = ui.table_error->rowCount();
    ui.table_error->setRowCount(currentRow + 1);
    
    QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(error.m_description));
    newItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
    ui.table_error->setItem(currentRow, 2, newItem);

    QSpinBox* spin = new QSpinBox(ui.table_error);
    spin->setValue(error.m_value);
    ui.table_error->setCellWidget(currentRow, 1, spin);

    QComboBox* combo = new QComboBox(ui.table_error);
    combo->addItems(detectionTypes.split(','));
    combo->setCurrentIndex(error.m_type);
    ui.table_error->setCellWidget(currentRow, 0, combo);

    ui.table_error->update();
}

void EditParameterDialog::addEnumLine(std::string showAs, int value)
{
    int currentRow = ui.table_enum->rowCount();
    ui.table_enum->setRowCount(currentRow + 1);
    
    QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(showAs));
    newItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
    ui.table_enum->setItem(currentRow, 1, newItem);

    QSpinBox* spin = new QSpinBox(ui.table_enum);
    spin->setValue(value);
    ui.table_enum->setCellWidget(currentRow, 0, spin);

    ui.table_enum->update();
}

void EditParameterDialog::addError()
{
    ConfigMap::ErrorDetector::Error error("EQ", "", 0);
    addErrorLine(error);
}

void EditParameterDialog::addEnum()
{
    addEnumLine("", 0);
}

void EditParameterDialog::deleteError()
{
    ui.table_error->removeRow(m_errorCurrentIndex);
    m_errorCurrentIndex = -1;
    ui.table_error->update();
    
    ui.button_deleteErrorDetector->setEnabled(false);
}

void EditParameterDialog::deleteEnum()
{
    ui.table_enum->removeRow(m_enumCurrentIndex);
    m_enumCurrentIndex = -1;
    ui.table_enum->update();

    ui.button_deleteEnum->setEnabled(false);
}

void EditParameterDialog::cellSelectedEnum(int row, int collumn)
{
    QTableWidgetItem* a_item = ui.table_enum->item(row, 1);
    a_item->setBackground(QBrush(Qt::green));
    
    if (m_enumCurrentIndex != -1)
    {
        a_item = ui.table_enum->item(m_enumCurrentIndex, 1);
        a_item->setBackground(QBrush(Qt::white));
    }
 
    m_enumCurrentIndex = row;
    ui.button_deleteEnum->setEnabled(true);
    ui.table_enum->update();
}

void EditParameterDialog::cellSelectedError(int row, int collumn)
{
    QTableWidgetItem* a_item = ui.table_error->item(row, 2);
    a_item->setBackground(QBrush(Qt::green));
    if (m_errorCurrentIndex != -1)
    {
        a_item = ui.table_error->item(m_errorCurrentIndex, 2);
        a_item->setBackground(QBrush(Qt::white));
    }

    m_errorCurrentIndex = row;
    ui.button_deleteErrorDetector->setEnabled(true);
    ui.table_error->update();
}
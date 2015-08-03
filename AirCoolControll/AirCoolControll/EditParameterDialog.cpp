#include "EditParameterDialog.h"

EditParameterDialog::EditParameterDialog(ConfigMap::Parameter& parameter,QDialog *parent)
    : QDialog(parent),
    m_editedParameter(parameter),
    m_editedParameterCopy(parameter)
{
    ui.setupUi(this);

    ui.lineEdit_description->setText(QString::fromStdString(m_editedParameter.m_description));
    connect(ui.lineEdit_description, SIGNAL(textChanged(const QString &)), this, SLOT(newDescription(const QString&)));

    ui.spinBox_register->setValue(m_editedParameter.m_registerNumber);
    connect(ui.spinBox_register, SIGNAL(valueChanged(int)), this, SLOT(newRegestryNumber(int)));

    ui.checkBox->setChecked(m_editedParameter.m_isBool);
    connect(ui.checkBox, SIGNAL(stateChanged(int)), this, SLOT(newBitState(int)));
    ui.spinBox_bitNumber->setEnabled(m_editedParameter.m_isBool);

    ui.spinBox_bitNumber->setValue(m_editedParameter.m_bitNumber);
    connect(ui.spinBox_bitNumber, SIGNAL(valueChanged(int)), this, SLOT(newBitNumber(int)));

    ui.spinBox_minValue->setValue(m_editedParameter.m_minValue);
    connect(ui.spinBox_minValue, SIGNAL(valueChanged(int)), this, SLOT(newMinValue(int)));

    ui.spinBox_maxValue->setValue(m_editedParameter.m_maxValue);
    connect(ui.spinBox_maxValue, SIGNAL(valueChanged(int)), this, SLOT(newMaxValue(int)));

    ui.comboBox_decodeMethod->addItems(QString("None,AT").split(','));
    connect(ui.comboBox_decodeMethod, SIGNAL(activated(int)), this, SLOT(newDecodeMethod(int)));

    connect(ui.button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui.button_ok, SIGNAL(clicked()), this, SLOT(okClicked()));
}

EditParameterDialog::~EditParameterDialog()
{

}

void EditParameterDialog::newDescription(const QString& desc)
{
    m_editedParameterCopy.m_description = desc.toStdString();
}

void EditParameterDialog::newRegestryNumber(int n)
{
    m_editedParameterCopy.m_registerNumber = n;
}

void EditParameterDialog::newBitState(int n)
{
    bool is_bool = (bool)n;
    m_editedParameterCopy.m_isBool = is_bool;
    ui.spinBox_bitNumber->setEnabled(is_bool);
}

void EditParameterDialog::newBitNumber(int n)
{
    m_editedParameterCopy.m_bitNumber = n;
}

void EditParameterDialog::newMinValue(int n)
{
    m_editedParameterCopy.m_minValue = n;
}

void EditParameterDialog::newMaxValue(int n)
{
    m_editedParameterCopy.m_maxValue = n;
}

void EditParameterDialog::newDecodeMethod(int n)
{
    const static char* methods[] = { "", "AT" };
    if (n < 0)
        return;

    m_editedParameterCopy.m_decodeMethod = methods[n];
}

void EditParameterDialog::okClicked()
{
    m_editedParameter = m_editedParameterCopy;
    done(1);
}

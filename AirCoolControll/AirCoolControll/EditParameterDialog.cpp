#include "EditParameterDialog.h"

EditParameterDialog::EditParameterDialog(ConfigMap::Parameter& parameter,QDialog *parent)
    : QDialog(parent),
    m_editedParameter(parameter)
{
    ui.setupUi(this);
}

EditParameterDialog::~EditParameterDialog()
{

}

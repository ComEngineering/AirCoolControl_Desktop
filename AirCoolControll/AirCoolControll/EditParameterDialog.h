#ifndef EDITPARAMETERDIALOG_H
#define EDITPARAMETERDIALOG_H

#include <QDialog>
#include "ui_EditParameterDialog.h"
#include "ConfigMap.h"

class EditParameterDialog : public QDialog
{
    Q_OBJECT

public:
    EditParameterDialog(ConfigMap::Parameter& parameter, QDialog *parent = 0);
    ~EditParameterDialog();

private:
    Ui::EditParameterDialog ui;
    ConfigMap::Parameter& m_editedParameter;
};

#endif // EDITPARAMETERDIALOG_H

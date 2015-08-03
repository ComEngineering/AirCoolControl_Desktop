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

private slots:
    void newDescription(const QString& desc);
    void newRegestryNumber(int n);
    void newBitState(int);
    void newBitNumber(int);
    void newMinValue(int);
    void newMaxValue(int);
    void newDecodeMethod(int);
    void okClicked();

private:
    Ui::EditParameterDialog ui;
    ConfigMap::Parameter  m_editedParameterCopy;
    ConfigMap::Parameter& m_editedParameter;
};

#endif // EDITPARAMETERDIALOG_H

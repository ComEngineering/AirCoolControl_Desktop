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
    void newBitState(int);
    void okClicked();
    void addError();
    void addEnum();
    void deleteError();
    void deleteEnum();
    void cellSelectedEnum(int, int);
    void cellSelectedError(int, int);

private:
    void initTables(void);
    void addErrorLine(const ConfigMap::ErrorDetector::Error& error);
    void addEnumLine(std::string showAs, int value);
    
private:
    Ui::EditParameterDialog ui;
    ConfigMap::Parameter& m_editedParameter;
    int                   m_enumCurrentIndex;
    int                   m_errorCurrentIndex;
};

#endif // EDITPARAMETERDIALOG_H

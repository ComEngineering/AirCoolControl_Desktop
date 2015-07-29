#ifndef NEWPARAMETERNAMEDIALOG_H
#define NEWPARAMETERNAMEDIALOG_H

#include <QDialog>
#include "ui_NewParameterNameDialog.h"
#include <set>

class NewParameterNameDialog : public QDialog
{
    Q_OBJECT

public:
    NewParameterNameDialog(const std::set<QString>& existingNames, QWidget *parent = 0);
    ~NewParameterNameDialog();

    QString getNewName() const;

private slots:
    void finish(void);
    void textEntered(const QString &text);

private:
    Ui::NewParameterNameDialog ui;
    const std::set<QString>& m_existingNames;
};

#endif // NEWPARAMETERNAMEDIALOG_H

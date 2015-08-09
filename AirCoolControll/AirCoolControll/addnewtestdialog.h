#ifndef ADDNEWTESTDIALOG_H
#define ADDNEWTESTDIALOG_H

#include <QDialog>
#include "ui_addnewtestdialog.h"
#include "TestStorage.h"

class AddNewTestDialog : public QDialog
{
    Q_OBJECT

public:
    AddNewTestDialog(TestStorage& storage, QWidget *parent = 0);
    ~AddNewTestDialog();
    
    SimpleTestShared getTest(void) const { return m_map; }

private slots:
    void copyFromAnotherTest();
    void addTest();
    void checkWidgetsEnabled();

private:
    Ui::AddNewTestDialog ui;
    SimpleTestShared m_map;
    TestStorage&  m_storage;
};

#endif // ADDNEWTESTDIALOG_H

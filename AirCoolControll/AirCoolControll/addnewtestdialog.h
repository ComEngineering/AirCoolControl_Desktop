#ifndef ADDNEWTESTDIALOG_H
#define ADDNEWTESTDIALOG_H

#include <QDialog>
#include "ui_addnewtestdialog.h"
#include "TestStorage.h"
#include <QIcon>

class AddNewTestDialog : public QDialog
{
    Q_OBJECT

public:
    AddNewTestDialog(TestStorage& storage, ConfigStorage& configs, QWidget *parent = 0);
    ~AddNewTestDialog();
    
    SimpleTestShared getTest(void) const { return m_map; }

private slots:
    void copyFromAnotherTest();
    void addTest();
    void checkWidgetsEnabled();
    void configNameSelected(QListWidgetItem * current, QListWidgetItem * previous);

private:
    ConfigMapShared findConfig() const;

private:
    Ui::AddNewTestDialog ui;
    SimpleTestShared   m_map;
    TestStorage&       m_storage;
    ConfigStorage&     m_configs;
    bool               m_cloned;

    QIcon              m_selectedIcon;
    QIcon              m_notSelectedIcon;
};

#endif // ADDNEWTESTDIALOG_H


#ifndef AddNewConfigDialog_H
#define AddNewConfigDialog_H

#include <QDialog>
#include "ui_AddNewConfigDialog.h"
#include "ConfigStorage.h"

class AddNewConfigDialog : public QDialog
{
    Q_OBJECT

public:
    AddNewConfigDialog(ConfigStorage& storage, QWidget *parent = 0);
    ~AddNewConfigDialog();

    ConfigMapShared getConfig(void) const { return m_map; }

private:
    void fillFields(void);

private slots:
    void copyFromAnotherConfig();
    void addConfig();
    void selectGraphFile();
    void checkWidgetsEnabled();

private:
    Ui::AddNewConfigDialog ui;
    ConfigMapShared m_map;
    ConfigStorage&  m_storage;
};

#endif // AddNewConfigDialog_H

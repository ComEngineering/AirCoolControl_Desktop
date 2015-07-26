#ifndef ADDNEWCONFIGWIDGET_H
#define ADDNEWCONFIGWIDGET_H

#include <QDialog>
#include "ui_AddNewConfigWidget.h"
#include "ConfigStorage.h"

class AddNewConfigWidget : public QDialog
{
    Q_OBJECT

public:
    AddNewConfigWidget(ConfigStorage& storage, QWidget *parent = 0);
    ~AddNewConfigWidget();

    ConfigMapShared getConfig(void) const { return m_map; }

private:
    void fillFields(void);

private slots:
    void copyFromAnotherConfig();
    void addConfig();
    void selectGraphFile();
    void checkWidgetsEnabled();

private:
    Ui::AddNewConfigWidget ui;
    ConfigMapShared m_map;
    ConfigStorage&  m_storage;
};

#endif // ADDNEWCONFIGWIDGET_H

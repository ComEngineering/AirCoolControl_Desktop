#ifndef EDITCONFIGWINDOW_H
#define EDITCONFIGWINDOW_H

#include <QDialog>
#include "ui_EditConfigWindow.h"
#include "ConfigMap.h"

class EditConfigWindow : public QDialog
{
    Q_OBJECT

public:
    EditConfigWindow(ConfigMapShared a_map, QWidget *parent = 0);
    ~EditConfigWindow();

private slots:
    void saveConfig();
    void addCoil();
    void addInput();
    void addOutput();
    void editCoil();
    void editInput();
    void editOutput();
    void deleteCoil();
    void deleteInput();
    void deleteOutput();
    void cellSelectedInput(int, int);
    void cellSelectedOutput(int, int);
    void cellSelectedCoils(int, int);

private:
    void initTab(ConfigMap::RegisterType type);
    void sellectionChanged(int oldIndex, int newIndex, QTableWidget* table);
    
private:
    Ui::EditConfigWindow    ui;
    ConfigMapShared         m_config;
    ConfigMap::ParameterMap m_map;
    QTableWidget*           m_tables[ConfigMap::REGISTER_PULL_COUNT];
    int                     m_currentIndex[ConfigMap::REGISTER_PULL_COUNT];
};

#endif // EDITCONFIGWINDOW_H

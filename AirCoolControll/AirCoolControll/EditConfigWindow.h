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

private:
    Ui::EditConfigWindow ui;
    ConfigMapShared m_map;
};

#endif // EDITCONFIGWINDOW_H

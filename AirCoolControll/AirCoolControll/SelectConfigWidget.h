#ifndef SELECTCONFIGWIDGET_H
#define SELECTCONFIGWIDGET_H

#include <QDialog>
#include "ui_SelectConfigWidget.h"
#include "ConfigStorage.h"

class SelectConfigWidget : public QDialog
{
    Q_OBJECT

public:
    SelectConfigWidget(ConfigStorage& storage, QWidget *parent = 0);
    ~SelectConfigWidget();

    ConfigMapShared getConfig(void) const;

private slots:
    void editOK(void);
    void checkWidgetsEnabled(void);

private:
    Ui::SelectConfigWidget ui;
    ConfigStorage&  m_storage;
};

#endif // SELECTCONFIGWIDGET_H

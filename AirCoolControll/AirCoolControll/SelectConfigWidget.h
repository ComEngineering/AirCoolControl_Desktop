#ifndef SELECTCONFIGWIDGET_H
#define SELECTCONFIGWIDGET_H

#include <QDialog>
#include "ui_SelectConfigWidget.h"
#include "ConfigStorage.h"

class SelectConfigWidget : public QDialog
{
    Q_OBJECT

public:
    SelectConfigWidget(std::vector<std::string>&& storage, QWidget *parent = 0);
    ~SelectConfigWidget();

    int getSelectedIndex(void) const;

private slots:
    void editOK(void);
    void editOK(const QModelIndex & index);
    void checkWidgetsEnabled(void);

private:
    Ui::SelectConfigWidget ui;
};

#endif // SELECTCONFIGWIDGET_H

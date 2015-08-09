#ifndef TESTSTAGEEDITWIDGET_H
#define TESTSTAGEEDITWIDGET_H

#include <QWidget>
#include "ui_TestStageEditWidget.h"

class TestStageEditWidget : public QWidget
{
    Q_OBJECT

public:
    TestStageEditWidget(QWidget *parent = 0);
    ~TestStageEditWidget();

private:
    Ui::TestStageEditWidget ui;
};

#endif // TESTSTAGEEDITWIDGET_H

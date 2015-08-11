#ifndef TESTSTAGEEDITWIDGET_H
#define TESTSTAGEEDITWIDGET_H

#include <QWidget>
#include "ui_TestStageEditWidget.h"
#include "SimpleTest.h"

class TestStageEditWidget : public QWidget
{
    Q_OBJECT

public:
    TestStageEditWidget(SimpleTest::Stage& test, QWidget *parent = 0);
    ~TestStageEditWidget();

private slots:
    void newTime(const QTime &);

private:
    Ui::TestStageEditWidget ui;
    SimpleTest::Stage& m_test;
    TestEditDialog*    m_parent;
};

#endif // TESTSTAGEEDITWIDGET_H

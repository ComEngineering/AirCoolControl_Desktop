#include "TestStageEditWidget.h"
#include "TestEditDialog.h"

TestStageEditWidget::TestStageEditWidget(SimpleTest::Stage& test, QWidget *parent)
    : QWidget(parent),
    m_test(test),
    m_parent(reinterpret_cast<TestEditDialog*>(parent))
{
    ui.setupUi(this);
    
    ui.timeEdit->setTime(QTime().addSecs(test.m_duration));

    connect(ui.timeEdit, SIGNAL(timeChanged(const QTime &)), this, SLOT(newTime(const QTime &)));
}

TestStageEditWidget::~TestStageEditWidget()
{

}

void TestStageEditWidget::newTime(const QTime &)
{
    QTime time = ui.timeEdit->time();
    m_test.m_duration = time.second() + time.minute() * 60 + time.hour() * 60 * 60;
    m_parent->durationChangedCallback();
}


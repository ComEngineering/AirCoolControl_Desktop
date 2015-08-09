#include "TestEditDialog.h"
#include <qtime>
#include "TestStageEditWidget.h"

TestEditDialog::TestEditDialog(SimpleTestShared test, QWidget *parent)
    : QDialog(parent),
    m_test(test),
    m_localCopy(*test)
{
    ui.setupUi(this);
    ui.edit_description->setText(QString::fromStdString(m_test->m_description));

    connect(ui.button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui.button_save, SIGNAL(clicked()), this, SLOT(saveConfig()));
    connect(ui.button_add_stage, SIGNAL(clicked()), this, SLOT(addStage()));
    connect(ui.button_delete_stage, SIGNAL(clicked()), this, SLOT(deleteStage()));

    connect(ui.stages_tab, SIGNAL(currentChanged(int)), this, SLOT(tabSelected(int)));

    updateContent();
}

TestEditDialog::~TestEditDialog()
{

}

void TestEditDialog::saveConfig()
{
    m_localCopy.m_description = ui.edit_description->text().toStdString();
    *m_test = m_localCopy;
    done(1);
}

void TestEditDialog::addStage()
{
    m_test->addStage(SimpleTest::Stage());
}

void TestEditDialog::deleteStage()
{

}

void TestEditDialog::tabSelected(int tn)
{

}

void TestEditDialog::updateContent(void)
{
    ui.stages_tab->clear();
    int n = 0;
    for (const auto& stage : m_localCopy.m_list)
    {
        TestStageEditWidget* stage = new TestStageEditWidget(this);
        ui.stages_tab->insertTab(n, stage, QString::number(n+1));
        n++;
    }
    ui.label_full_time->setText(QTime(0,0).addSecs(m_localCopy.getOverallDuraton()).toString("hh:mm:ss"));
    update();
}
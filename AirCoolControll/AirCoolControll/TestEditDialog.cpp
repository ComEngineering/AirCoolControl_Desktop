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
    //connect(ui.stages_tab, SIGNAL(currentChanged(int)), this, SLOT(tabSelected(int)));

    connect(ui.slider_time, SIGNAL(valueChanged(int)), this, SLOT(sliderMoved(int)));

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
    m_localCopy.addStage(SimpleTest::Stage());
    int activateIndex = ui.stages_tab->count();
    ui.stages_tab->addTab(new TestStageEditWidget(*(m_localCopy.m_list.end() - 1), this), QString::number(activateIndex + 1));
    ui.stages_tab->setCurrentIndex(activateIndex);
}

void TestEditDialog::deleteStage()
{
    int currentTab = ui.stages_tab->currentIndex();
    m_localCopy.deleteStage(currentTab);
    ui.stages_tab->removeTab(currentTab);
    ui.stages_tab->update();
    for (; currentTab < ui.stages_tab->count(); currentTab++)
    {
        ui.stages_tab->setTabText(currentTab, QString::number(currentTab + 1));
    }
}

void TestEditDialog::tabSelected(int tn)
{
    int startTime = m_localCopy.getStageStartTime(tn);
    ui.slider_time->setValue(startTime);
}

void TestEditDialog::updateContent(void)
{
    ui.stages_tab->clear();
    int n = 0;
    for (auto& stage : m_localCopy.m_list)
    {
        TestStageEditWidget* stageEditor = new TestStageEditWidget(stage, this);
        ui.stages_tab->addTab(stageEditor, QString::number(n+1));
        n++;
    }
    setTimeSlider();
    update();
}

void TestEditDialog::sliderMoved(int n)
{
    int stage = m_localCopy.getStageWithTime(n);
    ui.stages_tab->setCurrentIndex(stage);
}

void TestEditDialog::setTimeSlider()
{
    int overallDuration = m_localCopy.getOverallDuraton();
    ui.label_full_time->setText(QTime(0, 0).addSecs(overallDuration).toString("hh:mm:ss"));
    ui.slider_time->setMaximum(overallDuration);
}

void TestEditDialog::durationChangedCallback()
{
    setTimeSlider();
}
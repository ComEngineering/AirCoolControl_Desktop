#ifndef TESTEDITDIALOG_H
#define TESTEDITDIALOG_H

#include <QDialog>
#include "ui_TestEditDialog.h"
#include "SimpleTest.h"

class TestEditDialog : public QDialog
{
    Q_OBJECT

public:
    TestEditDialog(SimpleTestShared test, QWidget *parent = 0);
    ~TestEditDialog();

    void durationChangedCallback();

private:
    void updateContent(void);
    void setTimeSlider();

private slots:
    void saveConfig();
    void addStage();
    void deleteStage();
    void tabSelected(int tn);
    void sliderMoved(int);

private:
    Ui::TestEditDialog ui;
    SimpleTestShared m_test;
    SimpleTest       m_localCopy;
};

#endif // TESTEDITDIALOG_H

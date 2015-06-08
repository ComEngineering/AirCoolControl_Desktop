#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include <QWidget>
#include "ui_PreferencesWindow.h"

class PreferencesWindow : public QWidget
{
    Q_OBJECT

public:
    PreferencesWindow(QWidget *parent = 0);
    ~PreferencesWindow();

private slots:
    void attemptChanged(int);
    void intervalChanged(int);
    void t1Changed(int);
    void t2attemptChanged(int);

private:
    Ui::PreferencesWindow ui;
};

#endif // PREFERENCESWINDOW_H

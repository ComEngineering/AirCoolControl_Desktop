#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include <QDialog>
#include "ui_PreferencesWindow.h"

class PreferencesWindow : public QDialog
{
    Q_OBJECT

public:
    PreferencesWindow(QWidget *parent = 0);
    ~PreferencesWindow();

signals:
    void finished(void);

private slots:
    void attemptChanged(int);
    void intervalChanged(int);
    void t1Changed(int);
    void t2attemptChanged(int);

private:
    Ui::PreferencesWindow ui;
};

#endif // PREFERENCESWINDOW_H

#include "PreferencesWindow.h"
#include "Configurator.h"

PreferencesWindow::PreferencesWindow(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
     
    ui.attempts_spinBox->setValue(Configurator::getRetryCount());
    ui.requestInterval_spinBox->setValue(Configurator::getPullInterval());
    ui.requestTimeout_spinBox->setValue(Configurator::getMaximunTimeout());
    ui.symbolTimeout_spinBox->setValue(Configurator::getChankTimeout());

    connect(ui.okButton, SIGNAL(clicked()),this,SIGNAL(finished()));

    connect(ui.attempts_spinBox, SIGNAL(valueChanged(int)), this, SLOT(attemptChanged(int)));
    connect(ui.requestInterval_spinBox, SIGNAL(valueChanged(int)), this, SLOT(intervalChanged(int)));
    connect(ui.requestTimeout_spinBox, SIGNAL(valueChanged(int)), this, SLOT(t1Changed(int)));
    connect(ui.symbolTimeout_spinBox, SIGNAL(valueChanged(int)), this, SLOT(t2attemptChanged(int)));
}

PreferencesWindow::~PreferencesWindow()
{
    
}

void PreferencesWindow::attemptChanged(int n)
{
    Configurator::setRetryCount(n);
}

void PreferencesWindow::intervalChanged(int n)
{
    Configurator::setPullInterval(n);
}

void PreferencesWindow::t1Changed(int n)
{
    Configurator::setMaximunTimeout(n);
}

void PreferencesWindow::t2attemptChanged(int n)
{
    Configurator::setChankTimeout(n);
}

#include "aircoolcontroll.h"

AirCoolControll::AirCoolControll(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_comunicator = new Cooller_ModBusController(ui.stateWidget, ui.configWidget);
    connect(m_comunicator, SIGNAL(newStatus(const QString&)), ui.statusBar, SLOT(showMessage(const QString&, int)));
   /* ui.stateWidget->leftWindState(true);
    ui.stateWidget->rightWindState(true);*/
}

AirCoolControll::~AirCoolControll()
{
    delete m_comunicator;
}

CoolerStateWidget * AirCoolControll::getStateWidget()
{
    return ui.stateWidget;
}

ModBusDialog *      AirCoolControll::getConfigWidget()
{
    return ui.configWidget;
}

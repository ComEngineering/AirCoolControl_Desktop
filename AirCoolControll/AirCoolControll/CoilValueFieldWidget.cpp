#include "CoilValueFieldWidget.h"
#include <qtoolbutton.h>

CoilValueFieldWidget::CoilValueFieldWidget(DeviceExplorer* explorer, const std::string& name, QWidget *parent)
    : ValueFieldWidget(explorer,name,parent)
{
    m_button = new QToolButton(this);
    m_button->setCheckable(true);
    m_button->setText(tr("Off"));
    connect(m_button, SIGNAL(clicked()), this, SLOT(onCoilChanged()));
}

CoilValueFieldWidget::~CoilValueFieldWidget()
{
    delete m_button;
}

void CoilValueFieldWidget::setValue(QVariant value)
{
}

void CoilValueFieldWidget::onCoilChanged()
{
    int d = m_button->isChecked() ? 1 : 0;
    m_button->setText(d ? tr("On") : tr("Off"));
    m_explorer->sendValueToDevice(ConfigMap::COIL, QString::fromStdString(m_name), d);
}
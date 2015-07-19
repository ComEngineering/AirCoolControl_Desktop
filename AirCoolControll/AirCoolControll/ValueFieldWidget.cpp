#include "ValueFieldWidget.h"

ValueFieldWidget::ValueFieldWidget(DeviceExplorer* explorer, const std::string& name, QWidget *parent)
    : QWidget(parent),
    m_explorer(explorer),
    m_name(name)
{

}

ValueFieldWidget::~ValueFieldWidget()
{

}

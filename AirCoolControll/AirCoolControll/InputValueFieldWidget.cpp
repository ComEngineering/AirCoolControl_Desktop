#include "InputValueFieldWidget.h"
#include <qgridlayout.h>

InputValueFieldWidget::InputValueFieldWidget(DeviceExplorer* explorer, const std::string& name, QWidget *parent)
    : ValueFieldWidget(explorer,name,parent)
{
    m_gridLayout = new QGridLayout(this);
    setLayout(m_gridLayout);
    m_label = new QLabel(this);
    QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_label->setSizePolicy(sizePolicy);
    m_gridLayout->addWidget(m_label, 0, 0, 1, 1);
}

InputValueFieldWidget::~InputValueFieldWidget()
{
    delete m_label;
    delete m_gridLayout;
}

void InputValueFieldWidget::paintEvent(QPaintEvent *event)
{
    const static char errorStyle[] = "QLabel { background-color : red; color : black; }";
    const static char valueStyle[] = "QLabel { background-color : white; color : black; }";

    bool isValid;
    int number = m_value.toInt(&isValid);
    m_label->setText(isValid ? QString::number(number) : m_value.toString());
    m_label->setStyleSheet(isValid ? valueStyle : errorStyle);
    m_label->setGeometry(this->rect());
    
    m_label->update();
}
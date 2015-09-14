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

void InputValueFieldWidget::setValue(QVariant value) 
{ 
    const static char errorStyle[] = "QLabel { background-color : red; color : black; }";
    const static char valueStyle[] = "QLabel { background-color : white; color : black; }";

    bool isValid;
    int number = value.toInt(&isValid);
    m_label->setText(isValid ? QString::number(number) : value.toString());
    m_label->setStyleSheet(isValid ? valueStyle : errorStyle);

    repaint();
}

void InputValueFieldWidget::paintEvent(QPaintEvent *event)
{
    m_label->setGeometry(this->rect());
}
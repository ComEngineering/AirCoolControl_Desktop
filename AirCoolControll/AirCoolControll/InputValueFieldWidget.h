#ifndef INPUTVALUEFIELDWIDGET_H
#define INPUTVALUEFIELDWIDGET_H

#include "ValueFieldWidget.h"
#include <qlabel.h>
#include <qgridlayout.h>

class InputValueFieldWidget : public ValueFieldWidget
{
    Q_OBJECT

public:
    InputValueFieldWidget(DeviceExplorer* explorer, const std::string& name, QWidget *parent = 0);
    ~InputValueFieldWidget();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QLabel * m_label;
    QGridLayout* m_gridLayout;
};

#endif // INPUTVALUEFIELDWIDGET_H

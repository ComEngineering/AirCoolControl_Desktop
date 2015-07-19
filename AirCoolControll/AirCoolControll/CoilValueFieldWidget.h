#ifndef COILVALUEFIELDWIDGET_H
#define COILVALUEFIELDWIDGET_H

#include "ValueFieldWidget.h"
#include <string>
#include <qtoolbutton.h>

class CoilValueFieldWidget : public ValueFieldWidget
{
    Q_OBJECT

public:
    CoilValueFieldWidget(DeviceExplorer* explorer, const std::string& name , QWidget *parent = 0);
    ~CoilValueFieldWidget();

private slots:
    void onCoilChanged();

private:
    QToolButton* m_button;
};

#endif // COILVALUEFIELDWIDGET_H

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

    virtual void setValue(QVariant value) override;

private slots:
    void onCoilChanged();

private:
    QToolButton* m_button;
};

#endif // COILVALUEFIELDWIDGET_H

#ifndef VALUEFIELDWIDGET_H
#define VALUEFIELDWIDGET_H

#include <QWidget>
#include <QVariant>
#include "DeviceExplorer.h"

class ValueFieldWidget : public QWidget
{
    Q_OBJECT

public:
    ValueFieldWidget(DeviceExplorer* explorer, const std::string& name, QWidget *parent = 0);
    ~ValueFieldWidget();

    virtual void setValue(QVariant value) = 0; 

protected:
    DeviceExplorer* m_explorer;
    std::string     m_name;
};

#endif // VALUEFIELDWIDGET_H

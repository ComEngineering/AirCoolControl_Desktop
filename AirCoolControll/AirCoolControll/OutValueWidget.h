#ifndef OUTVALUEWIDGET_H
#define OUTVALUEWIDGET_H

#include "ValueFieldWidget.h"
#include <qspinbox.h>
#include <qcombobox.h>
#include "ConfigMap.h"

class OutValueWidget : public ValueFieldWidget
{
    Q_OBJECT

public:
    OutValueWidget(DeviceExplorer* explorer, const std::string& name, const ConfigMap::Parameter& parameter, QWidget *parent = 0);
    ~OutValueWidget();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void registerSet(int v);
    void enumItemActivated(int);

private:
    QSpinBox *           m_edit;
    QComboBox*           m_combo;
    QGridLayout*         m_gridLayout;
    ConfigMap::Parameter m_parameters;
};

#endif // OUTVALUEWIDGET_H

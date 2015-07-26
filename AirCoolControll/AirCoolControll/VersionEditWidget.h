#ifndef VERSIONEDITWIDGER_H
#define VERSIONEDITWIDGER_H

#include <QWidget>
#include "ui_VersionEditWidget.h"
#include "VersionStorage.h"

class VersionEditWidget : public QWidget
{
    Q_OBJECT

public:
    VersionEditWidget(QWidget *parent = 0);
    ~VersionEditWidget();

    void setVersion(const VersionStorage& version);
    VersionStorage getVersion(void) const;
private:
    Ui::VersionEditWidget ui;
};

#endif // VERSIONEDITWIDGER_H

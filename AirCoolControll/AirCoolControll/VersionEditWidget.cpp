#include "VersionEditWidget.h"

VersionEditWidget::VersionEditWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

VersionEditWidget::~VersionEditWidget()
{

}

void VersionEditWidget::setVersion(const VersionStorage& version)
{
    ui.minor_version->setText(QString::number(version.m_minor));
    ui.major_version->setText(QString::number(version.m_major));
    ui.release_version->setText(QString::number(version.m_revision));
}

VersionStorage VersionEditWidget::getVersion(void) const
{
    return VersionStorage(ui.major_version->text() + '.' + ui.minor_version->text() + '.' + ui.release_version->text());
}
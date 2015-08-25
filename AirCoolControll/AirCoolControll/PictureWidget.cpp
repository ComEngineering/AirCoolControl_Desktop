#include "PictureWidget.h"
#include <qpainter.h>
#include "Configurator.h"
#include "DeviceExplorer.h"

PictureWidget::PictureWidget(QWidget *parent)
    : QWidget(parent),
    m_device(NULL)
{
    
}

PictureWidget::~PictureWidget()
{

}

void PictureWidget::setConfig(const ConfigMapShared map, DeviceExplorer *device)
{
    if (map->getUItype() != "picture")
    {
        return;
    }

    if (!m_pixmap.load(Configurator::getConfigFilesPath() + QString::fromStdString(map->getUIpicturePath())))
    {
        return;
    }

    m_map = map;
    m_device = device;
}

void PictureWidget::paintEvent(QPaintEvent * event)
{
    if (!m_device)
        return;

    QPainter painter(this);
    painter.drawPixmap(rect(), m_pixmap);
    float scaleX = float(width()) / m_pixmap.width();
    float scaleY = float(height()) / m_pixmap.height();
    for (const auto& tag : m_map->getPlaceholders())
    {
        QVariant v = m_device->getRegisterValue(tag.m_variableName);
        QString vs = v.toString();
        painter.drawText(QPoint(tag.m_x * scaleX, tag.m_y * scaleY), vs);
    }
}

float PictureWidget::getAspectRatio(void) const
{
    int w = m_pixmap.width();
    int h = m_pixmap.height();

    return float(w) / h;
}
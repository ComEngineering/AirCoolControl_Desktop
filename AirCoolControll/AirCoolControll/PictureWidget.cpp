#include "PictureWidget.h"
#include <qpainter.h>
#include "Configurator.h"

PictureWidget::PictureWidget(QWidget *parent)
    : QWidget(parent),
    m_validVidget(false)
{
    
}

PictureWidget::~PictureWidget()
{

}

void PictureWidget::setConfig(const ConfigMapShared map)
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
    m_validVidget = true;
}

void PictureWidget::paintEvent(QPaintEvent * event)
{
    if (!m_validVidget)
        return;

    QPainter painter(this);
    painter.drawPixmap(rect(), m_pixmap);
}
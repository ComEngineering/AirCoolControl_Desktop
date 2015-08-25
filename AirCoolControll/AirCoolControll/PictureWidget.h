#ifndef PICTUREWIDGET_H
#define PICTUREWIDGET_H

#include <QWidget>
#include <qpixmap.h>
#include "ConfigMap.h"

class DeviceExplorer;

class PictureWidget : public QWidget
{
    Q_OBJECT

public:
    PictureWidget(QWidget *parent = 0);
    ~PictureWidget();

    void setConfig(const ConfigMapShared map, DeviceExplorer *device);
    bool isValid(void) const {
        return m_device != NULL;
    }
    float getAspectRatio(void) const;

protected:
    void	paintEvent(QPaintEvent * event);

private:
    ConfigMapShared m_map;
    DeviceExplorer *m_device;
    QPixmap         m_pixmap;
};

#endif // PICTUREWIDGET_H

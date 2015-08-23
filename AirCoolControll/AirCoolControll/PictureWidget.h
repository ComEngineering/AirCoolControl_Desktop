#ifndef PICTUREWIDGET_H
#define PICTUREWIDGET_H

#include <QWidget>
#include <qpixmap.h>
#include "ConfigMap.h"

class PictureWidget : public QWidget
{
    Q_OBJECT

public:
    PictureWidget(QWidget *parent = 0);
    ~PictureWidget();

    void setConfig(const ConfigMapShared map);

protected:
    void	paintEvent(QPaintEvent * event);

private:
    ConfigMapShared m_map;
    bool            m_validVidget;
    QPixmap         m_pixmap;
};

#endif // PICTUREWIDGET_H

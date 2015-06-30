#ifndef COOLLERGRAPHWIDGET_H
#define COOLLERGRAPHWIDGET_H

#include <QWidget>
#include "ui_coollergraphwidget.h"
#include <qtimer.h>
#include <qpixmap.h>

class CoollerGraphWidget : public QWidget
{
    Q_OBJECT

public:
    CoollerGraphWidget(QWidget *parent = 0);
    ~CoollerGraphWidget();

    void leftWindState(bool on);
    void rightWindState(bool on);

private slots:
    void updateViewElements(void);

private:
    enum       { AnimationSteps = 4 };
    Ui::CoollerGraphWidget ui;
    bool        m_leftWindOn;
    bool        m_rightWindOn;
    int         m_windState;
    QTimer *    m_viewTimer;

    QPixmap s_left_arrows[AnimationSteps];
    QPixmap s_right_arrows[AnimationSteps];
};

#endif // COOLLERGRAPHWIDGET_H

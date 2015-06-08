#include "coollergraphwidget.h"

CoollerGraphWidget::CoollerGraphWidget(QWidget *parent)
    : QWidget(parent),
    m_leftWindOn(false),
    m_rightWindOn(false),
    m_windState(0),
    m_viewTimer(new QTimer(this))
{
   /* s_left_arrows[0] = QPixmap(":/Images/left_stream_0.gif");
    s_left_arrows[1] = QPixmap(":/Images/left_stream_1.gif");
    s_left_arrows[2] = QPixmap(":/Images/left_stream_2.gif");
    s_left_arrows[3] = QPixmap(":/Images/left_stream_3.gif");

    s_right_arrows[0] = QPixmap(":/Images/right_stream_0.gif");
    s_right_arrows[1] = QPixmap(":/Images/right_stream_1.gif");
    s_right_arrows[2] = QPixmap(":/Images/right_stream_2.gif");
    s_right_arrows[3] = QPixmap(":/Images/right_stream_3.gif");*/

    ui.setupUi(this);
    
    ui.label_left_arrow->setHidden(true);
    ui.label_right_arrow->setHidden(true);

    connect(m_viewTimer, SIGNAL(timeout()), this, SLOT(updateViewElements(void)));
    m_viewTimer->setInterval(300);
    m_viewTimer->start();
}


CoollerGraphWidget::~CoollerGraphWidget()
{
    
}

void CoollerGraphWidget::updateViewElements(void)
{
    m_windState++;
    /*if (m_windState >= AnimationSteps)
        m_windState = 0;

    if (m_leftWindOn)
    {
        ui.label_left_arrow->setHidden(false);
        ui.label_left_arrow->setPixmap(s_left_arrows[m_windState]);
    }

    if (m_rightWindOn)
    {
        ui.label_right_arrow->setHidden(false);
        ui.label_right_arrow->setPixmap(s_right_arrows[m_windState]);
    }

    if (m_leftWindOn || m_rightWindOn)
    {
        update();
    }*/
}

void CoollerGraphWidget::leftWindState(bool on)
{
    m_leftWindOn = on;
}

void CoollerGraphWidget::rightWindState(bool on)
{
    m_rightWindOn = on;
}
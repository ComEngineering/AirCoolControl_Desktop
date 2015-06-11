#include "MdiSubWindowPermanent.h"

MdiSubWindowPermanent::MdiSubWindowPermanent(QWidget *child,QWidget *parent)
    : QMdiSubWindow(parent)
{
    setWidget(child);
    hide();
}

MdiSubWindowPermanent::~MdiSubWindowPermanent()
{

}

void MdiSubWindowPermanent::closeEvent(QCloseEvent * closeEvent)
{
    hide();
}

void MdiSubWindowPermanent::keyPressEvent(QKeyEvent * keyEvent)
{
    if (keyEvent->key() == Qt::Key_Escape)
        hide();
}
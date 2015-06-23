#ifndef MDISUBWINDOWPERMANENT_H
#define MDISUBWINDOWPERMANENT_H

#include <QMdiSubWindow>
#include <QKeyEvent>

class MdiSubWindowPermanent : public QMdiSubWindow
{
    Q_OBJECT

public:
    MdiSubWindowPermanent(QWidget *child,QWidget *parent = 0);
    ~MdiSubWindowPermanent();

protected:
    virtual void	closeEvent(QCloseEvent * closeEvent);
    virtual void	keyPressEvent(QKeyEvent * keyEvent);
};

#endif // MDISUBWINDOWPERMANENT_H

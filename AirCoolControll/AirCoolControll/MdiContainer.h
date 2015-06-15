#ifndef __MdiContainer__
#define __MdiContainer__

#include "MdiSubWindowPermanent.h"
#include <qwidget.h>

template<typename T>
class MdiContainer
{
public:            
    template<typename T>
    MdiContainer(QWidget * parent = NULL);
    MdiContainer(QWidget * parent = NULL) : m_widget(new T(parent)) { m_mdi = new MdiSubWindowPermanent(m_widget, parent); }
    
    ~MdiContainer()
    {
    } 

    template<typename T>
    QMdiSubWindow* container(void) const
    {
        return static_cast<QMdiSubWindow*>(m_mdi);
    }

    template<typename T>
    T* widget(void) const
    {
        return m_widget;
    }

    void activate()
    {
        m_mdi->show();
        m_mdi->activateWindow();
    }

    
private:
    MdiSubWindowPermanent* m_mdi;
    T    *                 m_widget;
};

#endif //__MdiContainer__
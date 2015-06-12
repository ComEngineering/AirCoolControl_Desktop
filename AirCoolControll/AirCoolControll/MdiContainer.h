#ifndef __MdiContainer__
#define __MdiContainer__

#include "MdiSubWindowPermanent.h"
#include <qobject.h>

template<typename T>
class MdiContainer
{
public:            
    template<typename T>
    MdiContainer(QObject * parent) : m_widget(new T(parent)), m_mdi(new MdiSubWindowPermanent(m_widget, parent)) { }
    
    ~MdiContainer()
    {
    } 
    
    template<typename T>
    operator QMdiSubWindow*(void) const
    {
        return static_cast<QMdiSubWindow*>(m_mdi);
    }

    template<typename T>
    T* widget(void)
    {
        return m_widget;
    }
    
private:
    template<typename T> MdiContainer(MdiContainer<T>&);
    template<typename T> MdiContainer(const MdiContainer<T>&);

    MdiSubWindowPermanent* m_mdi;
    T    *                 m_widget;
};

#endif //__MdiContainer__
#ifndef __PullerTaskBase__
#define __PullerTaskBase__

#include <memory>
#include "modbusuart_impl.h"
#include <qobject.h>

class PullerTaskBase : public QObject
{
    Q_OBJECT

public:
    PullerTaskBase(int id);
    virtual ~PullerTaskBase();

    virtual bool proceed(ModBusUART_Impl* modbus) = 0;
    virtual bool isItTimeToDo(void) const { return true; }

    int  getID() const;

private:
    int  m_id;
    
protected:
    int   m_failCounter;
};

typedef std::shared_ptr<PullerTaskBase> PullerTaskShared;

#endif //__PullerTaskBase__


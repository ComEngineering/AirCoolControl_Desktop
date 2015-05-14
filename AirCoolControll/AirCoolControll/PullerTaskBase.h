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

    virtual bool proceed(ModBusUART_ImplShared modbus) = 0;

    int  getID() const;

private:
    int  m_id;
};

typedef std::shared_ptr<PullerTaskBase> PullerTaskShared;

#endif //__PullerTaskBase__


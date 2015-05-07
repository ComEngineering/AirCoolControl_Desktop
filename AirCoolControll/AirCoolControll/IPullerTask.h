#ifndef __IPULLERTASK__
#define __IPULLERTASK__

#include <memory>
#include "modbusuart_impl.h"

class IPullerTask
{
public:
    IPullerTask();
    virtual ~IPullerTask();

    virtual bool proceed(ModBusUART_ImplShared modbus) = 0;
    virtual int  getID() const = 0;
};

typedef std::shared_ptr<IPullerTask> PullerTaskShared;

#endif //__IPULLERTASK__


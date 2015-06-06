#ifndef __PullerTaskBase__
#define __PullerTaskBase__

#include <memory>
#include "boost/date_time/posix_time/posix_time.hpp" 

class ModBusUART_Impl;

class PullerTaskBase 
{

public:
    PullerTaskBase(int id);
    virtual ~PullerTaskBase();

    virtual bool proceed(ModBusUART_Impl* modbus) = 0;
    virtual bool isItTimeToDo(void) const { return true; }

    int  getID() const;

private:
    int                      m_id;
    
protected:
    int                      m_failCounter;
    boost::posix_time::ptime m_lastSuccessfullAttemptTime;
};

typedef std::shared_ptr<PullerTaskBase> PullerTaskShared;

#endif //__PullerTaskBase__


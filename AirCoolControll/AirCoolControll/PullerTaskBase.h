#ifndef __PullerTaskBase__
#define __PullerTaskBase__

#include <memory>
#include "boost/date_time/posix_time/posix_time.hpp" 

class ModBusUART_Impl;

class PullerTaskBase 
{
public:
    enum Priority
    {
        High = 0,
        Midle,
        Low,
        Count
    };

    static Priority &NEXT(Priority &c) {
        assert(c < Count);
        c = static_cast<Priority>(c + 1);
        return c;
    }

    PullerTaskBase(int id,int speed = 9600);
    virtual ~PullerTaskBase();

    virtual bool proceed(ModBusUART_Impl* modbus) = 0;
    virtual bool isItTimeToDo(void) const { return true; }
    virtual Priority  priority() = 0;

    int  getID() const;
    int  getSpeed() const;

private:
    int                      m_id;
    int                      m_speed;
    
protected:
    int                      m_failCounter;
    boost::posix_time::ptime m_lastSuccessfullAttemptTime;
};

typedef std::shared_ptr<PullerTaskBase> PullerTaskShared;

#endif //__PullerTaskBase__


#ifndef __PullerSetCoilTask__
#define __PullerSetCoilTask__

#include "PullerTaskBase.h"

class PullerSetCoilTask :
    public PullerTaskBase
{
public:
    PullerSetCoilTask(int id, int speed, int regNumber, bool value);
    ~PullerSetCoilTask();

    Priority  priority(){ return High; }

    void proceed(ModBusUART_Impl* modbus);
    void succesCall(bool rc);

private:
    int   m_regNumber;
    bool  m_value;
};

typedef std::shared_ptr<PullerSetCoilTask> PullerSetCoilTaskShared;

#endif //__PullerSetCoilTask__
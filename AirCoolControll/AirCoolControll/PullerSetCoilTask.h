#ifndef __PullerWriteTask__
#define __PullerWriteTask__

#include "PullerTaskBase.h"

class PullerSetCoilTask :
    public PullerTaskBase
{
public:
    PullerSetCoilTask(int id, int regNumber, bool value);
    ~PullerSetCoilTask();

    bool proceed(ModBusUART_Impl* modbus);

private:
    int   m_regNumber;
    bool  m_value;
};

#endif //__PullerWriteTask__
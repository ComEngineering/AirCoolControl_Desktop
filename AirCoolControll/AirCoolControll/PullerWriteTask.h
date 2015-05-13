#ifndef __PullerWriteTask__
#define __PullerWriteTask__

#include "PullerTaskBase.h"

class PullerWriteTask :
    public PullerTaskBase
{
public:
    PullerWriteTask(int id,int regNumber,int value);
    virtual ~PullerWriteTask();

    bool proceed(ModBusUART_ImplShared modbus);

private:
    int m_regNumber;
    int m_value;
    int m_tryCounter;
};

#endif // __PullerWriteTask__

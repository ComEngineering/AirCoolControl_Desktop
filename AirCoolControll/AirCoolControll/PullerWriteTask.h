#ifndef __PullerWriteTask__
#define __PullerWriteTask__

#include "PullerTaskBase.h"

class PullerWriteTask :
    public PullerTaskBase
{
  
public:
    PullerWriteTask(int id,int speed,int regNumber,int value);
    virtual ~PullerWriteTask();

    Priority  priority(){ return Midle; }

    bool proceed(ModBusUART_Impl* modbus);

private:
    int m_regNumber;
    int m_value;
};

typedef std::shared_ptr<PullerWriteTask> PullerWriteTaskShared;

#endif // __PullerWriteTask__

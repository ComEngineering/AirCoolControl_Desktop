#ifndef __PullerWriteTask__
#define __PullerWriteTask__

#include "PullerTaskBase.h"

class PullerWriteTask :
    public PullerTaskBase
{
    Q_OBJECT
public:
    PullerWriteTask(int id,int regNumber,int value);
    virtual ~PullerWriteTask();

    bool proceed(ModBusUART_ImplShared modbus);

private:
    int m_regNumber;
    int m_value;
};

typedef std::shared_ptr<PullerWriteTask> PullerWriteTaskShared;

#endif // __PullerWriteTask__

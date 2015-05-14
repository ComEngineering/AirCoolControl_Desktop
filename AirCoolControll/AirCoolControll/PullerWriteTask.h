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

signals:
    void transmitionError(void);

private:
    int m_regNumber;
    int m_value;
    int m_tryCounter;
};

typedef std::shared_ptr<PullerWriteTask> PullerWriteTaskShared;

#endif // __PullerWriteTask__

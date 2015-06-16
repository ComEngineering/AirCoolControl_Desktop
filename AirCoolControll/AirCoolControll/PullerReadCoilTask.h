#ifndef __PullerReadCoilTask__
#define __PullerReadCoilTask__

#include "PullerReadTask.h"

class PullerReadCoilTask :
    public PullerReadTask
{
public:
    PullerReadCoilTask(int id, int speed, Interval& range);
    ~PullerReadCoilTask();

    bool proceed(ModBusUART_Impl* modbus);

};

typedef std::shared_ptr<PullerReadCoilTask> PullerReadCoilsTaskShared;

#endif //__PullerReadCoilTask__
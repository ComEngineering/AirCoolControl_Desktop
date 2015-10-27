#ifndef __PullerGetDeviceInfoTask__
#define __PullerGetDeviceInfoTask__

#include "PullerTaskBase.h"
#include "DeviceInfo.h"

class PullerGetDeviceInfoTask :
    public PullerTaskBase 
{
public:
    PullerGetDeviceInfoTask(int id, int speed, const QString& uartName, std::function<void(DeviceInfoShared)> cb);
    virtual ~PullerGetDeviceInfoTask();

    Priority  priority(){ return Low; }

    void proceed(ModBusUART_Impl* modbus);

    void succesCall(const QString& vendor, const QString& product, const QString& version);
    bool failCall();
 
private:
    QString m_uartName;
    std::function<void(DeviceInfoShared)> m_cb;
    ModBusUART_Impl* m_modbus;

};

typedef std::shared_ptr<PullerGetDeviceInfoTask> PullerGetDeviceInfoTaskShared;

#endif // __PullerGetDeviceInfoTask__

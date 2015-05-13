#ifndef __PullerGetDeviceInfoTask__
#define __PullerGetDeviceInfoTask__

#include "PullerTaskBase.h"

class PullerGetDeviceInfoTask :
    public PullerTaskBase
{
public:
    PullerGetDeviceInfoTask(int id);
    virtual ~PullerGetDeviceInfoTask();

    bool proceed(ModBusUART_ImplShared modbus);
    QString getVendor(){ return m_vendor; }
    QString getProduct(){ return m_product; }
    QString getVersion(){ return m_version; }

signals:
    void deviceNotFound(void);
    void deviceDetected(void);

private:
    QString m_vendor; 
    QString m_product; 
    QString m_version;
    int     m_tryCounter;
};

#endif // __PullerGetDeviceInfoTask__

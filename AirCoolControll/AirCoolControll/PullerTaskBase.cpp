#include "PullerTaskBase.h"
#include "Configurator.h"

PullerTaskBase::PullerTaskBase(int id,int speed) :
    m_id(id),
    m_speed(speed),
    m_failCounter(0),
    m_lastSuccessfullAttemptTime(boost::posix_time::microsec_clock::local_time() - boost::posix_time::milliseconds(Configurator::getPullInterval()))
{
}

PullerTaskBase::~PullerTaskBase()
{
}

int  PullerTaskBase::getID() const
{
    return m_id;
}

int  PullerTaskBase::getSpeed() const
{
    return m_speed;
}

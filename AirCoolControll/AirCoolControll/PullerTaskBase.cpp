#include "PullerTaskBase.h"


PullerTaskBase::PullerTaskBase(int id,int speed) :
    m_id(id),
    m_speed(speed),
    m_failCounter(0),
    m_lastSuccessfullAttemptTime(boost::gregorian::date(1971, boost::gregorian::May, 3))
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

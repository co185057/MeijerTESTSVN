#pragma once

#include <map>
#include "StateMachineObject.h"
#include "statem.h"

using namespace std;

namespace STATE_MACHINE_NAMESPACE
{

class STATEM_API ITimer
    : public IStateMachineObject
{
public:

    virtual ~ITimer(void) {};

    virtual const _TCHAR *GetName()=0;

    virtual bool IsPeriodic()=0;
};

typedef ITimer *PTIMER;

typedef map<StringType, PTIMER, StringComparator> CTimersMap;

}
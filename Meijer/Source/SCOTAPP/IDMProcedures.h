#ifndef _IDMPROCEDURES_H
#define _IDMPROCEDURES_H


#include "DMTriColorLightConstants.h"

class IDMProcedures
{
public:
    IDMProcedures() {}
    virtual ~IDMProcedures() {}

    virtual void setTriColorLight(DMTriColorLightColor, DMTriColorLightState, DMTriColorLightRequestType) = 0;
    
};

#endif // _IDMPROCEDURES_H

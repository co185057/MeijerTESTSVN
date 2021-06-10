//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOutOfService2.h
//
// TITLE: Class header for wait for helpf for bad card state, wait for TS testing
//
// This state accepts and handles inputs from:
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _OUTOFSERVICESTATE2
#define _OUTOFSERVICESTATE2

#include "SMOutOfService2Base.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMOutOfService2 : public SMOutOfService2Base
{
public:
    SMOutOfService2();

    virtual SMStateBase  *Initialize(void);
    virtual void     UnInitialize(void);
    virtual SMStateBase  *TimedOut(void);
    virtual long DMConfigureScaleDisplay(void);

protected:
    virtual void TBShow2x20(void);
    virtual long SecMgrInitialize(void);
    virtual SMStateBase* createLevel1Message( LPCTSTR szMsg,
        PSTEXT pstMsg,
        PSTEXT prompt,
        long   value,
        long   device,
        bool   checkHealth,
        bool displayHTML=false);

    virtual SMStateBase * DMHandleStartupError(void);
    virtual long DMPrintVersion(void);

private:
	bool fServerStarted;


  DECLARE_DYNCREATE(SMOutOfService2)// MFC Runtime class/object information
};

#endif
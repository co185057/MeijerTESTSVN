//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMStoreModeState.h
//
// Description: Derived from SMState.  All store mode states are derived from this state.
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSTOREMODESTATE
#define _SMSTOREMODESTATE

#include "SMStoreModeStateBase.h"

////////////////////////////////////////
#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMStoreModeState : public SMStoreModeStateBase
{
	
public:
    SMStoreModeState();
    virtual bool  storeMode();
    virtual bool PSRemoteLegalForState();
    virtual bool PSAssistMenuLegalForState();

    virtual SMStateBase * createLevel1Message( LPCTSTR szMsg,
        PSTEXT pstMsg,
        PSTEXT prompt,
        long   value,
        long   device,
        bool   checkHealth, 
        bool displayHTML=false);
    virtual SMStateBase * createLevel2Message( LPCTSTR szMsg,
        PSTEXT pstMsg,
        PSTEXT prompt,
        long   value,
        long   device,
        bool   checkHealth,
        bool displayHTML=false);
    virtual SMStateBase * createLevel3Message( LPCTSTR szMsg,
        PSTEXT pstMsg,
        PSTEXT prompt,
        long   value,
        long   device,
        bool   checkHealth,
        bool displayHTML=false);

    virtual bool DMCashRecyclerShouldReject(const CString &csStateName);
    virtual SMStateBase *DMAcceptor(void);
    virtual SMStateBase *HandleOutstandingInterventions();

    DECLARE_DYNCREATE(SMStoreModeState)// MFC Runtime class/object information
};

#endif

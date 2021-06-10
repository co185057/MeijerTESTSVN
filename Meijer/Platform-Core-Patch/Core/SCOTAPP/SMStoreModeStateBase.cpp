//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStoreModeStateBase.cpp
//
// TITLE: Base class for Store Mode states
//
// AUTHOR:  Jeff Connelly
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"
#include "SMStoreModeState.h"
#include "SMSystemMessage1.h"
#include "SMSystemMessage2.h"
#include "SMSystemMessage3.h"
#include "SMState.h"                //SSCOB-257

#define COMP_ID ID_SM               // base state component
#define T_ID _T("StoreModeStateBase")

IMPLEMENT_DYNCREATE(SMStoreModeStateBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(StoreModeState)


DEFINE_TIMESTAMP

////////////////////////////////////
SMStoreModeStateBase::SMStoreModeStateBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////////////////////////////////
SMStateBase * SMStoreModeStateBase::createLevel1Message( LPCTSTR szMsg,
		                                            PSTEXT pstMsg,
		                                            PSTEXT prompt,
		                                            long   value,
		                                            long   device,
		                                            bool   checkHealth )
{
  CREATE_AND_DISPENSE(SystemMessage1)(szMsg,pstMsg,prompt,value,device,checkHealth);
}

///////////////////////////////////////////////////////////////////
SMStateBase * SMStoreModeStateBase::createLevel2Message( LPCTSTR szMsg,
		                                            PSTEXT pstMsg,
		                                            PSTEXT prompt,
		                                            long   value,
		                                            long   device,
		                                            bool   checkHealth )
{
  CREATE_AND_DISPENSE(SystemMessage2)(szMsg,pstMsg,prompt,value,device,checkHealth);
}

///////////////////////////////////////////////////////////////////
SMStateBase * SMStoreModeStateBase::createLevel3Message( LPCTSTR szMsg,
		                                            PSTEXT pstMsg,
		                                            PSTEXT prompt,
		                                            long   value,
		                                            long   device,
		                                            bool   checkHealth )
{
  CREATE_AND_DISPENSE(SystemMessage3)(szMsg,pstMsg,prompt,value,device,checkHealth);
}

// Assist mode may be run legally from the RAP station
bool SMStoreModeStateBase::PSRemoteLegalForState() //awk.10.22.2
{
	return true;
}
// Assist mode may be run legally from the RAP station
bool SMStoreModeStateBase::PSAssistMenuLegalForState() //awk.10.22.2
{
	return true;
}


//SSCOB-257+
SMStateBase *SMStoreModeStateBase::PSParse(const MessageElement *me)
{
  
  // save all the PS parameters
  nPSLastEvt = (PSEVT)me->psInfo.nEvt;
  nPSLastParm = me->psInfo.lParm;
  CString csContext;

  csContext = me->psInfo.pszContext; 

  trace(L5,_T("Parse PS %d, wparam:0x%x, lparam:0x%x, csContext: %s"),nPSLastEvt,me->psInfo.wParm, nPSLastParm, csContext);

  if ( csContext.Find(_T("Attract")) != -1 )
  {
	 trace(L5, _T("Ignore any button press from Attract"));
	 return STATE_NULL;
  }

#ifdef _BUILDLIB_
	return SMStateBase::PSParse(me);
#else
	return SMState::PSParse(me);
#endif
}
//SSCOB-257-
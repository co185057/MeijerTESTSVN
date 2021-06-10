//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateUSSFBase.CPP
//
// TITLE: USSF SMStateBase functions, implement all Base SMStateBase::USSF* functions
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h> 					// MS MFC always needed first

#include "Common.h" 					//	common includes
#include "SMSystemMessage4.h"
#include "SMUSSFManager.h"
#include "USSFCommon.h"

//////////////////////////////////////////
// EVENTUALLY MOVE THIS INTO H-FILE!
//////////////////////////////////////////
#ifndef STATE_USSF_ENDSCRIPT
#define STATE_USSF_ENDSCRIPT	((SMStateBase *)(998))
#endif
#ifndef STATE_USSF_GOTO
#define STATE_USSF_GOTO	((SMStateBase *)(997))
#endif
#ifndef STATE_USSF_FLUSHEVENT
#define STATE_USSF_FLUSHEVENT	((SMStateBase *)(996))
#endif
//////////////////////////////////////////
//////////////////////////////////////////

#define COMP_ID ID_SM			   // base state component
#define T_ID	_T("SMUSSF")

extern CPSX*  m_pGAPSX; 
#ifdef _BUILDLIB_
USSFUtilityBase ub;
DLLEXPORT USSFUtilityBase &USSFutil = ub;
#endif

// Static USSF State.
SMStateBase *SMStateBase::pUSSFthis = NULL;
SMStateBase *SMStateBase::pUSSFNative = NULL;
long SMStateBase::lActivationSerial = 0;
long SMStateBase::lHookSerial = 0;
bool SMStateBase::bIgnoreUnknownAction = false;
bool SMStateBase::fItemOverride = false;
long SMStateBase::lUnapprovedUSSFItems = 0;
CCriticalSection SMStateBase::USSFEventCriticalSection;
CString SMStateBase::csExitName = _T("USSFExit.dll");
CString SMStateBase::csExitVersion = _T("2");
HMODULE SMStateBase::hUSSFExit = NULL;

void SMStateBase::SetOperationalState(SMStateBase *pOpState)
{
	pOperationalState = pOpState;
}

/*
enum HookState
{
	HOOKSTATE_INIT =		0,
	HOOKSTATE_PRE =			1,
	HOOKSTATE_HANDLER =		2,
	HOOKSTATE_UNHANDLED =	3,
	HOOKSTATE_AFTER =		4,
	HOOKSTATE_BEFORE =		5,
	HOOKSTATE_INSTEAD =		6,
	HOOKSTATE_POST =		7,
	HOOKSTATE_THEN =		8,
	HOOKSTATE_ELSE =		9,
	HOOKSTATE_SKIP =		10,
	HOOKSTATE_FINAL =		11,
	HOOKSTATE_NOUSSF =		12,
	HOOKSTATE_TERM =		13,
	HOOKSTATE_MAX =			14
};
*/

SMStateBase *SMStateBase::HookEntry(CString csEventName, long &lHookState, bool &bDone, SMStateBase *newState)
{
	//bool bDone = false;
	static CString csAltContext = _T("");
  	CString csDefaultEventName = csEventName;
	CString csRealEventName;
	//static CString csTraceBuffer;
	static SMStateBase *nextState = STATE_NULL;
	CString csTempstr;
	USSFutil.ResolveSymbol(csDefaultEventName, csRealEventName, csDefaultEventName);
	if ((newState != STATE_NULL) && (nextState == STATE_NULL))
	{
		nextState = newState;
		csTempstr.Format(_T("=<%s>"), getStateName(nextState));
		csHookTraceBuffer += csTempstr;
	}

	while(lHookState <= HOOKSTATE_MAX)
	{
		trace(L7, _T("HOOKTRACE EVENT<%s>; STATE<%d>"), csEventName, lHookState);

		switch(lHookState)
		{
		case HOOKSTATE_INIT:
			csAltContext = _T("");
			nextState = STATE_NULL;
			csHookTraceBuffer = _T("");
			csHookTraceBuffer.Format(_T("END HOOKTRACE={INIT<%d:%s:%s>"), lHookSerial++, csEventName, csRealEventName);
			//trace(L5, _T("BEGIN HOOKTRACE"));
			if (pUSSFNative != NULL)
			{
				if (isUSSFMode())
				{
					pUSSFthis = this;
					csTempstr.Format(_T(":CONTEXT<%s>"), GetActiveContext());
					csHookTraceBuffer += csTempstr;
				}
				else
				{
					pUSSFthis = pUSSFNative;
					bool bRC = getUSSFDefaultContext(csAltContext);
					csTempstr.Format(_T(":CONTEXT<%s>"), csAltContext);
					csHookTraceBuffer += csTempstr;
				}
				pUSSFthis -> SetPendingNextStep(_T("0"));	// THEN/FINAL
				CString csTempstr;
				//csTempstr.Format(_T("Standard Event (%s) Mapped to Real Event (%s)."), csDefaultEventName, csRealEventName);
				//trace(L6, csTempstr);
				lHookState = HOOKSTATE_PRE;
			}
			else
			{
				lHookState = HOOKSTATE_NOUSSF;
				csTempstr.Format(_T(":NOUSSF"));
				csHookTraceBuffer += csTempstr;
				return nextState;
			}
			break;
		case HOOKSTATE_PRE:
			if (nextState == STATE_NULL)
			{
				csTempstr.Format(_T(":PRE"));
				csHookTraceBuffer += csTempstr;
				/* Check Pre<X> Event. */
				nextState = pUSSFthis -> ProcessEvent(_T("Pre") + csRealEventName, csAltContext);
				lHookState = HOOKSTATE_HANDLER;
			}
			else
			{
				lHookState = HOOKSTATE_FINAL;
				csTempstr.Format(_T(":NOTNULL=<%s>"), getStateName(nextState));
				trace(L6, _T("HOOKSTATE <%d>; Next State <%s>"), lHookState, csTempstr);
				csHookTraceBuffer += csTempstr;
				return nextState;
			}
			break;
		case HOOKSTATE_HANDLER:
			if (nextState == STATE_NULL){
				long x = pUSSFthis -> isHandled(csRealEventName, csAltContext, isUSSFMode());
				switch(x)
				{
				case 1:		/* BEFORE */
					csTempstr.Format(_T(":BEFORE"));
					csHookTraceBuffer += csTempstr;
					nextState = pUSSFthis -> ProcessEvent(csRealEventName, csAltContext);
					if (nextState == STATE_NULL)
					{
						lHookState = HOOKSTATE_BEFORE;
					}
					else
					{
						lHookState = HOOKSTATE_FINAL;
						csTempstr.Format(_T(":NOTNULL=<%s>"), getStateName(nextState));
						csHookTraceBuffer += csTempstr;
					}
					return nextState;
					break;
				case 2:		/* AFTER */
					csTempstr.Format(_T(":AFTER"));
					csHookTraceBuffer += csTempstr;
					lHookState = HOOKSTATE_AFTER;
					return nextState;
					break;
				case 3:		/* INSTEAD */
					csTempstr.Format(_T(":INSTEAD"));
					csHookTraceBuffer += csTempstr;
					nextState = pUSSFthis -> ProcessEvent(csRealEventName, csAltContext);
					lHookState = HOOKSTATE_INSTEAD;
					return nextState;
					break;
				case 0:		/* UNHANDLED */
					csTempstr.Format(_T(":UNHANDLED"));
					csHookTraceBuffer += csTempstr;
				default:
					lHookState = HOOKSTATE_UNHANDLED;
					return nextState;
					break;
				}
			}
			else
			{
				lHookState = HOOKSTATE_FINAL;
				csTempstr.Format(_T(":NOTNULL=<%s>"), getStateName(nextState));
				trace(L6, _T("HOOKSTATE <%d>; Next State <%s>"), lHookState, csTempstr);
				csHookTraceBuffer += csTempstr;
				return nextState;
			}
			break;
		case HOOKSTATE_INSTEAD:
			if (nextState == STATE_NULL)
			{
				lHookState = HOOKSTATE_POST;
			}
			else
			{
				csTempstr.Format(_T(":NOTNULL=<%s>"), getStateName(nextState));
				csHookTraceBuffer += csTempstr;
				lHookState = HOOKSTATE_FINAL;
				return nextState;
			}
			break;
		case HOOKSTATE_BEFORE:
			if (nextState == STATE_NULL)
			{
				lHookState = HOOKSTATE_POST;
			}
			else
			{
				csTempstr.Format(_T(":NOTNULL=<%s>"), getStateName(nextState));
				csHookTraceBuffer += csTempstr;
				lHookState = HOOKSTATE_FINAL;
				return nextState;
			}
			break;
		case HOOKSTATE_AFTER:
			if (nextState == STATE_NULL)
			{
				nextState = pUSSFthis -> ProcessEvent(csRealEventName, csAltContext);
				if (nextState == STATE_NULL)
				{
					lHookState = HOOKSTATE_POST;
				}
				else
				{
					csTempstr.Format(_T(":NOTNULL=<%s>"), getStateName(nextState));
					csHookTraceBuffer += csTempstr;
					trace(L6, _T("HOOKSTATE <%d>; Next State <%s>"), lHookState, csTempstr);
					lHookState = HOOKSTATE_FINAL;
					return nextState;
				}
			}
			else
			{
				csTempstr.Format(_T(":NOTNULL=<%s>"), getStateName(nextState));
				csHookTraceBuffer += csTempstr;
				lHookState = HOOKSTATE_FINAL;
				return nextState;
			}
			break;
		case HOOKSTATE_UNHANDLED:
			if (nextState == STATE_NULL)
			{
				lHookState = HOOKSTATE_POST;
			}
			else
			{
				csTempstr.Format(_T(":NOTNULL=<%s>"), getStateName(nextState));
				csHookTraceBuffer += csTempstr;
				lHookState = HOOKSTATE_FINAL;
				return nextState;
			}
			break;
		case HOOKSTATE_POST:
			if (nextState == STATE_NULL)
			{
				csTempstr.Format(_T(":POST"));
				csHookTraceBuffer += csTempstr;
				/* Check Post<X> Event. */
				nextState = pUSSFthis -> ProcessEvent(_T("Post") + csRealEventName, csAltContext);

				if (nextState == STATE_NULL)
				{
					// Decide whether to go to THEN or ELSE next!
					CString nextStep = pUSSFthis -> GetPendingNextStep();
					if (nextStep == _T("1") || nextStep == _T("ELSE"))
					{
						csTempstr.Format(_T(":ELSE"));
						csHookTraceBuffer += csTempstr;
						//trace(L5, _T("NextStep <ELSE>; Result=<%d>."), nextState);
						lHookState = HOOKSTATE_ELSE;
						return nextState;
					}
					else if (nextStep != _T("0")) /* BYPASS */
					{
						csTempstr.Format(_T(":SKIP"));
						csHookTraceBuffer += csTempstr;
						//trace(L5, csHookTraceBuffer);
						lHookState = HOOKSTATE_SKIP;
						return nextState;
					}
					else
					{
						csTempstr.Format(_T(":THEN"));
						csHookTraceBuffer += csTempstr;
						trace(L6, _T("HOOKSTATE <%d>; Next State <%s>"), lHookState, csTempstr);
						lHookState = HOOKSTATE_THEN;
						return nextState;
					}
				}
				else
				{
					csTempstr.Format(_T(":NOTNULL=<%s>"), getStateName(nextState));
					csHookTraceBuffer += csTempstr;
					lHookState = HOOKSTATE_FINAL;
					return nextState;
				}
			}
			break;
		case HOOKSTATE_THEN:
			lHookState = HOOKSTATE_FINAL;
			if (nextState != STATE_NULL)
			{
				csTempstr.Format(_T(":NOTNULL=<%s>"), getStateName(nextState));
				csHookTraceBuffer += csTempstr;
			}
			return nextState;
			break;
		case HOOKSTATE_SKIP:
			lHookState = HOOKSTATE_FINAL;
			if (nextState != STATE_NULL)
			{
				csTempstr.Format(_T(":NOTNULL=<%s>"), getStateName(nextState));
				csHookTraceBuffer += csTempstr;
			}
			return nextState;
			break;
		case HOOKSTATE_ELSE:
			lHookState = HOOKSTATE_FINAL;
			if (nextState != STATE_NULL)
			{
				csTempstr.Format(_T(":NOTNULL=<%s>"), getStateName(nextState));
				csHookTraceBuffer += csTempstr;
			}
			return nextState;
			break;
		case HOOKSTATE_NOUSSF:
			lHookState = HOOKSTATE_FINAL;
			return nextState;
			break;
		case HOOKSTATE_FINAL:
			if (nextState == STATE_USSF_FLUSHEVENT)
			{
				csTempstr.Format(_T(":END=<FLUSHED>"));
				nextState = STATE_NULL;
			}
			else
			{
				csTempstr.Format(_T(":END=<NORMAL>"));
			}
			csHookTraceBuffer += csTempstr;
			//trace(L5, csHookTraceBuffer);
			lHookState = HOOKSTATE_TERM;
			return nextState;
			break;
		case HOOKSTATE_TERM:
			//csTempstr.Format(_T("};"));
			//csHookTraceBuffer += csTempstr;
			lHookState = HOOKSTATE_MAX;
			return nextState;
			break;
		case HOOKSTATE_MAX:
			bDone = true;
			return nextState;
			break;
		default:
			lHookState++;
			break;
		}
	}

	return nextState;
}

SMStateBase *SMStateBase::StateEntry()
{
	//pState->GetRuntimeClass()->m_lpszClassName;
	//Need to set active context to be state name to have a chance to hook.
	// The context name to be set may be the same or different to the state name.
	// HOWEVER, this usually does not occur until state initialization.
	CString csHookTraceBuffer, csTempstr;

	SMStateBase *nextState = STATE_NULL;		
	if (pUSSFNative != NULL)									
	{
		CString csDefaultEventName = _T("StateEntry"), csRealEventName;
		USSFutil.ResolveSymbol(csDefaultEventName, csRealEventName, csDefaultEventName);
		//CString csTempstr;
		//csTempstr.Format(_T("Standard Event (%s) Mapped to Real Event (%s)."), csDefaultEventName, csRealEventName);
		//trace(L6, csTempstr);	

		csHookTraceBuffer.Format(_T("END ENTRYTRACE={INIT<%d:%s:%s>"), lHookSerial++, csDefaultEventName, csRealEventName);

		// Set the Active State [not Active Context] to the same value (state name).
		CString csStateName;
		bool bRC = getUSSFDefaultContext(csStateName);
		csTempstr.Format(_T(":CONTEXT<%s>"), csStateName);
		csHookTraceBuffer += csTempstr;

		//pUSSFNative -> SetActiveState(csStateName);
		//pUSSFNative -> SetOperationalState((SMStateBase *)this);
		if (isUSSFMode())
		{
			pUSSFthis = this;
		}
		else
		{
			pUSSFNative -> SetOperationalState((SMStateBase *)this);
			pUSSFthis = pUSSFNative;
			SetActivationState(csStateName, _T(""));
			MarkActivation(_T("FLMODE"));
			SaveActivationData();
		}
		pUSSFthis -> SetActiveState(csStateName);

		if (nextState == STATE_NULL)
		{
			csTempstr.Format(_T(":PRE"));
			csHookTraceBuffer += csTempstr;

			long x = pUSSFthis -> isHandled(_T("PreEntry"), csStateName, isUSSFMode());
			switch(x)
			{
			case 1:
			case 2:
			case 3:
				/******* DEPRECATED START *******/
				// Check PreEntry Event.
				////////////////////////////////////////////////////////
				//SetPendingEvent(_T("PreEntry"));		
				//nextState = pUSSFthis -> MakeNextMove(csStateName);	
				nextState = pUSSFthis -> ProcessEvent(_T("PreEntry"), csStateName);	
				////////////////////////////////////////////////////////
				/******* DEPRECATED END *******/
				break;
			default:
				// Check PreStateEntry Event.
				////////////////////////////////////////////////////////
				//SetPendingEvent(_T("Pre") + csRealEventName);		
				//nextState = pUSSFthis -> MakeNextMove(csStateName);		
				nextState = pUSSFthis -> ProcessEvent(_T("Pre") + csRealEventName, csStateName);		
				////////////////////////////////////////////////////////
				break;
			}
		}

		if (nextState == STATE_NULL)
		{
			long x = pUSSFthis -> isHandled(csRealEventName, csStateName, isUSSFMode());					
			switch(x)										
			{																				
				case 1:		// BEFORE			
					csTempstr.Format(_T(":BEFORE"));
					csHookTraceBuffer += csTempstr;
					//SetPendingEvent(csRealEventName);		
					//nextState = pUSSFthis -> MakeNextMove(csStateName);		
					nextState = pUSSFthis -> ProcessEvent(csRealEventName, csStateName);		
					if (nextState == STATE_NULL)			
					{							
						csTempstr.Format(_T("=<Initialize()>"));
						csHookTraceBuffer += csTempstr;
						nextState = Initialize();									
					}										
					break;
				case 2:		// AFTER				
					csTempstr.Format(_T(":AFTER=<Initialize()>"));
					csHookTraceBuffer += csTempstr;
					nextState = Initialize();									
					if (nextState == STATE_NULL)			
					{										
						//SetPendingEvent(csRealEventName);	
						//nextState = pUSSFthis -> MakeNextMove(csStateName);	
						nextState = pUSSFthis -> ProcessEvent(csRealEventName, csStateName);	
					}										
					break;
				case 3:		// INSTEAD										
					//SetPendingEvent(csRealEventName);		
					//nextState = pUSSFthis -> MakeNextMove(csStateName);	
					csTempstr.Format(_T(":INSTEAD"));
					csHookTraceBuffer += csTempstr;
					nextState = pUSSFthis -> ProcessEvent(csRealEventName, csStateName);		
					break;									
				case 0:		// Not Handled!
				default:	// Do the regular FastLane thing!
					csTempstr.Format(_T(":UNHANDLED=<Initialize()>"));
					csHookTraceBuffer += csTempstr;
					nextState = Initialize();
					break;	
			}	
		}

		if (nextState == STATE_NULL)
		{
			csTempstr.Format(_T(":POST"));
			csHookTraceBuffer += csTempstr;

			long x = pUSSFthis -> isHandled(_T("PostEntry"), csStateName, isUSSFMode());
			switch(x)
			{
			case 1:
			case 2:
			case 3:
				/******* DEPRECATED START *******/
				// Check PostEntry Event.
				////////////////////////////////////////////////////////
				//SetPendingEvent(_T("PostEntry"));		
				//nextState = pUSSFthis -> MakeNextMove(csStateName);		
				nextState = pUSSFthis -> ProcessEvent(_T("PostEntry"), csStateName);		
				////////////////////////////////////////////////////////
				/******* DEPRECATED END *******/
				break;
			default:
				// Check PostStateEntry Event.
				////////////////////////////////////////////////////////
				//SetPendingEvent(_T("Post") + csRealEventName);		
				//nextState = pUSSFthis -> MakeNextMove(csStateName);		
				nextState = pUSSFthis -> ProcessEvent(_T("Post") + csRealEventName, csStateName);		
				////////////////////////////////////////////////////////
				break;
			}
		}
	}
	else												
	{
		csTempstr.Format(_T(":NOUSSF=<Initialize()>"));
		csHookTraceBuffer += csTempstr;
		nextState = Initialize();												
	}

	if ((nextState > STATE_ERROR) || (nextState < 0))
	{
		if (!nextState -> isUSSFMode())
		{
			bIgnoreUnknownAction = false;
		}
	}

	if (nextState != STATE_NULL)
	{
		csTempstr.Format(_T(":NOTNULL=<%s>"), getStateName(nextState));
		csHookTraceBuffer += csTempstr;
	}
	csTempstr.Format(_T(":END};"));
	csHookTraceBuffer += csTempstr;

	if (co.fOperationsEnableUSSF)
		trace(L5, csHookTraceBuffer);
	else
		trace(L7, csHookTraceBuffer);

	return nextState;								
}

void SMStateBase::StateExit()
{
	CString csHookTraceBuffer, csTempstr;
	SMStateBase *nextState = STATE_NULL;
	
	if (pUSSFNative != NULL)									
	{	
		CString csDefaultEventName = _T("StateExit"), csRealEventName;
		USSFutil.ResolveSymbol(csDefaultEventName, csRealEventName, csDefaultEventName);
		//CString csTempstr;
		//csTempstr.Format(_T("Standard Event (%s) Mapped to Real Event (%s)."), csDefaultEventName, csRealEventName);
		//trace(L6, csTempstr);

		csHookTraceBuffer.Format(_T("END EXITTRACE={INIT<%d:%s:%s>"), lHookSerial++, csDefaultEventName, csRealEventName);

		CString csStateName;
		bool bRC = getUSSFDefaultContext(csStateName);
		csTempstr.Format(_T(":CONTEXT<%s>"), csStateName);
		csHookTraceBuffer += csTempstr;

		if (isUSSFMode())
		{
			pUSSFthis = this;
		}
		else
		{
			pUSSFthis = pUSSFNative;
		}

		if (nextState == STATE_NULL)
		{
			csTempstr.Format(_T(":PRE"));
			csHookTraceBuffer += csTempstr;

			long x = pUSSFthis -> isHandled(_T("PreExit"), csStateName, isUSSFMode());
			switch(x)
			{
			case 1:
			case 2:
			case 3:
				/******* DEPRECATED START *******/
				// Check PreExit Event.
				////////////////////////////////////////////////////////
				//SetPendingEvent(_T("PreExit"));		
				//nextState = pUSSFthis -> MakeNextMove(csStateName);		
				nextState = pUSSFthis -> ProcessEvent(_T("PreExit"), csStateName);		
				////////////////////////////////////////////////////////
				/******* DEPRECATED END *******/
				break;
			default:
				// Check PreStateExit Event.
				////////////////////////////////////////////////////////
				//SetPendingEvent(_T("Pre") + csRealEventName);		
				//nextState = pUSSFthis -> MakeNextMove(csStateName);		
				nextState = pUSSFthis -> ProcessEvent(_T("Pre") + csRealEventName, csStateName);		
				////////////////////////////////////////////////////////
				break;
			}
		}

		if (nextState == STATE_NULL)
		{
			long x = pUSSFthis -> isHandled(csRealEventName, csStateName, isUSSFMode());					
			switch(x)										
			{																			
				case 1:		// BEFORE										
					//SetPendingEvent(csRealEventName);		
					//nextState = pUSSFthis -> MakeNextMove(csStateName);
					csTempstr.Format(_T(":BEFORE"));
					csHookTraceBuffer += csTempstr;
					nextState = pUSSFthis -> ProcessEvent(csRealEventName, csStateName);		
					if (nextState == STATE_NULL)			
					{
						csTempstr.Format(_T("=<UnInitialize()>"));
						csHookTraceBuffer += csTempstr;
						UnInitialize();									
					}										
					break;
				case 2:		// AFTER	
					csTempstr.Format(_T(":AFTER=<UnInitialize()>"));
					csHookTraceBuffer += csTempstr;
					UnInitialize();									
					if (nextState == STATE_NULL)			
					{										
						//SetPendingEvent(csRealEventName);	
						//nextState = pUSSFthis -> MakeNextMove(csStateName);	
						nextState = pUSSFthis -> ProcessEvent(csRealEventName, csStateName);	
					}										
					break;	
				case 3:		// INSTEAD						
					//SetPendingEvent(csRealEventName);		
					//nextState = pUSSFthis -> MakeNextMove(csStateName);	
					csTempstr.Format(_T(":INSTEAD"));
					csHookTraceBuffer += csTempstr;
					nextState = pUSSFthis -> ProcessEvent(csRealEventName, csStateName);		
					break;
				case 0:		// Not Handled!
				default:	// Do the regular FastLane thing!
					csTempstr.Format(_T(":UNHANDLED=<UnInitialize()>"));
					csHookTraceBuffer += csTempstr;
					UnInitialize();										
					break;		
			}
		}

		if (nextState == STATE_NULL)
		{
			csTempstr.Format(_T(":POST"));
			csHookTraceBuffer += csTempstr;

			long x = pUSSFthis -> isHandled(_T("PostExit"), csStateName, isUSSFMode());
			switch(x)
			{
			case 1:
			case 2:
			case 3:
				/******* DEPRECATED START *******/
				// Check PostExit Event.
				////////////////////////////////////////////////////////
				//SetPendingEvent(_T("PostExit"));		
				//nextState = pUSSFthis -> MakeNextMove(csStateName);		
				nextState = pUSSFthis -> ProcessEvent(_T("PostExit"), csStateName);		
				////////////////////////////////////////////////////////
				/******* DEPRECATED END *******/
				break;
			default:
				// Check PostStateExit Event.
				////////////////////////////////////////////////////////
				//SetPendingEvent(_T("Post") + csRealEventName);		
				//nextState = pUSSFthis -> MakeNextMove(csStateName);		
				nextState = pUSSFthis -> ProcessEvent(_T("Post") + csRealEventName, csStateName);		
				////////////////////////////////////////////////////////
				break;
			}
		}
	}													
	else												
	{
		csTempstr.Format(_T(":NOUSSF=<UnInitialize()>"));
		csHookTraceBuffer += csTempstr;
		UnInitialize();												
	}
	
	// Here, we don't really need nextState since the return type is void.
	// If nextState is set to a real state, we must delete it to avoid a leak!
	if ((nextState > STATE_ERROR) || (nextState < 0))
	{
		delete nextState;
	}

	if (!isUSSFMode())
	{
		if (pUSSFNative != NULL)
		{
			pUSSFNative -> SetOperationalState((SMStateBase *)pUSSFNative);
		}
	}

	csTempstr.Format(_T(":END};"));
	csHookTraceBuffer += csTempstr;
	if (co.fOperationsEnableUSSF)
		trace(L5, csHookTraceBuffer);
	else
		trace(L7, csHookTraceBuffer);

	return;						
}
///////////////////////////////////////////////////////////////////

bool SMStateBase::getUSSFDefaultContext(CString &csDefaultContext)
{
	csDefaultContext = this->GetRuntimeClass()->m_lpszClassName;
	return true;
}

CString SMStateBase::getStateName(const SMStateBase * const pState)
{
   int classType = (int)pState;
   switch (classType)
   {
   case (int)STATE_NULL:
       return _T("STATE_NULL");
       break;
   case (int)STATE_STOP:
       return _T("STATE_STOP");
       break;
   case (int)STATE_RETURN:
       return _T("STATE_RETURN");
       break;
   case (int)STATE_RESUME:
       return _T("STATE_RESUME");
       break;
   case (int)STATE_RESTART:
       return _T("STATE_RESTART");
       break;
   case (int)STATE_ENDED:
       return _T("STATE_ENDED");
       break;
   case (int)STATE_ERROR:
       return _T("STATE_ERROR");
       break;
   default:
       return pState->GetRuntimeClass()->m_lpszClassName;
       break;
   }
} 

SMStateBase *SMStateBase::InitializeUSSF(void)
{
  trace(L5, _T("SMStateBase::InitializeUSSF()"));

  SMStateBase *returnState = STATE_NULL;

  fItemOverride = false;

  if (pUSSFNative == NULL)
  //if (pUSSFNative == NULL)
  {
	trace(L5, _T("SMStateBase::InitializeUSSF() <pUSSFNative=NULL>"));

	// Set Transaction Variable to Default Name of USSFExit DLL
	CPSX *m_pPSX = ps.GetPSXObject();

#ifdef _DEBUG
#ifdef _UNICODE
	csExitName = _T("USSFExitUD.dll");
#else
	csExitName = _T("USSFExitD.dll");
#endif
#else
#ifdef _UNICODE
	csExitName = _T("USSFExitU.dll");
#else
	csExitName = _T("USSFExit.dll");
#endif
#endif

	USSFutil.SetTransactionVariable(_T("USSF_EXITNAME"), csExitName);
    USSFutil.SetTransactionVariable(_T("USSF_EXITVERSION"), csExitVersion);


	typedef SMStateBase *(*ExitInitialize) (CPSX*, PSProceduresBase &);

	MAKEGENERICSTATE(SMUSSFManager, pUSSFNative);
	// Following line is redundant - constructor should have already done it!
	pUSSFNative -> SetOperationalState((SMStateBase *)pUSSFNative);

	trace(L5, _T("SMStateBase::InitializeUSSF() <Created pUSSFNative>"));

	// Load the Display Shop XML.
	if (myDisplayShop == NULL)
	{
		myDisplayShop = new DisplayShop();
	}

	myDisplayShop -> LoadDefinition();

	USSFutil.ResolveSymbol(_T("@ExitName"), csExitName);
	USSFutil.ResolveSymbol(_T("@ExitVersion"), csExitVersion);

	try
	{
		// Check for a method in USSFExit.dll
		//HMODULE hMod = LoadLibrary(_T("C:\\Scot\\bin\\USSFExit.dll"));
		hUSSFExit = LoadLibrary(csExitName);
		if (hUSSFExit != NULL)
		{
			trace(L5, _T("%s Loaded."), csExitName);
			ExitInitialize fProc= (ExitInitialize)GetProcAddress( hUSSFExit, (LPCSTR)"Initialize" );
			if (fProc != NULL)
			{
				// Call the User Exit!
				CPSX *m_pPSX = ps.GetPSXObject();
				trace(L5, _T("Calling %s(Initialize)"), csExitName);
				//SMStateBase *rc = fProc(m_pPSX, ps);
				returnState = fProc(m_pPSX, ps);
				if (returnState == STATE_NULL)
				{
					trace(L5, _T("%s(Initialize) Returned STATE_NULL."), csExitName);
				}
				else if (returnState == STATE_STOP)
				{
					FreeLibrary(hUSSFExit);
					hUSSFExit = NULL;
					trace(L5, _T("%s Loaded; Invalid Version Detected."), csExitName);
					returnState = STATE_NULL;		// Do NOT actually stop - just disable library.
				}
				else
				{
					trace(L5, _T("%s(Initialize) DID NOT Return STATE_NULL."), csExitName);
				}
			}
		}
		else
		{
			trace(L5, _T("Failed to Load %s"), csExitName);
		}
	}
	catch(...)
	{
		trace(L5, _T("Exception Within %s(Initialize)"), csExitName);
	}
  }
  return returnState;
}

void SMStateBase::UnInitializeUSSF(void)
{
  SMStateBase *returnState = STATE_NULL;

  try
  {
	typedef SMStateBase *(*ExitUnInitialize) (void);

	// Check for a method in USSFExit.dll
	//HMODULE hMod = LoadLibrary(_T("C:\\Scot\\bin\\USSFExit.dll"));
	if (hUSSFExit != NULL)
	{
		trace(L5, _T("%s Currently Loaded."), csExitName);
		ExitUnInitialize fProc= (ExitUnInitialize)GetProcAddress( hUSSFExit, (LPCSTR)"UnInitialize" );
		if (fProc != NULL)
		{
			// Call the User Exit!
			trace(L5, _T("Calling %s(UnInitialize)"), csExitName);
			SMStateBase *returnState = fProc();
			if (returnState == STATE_NULL)
			{
				trace(L5, _T("%s(UnInitialize) Returned STATE_NULL."), csExitName);
			}
			else
			{
				trace(L5, _T("%s(UnInitialize) DID NOT Return STATE_NULL."), csExitName);
			}
		}
	}
	else
	{
		trace(L5, _T("%s <%s> Not Loaded."), csExitName, csExitVersion);
	}
  }
  catch(...)
  {
	  trace(L5, _T("Exception Within %s(UnInitialize)"), csExitName);
  }

  if (pUSSFNative != NULL)
  {
	  delete pUSSFNative;
  }
}

SMStateBase *SMStateBase::MakeUSSFState(void)
{
#ifdef _BUILDLIB_
	CRuntimeClass* pSMUSSFManager = RUNTIME_CLASS(SMUSSFManagerBase);
	CObject* poSMUSSFManager = pSMUSSFManager->CreateObject();
	return (SMStateBase*)((SMUSSFManagerBase*) poSMUSSFManager);
#else
	CRuntimeClass* pSMUSSFManager = RUNTIME_CLASS(SMUSSFManager);
	CObject* poSMUSSFManager = pSMUSSFManager->CreateObject();
	return (SMStateBase*)((SMUSSFManager*) poSMUSSFManager);	
#endif
}

long SMStateBase::StoreAPPLData(CString name, COleVariant vValue, CString prefix)
{
  USES_CONVERSION;

  name = prefix + name;
  long rc = USSFutil.SetTransactionVariable(name, vValue);
  vValue.ChangeType(VT_BSTR);
  CString csValue = OLE2T(vValue.bstrVal);
  CString csTempstr;
  csTempstr.Format(_T("StoreAPPLData<%s%s=%s>"), prefix, name, csValue);
  trace(L7, csTempstr);
  return rc;

}

long SMStateBase::StoreUSSFData(CString name, COleVariant vValue)
{
  USES_CONVERSION;

  name.MakeUpper();
  name = _T("USSF_") + name;
  long rc = USSFutil.SetTransactionVariable(name, vValue);
  vValue.ChangeType(VT_BSTR);
  CString csValue = OLE2T(vValue.bstrVal);
  CString csTempstr;
  csTempstr.Format(_T("StoreUSSFData<%s=%s>"), name, csValue);
  trace(L7, csTempstr);
  return rc;

}

int SMStateBase::DoTranslationFromWCToMB( char *ch, const wchar_t *wChar, size_t count,
											 bool bIgnoreNull )
{
	int i=0;
	int noOfChar = 0;	
	
	for( int k=0;k<count;k++ )
	{
		i = wctomb(ch, *wChar);		

		printf("Multibyte Result : %x\n", *ch);
		if( i < 0 )
			return noOfChar;
		else
			noOfChar += i;

		// check if it's null terminator
		if( *ch == '\x0' )
		{
			// if it doesn't ignore the null terminator then returns the number of
			// translated character
			if( !bIgnoreNull )
			{
				return noOfChar;
			}			
		}	
		ch += i;
		
		wChar++;					
		
		printf("NoOf Multibyte Character : %d\n", noOfChar);
	}	
	return noOfChar;
}

long SMStateBase::ShowPOSDisplay(CString csLine1, CString csLine2)
{
	ps.Echo((LPCTSTR)csLine1,(LPCTSTR)csLine2);
	USSFutil.SetTransactionVariable(_T("EchoTOP"), (LPCTSTR)csLine1);
	USSFutil.SetTransactionVariable(_T("EchoBOT"), (LPCTSTR)csLine2);

	return 0;
}

long SMStateBase::ClearPOSDisplay(void)
{
	long rc = ShowPOSDisplay(_T(""), _T(""));
	TBShow2x20();
	return rc;
}

long SMStateBase::SendKeySequence(CString csSequence, int iSleep)
{
	long rc = 0;

	for (int i = 0; i < csSequence.GetLength(); i++)
	{
		//TBKeyPress(csSequence[i]);
		SendKeySequence(csSequence[i], iSleep);
		//if (iSleep > 0)
		//{
		//	Sleep(iSleep);
		//}
	}

	return rc;
}

long SMStateBase::SendKeySequence(unsigned char ucSequence, int iSleep)
{
	long rc = 0;

	TBKeyPress(ucSequence);
	if (iSleep > 0)
	{
		Sleep(iSleep);
	}

	return rc;
}

long SMStateBase::SendPOSData(CString csSource)
{
  trace(L6, _T("SendPOSData(<%s>)"), csSource);

  long rc = 0;

  CString csTempstr, csTarget;

  USSFutil.ResolveSymbol(csSource, csTempstr);
  USSFutil.ParsePOSData(csTempstr, csTarget);

  for (int i = 0; i < csTarget.GetLength(); i++)
  {
	TBKeyPress((unsigned char)csTarget[i]);
  }

  trace(L6, _T("SendPOSData() Returns <%d>."), rc);

  return rc;
}

void SMStateBase::SetPendingEvent(CString event)
{
   trace(L7, _T("%s Event Pending."), event);

   USSFutil.SetTransactionVariable(_T("USSF_PendingEvent"), event);
}

SMStateBase *SMStateBase::ValidateContext(CString csContextName)
{
	SMStateBase *returnState = STATE_NULL;

	if (csContextName != _T(""))
	{
		if (vcs_ContextList.GetSize() == 0)
		{
			if( PSX_SUCCESS != m_pGAPSX->GetContextNameList( vcs_ContextList ) )
			{
				trace(L5, _T("Could Not Get Context List"));
			}
		}
		if (vcs_ContextList.GetSize() == 0)
		{
			// Check against full list of contexts.
			// If not in the list, then Internal Error.
			bool bFlag = false;
			for (int i = 0; i < vcs_ContextList.GetSize(); i++)
			{
				if (vcs_ContextList[i] == csContextName)
				{
					bFlag = true;
					break;
				}
			}
			if (!bFlag)
			{
				// Throw the error.
				//bTransitionLock = false;
				trace(L6,_T("Specified Context Does Not Exist (%s)."), csContextName);
				CString csValue;
				csValue.Format(_T("Context Does Not Exist=<%s>."), csContextName);
				ERRHANDLE(_T("ContextError"), _T("Internal Error"), _T("SMStateUSSFBase::ValidateContext()"), csValue);
			}
		}
	}

	return returnState;
}

CString SMStateBase::GetActiveState(void)
{
   trace(L7, _T("SMStateBase::GetActiveState(void)"));
   CString csActiveState;
   USSFutil.GetTransactionVariable(_T("USSFActiveState"), csActiveState);
   return csActiveState;

}

CString SMStateBase::GetActiveContext(void)
{
  trace(L7, _T("SMStateBase::GetActiveContext(void)"));
  CString csActiveContext;
  USSFutil.GetTransactionVariable(_T("USSFActiveContext"), csActiveContext);
  return csActiveContext;

}

void SMStateBase::ClearActivationDB(void)
{
  trace(L5, _T("SMStateBase::ClearActivationDB(void)"));

  if (!ActivationDB.empty())
  {
    ActivationDB.erase(ActivationDB.begin(), ActivationDB.end() - 1);
  }
}

void SMStateBase::SaveActivationData(void)
{
	
  trace(L7, _T("SMStateBase::SaveActivationData(void)"));
  /*

  // Push existing activation.
  thisActivation.csTimeStamp = _T(__TIMESTAMP__);
  thisActivation.bInScope = true;
  thisActivation.lSerial = lActivationSerial++;
  //thisActivation.lLinkage = -1;
  trace(L7, _T("Saving Activation Record: state = %s, context = %s; ref = %s"), thisActivation.CurrentState,
            thisActivation.CurrentContext, thisActivation.ReferenceKey);
  ActivationDB.push_back(thisActivation);

  // Finally, Create New Activation Record.
  thisActivation.lLinkage = -1;
  thisActivation.ReferenceKey = _T("");
  SetActivationState();
  */
  return;
}

void SMStateBase::SetActivationState(CString state, CString context)
{
  trace(L7, _T("SMStateBase::SetActivationState(CString state, CString context)"));

  // Make new activation record.
  trace(L7, _T("Setting Activation Record: state = %s; context = %s"), state, context);
  thisActivation.CurrentState = state;
  thisActivation.CurrentContext = context;
}

void SMStateBase::SetActivationState(void)
{
  trace(L7, _T("SMStateBase::SetActivationState(void)"));

  // Make new activation record.
  trace(L7, _T("Setting Activation Record: state = %s; context = %s"), GetActiveState(), GetActiveContext());
  thisActivation.CurrentState = GetActiveState();
  thisActivation.CurrentContext = GetActiveContext();
}

void SMStateBase::MarkActivation(CString csReference)
{
  trace(L7, _T("SMStateBase::MarkActivation(CString csReference)"));

  thisActivation.ReferenceKey = csReference;
}

void SMStateBase::SetPendingNextStep(CString step)
{
  trace(L7, _T("SMStateBase::SetPendingNextStep(CString step)"));

  step.MakeUpper();
  USSFutil.SetTransactionVariable(_T("USSF_NEXTSTEP"), step);

}

CString SMStateBase::GetPendingNextStep(void)
{
  trace(L7, _T("SMStateBase::GetPendingNextStep(void)"));

  CString csNextStep;
  USSFutil.GetTransactionVariable(_T("USSF_NEXTSTEP"), csNextStep);
  return csNextStep;

}

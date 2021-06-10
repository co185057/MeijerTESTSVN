#ifndef _UNIVERSALSSF
#define _UNIVERSALSSF

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

#ifdef _BUILDLIB_
  #define MAKEGENERICSTATE(a,b)								\
		CRuntimeClass* p##a = RUNTIME_CLASS(##a##Base);		\
		CObject* po##a = p##a->CreateObject();				\
		b = (SMStateBase*)((##a##Base*) po##a);
#else
  #define MAKEGENERICSTATE(a,b)							\
		CRuntimeClass* p##a = RUNTIME_CLASS(##a);		\
		CObject* po##a = p##a->CreateObject();			\
		b = (SMStateBase*)((a*) po##a);	
#endif

#define MAKEANCHORSTATE(stateA, stateB)	\
	if (nextState.Find(_T(stateA)) >= 0)	\
	{										\
		trace(L5, _T("Returning State <%s>."), stateB);	\
		setAnchorState(stateB);			\
		return STATE_NULL; \
	}

#define CREATENEWSTATE(stateA, stateB)	\
  if (nextState.Find(_T(stateA)) >= 0)	\
  {										\
	trace(L5, _T("Returning State <%s>."), stateB);	\
	return createState(stateB);			\
  }

#define GETUSSFEVENTDATA(event, basecontext, altcontext, directive, variable)	\
	CString variable;	\
	CString cs##variable##Default;	\
	{	\
		m_pGAPSX -> GetCustomDataVar(event + directive, cs##variable##Default, _T(""), csUSSFDefaultContext);	\
		trace(L8, _T("Default <%s> = <%s>."), event + directive, cs##variable##Default);	\
		if (altcontext == _T(""))	\
		{	\
			m_pGAPSX -> GetCustomDataVar(event + directive, variable, _T(""), basecontext); \
		}	\
		else	\
		{	\
			m_pGAPSX -> GetCustomDataVar(event + directive, variable, _T(""), altcontext); \
		}	\
		USSFutil.ResolveSymbol(variable, variable, cs##variable##Default);	\
		trace(L8, _T("<%s> = <%s>."), event + directive, variable);	\
	}

#define GETUSSFCONTEXTDATA(basecontext, altcontext, directive, variable)	\
	CString cs##variable##Default;	\
	{	\
		m_pGAPSX -> GetCustomDataVar(directive, cs##variable##Default, _T(""), csUSSFDefaultContext);	\
		trace(L8, _T("Default <%s> = <%s>."), directive, cs##variable##Default);	\
		if (altcontext == _T(""))	\
		{	\
			m_pGAPSX -> GetCustomDataVar(directive, variable, _T(""), basecontext); \
		}	\
		else	\
		{	\
			m_pGAPSX -> GetCustomDataVar(directive, variable, _T(""), altcontext); \
		}	\
		USSFutil.ResolveSymbol(variable, variable, cs##variable##Default);	\
		trace(L8, _T("<%s> = <%s>."), directive, variable);	\
	}

// a - data source - 
// b - data target - PSX Transaction Variable.
#define APPLDATA(a, b)		\
	{														\
		if (pUSSFNative != NULL)							\
		{													\
			pUSSFNative -> StoreAPPLData(a, b, _T(""));		\
		}													\
	}			

// a - data source - 
// b - data target - PSX Transaction Variable.
#define USSFDATA(a, b)		\
	{														\
		if (pUSSFNative != NULL)							\
		{													\
			pUSSFNative -> StoreUSSFData(a, b);				\
		}													\
	}														

#define USSFHOOK(a, b, c)	\
{\
	SMStateBase *nextState; \
	if (co.fOperationsEnableUSSF) \
	{ \
	long lHookState = -1;	\
	CString csTempstr;	\
	bool bDone = false;	\
	SMStateBase *unhandledState = STATE_NULL;	\
	nextState = HookEntry(a, lHookState, bDone, unhandledState);	\
	while(!bDone)	\
	{\
		switch(lHookState)	\
		{\
			case HOOKSTATE_BEFORE:	\
			case HOOKSTATE_AFTER:	\
			case HOOKSTATE_UNHANDLED:	\
			case HOOKSTATE_NOUSSF:	\
				csTempstr.Format(_T("=<%s>"), _T(#b));	\
				csHookTraceBuffer += csTempstr;	\
				trace(L7, csHookTraceBuffer);\
				b;	\
				unhandledState = nextState; \
				break;	\
			case HOOKSTATE_TERM:	\
				csTempstr.Format(_T("=<%s>};"), _T(#c));	\
				csHookTraceBuffer += csTempstr;	\
				trace(L7, csHookTraceBuffer);\
				break;	\
		}\
		nextState = HookEntry(a, lHookState, bDone, unhandledState);	\
	}\
	csHookTraceBuffer.Empty(); \
	c;	\
	} \
	else \
	{ \
	b; \
	c; \
	}\
}

#define USSFEVENT(a, b, c)	\
{\
	long lHookState = -1;	\
	CString csTempstr;	\
	bool bDone = false;	\
	SMStateBase *unhandledState = STATE_NULL;	\
	SMStateBase *nextState = HookEntry(a, lHookState, bDone, unhandledState);	\
	while(!bDone)	\
	{\
		switch(lHookState)	\
		{\
			case HOOKSTATE_NOUSSF:	\
				csTempstr.Format(_T("=<%s>"), _T(#b));	\
				csHookTraceBuffer += csTempstr;	\
				b;	\
				break;	\
			case HOOKSTATE_THEN:	\
				csTempstr.Format(_T("=<%s>"), _T(#b));	\
				csHookTraceBuffer += csTempstr;	\
				b;	\
				break;	\
			case HOOKSTATE_ELSE:	\
				csTempstr.Format(_T("=<%s>"), _T(#c));\
				csHookTraceBuffer += csTempstr;\
				c;	\
				break;	\
			case HOOKSTATE_TERM:	\
				csTempstr.Format(_T("};"));	\
				csHookTraceBuffer += csTempstr;	\
				trace(L7, csHookTraceBuffer);\
				break;	\
		}\
		nextState = HookEntry(a, lHookState, bDone, unhandledState);	\
	}\
	csHookTraceBuffer.Empty(); \
}

#define ERRHANDLE(event, message, method, loc) \
{ \
  CString csTempstr, csType, csMessage, csMethod, csInfo, csState, csContext;	\
  csType.Format(_T("Type=<%s>;"), event);	\
  csMessage.Format(_T("Msg=<%s>;"), message);	\
  csMethod.Format(_T("Module=<%s>;"), method);	\
  csInfo.Format(_T("Info=<%s>;"), loc);	\
  csState.Format(_T("State=<%s>;"), GetActiveState());	\
  csContext.Format(_T("Context=<%s>;"), GetActiveContext());	\
  csTempstr = csType + '\n' + csMessage + '\n' + csMethod + '\n' + csInfo + '\n' + csState + '\n' + csContext;	\
  USSFutil.SetTransactionVariable(_T("USSF_ERRORMESSAGE"), csTempstr); \
  csTempstr = csType + csMessage + csMethod + csInfo + csState + csContext;	\
  trace(L6, csTempstr); \
  /*SetPendingEvent(event);*/  \
  /*return MakeNextMove();*/ \
  return ProcessEvent(event);	\
}

#define USSF_ACTION(name, function)	\
if (!thisAction.CompareNoCase(name))	\
{	\
    trace(L6, _T("Processing Action: %s"), name); \
	bActionTaken = true;	\
	returnState = USSFutil.USSF_##function##_Action(pOperationalState, myArgs);	\
}

#define USSF_LOCAL_ACTION(name, function)	\
if (!thisAction.CompareNoCase(name))	\
{	\
    trace(L6, _T("Processing Action: %s"), name); \
	bActionTaken = true;	\
	returnState = USSF_##function##_Action(pOperationalState, myArgs);	\
}

#define USSF_USEREXIT_ACTION(name, function, args)	\
try {	\
	if (!thisAction.CompareNoCase(name))	\
	{	\
		bActionTaken = true;	\
		returnState = USSFutil.USSF_##function##_Action(args);	\
	}	\
}	\
catch(...)	\
{	\
	returnState = -1;	\
}

#endif
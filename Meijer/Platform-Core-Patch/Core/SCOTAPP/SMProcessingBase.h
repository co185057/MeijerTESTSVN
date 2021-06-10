//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMProcessingBase.h
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PROCESSBASESTATE
#define _PROCESSBASESTATE

#include "SMHelpModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

SMProcessingBase : public STATE(HelpModeState)
{

public:
	SMProcessingBase();
	virtual SMStateBase  *Initialize(void);
	
protected:
	DECLARE_DYNCREATE(SMProcessingBase)// MFC Runtime class/object information
};

#endif

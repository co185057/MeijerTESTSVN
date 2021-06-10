//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAssistMode.h
//
// TITLE: Class header for Assist mode keyboard entry mode
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSMASSISTMODE
#define _SMSMASSISTMODE

#include "SMSmAssistModeBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmAssistMode : public SMSmAssistModeBase
{
public:
	SMSmAssistMode();
	// virtual SMStateBase  *Initialize();
protected:
	virtual SMStateBase  *PSAssistKey(const long nKey);// Assist Mode or Stop program
	virtual SMStateBase  *PSKeystroke(const long wParam, const long lParam);
    virtual SMStateBase  *PSNumericKey(const long lKey);
    virtual SMStateBase  *PSAlphaNumericKey(const long);

  DECLARE_DYNCREATE(SMSmAssistMode) // MFC Runtime class/object information
};

#endif
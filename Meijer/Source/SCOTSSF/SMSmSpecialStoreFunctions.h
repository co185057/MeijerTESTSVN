//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSpecialStoreFunctions.h
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// Start of JZB.SSF.5.0.0
// -- This class added to manage the upper level of Tesco Report Buttons
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmSpecialStoreFunctionsSTATE
#define _SmSpecialStoreFunctionsSTATE

#include "SMSmSystemFunctionsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmSpecialStoreFunctions : public SMSmSystemFunctionsBase
{
public:
	SMSmSpecialStoreFunctions();
	void showScreen(const bool traceOn);
    SMStateBase  *PSButton1(void);	
    SMStateBase  *PSButton2(void);	
    SMStateBase  *PSButton3(void);	
    SMStateBase  *PSButton8(void);	
  DECLARE_DYNCREATE(SMSmSpecialStoreFunctions)// MFC Runtime class/object information
};

#endif
// End of JZB.SSF.5.0.0
//////////////////////////////////////////////////////////////////////////////////////////////////

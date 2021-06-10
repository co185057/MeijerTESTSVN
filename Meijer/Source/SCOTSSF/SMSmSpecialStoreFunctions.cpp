//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSpecialStoreFunctions.CPP
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// Start of JZB.SSF.5.0.0
// -- This class added to manage the upper level of Tesco Report Buttons
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSmSpecialStoreFunctions.h"
#include "SMSmGetSpecialFuncValue.h"
#include "SMSmSelectTillPickUpFunction.h"
#include "SMSmSystemFunctions.h"
#include "SMInProgress.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmSpecialStoreFunctions")

// global
int nSpecialFuncNum = 0;

IMPLEMENT_DYNCREATE(SMSmSpecialStoreFunctions, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSmSpecialStoreFunctions::SMSmSpecialStoreFunctions()
{
    IMPLEMENT_TIMESTAMP
}

void SMSmSpecialStoreFunctions::showScreen(const bool traceOn)
{
    ps.SetFrame(STORE_MODE_FRAME);
    ps.SetBackground(BMP_SMBLANK);
    ps.ShowFrame();
    ps.ShowSMTotal(false);
    ps.SMShowItemReceipt(false);
    ps.SetTitle(TTL_SYSTEMFUNCTIONS, PS_BLANK);
    ps.SetLeadthruText(SSF_BTT_SPECIALREPORTS, PS_BLANK);
    ps.Prompt(PS_NOTEXT);

    ps.SMButton(_T("SMButton1"), SSF_BTT_TILLPICKUP,true); 
    ps.SMButton(_T("SMButton2"), SSF_BTT_ROA,true); 
    ps.SMButton(_T("SMButton3"), SSF_BTT_TILLCHECKREADING,true);  
    ps.SMButton(_T("SMButton4"), PS_NOTEXT,false); //No 
    ps.SMButton(_T("SMButton5"), PS_NOTEXT,false); //No 
    ps.SMButton(_T("SMButton6"), PS_NOTEXT,false); //No 
    ps.SMButton(_T("SMButton7"), PS_NOTEXT,false); //No 
    ps.SMButton(_T("SMButton8"), BTT_GOBACK, true);
}

/////////////////////////////////////////////
// Button 1 - Till Pick Up
/////////////////////////////////////////////
SMStateBase  *SMSmSpecialStoreFunctions::PSButton1(void)
{
    nSpecialFuncNum = SPEC_FUNC_TILLPICKUP;
    RETURNSTATE(SmSelectTillPickUpFunction)
}

/////////////////////////////////////////////
// Button 2 - ROA
/////////////////////////////////////////////
SMStateBase  *SMSmSpecialStoreFunctions::PSButton2(void)
{
    nSpecialFuncNum = SPEC_FUNC_ROA;
    RETURNSTATE(SmGetSpecialFuncValue)
}

/////////////////////////////////////////////
// Button 3 - Till Check Reading
/////////////////////////////////////////////
SMStateBase  *SMSmSpecialStoreFunctions::PSButton3(void)
{
	trace(L6,_T("JZB.SSF.5.0.1 (TBHandleReportsFunction %d, 0, NULL"), SPEC_FUNC_TILLCHECKREADING);
    TBHandleReportsFunction(SPEC_FUNC_TILLCHECKREADING, 0, 0);
    RETURNSTATE(SmSystemFunctions);
}

/////////////////////////////////////////////
// Button 8 - Go Back
/////////////////////////////////////////////
SMStateBase  *SMSmSpecialStoreFunctions::PSButton8(void)
{
    RETURNSTATE(SmSystemFunctions);
}
// End of JZB.SSF.5.0.0
//////////////////////////////////////////////////////////////////////////////////////////////////

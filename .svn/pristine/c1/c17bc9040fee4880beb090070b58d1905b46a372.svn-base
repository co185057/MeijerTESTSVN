//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSelectTillPickUpFunction.CPP
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// Start of JZB.SSF.5.0.0
// -- This class was added to manage the Tesco Report Buttons
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSmSelectTillPickUpFunction.h"
#include "SMSmGetSpecialFuncValue.h"	// This state
#include "SMSmSystemFunctions.h"
#include "SMSmSpecialStoreFunctions.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    "SMSmSelectTillPickUpFunction"

// global
extern int nSpecialFuncNum;

IMPLEMENT_DYNCREATE(SMSmSelectTillPickUpFunction, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSmSelectTillPickUpFunction::SMSmSelectTillPickUpFunction()
{
    IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMSmSelectTillPickUpFunction::Initialize(void)
{
    ps.SetFrame(STORE_MODE_FRAME);
    ps.SetBackground(BMP_SMBLANK);
    ps.ShowFrame();
    ps.ShowSMTotal(false);
    ps.SMShowItemReceipt(false);
    ps.SetTitle(TTL_SYSTEMFUNCTIONS, PS_BLANK);
    ps.SetLeadthruText(SSF_BTT_TILLPICKUP, PS_BLANK);
    ps.Prompt(PS_NOTEXT);

    ps.SMButton(_T("SMButton1"), SSF_TPU_BUTTON1,true); 
    ps.SMButton(_T("SMButton2"), SSF_TPU_BUTTON2,true); 
    ps.SMButton(_T("SMButton3"), SSF_TPU_BUTTON3,true);  
    ps.SMButton(_T("SMButton4"), SSF_TPU_BUTTON4,true);
    ps.SMButton(_T("SMButton5"), PS_NOTEXT,false); //No 
    ps.SMButton(_T("SMButton6"), PS_NOTEXT,false); //No 
    ps.SMButton(_T("SMButton7"), PS_NOTEXT,false); //No 
    ps.SMButton(_T("SMButton8"), BTT_GOBACK, true);

    return STATE_NULL;                // stay in SMSmGetSpecialFuncValue state
}

/////////////////////////////////////////////
// Button 1 - 
/////////////////////////////////////////////
SMStateBase  *SMSmSelectTillPickUpFunction::PSButton1(void)
{
	trace(L6,"JZB.SSF.5.1.1 (TBHandleReportsFunction %d, 1, NULL", nSpecialFuncNum);
    TBHandleReportsFunction(nSpecialFuncNum, 1, 0);  
    nSpecialFuncNum = 0;  // set back to zero when done
    RETURNSTATE(SmSystemFunctions);
}

/////////////////////////////////////////////
// Button 2 - 
/////////////////////////////////////////////
SMStateBase  *SMSmSelectTillPickUpFunction::PSButton2(void)
{
	trace(L6,"JZB.SSF.5.1.2 (TBHandleReportsFunction %d, 2, NULL", nSpecialFuncNum);
    TBHandleReportsFunction(nSpecialFuncNum, 2, 0);
    nSpecialFuncNum = 0;  // set back to zero when done
    RETURNSTATE(SmSystemFunctions);
}

/////////////////////////////////////////////
// Button 3 - 
/////////////////////////////////////////////
SMStateBase  *SMSmSelectTillPickUpFunction::PSButton3(void)
{
	trace(L6,"JZB.SSF.5.1.3 (TBHandleReportsFunction %d, 3, NULL", nSpecialFuncNum);
    TBHandleReportsFunction(nSpecialFuncNum, 3, 0);
    nSpecialFuncNum = 0;  // set back to zero when done
    RETURNSTATE(SmSystemFunctions);
}

/////////////////////////////////////////////
// Button 4 - 
/////////////////////////////////////////////
SMStateBase  *SMSmSelectTillPickUpFunction::PSButton4(void)
{
	trace(L6,"JZB.SSF.5.1.4 (TBHandleReportsFunction %d, 4, NULL", nSpecialFuncNum);
    TBHandleReportsFunction(nSpecialFuncNum, 4, 0);
    nSpecialFuncNum = 0;  // set back to zero when done
    RETURNSTATE(SmSystemFunctions);
}

/////////////////////////////////////////////
// Button 8 - Go Back
/////////////////////////////////////////////
SMStateBase  *SMSmSelectTillPickUpFunction::PSButton8(void)
{
  ps.Prompt(PS_NOTEXT);
  PSClearInput();
  RETURNSTATE(SmSpecialStoreFunctions);
}
// End of JZB.SSF.5.0.0
//////////////////////////////////////////////////////////////////////////////////////////////////

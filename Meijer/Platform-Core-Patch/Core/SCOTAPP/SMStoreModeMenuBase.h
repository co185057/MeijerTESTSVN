//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMStoreModeMenuBase.h
//
// Description: Derived from SMStoreModeStateBase.
//              Contains standard storemode menu functions
//
// Author: Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMStoreModeMenuBase
#define _SMStoreModeMenuBase

#include "SMState.h"
#include "SMStoreModeState.h"



////////////////////////////////////////
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMStoreModeMenuBase : public STATE(StoreModeState)
{
	
public:
	SMStoreModeMenuBase();
    static BEST nStoreModeMenuState;  // Current store mode menu state
    static SMStateBase* CreateStoreModeMenu();

protected:
    virtual SMStateBase  *OnCloseLaneButton(void);
    virtual SMStateBase  *OnSuspendButton(void);
    virtual SMStateBase  *OnReportingButton(void);
    virtual SMStateBase  *OnSystemFunctionsButton(void);
    virtual SMStateBase  *OnMediaStatusButton(void);
    virtual SMStateBase  *OnAssistModeButton(void);
    virtual SMStateBase  *OnPrintBarcodeButton(void);
    virtual SMStateBase  *OnModeOfOperationButton(void);
    virtual SMStateBase  *OnLoadOptionsButton(void);
    virtual SMStateBase  *OnEditPickListButton(void);
    virtual SMStateBase  *OnUtilityButton(void);
    virtual SMStateBase  *OnSecurityMaintButton(void);
    virtual SMStateBase  *OnGoBackButton(void);
    

    virtual SMStateBase  *PSCmdButton(CString csContext, CString csControl, CString csCommand); //RFQ 986

    virtual void ShowStoreModeMenuButtons();
    virtual void ShowCloseLaneButton();
    virtual void ShowSuspendButton();
    virtual void ShowReportingButton();
    virtual void ShowMediaStatusButton();
    virtual void ShowSystemFunctionsButton();
    virtual void ShowAssistModeButton();
    virtual void ShowModeOfOperationButton();
    virtual void ShowSecurityMaintButton();
    virtual void ShowPrintBarcodeButton();
    virtual void ShowLoadOptionsButton();
    virtual void ShowUtilityButton();
    virtual void ShowGoBackButton();   


  DECLARE_DYNCREATE(SMStoreModeMenuBase)// MFC Runtime class/object information
};


#endif

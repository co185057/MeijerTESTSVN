//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStoreModeMenu.h
//
// TITLE: Class header for store mode Menu state
//
//
// AUTHOR:  Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _StoreModeMenuSTATE
#define _StoreModeMenuSTATE

#include "SMStoreModeMenuBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMStoreModeMenu : public SMStoreModeMenuBase
{
public:
    SMStoreModeMenu();

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
    virtual SMStateBase  *OnFullAssistButton(void);

    virtual SMStateBase  *PSCmdButton(CString csContext, CString csControl, CString csCommand);

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
    virtual void ShowFullAssistButton();

    DECLARE_DYNCREATE(SMStoreModeMenu)// MFC Runtime class/object information
};

#endif
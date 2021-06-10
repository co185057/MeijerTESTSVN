//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMProduceFavorites.h
//
// TITLE: Class header for produce favorites state
//
// TIME:  08/08/00
//
// AUTHOR:    Dale Ehrhardt
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PRODUCEFAVORITESSTATE
#define _PRODUCEFAVORITESSTATE

#include "SMProduceFavoritesBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMProduceFavorites : public SMProduceFavoritesBase
{
public:
    SMProduceFavorites();
    virtual SMStateBase  *Initialize(void);
    virtual void     UnInitialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnSkipBaggingAllowed(void);
    virtual SMStateBase  *OnBackToLGW(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton4(void);
    virtual SMStateBase  *PSListLookup(CString);
    virtual SMStateBase  *PSListLookupTab(LPCTSTR);
    virtual SMStateBase  *PSListScroll(const long);
    virtual SMStateBase  *DMScanner(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *SASecMgrParse( const MessageElement*);
    virtual SMStateBase  *PSShiftKey(void);
    virtual SMStateBase  *PSHotKey(CString &);
    virtual SMStateBase  *PSSpaceKey(void);
    virtual SMStateBase  *PSAltGrKey(void);
    virtual SMStateBase  *PSBackSpaceKey(void);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *PSCharKey(TCHAR c);
    virtual SMStateBase  *SearchStringFromList(void);
 
    virtual void SetPSContext();

    virtual void SetAppStateToSecurity(void);
    virtual void LoadPersonalizedPicklist(void);
    virtual void UpdateDisplayTextsAndButtons(void);
    virtual bool GetSearchKey(CString &csSearchKey);
    virtual void SendSearchString(const CString csSearch);
    virtual bool CheckSubCategoryDisplay(const CString csTabValue);
    virtual void SetDisplayToNoTabSelected(const int nButtonCount);
    virtual void SetupTabDisplayFromOtherStates(void);
    virtual void ResetNextGenUIPicklistDisplayLevelsData(bool bResetSearch=false);
    virtual void SetupTabDisplayFromScanAndBag(BEST prevAnchorState, bool bIsFromQuickPick);
    virtual void UpdateTextsAndButtonsAfterTabSetup(bool bIsFromQuickPick, BEST prevAnchorState);

  DECLARE_DYNCREATE(SMProduceFavorites) // MFC Runtime class/object information

};

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMProduceFavoritesBase.h
//
// TITLE: Class header for produce favorites state
//
// AUTHOR:    Ehrhardt, Dale
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PRODUCEFAVORITESBASESTATE
#define _PRODUCEFAVORITESBASESTATE

#ifdef _CPPUNIT
   #include "FakeSMCustomerModeState.h"
#else
   #include "SMCustomerModeState.h"
#endif // _CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 

#define STR_SEPARATOR		_T("@@") //Personalization Picklist

SMProduceFavoritesBase : public SMCUSTOMERMODESTATE
{
public:
	SMProduceFavoritesBase();
  virtual SMStateBase  *Initialize(void);
#ifndef _CPPUNIT
  virtual void     UnInitialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
#endif //_CPPUNIT
  virtual SMStateBase  *OnSkipBaggingAllowed(void);
#ifndef _CPPUNIT
  virtual SMStateBase  *OnBackToLGW(void); //430778		

#endif //_CPPUNIT
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSListLookup(CString);// override for list/lookups
#ifndef _CPPUNIT
// NewUI
  virtual SMStateBase  *PSListLookupTab(LPCTSTR);
  virtual SMStateBase  *PSListScroll(const long);
// NewUI
#endif //_CPPUNIT
  virtual SMStateBase  *DMScanner(void);

  virtual SMStateBase *DMAcceptor(void);

  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSButton4(void);
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButtonHelp(void);

  virtual SMStateBase  *TimedOut(void);
  CTime m_EnterPicklistTime; //Personalization picklist
  virtual SMStateBase  *SASecMgrParse( const MessageElement*);	//SR078
  virtual SMStateBase  *PSShiftKey(void);
  virtual SMStateBase  *PSHotKey(CString &);

#endif
  virtual SMStateBase  *PSSpaceKey(void);
  virtual SMStateBase  *PSAltGrKey(void);
  virtual SMStateBase  *PSBackSpaceKey(void);
  virtual SMStateBase  *PSClearKey(void);
  virtual SMStateBase  *PSCharKey(TCHAR c);
  virtual SMStateBase  *SearchStringFromList(void);
  virtual SMStateBase  *PSSymbolKey(void);

#ifndef _CPPUNIT
  virtual void SetPSContext();
  virtual void SetKeyboardSearch(void);

  virtual void HideFavoritesButton(const long nSelectIndex,
                              const long nButtonCount, const bool bShow);
#endif // _CPPUNIT
  virtual long SelectedTabIndex(bool bCategory, CString csCategory, BEST previousAnchorState, bool bScroll = false);
  

  virtual void ResetTabSelectedTabFromSearch(void);
  virtual bool IsFavButtonEnabled(void);
  static bool bKeyInCodeButtonPressed; 
  static CString m_csCategory;
  static bool m_bSymbolOn;

  enum IPSProcedures::HKStatus m_HKStatus;
  bool m_bShiftOn;
  bool m_bAltGrOn;
  CString m_csProduceFavoritesContext;
  
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
  
  DECLARE_DYNCREATE(SMProduceFavoritesBase)// MFC Runtime class/object information
		
};

//DECLARE_STATE(ProduceFavorites)

#endif

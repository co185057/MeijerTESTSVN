//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateCU.h
//
//
// AUTHOR:    Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSTATECU
#define _SMSTATECU


protected:
  virtual SMStateBase *CUParse(MessageElement *me);
  virtual SCOT_LANGUAGE_TYPE GetPreferredLanguage(bool bOrigValue = false);
  virtual void SetPreferredLanguage(SCOT_LANGUAGE_TYPE langChosen, CString csSelectionMethod);
  virtual bool NeedPreferenceUpdatePermission();
  virtual void PreferencesReceived();
  virtual bool LoyaltyCheck(CString csLookupType, CString csLookupData);
  virtual bool IsPersonalizedLanguageChange();
  virtual bool IsPersonalizedVolumeChange(); 
  virtual void PreferenceUpdate();
  virtual void SetPersonalTheme(CString csForRedrawLock);
//#ifdef _SSCOMOBILE
  virtual SMStateBase *OnMobileTransactionStart(CString csTransactionInfo);
//#endif // _SSCOMOBILE

public:
  virtual SMStateBase *OnPLAValidation();
  virtual SMStateBase *OnPLALookUp();
  virtual SMStateBase *OnPLAHandInView();
  virtual SMStateBase *OnPLAHandInViewAfterRetry();
  virtual SMStateBase *OnPLATransition();
  virtual SMStateBase *OnLookupComplete (long byteCount);
  virtual SMStateBase *OnPersonalizationOptionsReceived();

#endif


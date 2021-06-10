//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	  SMStateCUBase.h
//
//
// AUTHOR:    Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSTATECUBASE
#define _SMSTATECUBASE

//#define LOOKUP_COMPLETE               1001

#ifndef _CPPUNIT
private:
  static CString csPersonalThemeUsed; //SR814.5 SR815.6 SR851 Personalization and Security
#endif //CPPUNIT
protected:
  virtual SMStateBase *CUParse(MessageElement *me);
#ifndef _CPPUNIT
  virtual SCOT_LANGUAGE_TYPE GetPreferredLanguage(bool bOrigValue = false);
  virtual void SetPreferredLanguage(SCOT_LANGUAGE_TYPE langChosen, CString csSelectionMethod);
  virtual bool NeedPreferenceUpdatePermission();  //Should I Ask user whether to update preferences?
  virtual void PreferencesReceived();  //Perform actions when preferences received from webserver
  virtual bool LoyaltyCheck(CString csLookupType, CString csLookupData);  
  CString SMStateBase::GetAccountDataFromMSRSwipe(CString csAllTrackData);
  //+SR815.5 Personalized Volume
  virtual bool IsPersonalizedLanguageChange();
  virtual bool IsPersonalizedVolumeChange(); 
  virtual void PreferenceUpdate();
  //-SR815.5 Personalized Volume

  //+SR814.5 SR815.6 SR851 Personalization and Security
  virtual void SetPersonalTheme(CString csForRedrawLock); 
  void SMStateBase::SetPersonalThemeUsed(CString csThemeUsed);
  CString SMStateBase::GetPersonalThemeUsed();  
  //-SR814.5 SR815.6 SR851 Personalization and Security 
#endif //CPPUNIT
  virtual SMStateBase *OnMobileTransactionStart(CString csTransactionInfo);
public:
  virtual SMStateBase *OnPLAValidation(); //SSCOADK-260
  virtual SMStateBase *OnPLALookUp();
  virtual SMStateBase *OnPLAHandInView();
  virtual SMStateBase *OnPLAHandInViewAfterRetry();
  virtual SMStateBase *OnPLATransition();
#ifndef _CPPUNIT
  virtual SMStateBase *OnLookupComplete (long byteCount);
  virtual SMStateBase *OnPersonalizationOptionsReceived();

#endif //CPPUNIT
#endif

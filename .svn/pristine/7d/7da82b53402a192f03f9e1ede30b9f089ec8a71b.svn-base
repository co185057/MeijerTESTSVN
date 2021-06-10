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

#define LOOKUP_COMPLETE               1001

protected:
  virtual SMStateBase *CUParse(MessageElement *me);
  virtual SCOT_LANGUAGE_TYPE GetPreferredLanguage(bool bOrigValue = false);
  virtual void SetPreferredLanguage(SCOT_LANGUAGE_TYPE langChosen, CString csSelectionMethod);
  virtual bool NeedPreferenceUpdatePermission();  //Should I Ask user whether to update preferences?
  virtual void PreferencesReceived();  //Perform actions when preferences received from webserver

public:
  virtual SMStateBase *OnLookupComplete (long byteCount);

#endif

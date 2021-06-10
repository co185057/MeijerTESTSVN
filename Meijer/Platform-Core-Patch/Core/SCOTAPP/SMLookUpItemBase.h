//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMLookUpItemBase.h
//
// TITLE: Class header for Key In Code state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LOOKUPITEMBASESTATE
#define _LOOKUPITEMBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif



SMLookUpItemBase : public SMCustomerModeState
{
public:
  enum HKStatus{
      HK_ACUTEON,
      HK_GRAVEON,
      HK_CIRCUMFLEXON,
      HK_UMLAUTON,
      HK_TILDEON,
      HK_NORMAL
    };

	SMLookUpItemBase();
  virtual SMStateBase  *Initialize(void);
  virtual void     UnInitialize(void);

protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSSpaceKey(void);
  virtual SMStateBase  *PSEnterKey(void);
  virtual SMStateBase  *PSClearKey(void);
  virtual SMStateBase  *DMScanner(void);
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *PSShiftKey(void);
  virtual SMStateBase  *PSAltGrKey(void);
  virtual SMStateBase  *PSHotKey(CString &);
  virtual SMStateBase  *PSCharKey(TCHAR c);


private:
   int m_nCharLimit;
   enum HKStatus m_HKStatus;
   bool m_bShiftOn;
   bool m_bAltGrOn;
   CStringArray m_HKSensitiveControls;
   CString m_csFrameName;  

   void PopulateHKSensitiveControls();
   void PopulateHotKeys();

   long GetTokenCount(const CString& csStr, const TCHAR cDelimiter);

   DECLARE_DYNCREATE(SMLookUpItemBase)// MFC Runtime class/object information		
};

#endif


//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMLookUpItemBase.h
//
// TITLE: Class header for Key In Code state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LOOKUPITEMBASESTATE
#define _LOOKUPITEMBASESTATE

#ifndef _CPPUNIT
#include "SMCustomerModeState.h"
#else
#include "FakeSMCustomerModeState.h"
#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif



SMLookUpItemBase : public SMCUSTOMERMODESTATE
{
public:
	SMLookUpItemBase();
#ifndef _CPPUNIT
  virtual SMStateBase  *Initialize(void);
  virtual void     UnInitialize(void);

#endif //_CPPUNIT
protected:
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSSpaceKey(void);
#endif //_CPPUNIT
  virtual SMStateBase  *PSEnterKey(void);
#ifndef _CPPUNIT
  virtual SMStateBase  *PSClearKey(void);
#endif //_CPPUNIT
  virtual SMStateBase  *DMScanner(void);
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *PSShiftKey(void);
  virtual SMStateBase  *PSAltGrKey(void);
  virtual SMStateBase  *PSHotKey(CString &);
  virtual SMStateBase  *PSCharKey(TCHAR c);
  virtual void SetPSContext();

#endif //_CPPUNIT
protected:
   int m_nCharLimit;
   CString m_csFrameName;  
public:static enum IPSProcedures::HKStatus m_HKStatus;
   static bool m_bShiftOn;
   
   static bool m_bAltGrOn;


   DECLARE_DYNCREATE(SMLookUpItemBase)// MFC Runtime class/object information		
};

#endif


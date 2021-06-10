//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMAssistedTenderBase.h
//
// TITLE: Class header for Assisted Tender State
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ASSISTEDTENDERSTATEBASE
#define _ASSISTEDTENDERSTATEBASE

#include "SMRAPDataNeeded.h"


#ifdef _SCOTSSF_
class DLLIMPORT 
#else
class DLLEXPORT 
#endif

SMAssistedTenderBase : public STATE(RAPDataNeeded)
   {
   public:
      SMAssistedTenderBase();
      SMAssistedTenderBase(LPCTSTR sDesc);
	   SMStateBase *Deliver(LPCTSTR sDesc);
      virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
      virtual SMStateBase *Initialize();
      virtual void UnInitialize();
	  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
	  virtual SMStateBase  *OnWtDecrease(void);
      DECLARE_DYNCREATE(SMAssistedTenderBase);// MFC Runtime class/object information
      virtual bool IsLoginBypassAllowed() { return false; };

   protected:
      virtual void SetupViewData();
      virtual SMStateBase *HandleRAPData();
      virtual void showScreen(bool passwordVerified);
      virtual SMStateBase *PSButtonGoBack(void);
      virtual SMStateBase *PSButton1(void);

      CString m_sDesc;
   };

#endif
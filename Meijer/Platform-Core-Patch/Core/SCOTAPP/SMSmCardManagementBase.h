//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCardManagementBase.h
//
// TITLE: Class header for store mode CardManagement state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmSmCardManagementBaseSTATE
#define _SmSmCardManagementBaseSTATE

#include "SMStoreModeState.h"


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif


SMSmCardManagementBase : public STATE(StoreModeState)
{
public:
  SMSmCardManagementBase();							  // constructor
  virtual SMStateBase  *Initialize(void);        // required override
  virtual void UnInitialize();

protected :
  virtual SMStateBase  *PSButton1(void); 
  virtual SMStateBase  *PSButton8(void); // Go Back button
  virtual SMStateBase  *PSEnterKey(void);        // override for enter key
  virtual SMStateBase  *PSClearKey(void);
  virtual SMStateBase  *PSNumericKey(const long lKey);
  virtual SMStateBase  *PSOtherSpecialKeys(const long); // minus key
  virtual SMStateBase  *PSKeystroke(const long wParam, const long lParam); 
  virtual void showScreen();
  virtual void ShowTextData(void);
  virtual void ShowImgData(void);
  virtual bool LoadAmt(const long lValue);
  virtual CString GetNewCylinderPosition(CString csDefRect, long lCount, long lCapacity);
  virtual void ChangeAndShowContext( void);
  long m_lRejectBinCount;
  long m_lDispenserBinCount;
  long m_lRejectBinChangeCount;
  long m_lDispenserBinChangeCount;
  static CString m_csRectCylinder8;
  static CString m_csRectCylinder9;

  DECLARE_DYNCREATE(SMSmCardManagementBase)// MFC Runtime class/object information
};


#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCardManagement.h
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMCARDMANAGEMENT
#define _SMCARDMANAGEMENT

#include "SMSmCardManagementBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmCardManagement : public SMSmCardManagementBase
{
public:
    SMSmCardManagement();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();

protected :
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase  *PSEnterKey(void);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *PSNumericKey(const long lKey);
    virtual SMStateBase  *PSOtherSpecialKeys(const long);
    virtual SMStateBase  *PSKeystroke(const long wParam, const long lParam); 
    virtual void showScreen();
    virtual void ShowTextData(void);
    virtual void ShowImgData(void);
    virtual bool LoadAmt(const long lValue);
    virtual CString GetNewCylinderPosition(CString csDefRect, long lCount, long lCapacity);
    virtual void ChangeAndShowContext( void);

    DECLARE_DYNCREATE(SMSmCardManagement)// MFC Runtime class/object information
};

#endif
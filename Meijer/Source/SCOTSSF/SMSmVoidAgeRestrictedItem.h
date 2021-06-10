//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmVoidAgeRestrictedItem.h
//
// TITLE: Implementation for SOTF 6226
// DATE:  11/11/09
//
// AUTHOR:    Shawn Marks
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSMVOIDAGERESTRICTEDITEM
#define _SMSMVOIDAGERESTRICTEDITEM

#include "SMSmRestrictedItems.h"



#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmVoidAgeRestrictedItem : public SMSmRestrictedItems
{
public:
	SMSmVoidAgeRestrictedItem();
    SMStateBase  *Initialize();
    virtual void UnInitialize();

    virtual bool PSAssistMenuLegalForState() { return true; };
    virtual bool PSRemoteLegalForState() { return true; };

protected:

    virtual SMStateBase  *PSButton1(void);         // reenter DOB  
    virtual SMStateBase  *PSButton2(void);         // void current item     
    virtual SMStateBase  *PSButton8(void);         // return to shopping

    virtual SMStateBase  *PSReceiptUp(void); 
    virtual SMStateBase  *PSReceiptDown(void);
    virtual void AddAgeDescription(CString csID, long lAge);
    virtual SMStateBase * TimedOut(void);
    virtual SMStateBase *TBItemDetails(void);
    virtual void BuildEReceipt(void);
    

private:
    CString m_csScreenContext;
    CString m_csVoidCurrentItemKey;
    CString m_csReceiptSrc;
    CString m_csReceiptRestrictedSrc;


  DECLARE_DYNCREATE(SMSmVoidAgeRestrictedItem) // MFC Runtime class/object information
};

#endif
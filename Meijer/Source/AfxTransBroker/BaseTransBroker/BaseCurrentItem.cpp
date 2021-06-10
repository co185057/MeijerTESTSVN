// CBaseCurrentItem.cpp: implementation of the BaseCurrentItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseCurrentItem.h"
#include "CustomerTBProperties.h"
#include "TraceSystem.h"
#include "TBUtilities.h"
#define T_ID _T("CBaseCurrentItem")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseCurrentItem::CBaseCurrentItem():
m_bDetails(false),
m_bOverride(false),
m_bQuantityLimitExceeded(false),
m_bQuantityConfirmed(false),
m_nScanCodeType(-1),
m_nDealQuantity(0),
m_nItemQuantityLimit(0),
m_nRequireSecBagging(-1),
m_nRequireSecSubstChk(-1),
m_csCategory(_T("")),
m_ExceptionCode(XNONE),
m_dwExceptionFlags(0),
m_dwTBOptionFlags(0),
m_csExceptionWav(_T(""))
{
}

CBaseCurrentItem::~CBaseCurrentItem()
{
}

/*************************************************************************
* Reset - calls ResetItemDetails and ResetItemExtendedDetails
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::Reset()
{
    tbtraceIO(_T("Reset()"));
    ResetItemDetails(); // calls the ItemDetailsMsg Reset() too
    ResetItemExtendedDetails();
    ResetItemProperties();
}

void CBaseCurrentItem::ResetItemProperties()
{
    CCustomerTBProperties *pProp= CCustomerTBProperties::instance();
    pProp->SetProperty(TBPROP_ITEMCODE, _T(""));
    pProp->SetProperty(TBPROP_ITEMPRICE, _T(""));
    pProp->SetProperty(TBPROP_QTY, _T(""));
    pProp->SetProperty(TBPROP_ENTRYID, _T(""));
}

/*************************************************************************
* ResetItemDetails - calls CItemDetailsMsg::Reset, resets all item details
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::ResetItemDetails()
{
    tbtrace(TM_ENTRY, _T("+-ResetItemDetails()"));
    CItemDetailsMsg::Reset();
    m_bDetails=false;
    m_bOverride=false;
    m_bQuantityLimitExceeded=false;
    m_bQuantityConfirmed=false;
    m_nScanCodeType=-1;
    m_nDealQuantity=0;
    m_nItemQuantityLimit=0;
    m_nRequireSecBagging=-1;
    m_nRequireSecSubstChk=-1;
    m_csCategory=_T("");
}

/*************************************************************************
* ResetItemExtendedDetails - resets all item extended details
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::ResetItemExtendedDetails()
{
    tbtrace(TM_INFO, _T("+-ResetItemExtendedDetails()"));
    m_dwExceptionFlags=0;
    m_ExceptionCode=XNONE;
    m_dwTBOptionFlags=0;
    m_csExceptionWav=_T("");
}

/*************************************************************************
* Refresh - casts a CItemDetailsMsg to a this*, get msg data into class
*
* Parameters: 
*  pMsg  - CItemDetailsMsg *
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::Refresh(CItemDetailsMsg *pMsg)
{
    tbtraceIO(_T("Refresh()"));
    // lock if other threads can get here
    delete [] m_pExtensionBuf;	// delete old pointer
    *((CItemDetailsMsg *) this) = *pMsg;  // USE DEFAULT COPY 
    m_pExtensionBuf= pMsg->GetAndForgetExtensionData();// take ownership of this pointer.
    // unlock
}

/*************************************************************************
* SetRequireSecBagging - accessor for member variable
*
* Parameters: 
*  lRequireSecBagging - long
*
* Returns: void
*************************************************************************/
void   CBaseCurrentItem::SetRequireSecBagging(long lRequireSecBagging)
{
    m_nRequireSecBagging= lRequireSecBagging;
}

/*************************************************************************
* GetRequireSecBagging - accessor for member variable
*
* Parameters: none
*
* Returns: member variable
*************************************************************************/
long   CBaseCurrentItem::GetRequireSecBagging()
{
    return m_nRequireSecBagging;
}

/*************************************************************************
* SetRequireSecSubstChk - accessor for member variable
*
* Parameters: 
*  lRequireSecSubstChk - long
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::SetRequireSecSubstChk(long lRequireSecSubstChk)
{
    m_nRequireSecSubstChk= lRequireSecSubstChk;
}

/*************************************************************************
* GetRequireSecSubstChk - accessor for member variable
*
* Parameters: none
*
* Returns: member variable
*************************************************************************/
long CBaseCurrentItem::GetRequireSecSubstChk()
{
    return m_nRequireSecSubstChk;
}

/*************************************************************************
* SetCategory - accessor for member variable
*
* Parameters: 
*  szCategory - LPCTSTR
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::SetCategory(LPCTSTR szCategory)
{
    m_csCategory= szCategory;
}

/*************************************************************************
* GetCategory - accessor for member variable
*
* Parameters: none
*
* Returns: member variable
*************************************************************************/
CString CBaseCurrentItem::GetCategory()
{
    return m_csCategory;
}

/*************************************************************************
* SetExceptionCode - accessor for member variable
*
* Parameters: 
*  TBXCODE ExceptionCode
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::SetExceptionCode(TBXCODE ExceptionCode)
{
    m_ExceptionCode= ExceptionCode;
}

/*************************************************************************
* GetExceptionCode - accessor for member variable
*
* Parameters: none
*
* Returns: member variable
*************************************************************************/
TBXCODE CBaseCurrentItem::GetExceptionCode( )
{
    return m_ExceptionCode;
}

/*************************************************************************
* SetExceptionFlags - accessor for member variable
*
* Parameters: 
*  DWORD dwExceptionFlags
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::SetExceptionFlags(DWORD dwExceptionFlags)
{
    m_dwExceptionFlags = dwExceptionFlags;
}

/*************************************************************************
* GetExceptionFlags - accessor for member variable
*
* Parameters: none
*
* Returns: member variable
*************************************************************************/
DWORD CBaseCurrentItem::GetExceptionFlags( )
{
    return m_dwExceptionFlags;
}

/*************************************************************************
* SetTBOptionFlags - accessor for member variable
*
* Parameters: 
*  DWORD dwTBOptionFlags
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::SetTBOptionFlags(DWORD dwTBOptionFlags)
{
    m_dwTBOptionFlags=dwTBOptionFlags;
}

/*************************************************************************
* GetTBOptionFlags - accessor for member variable
*
* Parameters: none
*
* Returns: member variable
*************************************************************************/
DWORD CBaseCurrentItem::GetTBOptionFlags( )
{
    return m_dwTBOptionFlags;
}

/*************************************************************************
* SetExceptionWav - accessor for member variable
*
* Parameters: 
*  LPCTSTR szExceptionWav
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::SetExceptionWav(LPCTSTR szExceptionWav)
{
    m_csExceptionWav= szExceptionWav;
}

/*************************************************************************
* GetExceptionWav - accessor for member variable
*
* Parameters: none
*
* Returns: member variable
*************************************************************************/
CString CBaseCurrentItem::GetExceptionWav( )
{
    return m_csExceptionWav;
}

/*************************************************************************
* SetDealQuantity - accessor for member variable
*
* Parameters: 
*  long lDealQuantity
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::SetDealQuantity(long lDealQuantity)
{
    m_nDealQuantity= lDealQuantity;
}

/*************************************************************************
* GetDealQuantity - accessor for member variable
*
* Parameters: none
*
* Returns: member variable
*************************************************************************/
long CBaseCurrentItem::GetDealQuantity(void)
{
    return m_nDealQuantity;
}

/*************************************************************************
* SetItemQuantityLimit - accessor for member variable
*
* Parameters: 
*  long lItemQuantityLimit
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::SetItemQuantityLimit(long lItemQuantityLimit)
{
    m_nItemQuantityLimit= lItemQuantityLimit;
}

/*************************************************************************
* GetItemQuantityLimit - accessor for member variable
*
* Parameters: none
*
* Returns: member variable
*************************************************************************/
long CBaseCurrentItem::GetItemQuantityLimit(void)
{
    return m_nItemQuantityLimit;
}

/*************************************************************************
* SetDetails - accessor for member variable
*
* Parameters: 
*  bool bDetails
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::SetDetails(bool bDetails)
{
    m_bDetails= bDetails;
}

/*************************************************************************
* GetDetails - accessor for member variable
*
* Parameters: none
*
* Returns: member variable
*************************************************************************/
bool CBaseCurrentItem::GetDetails(void)
{
    return m_bDetails;
}

/*************************************************************************
* SetOverride - accessor for member variable
*
* Parameters: 
*  bool bOverride
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::SetOverride(bool bOverride)
{
    m_bOverride= bOverride;
}

/*************************************************************************
* GetOverride - accessor for member variable
*
* Parameters: none
*
* Returns: member variable
*************************************************************************/
bool CBaseCurrentItem::GetOverride(void)
{
    return m_bOverride;
}

/*************************************************************************
* SetQuantityConfirmed - accessor for member variable
*
* Parameters: 
*  bool bQuantityConfirmed
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::SetQuantityConfirmed(bool bQuantityConfirmed)
{
    m_bQuantityConfirmed= bQuantityConfirmed;
}

/*************************************************************************
* GetQuantityConfirmed - accessor for member variable
*
* Parameters: none
*
* Returns: member variable
*************************************************************************/
bool CBaseCurrentItem::GetQuantityConfirmed(void)
{
    return m_bQuantityConfirmed;
}

/*************************************************************************
* SetQuantityLimitExceeded - accessor for member variable
*
* Parameters: 
*  bool bQuantityLimitExceeded
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::SetQuantityLimitExceeded(bool bQuantityLimitExceeded)
{
    m_bQuantityLimitExceeded= bQuantityLimitExceeded;
}

/*************************************************************************
* GetQuantityLimitExceeded - accessor for member variable
*
* Parameters: none
*
* Returns: member variable
*************************************************************************/
bool CBaseCurrentItem::GetQuantityLimitExceeded(void)
{
    return m_bQuantityLimitExceeded;
}


/*************************************************************************
* SetScanCodeType - accessor for member variable
*
* Parameters: 
*  long lScanCodeType
*
* Returns: void
*************************************************************************/
void CBaseCurrentItem::SetScanCodeType(long lScanCodeType)
{
    m_nScanCodeType= lScanCodeType;
}

/*************************************************************************
* GetScanCodeType - accessor for member variable
*
* Parameters: none
*
* Returns: member variable
*************************************************************************/
long CBaseCurrentItem::GetScanCodeType(void)
{
    return m_nScanCodeType;
}


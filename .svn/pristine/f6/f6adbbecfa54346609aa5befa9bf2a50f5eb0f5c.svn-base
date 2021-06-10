//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCmDataEntry.CPP
//
// TITLE: Class implementation for customer mode Data Entry state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "Reporting.h"	    // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("CReport")
CReporting myCReporting;
DLLEXPORT CReporting &rp = myCReporting;


//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
CReporting::CReporting()
{
}

long CReporting::TBCreateReportEntry(CString csRecordType, CString csOtherValues)
{
    return CReportingBase::TBCreateReportEntry(csRecordType, csOtherValues);
}

long CReporting::InterventionStart(CString csDesc, RAHANDLE hlD,DWORD dwType)
{
    return CReportingBase::InterventionStart(csDesc, hlD,dwType);
}

long CReporting::InterventionEnd(CString csDesc, RAHANDLE hlD)
{
    return CReportingBase::InterventionEnd(csDesc, hlD);
}

long CReporting::ItemException(CString csItemDesc, CString csItemSku, long lItemPrice,CString csObservedWts, CString csExpected )
{
    return CReportingBase::ItemException(csItemDesc, csItemSku, lItemPrice, csObservedWts, csExpected );
}

long CReporting::UnknownItemException(CString csItemSku)
{
    return CReportingBase::UnknownItemException(csItemSku);
}

long CReporting::TransactionStart(CString csBusinessDate)
{
    return CReportingBase::TransactionStart(csBusinessDate);
}

long CReporting::PickListItemSold( const CString& strItemUPC, int nTimesSold )
{
    return CReportingBase::PickListItemSold( strItemUPC, nTimesSold );
}

long CReporting::ModeChange(int ID, CString csModeName, CString csChangeType)
{
    return CReportingBase::ModeChange(ID, csModeName, csChangeType);
}

long CReporting::OperatorLogin(int ID, CString csSignOn, CString csApplication)
{
    return CReportingBase::OperatorLogin(ID, csSignOn, csApplication);
}

long CReporting::TransactionEnd()
{
    return CReportingBase::TransactionEnd();
}

long CReporting::TenderStart(int nTenderType)
{
    return CReportingBase::TenderStart(nTenderType);
}

long CReporting::TenderEnd (long ltendertotal)
{
    return CReportingBase::TenderEnd (ltendertotal);
}

long CReporting::CurrencyAccepted(bool bBill, CString csDen, int nQuantity, long lAcceptedAmount)
{
    return CReportingBase::CurrencyAccepted(bBill, csDen, nQuantity, lAcceptedAmount);
}

long CReporting::CurrencyDispensed(bool bBill, CString csDen,long lQuantity, long lDispensedAmount)
{
    return CReportingBase::CurrencyDispensed(bBill, csDen, lQuantity, lDispensedAmount);
}

long CReporting::CurrencyCashManagement( CArray<int,int>& caValueList, CArray<int,int>& caChangeInCount, CString csUserID, CString csCashierID )
{
    return CReportingBase::CurrencyCashManagement(caValueList, caChangeInCount, csUserID, csCashierID);
}

long CReporting::CurrencyCashManagement( CArray<CString,CString>& caCashAcceptorDenList, 
                                        CArray<int,int>& caCashAcceptorCountBalanceList,
                                        CArray<int,int>& caCashAcceptorCountChangeList,
                                        CArray<CString,CString>& caCashDispenserDenList, 
                                        CArray<int,int>& caCashDispenserCountBalanceList,
                                        CArray<int,int>& caCashDispenserCountChangeList,
                                        CArray<CString,CString>& caCoinAcceptorDenList, 
                                        CArray<int,int>& caCoinAcceptorCountBalanceList,
                                        CArray<int,int>& caCoinAcceptorCountChangeList,
                                        CArray<CString,CString>& caCoinDispenserDenList, 
                                        CArray<int,int>& caCoinDispenserCountBalanceList,
                                        CArray<int,int>& caCoinDispenserCountChangeList,
                                        CString csUserID, CString csCashierID )
{
    return CReportingBase::CurrencyCashManagement(caCashAcceptorDenList, 
        caCashAcceptorCountBalanceList,
        caCashAcceptorCountChangeList,
        caCashDispenserDenList,
        caCashDispenserCountBalanceList,
        caCashDispenserCountChangeList,
        caCoinAcceptorDenList,
        caCoinAcceptorCountBalanceList,
        caCoinAcceptorCountChangeList,
        caCoinDispenserDenList,
        caCoinDispenserCountBalanceList,
        caCoinDispenserCountChangeList,
        csUserID,
        csCashierID);
}

long CReporting::CurrencyCashManagement( int nDeviceType, CArray<CString,CString>& caDenomList, CArray<int,int>& caChangeInCount,
                                        CArray<int,int>& caBalanceCount, CString csUserID, CString csCashierID )
{
    return CReportingBase::CurrencyCashManagement(nDeviceType, caDenomList, caChangeInCount, caBalanceCount, csUserID, csCashierID);
}

long CReporting::CurrencyCashManagement(CString csTime, long ID, int nDeviceType, CString csDen, long lBalanceInCount,
                                        long lChangeInCount, CString csUserID, CString csCashierID, int nTerminalNumber )
{
    return CReportingBase::CurrencyCashManagement(csTime, ID, nDeviceType, csDen, lBalanceInCount, lChangeInCount, csUserID, csCashierID, nTerminalNumber);
}

long CReporting::CurrencyCashManagement(CString csTime, long ID, bool bBill, CString csDen,long lChangeInCount, CString csUserID, CString csCashierID)
{
    return CReportingBase::CurrencyCashManagement(csTime, ID, bBill, csDen, lChangeInCount, csUserID, csCashierID);
}

long CReporting::Signatures(HFILE hFile , BSTR pSigData)
{
    return CReportingBase::Signatures(hFile , pSigData);
}

long CReporting::SaveReceiptData(LPCTSTR lpFileName , BSTR pSigData, LPCTSTR lpReceiptType)
{
    return CReportingBase::SaveReceiptData(lpFileName , pSigData, lpReceiptType);
}

void CReporting::HexToAscii(BSTR bBuf, BSTR *bOutString, int iLength)
{
    CReportingBase::HexToAscii(bBuf, bOutString, iLength);
}

long CReporting::ID(long lIDtype, long lIDOperation)
{
    return CReportingBase::ID(lIDtype, lIDOperation);
}

void CReporting::InitData()
{
    CReportingBase::InitData();
}

void CReporting::SetTerminalNumber(const CString csNumber)
{
    CReportingBase::SetTerminalNumber(csNumber);
}

CString CReporting::GetTerminalNumber()
{
    return CReportingBase::GetTerminalNumber();
}

void CReporting::SetInterventionTerminalNumber(const CString csNumber)
{
    CReportingBase::SetInterventionTerminalNumber(csNumber);
}

CString CReporting::GetInterventionTerminalNumber()
{
    return CReportingBase::GetInterventionTerminalNumber();
}

void CReporting::SetTenderTerminalNumber(const CString csNumber)
{
    CReportingBase::SetTenderTerminalNumber(csNumber);
}

void CReporting::SetTransactionNumber(const CString csNumber)
{
    CReportingBase::SetTransactionNumber(csNumber);
}

CString CReporting::GetTransactionNumber()
{
    return CReportingBase::GetTransactionNumber();
}

void CReporting::SetOperator(const CString csId)
{
    CReportingBase::SetOperator(csId);
}

CString CReporting::GetOperator()
{
    return CReportingBase::GetOperator();
}

void CReporting::ClearOperator()
{
    CReportingBase::ClearOperator();
}

void CReporting::SetInterventionEndInProgress(bool bValue)
{
    CReportingBase::SetInterventionEndInProgress(bValue);
}

void CReporting::SetItemCount(long lCount)
{
    CReportingBase::SetItemCount(lCount);
}

void CReporting::SetTransactionAmount(long lTotalAmount)
{
    CReportingBase::SetTransactionAmount(lTotalAmount);
}

void CReporting::SetPagerEvent(bool bPagerStatus)
{
    CReportingBase::SetPagerEvent(bPagerStatus);
}

CString CReporting::GetTimeAsString()
{
    return CReportingBase::GetTimeAsString();
}

long CReporting::RAPDispenseChange(long lDispensedAmount)
{
    return CReportingBase::RAPDispenseChange( lDispensedAmount);
}

long CReporting::LogDeviceError (CString csDeviceErrorName, CString csDeviceErrorType)
{
    return CReportingBase::LogDeviceError (csDeviceErrorName, csDeviceErrorType);
}

long CReporting::LogItemDetails()
{
    return CReportingBase::LogItemDetails();
}

void CReporting::SetLanguageName(CString& csLanguage)
{
    CReportingBase::SetLanguageName( csLanguage);
}

void CReporting::RPBusinessDate()
{
    CReportingBase::RPBusinessDate();
}

void CReporting::GetCurrentTBBusinessDate()
{
    CReportingBase::GetCurrentTBBusinessDate();
}

bool CReporting::IsTransactionInProgress()
{
    return CReportingBase::IsTransactionInProgress();
}

CString CReporting::EscapeText(const CString &text)
{
    return CReportingBase::EscapeText(text);
}

bool CReporting::IsInterventionEndInProgress()
{
    return CReportingBase::IsInterventionEndInProgress();
}

bool CReporting::IsCashManagementInProgress()
{
    return CReportingBase::IsCashManagementInProgress();
}

void CReporting::SetCashManagementInProgress(bool cmInProgress)
{
    CReportingBase::SetCashManagementInProgress(cmInProgress);
}

long CReporting::WriteToCollector(CString csXmlFormatted)
{
    return CReportingBase::WriteToCollector(csXmlFormatted);
}

long CReporting::SaveSignatureFileData(CFile* cfPointer , BSTR pSigData, LPCTSTR lpReceiptType)
{
    return CReportingBase::SaveSignatureFileData(cfPointer, pSigData, lpReceiptType);
}

long CReporting::TransactionCompleted(CString csTerminalNumber)
{
    return CReportingBase::TransactionCompleted(csTerminalNumber);
}

long CReporting::WriteReportData(BSTR bxml)
{
    return CReportingBase::WriteReportData(bxml);
}

long CReporting::CreateDOM()
{
    return CReportingBase::CreateDOM();
}

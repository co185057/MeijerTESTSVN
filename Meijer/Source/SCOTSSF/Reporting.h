//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  CReporting.h
//
// TITLE: Class header for Reporting
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ReportingSSF
#define _ReportingSSF

#include "ReportingBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
    CReporting : public CReportingBase
{
public:
    CReporting();		  // constructor
    virtual long TBCreateReportEntry(CString csRecordType, CString csOtherValues);
    virtual long InterventionStart(CString csDesc, RAHANDLE hlD,DWORD dwType);
    virtual long InterventionEnd(CString csDesc, RAHANDLE hlD);
    virtual long ItemException(CString csItemDesc, CString csItemSku, long lItemPrice,CString csObservedWts, CString csExpected );
    virtual long UnknownItemException(CString csItemSku); 
    virtual long TransactionStart(CString csBusinessDate);
    virtual long PickListItemSold( const CString& strItemUPC, int nTimesSold = 1 );
    virtual long ModeChange(int ID, CString csModeName, CString csChangeType);
    virtual long OperatorLogin(int ID, CString csSignOn, CString csApplication);
    virtual long TransactionEnd();
    virtual long TenderStart(int nTenderType);
    virtual long TenderEnd (long ltendertotal);
    virtual long CurrencyAccepted(bool bBill, CString csDen, int nQuantity, long lAcceptedAmount);
    virtual long CurrencyDispensed(bool bBill, CString csDen,long lQuantity, long lDispensedAmount);
    virtual long CurrencyCashManagement( CArray<int,int>& caValueList, CArray<int,int>& caChangeInCount, CString csUserID, CString csCashierID );
    virtual long CurrencyCashManagement(CString csTime, long ID, bool bBill, CString csDen,long lChangeInCount, CString csUserID, CString csCashierID);
    virtual long CurrencyCashManagement(CString csTime, long ID, int nDeviceType, CString csDen, long lBalanceInCount, long lChangeInCount, CString csUserID, CString csCashierID, int nTerminalNumber );
    virtual long CurrencyCashManagement( CArray<CString,CString>& caCashAcceptorDenList, 
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
        CString csUserID, CString csCashierID );
    virtual long CurrencyCashManagement( int nDeviceType, CArray<CString,CString>& caDenomList, CArray<int,int>& caChangeInCount, CArray<int,int>& caBalanceCount, CString csUserID, CString csCashierID );

    virtual long Signatures(HFILE hFile , BSTR pSigData);
    virtual long SaveReceiptData(LPCTSTR, BSTR, LPCTSTR);
    virtual void HexToAscii(BSTR bBuf, BSTR *bOutString, int iLength);
    virtual long ID(long lIDtype, long lIDOperation);
    virtual void InitData();
    virtual void SetTerminalNumber(const CString csNumber);
    virtual CString GetTerminalNumber();
    virtual void SetInterventionTerminalNumber(const CString csNumber);
    virtual CString GetInterventionTerminalNumber(); //SR814.5 SR815.6 SR851 Personalization and Security
    virtual void SetTenderTerminalNumber(const CString csNumber);
    virtual void SetTransactionNumber(const CString csNumber);
    virtual CString GetTransactionNumber();
    virtual void SetOperator(const CString csId);
    virtual CString GetOperator();
    virtual void ClearOperator();
    virtual void SetInterventionEndInProgress(bool bValue);  // TAR 311902
    virtual void SetItemCount(long lCount);
    virtual void SetTransactionAmount(long lTotalAmount);
    virtual void SetPagerEvent(bool bPagerStatus);
    virtual CString GetTimeAsString();
    virtual long RAPDispenseChange(long lDispensedAmount);
    virtual long LogDeviceError (CString csDeviceErrorName, CString csDeviceErrorType);
    virtual long LogItemDetails();
    virtual void SetLanguageName(CString& csLanguage);
    virtual void RPBusinessDate();
    virtual void GetCurrentTBBusinessDate();
    virtual bool IsTransactionInProgress(); //TAR 343053
    virtual CString EscapeText(const CString &text);
    virtual bool IsInterventionEndInProgress(); //429111
    virtual bool IsCashManagementInProgress();
    virtual void SetCashManagementInProgress(bool cmInProgress);

    long SaveSignatureFileData(CFile* cfPointer , BSTR pSigData, LPCTSTR lpReceiptType);

    virtual long WriteToCollector(CString csXmlFormatted);

    virtual long TransactionCompleted(CString csTerminalNumber);

protected:
    virtual long WriteReportData(BSTR bxml);
    virtual long CreateDOM();
};

#endif

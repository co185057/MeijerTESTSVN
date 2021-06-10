//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    ReportingBase.h
//
// TITLE:   Log data for FastLane 3.1 Reporting Feature
//
// Author:  Tara Duncan
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CPPUNIT
#include "CollectorBase.h"
#else
#include "FakeCommon.h"
#include "FakeCollectorBase.h"
#endif //_CPPUNIT
#include "ReportingBaseConstants.h"
#include "IReporting.h"
#include "SMStateConstants.h"

#ifndef _REPORTING
#define _REPORTING


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 CReportingBase : public COLLECTORBASE, public IReporting
  {
  public:
	  CString m_csTransactionID;
  long m_lBias;		//SRXXX
#ifndef _CPPUNIT
  virtual long TBCreateReportEntry(CString csRecordType, CString csOtherValues);
  virtual long InterventionStart(CString csDesc, RAHANDLE hlD,DWORD dwType);
  virtual long InterventionEnd(CString csDesc, RAHANDLE hlD);
  virtual long ItemException(CString csItemDesc, CString csItemSku, long lItemPrice,CString csObservedWts, CString csExpected );
  virtual long UnknownItemException(CString csItemSku); 
  virtual long TransactionStart(CString csBusinessDate);
  virtual long PickListItemSold( const CString& strItemUPC, int nTimesSold = 1 );
  virtual long ModeChange(int ID, CString csModeName, CString csChangeType);
  virtual long OperatorLogin(int ID, CString csSignOn, CString csApplication);
#endif //_CPPUNIT
  virtual long TransactionEnd();
#ifndef _CPPUNIT
  virtual long TenderStart(int nTenderType);
  virtual long TenderEnd (long ltendertotal);
  virtual long CurrencyAccepted(bool bBill, CString csDen, int nQuantity, long lAcceptedAmount);
  virtual long CurrencyDispensed(bool bBill, CString csDen,long lQuantity, long lDispensedAmount);
  virtual long CurrencyCashManagement( CArray<int,int>& caValueList, CArray<int,int>& caChangeInCount, CString csUserID, CString csCashierID );
  virtual long CurrencyCashManagement(CString csTime, long ID, bool bBill, CString csDen,long lChangeInCount, CString csUserID, CString csCashierID);
//  virtual long CurrencyCashManagement(CString csTime, long ID, bool bBill, CString csDen,long lChangeInCount, CString csUserID, CString csCashierID, CString nTerminalNumber);
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
#endif
  virtual long ID(long lIDtype, long lIDOperation);
  virtual void InitData();
#ifndef _CPPUNIT
  virtual void SetTerminalNumber(const CString csNumber){m_csTerminalNumber = csNumber;};
  virtual CString GetTerminalNumber(){return m_csTerminalNumber;};
  virtual void SetInterventionTerminalNumber(const CString csNumber){m_csInterventionTerminalNumber = csNumber;};
  virtual CString GetInterventionTerminalNumber(){return m_csInterventionTerminalNumber;}; //SR814.5 SR815.6 SR851 Personalization and Security
  virtual void SetTenderTerminalNumber(const CString csNumber){m_csTenderTerminalNumber = csNumber;};
  virtual void SetTransactionNumber(const CString csNumber){m_csTransactionNumber=csNumber;};
  virtual CString GetTransactionNumber(){return m_csTransactionNumber;};
  virtual void SetOperator(const CString csId);
  virtual CString GetOperator(){return m_csOperatorNumber;}
  virtual void ClearOperator();
  virtual void SetInterventionEndInProgress(bool bValue);  // TAR 311902
  virtual void SetItemCount(long lCount) {m_lItemCount = lCount;};
  virtual void SetTransactionAmount(long lTotalAmount) { m_lTransactionTotal = lTotalAmount;};
  virtual void SetPagerEvent(bool bPagerStatus){ m_bpagerEvent = bPagerStatus;};
#endif
  virtual CString GetTimeAsString();
#ifndef _CPPUNIT
  virtual long RAPDispenseChange(long lDispensedAmount);
  virtual long LogDeviceError (CString csDeviceErrorName, CString csDeviceErrorType);
  virtual long LogItemDetails();
  virtual void SetLanguageName(CString& csLanguage);
  virtual void RPBusinessDate();
  virtual void GetCurrentTBBusinessDate();
  virtual bool IsTransactionInProgress(); //TAR 343053
  virtual CString EscapeText(const CString &text);
  virtual bool IsInterventionEndInProgress(); //429111
  virtual bool IsCashManagementInProgress(){return m_bCashManagementInProgress; } // TAR 450658
  virtual void SetCashManagementInProgress(bool cmInProgress){m_bCashManagementInProgress = cmInProgress;} // TAR 450658
#endif //_CPPUNIT
  bool m_bForceEnd;
  bool m_bItemAssoc;
  bool m_bRapAssisted;
  bool m_bTransAssisted;
  bool m_bIsInterventionApproved;    //SR803  
  long m_lAttendantEntryID; // A unique per-transaction ID for attendant mode entry and exit
  long m_lOperatorLoginID; // A unique per-transaction ID for operator login
  bool m_bOffline;
  bool m_bBill; 
  bool m_bTelnetActive;
  long m_lEntryID;          // A unique per-transaction ID for general use.
                            // Currently used to log SafePay note cover/cassette
                            // removal.
  TRANSDISPOSITION  m_nDisposition;
  CString m_csBusinessDate;
  CReportingBase();
  virtual ~CReportingBase();
// RFQ 7477
 //Meijer CQ 5572
  std::vector<strtCurrency> m_SCurrencyList;
  void ResetCurrencyList(void);
  CString GetGurrencyList(void);
  //end CQ
// RFQ 7477
#ifndef _CPPUNIT
//  protected:
  long SaveSignatureFileData(CFile* cfPointer , BSTR pSigData, LPCTSTR lpReceiptType);
#endif //_CPPUNIT

//  private:
  virtual long WriteToCollector(CString csXmlFormatted);
  long m_lItemCount;
  long m_lTransactionTotal;
  bool m_bpagerEvent; 
  bool m_bInterventionEndInProgress;  // TAR 311902
  bool m_bTransactionInProgress; //TAR 343053
  bool m_bTransactionNumberSet; //TAR410994
  bool m_bCashManagementInProgress; // TAR 450658
  bool m_bMobileShopper;
  /////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////
  //CString formats
  CString m_csNameStart;
  CString m_csOperatorNumber;
  CString m_csTerminalNumber;               //Terminal Lane Number
  CString m_csTransactionNumber;
  CString m_csTenderTerminalNumber;         //Where tender came from
  CString m_csInterventionTerminalNumber;   //Where Intervention was cleared
  CString m_csRecord;
  CString m_csStart;
  CString m_csTypeStart;
  CString m_csTypeEnd;
  CString m_csTenderOperator;
  CString m_csLanguage;



  DECLARE_DYNCREATE(CReportingBase)// MFC Runtime class/object information

  
};

#endif

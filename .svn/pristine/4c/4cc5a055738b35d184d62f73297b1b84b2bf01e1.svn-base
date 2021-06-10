//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    Reporting.h
//
// TITLE:   Log data for FastLane 3.1 Reporting Feature
//
// Author:  Tara Duncan
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "CollectorBase.h"
#include <vector>

#ifndef _REPORTING
#define _REPORTING
#define ATTSTATION              _T("Station")
#define ATTBUSINESS             _T("BusinessDate") 
#define ATTLANE                 _T("Lane")
#define ATTSTOREID              _T("StoreTransactionID")
#define ATTTYPE                 _T("Type")
#define ATTEND                  _T("End")
#define ATTSTART                _T("Start")
#define ATTID                   _T("ID")
#define ATTTIME                 _T("Time")
#define ATTCLASS                _T("Class")
#define ATTOPERATOR             _T("Operator")
#define ATTINTERVENTIONTYPE     _T("InterventionType")
#define ATTITEMCOUNT            _T("NumberOfItems")
#define ATTIDID                 _T("ID")
#define ATTMSMATCHDESC          _T("Description")
#define ATTITEMSKU              _T("SKU")
#define ATTITEMUPC				_T("UPC")
#define ATTOBSERVEDWEIGHT       _T("ObservedWeight")
#define ATTEXPECTEDWEIGHT       _T("ExpectedWeight")
#define ATTSLIPDATA             _T("SlipData") 
#define ATTSIGNATUREDATA        _T("SignatureData")
#define ATTDISPOSITION          _T("Disposition")
#define ATTRECEIPTTYPE			_T("ReceiptType")
#define ATTAMOUNT               _T("Amount")
#define ATTCURRENCYTYPE         _T("Note")
#define ATTTENDERTYPE           _T("TenderType")
#define ATTDENOMINATION         _T("Denomination")
#define ATTQUANTITY             _T("Quantity")
#define ATTCHANGEINVALUE        _T("ChangeInValue") //dp185016 support glory device
#define ATTCHANGEINCOUNT        _T("ChangeInCount") 
#define ATTBALANCEINCOUNT       _T("BalanceInCount") //pw500001 support for cashmanagement 
#define ATTUSERID               _T("UserId")        
#define ATTCASHIERID            _T("CashierId")     
#define ATTLOCKBOX              _T("LockBox")       //dp185016 support glory device
#define ATTPAGEREVENT           _T("Pager")
#define ATTITEMPRICE            _T("Price")
#define ATTOFFLINE              _T("OfflineFlag")
#define ATTTRANSASSISTED        _T("AssistedFlag")
#define ATTRAPASSISTED          _T("RAPAssistedFlag")
#define ATTTRANSATTENDANT       _T("AttendantFlag")
#define ATTEXCEPTYPE            _T("ExceptionType")
#define ATTLANGUAGE				_T("Language")

#define ATTCODE                 _T("Code")
#define ATTDESCRIPTION          _T("Description")
#define ATTDEPARTMENT           _T("Department")
#define ATTEXTPRICE             _T("ExtPrice")
#define ATTUNITPRICE            _T("UnitPrice")
#define ATTPRICERQD             _T("PriceRqd")
#define ATTQTYSOLD              _T("QtySold")
#define ATTDEALQTY              _T("DealQty")
#define ATTQTYRQD               _T("QtyRqd")
#define ATTQTYCONFIRMED         _T("QtyConfirmed")
#define ATTQTYLIMITEXCEEDED     _T("QtyLimitExceeded")
#define ATTWGTRQD		        _T("WgtRqd")
#define ATTWGTSOLD				_T("WgtSold")
#define ATTCOUPON				_T("Coupon")
#define ATTNOTFOUND		        _T("NotFound")
#define ATTNOTFORSALE	    	_T("NotForSale")
#define ATTVISVERIFY		    _T("VisVerify")
#define ATTRESTRICTED	    	_T("Restricted")
#define ATTRESTRICTEDAGE		_T("RestrictedAge")
#define ATTVOID					_T("Void")
#define ATTLINKED		        _T("Linked")
#define ATTCRATEANSWER  		_T("CrateAnswer")
#define ATTSECURITYBAGGINGRQD	_T("SecurityBaggingRqd")
#define ATTSECURITYSUBCHKRQD	_T("SecuritySubChkRqd")
#define ATTSECURITYTAG		    _T("SecurityTag")
#define ATTZEROWEIGHT   		_T("ZeroWeight")
#define ATTPICKLIST		        _T("PickList")
#define ATTTARERQD              _T("TareRqd")
#define ATTITEMID               _T("ItemID")
#define ATTPAGERT               _T("1")
#define ATTPAGERF               _T("0")
#define ATTACTIVE               _T("Trans")
#define ATTIN                   _T("1")
#define ATTOUT                  _T("0")
#define ATTIMMEDIATE            _T("IMMEDIATE")
#define ATTDELAYED              _T("DELAYED")
#define ATTUNKNOWN              _T("UNKNOWNITEM")
#define ATTSUSPEND              _T("SUSPEND")
#define ATTTIMESSOLD			_T("TimesSold")
#define ATTMODENAME				_T("ModeName")
#define ATTMODEID				_T("ID")
#define ATTMODECHANGETYPE		_T("TypeChange")
#define INTERVENTION            _T("Intervention Time=")
#define TBNAME                  _T("TB Time=")
#define TRANSACTION             _T("Transaction Time=")
#define MODECHANGE				_T("ModeChange Time=")
#define ITEMEXCEPTION           _T("ItemException Time=")
#define TENDERS                 _T("Tender Time=")
#define CURRENCYA               _T("CurrencyAccepted Time=")
#define CURRENCYD               _T("CurrencyDispensed Time=")
#define CURRENCYCASHMANAGEMENT  _T("CurrencyCashManagement Time=")
#define PICKLISTITEMSOLD        _T("PickListItemSold")
#define SIGNATURES              _T("Signature Time=")
#define AUTOADVUNEXPINC			_T("AutoAdvUnexpInc")
#define AUTOADVMISMATCH			_T("AutoAdvMismatch")
#define RAPDISPENSECHANGE       _T("RAPDispenseChange Time=")
#define LOGDEVICEERROR          _T("DeviceError Time=")
#define LOGITEMDETAILS          _T("ItemSold Time=")
#define ATTDEVICE               _T("Device")
#define ATTDEVICETYPE           _T("DeviceType") //pw500001 support for cashmanagement
#define ATTERRORTYPE            _T("ErrorType")
#define ATTENDANT				_T("ATTENDANT")
#define ENTER					_T("ENTER")
#define EXIT					_T("EXIT")
#define OPERATORLOGIN			_T("OperatorLogin Time=")
#define SIGNON					_T("SignOn")
#define APPLICATION				_T("Application")
#define SCOTAPP					_T("SCOTAPP")
#define MODE_NOTECOVER    _T("NOTECOVER")       // SafePay cash recycler.
#define MODE_NOTECASSETTE _T("NOTECASSETTE")    // SafePay cash recycler.
#define ATTBIAS					_T("Bias")		// SRXXX
#define DEGRADEDMODE			_T("DEGRADEDMODE")	//SR742

#define SECURITYDISABLED         _T("SecurityDisabled")

//////////////////////////////////////////////////////////////////////////////////////////////////
enum TRANSDISPOSITION
{
    RP_DISPOSITION_CANCEL = 1,
    RP_DISPOSITION_SUSPEND,
    RP_DISPOSITION_COMPLETE,
    RP_DISPOSITION_FORCED
};

enum IDTYPE
{
   RP_IDTYPE_CURRENCYA    = 1, 
   RP_IDTYPE_CURRENCYD    = 2,
   RP_IDTYPE_TENDER       = 3,
   RP_IDTYPE_ITEMS        = 4,
   RP_IDTYPE_TRANS        = 5,
   RP_IDTYPE_SIGNID,
   RP_IDTYPE_ITEMSSOLD
};
enum IDOPERATION
{
    RP_IDOPERATION_CLEARALL = 1,
    RP_IDOPERATION_INCREMENT,
    RP_IDOPERATION_GET
};

//Meijer CQ 5572
typedef struct
{
	long lDen;
	bool bBill;
	long lQuantity;
	bool bAccepted;

}strtCurrency;

#define MAX_NUMBER_DENOMINATIONS	10
#define DOLLAR_DENOMINATION_INDEX   3
//end CQ


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 CReporting : public CCollectorBase
  {
  public:
  long m_lBias;		//SRXXX
  long TBCreateReportEntry(CString csRecordType, CString csOtherValues);
  long InterventionStart(CString csDesc, RAHANDLE hlD,DWORD dwType);
  long InterventionEnd(CString csDesc, RAHANDLE hlD);
  long ItemException(CString csItemDesc, CString csItemSku, long lItemPrice,CString csObservedWts, CString csExpected );
  long UnknownItemException(CString csItemSku); 
  long TransactionStart(CString csBusinessDate);
  long PickListItemSold( const CString& strItemUPC, int nTimesSold = 1 );
  long ModeChange(int ID, CString csModeName, CString csChangeType);
  long OperatorLogin(int ID, CString csSignOn, CString csApplication);
  long TransactionEnd();
  long TenderStart(int nTenderType);
  long TenderEnd (long ltendertotal);
  long CurrencyAccepted(bool bBill, CString csDen, int nQuantity, long lAcceptedAmount);
  long CurrencyDispensed(bool bBill, CString csDen,long lQuantity, long lDispensedAmount);
  long CurrencyCashManagement( CArray<int,int>& caValueList, CArray<int,int>& caChangeInCount, CString csUserID, CString csCashierID );
  long CurrencyCashManagement(CString csTime, long ID, bool bBill, CString csDen,long lChangeInCount, CString csUserID, CString csCashierID);
  long CurrencyCashManagement(CString csTime, long ID, bool bBill, CString csDen,long lChangeInCount, CString csUserID, CString csCashierID, CString nTerminalNumber);
  long CurrencyCashManagement(CString csTime, long ID, int nDeviceType, CString csDen, long lBalanceInCount, long lChangeInCount, CString csUserID, CString csCashierID, int nTerminalNumber );
  long CurrencyCashManagement( CArray<CString,CString>& caCashAcceptorDenList, 
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
  long CurrencyCashManagement( int nDeviceType, CArray<CString,CString>& caDenomList, CArray<int,int>& caChangeInCount, CArray<int,int>& caBalanceCount, CString csUserID, CString csCashierID );

  long Signatures(HFILE hFile , BSTR pSigData);
  long SaveReceiptData(LPCTSTR, BSTR, LPCTSTR);
  void HexToAscii(BSTR bBuf, BSTR *bOutString, int iLength);
  long ID(long lIDtype, long lIDOperation);
  void InitData();
  void SetTerminalNumber(const CString csNumber){m_csTerminalNumber = csNumber;};
  CString GetTerminalNumber(){return m_csTerminalNumber;};
  void SetInterventionTerminalNumber(const CString csNumber){m_csInterventionTerminalNumber = csNumber;};
  void SetTenderTerminalNumber(const CString csNumber){m_csTenderTerminalNumber = csNumber;};
  void SetTransactionNumber(const CString csNumber){m_csTransactionNumber=csNumber;};
  void SetOperator(const CString csId);
  CString GetOperator(){return m_csOperatorNumber;}
  void ClearOperator();
  void SetInterventionEndInProgress(bool bValue);  // TAR 311902
  void SetItemCount(long lCount) {m_lItemCount = lCount;};
  void SetTransactionAmount(long lTotalAmount) { m_lTransactionTotal = lTotalAmount;};
  void SetPagerEvent(bool bPagerStatus){ m_bpagerEvent = bPagerStatus;};
  CString GetTimeAsString();
  long RAPDispenseChange(long lDispensedAmount);
  long LogDeviceError (CString csDeviceErrorName, CString csDeviceErrorType);
  long LogItemDetails();
  void SetLanguageName(CString& csLanguage);
  void RPBusinessDate();
  void GetCurrentTBBusinessDate();
  bool IsTransactionInProgress(); //TAR 343053
  bool m_bForceEnd;
  bool m_bItemAssoc;
  bool m_bRapAssisted;
  bool m_bTransAssisted;
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
  CReporting();
  virtual ~CReporting();

  //Meijer CQ 5572
  std::vector<strtCurrency> m_SCurrencyList;
  void ResetCurrencyList(void);
  CString GetGurrencyList(void);
  //end CQ

  protected:
  long SaveSignatureFileData(CFile* cfPointer , BSTR pSigData, LPCTSTR lpReceiptType);

  private:
  long WriteToCollector(CString csXmlFormatted);
  long m_lItemCount;
  long m_lTransactionTotal;
  bool m_bpagerEvent; 
  bool m_bInterventionEndInProgress;  // TAR 311902
  bool m_bTransactionInProgress; //TAR 343053
  bool m_bTransactionNumberSet; //TAR410994
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



  DECLARE_DYNCREATE(CReporting)// MFC Runtime class/object information

  
};

#endif

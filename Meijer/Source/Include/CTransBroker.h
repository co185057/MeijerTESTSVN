// CTransBroker.h: interface for the CTransBroker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSBROKER_H__A52B4FE4_682D_11D2_863A_00A0249ED384__INCLUDED_)
#define AFX_TRANSBROKER_H__A52B4FE4_682D_11D2_863A_00A0249ED384__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "afxmt.h"
//
#include <afxdisp.h>

#pragma once
#ifdef _REMOTECONNECTION
#define RCDECLSPEC __declspec(dllexport) 
#else
#define RCDECLSPEC __declspec(dllimport) 
#endif

#pragma warning(disable:4786)
#include "AFXTEMPL.h"
#include "Descriptor.h"
#include "DescriptorList.h"

#include "TransBroker.h"
#include "CommonServices.h" 
#include "DevicePropertyChanges.h"

//definition of TransBroker.dll functions for backward compatibility with older TBs
typedef TBRC (__cdecl *FP_GETITEMEXTENDEDDETAILS)(PSItemExtendedDetails psItemExtendedDetails);
typedef TBRC (__cdecl *FP_ENTERTENDERSTATE)(const bool bEnter);
#ifdef _UNICODE
typedef TBRC (__cdecl *FP_SENDSIGDATA)(LPCTSTR CustomerName, LPCTSTR cSigData, 
       DWORD nLength);
#else
typedef TBRC (__cdecl *FP_SENDSIGDATA)(LPCTSTR CustomerName, LPCSTR cSigData, 
       DWORD nLength);
#endif
typedef TBRC (__cdecl *FP_SETTRAININGMODESTATE)(const bool bEnter);
typedef TBRC (__cdecl *FP_SETBUSINESSDATE)(LPCTSTR szSetBusinessDate);
typedef TBRC (__cdecl *FP_GETMESSAGEINFO)(PSMessageInfo psMessageInfo);  // TAR 127247.
typedef TBRC (__cdecl *FP_CREATEREPORTENTRY)(PSCreateReportEntry psCreateReportEntry);
typedef TBRC (__cdecl *FP_GETCURRENTBUSINESSDATE)(CString &csBusinessDate);
typedef TBRC (__cdecl *FP_GETCURRENTTRANSACTIONNUMBER)(void);
typedef TBRC (__cdecl *FP_GETCUSTOMERMESSAGE)(LPTSTR* psCustomerMessageText, LPTSTR* psCustomerMessageAudio);
typedef TBRC (__cdecl *FP_GETREWARDLINEDETAILS) (PSRewardLineDetails psRewardLineDetails);
typedef TBRC (__cdecl *FP_NEEDSTARTOFDAY)(TBSTARTOFDAYSTATE &eState);
typedef TBRC (__cdecl *FP_PRINT)(LPCTSTR szPrintFile);
typedef TBRC (__cdecl *FP_SUSPENDTRANSACTIONEX)(const BOOL fOverride, LPCTSTR szSuspendReasons);
typedef TBRC (__cdecl *FP_GETPRINTERERROR)(LPTSTR* psErrorMessage);
typedef TBRC (__cdecl *FP_GETEXTENSIONSTATESETUP) (LPTSTR szUICtx, LPTSTR szDevCtx, psPropChangeRequestList pExtChangeList);
typedef TBSTATE (__cdecl *FP_NOTIFYEXTENSIONSTATEEVENT)(psExtStateEventNotifyIn pEventNotifyIn, psExtStateEventNotifyOut pEventNotifyOut);

typedef TBRC (__cdecl *FP_ENTERMICR)(
   LPCTSTR szRawData,         // Raw MICR data from the device.
   LPCTSTR szTransitNumber,   // Bank Transit Number
   LPCTSTR szAccountNumber,   // Customer Account Number
   LPCTSTR szBankNumber,      // ABA Bank number
   LPCTSTR szSerialNumber,    // Check document number
   LPCTSTR szEPC,             // Extended Processigng Code
   long lType,                // Type (1=Personal, 2=Business, 99=Unknown)
   long lCountryCode);        // Country Code (1=USA, 2=Canada, 3=Mexico, 99=Unknown)
typedef long (__cdecl *FP_GETDOCENDORSEMENTINFO)(
   long *plNumRemaining,
   LPTSTR *ppszDocDescription,// Operator prompt or document description.
   LPTSTR *ppszDocFront,      // Text to print on front of document.
   LPTSTR *ppszDocBack);      // Text to print on back of document.
typedef TBRC (__cdecl *FP_SETCASHDRAWERSTATE)(
   long iState);               // Bitmapped (Set=Open, Clear=Closed - lsb is drawer 1, etc.)
typedef TBRC  (__cdecl *FP_GETEXPECTEDCASHDRAWERSTATE)(long*);     // Bitmapped (lsb is drawer 1, etc.)
typedef TBRC (__cdecl *FP_SETSECURITYLEVEL)(
   long lSecurityLevel);      // Security level set by keylock or other auth means.
typedef TBRC  (__cdecl *FP_GETEXPECTEDSECURITYLEVEL)(long*);
typedef TBRC  (__cdecl *FP_GETCURRENTSECURITYLEVEL)(long*);
typedef TBRC (__cdecl *FP_ENDORSEMENTCOMPLETED)(
   bool bSuppressSubsequent/* = false*/);  // Flag to indicate to TB not to fire addl.
                                       // TB_ENDORSEDOCUMENT messages.
typedef TBRC (__cdecl *FP_GETASSISTMODESTATUS)(
   long *plAssistModeStatus);
typedef TBRC (__cdecl *FP_GETMICRPROMPT)(
   LPTSTR *ppszMICRPrompt);// Operator prompt or MICR insertion
typedef TBRC (__cdecl *FP_GETDATANEEDEDVIEW)(PSDataNeededView psDataNeededView);
typedef TBRC (__cdecl *FP_SENDDATANEEDED)(LPCTSTR szDataNeededDetails);
typedef TBRC (__cdecl *FP_GETLOYALTYCARDNUMBER)(LPTSTR *pszLoyaltyCardNumber);
typedef BOOL (__cdecl *FP_DISABLETENDERBUTTON)(TBTENDERTYPE nTenderType);
typedef TBRC (__cdecl *FP_GETGIFTCARDBALANCE)(long *plBalance);
typedef TBRC (__cdecl *FP_LOGRECEIPT)(LPTSTR* lpFileName,BSTR* bstrSignatureString, LPTSTR* lpReceiptType);
typedef TBRC (__cdecl *FP_PREPROCESSSIGNATUREDATA)(LPCTSTR lpFileName,BSTR bstrSignatureData, LPCTSTR lpReceiptType, BOOL* bProcessData);
typedef TBRC (__cdecl *FP_GETNEXTSTATE)(LPCTSTR lpLastStateName, LPCTSTR lpNextStateName, bool *pbOverride, LPTSTR *lpOverrideStateName, CDescriptorList &dlParameterList);
typedef TBRC (__cdecl *FP_PREPROCESSDATA)(LPCTSTR lpDataString, LPCTSTR lpEventName, bool *pbAllowDefaultProcessing);
typedef TBRC (__cdecl *FP_SYNCTBORPOS)(void); //tar 288628
typedef TBRC (__cdecl *FP_POWERSHUTDOWN)(void); //dp185016 tar 265463
typedef TBRC (__cdecl *FP_SENDTBCOMMAND)(long lCommandID, va_list pArgs);
typedef TBRC (__cdecl *FP_VALIDATEHEADCASHIER)(LPCTSTR szCashierID, LPCTSTR szCashierPw); //dp185016 support glory device
typedef TBRC (__cdecl *FP_TENDERSELECTED)(TBTENDERTYPE nTenderType); // (*) SR797 ====================
//Old TBAPIs list: ++begin
// to follow new method of Linking through the Window's LoadLibrary\GetProcAddress methodology with the help of functional pointers.
typedef TBRC (__cdecl *FP_GETITEMDETAILS)(PSItemDetails psItemDetails);
typedef TBRC (__cdecl *FP_CANCELTRANSACTION)(const BOOL fOverride);
typedef TBRC (__cdecl *FP_COPYFILEFROMSERVER)(LPCTSTR szFileName, LPCTSTR szDestinationFilePath, const BOOL fForce);
typedef TBRC (__cdecl *FP_COPYFILETOSERVER)(LPCTSTR szFileName, LPCTSTR szSourceFilePath);
typedef TBRC (__cdecl *FP_ENTERITEM)(LPCTSTR szItemCode,
						   const BOOL fVoid, const BOOL fOverride,
						   const long nQuantity, const long nWeight,
						   const long nTareCode, const long lPrice, 
						   const BOOL fQuantityConfirmed, PSEnterItemDetails psMoreDetails);
typedef TBRC (__cdecl *FP_FINISH)(void);
typedef TBRC (__cdecl *FP_GETPINWORKINGKEY)(PSPINWorkingKey psPINWorkingKey);
typedef TBRC (__cdecl *FP_GETTENDERDETAILS)(PSTenderDetails psTenderDetails);
typedef TBRC (__cdecl *FP_GETTOTALDETAILS)(PSTotalDetails psTotalDetails);
typedef TBRC (__cdecl *FP_INITIALIZE)(HWND hWnd, LPCTSTR szFilePath);
typedef TBRC (__cdecl *FP_KEYPRESS)(const USHORT uKey);
typedef TBRC (__cdecl *FP_LOADOPTIONS)(void);
typedef TBRC (__cdecl *FP_LOGERRORTOSERVER)(LPCTSTR szErrorText, const int nSeverity,
						   const int nComponentID, LPCTSTR szComponentID,
						   LPCTSTR szModuleID, const int nModuleLineNumber);
typedef TBRC (__cdecl *FP_REFRESHSTATE)(void);
typedef TBRC (__cdecl *FP_RELEASEJOURNALDATA)(HFILE hJournalData);
typedef TBRC (__cdecl *FP_RELEASEMESSAGE)(PSMessageElement psMessage);
typedef TBRC (__cdecl *FP_RELEASERECEIPTDATA)(HFILE hReceiptData);
typedef TBRC (__cdecl *FP_RELEASESLIPDATA)(HFILE hSlipData);
typedef TBRC (__cdecl *FP_RESUMETRANSACTION)(LPCTSTR szSuspendID);
typedef TBRC (__cdecl *FP_SETMONITORALL)(BOOL fMonitor);
typedef TBRC (__cdecl *FP_START)(LPCTSTR szOverrideCode);
typedef TBRC (__cdecl *FP_SUSPENDTRANSACTION)(const BOOL fOverride);
typedef TBRC (__cdecl *FP_UNINITIALIZE)(void);
typedef TBRC (__cdecl *FP_EPSSTARTOFDAY)(LPVOID lp);
typedef TBRC (__cdecl *FP_EPSENDOFDAY)(LPVOID lp);
typedef TBRC (__cdecl *FP_GETAVAILABLEFOODSTAMPAMOUNT)(PSEBTAvailBalance psEBTAvailBalance);
typedef TBRC (__cdecl *FP_GETAVAILABLEBENEFITAMOUNT)(PSEBTAvailBalance psEBTAvailBalance);
typedef TBRC (__cdecl *FP_REQUESTFOODSTAMPELIGIBLETOTAL)();
typedef TBRC (__cdecl *FP_PARSEFOODSTAMPTOTAL)(PSTotalDetails psTotalDetails);
typedef TBRC (__cdecl *FP_SENDSIGNON)(void);
typedef TBRC (__cdecl *FP_SENDSIGNOFF)(void);
typedef TBRC (__cdecl *FP_SENDLOCKON)(void);
typedef TBRC (__cdecl *FP_SENDLOCKOFF)(void);
typedef TBRC (__cdecl *FP_INITHOSTAPPEXIT)(void);
typedef TBRC (__cdecl *FP_LANECLOSED)(const bool bEnter);
typedef TBRC (__cdecl *FP_ENTERDOB)(int nYear, int nMonth, int nDay, int nCustomerAge);
typedef TBRC (__cdecl *FP_SENDWEIGHT)(long lWeight);
typedef TBRC (__cdecl *FP_ENTERTENDERSTATE)(const bool bEnter);
typedef long (__cdecl *FP_GETTERMINALNUMBER)(void);
typedef long (__cdecl *FP_GETVERSION)(LPTSTR szVersionString, long nBufferLength);
typedef long (__cdecl *FP_GETTRANSACTIONLIMIT)(void);
typedef long (__cdecl *FP_GETQUANTITYLIMIT)(void);
typedef BOOL (__cdecl *FP_ISALLOWED)(TBFEATURE nFeature);
typedef BOOL (__cdecl *FP_VALIDOPERATOR)(LPCTSTR szOperatorID);
typedef BOOL (__cdecl *FP_VALIDPASSWORD)(LPCTSTR szPassword);
typedef HFILE (__cdecl *FP_GETJOURNALDATA)(void);
typedef HFILE (__cdecl *FP_GETRECEIPTDATA)(void);
typedef HFILE (__cdecl *FP_GETSLIPDATA)(void);
typedef TBSTATE (__cdecl *FP_PROCESSMESSAGE)(PSMessageElement psMessage);
typedef TBLOYALTYCARDTYPE (__cdecl *FP_VERIFYLOYALTYCARD)(LPCTSTR szCardData, int nDataLen);

#ifdef _UNICODE
typedef TBRC (__cdecl *FP_ENTERTENDER)(TBTENDERTYPE nTenderType, const long lAmount,
						   LPCTSTR szCardData, const int nPINLength,
						   LPCTSTR szEncryptedPIN, const BOOL fOverride,
						   const BOOL fVoid);
#else
typedef TBRC (__cdecl *FP_ENTERTENDER)(TBTENDERTYPE nTenderType, const long lAmount,
						   LPCTSTR szCardData, const int nPINLength,
						   LPCSTR szEncryptedPIN, const BOOL fOverride,
						   const BOOL fVoid);
#endif

#ifdef _UNICODE
typedef BOOL (__cdecl *FP_VALIDATEOPERATOR)(LPCTSTR szOperatorID, LPCTSTR szOperatorPw);
#else
typedef BOOL (__cdecl *FP_VALIDATEOPERATOR)(LPCTSTR szOperatorID, LPCSTR szOperatorPw);
#endif

#ifdef _UNICODE
typedef TBRC (__cdecl *FP_GET2X20)(LPCTSTR szDisplay);
#else
typedef TBRC (__cdecl *FP_GET2X20)(LPCSTR szDisplay);
#endif

#ifdef _UNICODE
typedef TBRC (__cdecl *FP_SENDMSG2HOST)(LPCTSTR szSend,const int iMsgLen, const TBDEVICEID nDeviceClass,const UCHAR nFunctionCode,
					const long	lOptFileHandle, const bool  fOriginatePipeMsg);
#else
typedef TBRC (__cdecl *FP_SENDMSG2HOST)(LPCSTR szSend,const int iMsgLen, const TBDEVICEID nDeviceClass,const UCHAR nFunctionCode,
					const long	lOptFileHandle, const bool  fOriginatePipeMsg);
#endif

//Old TBAPIs list: --end
//BAM
//Begin RFC 356020
typedef TBRC (__cdecl *FP_ENTERMICRWITHTYPE)(
   TBTENDERTYPE nTenderType,
   LPCTSTR szRawData,         // Raw MICR data from the device.
   LPCTSTR szTransitNumber,   // Bank Transit Number
   LPCTSTR szAccountNumber,   // Customer Account Number
   LPCTSTR szBankNumber,      // ABA Bank number
   LPCTSTR szSerialNumber,    // Check document number
   LPCTSTR szEPC,             // Extended Processigng Code
   long lType,                // Type (1=Personal, 2=Business, 99=Unknown)
   long lCountryCode);        // Country Code (1=USA, 2=Canada, 3=Mexico, 99=Unknown)
   //BAM
//End RFC 356020
class CTransBroker
{
private:
	SCOTAPPErrorObject* pErrorObj; // App sets this member and null values turns off logging
	TraceObject*		pTraceObj; // App sets this member and null values turns off tracing of TB APIs
  bool fTBInit; // True if TB is initialized

public:
	CTransBroker();
	virtual ~CTransBroker();
	CTransBroker(TraceObject*	pTraceObj, SCOTAPPErrorObject* pErrorObj);
	void SetLogObjects(TraceObject*	pTraceObj, SCOTAPPErrorObject* AppTBErrorObj);
	void InitFunctionPointers(); 

public:
   // The following members are declared static to keep things simple 
   // in terms of maintaining a table of function pointers and their
   // associated process names.  This is done with the assumption that
   // there will only be one instance of CTransBroker within any process.

	static FP_GETITEMEXTENDEDDETAILS        sm_lpfnGetItemExtendedDetails;
	static FP_ENTERTENDERSTATE              sm_lpfnEnterTenderState;
    static FP_SENDSIGDATA                   sm_lpfnSendSigData;
	static FP_SETTRAININGMODESTATE          sm_lpfnSetTrainingModeState;
    static FP_SETBUSINESSDATE               sm_lpfnSetBusinessDate;
	static FP_GETMESSAGEINFO                sm_lpfnGetMessageInfo;
	static FP_GETCURRENTTRANSACTIONNUMBER   sm_lpfnGetCurrentTransactionNumber;
    static FP_GETCURRENTBUSINESSDATE        sm_lpfnGetCurrentBusinessDate;
	static FP_GETCUSTOMERMESSAGE            sm_lpfnGetCustomerMessage;
	static FP_CREATEREPORTENTRY             sm_lpfnCreateReportEntry;
	static FP_GETREWARDLINEDETAILS          sm_lpfnGetRewardLineDetails;
	static FP_NEEDSTARTOFDAY		        sm_lpfnNeedStartOfDay;
   static FP_ENTERMICR                     sm_lpfnEnterMICR;
   static FP_GETDOCENDORSEMENTINFO         sm_lpfnGetDocEndorsementInfo;
   static FP_SETCASHDRAWERSTATE            sm_lpfnSetCashDrawerState;
   static FP_GETEXPECTEDCASHDRAWERSTATE    sm_lpfnGetExpectedCashDrawerState;
   static FP_SETSECURITYLEVEL              sm_lpfnSetSecurityLevel;
   static FP_GETEXPECTEDSECURITYLEVEL      sm_lpfnGetExpectedSecurityLevel;
   static FP_GETCURRENTSECURITYLEVEL       sm_lpfnGetCurrentSecurityLevel;
   static FP_ENDORSEMENTCOMPLETED          sm_lpfnEndorsementCompleted;
   static FP_GETASSISTMODESTATUS           sm_lpfnGetAssistModeStatus;
   static FP_GETMICRPROMPT                 sm_lpfnGetMICRPrompt;  
   static FP_PRINT                         sm_lpfnPrint;
   static FP_SUSPENDTRANSACTIONEX          sm_lpfnSuspendTransactionEx;
	static FP_GETPRINTERERROR               sm_lpfnGetPrinterError;
    static FP_GETDATANEEDEDVIEW             sm_lpfnGetDataNeededView;
    static FP_SENDDATANEEDED                sm_lpfnSendDataNeeded;
	static FP_GETLOYALTYCARDNUMBER          sm_lpfnGetLoyaltyCardNumber;
	static FP_DISABLETENDERBUTTON           sm_lpfnDisableTenderButton;
	static FP_GETGIFTCARDBALANCE            sm_lpfnGetGiftCardBalance;
	static FP_LOGRECEIPT					 sm_lpfnLogReceipt;
	static FP_PREPROCESSSIGNATUREDATA       sm_lpfnPreprocessSignatureData;
	static FP_GETEXTENSIONSTATESETUP     sm_lpfnGetExtensionStateSetup;
	static FP_NOTIFYEXTENSIONSTATEEVENT  sm_lpfnNotifyExtensionStateEvent;
    static FP_GETNEXTSTATE                  sm_lpfnGetNextState;
    static FP_PREPROCESSDATA                sm_lpfnPreprocessData;
    static FP_SYNCTBORPOS                   sm_lpfnSyncTBorPOS; //tar 288628
	static FP_POWERSHUTDOWN                 sm_lpfnPowerShutdown; //dp185016 tar 265463
    static FP_SENDTBCOMMAND                 sm_lpfnSendTBCommand;
	static FP_VALIDATEHEADCASHIER           sm_lpfnValidateHeadCashier; //dp185016 support glory device
    static FP_TENDERSELECTED				sm_lpfnTenderSelected; // (*) SR797 ====================

	//Old TBAPIs list: ++begin
    static FP_GETITEMDETAILS				sm_lpfnGetItemDetails;
   	static FP_CANCELTRANSACTION				sm_lpfnCancelTransaction;
	static FP_COPYFILEFROMSERVER			sm_lpfnCopyFileFromServer;
	static FP_COPYFILETOSERVER				sm_lpfnCopyFileToServer;
	static FP_ENTERITEM						sm_lpfnEnterItem;
	static FP_FINISH						sm_lpfnFinish;
	static FP_GETPINWORKINGKEY				sm_lpfnGetPINWorkingKey;
	static FP_GETTENDERDETAILS				sm_lpfnGetTenderDetails;
	static FP_GETTOTALDETAILS				sm_lpfnGetTotalDetails;
	static FP_INITIALIZE					sm_lpfnInitialize;
	static FP_KEYPRESS						sm_lpfnKeyPress;
	static FP_LOADOPTIONS					sm_lpfnLoadOptions;
	static FP_LOGERRORTOSERVER				sm_lpfnLogErrorToServer;
	static FP_REFRESHSTATE					sm_lpfnRefreshState;
	static FP_RELEASEJOURNALDATA			sm_lpfnReleaseJournalData;
	static FP_RELEASEMESSAGE				sm_lpfnReleaseMessage;
	static FP_RELEASERECEIPTDATA			sm_lpfnReleaseReceiptData;
	static FP_RELEASESLIPDATA				sm_lpfnReleaseSlipData;
	static FP_RESUMETRANSACTION				sm_lpfnResumeTransaction;
	static FP_SETMONITORALL					sm_lpfnSetMonitorAll;
	static FP_START							sm_lpfnStart;
	static FP_SUSPENDTRANSACTION			sm_lpfnSuspendTransaction;
	static FP_UNINITIALIZE					sm_lpfnUninitialize;
	static FP_EPSSTARTOFDAY					sm_lpfnEPSStartOfDay;
	static FP_EPSENDOFDAY					sm_lpfnEPSEndOfDay;
	static FP_GETAVAILABLEFOODSTAMPAMOUNT	sm_lpfnGetAvailableFoodStampAmount;
	static FP_GETAVAILABLEBENEFITAMOUNT		sm_lpfnGetAvailableBenefitAmount;
	static FP_REQUESTFOODSTAMPELIGIBLETOTAL sm_lpfnRequestFoodStampEligibleTotal;
	static FP_PARSEFOODSTAMPTOTAL			sm_lpfnParseFoodStampTotal;
	static FP_SENDSIGNON					sm_lpfnSendSignOn;
	static FP_SENDSIGNOFF					sm_lpfnSendSignOff;
	static FP_SENDLOCKON					sm_lpfnSendLockOn;
	static FP_SENDLOCKOFF					sm_lpfnSendLockOff;
	static FP_INITHOSTAPPEXIT				sm_lpfnInitHostAppExit;
	static FP_LANECLOSED					sm_lpfnLaneClosed;
	static FP_ENTERDOB						sm_lpfnEnterDOB;
	static FP_SENDWEIGHT					sm_lpfnSendWeight;
	static FP_GETTERMINALNUMBER				sm_lpfnGetTerminalNumber;
	static FP_GETVERSION					sm_lpfnGetVersion;
	static FP_GETTRANSACTIONLIMIT			sm_lpfnGetTransactionLimit;
	static FP_GETQUANTITYLIMIT				sm_lpfnGetQuantityLimit;
	static FP_ISALLOWED						sm_lpfnIsAllowed;
	static FP_VALIDOPERATOR					sm_lpfnValidOperator;
	static FP_VALIDPASSWORD					sm_lpfnValidPassword;
	static FP_GETJOURNALDATA				sm_lpfnGetJournalData;
	static FP_GETRECEIPTDATA				sm_lpfnGetReceiptData;
	static FP_GETSLIPDATA					sm_lpfnGetSlipData;
	static FP_PROCESSMESSAGE				sm_lpfnProcessMessage;
	static FP_VERIFYLOYALTYCARD				sm_lpfnVerifyLoyaltyCard;
	static FP_ENTERTENDER					sm_lpfnEnterTender;
	static FP_VALIDATEOPERATOR				sm_lpfnValidateOperator;
	static FP_GET2X20						sm_lpfnGet2x20;
	static FP_SENDMSG2HOST					sm_lpfnSendMsg2Host;
    static FP_ENTERMICRWITHTYPE			   sm_lpfnEnterMICRWithType; //BAM

	//Old TBAPIs list: --end
	TBRC  EnterTenderState(const bool bEnter);  // Enter or leave tender screen
	TBRC  SendSigData(LPCTSTR CustomerName, 
       LPCTSTR cSigData, 
       DWORD nLength); 

	TBRC  GetItemExtendedDetails(PSItemExtendedDetails psItemExtendedDetails);

	TBRC  CancelTransaction(const BOOL fOverride);
	TBRC  CopyFileFromServer(LPCTSTR szSourceFile, LPCTSTR
							szDestinationFile, const BOOL fForce);
	TBRC  CopyFileToServer(LPCTSTR szSourceFile, LPCTSTR szDestFile);
	TBRC  EnterDOB(int nYear, int nMonth, int nDay, int nCustomerAge);
	TBRC  SendWeight(long lWeight);

	TBRC  EnterItem(LPCTSTR szItemCode,
					PSEnterItemDetails psMoreItemDetails,
					const BOOL fVoid, const BOOL fOverride,
					const long nQuantity, const long nWeight,
					const long nTareCode, const long lPrice, 
					const BOOL fQuantityConfirmed);

	TBRC  EnterTender(TBTENDERTYPE nTenderType, const long lAmount,
					  LPCTSTR szCardData, const int nPINLength,
					  LPCTSTR szEncryptedPIN, const BOOL fOverride,
					  const BOOL fVoid);
	TBRC  Finish(void);
	TBRC  Get2x20(LPCTSTR szDisplayStr);
	TBRC  GetItemDetails(PSItemDetails psItemDetails);
	HFILE GetJournalData(void);
	TBRC  GetPINWorkingKey(PSPINWorkingKey psPINWorkingKey);
	HFILE GetReceiptData(void);
	HFILE GetSlipData(void);
	long  GetTransactionLimit(void);
	long  GetQuantityLimit(void);
	TBRC  GetTenderDetails(PSTenderDetails psTenderDetails);
	long  GetTerminalNumber(void);
    long  GetCurrentTransactionNumber(void);
    TBRC  GetCurrentBusinessDate(CString &csBusinessDate);
	TBRC  GetTotalDetails(PSTotalDetails psTotalDetails);
	TBRC  ParseFoodStampTotal(PSTotalDetails psTotalDetails);
	long  GetVersion(CString &cVersionString);
	TBRC  Initialize(HWND hWnd, LPCTSTR szFilePath);
	BOOL  IsAllowed(TBFEATURE nFeature);
	TBRC  KeyPress(const USHORT uKey);
	TBRC  LoadOptions(void);
	TBRC  LogErrorToServer(LPCTSTR szErrorText, const int nSeverity,
						   const int nComponentID, LPCTSTR szComponentID,
						   LPCTSTR szModuleID, const int nModuleLineNumber);
	TBSTATE ProcessMessage(MessageElement* psMessage);
	TBRC  RefreshState(void);
	TBRC  ReleaseJournalData(HFILE hJournalData);
	TBRC  ReleaseMessage(PSMessageElement psMessage);
	TBRC  ReleaseReceiptData(HFILE hReceiptData);
	TBRC  ReleaseSlipData(HFILE hSlipData);
	TBRC  ResumeTransaction(LPCTSTR szSuspendID);
	TBRC  SendMsg2Host(LPCTSTR szSend,const int iMsgLen, const TBDEVICEID nDeviceClass,const UCHAR nFunctionCode = 0,
					   const long lOptFileHandle = 0, const bool  fOriginatePipeMsg = true);
	TBRC  SetMonitorAll(BOOL fMonitor);
	TBRC  Start(LPCTSTR szOverrideCode);
	TBRC  SuspendTransaction(const BOOL fOverride);
	TBRC  Uninitialize(void);

	BOOL  ValidateOperator(LPCTSTR szOperatorID, LPCTSTR szOperatorPw);
	BOOL  ValidOperator(LPCTSTR szOperatorID);
	BOOL  ValidPassword(LPCTSTR szOperatorPw);

	TBLOYALTYCARDTYPE VerifyLoyaltyCard(LPCTSTR szCardData, int nDataLen);
	TBRC  EPSEndOfDay(LPVOID lp);
	TBRC  EPSStartOfDay(LPVOID lp);
	TBRC  RequestFoodStampEligibleTotal();
	TBRC  GetAvailableFoodStampAmount(PSEBTAvailBalance psEBTAvailBalance);
	TBRC  GetAvailableBenefitAmount(PSEBTAvailBalance psEBTAvailBalance);
	TBRC  SendSignOn(void);
	TBRC  SendSignOff(void);
	TBRC  SendLockOn(void);				    		// set the manager's key lock ON
	TBRC  SendLockOff(void);						// set the manager's key lock OFF
	TBRC  InitHostAppExit(void);
	TBRC  LaneClosed(const bool bEnter);  // Enter or leave Lane Closed screen
	TBRC  IsTBInitialized(void);   // return TB_SUCCESS if TB is initialized
	TBRC  SetTrainingModeState(const bool bTMState);
    TBRC  SetBusinessDate(LPCTSTR szSetBusinessDate);
	TBRC  GetMessageInfo(PSMessageInfo psMessageInfo);
	TBRC  CreateReportEntry(PSCreateReportEntry psCreateReportEntry);

	TBRC  GetCustomerMessage(LPTSTR* psCustomerMessageText, LPTSTR* psCustomerMessageAudio);
	TBRC  GetRewardLineDetails(PSRewardLineDetails psRewardLineDetails);
    TBRC  NeedStartOfDay(TBSTARTOFDAYSTATE &eState);
	TBRC  SuspendTransactionEX(const BOOL fOverride, LPCTSTR szSuspendReasons);
	TBRC  Print(LPCTSTR szPrintFile);
	TBRC  GetPrinterError(LPTSTR* psErrorMessage);
	TBRC  GetDataNeededView(PSDataNeededView);
	TBRC  SendDataNeeded(LPCTSTR);

   TBRC EnterMICR(
      LPCTSTR szRawData,         // Raw MICR data from the device.
      LPCTSTR szTransitNumber,   // Bank Transit Number
      LPCTSTR szAccountNumber,   // Customer Account Number
      LPCTSTR szBankNumber,      // ABA Bank number
      LPCTSTR szSerialNumber,    // Check document number
      LPCTSTR szEPC,             // Extended Processigng Code
      long lType,                // Type (1=Personal, 2=Business, 99=Unknown)
      long lCountryCode);        // Country Code (1=USA, 2=Canada, 3=Mexico, 99=Unknown)
   TBRC GetDocEndorsementInfo(
      long *plNumRemaining,      // Number of endorsements reamining.
      LPTSTR *ppszDocDescription,// Operator prompt or document description.
      LPTSTR *ppszDocFront,      // Text to print on front of document.
      LPTSTR *ppszDocBack);      // Text to print on back of document.
   TBRC SetCashDrawerState(long iState);               // Bitmapped (Set=Open, Clear=Closed - lsb is drawer 1, etc.)
   TBRC GetExpectedCashDrawerState(long *plExpectedState);     // Bitmapped (lsb is drawer 1, etc.)
   TBRC SetSecurityLevel(long lSecurityLevel);      // Security level set by keylock or other auth means.
   TBRC GetExpectedSecurityLevel(long *plExpectedLevel);
   TBRC GetCurrentSecurityLevel(long *plCurrentLevel);
   TBRC EndorsementCompleted(
      bool bSuppressSubsequent = false);  // Flag to indicate to TB not to fire addl.
                                          // TB_ENDORSEDOCUMENT messages.
   TBRC GetAssistModeStatus(
      long *plGetAssistModeStatus); 
   TBRC GetMICRPrompt(
      LPTSTR *ppszMICRPrompt);// Operator prompt or MICR Insertion
   TBRC GetLoyaltyCardNumber(LPTSTR *pszLoyaltyCardNumber);
   BOOL DisableTenderButton(TBTENDERTYPE nTenderType);
   TBRC GetGiftCardBalance(long *plBalance);
   TBRC  LogReceipt(LPTSTR* lpFileName,BSTR* bstrSignatureString, LPTSTR* lpReceiptType);
   TBRC  PreprocessSignatureData(LPCTSTR lpFileName,BSTR bstrSignatureData, LPCTSTR lpReceiptType, BOOL* bProcessData);
   TBRC	   GetExtensionStateSetup(LPTSTR szUICtx, LPTSTR szDevCtx, psPropChangeRequestList pExtChangeList);
   TBSTATE NotifyExtensionStateEvent(psExtStateEventNotifyIn pEventNotifyIn, psExtStateEventNotifyOut pEventNotifyOut);
   TBRC GetNextState(LPCTSTR lpLastStateName, LPCTSTR lpNextStateName, bool *pbOverride, 
                       LPTSTR *lpOverrideStateName, CDescriptorList &dlParameterList);
   TBRC PreprocessData(LPCTSTR lpDataString, LPCTSTR lpEventName, bool *pbAllowDefaultProcessing);

   //+dp185016 
   TBRC SyncTBorPOS();
   //-dp185016
   TBRC  PowerShutdown(void); //dp185016 tar 265463
   TBRC SendTBCommand(long lCommandID, va_list pArgs);
   BOOL ValidateHeadCashier( LPCTSTR szCashierID, LPCTSTR szCashierPw ); //dp185016 support glory device
     //BAM
	    //Begin RFC 356020
   TBRC EnterMICRWithType(
	  TBTENDERTYPE nTenderType,
      LPCTSTR szRawData,         // Raw MICR data from the device.
      LPCTSTR szTransitNumber,   // Bank Transit Number
      LPCTSTR szAccountNumber,   // Customer Account Number
      LPCTSTR szBankNumber,      // ABA Bank number
      LPCTSTR szSerialNumber,    // Check document number
      LPCTSTR szEPC,             // Extended Processigng Code
      long lType,                // Type (1=Personal, 2=Business, 99=Unknown)
      long lCountryCode);        // Country Code (1=USA, 2=Canada, 3=Mexico, 99=Unknown)
   //End RFC 356020
	 //BAM 
   TBRC TenderSelected(TBTENDERTYPE nTenderType); // (*) SR797 ====================   
};

#endif // !defined(AFX_TRANSBROKER_H__A52B4FE4_682D_11D2_863A_00A0249ED384__INCLUDED_)


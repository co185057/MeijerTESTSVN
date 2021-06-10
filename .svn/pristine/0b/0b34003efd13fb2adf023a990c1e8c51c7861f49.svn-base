// TransBroker.cpp: implementation of the CTransBroker class.
// RFC 247969 - Added Entrance and Exit trace statements to each method in this class
//////////////////////////////////////////////////////////////////////

//#include "windows.h"
#include <afxwin.h>
#include <afxext.h>
#include "CTransBroker.h"

#define Error if (pErrorObj) pErrorObj->Error              // the global trace object
#define Trace if (pTraceObj) pTraceObj->Trace


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//#define SCOTAppTB   500
#define COMP_ID ID_TI
#define COMP    _T("TBInter")
#define T_ID _T("TBInter")

FP_GETITEMEXTENDEDDETAILS        CTransBroker::sm_lpfnGetItemExtendedDetails;
FP_ENTERTENDERSTATE              CTransBroker::sm_lpfnEnterTenderState;
FP_SENDSIGDATA                   CTransBroker::sm_lpfnSendSigData;
FP_SETTRAININGMODESTATE          CTransBroker::sm_lpfnSetTrainingModeState;
FP_GETMESSAGEINFO                CTransBroker::sm_lpfnGetMessageInfo;
FP_CREATEREPORTENTRY             CTransBroker::sm_lpfnCreateReportEntry;
FP_GETCURRENTTRANSACTIONNUMBER   CTransBroker::sm_lpfnGetCurrentTransactionNumber;
FP_GETCURRENTBUSINESSDATE        CTransBroker::sm_lpfnGetCurrentBusinessDate;
FP_SETBUSINESSDATE               CTransBroker::sm_lpfnSetBusinessDate;
FP_GETCUSTOMERMESSAGE            CTransBroker::sm_lpfnGetCustomerMessage;
FP_GETREWARDLINEDETAILS          CTransBroker::sm_lpfnGetRewardLineDetails;
FP_NEEDSTARTOFDAY		         CTransBroker::sm_lpfnNeedStartOfDay;
FP_ENTERMICR                     CTransBroker::sm_lpfnEnterMICR;
FP_GETDOCENDORSEMENTINFO         CTransBroker::sm_lpfnGetDocEndorsementInfo;
FP_SETCASHDRAWERSTATE            CTransBroker::sm_lpfnSetCashDrawerState;
FP_GETEXPECTEDCASHDRAWERSTATE    CTransBroker::sm_lpfnGetExpectedCashDrawerState;
FP_SETSECURITYLEVEL              CTransBroker::sm_lpfnSetSecurityLevel;
FP_GETEXPECTEDSECURITYLEVEL      CTransBroker::sm_lpfnGetExpectedSecurityLevel;
FP_GETCURRENTSECURITYLEVEL       CTransBroker::sm_lpfnGetCurrentSecurityLevel;
FP_ENDORSEMENTCOMPLETED          CTransBroker::sm_lpfnEndorsementCompleted;
FP_PRINT                         CTransBroker::sm_lpfnPrint;
FP_SUSPENDTRANSACTIONEX          CTransBroker::sm_lpfnSuspendTransactionEx;
FP_GETPRINTERERROR               CTransBroker::sm_lpfnGetPrinterError;
FP_GETASSISTMODESTATUS           CTransBroker::sm_lpfnGetAssistModeStatus;
FP_GETMICRPROMPT                 CTransBroker::sm_lpfnGetMICRPrompt;
FP_GETDATANEEDEDVIEW             CTransBroker::sm_lpfnGetDataNeededView;
FP_SENDDATANEEDED                CTransBroker::sm_lpfnSendDataNeeded;
FP_GETLOYALTYCARDNUMBER          CTransBroker::sm_lpfnGetLoyaltyCardNumber;
FP_DISABLETENDERBUTTON           CTransBroker::sm_lpfnDisableTenderButton;
FP_GETGIFTCARDBALANCE            CTransBroker::sm_lpfnGetGiftCardBalance;
FP_LOGRECEIPT				     CTransBroker::sm_lpfnLogReceipt;
FP_PREPROCESSSIGNATUREDATA       CTransBroker::sm_lpfnPreprocessSignatureData;
FP_SENDTBCOMMAND                 CTransBroker::sm_lpfnSendTBCommand;
FP_GETEXTENSIONSTATESETUP        CTransBroker::sm_lpfnGetExtensionStateSetup;
FP_NOTIFYEXTENSIONSTATEEVENT     CTransBroker::sm_lpfnNotifyExtensionStateEvent;
FP_GETNEXTSTATE                  CTransBroker::sm_lpfnGetNextState;
FP_PREPROCESSDATA                CTransBroker::sm_lpfnPreprocessData;
FP_SYNCTBORPOS                   CTransBroker::sm_lpfnSyncTBorPOS;//tar 288628
FP_POWERSHUTDOWN                 CTransBroker::sm_lpfnPowerShutdown;
FP_VALIDATEHEADCASHIER          CTransBroker::sm_lpfnValidateHeadCashier; //dp185016 support glory device
//Old TBAPIs list: ++begin
FP_GETITEMDETAILS				CTransBroker::sm_lpfnGetItemDetails;
FP_CANCELTRANSACTION			CTransBroker::sm_lpfnCancelTransaction;
FP_COPYFILEFROMSERVER			CTransBroker::sm_lpfnCopyFileFromServer;
FP_COPYFILETOSERVER				CTransBroker::sm_lpfnCopyFileToServer;
FP_ENTERITEM					CTransBroker::sm_lpfnEnterItem;
FP_FINISH						CTransBroker::sm_lpfnFinish;
FP_GETPINWORKINGKEY				CTransBroker::sm_lpfnGetPINWorkingKey;
FP_GETTENDERDETAILS				CTransBroker::sm_lpfnGetTenderDetails;
FP_GETTOTALDETAILS				CTransBroker::sm_lpfnGetTotalDetails;
FP_INITIALIZE					CTransBroker::sm_lpfnInitialize;
FP_KEYPRESS						CTransBroker::sm_lpfnKeyPress;
FP_LOADOPTIONS					CTransBroker::sm_lpfnLoadOptions;
FP_LOGERRORTOSERVER				CTransBroker::sm_lpfnLogErrorToServer;
FP_REFRESHSTATE					CTransBroker::sm_lpfnRefreshState;
FP_RELEASEJOURNALDATA			CTransBroker::sm_lpfnReleaseJournalData;
FP_RELEASEMESSAGE				CTransBroker::sm_lpfnReleaseMessage;
FP_RELEASERECEIPTDATA			CTransBroker::sm_lpfnReleaseReceiptData;
FP_RELEASESLIPDATA				CTransBroker::sm_lpfnReleaseSlipData;
FP_RESUMETRANSACTION			CTransBroker::sm_lpfnResumeTransaction;
FP_SETMONITORALL				CTransBroker::sm_lpfnSetMonitorAll;
FP_START						CTransBroker::sm_lpfnStart;
FP_SUSPENDTRANSACTION			CTransBroker::sm_lpfnSuspendTransaction;
FP_UNINITIALIZE					CTransBroker::sm_lpfnUninitialize;
FP_EPSSTARTOFDAY				CTransBroker::sm_lpfnEPSStartOfDay;
FP_EPSENDOFDAY					CTransBroker::sm_lpfnEPSEndOfDay;
FP_GETAVAILABLEFOODSTAMPAMOUNT	CTransBroker::sm_lpfnGetAvailableFoodStampAmount;
FP_GETAVAILABLEBENEFITAMOUNT	CTransBroker::sm_lpfnGetAvailableBenefitAmount;
FP_REQUESTFOODSTAMPELIGIBLETOTAL	CTransBroker::sm_lpfnRequestFoodStampEligibleTotal;
FP_PARSEFOODSTAMPTOTAL				CTransBroker::sm_lpfnParseFoodStampTotal;
FP_SENDSIGNON					CTransBroker::sm_lpfnSendSignOn;
FP_SENDSIGNOFF					CTransBroker::sm_lpfnSendSignOff;
FP_SENDLOCKON					CTransBroker::sm_lpfnSendLockOn;
FP_SENDLOCKOFF					CTransBroker::sm_lpfnSendLockOff;
FP_INITHOSTAPPEXIT				CTransBroker::sm_lpfnInitHostAppExit;
FP_LANECLOSED					CTransBroker::sm_lpfnLaneClosed;
FP_ENTERDOB						CTransBroker::sm_lpfnEnterDOB;
FP_SENDWEIGHT					CTransBroker::sm_lpfnSendWeight;
FP_GETTERMINALNUMBER			CTransBroker::sm_lpfnGetTerminalNumber;
FP_GETVERSION					CTransBroker::sm_lpfnGetVersion;
FP_GETTRANSACTIONLIMIT			CTransBroker::sm_lpfnGetTransactionLimit;
FP_GETQUANTITYLIMIT				CTransBroker::sm_lpfnGetQuantityLimit;
FP_ISALLOWED					CTransBroker::sm_lpfnIsAllowed;
FP_VALIDOPERATOR				CTransBroker::sm_lpfnValidOperator;
FP_VALIDPASSWORD				CTransBroker::sm_lpfnValidPassword;
FP_GETJOURNALDATA				CTransBroker::sm_lpfnGetJournalData;
FP_GETRECEIPTDATA				CTransBroker::sm_lpfnGetReceiptData;
FP_GETSLIPDATA					CTransBroker::sm_lpfnGetSlipData;
FP_PROCESSMESSAGE				CTransBroker::sm_lpfnProcessMessage;
FP_VERIFYLOYALTYCARD			CTransBroker::sm_lpfnVerifyLoyaltyCard;
FP_ENTERTENDER					CTransBroker::sm_lpfnEnterTender;
FP_VALIDATEOPERATOR				CTransBroker::sm_lpfnValidateOperator;
FP_GET2X20						CTransBroker::sm_lpfnGet2x20;
FP_SENDMSG2HOST					CTransBroker::sm_lpfnSendMsg2Host;
FP_ENTERMICRWITHTYPE            CTransBroker::sm_lpfnEnterMICRWithType;  //RFC 356020

//Old TBAPIs list: --end

typedef struct _stFnPtrMap
   {
   LPCSTR szName;
   FARPROC *lppProc;
   } FUNCTIONPOINTERMAP;

//
// To add a new function to CTransBroker, simply:
// * add the appropriate typedef to CTransBroker.h
// * add a static member pointer to a function of that type to the CTransBroker.h
// * add the declaration of the static to the list of statics above.
// * add the function name and function pointer to the table below.
// * implement a wrapper function that calls the fn ptr if the fn exists.
// That's it!  Simple...
//

FUNCTIONPOINTERMAP s_FnPtrMap[] = 
   {
      {"TBGetItemExtendedDetails",       (FARPROC*)&CTransBroker::sm_lpfnGetItemExtendedDetails,},
      {"TBEnterTenderState",             (FARPROC*)&CTransBroker::sm_lpfnEnterTenderState,},
      {"TBSendSigData",                  (FARPROC*)&CTransBroker::sm_lpfnSendSigData,},
      {"TBSetTrainingModeState",         (FARPROC*)&CTransBroker::sm_lpfnSetTrainingModeState,},
      {"TBGetMessageInfo",               (FARPROC*)&CTransBroker::sm_lpfnGetMessageInfo,},
	  {"TBCreateReportEntry",            (FARPROC*)&CTransBroker::sm_lpfnCreateReportEntry,},//TD 3.1
      {"TBGetCurrentTransactionNumber",  (FARPROC*)&CTransBroker::sm_lpfnGetCurrentTransactionNumber,},
      {"TBGetCurrentBusinessDate",       (FARPROC*)&CTransBroker::sm_lpfnGetCurrentBusinessDate,},
      {"TBSetBusinessDate",              (FARPROC*)&CTransBroker::sm_lpfnSetBusinessDate,},
      {"TBGetCustomerMessage",           (FARPROC*)&CTransBroker::sm_lpfnGetCustomerMessage,},
      {"TBGetRewardLineDetails",         (FARPROC*)&CTransBroker::sm_lpfnGetRewardLineDetails,},
      {"TBNeedStartOfDay",               (FARPROC*)&CTransBroker::sm_lpfnNeedStartOfDay,},
      {"TBEnterMICR",                    (FARPROC*)&CTransBroker::sm_lpfnEnterMICR,},
      {"TBGetDocEndorsementInfo",        (FARPROC*)&CTransBroker::sm_lpfnGetDocEndorsementInfo,},
      {"TBSetCashDrawerState",           (FARPROC*)&CTransBroker::sm_lpfnSetCashDrawerState,},
      {"TBGetExpectedCashDrawerState",   (FARPROC*)&CTransBroker::sm_lpfnGetExpectedCashDrawerState,},
      {"TBSetSecurityLevel",             (FARPROC*)&CTransBroker::sm_lpfnSetSecurityLevel,},
      {"TBGetExpectedSecurityLevel",     (FARPROC*)&CTransBroker::sm_lpfnGetExpectedSecurityLevel,},
      {"TBGetCurrentSecurityLevel",      (FARPROC*)&CTransBroker::sm_lpfnGetCurrentSecurityLevel,},
      {"TBEndorsementCompleted",         (FARPROC*)&CTransBroker::sm_lpfnEndorsementCompleted,},
      {"TBAssistModeStatus",             (FARPROC*)&CTransBroker::sm_lpfnGetAssistModeStatus,},
      {"TBPrint",                        (FARPROC*)&CTransBroker::sm_lpfnPrint,},
      {"TBSuspendTransactionEX",         (FARPROC*)&CTransBroker::sm_lpfnSuspendTransactionEx,},      
      {"TBGetPrinterError",              (FARPROC*)&CTransBroker::sm_lpfnGetPrinterError,},      
      {"TBGetAssistModeStatus",          (FARPROC*)&CTransBroker::sm_lpfnGetAssistModeStatus,},      
      {"TBGetMICRPrompt",                (FARPROC*)&CTransBroker::sm_lpfnGetMICRPrompt,},      
      {"TBGetDataNeededView",            (FARPROC*)&CTransBroker::sm_lpfnGetDataNeededView,},
	  {"TBSendDataNeeded",               (FARPROC*)&CTransBroker::sm_lpfnSendDataNeeded,},
	  {"TBGetLoyaltyCardNumber",         (FARPROC*)&CTransBroker::sm_lpfnGetLoyaltyCardNumber,},
	  {"TBDisableTenderButton",          (FARPROC*)&CTransBroker::sm_lpfnDisableTenderButton,},
	  {"TBGetGiftCardBalance",           (FARPROC*)&CTransBroker::sm_lpfnGetGiftCardBalance,},
      {"TBLogReceipt",                   (FARPROC*)&CTransBroker::sm_lpfnLogReceipt,},
      {"TBPreprocessSignatureData",      (FARPROC*)&CTransBroker::sm_lpfnPreprocessSignatureData,},
      {"TBSendTBCommand",                (FARPROC*)&CTransBroker::sm_lpfnSendTBCommand,},
      {"TBGetExtensionStateSetup",       (FARPROC*)&CTransBroker::sm_lpfnGetExtensionStateSetup},
      {"TBNotifyExtensionStateEvent",    (FARPROC*)&CTransBroker::sm_lpfnNotifyExtensionStateEvent},
      {"TBGetNextState",                 (FARPROC*)&CTransBroker::sm_lpfnGetNextState,},
      {"TBPreprocessData",               (FARPROC*)&CTransBroker::sm_lpfnPreprocessData,},
      {"TBSyncTBorPOS",                  (FARPROC*)&CTransBroker::sm_lpfnSyncTBorPOS,}, //tar 288628
      {"TBPowerShutdown",                (FARPROC*)&CTransBroker::sm_lpfnPowerShutdown,},   //dp185016 tbpowershutdown added rfc 265463
      {"TBValidateHeadCashier",          (FARPROC*)&CTransBroker::sm_lpfnValidateHeadCashier,},   //dp185016 support glory device
      //Old TBAPIs list: ++begin	
		{"TBGetItemDetails",				 (FARPROC*)&CTransBroker::sm_lpfnGetItemDetails,},
		{"TBCancelTransaction",				(FARPROC*)&CTransBroker::sm_lpfnCancelTransaction,},
		{"TBCopyFileFromServer",			(FARPROC*)&CTransBroker::sm_lpfnCopyFileFromServer,},
		{"TBCopyFileToServer",				(FARPROC*)&CTransBroker::sm_lpfnCopyFileToServer,},
		{"TBEnterItem",						(FARPROC*)&CTransBroker::sm_lpfnEnterItem,},
		{"TBFinish",						(FARPROC*)&CTransBroker::sm_lpfnFinish,},
		{"TBGetPINWorkingKey",				(FARPROC*)&CTransBroker::sm_lpfnGetPINWorkingKey,},
		{"TBGetTenderDetails",				(FARPROC*)&CTransBroker::sm_lpfnGetTenderDetails,},
		{"TBGetTotalDetails",				(FARPROC*)&CTransBroker::sm_lpfnGetTotalDetails,},
		{"TBInitialize",					(FARPROC*)&CTransBroker::sm_lpfnInitialize,},
		{"TBKeyPress",						(FARPROC*)&CTransBroker::sm_lpfnKeyPress,},
		{"TBLoadOptions",					(FARPROC*)&CTransBroker::sm_lpfnLoadOptions,},
		{"TBLogErrorToServer",				(FARPROC*)&CTransBroker::sm_lpfnLogErrorToServer,},
		{"TBRefreshState",					(FARPROC*)&CTransBroker::sm_lpfnRefreshState,},
		{"TBReleaseJournalData",			(FARPROC*)&CTransBroker::sm_lpfnReleaseJournalData,},
		{"TBReleaseMessage",				(FARPROC*)&CTransBroker::sm_lpfnReleaseMessage,},
		{"TBReleaseReceiptData",			(FARPROC*)&CTransBroker::sm_lpfnReleaseReceiptData,},
		{"TBReleaseSlipData",				(FARPROC*)&CTransBroker::sm_lpfnReleaseSlipData,},
		{"TBResumeTransaction",				(FARPROC*)&CTransBroker::sm_lpfnResumeTransaction,},
		{"TBSetMonitorAll",					(FARPROC*)&CTransBroker::sm_lpfnSetMonitorAll,},
		{"TBStart",							(FARPROC*)&CTransBroker::sm_lpfnStart,},
		{"TBSuspendTransaction",			(FARPROC*)&CTransBroker::sm_lpfnSuspendTransaction,},
		{"TBUninitialize",					(FARPROC*)&CTransBroker::sm_lpfnUninitialize,},
		{"TBEPSStartOfDay",					(FARPROC*)&CTransBroker::sm_lpfnEPSStartOfDay,},
		{"TBEPSEndOfDay",					(FARPROC*)&CTransBroker::sm_lpfnEPSEndOfDay,},
		{"TBGetAvailableFoodStampAmount",	(FARPROC*)&CTransBroker::sm_lpfnGetAvailableFoodStampAmount,},
		{"TBGetAvailableBenefitAmount",		(FARPROC*)&CTransBroker::sm_lpfnGetAvailableBenefitAmount,},
		{"TBRequestFoodStampEligibleTotal",	(FARPROC*)&CTransBroker::sm_lpfnRequestFoodStampEligibleTotal,},
		{"TBParseFoodStampTotal",			(FARPROC*)&CTransBroker::sm_lpfnParseFoodStampTotal,},
		{"TBSendSignOn",					(FARPROC*)&CTransBroker::sm_lpfnSendSignOn,},
		{"TBSendSignOff",					(FARPROC*)&CTransBroker::sm_lpfnSendSignOff,},
		{"TBSendLockOn",					(FARPROC*)&CTransBroker::sm_lpfnSendLockOn,},
		{"TBSendLockOff",					(FARPROC*)&CTransBroker::sm_lpfnSendLockOff,},
		{"TBInitHostAppExit",				(FARPROC*)&CTransBroker::sm_lpfnInitHostAppExit,},
		{"TBLaneClosed",					(FARPROC*)&CTransBroker::sm_lpfnLaneClosed,},
		{"TBEnterDOB",						(FARPROC*)&CTransBroker::sm_lpfnEnterDOB,},
		{"TBSendWeight",					(FARPROC*)&CTransBroker::sm_lpfnSendWeight,},
		{"TBGetTerminalNumber",				(FARPROC*)&CTransBroker::sm_lpfnGetTerminalNumber,},
		{"TBGetVersion",					(FARPROC*)&CTransBroker::sm_lpfnGetVersion,},
		{"TBGetTransactionLimit",			(FARPROC*)&CTransBroker::sm_lpfnGetTransactionLimit,},
		{"TBGetQuantityLimit",				(FARPROC*)&CTransBroker::sm_lpfnGetQuantityLimit,},
		{"TBIsAllowed",						(FARPROC*)&CTransBroker::sm_lpfnIsAllowed,},
		{"TBValidOperator",					(FARPROC*)&CTransBroker::sm_lpfnValidOperator,},
		{"TBValidPassword",					(FARPROC*)&CTransBroker::sm_lpfnValidPassword,},
		{"TBGetJournalData",				(FARPROC*)&CTransBroker::sm_lpfnGetJournalData,},
		{"TBGetReceiptData",				(FARPROC*)&CTransBroker::sm_lpfnGetReceiptData,},
		{"TBGetSlipData",					(FARPROC*)&CTransBroker::sm_lpfnGetSlipData,},
		{"TBProcessMessage",				(FARPROC*)&CTransBroker::sm_lpfnProcessMessage,},
		{"TBVerifyLoyaltyCard",				(FARPROC*)&CTransBroker::sm_lpfnVerifyLoyaltyCard,},
		{"TBEnterTender",					(FARPROC*)&CTransBroker::sm_lpfnEnterTender,},
		{"TBValidateOperator",				(FARPROC*)&CTransBroker::sm_lpfnValidateOperator,},
		{"TBGet2x20",						(FARPROC*)&CTransBroker::sm_lpfnGet2x20,},
		{"TBSendMsg2Host",					(FARPROC*)&CTransBroker::sm_lpfnSendMsg2Host,},
        {"TBEnterMICRWithType",             (FARPROC*)&CTransBroker::sm_lpfnEnterMICRWithType,},   //RFC 356020

		//Old TBAPIs list: --end
	  // Add all new functions BEFORE the following element!
   {NULL,NULL}
   };


void GetTBReturnStateName(TBRC nTBState, CString & csStateName);
void GetTBReturnCodeName(TBRC nTBRetCode, CString & csCodeName);

CTransBroker::CTransBroker()
{
	Trace(L0,_T("+CTransBroker::CTransBroker()"));
	pTraceObj = NULL;
	pErrorObj = NULL;
	fTBInit = false;
  InitFunctionPointers();
  Trace(L0,_T("-CTransBroker::CTransBroker()"));
}


CTransBroker::CTransBroker(TraceObject*	pAppTraceObj, SCOTAPPErrorObject* pAppErrorObj)
{
  Trace(L0,_T("+CTransBroker::CTransBroker()"));
  pTraceObj = pAppTraceObj;
  pErrorObj = pAppErrorObj;
  InitFunctionPointers();
  Trace(L0,_T("-CTransBroker::CTransBroker()"));
}


CTransBroker::~CTransBroker()
{
	Trace(L0,_T("+CTransBroker::~CTransBroker()"));
	// allow application to delete these objects
	pTraceObj = NULL;
	pErrorObj = NULL;
   // Clear out static function pointers.
   for (int iIndex = 0; s_FnPtrMap[iIndex].szName; iIndex++)
      {
      *(s_FnPtrMap[iIndex]).lppProc = NULL;
      }
   Trace(L0,_T("-CTransBroker::~CTransBroker()"));
}

void CTransBroker::InitFunctionPointers()
{
  HMODULE hModule;
  CString csModuleName;
  Trace(L0,_T("+CTransBroker::InitFunctionPointers()"));
#ifdef _DEBUG
#ifdef _UNICODE
	csModuleName = _T("TransBrokerUD.dll");
#else
	csModuleName = _T("TransBrokerD.dll");
#endif
#else
#ifdef _UNICODE
	csModuleName = _T("TransBrokerU.dll");
#else
	csModuleName = _T("TransBroker.dll");
#endif
#endif

	hModule = GetModuleHandle(csModuleName);

   if (hModule)
      {
      for (int iIndex = 0; s_FnPtrMap[iIndex].szName; iIndex++)
         {
         // This assertion likely means more than one instance of 
         // CTransBroker has been created.  If you really want to do this,
         // some implementation deatails (namely statics) of this class 
         // will have to change.
         ASSERT(NULL == *(s_FnPtrMap[iIndex]).lppProc);

         *(s_FnPtrMap[iIndex]).lppProc = GetProcAddress(hModule,  s_FnPtrMap[iIndex].szName);

         if(!*(s_FnPtrMap[iIndex]).lppProc)
            {
            LPVOID lpMsgBuf;
            FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | 
               FORMAT_MESSAGE_FROM_SYSTEM | 
               FORMAT_MESSAGE_IGNORE_INSERTS,
               NULL,
               GetLastError(),
               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
               (LPTSTR) &lpMsgBuf,
               0,
               NULL 
               );

            Trace(L6,_T("InitFunctionPointers GetProcAddress(%s) failed::%s"),s_FnPtrMap[iIndex].szName,(LPCTSTR)lpMsgBuf);
            // Free the message buffer.
            LocalFree( lpMsgBuf );
            }
         }
      }
   else
      {
      LPVOID lpMsgBuf;
      FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | 
         FORMAT_MESSAGE_FROM_SYSTEM | 
         FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL,
         GetLastError(),
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         (LPTSTR) &lpMsgBuf,
         0,
         NULL 
         );
      Trace(L0,_T("InitFunctionPointers GetModuleHandle(%s) failed:%s"),csModuleName,(LPCTSTR)lpMsgBuf);
      LocalFree( lpMsgBuf );
      }
   Trace(L0,_T("-CTransBroker::InitFunctionPointers()"));
}

void CTransBroker::SetLogObjects(TraceObject*	pAppTraceObj, SCOTAPPErrorObject* pAppErrorObj)
{
	pTraceObj = pAppTraceObj;
	pErrorObj = pAppErrorObj;
//	Trace(L6, "SetLogObjects");
	Trace(L0,_T("CTransBroker::SetLogObjects"));
}


TBRC CTransBroker::CancelTransaction(const BOOL fOverride=FALSE)
{
	Trace(L0,_T("+CTransBroker::CancelTransaction"));
	TBRC nRetCode;
	//nRetCode = ::TBCancelTransaction(fOverride);
	if(sm_lpfnCancelTransaction)
		nRetCode = (*sm_lpfnCancelTransaction)(fOverride);
	
	// Function name with input parameters and output state
	CString csRetCode;
	::GetTBReturnCodeName(nRetCode, csRetCode);
//	Trace(L6, "CancelTransaction: fOverride(In) = %d, RetCode = %s", fOverride, csRetCode);
	Trace(L0,_T("-CTransBroker::CancelTransaction"));
	return nRetCode;  
}

TBRC CTransBroker::CopyFileFromServer(LPCTSTR szFileName, LPCTSTR
						   szDestinationFilePath, const BOOL fForce)
{
	Trace(L0,_T("+CTransBroker::CopyFileFromServer"));
	TBRC nRetCode;
		//nRetCode = ::TBCopyFileFromServer(szFileName, szDestinationFilePath, fForce);
	if(sm_lpfnCopyFileFromServer)
		nRetCode = (*sm_lpfnCopyFileFromServer)(szFileName, szDestinationFilePath, fForce);
	else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
	// Function name with input parameters and output state
	if (nRetCode == TB_E_FILENOTFOUND)
	{
		// SCOTApp calls this function to let TB to copy any updated or optional files
		// If TB doesn't find a file to copy from server, it is not an error w.r.t SCOT
		// and hence don't log it as an error:  TAR: 149978
		nRetCode = TB_SUCCESS;
	}
	Trace(L0,_T("-CTransBroker::CopyFileFromServer"));
	return nRetCode;  
}

TBRC CTransBroker::CopyFileToServer(LPCTSTR szFileName, LPCTSTR szSourceFilePath)
{
	Trace(L0,_T("+CTransBroker::CopyFileToServer"));
	TBRC nRetCode;
	//nRetCode = ::TBCopyFileToServer(szFileName, szSourceFilePath);
	if(sm_lpfnCopyFileToServer)
		nRetCode = (*sm_lpfnCopyFileToServer)(szFileName, szSourceFilePath);
	else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
	// Function name with input parameters and output state
	CString csRetCode;
	::GetTBReturnCodeName(nRetCode, csRetCode);
//	Trace(L6, "CopyFileToServer: szFileName(In)=%s, szSourceFilePath(In)=%s, RetCode = %s",
//		  szFileName, szSourceFilePath, csRetCode);
	Trace(L0,_T("-CTransBroker::CopyFileToServer"));
	return nRetCode;  
}



TBRC CTransBroker::EnterItem(LPCTSTR szItemCode, 
						   PSEnterItemDetails psMoreItemDetails, 
						   const BOOL fVoid=FALSE,
						   const BOOL fOverride=FALSE, const long nQuantity=0,
						   const long nWeight=0, const long nTareCode=0, 
						   const long lPrice=0, const BOOL fQuantityConfirmed=FALSE)
{
	Trace(L0,_T("+CTransBroker::EnterItem"));
	TBRC nRetCode;
	/*nRetCode = ::TBEnterItem(szItemCode, fVoid, fOverride, nQuantity, nWeight,
							 nTareCode, lPrice, fQuantityConfirmed, psMoreItemDetails);*/
	if(sm_lpfnEnterItem)
		nRetCode = (*sm_lpfnEnterItem)(szItemCode, fVoid, fOverride, nQuantity, nWeight,
							 nTareCode, lPrice, fQuantityConfirmed, psMoreItemDetails);

	CString csRetCode;
	::GetTBReturnCodeName(nRetCode, csRetCode);
//	Trace(L6, "EnterItem:Code:%s,Void:%d,Ovrd:%d,Qty:%d,Wgt:%d,Tare:%d,Price:%d,UPrice:%d,QtyCnf:%d,ScanType:%d,OrgCode:%s,RetCode:%s",\
//		  szItemCode, fVoid, fOverride, nQuantity, nWeight, nTareCode, lPrice, psMoreItemDetails->lUnitPrice,fQuantityConfirmed,\
//		  psMoreItemDetails->lItemScanCodeType, psMoreItemDetails->szOriginalCode, csRetCode);
	Trace(L0,_T("-CTransBroker::EnterItem"));
	return nRetCode;  
}

TBRC CTransBroker::EnterTender(TBTENDERTYPE nTenderType, const long lAmount,
						   LPCTSTR szCardData=NULL, const int nPINLength=0,
						   LPCTSTR szEncryptedPIN=NULL, const BOOL fOverride=FALSE,
						   const BOOL fVoid=FALSE)
{
	Trace(L0,_T("+CTransBroker::EnterTender"));
	TBRC nRetCode;
	/*nRetCode = ::TBEnterTender(nTenderType, lAmount, szCardData, nPINLength, szEncryptedPIN,
							   fOverride, fVoid);*/
	if(sm_lpfnEnterTender)
		nRetCode = (*sm_lpfnEnterTender)(nTenderType, lAmount, szCardData, nPINLength, szEncryptedPIN,
							   fOverride, fVoid);

#ifdef LOG_THIS
	// log info
	CString csRetCode, csCardData;
	::GetTBReturnCodeName(nRetCode, csRetCode);
	CString csEncryptedPIN(szEncryptedPIN, 16);

	// extract card data
	if(szCardData && _tcsclen(szCardData))
	{
		short int nT1, nT2, nT3, nHeaderLen;
		nT1 = nT2 = nT3 = 0;
		nT1 = (short ) *szCardData;		//length of Track 1
		nT2 = (short ) *(szCardData+2);	//length of Track 2
		nT3 = (short ) *(szCardData+4);	//length of Track 3
		nHeaderLen = nT1+nT2+nT3;
		LPTSTR szTrackData = new _TCHAR[nHeaderLen+1];
		memcpy(szTrackData, szCardData+(3 * sizeof(short)), nHeaderLen+1);
		szTrackData[nHeaderLen] = _T('\0');
		csCardData = szTrackData;
		delete [] szTrackData;
	}

//	Trace(L6, "EnterTender:TdrType:%d,Amt:%d,Data:%s,PNLng:%d,NcryptPN:%s,Od:%d,Vd:%d,retCode:%s",			
//			nTenderType, lAmount, csCardData, nPINLength, csEncryptedPIN, fOverride, fVoid, csRetCode);

	csEncryptedPIN.Empty();
	csCardData.Empty();
#endif // LOG_THIS
	Trace(L0,_T("-CTransBroker::EnterTender"));
	return nRetCode;  
}

TBRC CTransBroker::Finish(void)
{
	Trace(L0,_T("+CTransBroker::Finish(void)"));
	TBRC nRetCode;
	//nRetCode = ::TBFinish();
	if(sm_lpfnFinish)
		nRetCode = (*sm_lpfnFinish)();
	CString csRetCode;
	::GetTBReturnCodeName(nRetCode, csRetCode);
//	Trace(L6, "Finish()");
	Trace(L0,_T("-CTransBroker::Finish(void)"));
	return nRetCode;  
}

TBRC CTransBroker::GetItemDetails(PSItemDetails psItemDetails)
{
	Trace(L0,_T("+CTransBroker::GetItemDetails(void)"));
	TBRC nRetCode;
	//nRetCode = ::TBGetItemDetails(psItemDetails);
	if(sm_lpfnGetItemDetails)
		nRetCode = (*sm_lpfnGetItemDetails)(psItemDetails);

	CString csRetCode;
	::GetTBReturnCodeName(nRetCode, csRetCode);
/*	Trace(L6,"ItmDetail:Code:%s,Desc:%s,DescLn:%d,Exp:%d,Unp:%d,Qty:%d,Wgt:%d,DQty:%d,Cpn?:%d,Qty?:%d,Wgt?:%d,Prc?:%d,NotFound:%d,NotForSal:%d,Rstd?:%d,scantype:%d,LnkItm?:%d,Err:%s,ErrLng:%d, RetCode:%s",
			psItemDetails->szItemCode,
			psItemDetails->szDescription,
			psItemDetails->lDescriptionLength,
			psItemDetails->lExtPrice,
			psItemDetails->lUnitPrice,
			psItemDetails->lQuantity,
			psItemDetails->lWeight,
			psItemDetails->lDealQuantity,
			psItemDetails->fCoupon,
			psItemDetails->fNeedQuantity,
			psItemDetails->fNeedWeight,
			psItemDetails->fNeedPrice,
			psItemDetails->fNotFound,
			psItemDetails->fNotForSale,
			psItemDetails->fRestricted,
			psItemDetails->lScanCodeType,
			psItemDetails->bLinkedItem,
			psItemDetails->szErrDescription,
			psItemDetails->lErrDescriptionLength, csRetCode);
*/
	Trace(L0,_T("-CTransBroker::GetItemDetails(void)"));
	return nRetCode;  
}

TBRC CTransBroker::GetItemExtendedDetails(PSItemExtendedDetails psItemExtendedDetails)
{
	Trace(L0,_T("+CTransBroker::GetItemExtendedDetails"));
	TBRC nRetCode;
	
  if(sm_lpfnGetItemExtendedDetails)
		nRetCode = (*sm_lpfnGetItemExtendedDetails)(psItemExtendedDetails);
  else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
	CString csRetCode;
	::GetTBReturnCodeName(nRetCode, csRetCode);
	Trace(L0,_T("-CTransBroker::GetItemExtendedDetails"));
	return nRetCode;  
}


// Enter or Leave lane Closed state
TBRC CTransBroker::EnterTenderState(const bool bEnter)  
{
	Trace(L0,_T("+CTransBroker::EnterTenderState"));
	TBRC nRetCode;
	
  if(sm_lpfnEnterTenderState)
		nRetCode = (*sm_lpfnEnterTenderState)(bEnter);
  else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

    CString csRetCode;
	::GetTBReturnCodeName(nRetCode, csRetCode);
    Trace(L6, _T("EnterTenderState retCode:%s"), csRetCode);
    Trace(L0,_T("-CTransBroker::EnterTenderState"));	
	return nRetCode; 
}

TBRC CTransBroker::SendSigData(LPCTSTR CustomerName, 
       LPCTSTR cSigData, 
       DWORD nLength)
{
	Trace(L0,_T("+CTransBroker::SendSigData"));
	TBRC nRetCode;
	
  if(sm_lpfnSendSigData)
		nRetCode = (*sm_lpfnSendSigData)(CustomerName, cSigData, nLength);
  else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;


  CString csRetCode;
	::GetTBReturnCodeName(nRetCode, csRetCode);
  Trace(L6, _T("SendSigData retCode:%s"), csRetCode);
	Trace(L0,_T("-CTransBroker::SendSigData"));
	return nRetCode; 
}


TBRC CTransBroker::GetTenderDetails(PSTenderDetails psTenderDetails)
{
	Trace(L0,_T("+CTransBroker::GetTenderDetails"));
	TBRC nRetCode;
	//nRetCode = ::TBGetTenderDetails(psTenderDetails);
	if(sm_lpfnGetTenderDetails)
		nRetCode = (*sm_lpfnGetTenderDetails)(psTenderDetails);
	else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

	CString csRetCode;
	::GetTBReturnCodeName(nRetCode, csRetCode);
/*	Trace(L6, "TndrDetail:lTndr:%d, lBal:%d,lChange:%d,Desc:%s,DescLn:%d,Err:%s,ErrLn:%d, retCode:%s",
			psTenderDetails->lTender,
			psTenderDetails->lBalance,
			psTenderDetails->lChange,
			psTenderDetails->szDescription,
			psTenderDetails->lDescriptionLength,
			psTenderDetails->szErrDescription,
		psTenderDetails->lErrDescriptionLength, csRetCode); 
*/
	Trace(L0,_T("-CTransBroker::GetTenderDetails"));
	return nRetCode;  
}

TBRC CTransBroker::Print(LPCTSTR szPrintFile)
{
	Trace(L0,_T("+CTransBroker::Print"));
	TBRC nRetCode;
   if(sm_lpfnPrint)
		nRetCode = (*sm_lpfnPrint)(szPrintFile);
   else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

	CString csRetCode;
	::GetTBReturnCodeName(nRetCode, csRetCode);
	Trace(L0,_T("-CTransBroker::Print"));
	return nRetCode;  
}

TBRC CTransBroker::GetTotalDetails(PSTotalDetails psTotalDetails)
{
	Trace(L0,_T("+CTransBroker::GetTotalDetails"));
	TBRC nRetCode;
	//nRetCode = ::TBGetTotalDetails(psTotalDetails);
	if(sm_lpfnGetTotalDetails)
		nRetCode = (*sm_lpfnGetTotalDetails)(psTotalDetails);
	else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

	CString csRetCode;
	::GetTBReturnCodeName(nRetCode, csRetCode);
/*	Trace(L6, "TotalDetail:Total:%d,Tax:%d,FoodStampDue:%d,Discount:%d,Err:%s,ErrLn:%d, retCode:%s",
			psTotalDetails->lTotal,
			psTotalDetails->lTaxDue,
			psTotalDetails->lFoodStampDue,
			psTotalDetails->lDiscount,
			psTotalDetails->szErrDescription,
			psTotalDetails->lErrDescriptionLength,
		csRetCode);
*/
	Trace(L0,_T("-CTransBroker::GetTotalDetails"));
	return nRetCode;  
}


long CTransBroker::GetTransactionLimit(void)
{
   Trace(L0,_T("+CTransBroker::GetTransactionLimit(void)"));
   long  rc=0;
   //rc = ::TBGetTransactionLimit();
   if(sm_lpfnGetTransactionLimit)
		rc = (*sm_lpfnGetTransactionLimit)();
	else
		rc = TB_E_FUNCTIONNOTSUPPORTED;
   Trace(L0,_T("-CTransBroker::GetTransactionLimit(void)"));
   return rc;
}

long CTransBroker::GetQuantityLimit(void)
{
	Trace(L0,_T("+CTransBroker::GetQuantityLimit(void)"));
   long  rc=0;
   //rc = ::TBGetQuantityLimit();
   if(sm_lpfnGetQuantityLimit)
		rc = (*sm_lpfnGetQuantityLimit)();
	else
		rc = TB_E_FUNCTIONNOTSUPPORTED;
   Trace(L0,_T("-CTransBroker::GetQuantityLimit(void)"));
   return rc;
}


HFILE CTransBroker::GetJournalData()
{
	Trace(L0,_T("+CTransBroker::GetJournalData()"));
	HFILE nRetHandle;
	//nRetHandle = ::TBGetJournalData();
	if(sm_lpfnGetJournalData)
		nRetHandle = (*sm_lpfnGetJournalData)();
	else
		nRetHandle = TB_E_FUNCTIONNOTSUPPORTED;
	Trace(L0,_T("-CTransBroker::GetJournalData()"));
	return nRetHandle;  
}

HFILE CTransBroker::GetReceiptData()
{
	Trace(L0,_T("+CTransBroker::GetReceiptData()"));
	HFILE nRetHandle;
	//nRetHandle = ::TBGetReceiptData();
	if(sm_lpfnGetReceiptData)
		nRetHandle = (*sm_lpfnGetReceiptData)();
	else
		nRetHandle = TB_E_FUNCTIONNOTSUPPORTED;
	Trace(L0,_T("-CTransBroker::GetReceiptData()"));
	return nRetHandle;  
}

HFILE CTransBroker::GetSlipData()
{
	Trace(L0,_T("+CTransBroker::GetSlipData()"));
	HFILE nRetHandle;
	//nRetHandle = ::TBGetSlipData();
	if(sm_lpfnGetSlipData)
		nRetHandle = (*sm_lpfnGetSlipData)();
	else
		nRetHandle = TB_E_FUNCTIONNOTSUPPORTED;
	Trace(L0,_T("-CTransBroker::GetSlipData()"));
	return nRetHandle;  
}

TBRC CTransBroker::GetPINWorkingKey(PSPINWorkingKey psPINWorkingKey)
{
	Trace(L0,_T("+CTransBroker::GetPINWorkingKey"));
	TBRC nRetCode;
	//nRetCode = ::TBGetPINWorkingKey(psPINWorkingKey);
	if(sm_lpfnGetPINWorkingKey)
		nRetCode = (*sm_lpfnGetPINWorkingKey)(psPINWorkingKey);
	else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
	Trace(L0,_T("-CTransBroker::GetPINWorkingKey"));
	return nRetCode;  
}

TBRC CTransBroker::ReleaseMessage(PSMessageElement psMessage)
{
	Trace(L0,_T("+CTransBroker::ReleaseMessage"));
	TBRC nRetCode;
	//nRetCode = ::TBReleaseMessage(psMessage);
	if(sm_lpfnReleaseMessage)
		nRetCode = (*sm_lpfnReleaseMessage)(psMessage);
	else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
	Trace(L0,_T("-CTransBroker::ReleaseMessage"));
	return nRetCode;  
}

long CTransBroker::GetTerminalNumber(void)
{
	Trace(L0,_T("+CTransBroker::GetTerminalNumber(void)"));
	long nTermNum;
	//nTermNum = ::TBGetTerminalNumber();
	if(sm_lpfnGetTerminalNumber)
		nTermNum = (*sm_lpfnGetTerminalNumber)();
	Trace(L0,_T("-CTransBroker::GetTerminalNumber(void)"));
	return nTermNum;  
}

long CTransBroker::GetCurrentTransactionNumber(void)
{
	Trace(L0,_T("+CTransBroker::GetCurrentTransactionNumber(void)"));
	long nTrxNum = -9999;
	if (sm_lpfnGetCurrentTransactionNumber)
	{
		nTrxNum = (*sm_lpfnGetCurrentTransactionNumber)();
	}
	Trace(L0,_T("-CTransBroker::GetCurrentTransactionNumber(void)"));
	return nTrxNum;  
}

long CTransBroker::GetVersion(CString &cVersionString)
{
	Trace(L0,_T("+CTransBroker::GetVersion"));
   long  len, rc=0;

   //len = ::TBGetVersion(NULL, 0);
   if(sm_lpfnGetVersion)
		len = (*sm_lpfnGetVersion)(NULL, 0);
   if (len > 0) {
	  LPTSTR pchBuffer = new _TCHAR[len];
	  if (pchBuffer!=NULL) {
		 //rc = ::TBGetVersion(pchBuffer, len);
		 rc = (*sm_lpfnGetVersion)(pchBuffer, len);	
		 cVersionString = pchBuffer;
		 delete [] pchBuffer;
	  }
   }
	Trace(L0,_T("-CTransBroker::GetVersion"));
   return rc;
}

TBSTATE CTransBroker::ProcessMessage(MessageElement* psMessage)
{
	Trace(L0,_T("+CTransBroker::ProcessMessage"));
	TBSTATE nRetState;
	PSMessageElement pTmpMessage = (PSMessageElement) psMessage;
	//nRetState = ::TBProcessMessage(pTmpMessage);
	if(sm_lpfnProcessMessage)
		nRetState = (*sm_lpfnProcessMessage)(pTmpMessage);
	Trace(L0,_T("-CTransBroker::ProcessMessage"));
	return nRetState;  
}

TBRC CTransBroker::Initialize(HWND hWnd, LPCTSTR szFilePath)
{
	Trace(L0,_T("+CTransBroker::Initialize"));
	TBRC nRetCode;
		//nRetCode = ::TBInitialize(hWnd, szFilePath);
	if(sm_lpfnInitialize)
		nRetCode = (*sm_lpfnInitialize)(hWnd, szFilePath);
	// TAR 203679 - set fTBInit flag to false if TBInitialize failed
	if (nRetCode < TB_SUCCESS)
		fTBInit = false;
	else
		fTBInit = true;
	Trace(L0,_T("-CTransBroker::Initialize"));
	return nRetCode;  
}

BOOL CTransBroker::IsAllowed(TBFEATURE nFeature)
{
	Trace(L0,_T("+CTransBroker::IsAllowed"));
	BOOL nRetState;
	//nRetState = ::TBIsAllowed(nFeature);
	if(sm_lpfnIsAllowed)
		nRetState = (*sm_lpfnIsAllowed)(nFeature);
	Trace(L0,_T("-CTransBroker::IsAllowed"));
	return nRetState;  
}

TBRC CTransBroker::KeyPress(const USHORT uKey)
{
	Trace(L0,_T("+CTransBroker::KeyPress"));
	TBRC nRetCode;
	//nRetCode = ::TBKeyPress(uKey);
	if(sm_lpfnKeyPress)
		nRetCode = (*sm_lpfnKeyPress)(uKey);
	Trace(L0,_T("-CTransBroker::KeyPress"));
	return nRetCode;  
}

TBRC CTransBroker::LoadOptions(void)
{
	Trace(L0,_T("+CTransBroker::LoadOptions(void)"));
	TBRC nRetCode;
	//nRetCode = ::TBLoadOptions();
	if(sm_lpfnLoadOptions)
		nRetCode = (*sm_lpfnLoadOptions)();
	Trace(L0,_T("-CTransBroker::LoadOptions(void)"));
	return nRetCode;  
}

TBRC CTransBroker::LogReceipt(LPTSTR* lpFileName,BSTR* bstrSignatureString, LPTSTR* lpReceiptType)
{
  Trace(L0,_T("+CTransBroker::LogReceipt"));
  TBRC nRetCode;
	
  if(sm_lpfnLogReceipt)
		nRetCode = (*sm_lpfnLogReceipt)(lpFileName, bstrSignatureString,lpReceiptType );
  else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
  CString csRetCode;
  ::GetTBReturnCodeName(nRetCode, csRetCode);

  Trace(L6, _T("LogReceipt retCode:%s"), csRetCode);
  Trace(L0,_T("-CTransBroker::LogReceipt"));
  return nRetCode;   
}

TBRC CTransBroker::PreprocessSignatureData(LPCTSTR lpFileName,BSTR bstrSignatureData, LPCTSTR lpReceiptType, BOOL* bProcessData)
{
  Trace(L0,_T("+CTransBroker::PreprocessSignatureData"));
  TBRC nRetCode;
	
  if(sm_lpfnPreprocessSignatureData)
		nRetCode = (*sm_lpfnPreprocessSignatureData)(lpFileName, bstrSignatureData, lpReceiptType, bProcessData);
  else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
  CString csRetCode;
  ::GetTBReturnCodeName(nRetCode, csRetCode);

  Trace(L6, _T("PreprocessSignatureData retCode:%s"), csRetCode);
  Trace(L0,_T("-CTransBroker::PreprocessSignatureData"));
  return nRetCode;   
}


TBRC CTransBroker::LogErrorToServer(LPCTSTR szErrorText, const int nSeverity,
						   const int nComponentID, LPCTSTR szComponentID,
						   LPCTSTR szModuleID, const int nModuleLineNumber)
{
	Trace(L0,_T("+CTransBroker::LogErrorToServer"));
	TBRC nRetCode;
	/*nRetCode = ::TBLogErrorToServer(szErrorText, nSeverity, nComponentID, szComponentID,
						 szModuleID, nModuleLineNumber);*/
	if(sm_lpfnLogErrorToServer)
		nRetCode = (*sm_lpfnLogErrorToServer)(szErrorText, nSeverity, nComponentID, szComponentID,
						 szModuleID, nModuleLineNumber);
	Trace(L0,_T("-CTransBroker::LogErrorToServer"));
	return nRetCode;  
}

TBRC CTransBroker::RefreshState(void)
{
	Trace(L0,_T("+CTransBroker::RefreshState(void)"));
	TBRC nRetCode;
	//nRetCode = ::TBRefreshState();
	if(sm_lpfnRefreshState)
		nRetCode = (*sm_lpfnRefreshState)();
	Trace(L0,_T("-CTransBroker::RefreshState(void)"));
	return nRetCode;  
}

TBRC CTransBroker::ReleaseJournalData(HFILE hDataFile)
{
	Trace(L0,_T("+CTransBroker::ReleaseJournalData"));
	TBRC nRetCode;
	//nRetCode = ::TBReleaseJournalData(hDataFile);
	if(sm_lpfnReleaseJournalData)
		nRetCode = (*sm_lpfnReleaseJournalData)(hDataFile);
	Trace(L0,_T("-CTransBroker::ReleaseJournalData"));
	return nRetCode;  
}

TBRC CTransBroker::ReleaseReceiptData(HFILE hDataFile)
{
	Trace(L0,_T("+CTransBroker::ReleaseReceiptData"));
	TBRC nRetCode;
	//nRetCode = ::TBReleaseReceiptData(hDataFile);
	if(sm_lpfnReleaseReceiptData)
		nRetCode = (*sm_lpfnReleaseReceiptData)(hDataFile);
	Trace(L0,_T("-CTransBroker::ReleaseReceiptData"));
	return nRetCode;  
}

TBRC CTransBroker::ReleaseSlipData(HFILE hDataFile)
{
	Trace(L0,_T("+CTransBroker::ReleaseSlipData"));
	TBRC nRetCode;
	//nRetCode = ::TBReleaseSlipData(hDataFile);
	if(sm_lpfnReleaseSlipData)
		nRetCode = (*sm_lpfnReleaseSlipData)(hDataFile);
	Trace(L0,_T("-CTransBroker::ReleaseSlipData"));
	return nRetCode;  
}

TBRC CTransBroker::ResumeTransaction(LPCTSTR szSuspendID)
{
	Trace(L0,_T("+CTransBroker::ResumeTransaction"));
	TBRC nRetCode;
	//nRetCode = ::TBResumeTransaction(szSuspendID);
	if(sm_lpfnResumeTransaction)
		nRetCode = (*sm_lpfnResumeTransaction)(szSuspendID);
	Trace(L0,_T("-CTransBroker::ResumeTransaction"));
	return nRetCode;  
}

TBRC CTransBroker::SetMonitorAll(BOOL fMonitor=TRUE)
{
	Trace(L0,_T("+CTransBroker::SetMonitorAll"));
	TBRC nRetCode;
	//nRetCode = ::TBSetMonitorAll(fMonitor); 
	if(sm_lpfnSetMonitorAll)
		nRetCode = (*sm_lpfnSetMonitorAll)(fMonitor);
	Trace(L0,_T("-CTransBroker::SetMonitorAll"));
	return nRetCode;  

}

TBRC CTransBroker::Start(LPCTSTR szOverrideCode)
{
	Trace(L0,_T("+CTransBroker::Start"));
	TBRC nRetCode;
	//nRetCode =  ::TBStart(szOverrideCode);
	if(sm_lpfnStart)
		nRetCode = (*sm_lpfnStart)(szOverrideCode);
	Trace(L0,_T("-CTransBroker::Start"));
	return nRetCode;
}

// This function is no longer being called by any standard routines
TBRC CTransBroker::SuspendTransaction(const BOOL fOverride)
{
	Trace(L0,_T("+CTransBroker::SuspendTransaction"));
	TBRC nRetCode;
   //nRetCode = ::TBSuspendTransaction(fOverride);
	if(sm_lpfnSuspendTransaction)
		nRetCode = (*sm_lpfnSuspendTransaction)(fOverride);
    Trace(L0,_T("-CTransBroker::SuspendTransaction"));
	return nRetCode;
}

TBRC CTransBroker::SuspendTransactionEX(const BOOL fOverride, LPCTSTR szSuspendReasons)
{
	Trace(L0,_T("+CTransBroker::SuspendTransactionEX"));
	TBRC nRetCode;
   // Call the suspend transaction for fiscal printer if it exists
   // Otherwise, call the "old" function
   if(sm_lpfnSuspendTransactionEx)
   {
		nRetCode = (*sm_lpfnSuspendTransactionEx)(fOverride, szSuspendReasons);
   }
   else
   {
      nRetCode = ::TBSuspendTransaction(fOverride);
   }
   Trace(L0,_T("-CTransBroker::SuspendTransactionEX"));
	return nRetCode;
}
TBRC CTransBroker::Uninitialize(void)
{
	Trace(L0,_T("+CTransBroker::Uninitialize(void)"));
	TBRC nRetCode;
	//nRetCode = ::TBUninitialize();
	if(sm_lpfnUninitialize)
		nRetCode = (*sm_lpfnUninitialize)();
	fTBInit = false;
	Trace(L0,_T("-CTransBroker::Uninitialize(void)"));
	return nRetCode;
}

BOOL CTransBroker::ValidateOperator(LPCTSTR szOperatorID, LPCTSTR szOperatorPw)
{
	Trace(L0,_T("+CTransBroker::ValidateOperator"));
	BOOL nRetState;
	//nRetState = ::TBValidateOperator(szOperatorID, szOperatorPw);
	if(sm_lpfnValidateOperator)
		nRetState = (*sm_lpfnValidateOperator)(szOperatorID, szOperatorPw);
	Trace(L0,_T("-CTransBroker::ValidateOperator"));
	return nRetState;
}

BOOL CTransBroker::ValidOperator(LPCTSTR szOperatorID)
{
	Trace(L0,_T("+CTransBroker::ValidOperator"));
	BOOL nRetState;
	//nRetState = ::TBValidOperator(szOperatorID);
	if(sm_lpfnValidOperator)
		nRetState = (*sm_lpfnValidOperator)(szOperatorID);
	Trace(L0,_T("-CTransBroker::ValidOperator"));
	return nRetState;
}

BOOL CTransBroker::ValidPassword(LPCTSTR szOperatorPw)
{
	Trace(L0,_T("+CTransBroker::ValidPassword"));
	BOOL nRetState;
	//nRetState = ::TBValidPassword(szOperatorPw);
	if(sm_lpfnValidPassword)
		nRetState = (*sm_lpfnValidPassword)(szOperatorPw);
	Trace(L0,_T("-CTransBroker::ValidPassword"));
	return nRetState;
}

TBLOYALTYCARDTYPE CTransBroker::VerifyLoyaltyCard(LPCTSTR szCardData, int nDataLen)
{
	Trace(L0,_T("+CTransBroker::VerifyLoyaltyCard"));
	TBLOYALTYCARDTYPE nRetCardType;
	//nRetCardType = ::TBVerifyLoyaltyCard(szCardData,nDataLen);
	if(sm_lpfnVerifyLoyaltyCard)
		nRetCardType = (*sm_lpfnVerifyLoyaltyCard)(szCardData,nDataLen);
	Trace(L0,_T("-CTransBroker::VerifyLoyaltyCard"));
	return nRetCardType;
}

TBRC  CTransBroker::SendMsg2Host(LPCTSTR szSend,const int iMsgLen, const TBDEVICEID nDeviceClass,
								 const UCHAR nFunctionCode, const long	lOptFileHandle,
								 const bool  fOriginatePipeMsg)
{
	Trace(L0,_T("+CTransBroker::SendMsg2Host"));
	TBRC nRetCode;
		//nRetCode = ::TBSendMsg2Host(szSend,iMsgLen,nDeviceClass,nFunctionCode,lOptFileHandle,fOriginatePipeMsg);
	if(sm_lpfnSendMsg2Host)
		nRetCode = (*sm_lpfnSendMsg2Host)(szSend,iMsgLen,nDeviceClass,nFunctionCode,lOptFileHandle,fOriginatePipeMsg);
	Trace(L0,_T("-CTransBroker::SendMsg2Host"));
	return nRetCode;
}


TBRC  CTransBroker::Get2x20(LPCTSTR szDisplayStr)
{
	Trace(L0,_T("+CTransBroker::Get2x20"));
	TBRC nRetCode;
	//nRetCode = ::TBGet2x20(szDisplayStr);
	if(sm_lpfnGet2x20)
		nRetCode = (*sm_lpfnGet2x20)(szDisplayStr);
	Trace(L0,_T("-CTransBroker::Get2x20"));
	return nRetCode;
}


TBRC  CTransBroker::EPSEndOfDay(LPVOID lp)
{
	Trace(L0,_T("+CTransBroker::EPSEndOfDay"));
	TBRC nRetCode;
	//nRetCode = ::TBEPSEndOfDay(lp);
	if(sm_lpfnEPSEndOfDay)
		nRetCode = (*sm_lpfnEPSEndOfDay)(lp);
	Trace(L0,_T("-CTransBroker::EPSEndOfDay"));
	return nRetCode;
}

TBRC  CTransBroker::EPSStartOfDay(LPVOID lp)
{
	Trace(L0,_T("+CTransBroker::EPSStartOfDay"));
	TBRC nRetCode;
	//nRetCode = ::TBEPSStartOfDay(lp);
	if(sm_lpfnEPSStartOfDay)
		nRetCode = (*sm_lpfnEPSStartOfDay)(lp);
	Trace(L0,_T("-CTransBroker::EPSStartOfDay"));
	return nRetCode;
}

TBRC CTransBroker::GetAvailableFoodStampAmount(PSEBTAvailBalance psEBTAvailBalance)
{
	Trace(L0,_T("+CTransBroker::GetAvailableFoodStampAmount"));
	TBRC nRetCode;
	//nRetCode = ::TBGetAvailableFoodStampAmount(psEBTAvailBalance);
	if(sm_lpfnGetAvailableFoodStampAmount)
		nRetCode = (*sm_lpfnGetAvailableFoodStampAmount)(psEBTAvailBalance);
	Trace(L0,_T("-CTransBroker::GetAvailableFoodStampAmount"));
	return nRetCode;
}

TBRC CTransBroker::GetAvailableBenefitAmount(PSEBTAvailBalance psEBTAvailBalance)
{
	Trace(L0,_T("+CTransBroker::GetAvailableBenefitAmount"));
	TBRC nRetCode;
	//nRetCode = ::TBGetAvailableBenefitAmount(psEBTAvailBalance);
	if(sm_lpfnGetAvailableBenefitAmount)
		nRetCode = (*sm_lpfnGetAvailableBenefitAmount)(psEBTAvailBalance);
	Trace(L0,_T("-CTransBroker::GetAvailableBenefitAmount"));
	return nRetCode;
}

TBRC CTransBroker::RequestFoodStampEligibleTotal(void)
{
	Trace(L0,_T("+CTransBroker::RequestFoodStampEligibleTotal"));
	TBRC nRetCode;
	//nRetCode = ::TBRequestFoodStampEligibleTotal();
	if(sm_lpfnRequestFoodStampEligibleTotal)
		nRetCode = (*sm_lpfnRequestFoodStampEligibleTotal)();
	Trace(L0,_T("-CTransBroker::RequestFoodStampEligibleTotal"));
	return nRetCode;
}

TBRC CTransBroker::ParseFoodStampTotal(PSTotalDetails psTotalDetails)
{
	Trace(L0,_T("+CTransBroker::ParseFoodStampTotal"));
	TBRC nRetCode;
	//nRetCode = ::TBParseFoodStampTotal(psTotalDetails);
	if(sm_lpfnParseFoodStampTotal)
		nRetCode = (*sm_lpfnParseFoodStampTotal)(psTotalDetails);
	Trace(L0,_T("-CTransBroker::ParseFoodStampTotal"));
	return nRetCode;
}


TBRC CTransBroker::SendSignOn(void)
{
	Trace(L0,_T("+CTransBroker::SendSignOn(void)"));
	TBRC nRetCode;
	//nRetCode = ::TBSendSignOn();
	if(sm_lpfnSendSignOn)
		nRetCode = (*sm_lpfnSendSignOn)();
	Trace(L0,_T("-CTransBroker::SendSignOn(void)"));
	return nRetCode;
}


TBRC CTransBroker::SendSignOff(void)
{
	Trace(L0,_T("+CTransBroker::SendSignOff(void)"));
	TBRC nRetCode;
	//nRetCode = ::TBSendSignOff();
	if(sm_lpfnSendSignOff)
		nRetCode = (*sm_lpfnSendSignOff)();
	Trace(L0,_T("-CTransBroker::SendSignOff(void)"));
	return nRetCode;
}


// set the manager's key lock ON
TBRC CTransBroker::SendLockOn(void)
{
	Trace(L0,_T("+CTransBroker::SendLockOn(void)"));
	TBRC nRetCode;
	//nRetCode = ::TBSendLockOn();
	if(sm_lpfnSendLockOn)
		nRetCode = (*sm_lpfnSendLockOn)();
	Trace(L0,_T("-CTransBroker::SendLockOn(void)"));
	return nRetCode;
}


// set the manager's key lock OFF
TBRC CTransBroker::SendLockOff(void)
{
	Trace(L0,_T("+CTransBroker::SendLockOff(void)"));
	TBRC nRetCode;
	//nRetCode = ::TBSendLockOff();
	if(sm_lpfnSendLockOff)
		nRetCode = (*sm_lpfnSendLockOff)();
	Trace(L0,_T("-CTransBroker::SendLockOff(void)"));
	return nRetCode;
}


// send key(s) to host application to exit
TBRC CTransBroker::InitHostAppExit(void)
{
	Trace(L0,_T("+CTransBroker::InitHostAppExit(void)"));
	TBRC nRetCode;
	//nRetCode = ::TBInitHostAppExit();
	if(sm_lpfnInitHostAppExit)
		nRetCode = (*sm_lpfnInitHostAppExit)();
	Trace(L0,_T("-CTransBroker::InitHostAppExit(void)"));
	return nRetCode;
}

// Enter or Leave lane Closed state
TBRC CTransBroker::LaneClosed(const bool bEnter)  
{
	Trace(L0,_T("+CTransBroker::LaneClosed"));
	TBRC nRetCode;
		//nRetCode = ::TBLaneClosed(bEnter);
	if(sm_lpfnLaneClosed)
		nRetCode = (*sm_lpfnLaneClosed)(bEnter);
	Trace(L0,_T("-CTransBroker::LaneClosed"));
	return nRetCode;
}



TBRC CTransBroker::EnterDOB(int nYear, int nMonth, int nDay, int nCustomerAge)
{
	Trace(L0,_T("+CTransBroker::EnterDOB"));
	TBRC nRetCode;
	//nRetCode = ::TBEnterDOB(nYear, nMonth, nDay, nCustomerAge);
	if(sm_lpfnEnterDOB)
		nRetCode = (*sm_lpfnEnterDOB)(nYear, nMonth, nDay, nCustomerAge);
	Trace(L0,_T("-CTransBroker::EnterDOB"));
	return nRetCode;
}

TBRC CTransBroker::SendWeight(long lWeight)
{
	Trace(L0,_T("+CTransBroker::SendWeight"));
	TBRC nRetCode;
	//nRetCode = ::TBSendWeight(lWeight);
	if(sm_lpfnSendWeight)
		nRetCode = (*sm_lpfnSendWeight)(lWeight);
	Trace(L0,_T("-CTransBroker::SendWeight"));
	return nRetCode;
}

void GetTBReturnCodeName(TBRC nTBRetCode, CString & csCodeName)
{
	// please update this function if you add or see new TB return codes
	// a code is new if it is not listed below.
	static LPTSTR pTBReturnCodes[] =
	{
		_T("TB_SUCCESS"),				//	      = 0;
		_T("TB_SUCCESS_IMMEDIATE"),		//		  = 1;
		_T("TB_E_INVALIDUSERID"),		//	      = -2;
		_T("TB_E_INVALIDUSERPASSWORD"), //        = -3;
		_T("TB_E_INVALIDSTATE"),		//        = -4;
		_T("TB_E_FILENOTFOUND"),		//        = -5;
		_T("TB_E_INCOMPLETEPROCEDURE"), //		  = -6;
		_T("TB_E_INVALIDWINDOW"),		//		  = -7;
		_T("TB_E_DISKFULL"),			//        = -8;
		_T("TB_E_INVALIDPARAM"),		//		  = -9;
		_T("TB_E_NOHOSTCREDIT"),		//		  = -10;
		_T("TB_E_NOWORKINGKEY"),		//		  = -11;
		_T("TB_E_NOTINITIALIZED"),		//		  = -100;
		_T("TB_E_NOHOST"),				//		  = -101;
		_T("TB_E_UNEXPECTED"),			//		  = -666;
		_T("TB_E_FUNCTIONNOTSUPPORTED"),//		  = -12;
		_T("TB_E_ITEMFAILED"),			//		  = -13;
		_T("TB_E_CARDDATA_MISSING"),    //		  = -14;
		_T("TB_IGNORE")					//		  = 11;
	};

	short int nTBRetCodeNameIndex = -1;
	switch (nTBRetCode)
	{
        case  TB_SUCCESS:				//	      = 0;
				nTBRetCodeNameIndex = 0;
				break;
		case TB_SUCCESS_IMMEDIATE:      //		  = 1
				nTBRetCodeNameIndex = 1;
				break;
        case  TB_E_INVALIDUSERID:		//	      = -2;
				nTBRetCodeNameIndex = 2;
				break;
        case  TB_E_INVALIDUSERPASSWORD: //        = -3;
				nTBRetCodeNameIndex = 3;
				break;
        case  TB_E_INVALIDSTATE:		//        = -4;
				nTBRetCodeNameIndex = 4;
				break;
        case  TB_E_FILENOTFOUND:		//        = -5;
				nTBRetCodeNameIndex = 5;
				break;
        case  TB_E_INCOMPLETEPROCEDURE: //		  = -6;
				nTBRetCodeNameIndex = 6;
				break;
        case  TB_E_INVALIDWINDOW:		//		  = -7;
				nTBRetCodeNameIndex = 7;
				break;
        case  TB_E_DISKFULL:			//        = -8;
				nTBRetCodeNameIndex = 8;
				break;
        case  TB_E_INVALIDPARAM:		//		  = -9;
				nTBRetCodeNameIndex = 9;
				break;
        case  TB_E_NOHOSTCREDIT:		//		  = -19;
				nTBRetCodeNameIndex = 10;
				break;
        case  TB_E_NOWORKINGKEY:		//		  = -11;
				nTBRetCodeNameIndex = 11;
				break;
        case  TB_E_NOTINITIALIZED:		//		  = -100;
				nTBRetCodeNameIndex = 12;
				break;
        case  TB_E_NOHOST:				//		  = -101;
				nTBRetCodeNameIndex = 13;
				break;
        case  TB_E_UNEXPECTED:			//		  = -666;
				nTBRetCodeNameIndex = 14;
				break;
		case TB_E_FUNCTIONNOTSUPPORTED: //        = -12
				nTBRetCodeNameIndex = 15;
				break;
        case TB_E_ITEMFAILED:           //        = -13
				nTBRetCodeNameIndex = 16;
				break;
        case TB_E_CARDDATA_MISSING:     //		  = -14
				nTBRetCodeNameIndex = 17;
				break;
		case TB_IGNORE:					//		  = 11
				nTBRetCodeNameIndex = 18;
				break;				
		default:
				ASSERT(0); // this function has to be updated with new TB return codes
				break;
	}

	if (nTBRetCodeNameIndex >= 0)
	{
		csCodeName = (LPCTSTR) pTBReturnCodes[nTBRetCodeNameIndex];
	}
	else
	{
		csCodeName = _T("UNKNOWN TB RETURN CODE");
	}

}

void GetTBReturnStateName(TBRC nTBState, CString & csStateName)
{
	static LPTSTR pTBReturnStates[] =
	{
		_T("NewTBState"),
		_T("CLOSED"),
		_T("CMOS_DOSCLOSE"),
		_T("CMOS_DOSOPEN"),
		_T("CMOS_DOSREAD"),
		_T("CMOS_DOSSEEK"),
		_T("CMOS_DOSWRITE"),
		_T("CMOS_PURGE"),
		_T("COMPLETEPRINT"),
		_T("COUPONEXCEEDED"),
		_T("COUPONNOTMATCH"),
		_T("IGNORE"),
		_T("INVALIDITEMVOID"),
		_T("ITEMQTYEXCEEDED"), // this is a threshold to ask for confirmation - not a per item limit
		_T("ITEMRECALLED"),
		_T("ITEMSOLD"),
		_T("ITEMUNKNOWN"),
		_T("LOYALTYCARD"),
		_T("LOYALTYINVALID"),
		_T("NEEDOVERRIDE"),
		_T("NEEDPRICE"),
		_T("NEEDQUANTITY"),
		_T("NEEDTARE"),
		_T("NEEDWEIGHT"),
		_T("OPTIONLOADING"),
		_T("PRINTIMMEDIATE"),
		_T("PRINTIMMEDIATECUT"),
		_T("READY"),
		_T("SECURED"),
		_T("TENDERACCEPTED"),
		_T("TENDERDECLINED"),
		_T("TOTAL"),
		_T("TRXEND"),
		_T("TRXLIMITEXCEEDED"),
		_T("TRXVOIDED"),
		_T("TRXVOIDEXCEEDED"),
		_T("UNKNOWN"),
		_T("VOIDLIMITEXCEEDED"),
		_T("VOIDNOTMATCH"),
		_T("VOIDITEMBEFORECOUPON"),
		_T("TRXSUSPENDED"),
		_T("TRXSUSPENSIONUNCOMPLETED"),
		_T("TENDERDECLINEDKEEPCARD"),
		_T("CASHWAUTHORIZED"),
		_T("CASHWNOTAUTHORIZED"),
		_T("CASHWCONFIRMED"),
		_T("CASHWNOTCONFIRMED"),
		_T("CASHWCANCELLED"),
		_T("INVALIDPIN"),
		_T("EPSOFFLINE"),
		_T("EPSSODOK"),	// SOD: start of day
		_T("EPSSODNOK"),
		_T("EPSEODOK"),	// EOD: end of day
		_T("EPSEODNOK"),
		_T("TENDERFOODSTAMPINSUFFICIENT"),
		_T("FOODSTAMPELIGIBLETOTAL"),
		_T("ITEM_QUANTITY_RESTRICTED"), // this is a per item limit - not the same as the threshold for getting confirmation
		_T("VISUAL_VERIFY"),
		_T("TIMERESTRICTEDITEM"),
		_T("TENDERCASHBENEFITINSUFFICIENT"),
		_T("ITEMSALECOMPLETED"),            // if the item sold is complete. SCOT will enable scanner when it receives this state
		_T("INVALIDAMOUNT"),
		_T("EPSONLINE"),
		_T("NEWENCRYPTIONKEY"),
		_T("INTTRXREPLY"),
		_T("ATMPOWERUP"),
		_T("ATMOFFLINE"),
		_T("ATMONLINE"),
		_T("ATMEJPRINTCOMPLETE"),
  	    _T("TENDEREBTINSUFFICIENT"),
		_T("MESSAGESCREEN"),                   // TAR 127247
		_T("ENTEREDTRAINING"),
		_T("LEFTTRAINING"),
		_T("TENDERCANCELLED"),
		_T("CUSTOMERMESSAGE"),			// optional: TB implements this state to support consumer marketing
		_T("REWARDLINE"),
		_T("CRATEABLEITEM"),
		_T("NEEDWEIGHTBIZERBA"),
		_T("SENDMESSAGETOSCOT"),
		_T("TRXEND_PRINTANDDISPENSE"),
		_T("CLOSED_NOPRINTANDNODISPENSE"),
		_T("NEEDMOREDATA"),
		_T("NEEDMICR"),
		_T("ENDORSEDOCUMENT"),
		_T("NEWCASHDRAWERSTATE"),
		_T("NEWSECURITYLEVEL"),
		_T("DOCUMENTNUM"),
      _T("COMPLETEPRINTANDCUT"),
      _T("ASSISTMODESTATUSCHANGED"),
 		_T("PRINTER_VERIFY"),
		_T("CREATE_REPORT_ENTRY"),
		_T("ASSISTMODETABFLUSH"),
        _T("GETBUSINESSDATE"),
		_T("HOSTOFFLINE"),
		_T("HOSTONLINE"),
    _T("CHANGESTATE"), // tar 284767
    _T("OUTOFSYNCNEEDSTOP"), //dp185016 Req SR058
    _T("OUTOFSYNCNEEDASSISTMODE"), //dp185016 Req SR058
		_T("TENDERVOIDED"),  
		_T("LANECLOSED"),	//RFC 986   
		_T("LANEOPEN"),	    //RFC 986
		_T("REPRINTRECEIPT"), 
		_T("REPRINTRECEIPTFAILED"),
		_T("UNKNOWNTBSTATE")
	};

	if (nTBState >= 1 && nTBState < (sizeof(pTBReturnStates) / sizeof(pTBReturnStates[0])))
	{
      csStateName = (LPCTSTR) pTBReturnStates[nTBState];
	}
	else
	{
	   TRACE(_T("warning: !!!!! Couldnt find the state name for TB state id: %d"), nTBState);
		csStateName = (LPCTSTR) pTBReturnStates[0];
	}
}

TBRC CTransBroker::IsTBInitialized()
{
	Trace(L0,_T("+CTransBroker::IsTBInitialized()"));
	TBRC nRetCode;
	if(fTBInit)
	  nRetCode = TB_SUCCESS;
	else
		nRetCode = TB_E_NOTINITIALIZED;
	Trace(L0,_T("-CTransBroker::IsTBInitialized()"));
	return nRetCode;
}


TBRC CTransBroker::SetTrainingModeState(const bool bTMState)
{
	Trace(L0,_T("+CTransBroker::SetTrainingModeState"));
	TBRC nRetCode;
	if (sm_lpfnSetTrainingModeState)
	{
		nRetCode = (*sm_lpfnSetTrainingModeState)(bTMState);
	}
	else
	{
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
	}

	CString csRetCode;
	::GetTBReturnCodeName(nRetCode, csRetCode);

	Trace(L6, _T("SetTrainingModeState retCode:%s"), csRetCode);
	Trace(L0,_T("-CTransBroker::SetTrainingModeState"));
	return nRetCode; 
}

// TAR 127247 - GetMessageInfo()
 
TBRC CTransBroker::GetMessageInfo(PSMessageInfo psMessageInfo)
{
  Trace(L0,_T("+CTransBroker::GetMessageInfo"));
  TBRC nRetCode;
	
  if(sm_lpfnGetMessageInfo)
		nRetCode = (*sm_lpfnGetMessageInfo)(psMessageInfo);
  else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
  CString csRetCode;
  ::GetTBReturnCodeName(nRetCode, csRetCode);

  Trace(L6, _T("GetMessageInfo retCode:%s"), csRetCode);
  Trace(L0,_T("-CTransBroker::GetMessageInfo"));
  return nRetCode;   
}
TBRC CTransBroker::CreateReportEntry(PSCreateReportEntry psCreateReportEntry)
{
  Trace(L0,_T("+CTransBroker::CreateReportEntry"));
  TBRC nRetCode;
	
  if(sm_lpfnCreateReportEntry)
		nRetCode = (*sm_lpfnCreateReportEntry)(psCreateReportEntry);
  else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
  CString csRetCode;
  ::GetTBReturnCodeName(nRetCode, csRetCode);

  Trace(L6, _T("CreateReportEntry retCode:%s"), csRetCode);
  Trace(L0,_T("-CTransBroker::CreateReportEntry"));
  return nRetCode;   
}
TBRC CTransBroker::GetCurrentBusinessDate(CString &csBusinessDate)
{
  Trace(L0,_T("+CTransBroker::GetCurrentBusinessDate"));
  TBRC nRetCode;
	
  if(sm_lpfnGetCurrentBusinessDate)
		nRetCode = (*sm_lpfnGetCurrentBusinessDate)(csBusinessDate);
  else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
  CString csRetCode;
  ::GetTBReturnCodeName(nRetCode, csRetCode);

  Trace(L6, _T("GetCurrentBusinessDate retCode:%s"), csRetCode);
  Trace(L0,_T("-CTransBroker::GetCurrentBusinessDate"));
  return nRetCode;   
}
TBRC CTransBroker::SetBusinessDate(LPCTSTR szBusinessDate)
{
  Trace(L0,_T("+CTransBroker::SetBusinessDate"));
  TBRC nRetCode;
	
  if(sm_lpfnSetBusinessDate)
		nRetCode = (*sm_lpfnSetBusinessDate)(szBusinessDate);
  else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
  CString csRetCode;
  ::GetTBReturnCodeName(nRetCode, csRetCode);

  Trace(L6, _T("SetBusinessDate retCode:%s"), csRetCode);
  Trace(L0,_T("-CTransBroker::SetBusinessDate"));
  return nRetCode;   
}


TBRC CTransBroker::GetCustomerMessage(LPTSTR* psCustomerMessageText, LPTSTR* psCustomerMessageAudio)
{
  Trace(L0,_T("+CTransBroker::GetCustomerMessage"));
  TBRC nRetCode;
	
  if(sm_lpfnGetCustomerMessage)
		nRetCode = (*sm_lpfnGetCustomerMessage)(psCustomerMessageText, psCustomerMessageAudio);
  else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

  CString csRetCode;
  ::GetTBReturnCodeName(nRetCode, csRetCode);

  Trace(L6, _T("GetCustomerMessage() returns %s"), csRetCode);
  Trace(L0,_T("-CTransBroker::GetCustomerMessage"));
  return nRetCode;
}


TBRC CTransBroker::GetRewardLineDetails(PSRewardLineDetails psRewardLineDetails)
{
  Trace(L0,_T("+CTransBroker::GetRewardLineDetails"));
  TBRC nRetCode;
	
  if(sm_lpfnGetRewardLineDetails)
		nRetCode = (*sm_lpfnGetRewardLineDetails)(psRewardLineDetails);
  else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
  CString csRetCode;
  ::GetTBReturnCodeName(nRetCode, csRetCode);

  Trace(L6, _T("GetRewardLineDetails() returns %s"), csRetCode);
  Trace(L0,_T("-CTransBroker::GetRewardLineDetails"));
  return nRetCode;
}

TBRC CTransBroker::NeedStartOfDay(TBSTARTOFDAYSTATE &eState)
{
  Trace(L0,_T("+CTransBroker::NeedStartOfDay"));
  TBRC nRetCode;
	
  if(sm_lpfnNeedStartOfDay)
		nRetCode = (*sm_lpfnNeedStartOfDay)(eState);
  else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
  CString csRetCode;
  ::GetTBReturnCodeName(nRetCode, csRetCode);

  Trace(L6, _T("NeedStartOfDay() returns %s"), csRetCode);
  Trace(L0,_T("-CTransBroker::NeedStartOfDay"));
  return nRetCode;
}

TBRC CTransBroker::GetPrinterError(LPTSTR* psErrorMessage)
{
  Trace(L0,_T("+CTransBroker::GetPrinterError"));
  TBRC nRetCode;
	
  if(sm_lpfnGetPrinterError)
		nRetCode = (*sm_lpfnGetPrinterError)(psErrorMessage);
  else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

  CString csRetCode;
  ::GetTBReturnCodeName(nRetCode, csRetCode);

  Trace(L6, _T("GetPrinterError() returns %s"), csRetCode);
  Trace(L0,_T("-CTransBroker::GetPrinterError"));
  return nRetCode;
}

TBRC CTransBroker::EnterMICR(
   LPCTSTR szRawData,         // Raw MICR data from the device.
   LPCTSTR szTransitNumber,   // Bank Transit Number
   LPCTSTR szAccountNumber,   // Customer Account Number
   LPCTSTR szBankNumber,      // ABA Bank number
   LPCTSTR szSerialNumber,    // Check document number
   LPCTSTR szEPC,             // Extended Processigng Code
   long lType,                // Type (1=Personal, 2=Business, 99=Unknown)
   long lCountryCode)         // Country Code (1=USA, 2=Canada, 3=Mexico, 99=Unknown)
   {
   Trace(L0,_T("+CTransBroker::EnterMICR"));
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

   if(sm_lpfnEnterMICR)
	   nRetCode = (*sm_lpfnEnterMICR)(
         szRawData,             
         szTransitNumber,       
         szAccountNumber,       
         szBankNumber,          
         szSerialNumber,        
         szEPC,                 
         lType,
         lCountryCode);

   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("EnterMICR() returns %s"), csRetCode);
   Trace(L0,_T("-CTransBroker::EnterMICR"));
   return nRetCode;
   }

//Begin RFC 356020
TBRC CTransBroker::EnterMICRWithType(
	TBTENDERTYPE nTenderType,
   LPCTSTR szRawData,         // Raw MICR data from the device.
   LPCTSTR szTransitNumber,   // Bank Transit Number
   LPCTSTR szAccountNumber,   // Customer Account Number
   LPCTSTR szBankNumber,      // ABA Bank number
   LPCTSTR szSerialNumber,    // Check document number
   LPCTSTR szEPC,             // Extended Processigng Code
   long lType,                // Type (1=Personal, 2=Business, 99=Unknown)
   long lCountryCode)         // Country Code (1=USA, 2=Canada, 3=Mexico, 99=Unknown)
   {
   Trace(L0,_T("+CTransBroker::EnterMICRWithType"));
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

   if(sm_lpfnEnterMICRWithType)
	   nRetCode = (*sm_lpfnEnterMICRWithType)(
	     nTenderType,
         szRawData,             
         szTransitNumber,       
         szAccountNumber,       
         szBankNumber,          
         szSerialNumber,        
         szEPC,                 
         lType,
         lCountryCode);

   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("EnterMICRWithType() returns %s"), csRetCode);
   Trace(L0,_T("-CTransBroker::EnterMICRWithType"));
   return nRetCode;
   }
//End RFC 356020

TBRC CTransBroker::GetDocEndorsementInfo(
   long *plNumRemaining,      // Number of endorsements reamining.
   LPTSTR *ppszDocDescription,// Operator prompt or document description.
   LPTSTR *ppszDocFront,      // Text to print on front of document.
   LPTSTR *ppszDocBack)       // Text to print on back of document.
   {
   Trace(L0,_T("+CTransBroker::GetDocEndorsementInfo"));
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

   if(sm_lpfnGetDocEndorsementInfo)
      {
	   nRetCode = (*sm_lpfnGetDocEndorsementInfo)(
         plNumRemaining,
		 ppszDocDescription,         
         ppszDocFront,               
         ppszDocBack);
      }

   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("GetDocEndorsementInfo() returns %s, code:%d"), csRetCode, nRetCode);
   Trace(L0,_T("-CTransBroker::GetDocEndorsementInfo"));
   return nRetCode;
   }

TBRC CTransBroker::SetCashDrawerState(
   long iState)                // Bitmapped (Set=Open, Clear=Closed - lsb is drawer 1, etc.)
   {
   Trace(L0,_T("+CTransBroker::SetCashDrawerState"));
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

   if(sm_lpfnSetCashDrawerState)
	   nRetCode = (*sm_lpfnSetCashDrawerState)(iState);

   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("SetCashDrawerState() returns %s"), csRetCode);
   Trace(L0,_T("-CTransBroker::SetCashDrawerState"));
   return nRetCode;
   }

TBRC CTransBroker::GetExpectedCashDrawerState(long *plExpectedState)    // Bitmapped (lsb is drawer 1, etc.)
   {
   Trace(L0,_T("+CTransBroker::GetExpectedCashDrawerState"));
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

   if(sm_lpfnGetExpectedCashDrawerState)
      {
	   nRetCode = (*sm_lpfnGetExpectedCashDrawerState)(plExpectedState);
      }

   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("GetExpectedCashDrawerState() returns %s"), csRetCode);
   Trace(L0,_T("-CTransBroker::GetExpectedCashDrawerState"));
   return nRetCode;
   }

TBRC CTransBroker::SetSecurityLevel(
   long lSecurityLevel)      // Security level set by keylock or other auth means.
   {
   Trace(L0,_T("+CTransBroker::SetSecurityLevel"));
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

   if(sm_lpfnSetSecurityLevel)
	   nRetCode = (*sm_lpfnSetSecurityLevel)(lSecurityLevel);

   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("SetSecurityLevel() returns %s"), csRetCode);
   Trace(L0,_T("-CTransBroker::SetSecurityLevel"));
   return nRetCode;
   }

TBRC CTransBroker::GetExpectedSecurityLevel(long *plExpectedLevel)
   {
   Trace(L0,_T("+CTransBroker::GetExpectedSecurityLevel"));
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

   if(sm_lpfnGetExpectedSecurityLevel)
      {
	   nRetCode = (*sm_lpfnGetExpectedSecurityLevel)(plExpectedLevel);
      }

   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("GetExpectedSecurityLevel() returns %s"), csRetCode);
   Trace(L0,_T("-CTransBroker::GetExpectedSecurityLevel"));
   return nRetCode;
   }

TBRC CTransBroker::GetCurrentSecurityLevel(long *plCurrentLevel)
   {
   Trace(L0,_T("+CTransBroker::GetCurrentSecurityLevel"));
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

   if(sm_lpfnGetCurrentSecurityLevel)
      {
	  nRetCode = (*sm_lpfnGetCurrentSecurityLevel)(plCurrentLevel);
       }

   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("GetCurrentSecurityLevel() returns %s"), csRetCode);
   Trace(L0,_T("-CTransBroker::GetCurrentSecurityLevel"));
   return nRetCode;
   }

TBRC CTransBroker::EndorsementCompleted(
   bool bSuppressSubsequent)           // Flag to indicate to TB not to fire addl.
                                       // TB_ENDORSEDOCUMENT messages.
   {
   Trace(L0,_T("+CTransBroker::EndorsementCompleted"));
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

   if(sm_lpfnEndorsementCompleted)
	   nRetCode = (*sm_lpfnEndorsementCompleted)(bSuppressSubsequent);

   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("EndorsementCompleted() returns %s"), csRetCode);
   Trace(L0,_T("-CTransBroker::EndorsementCompleted"));
   return nRetCode;
   }

TBRC CTransBroker::GetAssistModeStatus(
   long *plAssistModeStatus)           
   {
   Trace(L0,_T("+CTransBroker::GetAssistModeStatus"));
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

   if(sm_lpfnGetAssistModeStatus)
	   nRetCode = (*sm_lpfnGetAssistModeStatus)(plAssistModeStatus);

   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("GetAssistModeStatus() returns %s"), csRetCode);
   Trace(L0,_T("-CTransBroker::GetAssistModeStatus"));
   return nRetCode;
   }

TBRC CTransBroker::GetMICRPrompt(
   LPTSTR *ppszMICRPrompt) // Operator prompt or MICR insertion.
   {
	Trace(L0,_T("+CTransBroker::GetMICRPrompt"));
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

   if(sm_lpfnGetMICRPrompt)
	   nRetCode = (*sm_lpfnGetMICRPrompt)(ppszMICRPrompt);

   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("GetMICRPrompt() returns %s"), csRetCode);
   Trace(L0,_T("-CTransBroker::GetMICRPrompt"));
   return nRetCode;
   }

TBRC CTransBroker::GetDataNeededView(PSDataNeededView psDataNeededView)
{
  Trace(L0,_T("+CTransBroker::GetDataNeededView"));
  TBRC nRetCode;
	
  if(sm_lpfnGetDataNeededView)
		nRetCode = (*sm_lpfnGetDataNeededView)(psDataNeededView);
  else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
  CString csRetCode;
  ::GetTBReturnCodeName(nRetCode, csRetCode);

  Trace(L6, _T("GetDataNeededView retCode:%s"), csRetCode);
  Trace(L0,_T("-CTransBroker::GetDataNeededView"));
  return nRetCode;   
}

TBRC CTransBroker::SendDataNeeded(LPCTSTR szDataNeededDetails)
{
  Trace(L0,_T("+CTransBroker::SendDataNeeded"));
  TBRC nRetCode;
	
  if(sm_lpfnSendDataNeeded)
		nRetCode = (*sm_lpfnSendDataNeeded)(szDataNeededDetails);
  else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
  CString csRetCode;
  ::GetTBReturnCodeName(nRetCode, csRetCode);

  Trace(L6, _T("SendDataNeeded retCode:%s"), csRetCode);
  Trace(L0,_T("-CTransBroker::SendDataNeeded"));
  return nRetCode;   
}

TBRC CTransBroker::GetLoyaltyCardNumber(
   LPTSTR *pszLoyaltyCardNumber) 
{
   Trace(L0,_T("+CTransBroker::GetLoyaltyCardNumber"));
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

   if(sm_lpfnGetLoyaltyCardNumber)
   {
	   nRetCode = (*sm_lpfnGetLoyaltyCardNumber)(pszLoyaltyCardNumber);
   }

   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("GetLoyaltyCardNumber() returns %s"), csRetCode);
   Trace(L0,_T("-CTransBroker::GetLoyaltyCardNumber"));
   return nRetCode;
}

BOOL CTransBroker::DisableTenderButton(
   TBTENDERTYPE nTenderType) 
{
   Trace(L0,_T("+CTransBroker::DisableTenderButton"));
   BOOL nRetCode = FALSE;

   if(sm_lpfnDisableTenderButton)
   {
	   nRetCode = (*sm_lpfnDisableTenderButton)(nTenderType);
   }

   Trace(L6, _T("DisableTenderButton() returns %d"), nRetCode);
   Trace(L0,_T("-CTransBroker::DisableTenderButton"));
   return nRetCode;
}

TBRC CTransBroker::GetGiftCardBalance(
   long *plBalance) 
{
   Trace(L0,_T("+CTransBroker::GetGiftCardBalance"));
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;

   if(sm_lpfnGetGiftCardBalance)
   {
	   nRetCode = (*sm_lpfnGetGiftCardBalance)(plBalance);
   }

   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("GetGiftCardBalance() returns %s"), csRetCode);
   Trace(L0,_T("-CTransBroker::GetGiftCardBalance"));
   return nRetCode;
}

TBRC CTransBroker::SendTBCommand(long lCommandID, va_list pArgs)
{
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
   if(sm_lpfnSendTBCommand)
   {
	   nRetCode = (*sm_lpfnSendTBCommand)(lCommandID, pArgs);
   }

   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("SendTBCommand() returns %s"), csRetCode);
   return nRetCode;

}

TBRC CTransBroker::GetExtensionStateSetup(LPTSTR szUICtx, LPTSTR szDevCtx, psPropChangeRequestList pExtChangeList)
{
	Trace(L0,_T("+CTransBroker::GetExtensionStateSetup"));
	TBRC nRetCode;

	if(sm_lpfnGetExtensionStateSetup)
		nRetCode = (*sm_lpfnGetExtensionStateSetup)(szUICtx, szDevCtx, pExtChangeList);
	else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
	CString csRetCode;
	::GetTBReturnCodeName(nRetCode, csRetCode);

	Trace(L6, _T("GetExtensionStateSetup retCode:%s"), csRetCode);
	Trace(L0,_T("-CTransBroker::GetExtensionStateSetup"));
	return nRetCode;   
}

TBSTATE CTransBroker::NotifyExtensionStateEvent(psExtStateEventNotifyIn pEventNotifyIn, psExtStateEventNotifyOut pEventNotifyOut)
{
	Trace(L0,_T("+CTransBroker::NotifyExtensionStateEvent"));
	TBSTATE nRetCode;

	if(sm_lpfnNotifyExtensionStateEvent)
		nRetCode = (*sm_lpfnNotifyExtensionStateEvent)(pEventNotifyIn, pEventNotifyOut);
	else
		nRetCode = TB_IGNORE;  // it means TB_E_FUNCTIONNOTSUPPORTED in this case

	CString csRetCode;
	::GetTBReturnStateName(nRetCode, csRetCode);

	Trace(L6, _T("NotifyExtensionStateEvent retState:%s"), csRetCode);
	Trace(L0,_T("-CTransBroker::NotifyExtensionStateEvent"));
	return nRetCode;   
}

TBRC CTransBroker::GetNextState(LPCTSTR lpLastStateName, 
                                LPCTSTR lpNextStateName, 
                                bool *pbOverride, 
                                LPTSTR *lpOverrideStateName, 
                                CDescriptorList &dlParameterList)
{
   Trace(L0,_T("+CTransBroker::GetNextState"));
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
   if(sm_lpfnGetNextState)
   {
	   nRetCode = (*sm_lpfnGetNextState)(lpLastStateName, lpNextStateName, pbOverride, lpOverrideStateName, dlParameterList);
   }

   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("GetNextState() returns %s"), csRetCode);
   Trace(L0,_T("-CTransBroker::GetNextState"));
   return nRetCode;

}

TBRC CTransBroker::PreprocessData(LPCTSTR lpDataString,
                              LPCTSTR lpEventName,
                              bool *pbAllowDefaultProcessing)
{
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
   if(sm_lpfnPreprocessData)
   {
	   nRetCode = (*sm_lpfnPreprocessData)(lpDataString, lpEventName, pbAllowDefaultProcessing);
   }

   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("PreprocessData() returns %s"), csRetCode);
   return nRetCode;

}


//+dp185016 
TBRC CTransBroker::SyncTBorPOS()
{
   Trace(L0,_T("+CTransBroker::SyncTBorPOS"));
   TBRC nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
   if(sm_lpfnSyncTBorPOS)//tar 288628
   {
      nRetCode = (*sm_lpfnSyncTBorPOS)();
   }
   CString csRetCode;
   ::GetTBReturnCodeName(nRetCode, csRetCode);
   Trace(L6, _T("SyncTBorPOS() returns %s"), csRetCode);
   Trace(L0,_T("-CTransBroker::SyncTBorPOS"));
   return nRetCode;

}
//-dp185016

//dp185016 tbpowershutdown added rfc 265463
TBRC CTransBroker::PowerShutdown()
{
  Trace(L0,_T("+CTransBroker::PowerShutdown"));
  TBRC nRetCode;

  if(sm_lpfnPowerShutdown)
		nRetCode = (*sm_lpfnPowerShutdown)();
  else
		nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
  CString csRetCode;
  ::GetTBReturnCodeName(nRetCode, csRetCode);

  Trace(L6, _T("PowerShutdown retCode:%s"), csRetCode);
  Trace(L0,_T("-CTransBroker::PowerShutdown"));
  return nRetCode;   
}

//+dp185016 support glory device
BOOL CTransBroker::ValidateHeadCashier( LPCTSTR szCashierID, LPCTSTR szCashierPw )
{
	Trace(L0,_T("+CTransBroker::ValidateHeadCashier"));
  
	BOOL bRetCode = FALSE;  //TAR 354815

	if(sm_lpfnValidateHeadCashier)
	{
		bRetCode = (*sm_lpfnValidateHeadCashier)(szCashierID, szCashierPw);
	}	

	Trace(L0,_T("-CTransBroker::ValidateHeadCashier"));
	
	return bRetCode;
}
//-dp185016

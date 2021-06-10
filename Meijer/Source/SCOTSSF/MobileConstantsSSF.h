// MobileConstantsSSF.h.
// ADK
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOBILECONSTANTSSSF_H__INCLUDED_)
#define AFX_MOBILECONSTANTSSSF_H__INCLUDED_


#define MOBILE_START _T("START_MOBILE")
#define MOBILE_RESCAN_NORESCAN 0
#define MOBILE_RESCANITEM_POSANDMYSCAN 0
#define MOBILE_RESCANITEM_POSONLY 1
#define MOBILE_RESCANITEM_MYSCANONLY 2

#define MOBILE_ORIGIN_TERMINAL_RESCAN 11

// used in ItemProperty for UploadTicket
#define MOBILE_ITEMPROP_NORMAL 0
#define MOBILE_ITEMPROP_VOIDED 1
#define MOBILE_ITEMPROP_NOTSCANNABLE 2
#define MOBILE_ITEMPROP_ADDITIONALSALE 3
#define MOBILE_ITEMPROP_MYSCAN_MARKDOWN 9

//Transaction Statuses
const TCHAR szTSAttract[]                       = _T("ATTRACT");
const TCHAR szTSMobileReplayStarted[]           = _T("MOBILE_REPLAY_STARTED");
const TCHAR szTSMobileReplayEnded[]             = _T("MOBILE_REPLAY_ENDED");
const TCHAR szTSItemization[]                   = _T("ITEMIZATION");
const TCHAR szTSTransactionComplete[]           = _T("TRANSACTION_COMPLETE");
const TCHAR szTSTenderStart[]                   = _T("TENDER_COMPLETE");
const TCHAR szTSTransactionContinue[]           = _T("TRANSACTION_CONTINUE");
const TCHAR szTSMobileItemSaleStarted[]         = _T("MOBILE_ITEM_SALE_STARTED");
const TCHAR szTSItemSaleStarted[]               = _T("ITEM_SALE_STARTED");
const TCHAR szTSItemSold[]                      = _T("ITEM_SOLD");
const TCHAR szTSCustomerMode[]                  = _T("CUSTOMER_MODE");
const TCHAR szTSStoreMode[]                     = _T("STORE_MODE");
const TCHAR szTSHelpMode[]                      = _T("HELP_MODE");             
const TCHAR szTSItemVoided[]                    = _T("ITEM_VOIDED"); 
const TCHAR szTSItemCancelled[]                 = _T("CANCEL_ITEM");
const TCHAR szTSBalancePaid[]                   = _T("BALANCE_PAID");

//Interventions - name of MobileCartAudit.ini section name for intervention
const TCHAR szMobileCartTransferFailed[]          = _T("MobileTransferFailed");
const TCHAR szMobileCartUploadFailed[]            = _T("MobileUploadFailed");


#endif

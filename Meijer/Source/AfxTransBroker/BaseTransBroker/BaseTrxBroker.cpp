//*************************************************************************************
//  BaseTrxBroker.cpp   :   Definition file for the CBaseTrxBroker class
//*************************************************************************************

#include "stdafx.h"

#define CUSTOM_TBSTATES
#include "BaseTBSharedDefs.h"

#define BASE_TRX_BROKER
#include "BaseTrxBroker.h"
#include "traceSystem.h"
#include "SCOTEventCat.h"
#include "TBUtilities.h"
#include "BaseTBDefs.h"
#include "BaseObjectManager.h"
#include "BaseTBConnectPOS.h"
#include "BaseTBConnectFL.h"
#include "BaseMHTransaction.h"
#include "BaseMHItem.h"
#include "BaseMHTender.h"
#include "BaseMHTransaction.h"
#include "BaseMHDisplay.h"
#include "BaseMHJournal.h"
#include "BaseMHReceipt.h"
#include "BaseMHSlip.h"
#include "BaseMHOptions.h"
#include "BaseMHTransaction.h"
#include "BaseMHTotals.h"
#include "BaseMHKeyboard.h"
#include "BaseMHOperator.h"
#include "BaseMHKeylock.h"
#include "BaseMHPINPad.h"
#include "BaseMHSigCap.h"
#include "BaseMHPrinter.h"
#include "BaseMHMicr.h"
#include "BaseMHCashDrawer.h"
#include "BaseMHFiscalPrinter.h"
#include "returnStateMsg.h"
#include "BaseObjectManager.h"
#include "customerTBSharedDefs.h"
#include "pinpadmsg.h"
#include "displaymsg.h"
#include "TBGenMsg.h"
#include "customerTBProperties.h"
#include "customerMHGenMsg.h"
#include "customerMsgMap.h"

#define T_ID    _T("CBaseTrxBroker")

const CString START_PIPEEXE(_T("c:\\scot\\config\\PreSCOTStart.bat"));
const CString STOP_PIPEEXE(_T("c:\\scot\\config\\PostSCOTStop.bat")); 

CArray<CBaseSingleInstance*, CBaseSingleInstance*> CBaseSingleInstance::m_aSingletonArray;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBaseTrxBroker::CBaseTrxBroker()
:m_bStoreMode(false)
{
    m_bTBInit              = false;
    m_bPipeServerStarted   = false;
    m_bPreScotStart_Active = false;
    m_bInAssistMode      = FALSE;
    
    CCustomerMHOptions* pMHOptions = CCustomerMHOptions::instance();
    pMHOptions->Initialize();
    ptbto = new CTraceSystemMgr(pMHOptions->OptionValue(_T("TBTraceFileName")));  // Instance

    STARTtbtrace();   //initialize trace object for this thread
    
}

CBaseTrxBroker::~CBaseTrxBroker()
{  
    ENDtbtrace();
    delete ptbto;
    ptbto=NULL;
}

/*************************************************************************
* launchPreScotStart - this is only used in _DEBUG env's. Checks option value
*                   and status flag before launching the bat file. To preserve
*                   backwards compatability, this then calls startPipeServer.
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseTrxBroker::launchPreScotStart(void)
{
#ifdef _DEBUG
    tbtraceIO(_T("launchPreScotStart()"));
    CCustomerMHOptions *pCurrOpts = CCustomerMHOptions::instance();
    
    // if we haven't started the bat file, or the we can restart it with ever
    // stopping anything, then go ahead and launch the bat file.
    if ((!m_bPreScotStart_Active)  || 
        (!pCurrOpts->BoolOptionValue(_T("PreScotStartDependsOnPostScotStop"))) )
    {
        tbtrace(TM_INFO, _T("Calling START_PIPEEXE"));
        LaunchApp(START_PIPEEXE);
        m_bPreScotStart_Active = true;
    }

    if (pCurrOpts->BoolOptionValue(_T("TBUsesPipeServer")))
        startPipeServer();
#endif
}

/*************************************************************************
* launchPostScotStop - this is only used in _DEBUG env's. Checks option value
*                   and status flag before launching the bat file. To preserve
*                   backwards compatability, this then calls stopPipeServer.
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseTrxBroker::launchPostScotStop(void)
{
#ifdef _DEBUG
    tbtraceIO(_T("launchPostScotStop()"));
    
    // if we had started the bat file, then we need to stop it now.
    if (m_bPreScotStart_Active) 
    {
        tbtrace(TM_INFO, _T("Calling STOP_PIPEEXE"));
        LaunchApp(STOP_PIPEEXE);
        m_bPreScotStart_Active = false;;
    }

    if (CCustomerMHOptions::instance()->BoolOptionValue(_T("TBUsesPipeServer")))
        stopPipeServer();
#endif
}

/*************************************************************************
* startPipeServer - this doesn't really start pipeserver as that should
*                   be located in the prescotstart bat file, but to preserver
*                   backwards compatibility, the name remains. This looks for
*                   the pipeserver window to ensure it gets started correctly.
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseTrxBroker::startPipeServer(void) 
{
#ifdef _DEBUG
    tbtraceIO(_T("startPipeServer()"));

    if (!m_bPipeServerStarted)
    {
        // Loop for up to 5 seconds looking for PipeServer
        HWND hwnd = FindWindow(NULL, _T("pipeServer"));
        int nCnt = 0;
        while ((hwnd == NULL) && (nCnt++ < 50)) 
        {
            Sleep(100);   
            hwnd = FindWindow(NULL, _T("pipeServer"));
        }
        
        if (hwnd == NULL)
            tbtrace(TM_WARNING, _T("Pipeserver window handle not found!"));
        else
        {
            tbtrace(TM_INFO, _T("Pipeserver window located, started correctly"));
            m_bPipeServerStarted = true;
        }
    }
#endif
}

/*************************************************************************
* stopPipeServer - this only is called when debug is run. This function does
*                  not actually stop anything, but rather just checks for the
*                  pipeserver window. The function name stays to preserver
*                  backwards compatibility.
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseTrxBroker::stopPipeServer(void)
{
#ifdef _DEBUG
    tbtraceIO(_T("stopPipeServer()"));
    
    //  wait until pipeServer is killed - up to 5 seconds
    HWND hwnd = FindWindow(NULL, _T("pipeServer"));
    int nCnt = 0;
    while (hwnd && (nCnt++ < 50)) 
    {
        Sleep(100);   
        hwnd = FindWindow(NULL, _T("pipeServer"));  // is it still alive?
    }

    if (hwnd != NULL)
    {
        tbtrace(TM_WARNING, _T("Pipeserver not shut down in time, resetting flag anyway"));
    }
    
    m_bPipeServerStarted = false;
#endif
}

// EventLogging not supported in Release 4.0 x of FastLane.  Re-enable for use with earlier release.
//void CBaseTrxBroker::logEvent(EventClassEnum eventClass, EventTypeEnum eventType,
//                              WORD eventCategory, ULONG eventID, WORD stringCount, ...)
//{
//}


/*************************************************************************
**  TBAPIS - All the exposed interface points with Scotapp
*************************************************************************/


/*************************************************************************
* TBInitialize - This is not necessarily the first API called by SCOT! Used
*                to initialize the connection points to POS and SCOT.
*
* Parameters:
*  HWND hWnd - handle to the SCOT application window
*  LPCTSTR szFilePath - path for config files
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBInitialize(HWND hWnd, LPCTSTR szFilePath)
{
    tbtraceIO(_T("TBInitialize()"));
    TBRC rc = TB_SUCCESS;
    
    launchPreScotStart();
    
    CCustomerTBScotMsg::instance()->SetScotWndHandle(hWnd); 
    if (!m_bTBInit) 
    {
        m_bTBInit = true;
    }
    
    m_pConnectPOS= CCustomerTBConnectPOS::instance();
    m_pConnectPOS->InitEmulation();
    
    CCustomerMsgMap::instance()->Initialize();
    
    return rc;
}

/*************************************************************************
* TBUninitialize - This will be the last API called as Fastlane shuts down
*
* Parameters: none
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBUninitialize(void)
{
    tbtraceIO(_T("TBUninitialize()"));
    TBRC rc = TB_UNINIT_CLEANUP;
    
    m_pConnectPOS= CCustomerTBConnectPOS::instance();
    m_pConnectPOS->Shutdown();
    
    launchPostScotStop();
    
    // clean up the singleton objects that have the auto cleanup flag set.  
    CBaseSingleInstance::CleanupAll();
    m_bTBInit = false;
    
    return rc;
}


/*************************************************************************
* TBCancelTransaction - Fastlane needs to cancel the transaction
*
* Parameters: 
*  BOOL fOverride - use override if needed (TRUE/FALSE)
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBCancelTransaction(const BOOL fOverride)
{
    tbtraceIO(_T("TBCancelTransaction()"));
    CBaseMHTransaction* pMHTrx = CCustomerMHTransaction::instance();
    return pMHTrx->CancelTransaction(fOverride);
}

/*************************************************************************
* TBCopyFileFromServer - Fastlane requests TB copy a file from the server
*                        to the Fastlane client terminal.
*
* Parameters: 
*  LPCTSTR szFileName - file name of file to be copied
*  LPCTSTR szDestinationFilePath - complete path and filename of destination
*  BOOL fForce - TRUE = force copy, FALSE = copy only if date or size diff
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBCopyFileFromServer(LPCTSTR szFileName, 
                                          LPCTSTR szDestinationFilePath, 
                                          const BOOL fForce)
{
    tbtrace(TM_INFO, _T("TBCopyFileFromServer()"));
    CBaseTBConnectPOS* pTBConnectPOS = CCustomerTBConnectPOS::instance();
    return pTBConnectPOS->CopyFileFromServer(szFileName, szDestinationFilePath, fForce);
}

/*************************************************************************
* TBCopyFileToServer - Fastlane requests TB copy a file from the client
*                      machine to the server.
*
* Parameters: 
*  LPCTSTR szFileName - complete filename (with path) of file to be copied
*  LPCTSTR szSourceFilePath - complete path and filename of source location
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBCopyFileToServer(LPCTSTR szFileName, LPCTSTR szSourceFilePath)
{
    tbtrace(TM_INFO, _T("TBCopyFileToServer()"));
    CBaseTBConnectPOS* pTBConnectPOS = CCustomerTBConnectPOS::instance();
    return pTBConnectPOS->CopyFileToServer(szFileName, szSourceFilePath);
}

/*************************************************************************
* TBEnterDOB - Inform host application the Customer’s Date of Birth if needed
*
* Parameters: 
*  int nYear - valid year of customer DOB
*  int nMonth - valid month of customer DOB
*  int nDay - valid day of customer DOB
*  int nCustomerAge - calculated customer age
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBEnterDOB(int nYear, int nMonth, int nDay, int nCustomerAge)
{
    tbtraceIO(_T("TBEnterDOB()"));
    CBaseMHItem* pMHItem = CCustomerMHItem::instance();
    return pMHItem->EnterDOB(nYear, nMonth, nDay, nCustomerAge);
}

/*************************************************************************
* TBSendWeight - Inform host application of the weight currently on scale.
*                This is only called while in assist mode when FL detects
*                weight on the scale and the last return was TB_NEEDWEIGHT
*
* Parameters: 
*  long lWeight - the weight on the scale
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBSendWeight(long lWeight)
{
    tbtraceIO(_T("TBSendWeight()"));
    CBaseMHItem* pMHItem = CCustomerMHItem::instance();
    return pMHItem->SendWeight(lWeight);
}

/*************************************************************************
* TBEnterItem - Called by FL to sell or void an item.
*
* Parameters: 
*  LPCTSTR szItemCode - item code as keyed in or extracted from barcode if 
*                       items is scanned
*  BOOL fVoid - TRUE:void item, FALSE:sell item
*  BOOL fOverride - TRUE: use override if needed; FALSE: no override
*  long nQuantity - Quantity of the items to be entered into the transaction. 
*                   For non-quantity-required items, use the value 0.
*  long nWeight - Weight associated with each item
*  long nTareCode - Tare code associated with each item. TB calculates the 
*                   weight associated with this tare code automatically.
*  long lPrice - Price of each item.
*  BOOL fQuantityConfirmed - Used if nQuantity is above confirmation limit.
*  PSEnterItemDetails psMoreDetails - structure containing lUnitPrice, scan type
*                                     and multi-label barcode data
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBEnterItem(LPCTSTR szItemCode, const BOOL fVoid, const BOOL fOverride, 
                                 const long nQuantity, const long nWeight, const long nTareCode, 
                                 const long lPrice, const BOOL fQuantityConfirmed, 
                                 PSEnterItemDetails psMoreDetails)
{
    tbtraceIO(_T("TBEnterItem()"));
    CBaseMHItem* pMHItem = CCustomerMHItem::instance();
    return pMHItem->EnterItem(szItemCode, fVoid, fOverride, nQuantity, nWeight, nTareCode,
        lPrice, fQuantityConfirmed, psMoreDetails);
}

/*************************************************************************
* TBEnterTender - Called by FL to enter or void a tender. For additional info
*                 about PIN, see ADK on SPINWorkingKey and TBPINENCODING
*
* Parameters: 
*  TBTENDERTYPE nTenderType - tender type from enum (TB_TENDER_CASH, etc)
*  LONG lAmount - Amount of tender
*  LPCTSTR szCardData - Card track data - read ADK for exact format
*  int nPINLength - Length of the encrypted PIN block. 
*  LPCTSTR szEncryptedPIN - Encrypted PIN block.
*  BOOL fOverride - TRUE: use override if needed; FALSE: no override
*  BOOL fVoid - TRUE: void tender; FALSE: enter tender
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBEnterTender(TBTENDERTYPE nTenderType, const LONG lAmount, 
                                   LPCTSTR szCardData, const int nPINLength, LPCTSTR szEncryptedPIN,
                                   const BOOL fOverride, const BOOL fVoid)
{
    tbtraceIO(_T("TBEnterTender()"));
    CBaseMHTender* pMHTender = CCustomerMHTender::instance();
    return pMHTender->EnterTender(nTenderType, lAmount, szCardData, nPINLength,
        szEncryptedPIN, fOverride, fVoid);
}

/*************************************************************************
* TBEPSStartOfDay - FL requests TB perform a Start of Day - mostly used in
*                   IBM TB's.
*
* Parameters: 
*  LPVOID lp - Data, the actual type of data depends on the type of implementation
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBEPSStartOfDay(LPVOID lp)
{
    tbtraceIO(_T("TBEPSStartOfDay()"));
    CBaseMHTender* pMHTender = CCustomerMHTender::instance();
    return pMHTender->EPSStartOfDay(lp);
}

/*************************************************************************
* TBEPSEndOfDay - FL requests TB perform an End of Day - mostly used in
*                 IBM TB's.
*
* Parameters: 
*  LPVOID lp - Data, the actual type of data depends on the type of implementation
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBEPSEndOfDay(LPVOID lp)
{
    tbtraceIO(_T("TBEPSEndOfDay()"));
    CBaseMHTender* pMHTender = CCustomerMHTender::instance();
    return pMHTender->EPSEndOfDay(lp);
}

/*************************************************************************
* TBFinish - FL requests TB end the current sales session. TB_TRXEND state 
*            returned by TB tells FastLane to call this function.
*
* Parameters: none
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBFinish(void)
{
    tbtraceIO(_T("TBFinish()"));
    CBaseMHTransaction* pMHTrx = CCustomerMHTransaction::instance();
    return pMHTrx->Finish();
}

/*************************************************************************
* TBGet2x20 - Get 2x20 display message. This was originally used by FL to 
*             get the assist mode display when IBM only had a 2x20 display.
*             This is either not used or hacked in most non-IBM TB's.
*
* Parameters: 
*  LPCTSTR szDisplayStr - populate this with the current 2x20 display
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGet2x20(LPCTSTR szDisplayStr)
{
    tbtraceIO(_T("TBGet2x20()"));
    CBaseMHDisplay* pMHDisplay = CCustomerMHDisplay::instance();
    return pMHDisplay->Get2x20(szDisplayStr);
}

/*************************************************************************
* TBGetItemDetails - FL requests TB to populate a structure with pertinent
*                    item details. This will be called with FL receives
*                    TB_ITEMSOLD, or any other item sale related state event
*                    (TB_NEEDQUANTITY, TB_NEEDWEIGHT, etc)
*
* Parameters: 
*  PSItemDetails psItemDetails - populate this struct with the item details
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetItemDetails(PSItemDetails psItemDetails)
{
    tbtrace(TM_ENTRY, _T("TBGetItemDetails()"));
    CBaseMHItem* pMHItem = CCustomerMHItem::instance();
    return pMHItem->GetItemDetails(psItemDetails);
}

/*************************************************************************
* TBGetItemExtendedDetails - Get extended detailed information on the last 
*             item in the transaction. After confirming that the TB supports 
*             this function, with TBIsAllowed, the application should call 
*             this API when it receives TB_ITEMSOLD or any other item sale 
*             related state event from TB.
*
* Parameters: 
*  PSItemExtendedDetails psItemExtendedDetails - populate this struct with the details
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetItemExtendedDetails(PSItemExtendedDetails psItemExtendedDetails)
{
    tbtraceIO(_T("TBGetItemExtendedDetails()"));
    CBaseMHItem* pMHItem = CCustomerMHItem::instance();
    return pMHItem->GetItemExtendedDetails(psItemExtendedDetails);
}

/*************************************************************************
* TBGetTransactionLimit - A way to put limit on FastLane transaction total. 
*              If TB does not want to put a limit on the transaction total, 
*              it can return a very high value (Base TB returns 999999999)
*              FastLane App calls this function to display the transaction 
*              limit on the Attract screen.
*
* Parameters: none
*
* Returns: long - value of transaction limit
*************************************************************************/
long CBaseTrxBroker::TBGetTransactionLimit(void)
{
    tbtraceIO(_T("TBGetTransactionLimit()"));
    CBaseMHTransaction* pMHTrx = CCustomerMHTransaction::instance();
    return pMHTrx->GetTransactionLimit();
}

/*************************************************************************
* TBGetQuantityLimit - Get the quantity threshold per transaction. This is 
*      a threshold that causes the system to ask the user for confirmation. 
*
* Parameters: none
*
* Returns: long - quantity threshold for user confirmation
*************************************************************************/
long CBaseTrxBroker::TBGetQuantityLimit(void)
{
    tbtraceIO(_T("TBGetQuantityLimit()"));
    CBaseMHTransaction* pMHTrx = CCustomerMHTransaction::instance();
    return pMHTrx->GetQuantityLimit();
}

/*************************************************************************
* TBGetJournalData - Return the file handle of a file that contains journal 
*        presentation data. If TB wants to print some transaction data on 
*        the journal printer attached to the FastLane terminal, this 
*        function should return a valid handle to a file that contains journal 
*        data. The data is transparent to FastLane and is submitted to the 
*        journal without any modifications. The journal is printed either on 
*        request by TB (synchronous print request: TB_COMPLETEPRINT state) 
*        or at the end of each transaction.
*
* Parameters: none
*
* Returns: HFILE  - handle of a file (NULL = no journal data)
*************************************************************************/
HFILE CBaseTrxBroker::TBGetJournalData(void)
{
    tbtraceIO(_T("TBGetJournalData()"));
    CBaseMHJournal* pMHJournal = CCustomerMHJournal::instance();
    return pMHJournal->GetJournalData();
}

/*************************************************************************
* TBReleaseJournalData - Release the presentation data for the journal printer.
*
* Parameters: None
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBReleaseJournalData(HFILE hJournalData)
{
    tbtraceIO(_T("TBReleaseJournalData()"));
    CBaseMHJournal* pMHJournal = CCustomerMHJournal::instance();
    return pMHJournal->ReleaseJournalData(hJournalData);
}

/*************************************************************************
* TBGetReceiptData - Return the file handle of a file that contains receipt 
*        presentation data. If TB wants to print some transaction data on 
*        receipt printer attached to FastLane terminal, this function should 
*        return a valid handle to a file that contains receipt data. The 
*        data is transparent to FastLane and is submitted to receipt printer 
*        without any modifications. Receipt is printed either on request by 
*        TB (synchronous print request: TB_COMPLETEPRINT state) or at the 
*        end of each transaction.
*
* Parameters: none
*
* Returns: HFILE  - handle of a customer receipt file (NULL = no receipt data)
*************************************************************************/
HFILE CBaseTrxBroker::TBGetReceiptData(void)
{
    tbtraceIO(_T("TBGetReceiptData()"));
    CBaseMHReceipt* pMHReceipt = CCustomerMHReceipt::instance();
    return pMHReceipt->GetReceiptData();
}

/*************************************************************************
* TBReleaseReceiptData - Free the presentation data for the customer receipt.
*
* Parameters: None
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBReleaseReceiptData(HFILE hReceiptData)
{
    tbtraceIO(_T("TBReleaseReceiptData()"));
    CBaseMHReceipt* pMHReceipt = CCustomerMHReceipt::instance();
    return pMHReceipt->ReleaseReceiptData(hReceiptData);
}

/*************************************************************************
* TBGetSlipData - Return the file handle of a file that contains signature 
*        slip presentation data.
*
* Parameters: none
*
* Returns: HFILE  - handle of signature slip presentation fata (NULL = no data)
*************************************************************************/
HFILE CBaseTrxBroker::TBGetSlipData(void)
{
    tbtraceIO(_T("TBGetSlipData()"));
    CBaseMHSlip* pMHSlip = CCustomerMHSlip::instance();
    return pMHSlip->GetSlipData();
}

/*************************************************************************
* TBReleaseSlipData - Free the presentation data for the signature slip.
*
* Parameters: None
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBReleaseSlipData(HFILE hSlipData)
{
    tbtraceIO(_T("TBReleaseSlipData()"));
    CBaseMHSlip* pMHSlip = CCustomerMHSlip::instance();
    return pMHSlip->ReleaseSlipData(hSlipData);
}

/*************************************************************************
* TBGetPINWorkingKey - Get the PIN working key from the Host Credit module.
*       FastLane Application creates a pin-block using this information and
*       using the pin-block for PIN encryption/decryption.
*
* Parameters:
*  PSPINWorkingKey psPINWorkingKey - Pointer to the SPINWorkingKey data structure.
*       The application allocates memory for this structure. The TB function 
*       fills in the parameter
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetPINWorkingKey(PSPINWorkingKey psPINWorkingKey)
{
    tbtraceIO(_T("TBGetPINWorkingKey()"));
    CBaseMHPINPad* pMHPINPad = CCustomerMHPINPad::instance();
    return pMHPINPad->GetPINWorkingKey(psPINWorkingKey);
}

/*************************************************************************
* TBGetTenderDetails - Get detailed information on the last tender in the 
*                      transaction. The last tender can be accepted or declined 
*                      (TB_TENDERACCEPTED or TB_TENDERDECLINED).
*
* Parameters:
*  PSTenderDetails psTenderDetails - Pointer to the STenderDetails data structure
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetTenderDetails(PSTenderDetails psTenderDetails)
{
    tbtrace(TM_ENTRY, _T("TBGetTenderDetails()"));
    CBaseMHTender* pMHTender = CCustomerMHTender::instance();
    return pMHTender->GetTenderDetails(psTenderDetails);
}

/*************************************************************************
* TBGetTerminalNumber - Return POS client terminal number. FastLane assumes 
*                       positive terminal numbers only. Permits negative
*                       numbers to be used to return error status.
*
* Parameters: none
*
* Returns: long - terminal number
*************************************************************************/
long CBaseTrxBroker::TBGetTerminalNumber(void)
{
    tbtraceIO(_T("TBGetTerminalNumber()"));
    CBaseMHOptions* pMHOptions = CCustomerMHOptions::instance();
    return pMHOptions->GetTerminalNumber();
}

/*************************************************************************
* TBGetCurrentTransactionNumber - The long value returned should be the 
*            transaction number as determined by the POS. If the function 
*            is not implemented inside of the transaction broker, then the 
*            number -9999 is used for the transaction number. This number is 
*            used solely for reporting purposes. The transaction number is 
*            recorded in the reports. Base TB sets this to 0 and does not
*            increment it!
*
* Parameters: none
*
* Returns: long - terminal number
*************************************************************************/
long CBaseTrxBroker::TBGetCurrentTransactionNumber(void)
{
    tbtraceIO(_T("TBGetCurrentTransactionNumber()"));
    CBaseMHTransaction* pMHTrx = CCustomerMHTransaction::instance();
    return pMHTrx->GetCurrentTransactionNumber();
}

/*************************************************************************
* TBGetTotalDetails - Get detailed information on the total in the transaction. 
*                     FastLane calls this function after receiving TB_ITEMSOLD 
*                     or TB_TOTAL state event.
*
* Parameters:
*  PSTotalDetails psTotalDetails - Pointer to the STotalDetails data structure
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetTotalDetails(PSTotalDetails psTotalDetails)
{
    tbtrace(TM_ENTRY, _T("TBGetTotalDetails()"));
    CBaseMHTotals* pMHTotals = CCustomerMHTotals::instance();
    return pMHTotals->GetTotalDetails(psTotalDetails);
}

/*************************************************************************
* TBParseFoodStampTotal - Function to extract the Food Stamp Total of the purchase.
*         At the end of a transaction, get the total that can be paid by food 
*         stamps. First call TBRequestFoodStampEligibleTotal() which asks the host 
*         system to send that the amount paid using food stamps. This function 
*         TBParseFoodStampTotal() is then used to parse or extract the amount due.
*
* Parameters:
*  PSTotalDetails psTotalDetails - A PSTotalDetails structure, one element of
*                                  which holds the food stamp total
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBParseFoodStampTotal(PSTotalDetails psTotalDetails)
{
    tbtraceIO(_T("TBParseFoodStampTotal()"));
    CBaseMHTotals* pMHTotals = CCustomerMHTotals::instance();
    return pMHTotals->ParseFoodStampTotal(psTotalDetails);
}

/*************************************************************************
* TBGetVersion - Return the version number and information of the TB.
*                Version string conforms to the below format:
*                  "FastLane TB Ver xx.xx.xx for Solution Name"
*                This should be set using the CBaseCurrentOptions::SetVersionString
*                function. Base TB does not currently set this.
*
* Parameters: 
*  LPTSTR szVersionString - Buffer to contain the version string
*  long nBufferLength - Buffer length
*
* Returns: long - Length of the actual version string, including the NULL 
*                 character. If this is bigger than nBufferLength then the 
*                 buffer passed is not long enough to copy the entire version 
*                 string, but nBufferLength characters are still copied into 
*                 the buffer.
*************************************************************************/
long CBaseTrxBroker::TBGetVersion(LPTSTR szVersionString, long nBufferLength)
{
    tbtraceIO(_T("TBGetVersion()"));
    CBaseMHOptions* pMHOptions = CCustomerMHOptions::instance();
    return pMHOptions->GetVersion(szVersionString, nBufferLength);
}

/*************************************************************************
* TBIsAllowed - The application can use this function to query TB to see 
*       whether or not a certain feature is allowable in the current state.
*
* Parameters: 
*  TBFEATURE nFeature - feature (TBFEATURE enum - ex: TB_FT_TRAININGMODE)
*
* Returns: BOOL - TRUE:permitted; FALSE:not permitted
*************************************************************************/
BOOL CBaseTrxBroker::TBIsAllowed(TBFEATURE nFeature)
{
    tbtrace(TM_ENTRY, _T("TBIsAllowed()"));
    CBaseMHOptions* pMHOptions = CCustomerMHOptions::instance();
    return pMHOptions->IsAllowed(nFeature);
}

/*************************************************************************
* TBKeyPress - Send a key code. This is very useful in supporting the full 
*              Store Mode POS keyboard that mimics the host application’s 
*              functionality. This function is mainly used in the Assist Mode.
*
* Parameters: 
*  USHORT uKey - ASCII key value
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBKeyPress(const USHORT uKey)
{
    tbtraceIO(_T("TBKeyPress()"));
    CBaseMHKeyboard* pMHKeyboard = CCustomerMHKeyboard::instance();
    return pMHKeyboard->KeyPress(uKey);
}

/*************************************************************************
* TBLoadOptions - FastLane App calls this function immediately after calling 
*       TBInitialize() to request the host application to load its terminal 
*       options. The host application normally loads its terminal options at 
*       the start of the application. This function is provided so that while 
*       the application is still running, you can still choose to reload options.
*
* Parameters: none
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBLoadOptions(void)
{
    tbtraceIO(_T("TBLoadOptions()"));
    CBaseMHOptions* pMHOptions = CCustomerMHOptions::instance();
    return pMHOptions->Initialize();
}

/*************************************************************************
* TBLogErrorToServer - Log an error to the server. This feature may not be 
*       available in all solutions. If the solution does not support this 
*       feature, this function should always return TB_SUCCESS. 
*
* Parameters: 
*  LPCTSTR szErrorText - Full error text
*  int nSeverity - Error severity, 1-5 (with 1 being the highest)
*  int nComponentID - FastLane Component ID number
*  LPCTSTR szComponentID - Component ID string, TS, PS…
*  LPCTSTR szModuleID - Module ID string
*  int nModuleLineNumber - Module line number
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBLogErrorToServer(LPCTSTR szErrorText, const int nSeverity,
                                        const int nComponentID, LPCTSTR szComponentID,
                                        LPCTSTR szModuleID, const int nModuleLineNumber)
{
//    tbtraceIO(_T("TBLogErrorToServer()"));
    CBaseTBConnectPOS* pTBConnectPOS = CCustomerTBConnectPOS::instance();
    return pTBConnectPOS->LogErrorToServer(szErrorText, nSeverity, nComponentID,
        szComponentID, szModuleID, nModuleLineNumber);
}

/*************************************************************************
* TBRefreshState - Intended to refresh the current state of the host 
*       application. This generates a display notification event, at which 
*       time the application can call TBProcessMessage to determine the state 
*       of the host application. Base TB does not implement this yet!
*
* Parameters: None
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBRefreshState(void)
{
    tbtrace(TM_ENTRY, _T("+-TBRefreshState()"));
    return TB_SUCCESS;
}

/*************************************************************************
* TBResumeTransaction - Resume the transaction suspended. This is always
*       going to be solution specific, so no base implementation is done.
*
* Parameters:
*  LPCTSTR szSuspendID - Suspended transaction ID
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBResumeTransaction(LPCTSTR szSuspendID)
{
    tbtraceIO(_T("TBResumeTransaction()"));
    CBaseMHTransaction* pMHTrx = CCustomerMHTransaction::instance();
    return pMHTrx->ResumeTransaction(szSuspendID);
}

/*************************************************************************
* TBSendMsg2Host - If none of the existing TB APIs address your need to 
*       communicate with the host, use this function in SCOTSSF (FastLane 
*       Solution Specific Functionality module) implementation. By default, 
*       this function is not called by the FastLane application. This is 
*       not required if TB has an extended set of TB APIs exposed to use 
*       by SCOTSSF. This is not used in Base TB.
*
* Parameters:
*  LPCTSTR szSend - Message to send/write, NULL for DWO reads
*  int iMsgLen - message length
*  TBDEVICEID nDeviceClass - Device ID
*  UCHAR nFunctionCode - Function code
*  long  lOptFileHandle - Optional handle for file DWO file I/O operations
*  bool fOriginatePipeMsg - 
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBSendMsg2Host(LPCTSTR szSend, const int iMsgLen,
                                    const TBDEVICEID nDeviceClass, const UCHAR nFunctionCode,
                                    const long  lOptFileHandle, const bool fOriginatePipeMsg)
{
    tbtraceIO(_T("TBSendMsg2Host()"));
    CBaseTBConnectPOS* pTBConnectPOS = CCustomerTBConnectPOS::instance();
    return pTBConnectPOS->SendMsg2Host(szSend, iMsgLen, nDeviceClass, nFunctionCode,
        lOptFileHandle, fOriginatePipeMsg);
}

/*************************************************************************
* TBSetMonitorAll - TBAPI called to request TB track all display msgs
*
* Parameters: 
*  BOOL fMonitor - TRUE = entering assist mode, FALSE = exiting
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBSetMonitorAll(BOOL fMonitor)
{
    tbtrace(TM_INFO, _T("TBSetMonitorAll(%d)"), fMonitor);
    SetInAssistMode(fMonitor); // set member variable tracking assist mode status
    CCustomerMHDisplay::instance()->SetMonitorAll(fMonitor); // status also tracked here
    return TB_SUCCESS;
}


/*************************************************************************
* InAssistMode, SetInAssistMode - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
BOOL CBaseTrxBroker::InAssistMode( void)
{
    return m_bInAssistMode;
}

void CBaseTrxBroker::SetInAssistMode( BOOL b)
{
    m_bInAssistMode = b;
}

/*************************************************************************
* TBStart - TBAPI called to start a transaction. FL should call this API
*       before passing on any trx data. Violations of this agreement should
*       be opened as TAR's against FL. TB can return TB_SUCCESS_IMMEDIATE to
*       start a trx right away, or it can return TB_SUCESS which will cause 
*       FL to wait for a TB_READY TBSTATE.
*
* Parameters: 
*  LPCTSTR szOverrideCode - The 2 character string denoting the ISO639
*       language code for language that the customer has selected to use in 
*       the transaction. NULL if dual language is not supported.
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBStart(LPCTSTR szOverrideCode)
{
    tbtrace(TM_INFO, _T("TBStart()"));
    CBaseMHTransaction* pMHTrx = CCustomerMHTransaction::instance();
    return pMHTrx->Start(szOverrideCode);
}

/*************************************************************************
* TBSuspendTransaction - Suspend the current sales transaction.
*
* Parameters: 
*  BOOL fOverride - TRUE: use override if needed; FALSE: no override
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBSuspendTransaction(const BOOL fOverride)
{
    tbtraceIO(_T("TBSuspendTransaction()"));
    CBaseMHTransaction* pMHTrx = CCustomerMHTransaction::instance();
    return pMHTrx->SuspendTransaction(fOverride);
}

/*************************************************************************
* TBValidateOperator - If this is a valid Operator and PW then return TRUE.
*       Base TB does not have an implementation for this - solution specific.
*
* Parameters: 
*  LPCTSTR szOperatorID - Operator ID
*  LPCTSTR szOperatorPw - Operator password
*
* Returns: BOOL - TRUE if a valid cashier and PW, FALSE otherwise
*************************************************************************/
BOOL CBaseTrxBroker::TBValidateOperator(LPCTSTR szOperatorID, LPCTSTR szOperatorPw)
{
    tbtraceIO(_T("TBValidateOperator()"));
    CBaseMHOperator* pMHOperator = CCustomerMHOperator::instance();
	CString csPrefixBase64(_T("BASE64:"));
	
    CString csID(szOperatorID);
    CString csPW(szOperatorPw);
	
    if(csID.Find(csPrefixBase64) != -1)
    {
		//tbtrace(L0, _T("ValidateOperator encoded(%s,%s)"), szOperatorID,szOperatorPw);
		
        csID.Replace(csPrefixBase64,_T(""));
        csPW.Replace(csPrefixBase64,_T(""));
		
        csID = DecodeStringBase64(csID);
        csPW = DecodeStringBase64(csPW);
    }
    return pMHOperator->ValidateOperator(csID, csPW);
}

/*************************************************************************
* TBValidOperator - Validate Operator - not believed to be in use by FL
*
* Parameters: 
*  LPCTSTR szOperatorID - Operator ID
*
* Returns: BOOL - TRUE if a valid cashier, FALSE otherwise
*************************************************************************/
BOOL CBaseTrxBroker::TBValidOperator(LPCTSTR szOperatorID)
{
    tbtraceIO(_T("TBValidOperator()"));
    CBaseMHOperator* pMHOperator = CCustomerMHOperator::instance();
    return pMHOperator->ValidOperator(szOperatorID);
}

/*************************************************************************
* TBValidPassword - Validate Password - not believed to be in use by FL
*
* Parameters: 
*  LPCTSTR szOperatorPw - Operator PW
*
* Returns: BOOL - TRUE if a valid password, FALSE otherwise
*************************************************************************/
BOOL CBaseTrxBroker::TBValidPassword(LPCTSTR szOperatorPw)
{
    tbtraceIO(_T("TBValidPassword()"));
    CBaseMHOperator* pMHOperator = CCustomerMHOperator::instance();
    return pMHOperator->ValidPassword(szOperatorPw);
}

/*************************************************************************
* TBVerifyLoyaltyCard - Return true if the card data is of a loyalty 
*       type card. If TB returns LC_VALID, then the FL will go into a 
*       processing state, awaiting the TB_LOYALTYCARD or TB_LOYALTYINVALID
*       return.
*
* Parameters: 
*  LPCTSTR szCardData - Card information, in format L1L2L3T1T2T3, 
*       where L1, T1 are the track length and track data of track 1; L2, T2 are 
*       for track 2; and L3, T3 are for track 3. Track lengths are declared as short integers.
*  int nDataLen - data length
*
* Returns: TBLOYALTYCARDTYPE - - One of the following: LC_INVALID, LC_VALID, 
*       or LC_INSERTED_BEFORE
*************************************************************************/
TBLOYALTYCARDTYPE CBaseTrxBroker::TBVerifyLoyaltyCard(LPCTSTR szCardData, int nDataLen)
{
    tbtraceIO(_T("TBVerifyLoyaltyCard()"));
    CBaseMHTender* pMHTender = CCustomerMHTender::instance();
    return pMHTender->VerifyLoyaltyCard(szCardData, nDataLen);
}

/*************************************************************************
* TBGetAvailableFoodStampAmount - Function used to get the dollar amount of 
*       Food Stamps remaining on a Food Stamp card that could be used against 
*       a purchase once it has been determined that the balance on the card 
*       is insufficient to pay for all food-stamp-eligible items. After 
*       TB_TENDERFOODSTAMPINSUFFICIENT is received, this function is called 
*       to find how much balance is remaining on the card. The user is informed 
*       about insufficient funds and given a chance to cancel payment with 
*       the Food Stamp card.
*
* Parameters: 
*  PSEBTAvailBalance psEBTAvailBalance - Contains the current balance on that 
*       food stamp account
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetAvailableFoodStampAmount(PSEBTAvailBalance psEBTAvailBalance)
{
    tbtraceIO(_T("TBGetAvailableFoodStampAmount()"));
    CBaseMHTotals* pMHTotals = CCustomerMHTotals::instance();
    return pMHTotals->GetAvailableFoodStampAmount(psEBTAvailBalance);
}

/*************************************************************************
* TBGetAvailableBenefitAmount - Function used to get the dollar amount of 
*       Cash Benefit remaining on a Cash Benefit card that could be used 
*       against a purchase. After FastLane gets TB_TENDERCASHBENEFITINSUFFICIENT, 
*       the application calls this function to find how much is remaining 
*       on the card. The user is informed about insufficient funds and is given 
*       a chance to cancel the payment with the benefit card.
*
* Parameters: 
*  PSEBTAvailBalance psEBTAvailBalance
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetAvailableBenefitAmount(PSEBTAvailBalance psEBTAvailBalance)
{
    tbtraceIO(_T("TBGetAvailableBenefitAmount()"));
    CBaseMHTotals* pMHTotals = CCustomerMHTotals::instance();
    return pMHTotals->GetAvailableBenefitAmount(psEBTAvailBalance);
}

/*************************************************************************
* TBRequestFoodStampEligibleTotal - Function used to request that the host 
*       send the dollar amount that could be paid with Food Stamps. FL will
*       follow up with a call to TBParseFoodStampEligibleTotal().
*
* Parameters: none
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBRequestFoodStampEligibleTotal()
{
    tbtraceIO(_T("TBRequestFoodStampEligibleTotal()"));
    CBaseMHTotals* pMHTotals = CCustomerMHTotals::instance();
    return pMHTotals->RequestFoodStampEligibleTotal();
}

/*************************************************************************
* TBSendSignOn - FL requests an operator signon to the POS system. Not
*       used currently in base FL app.
*
* Parameters: none
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBSendSignOn(void)
{
    tbtraceIO(_T("TBSendSignOn()"));
    CBaseMHOperator* pMHOperator = CCustomerMHOperator::instance();
    return pMHOperator->SendSignOn();
}

/*************************************************************************
* TBSendSignOff - FL requests an operator signoff to the POS system. Not
*       used currently in base FL app.
*
* Parameters: none
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBSendSignOff(void)
{
    tbtraceIO(_T("TBSendSignOff()"));
    CBaseMHOperator* pMHOperator = CCustomerMHOperator::instance();
    return pMHOperator->SendSignOff();
}

/*************************************************************************
* TBSendLockOn - Do a manager key-lock on. Not currently used in core app
*
* Parameters: none
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBSendLockOn(void)
{
    tbtraceIO(_T("TBSendLockOn()"));
    CBaseMHKeylock* pMHKeylock = CCustomerMHKeylock::instance();
    return pMHKeylock->SendLockOn();
}

/*************************************************************************
* TBSendLockOff - Do a manager key-lock off. Not currently used in core app
*
* Parameters: none
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBSendLockOff(void)
{
    tbtraceIO(_T("TBSendLockOff()"));
    CBaseMHKeylock* pMHKeylock = CCustomerMHKeylock::instance();
    return pMHKeylock->SendLockOff();
}

/*************************************************************************
* TBInitHostAppExit - Initiate host application exit procedures or sequences.
*       This action is initiated by the FastLane user (can be in store mode). 
*       This API exists to help solution-specific exit procedures.
*
* Parameters: none
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBInitHostAppExit(void)
{
    tbtraceIO(_T("TBInitHostAppExit()"));
    CBaseTBConnectPOS* pTBConnectPOS = CCustomerTBConnectPOS::instance();
    return pTBConnectPOS->InitHostAppExit();
}

/*************************************************************************
* TBLaneClosed - Informs TB when FastLane enters or leaves Lane Closed screen.
*       Appropriate TB action can be solution-specific. This is not a request
*       from FL, but a statement of fact.
*
* Parameters: 
*  bool bEnter - Flag indicating whether entering (TRUE) or exiting (FALSE)
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBLaneClosed(const bool bEnter)
{
    tbtraceIO(_T("TBLaneClosed()"));
    CBaseMHTransaction* pMHTrx = CCustomerMHTransaction::instance();
    return pMHTrx->LaneClosed(bEnter);
}

/*************************************************************************
* TBEnterTenderState - Informs TB when FastLane enters or leaves the tender 
*       state. Appropriate TB action can be solution-specific. This is not 
*       a request from FL, but a statement of fact.
*
* Parameters: 
*  bool bEnter - Flag indicating whether entering (TRUE) or exiting (FALSE)
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBEnterTenderState(const bool bEnter)
{
    tbtraceIO(_T("TBEnterTenderState()"));
    CBaseMHTender* pMHTender = CCustomerMHTender::instance();
    return pMHTender->EnterTenderState(bEnter);
}

/*************************************************************************
* TBSendSigData - Sends captured signature data to TB if the SendSignatureDataToTB
*       option in SCOTOPT.DAT is set to Y and the SignatureCapture option is
*       set to Y. Data is in 5992 format.
*
* Parameters: 
*  LPCTSTR CustomerName - The customer’s name
*  LPCTSTR cSigData - The signature data in 5992 format
*  DWORD nLength - The length of the signature data
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBSendSigData(LPCTSTR CustomerName, LPCTSTR cSigData,  DWORD nLength)
{
    tbtraceIO(_T("TBSendSigData()"));
    CBaseMHSigCap* pMHSigCap = CCustomerMHSigCap::instance();
    return pMHSigCap->SendSigData(CustomerName, cSigData, nLength);
}

/*************************************************************************
* TBSetTrainingModeState - FastLane App calls this function in order to enter 
*       or leave training mode. After returning TB_SUCCESS to this call, TB 
*       should post a message to FastLane App to return either :
*       TB_ENTEREDTRAINING state if bTMState is true or
*       TB_LEFTTRAINING state if bTMState is false.
*
* Parameters: 
*  bool bTMState - true: enter training mode; false: exit training mode
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBSetTrainingModeState(const bool bTMState)
{
    tbtrace(TM_ENTRY, _T("TBSetTrainingModeState(%d)"), bTMState);
    CBaseMHTransaction* pMHTrx = CCustomerMHTransaction::instance();
    return pMHTrx->SetTrainingModeState(bTMState);
}

/*************************************************************************
* TBGetRewardLineDetails - When TB returns TB_REWARDLINE, FL calls this
*       function to collect the details of that reward line.
*
* Parameters: 
*  PSRewardLineDetails psRewardLineDetails - the reward line details struct
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetRewardLineDetails(PSRewardLineDetails psRewardLineDetails)
{
    tbtrace(TM_ENTRY, _T("TBGetRewardLineDetails()"));
    CBaseMHItem* pMHItem = CCustomerMHItem::instance();
    return pMHItem->GetRewardLineDetails(psRewardLineDetails);
}

/*************************************************************************
* TBPrint - Permits FastLane App to send print requests to TB when TB 
*       controls the printer
*
* Parameters: 
*  LPCTSTR szPrintFile - Fully qualified name of the file to be printed,
*       or NULL to tell TB to resume printing from the last error.
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBPrint(LPCTSTR szPrintFile)
{
    tbtraceIO(_T("TBPrint()"));
    CBaseMHPrinter* pMHPrinter = CCustomerMHPrinter::instance();
    return pMHPrinter->Print(szPrintFile);
}

/*************************************************************************
* TBSuspendTransactionEx - Suspend a sales transaction, and sends a list
*       of suspend reasons to TB
*
* Parameters: 
*  BOOL fOverride - TRUE: use override if needed; FALSE: no override
*  LPCTSTR szSuspendReasons - List of suspend reasons if any to be
*       appended to the receipt
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBSuspendTransactionEx(const BOOL fOverride, 
                                            LPCTSTR szSuspendReasons)
{
    tbtraceIO(_T("TBSuspendTransactionEx()"));
    CBaseMHTransaction* pMHTrx = CCustomerMHTransaction::instance();
    return pMHTrx->SuspendTransactionEx(fOverride, szSuspendReasons);
}

/*************************************************************************
* TBGetPrinterError - Gets descriptive Printer error message. The FL 
*       application calls this when it receives TB_PRINTER_VERIFY to get 
*       the message to be displayed for the current printer error
*
* Parameters: 
*  LPTSTR* psErrorMessage - Pointer to the buffer containing the message
*       string to be displayed
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetPrinterError(LPTSTR* psErrorMessage)
{
    tbtraceIO(_T("TBGetPrinterError()"));
    CBaseMHPrinter* pMHPrinter = CCustomerMHPrinter::instance();
    return pMHPrinter->GetPrinterError(psErrorMessage);
}

/*************************************************************************
* TBEnterMICR - This function is called by the FastLane App to report 
*       information retrieved from the MICR document. If MICR data could 
*       not be retrieved, the parameter szRawData and other string parameters 
*       can be an empty string and the value of the long parameters is zero.
*
* Parameters: 
*  LPCTSTR szRawData - The raw MICR data from the MICR reader device
*  LPCTSTR szTransitNumber - The bank’s transit (routing) number
*  LPCTSTR szAccountNumber - The customer’s account number
*  LPCTSTR szBankNumber - The bank’s ABA bank number
*  LPCTSTR szSerialNumber - The sequence (check) number of the check
*  LPCTSTR szEPC - The Extended Processing Code (EPC) on the check
*  long lType - The type of check:
*       1  personal check
*       2  Business or Commercial check
*       99 Unknown check type
*  long lCountryCode - The country code of the check:
*       1  United States check
*       2  Canadian check (Canada)
*       3  Mexican check (Mexico)
*       99 Unknown check origination
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBEnterMICR(LPCTSTR szRawData, LPCTSTR szTransitNumber,
                                 LPCTSTR szAccountNumber, LPCTSTR szBankNumber, LPCTSTR szSerialNumber, 
                                 LPCTSTR szEPC, long lType, long lCountryCode)
{
    tbtraceIO(_T("TBEnterMICR()"));
    CBaseMHMicr* pMHMicr = CCustomerMHMicr::instance();
    return pMHMicr->EnterMICR(szRawData, szTransitNumber, szAccountNumber, 
        szBankNumber, szSerialNumber, szEPC, lType, lCountryCode);
}

/*************************************************************************
* TBGetDocEndorsementInfo - This function is called by the FastLane application 
*       to retrieve information about the next document to be endorsed. This 
*       function must be called in response to a TB_ENDORSEDOCUMENT message
*       from TB; however, it may be called at anytime.
*
* Parameters: 
*  long *plNumRemaining - Number of endorsements remaining
*  LPTSTR *ppszDocDescription - Operator prompt or document description
*  LPTSTR *ppszDocFront - Text to print on front of the document
*  LPTSTR *ppszDocBack - Text to print on back of the document
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetDocEndorsementInfo(long *plNumRemaining, 
                                             LPTSTR *ppszDocDescription,
                                             LPTSTR *ppszDocFront, 
                                             LPTSTR *ppszDocBack)
{
    tbtraceIO(_T("TBGetDocEndorsementInfo()"));
    CBaseMHSlip* pMHSlip = CCustomerMHSlip::instance();
    return pMHSlip->GetDocEndorsementInfo(plNumRemaining, ppszDocDescription,
        ppszDocFront, ppszDocBack);
}

/*************************************************************************
* TBSetCashDrawerState - This is called by the FastLane App in response to 
*       the state TB_NEWCASHDRAWERSTATE or when a cash drawer state changes.
*
* Parameters: 
*  long lState - A bitmapped field. The drawer should be opened if the bit 
*       is set. The drawer is closed or not present when the bit is cleared.
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBSetCashDrawerState(long lState)
{
    tbtraceIO(_T("TBSetCashDrawerState()"));
    CCustomerCurrentCashDrawer* pCurrentCD = CCustomerCurrentCashDrawer::instance();
    return pCurrentCD->SetCashDrawerState(lState);
}

/*************************************************************************
* TBGetExpectedCashDrawerState - This function is called when the FastLane 
*       application needs identity of the state that TB expects the cash 
*       drawer to be. It can be called by the FastLane application at any 
*       time after TB is initialized.
*
* Parameters: 
*  long *plExpectedState - A pointer to a bitmapped field. The drawer
*       should be opened if the bit is set. The drawer is closed or not 
*       present when the bit is cleared.
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetExpectedCashDrawerState(long *plExpectedState)
{
    tbtraceIO(_T("TBGetExpectedCashDrawerState()"));
    CCustomerCurrentCashDrawer* pCurrentCD = CCustomerCurrentCashDrawer::instance();
    return pCurrentCD->GetExpectedCashDrawerState(plExpectedState);
}

/*************************************************************************
* TBSetSecurityLevel - Called by the FastLane App when the security level 
*       of the operator changes.
*
* Parameters: 
*  long lSecurityLevel - new security level from TBKEYLOCK enum
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBSetSecurityLevel(long lSecurityLevel)
{
    tbtraceIO(_T("TBSetSecurityLevel()"));
    CBaseMHKeylock* pMHKeylock = CCustomerMHKeylock::instance();
    return pMHKeylock->SetSecurityLevel(lSecurityLevel);
}

/*************************************************************************
* TBGetExpectedSecurityLevel - Called by the FastLane application so that 
*       it can learn what security level the TB currently wants to see.
*
* Parameters: 
*  long *plExpectedLevel - security level from TBKEYLOCK enum
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetExpectedSecurityLevel(long *plExpectedLevel)
{
    tbtraceIO(_T("TBGetExpectedSecurityLevel()"));
    CBaseMHKeylock* pMHKeylock = CCustomerMHKeylock::instance();
    return pMHKeylock->GetExpectedSecurityLevel(plExpectedLevel);
}

/*************************************************************************
* TBGetCurrentSecurityLevel - Called by the FastLane App so that the App can 
*       determine the TB’s current security level.
*
* Parameters: 
*  long *plExpectedLevel - security level from TBKEYLOCK enum
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetCurrentSecurityLevel(long *plCurrentLevel)
{
    tbtraceIO(_T("TBGetCurrentSecurityLevel()"));
    CBaseMHKeylock* pMHKeylock = CCustomerMHKeylock::instance();
    return pMHKeylock->GetCurrentSecurityLevel(plCurrentLevel);
}

/*************************************************************************
* TBEndorsementCompleted - This function is called by the FastLane application 
*       for each endorsement that has been printed. FastLane calls this API
*       once for each TB_ENDORSEDOCUMENT state returned to it.
*
* Parameters: 
*  bool bSuppressSubsequent - If the bSuppressSubsequent flag is set, TB 
*       must not send further TB_ENDORSEDOCUMENT states to FastLane for 
*       any endorsements currently pending.
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBEndorsementCompleted(bool bSuppressSubsequent)
{
    tbtraceIO(_T("TBEndorsementCompleted()"));
    CBaseMHSlip* pMHSlip = CCustomerMHSlip::instance();
    return pMHSlip->EndorsementCompleted(bSuppressSubsequent);
}

/*************************************************************************
* TBGetAssistModeStatus - FL calls this function to determine if it is allowed
*       to enter or exit assist mode. Currently, FL does not call this to see
*       when it can enter AM, but it is called to see if it can exit. Base
*       TB sets this to TB_AMSTATUSMAYEXIT and does not change it.
*
* Parameters: 
*  long *plAssistModeStatus - should be set to one of the following values:
*       TB_AMSTATUSMAYEXIT Operator may exit Assist Mode
*       TB_AMSTATUSMAYNOEXIT Operator may not exit Assist Mode
*       TB_AMSTATUSMAYENTER Operator may enter Assist Mode
*       TB_AMSTATUSMAYNOTENTER Operator may not enter Assist Mode
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetAssistModeStatus(long *plAssistModeStatus)
{
    tbtraceIO(_T("TBGetAssistModeStatus()"));
    CBaseMHTransaction* pMHTrx = CCustomerMHTransaction::instance();
    return pMHTrx->GetAssistModeStatus(plAssistModeStatus);
}

/*************************************************************************
* TBGetMICRPrompt - This function is called by the FastLane App when it 
*       receives a TB_NEEDMICR notification from the TB. It retrieves 
*       information requested by TB about the MICR document.
*
* Parameters: 
*  LPTSTR *ppszMICRPrompt - A pointer to a string pointer that is allocated
*       by TB. A prompt to be displayed to the operator to insert the MICR document.
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetMICRPrompt(LPTSTR *ppszMICRPrompt)
{
    tbtraceIO(_T("TBGetMICRPrompt()"));
    CBaseMHMicr* pMHMicr = CCustomerMHMicr::instance();
    return pMHMicr->GetMICRPrompt(ppszMICRPrompt);
}

/*************************************************************************
* TBGetMessageInfo - Get custom message information. The application calls 
*       this when it receives TB_MESSAGESCREEN state event from TB and also 
*       when it gets TB_E_ITEMFAILED return code from TBEnterItem().
*
* Parameters: 
*  PSMessageInfo psMessageInfo - Pointer to the SMessageInfo data structure
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetMessageInfo(PSMessageInfo psMessageInfo)
{
    tbtraceIO(_T("TBGetMessageInfo()"));
    CBaseTBConnectFL* pTBConnectFL = CCustomerTBConnectFL::instance();
    return pTBConnectFL->GetMessageInfo(psMessageInfo);
}

/*************************************************************************
* TBGetCustomerMessage - This function is needed if the DisplayCustomerMessage 
*       option, located in SCOTOPTS file, is turned on and TB wants to provide 
*       some customer specific or transaction specific message on the Scan and Bag
*       screen of the FastLane Application. FastLane calls this function only 
*       when TB returns TB_CUSTOMERMESSAGE state for TBProcessMessage call.
*
* Parameters: 
*  LPTSTR* psCustomerMessageText - TB has to allocate enough memory and fill
*       the customer message into the buffer with a null termination, permitting 
*       FastLane App to release the memory allocated by TB for this field.
*  LPTSTR* psCustomerMessageAudio - TB has to allocate enough memory and fill
*       the customer message audio file into the buffer with a null termination, 
*       permitting FastLane App to release the memory allocated by TB for this 
*       field. If the path is not included in the filename, FastLane assumes 
*       that it is the current sound files’ path of FastLane App.
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetCustomerMessage(LPTSTR* psCustomerMessageText, 
                                          LPTSTR* psCustomerMessageAudio)
{
    tbtraceIO(_T("TBGetCustomerMessage()"));
    CBaseTBConnectFL* pTBConnectFL = CCustomerTBConnectFL::instance();
    return pTBConnectFL->GetCustomerMessage(psCustomerMessageText, psCustomerMessageAudio);
}

/*************************************************************************
* TBGetDataNeededView - Get configuration view data from TB. The application 
*       calls this when it receives a TB_NEEDMOREDATA state event from TB. 
*       The TB uses this mechanism when it wants Fastlane to display a data 
*       entry screen.
*
* Parameters: 
*  PSDataNeededView psDataNeededView - Pointer to the SDataNeededView data structure
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetDataNeededView(PSDataNeededView psDataNeededView)
{
    tbtrace(TM_INFO, _T("TBGetDataNeededView()"));
    CBaseTBConnectFL* pTBConnectFL = CCustomerTBConnectFL::instance();
    return pTBConnectFL->GetDataNeededView(psDataNeededView);
}

/*************************************************************************
* TBGetLoyaltyCardNumber - Passes the loyalty card number information to 
*       the FastLane Core Application. Called when TB_LOYALTYCARD is returned
*       by the TB.
*
* Parameters: 
*  LPTSTR *pszLoyaltyCardNumber - ptr to the loyalty card number
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetLoyaltyCardNumber(LPTSTR *pszLoyaltyCardNumber)
{
    tbtrace(TM_INFO, _T("TBGetLoyaltyCardNumber()"));
    CBaseCurrentTransaction *pCurrTrx = CCustomerCurrentTransaction::instance();
    return pCurrTrx->GetLoyaltyCardNumber(pszLoyaltyCardNumber);
}

/*************************************************************************
* TBSendDataNeeded - Send the data entry information to the TB. The application 
*       calls this as a result of receiving user input from the view generated 
*       by the TBGetDataNeededView function.
*
* Parameters: 
*  LPCTSTR szDataNeededDetails - the first field represents the button pressed,
*       the second field is any numeric data entered (pinpad or scanner), the 
*       semicolon is used as the delimiter between fields
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBSendDataNeeded(LPCTSTR szDataNeededDetails)
{
    tbtrace(TM_INFO, _T("TBSendDataNeeded()"));
    CBaseTBConnectFL* pTBConnectFL = CCustomerTBConnectFL::instance();
    return pTBConnectFL->SendDataNeeded(szDataNeededDetails);
}

/*************************************************************************
* TBNeedStartOfDay - Deprecated - not used
*
* Parameters: NA
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBNeedStartOfDay(TBSTARTOFDAYSTATE &eState)
{
    tbtrace(TM_INFO, _T("TBNeedStartOfDay()"));
    eState = TB_STARTOFDAY_READY;
    return TB_SUCCESS;
}

/*************************************************************************
* CreateMessageObjectByTBDEVICEID - this function creates a new object
*       based on the device id passed in as a parameter, and returns a
*       ptr to that object.
*
* Parameters: 
*  TBDEVICEID nMsgSource - the device id of the object to be created
*
* Returns: CTBMsgObj * - a ptr to the new object, NULL if error
*************************************************************************/
CTBMsgObj *CBaseTrxBroker::CreateMessageObjectByTBDEVICEID(TBDEVICEID nMsgSource)
{
    tbtraceIO(_T("CreateMessageObjectByTBDEVICEID"));
    CTBMsgObj *pMsgObj=NULL;
    switch(nMsgSource) {
    case TBDEV_RETURN_STATE: // Return state message; no additional processing
        pMsgObj = new CReturnStateMsg();
        break;
    case TB_DEV_DRAWER1:
    case TB_DEV_DRAWER2:
        pMsgObj= new CCashDrawerMsg();
        break;
    case TB_DEV_KEYBOARD:
        pMsgObj= new CKeyboardMsg();
        break;
    case TB_DEV_KEYLOCK:
        pMsgObj= new CKeylockMsg();
        break;
    case TB_DEV_MSR:
        pMsgObj= new CMSRMsg();
        break;
    case TB_DEV_PRINTERJOURNAL:
    case TB_DEV_PRINTERRECEIPT:
    case TB_DEV_PRINTERSLIP:
        pMsgObj= new CPrinterMsg();
        break;
    case TB_DEV_MICR:
        pMsgObj= new CMicrMsg();
        break;
    case TB_DEV_PINPAD:
        pMsgObj= new CPinpadMsg();
        break;
    case TB_DEV_ITEM_DETAILS:
        pMsgObj= new CItemDetailsMsg();
        break;
    case TB_DEV_REWARD_DETAILS:
        pMsgObj= new CRewardLineDetailsMsg();
        break;
    case TB_DEV_TOTAL_DETAILS:
        pMsgObj= new CTotalDetailsMsg();
        break;
    case TB_DEV_TENDER_DETAILS:
        pMsgObj= new CTenderDetailsMsg();
        break;
    case TB_DEV_DISPLAYINTERMEDIATE:
    case TB_DEV_DISPLAYPRIMARY:
    case TB_DEV_DISPLAYSECONDARY:
    case TB_DEV_DISPLAYIMMEDIATE:
        
    case TB_DEV_EPSMODULE:
    case TB_DEV_CMOS:
    case TB_DEV_SCALE:
    case TB_DEV_SERIAL:
    case TB_DEV_SCANNER:
    case TB_DEV_ATMMODULE:
    case TB_DEV_DISPLAYDATANEEDED:
    case TB_DEV_UICOMMAND:
    case TB_DEV_UNKNOWN:
    default:
        tbtrace(TM_WARNING, _T("Unknown TBDEVICE Id:%c"), nMsgSource);
        break;
        
    }
    return pMsgObj;
}

/*************************************************************************
* CreateMessageObjectByMessageId - this function creates a new object
*       based on the message id passed in as a parameter, and returns a
*       ptr to that object.
*
* Parameters: 
*  BYTE messageId - the id of the message object to be created
*
* Returns: CTBMsgObj * - a ptr to the new object, NULL if error
*************************************************************************/
CTBMsgObj* CBaseTrxBroker::CreateMessageObjectByMessageId(BYTE messageId)
{
    tbtraceIO(_T("CreateMessageObjectByMessageId"));
    CTBMsgObj* objMsg = NULL;
    
    switch(messageId)
    {
    case TBMSG_TENDER:
        objMsg = new CTenderDetailsMsg();
        break;
    case TBMSG_TOTAL:
        objMsg = new CTotalDetailsMsg();
        break;
    case TBMSG_PRINTER:
        objMsg = new CPrinterMsg();
        break;
    case TBMSG_KEYLOCK:
        objMsg = new CKeylockMsg();
        break;        
    case TBMSG_MICR:
        objMsg = new CMicrMsg();
        break;
    case TBMSG_CASH_DRAWER:
        objMsg = new CCashDrawerMsg();
        break;
    case TBMSG_KEYBOARD: // don't think these are possible
        objMsg = new CTenderDetailsMsg();
        break;
    case TBMSG_ITEM:
        objMsg = new CItemDetailsMsg();
        break;
    case TBMSG_REWARD:
        objMsg = new CRewardLineDetailsMsg();
        break;
    case TBMSG_MSR:
        objMsg = new CMSRMsg();
        break;
    case TBMSG_SCALE:  // don't think these are possible
    case TBMSG_SCANNER: // don't think these are possible
    default:
        tbtrace(TM_WARNING, _T("Unknown Message Id:%c"), messageId);
        break;
    }
    
    return objMsg;
}

/*************************************************************************
* TBProcessMessage - Process the message posted by TB. TB analyzes the message 
*       and returns the state of the host application, based on the presentation 
*       data in the message. This function will farm out the work to the 
*       specific msg device id class to actually process the message.
*
* Parameters: 
*  PSMessageElement psMessage - Pointer to the TB message
*
* Returns: TBSTATE - the current state of the host application
*************************************************************************/
TBSTATE CBaseTrxBroker::TBProcessMessage(PSMessageElement psMessage)
{
    tbtraceIO(_T("TBProcessMessage"));
    TBSTATE  retState = TB_IGNORE;
    CTBMsgObj *pBaseObj=NULL;
    CMsgHandler *pMsgHandler=NULL;
    bool bMsgHandled = false;

    if (NULL == psMessage) 
    {
        tbtrace(TM_WARNING, _T("psMessage is null"));
    }
    else {
        TBDEVICEID nMsgSource = (TBDEVICEID) psMessage->nDeviceID;
        tbtrace(TM_INFO, _T("   DeviceID : %d"), nMsgSource);
        
        pBaseObj= reinterpret_cast<CTBMsgObj *>(psMessage->lParam);
        ASSERT(pBaseObj);
        if ((pBaseObj->GetMsgVersion() == VERSION_GENOBJ_BIN) ||
            (pBaseObj->GetMsgVersion() == VERSION_GENOBJ_XML)) {
            // this is a CTBGenMsg
            CTBGenMsg *pGenObj= dynamic_cast<CTBGenMsg*> (pBaseObj);
            ASSERT(pGenObj);
            if (pGenObj) {
                switch(pGenObj->GetMsgVersion())
                {
                case VERSION_GENOBJ_XML:
                    {
                        tbtrace(TM_DEBUG, _T("   GenMsg - XML"));
                        retState = CCustomerMHGenMsg::instance()->ProcessMessage(pGenObj);
                        bMsgHandled = true;
                        break;
                    }
                    
                case VERSION_GENOBJ_BIN:
                    {
                        tbtrace(TM_DEBUG, _T("   GenMsg - Bin"));
                        CTBMsgObj *pMsgObj= CreateMessageObjectByMessageId(pGenObj->GetTBMsgId());
                        if (pMsgObj) 
                        {
                            nMsgSource= pMsgObj->GetDeviceId();
                            int len=0;
                            BYTE *pBuf= pGenObj->Serialize(len);
                            pMsgObj->Parse(pBuf);
                            delete [] pBuf;
                            
                            // delete the CTBGenMsg object in the lparam 
                            // and replace it with the newly created object.
                            psMessage->lParam= (LPARAM) pMsgObj;
                            delete pBaseObj;  
                            pBaseObj= pMsgObj;
                        }
                        // now fall through to call the original MH handlers
                        bMsgHandled = false;
                        break;
                    }
                default:
                    tbtrace(TM_ERROR, _T("GenMsg - Unexpected Version:%d"),pGenObj->GetMsgVersion());
                    break;
                }
            }
        }

        // If not previously handled, then fall through to original handlers
        if (!bMsgHandled)
        {
            try {
                switch(nMsgSource) {
                case TBDEV_RETURN_STATE: // Return state message; no additional processing
                    {
                        CReturnStateMsg *pretStateMsg= dynamic_cast<CReturnStateMsg*> (reinterpret_cast<CTBMsgObj *>(psMessage->lParam));
                        ASSERT(pretStateMsg);
                        retState = (TBSTATE)pretStateMsg->GetRetState();
                        
                        if(NULL != pretStateMsg->m_pfnHandler)
                        {
                            TBRC rcDummy; // dummy
                            
                            bool bRetVal= (this->*pretStateMsg->m_pfnHandler)
                                (psMessage,
                                rcDummy,
                                NULL);
                        }
                        
                        if ((retState == TB_SENDMESSAGETOSCOT) ||  // here, lparam should be text msg to RAP
                            (retState == TB_FL_SETNEXTSTATE)) {      // here, lparam should be name of FL state to go to.
                            // we want to change lparam to point to a text buffer
                            LPARAM lp= (LPARAM) pretStateMsg->MessageToBuffer(); // gets deleted in ReleaseMessageLPARAM
                            // First, release LPARAM only, then change it.
                            ReleaseMessageLParam(psMessage);
                            // our pTBMsg pointer to CTBMsgObj is now deleted!
                            psMessage->lParam= lp; // gets deleted in ReleaseMessageLPARAM
                        }
                    }
                    break;
                    
                case TB_DEV_DISPLAYINTERMEDIATE:
                case TB_DEV_DISPLAYPRIMARY:
                case TB_DEV_DISPLAYSECONDARY:
                case TB_DEV_DISPLAYIMMEDIATE:
                    pMsgHandler = CCustomerMHDisplay::instance();
                    break;
                case TB_DEV_DRAWER1:
                case TB_DEV_DRAWER2:
                    pMsgHandler = CCustomerMHCashDrawer::instance();
                    break;
                case TB_DEV_KEYBOARD:
                    pMsgHandler = CCustomerMHKeyboard::instance();
                    break;
                case TB_DEV_KEYLOCK:
                    pMsgHandler = CCustomerMHKeylock::instance();
                    break;
                case TB_DEV_MSR:
                    pMsgHandler = CCustomerMHMSR::instance();
                    break;
                case TB_DEV_PRINTERJOURNAL:
                case TB_DEV_PRINTERRECEIPT:
                case TB_DEV_PRINTERSLIP:
                    pMsgHandler = CCustomerMHPrinter::instance();
                    break;
                case TB_DEV_MICR:
                    pMsgHandler = CCustomerMHMicr::instance();
                    break;
                case TB_DEV_PINPAD:
                    pMsgHandler = CCustomerMHPINPad::instance();
                    break;
                case TB_DEV_ITEM_DETAILS:
                case TB_DEV_REWARD_DETAILS:
                    pMsgHandler = CCustomerMHItem::instance();
                    break;
                case TB_DEV_TOTAL_DETAILS:
                    pMsgHandler = CCustomerMHTotals::instance();
                    break;
                case TB_DEV_TENDER_DETAILS:
                    pMsgHandler = CCustomerMHTender::instance();
                    break;
                case TB_DEV_FISCALPRINTER:
                    pMsgHandler = CCustomerMHFiscalPrinter::instance();
                    break;
                case TB_DEV_EPSMODULE:
                case TB_DEV_CMOS:
                case TB_DEV_SCALE:
                case TB_DEV_SERIAL:
                case TB_DEV_SCANNER:
                case TB_DEV_ATMMODULE:
                case TB_DEV_DISPLAYDATANEEDED:
                case TB_DEV_UNKNOWN:
                case TB_DEV_UICOMMAND:
                    {
                        tbtrace(TM_WARNING, _T( "Currently unhandled device message at Baselevel") );
                        retState = TB_IGNORE;
                    }
                    break;
                default:
                    {
                        tbtrace(TM_WARNING, _T( "Unknown device at Baselevel") );
                        retState = TB_IGNORE;
                    }
                    break;
                }
            }
            catch(...) 
            {
                tbtrace(TM_ERROR, _T("*** Unknown exception in ProcessMessages, device:%d"),nMsgSource);
            }
            if (pMsgHandler) 
                retState = pMsgHandler->ProcessMessage(pBaseObj);
        }
    }

    // At a minimum we know the trx's are dirty when items are sold
    if (TB_ITEMSOLD == retState)
        CCustomerCurrentTransaction::instance()->DirtyTrx(TRUE);

    return retState;
}

/*************************************************************************
* TBReleaseMessage - Free the message element posted by the TB and any
*       memory associated with it.
*
* Parameters: 
*  PSMessageElement psMessage - Pointer to the message element as passed to
*       the application as a notification message.
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBReleaseMessage(PSMessageElement psMessage)
{
    tbtrace(TM_ENTRY, _T("+TBReleaseMessage()"));
    TBRC rc = TB_SUCCESS;
    TBASSERT(psMessage);
    if (!psMessage) return TB_E_INVALIDPARAM;
    rc = ReleaseMessageLParam(psMessage);
    try
    {
        if (psMessage->pData)
            delete [] (TCHAR *)(psMessage->pData);
        delete psMessage;
    }
    catch(...)
    {
        // trace for exceptions while processing display messages
        tbtrace(TM_ERROR, _T("******  Unknown exception received ReleaseMessage()"));
    }
    tbtrace( TM_ENTRY, _T("-TBReleaseMessage() returning %s"), GetTBRCName(rc));
    return rc;
}

/*************************************************************************
* ReleaseMessageLParam - Frees the message element 
*
* Parameters: 
*  PSMessageElement psMessage - Pointer to the message element as passed to
*       the application as a notification message.
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::ReleaseMessageLParam(PSMessageElement psMessage)
{
   //tbtraceIO(_T("ReleaseMessageLParam()")); 
   // commented to reduce tracing.  will always be called by TBRelease if psMessage not null
   // otherwise, a warning is already logged

    TBRC rc = TB_SUCCESS;
    CTBMsgObj *pBase= NULL;
    
    // We need this outer try-catch block to catch any possible exceptions 
    // that may occur when deleteing a void* pointer.
    // Also, the tbtrace calls are important in the catch blocks. 
    // If we remove these, and the catch block is empty, the compiler (in release build) 
    // will optimize away the whole catch block and the exception will not be caught!
    // This will cause a crash. Microsoft documented this bug (Q247203) and recommends that 
    // we remove the Global Optimization switch, which is on by default in
    // release build.
    // The easier way out is to simply not leave the catch block empty. 
    // Trivial statements like "int i=0;" inside the catch block will NOT make it
    // "non-empty", because the compiler will whack this line out too, as part of
    // its optimization process. These problems will not occur in debug build however.
    try
    {
        if (psMessage->lParam)
        {
            try
            {
                pBase= dynamic_cast<CTBMsgObj *> (reinterpret_cast<CTBMsgObj *>(psMessage->lParam));
                if(pBase != NULL)
                {
                    delete pBase;
                }
                else
                {
                    // Ideally we should  never ever reach here, because in all new projects 
                    // we will be deriving all message objects from the base class
                    // However some FL messages require a text string, so if it is not a 
                    // MsgObj, then we assume it is a TCHAR buffer.
                    tbtrace(TM_WARNING, _T("ReleaseMessageLParam not CTBMsgObj. deleting as TCHAR*)"));
                    delete[] ((TCHAR*)psMessage->lParam);
                }
            }
            catch(...) // not a CTBMsgObj derived class.
            {
                tbtrace(TM_WARNING, _T("ReleaseMessageLParam exception (not CTBMsgObj. deleting as TCHAR*)"));
                delete[] ((TCHAR*)psMessage->lParam);
            }
        }
    }
    catch(...)
    {
        tbtrace(TM_ERROR, _T("ReleaseMessageLParam exception"));
    }
    return rc;
}

  
/*************************************************************************
* GetNextState
************************************************************************/

TBRC CBaseTrxBroker::GetNextState(LPCTSTR lpLastStateName, LPCTSTR lpNextStateName,
                              bool *pbOverride, LPTSTR *lpOverrideStateName, CDescriptorList &dlParameterList)
{
    *pbOverride = false;
    CCustomerTBProperties &tbProp= *CCustomerTBProperties::instance();
    CString csOverrideStateName= tbProp.GetProperty(TBPROP_NEWFLSTATE);

    tbtrace(TM_INFO, _T("GetNextState() oldFLState:%s, newFLState:%s, TBoverride:%s"),
        lpLastStateName, lpNextStateName, (LPCTSTR) csOverrideStateName);
    
    if (csOverrideStateName.GetLength() > 0)
    {
        *pbOverride = true;
        *lpOverrideStateName = new TCHAR[csOverrideStateName.GetLength() +1];
        _tcscpy(*lpOverrideStateName, (LPCTSTR)csOverrideStateName);
        tbProp.SetProperty(TBPROP_NEWFLSTATE, _T(""));

//        // Set parameters for descriptor list
//        csWork = _T("test");
//        dlParameterList.AddDescriptor(csWork);
//        dlParameterList.AddDescriptor((DWORD)0);  //PS_NOTEXT

    }
    return TB_SUCCESS;
}

/*************************************************************************
* GetTBSTATEName - returns a string representation of the TBSTATE (TB_ITEMSOLD,etc)
*
* Parameters: 
*  long key - long value of the TBSTATE (TBSTATE is an enum)
*
* Returns: TCHAR * - string of TBSTATE
*************************************************************************/
LPCTSTR CBaseTrxBroker::GetTBSTATEName(long key)
{
    for (int i=0; CoreTBStates[i].key != TB_MAXCORETBSTATEID; i++) {
        if (CoreTBStates[i].key == key)
            return CoreTBStates[i].szname;
    }
    for (i=0; CustomTBStates[i].key != TB_MAXCUSTOMSTATEID; i++) {
        if (CustomTBStates[i].key == key)
            return CustomTBStates[i].szname;
    }
    
    return _T("UNKNOWN");
}

/*************************************************************************
* GetTBSTATEFromName - return the long value of the TBSTATE given its
*       string representation
*
* Parameters: 
*  LPCTSTR szTBState - string of TBSTATE name
*
* Returns: long - long value of TBSTATE (from enum)
*************************************************************************/
long CBaseTrxBroker::GetTBSTATEFromName(LPCTSTR szTBState)
{
    for (int i=0; CoreTBStates[i].key != TB_MAXCORETBSTATEID; i++) {
        if (_tcscmp(CoreTBStates[i].szname, szTBState) == 0)
            return CoreTBStates[i].key;
    }
    for (i=0; CustomTBStates[i].key != TB_MAXCUSTOMSTATEID; i++) {
        if (_tcscmp(CustomTBStates[i].szname, szTBState) == 0)
            return CustomTBStates[i].key;
    }
    // did not find this TBSTATE return code!
    ASSERT(0);
    return 0;
}

/*************************************************************************
* GetTBRCName - returns a string representation of the TBRC (TB_SUCCESS,etc)
*
* Parameters: 
*  long key - long value of the TBRC 
*
* Returns: TCHAR * - string of TBRC
*************************************************************************/
LPCTSTR CBaseTrxBroker::GetTBRCName(long key)
{
    for (int i=0; CoreTBRCs[i].key != TB_MAXTBRC; i++) {
        if (CoreTBRCs[i].key == key)
            return CoreTBRCs[i].szname;
    }
    
    return _T("UNKNOWN");
}

/*************************************************************************
* TBGetProperty - returns a CString value from the property specified in the
*       parameter szProperty. Specify if this is a hook property or not using
*       the bHookProperty parameter.
*
* Parameters: 
*  LPCTSTR szProperty - name of Property to retrieve the value from
*  bool bHookProperty - is this a hook property, assumed false if not specified
*
* Returns: CString - value retrieved from property
*************************************************************************/
CString CBaseTrxBroker::TBGetProperty(LPCTSTR szProperty, bool bHookProperty /*=false*/)
{
   if (bHookProperty)
      return CCustomerTBProperties::instance()->GetHookProperty(szProperty);
   else
      return CCustomerTBProperties::instance()->GetProperty(szProperty);
}

/*************************************************************************
* TBSetProperty - set the value of a Property. Specify if this is a hook 
*       property or not using the bHookProperty parameter.
*
* Parameters: 
*  LPCTSTR szProperty - name of Property to retrieve the value from
*  LPCTSTR szValue - value to set the property with
*  bool bHookProperty - is this a hook property, assumed false if not specified
*
* Returns: void
*************************************************************************/
void CBaseTrxBroker::TBSetProperty(LPCTSTR szProperty, LPCTSTR szValue, bool bHookProperty /*=false*/)
{
   if (bHookProperty)
      CCustomerTBProperties::instance()->SetHookProperty(szProperty, szValue);
   else
      CCustomerTBProperties::instance()->SetProperty(szProperty, szValue);
}

/*************************************************************************
* SetFLState - this sends a return status message to FL. FL will ask TB to
*       process the message and TB will then return the szState param as the
*       new TBSTATE.
*
* Parameters: 
*  LPCTSTR szState - string of the TBSTATE that TB will eventually return to FL
*
* Returns: long - in Base TB, always TB_SUCCESS
*************************************************************************/
long CBaseTrxBroker::SetFLState( LPCTSTR szState/*=NULL*/ )
{
    tbtrace( TM_ENTRY, _T("SetFLState(%s)"), szState);
    TBSetProperty(TBPROP_NEWFLSTATE, szState);
    return CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_CHANGESTATE, NULL, NULL );
}

/*************************************************************************
* GetCurrentInProgressState - get the current InProgress TB state
*
* Parameters: none
*
* Returns: long - the value of the current InProgress TB state 
*************************************************************************/
long CBaseTrxBroker::GetCurrentInProgressState()
{
    return CCustomerCurrentTransaction::instance()->GetCurrentInProgressState();
}

/*************************************************************************
* GetCurrentInProgressState - returns the long value of the InProgress TB 
*       state of the string value passed in as a parameter - note, this is
*       not necessarily the current InProgress state.
*
* Parameters: 
*  LPCTSTR szStateName - string value of InProgress state to be looked up
*
* Returns: long - the value of the InProgress TB state 
*************************************************************************/
long CBaseTrxBroker::GetCurrentInProgressState(LPCTSTR szStateName)
{
    return CCustomerCurrentTransaction::instance()->GetCurrentInProgressState(szStateName);
}

/*************************************************************************
* GetCurrentInProgressStateName- returns the string value of the InProgress 
*       TB state of the long value passed in as a parameter - note, this is
*       not necessarily the current InProgress state.
*
* Parameters: 
*  long key - value of TB InProgress state to be looked up
*
* Returns: LPCTSTR - string of the TB InProgress state
*************************************************************************/
LPCTSTR CBaseTrxBroker::GetCurrentInProgressStateName(long key)
{
    return CCustomerCurrentTransaction::instance()->GetCurrentInProgressStateName(key);
}

/*************************************************************************
* SetCurrentInProgressState - sets the current TB InProgress state to the
*       value passed in
*
* Parameters: 
*  long nState - value of the new TB InProgress state
*
* Returns: void
*************************************************************************/
void CBaseTrxBroker::SetCurrentInProgressState(long nState)
{
    CCustomerCurrentTransaction::instance()->SetCurrentInProgressState( nState);
}

/*************************************************************************
* SetCurrentInProgressState - sets the current TB InProgress state to the
*       value passed in
*
* Parameters: 
*  LPCTSTR szStateName - string of new TB InProgress state, this will be
*       resolved to the long value
*
* Returns: void
*************************************************************************/
void CBaseTrxBroker::SetCurrentInProgressState(LPCTSTR szStateName)
{
    CCustomerCurrentTransaction::instance()->SetCurrentInProgressState(szStateName);
}

/*************************************************************************
* StoreMode, IsInStoreMode - accessors for the member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
void CBaseTrxBroker::StoreMode(bool b)
{
    m_bStoreMode = b;
}

bool CBaseTrxBroker::IsInStoreMode(void)
{
    return m_bStoreMode;
}

/*************************************************************************
* TriggerMsgMap - triggers parsing of the message map
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseTrxBroker::TriggerMsgMap()
{
    CCustomerMsgMap::instance()->TriggerMsgMap();
}


/*************************************************************************
* PreprocessSignatureData - 
*
* Parameters: none
*
* Returns: void
*************************************************************************/
TBRC CBaseTrxBroker::PreprocessSignatureData(LPCTSTR lpFileName,BSTR bstrSignatureData, LPCTSTR lpReceiptType, BOOL* bProcessData)
{
	tbtraceIO( _T("PreprocessSignatureData") );
	TBRC nRetCode;
	nRetCode = TB_E_FUNCTIONNOTSUPPORTED;
	return nRetCode;   
}

/*************************************************************************
* GetExtensionStateSetup - 
*
* Parameters: none
*
* Returns: void
*************************************************************************/
TBRC CBaseTrxBroker::GetExtensionStateSetup(LPTSTR szUICtx, LPTSTR szDevCtx, psPropChangeRequestList pExtChangeList)
{
	tbtraceIO(_T("GetExtensionStateSetup"));
	return TB_SUCCESS;
}


/*************************************************************************
* NotifyExtensionStateEvent - 
*
* Parameters: none
*
* Returns: void
*************************************************************************/
TBSTATE CBaseTrxBroker::NotifyExtensionStateEvent(psExtStateEventNotifyIn pEventNotifyIn, psExtStateEventNotifyOut pEventNotifyOut)
{
	tbtraceIO(_T("NotifyExtensionStateEvent"));
	return TB_IGNORE;
	
}

/*************************************************************************
* SendTBCommand - 
*
* Parameters: none
*
* Returns: void
*************************************************************************/
TBRC CBaseTrxBroker::SendTBCommand(long lCommandID, va_list pArgs)
{
//    tbtraceIO( _T("SendTBCommand()"));
	
	bool bHookProperty = TRUE;
	
    switch(lCommandID)
    {
		
	case TBCMD_ASSISTMENUSET:
		{   
			// base implementation is blank
			return TB_SUCCESS;
		}
		break;
		
	case TBCMD_GET_TBPROPERTY:
		bHookProperty = FALSE;
	case TBCMD_GET_HOOKPROPERTY:
		{
			CString * pstrProp = va_arg(pArgs, CString*);
			CString * pstrValue = va_arg(pArgs, CString*);
			
			*pstrValue = TBGetProperty(*pstrProp, bHookProperty);
			return TB_SUCCESS;
		}
		break;

	case TBCMD_SET_TBPROPERTY:
		bHookProperty = FALSE;
	case TBCMD_SET_HOOKPROPERTY:
		{
			CString * pstrProp = va_arg(pArgs, CString*);
			CString * pstrValue = va_arg(pArgs, CString*);
			TBSetProperty(*pstrProp, *pstrValue, bHookProperty);
			return TB_SUCCESS;
		}
		break;
	default:
		return TB_E_FUNCTIONNOTSUPPORTED;
		break;
	};
}


/*************************************************************************
* TBValidateHeadCashier - If this is a valid Cashier and PW then return TRUE.
*       Base TB does not have an implementation for this - solution specific.
*
* Parameters: 
*  LPCTSTR szCashierID - Cashier ID
*  LPCTSTR szCashierPw - Cashier password
*
* Returns: BOOL - TRUE if a valid cashier and PW, FALSE otherwise
*************************************************************************/
BOOL CBaseTrxBroker::TBValidateHeadCashier( LPCTSTR szCashierID, LPCTSTR szCashierPw )
{
   tbtraceIO(_T("TBValidateHeadCashier()"));
	return FALSE;   
}


/*************************************************************************
* TBCreateReportEntry
*
* Parameters: 
*
*  PSCreateReportEntry psCreateReportEntry
*  
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBCreateReportEntry(PSCreateReportEntry psCreateReportEntry)
{
    tbtraceIO( _T( "TBCreateReportEntry()"));

	return TB_E_FUNCTIONNOTSUPPORTED;

}

/*************************************************************************
* TBGetCurrentBusinessDate()
*
* Parameters: 
*
*  CString &csBusinessDate
*  
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetCurrentBusinessDate(CString &csBusinessDate)
{
    tbtraceIO( _T( "TBGetCurrentBusinessDate()"));

	return TB_E_FUNCTIONNOTSUPPORTED;

}


/*************************************************************************
* TBSetBusinessDate()
*
* Parameters: 
*
*  LPCTSTR szSetBusinessDate
*  
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBSetBusinessDate(LPCTSTR szSetBusinessDate)
{
    tbtraceIO( _T( "TBSetBusinessDate()"));

	return TB_E_FUNCTIONNOTSUPPORTED;

}


/*************************************************************************
* TBGetGiftCardBalance - Get the available on the gift card from the POS
*
* Parameters:
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseTrxBroker::TBGetGiftCardBalance(long *plBalance)
{
    tbtraceIO( _T( "TBGetGiftCardBalance()"));
    CBaseMHTotals* pMHTotals = CCustomerMHTotals::instance();

	return TB_E_FUNCTIONNOTSUPPORTED;
    // return pMHTotals->GetGiftCardBalance(long *plBalance);
}

/*************************************************************************
* TBDisableTenderButton
*
* Parameters: 
*  TBTENDERTYPE nTenderType - tender type from enum (TB_TENDER_CASH, etc)
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
BOOL CBaseTrxBroker::TBDisableTenderButton(TBTENDERTYPE nTenderType)
{
    tbtraceIO( _T( "TBDisableTenderButton()"));
    CBaseMHTender* pMHTender = CCustomerMHTender::instance();
	return FALSE;
    // return pMHTender->DisableTenderButton(TBTENDERTYPE nTenderType);

}

//////////////////////////////////////////////////////////////////////
///
/// TBEnterMICRWithType, Core API in support of MICR/Endorsement at FastLane
///
///	@param TBTENDERTYPE nTenderType - Tender Type
///	@param LPCTSTR szRawData - Raw MICR data from the device
/// @param LPCTSTR szTransitNumber - Bank Transit Number
/// @param LPCTSTR szAccountNumber - Customer Account Number
/// @param LPCTSTR szBankNumber - ABA Bank number
/// @param LPCTSTR szSerialNumber - Check document number
/// @param LPCTSTR szEPC - Extended Processing Code
/// @param long lType - Type (1=Personal, 2=Business, 99= Unknown)
/// @param long lCountryCode - Country Code (1=USA, 2=Canada, 3=Mexico, 99=Unknown)
///	@return TBRC - 
//////////////////////////////////////////////////////////////////////
TBRC CBaseTrxBroker::TBEnterMICRWithType(
               TBTENDERTYPE nTenderType,
               LPCTSTR szRawData,         
               LPCTSTR szTransitNumber,   
               LPCTSTR szAccountNumber,   
               LPCTSTR szBankNumber,      
               LPCTSTR szSerialNumber,    
               LPCTSTR szEPC,             
               long lType,                
               long lCountryCode)
{
    tbtraceIO(_T("TBEnterMICRWithType()"));
    CBaseMHMicr* pMHMicr = CCustomerMHMicr::instance();
    return pMHMicr->EnterMICRWithType(nTenderType, szRawData, szTransitNumber, szAccountNumber, 
        szBankNumber, szSerialNumber, szEPC, lType, lCountryCode);
}


char* CBaseTrxBroker::ConvertLPWSTRToLPSTR (LPWSTR lpwszStrIn, int sizein, int* sizeout)
{
	LPSTR pszOut = NULL;
	*sizeout=0;
	if (lpwszStrIn != NULL)
	{
		//	int nInputStrLen = wcslen (lpwszStrIn);
		int nInputStrLen = sizein;
		
		// Double NULL Termination
		int nOutputStrLen = WideCharToMultiByte (CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
		pszOut = new char [nOutputStrLen];
		
		if (pszOut)
		{
			memset (pszOut, 0x00, nOutputStrLen);
			WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
		}
		*sizeout=nOutputStrLen-2;
	}
	return pszOut;
}


CString CBaseTrxBroker::DecodeStringBase64(CString csEncodedData)
{
    int nBufferLength = csEncodedData.GetLength();
    LPTSTR sBuffer = csEncodedData.GetBuffer(nBufferLength);
    
    //need to convert to ANSI for Base64 Encoding
    char *myANSIdata;
    int myANSIDataSize = 0;
    int nDecodedSize=wcslen (sBuffer);
#ifdef _UNICODE

    myANSIdata = ConvertLPWSTRToLPSTR((LPTSTR)sBuffer,nDecodedSize,&myANSIDataSize);
#else 
    myANSIdata = new char[nBufferLength+1];
    myANSIDataSize = nBufferLength;
    strcpy(myANSIdata, sBuffer);
#endif

    nDecodedSize=4*nBufferLength*sizeof(TCHAR);
    char *buffer = new char[nDecodedSize];
    memset(buffer,0,nDecodedSize);
    Decode(myANSIdata, myANSIDataSize, buffer);

    CString csDecodedString = buffer;
    csEncodedData.ReleaseBuffer();

    delete [] buffer;
    delete [] myANSIdata;

    return csDecodedString;
}

void CBaseTrxBroker::Decode(const void* data, const size_t nLen, LPSTR szDest)
{
    int nBase64Index[256] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,62,63,62,62,63,52,53,54,55,56,57,58,59,60,61,
		0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
		21,22,23,24,25,0,0,0,0,63,0,26,27,28,29,30,31,32,33,34,35,36,37,38,
		39,40,41,42,43,44,45,46,47,48,49,50,51 };
	
    unsigned char* pEncodedStr = (unsigned char*)data;
    int pad = nLen > 0 && (nLen % 4 || pEncodedStr[nLen - 1] == '=');
    const size_t L = ((nLen + 3) / 4 - pad) * 4;
    //std::string str(L / 4 * 3 + pad, '\0');
    std::string str; 
    const size_t length = 3 *((nLen + 3) / 4);
    str.resize(length);
    char* out = &str[0];
	
    for (size_t i = 0, j = 0; i < L; i += 4)
    {
        int n = nBase64Index[pEncodedStr[i]] << 18 
			| nBase64Index[pEncodedStr[i + 1]] << 12 
			| nBase64Index[pEncodedStr[i + 2]] << 6 
			| nBase64Index[pEncodedStr[i + 3]];
        out[j++] = n >> 16;
        out[j++] = n >> 8 & 0xFF;
        out[j++] = n & 0xFF;
    }
    
    if (pad)
    {
        int n = nBase64Index[pEncodedStr[L]] << 18 | nBase64Index[pEncodedStr[L + 1]] << 12;
        out[j++] = n >> 16; 
		
        if (nLen > L + 2 && pEncodedStr[L + 2] != '=')
        {
            n |= nBase64Index[pEncodedStr[L + 2]] << 6;
            out[j++] = n >> 8 & 0xFF;
        }
    }
    str.resize(j);
    strcpy(szDest, str.c_str());
}
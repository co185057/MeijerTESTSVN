///////////////////////////////////////////////////////////////////////////
///
/// \file TBAPICore.h
/// \brief contains all TBAPI calls core Fastlane expects to be implemented.
///
//////////////////////////////////////////////////////////////////////
#include "TransBroker.h"
#include "CustomerTrxBroker.h"
#include "BaseObjectManager.h"


#define T_ID _T("TBAPICore")

CCustomerTrxBroker* pTB = NULL;

/***********************************************************************************************/
/*>>>>   DO NOT CHANGE THE ORDER OF LISTED CORE FUNCTIONS BECAUSE OF SCOTAPP STATIC LINK  <<<< */
/*>>>>   ALWAYS VERIFY ORDER HERE MATCHES THE ORDER THAT SCOTAPP WAS COMPILED WITH        <<<< */
/***********************************************************************************************/

//////////////////////////////////////////////////////////////////////
///
/// TBCancelTransaction, ### ADD DESC ###
///
///	@param BOOL
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBCancelTransaction(const BOOL fOverride)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return  pTB->TBCancelTransaction(fOverride);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBCopyFileFromServer,
///
///	@param LPCTSTR
///	@param LPCTSTR
///	@param BOOL
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBCopyFileFromServer(LPCTSTR szFileName, LPCTSTR
                          szDestinationFilePath, const BOOL fForce)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBCopyFileFromServer(szFileName, szDestinationFilePath, fForce);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBCopyFileToServer,
///
///	@param LPCTSTR
///	@param LPCTSTR
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBCopyFileToServer(LPCTSTR szFileName, LPCTSTR szSourceFilePath)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBCopyFileToServer(szFileName, szSourceFilePath);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBEnterItem,
///
///	@param LPCTSTR
///	@param BOOL
///	@param long
///	@param long
///	@param long
///	@param BOOL
///	@param PSEnterItemDetails
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBEnterItem(LPCTSTR szItemCode,
                 const BOOL fVoid, const BOOL fOverride,
                 const long nQuantity, const long nWeight,
                 const long nTareCode, const long lPrice, const BOOL fQuantityConfirmed, PSEnterItemDetails psMoreDetails)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBEnterItem(szItemCode, fVoid, fOverride, nQuantity, nWeight, nTareCode, lPrice, fQuantityConfirmed, psMoreDetails);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBEnterTender,
///
///	@param TBTENDERTYPE
///	@param long LPCTSTR
///	@param int
///	@param LPCTSTR
///	@param BOOL
///	@param BOOL
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBEnterTender( TBTENDERTYPE nTenderType, 
                   const long   lAmount,
                   LPCTSTR      szCardData, 
                   const int    nPINLength,
                   LPCTSTR       szEncryptedPIN, 
                   const BOOL   fOverride,
                   const BOOL   fVoid)
{
    return (pTB ? pTB->TBEnterTender( nTenderType, lAmount, szCardData, nPINLength, 
        szEncryptedPIN, fOverride, fVoid) : TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBFinish,
///
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBFinish(void)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBFinish();
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetItemDetails,
///
///	@param PSItemDetails
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBGetItemDetails(PSItemDetails psItemDetails)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBGetItemDetails(psItemDetails);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetItemExtendedDetails
///
///	@param PSItemExtendedDetails
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBGetItemExtendedDetails(PSItemExtendedDetails psItemExtendedDetails)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return pTB->TBGetItemExtendedDetails(psItemExtendedDetails);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetJournalData,
///
///	@return HFILE
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
HFILE TBGetJournalData(void)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBGetJournalData();
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetPINWorkingKey, 
///
///	@param PSPINWorkingKey
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBGetPINWorkingKey(PSPINWorkingKey psPINWorkingKey)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBGetPINWorkingKey(psPINWorkingKey);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetReceiptData,
///
///	@return HFILE
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
HFILE TBGetReceiptData(void)
{
    if (!pTB)
    {
        return NULL;
    }
    else
    {
        return   pTB->TBGetReceiptData();
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetSlipData,
///
///	@return HFILE
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
HFILE TBGetSlipData(void)
{
    if (!pTB)
    {
        return NULL;
    }
    else
    {
        return   pTB->TBGetSlipData();
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetTenderDetails,
///
///	@param PSTenderDetails
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBGetTenderDetails(PSTenderDetails psTenderDetails)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBGetTenderDetails(psTenderDetails);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBPrint,
///
///	@param LPCTSTR
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBPrint(LPCTSTR szPrintFile)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return pTB->TBPrint(szPrintFile);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetTerminalNumber,
///
///	@return long
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
long TBGetTerminalNumber(void)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBGetTerminalNumber();
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetCurrentTransactionNumber,
///
///	@return long
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
long TBGetCurrentTransactionNumber(void)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return pTB->TBGetCurrentTransactionNumber();
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetTotalDetails,
///
///	@param PSTotalDetails
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBGetTotalDetails(PSTotalDetails psTotalDetails)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBGetTotalDetails(psTotalDetails);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetVersion,
///
///	@param LPTSTR
///	@param long
///	@return long
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
long TBGetVersion(LPTSTR szVersionString, long nBufferLength)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBGetVersion(szVersionString, nBufferLength);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBInitialize
///
///	@param HWND
///	@param LPCTSTR
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBInitialize(HWND hWnd, LPCTSTR szFilePath)
{
    if (!pTB)
    {
        pTB = CCustomerTrxBroker::instance();
    }
    TBASSERT(pTB);
    return pTB->TBInitialize(hWnd, szFilePath);
}

//////////////////////////////////////////////////////////////////////
///
/// TBIsAllowed, 
///
///	@param TBFEATURE
///	@return BOOL
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
BOOL TBIsAllowed(TBFEATURE nFeature)
{
    if (!pTB)
    {
        return false;
    }
    else
    {
        return   pTB->TBIsAllowed(nFeature);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBKeyPress,
///
///	@param USHORT
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBKeyPress(const USHORT uKey)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBKeyPress(uKey);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBLoadOptions,
///
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBLoadOptions(void)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBLoadOptions();
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBLogErrorToServer,
///
///	@param LPCTSTR
///	@param int
///	@param int
///	@param LPCTSTR
///	@param LPCTSTR
///	@param int
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBLogErrorToServer(LPCTSTR szErrorText, const int nSeverity,
                        const int nComponentID, LPCTSTR szComponentID,
                        LPCTSTR szModuleID, const int nModuleLineNumber)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBLogErrorToServer(szErrorText, nSeverity, nComponentID, szComponentID, szModuleID, nModuleLineNumber);
        
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBProcessMessage,
///
///	@param PSMessageElement
///	@return TBSTATE
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBSTATE TBProcessMessage(PSMessageElement psMessage)
{
    return (pTB ? pTB->TBProcessMessage(psMessage) : TB_UNKNOWN);
}

//////////////////////////////////////////////////////////////////////
///
/// TBRefreshState,
///
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBRefreshState(void)
{
    return (pTB ? pTB->TBRefreshState() : TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBReleaseJournalData,
///
///	@param HFILE
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBReleaseJournalData(HFILE hJournalData)
{
    return (pTB ? pTB->TBReleaseJournalData(hJournalData): TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBReleaseMessage,
///
///	@param PSMessageElement
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBReleaseMessage(PSMessageElement psMessage)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBReleaseMessage(psMessage);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBReleaseReceiptData,
///
///	@param HFILE
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBReleaseReceiptData(HFILE hReceiptData)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBReleaseReceiptData(hReceiptData);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBReleaseSlipData,
///
///	@param HFILE
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBReleaseSlipData(HFILE hSlipData)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBReleaseSlipData(hSlipData);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBResumeTransaction
///
///	@param LPCTSTR
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBResumeTransaction(LPCTSTR szSuspendID)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBResumeTransaction(szSuspendID);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBSetMonitorAll,
///
///	@param BOOL
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBSetMonitorAll(BOOL fMonitor)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBSetMonitorAll(fMonitor);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBStart,
///
///	@param LPCTSTR
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBStart(LPCTSTR szOverrideCode)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBStart(szOverrideCode);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBSuspendTransaction,
///
///	@param BOOL
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBSuspendTransaction(const BOOL fOverride)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBSuspendTransaction(fOverride);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBSuspendTransactionEx,
///
///	@param BOOL
///	@param LPCTSTR
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBSuspendTransactionEx(const BOOL fOverride, LPCTSTR szSuspendReasons)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return pTB->TBSuspendTransactionEx(fOverride, szSuspendReasons);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBUninitialize,
///
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBUninitialize(void)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        TBRC rc = pTB->TBUninitialize();
        
        if ( TB_UNINIT_CLEANUP == rc )
        {
            delete pTB;
            pTB = NULL;
            rc = TB_SUCCESS;
        }
        
        return rc;
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBValidateOperator
///
///	@param LPCTSTR
///	@param LPCTSTR
///	@return BOOL
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
BOOL TBValidateOperator(LPCTSTR szOperatorID, LPCTSTR szOperatorPw)
{
    if (!pTB)
    {
        return false;
    }
    else
    {
        return   pTB->TBValidateOperator(szOperatorID, szOperatorPw);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBValidOperator, 
///
///	@param LPCTSTR
///	@return BOOL
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
BOOL TBValidOperator(LPCTSTR szOperatorID)
{
    if (!pTB)
    {
        return false;
    }
    else
    {
        return   pTB->TBValidOperator(szOperatorID);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBValidPassword,
///
///	@param LPCTSTR
///	@return BOOL
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
BOOL TBValidPassword(LPCTSTR szPassword)
{
    if (!pTB)
    {
        return false;
    }
    else
    {
        return   pTB->TBValidPassword(szPassword);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetTransactionLimit,
///
///	@return long
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
long TBGetTransactionLimit(void)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBGetTransactionLimit();
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetQuantityLimit,
///
///	@return long
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
long TBGetQuantityLimit(void)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBGetQuantityLimit();
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBVerifyLoyaltyCard,
///
///	@param LPCTSTR
///	@param in
///	@return TBLOYALTYCARDTYPE
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBLOYALTYCARDTYPE TBVerifyLoyaltyCard(LPCTSTR szCardData, int nDataLen)
{
    if (!pTB)
    {
        return LC_INVALID;
    }
    else
    {
        return   pTB->TBVerifyLoyaltyCard(szCardData, nDataLen);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBSendMsg2Host,
///
///	@param LPCTSTR
///	@param int
///	@param TBDEVICEID
///	@param UCHAR
///	@param long
///	@param bool
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBSendMsg2Host(LPCTSTR szSend,const int iMsgLen, const TBDEVICEID nDeviceClass,const UCHAR nFunctionCode,
                    const long  lOptFileHandle, const bool  fOriginatePipeMsg)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return pTB->TBSendMsg2Host(szSend,iMsgLen,nDeviceClass,nFunctionCode,
            lOptFileHandle,fOriginatePipeMsg);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGet2x20,
///
///	@param LPCTSTR
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBGet2x20(LPCTSTR szDisplay)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return pTB->TBGet2x20(szDisplay);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBEPSStartOfDay,
///
///	@param LPVOID
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBEPSStartOfDay(LPVOID lp)
{
    if (!pTB) {
        return TB_E_NOTINITIALIZED;
    } else {
        return pTB->TBEPSStartOfDay(lp);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBEPSEndOfDay,
///
///	@param LPVOID
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBEPSEndOfDay(LPVOID lp)
{
    if (!pTB) {
        return TB_E_NOTINITIALIZED;
    } else {
        return pTB->TBEPSEndOfDay(lp);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetAvailableFoodStampAmount,
///
///	@param PSEBTAvailBalance
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBGetAvailableFoodStampAmount(PSEBTAvailBalance psEBTAvailBalance)
{
    return (pTB ? pTB->TBGetAvailableFoodStampAmount(psEBTAvailBalance) : TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetAvailableBenefitAmount,
///
///	@param PSEBTAvailBalance
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBGetAvailableBenefitAmount(PSEBTAvailBalance psEBTAvailBalance)
{
    return (pTB ? pTB->TBGetAvailableBenefitAmount(psEBTAvailBalance) : TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBRequestFoodStampEligibleTotal,
///
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBRequestFoodStampEligibleTotal()
{
    return (pTB ? pTB->TBRequestFoodStampEligibleTotal() : TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBParseFoodStampTotal,
///
///	@param PSTotalDetails
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBParseFoodStampTotal(PSTotalDetails psTotalDetails)
{
    return (pTB ? pTB->TBParseFoodStampTotal(psTotalDetails) : TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBSendSignOn,
///
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose 
TBRC TBSendSignOn(void)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBSendSignOn();
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBSendSignOff,
///
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose 
TBRC TBSendSignOff(void)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBSendSignOff();
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBSendLockOn,
///
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose 
TBRC TBSendLockOn(void)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBSendLockOn();
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBSendLockOff,
///
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose 
TBRC TBSendLockOff(void)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBSendLockOff();
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBInitHostAppExit,
///
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose 
TBRC TBInitHostAppExit(void)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBInitHostAppExit();
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBLaneClosed,
///
///	@param bool
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose 
TBRC TBLaneClosed(const bool bEnter)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBLaneClosed(bEnter);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBEnterDOB,
///
///	@param int
///	@param int
/// @param int
///	@param int
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose 
TBRC TBEnterDOB(int nYear, int nMonth, int nDay, int nCustomerAge)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBEnterDOB(nYear, nMonth, nDay, nCustomerAge);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBSendWeight,
///
///	@param long
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose 
TBRC TBSendWeight(long lWeight)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBSendWeight(lWeight);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBEnterTenderState,
///
///	@param bool
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBEnterTenderState( const bool bEnter)
{
    return (pTB ? pTB->TBEnterTenderState( bEnter) : TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBSendSigData,
///
///	@param LPCTSTR
///	@param LPCTSTR
///	@param DWORD
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose 
TBRC TBSendSigData(LPCTSTR CustomerName, LPCTSTR cSigData,  DWORD nLength)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBSendSigData(CustomerName, cSigData, nLength);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBSetTrainingModeState,
///
///	@param bool
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBSetTrainingModeState(bool bTMState)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBSetTrainingModeState(bTMState);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetMessageInfo,
///
///	@param PSMessageInfo
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBGetMessageInfo(PSMessageInfo psMessageInfo)                                                       
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return   pTB->TBGetMessageInfo(psMessageInfo);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetCustomerMessage,
///
///	@param LPTSTR *
///	@param LPTSTR *
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBGetCustomerMessage(LPTSTR* pCustomerMessage, LPTSTR* pCustomerWaveFile)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return pTB->TBGetCustomerMessage(pCustomerMessage, pCustomerWaveFile);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetPrinterError,
///
///	@param LPTSTR *
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBGetPrinterError(LPTSTR* pErrorMessage)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return pTB->TBGetPrinterError(pErrorMessage);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetRewardLineDetails,
///
///	@param PSRewardLineDetails
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBGetRewardLineDetails(PSRewardLineDetails psRewardLineDetails)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return pTB->TBGetRewardLineDetails(psRewardLineDetails);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBEnterMICR,
///
///	@param LPCTSTR
///	@param LPCTSTR
///	@param LPCTSTR
///	@param LPCTSTR
///	@param LPCTSTR
///	@param LPCTSTR
///	@param long
///	@param long
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose 
TBRC TBEnterMICR( LPCTSTR szRawData, 
                 LPCTSTR szTransitNumber, 
                 LPCTSTR szAccountNumber, 
                 LPCTSTR szBankNumber,
                 LPCTSTR szSerialNumber, 
                 LPCTSTR szEPC, 
                 long    lType, 
                 long    lCountryCode)
{
    return (pTB ? pTB->TBEnterMICR( szRawData, szTransitNumber, szAccountNumber, 
        szBankNumber, szSerialNumber, szEPC, lType, lCountryCode) : TB_E_NOTINITIALIZED); 
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetDocEndorsementInfo
///
///	@param long
///	@param LPTSTR
///	@param LPTSTR
///	@param LPTSTR
///	@return long
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose 
long TBGetDocEndorsementInfo( long   *plNumRemaining,
                             LPTSTR *ppszDocDescription, 
                             LPTSTR *ppszDocFront, 
                             LPTSTR *ppszDocBack)
{
    return( pTB ? pTB->TBGetDocEndorsementInfo( plNumRemaining, ppszDocDescription,
        ppszDocFront,ppszDocBack) : TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBEndorsementCompleted,
///
///	@param bool
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" 
APIExpose TBRC TBEndorsementCompleted( bool bSuppressSubsequent)
{
    return (pTB ? pTB->TBEndorsementCompleted( bSuppressSubsequent) : TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBSetCashDrawerState,
///
///	@param long
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose 
TBRC TBSetCashDrawerState(long iState)
{
    return (pTB ? pTB->TBSetCashDrawerState(iState) : TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetExpectedCashDrawerState,
///
///	@param long *
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose 
TBRC TBGetExpectedCashDrawerState(long *plExpectedState)
{
    return (pTB ? pTB->TBGetExpectedCashDrawerState(plExpectedState) : TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBSetSecurityLevel,
///
///	@param long
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose 
TBRC TBSetSecurityLevel(long lSecurityLevel)
{
    return (pTB ? pTB->TBSetSecurityLevel(lSecurityLevel) : TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetExpectedSecurityLevel, 
///
///	@param long *
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose 
TBRC TBGetExpectedSecurityLevel(long* plExpectedState)
{
    return (pTB ? pTB->TBGetExpectedSecurityLevel(plExpectedState) : TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetCurrentSecurityLevel,
///
///	@param long *
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose 
TBRC TBGetCurrentSecurityLevel(long *plExpectedLevel)
{
    return (pTB ? pTB->TBGetCurrentSecurityLevel(plExpectedLevel) : TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetAssistModeStatus,
///
///	@param long *
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBGetAssistModeStatus(long *plAssistModeStatus)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return pTB->TBGetAssistModeStatus(plAssistModeStatus);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetMICRPrompt,
///
///	@param LPTSTR *
/// @return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBGetMICRPrompt(LPTSTR *ppszMICRPrompt)
{
    return (pTB ? pTB->TBGetMICRPrompt( ppszMICRPrompt) : TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetDataNeededView,
///
///	@param PSDataNeededView
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBGetDataNeededView(PSDataNeededView psDataNeededView)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return pTB->TBGetDataNeededView(psDataNeededView);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBSendDataNeeded,
///
///	@param LPCTSTR
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBSendDataNeeded(LPCTSTR szDataNeededDetails)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return pTB->TBSendDataNeeded(szDataNeededDetails);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetLoyaltyCardNumber,
///
///	@param LPTSTR *
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
TBRC TBGetLoyaltyCardNumber(LPTSTR *pszLoyaltyCardNumber)
{
    return (pTB ? pTB->TBGetLoyaltyCardNumber( pszLoyaltyCardNumber) : TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetNextState,
///
///	@param LPCTSTR
///	@param LPCTSTR
///	@param bool *
///	@param LPTSTR
///	@param CDescriptorList
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" 
APIExpose TBRC TBGetNextState(LPCTSTR lpLastStateName, 
                              LPCTSTR lpNextStateName, 
                              bool *pbOverride, 
                              LPTSTR *lpOverrideStateName, 
                              CDescriptorList &dlParameterList)
{
    return (pTB ? pTB->GetNextState(lpLastStateName, lpNextStateName, pbOverride, lpOverrideStateName, dlParameterList): TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBPreprocessSignatureData,
///
///	@param LPCTSTR
///	@param BSTR
///	@param LPCTSTR
///	@param BOOL *
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C"
APIExpose    TBRC  TBPreprocessSignatureData(LPCTSTR lpFileName,
										   BSTR bstrSignatureData, 
										   LPCTSTR lpReceiptType, 
										   BOOL* bProcessData)
{
	return (pTB ? pTB->PreprocessSignatureData( lpFileName, bstrSignatureData, lpReceiptType, bProcessData ): TB_E_NOTINITIALIZED);
}

//////////////////////////////////////////////////////////////////////
///
/// TBGetExtensionStateSetup,
///
///	@param LPTSTR
///	@param LPTSTR
///	@param psPropChangeRequestList
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C" 
APIExpose TBRC	TBGetExtensionStateSetup(LPTSTR szUICtx, LPTSTR szDevCtx, psPropChangeRequestList pExtChangeList)
{
	if (!pTB)
	{
		return TB_E_NOTINITIALIZED;
	}
	else
	{
		return pTB->GetExtensionStateSetup(szUICtx, szDevCtx, pExtChangeList);
	}
}

//////////////////////////////////////////////////////////////////////
///
/// TBNotifyExtensionStateEvent,
///
///	@param psExtStateEventNotifyIn
///	@param psExtStateEventNotifyOut
///	@return TBSTATE
//////////////////////////////////////////////////////////////////////
extern "C" 
APIExpose TBSTATE TBNotifyExtensionStateEvent(psExtStateEventNotifyIn pEventNotifyIn, psExtStateEventNotifyOut pEventNotifyOut)
{
	if (!pTB)
	{
		return TB_IGNORE;
	}
	else
	{
		return pTB->NotifyExtensionStateEvent(pEventNotifyIn, pEventNotifyOut);
	}
}

//////////////////////////////////////////////////////////////////////
///
/// TBSendTBCommand,
///
///	@param long
///	@param va_list
///	@return TBRC
//////////////////////////////////////////////////////////////////////
extern "C"
APIExpose TBRC TBSendTBCommand(long lDataID, va_list pArgs)
{
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return pTB->SendTBCommand(lDataID, pArgs);
    }
}

//////////////////////////////////////////////////////////////////////
///
/// TBValidateHeadCashier
///
///	@param LPCTSTR
///	@param LPCTSTR
///	@return BOOL
//////////////////////////////////////////////////////////////////////
extern "C" APIExpose
BOOL TBValidateHeadCashier( LPCTSTR szCashierID, LPCTSTR szCashierPw )
{
    if (!pTB)
    {
        return false;
    }
    else
    {
        return   pTB->TBValidateHeadCashier(szCashierID, szCashierPw);
    }
}

extern "C" APIExpose
TBRC TBCreateReportEntry(PSCreateReportEntry psCreateReportEntry)                                               
{
	if (!pTB)
	{
		return TB_E_NOTINITIALIZED;
	}
	else
	{
		return	pTB->TBCreateReportEntry(psCreateReportEntry);
	}
}

extern "C" APIExpose
TBRC TBGetCurrentBusinessDate(CString &csBusinessDate)
{
	if (!pTB)
	{
		return TB_E_NOTINITIALIZED;
	}
	else
	{
		return	pTB->TBGetCurrentBusinessDate(csBusinessDate);
	}
}

extern "C" APIExpose
TBRC TBSetBusinessDate(LPCTSTR szSetBusinessDate)
{
	if (!pTB)
	{
		return TB_E_NOTINITIALIZED;
	}
	else
	{
		return	pTB->TBSetBusinessDate(szSetBusinessDate);
	}
}

extern "C" 
APIExpose TBRC TBGetGiftCardBalance(long *plBalance)
{
	if (!pTB)
	{
		return TB_E_NOTINITIALIZED;
	}
	else
	{
		return pTB->TBGetGiftCardBalance(plBalance);
	}
}

extern "C" 
APIExpose BOOL TBDisableTenderButton(TBTENDERTYPE nTenderType)
{
	if (!pTB)
	{
		return FALSE;
	}
	else
	{
		return pTB->TBDisableTenderButton(nTenderType);
	}
}

//Begin RFC 356020
extern "C"
APIExpose TBRC TBEnterMICRWithType(
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
    if (!pTB)
    {
        return TB_E_NOTINITIALIZED;
    }
    else
    {
        return pTB->TBEnterMICRWithType(
            nTenderType,
            szRawData,
            szTransitNumber,
            szAccountNumber,
            szBankNumber,
            szSerialNumber,
            szEPC,
            lType,
            lCountryCode);
    }
}
//End RFC 356020

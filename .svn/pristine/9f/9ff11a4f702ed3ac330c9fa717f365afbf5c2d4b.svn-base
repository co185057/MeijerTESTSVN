//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    SMStateDM.CPP
//
// TITLE:   DM SMState functions, implement all SMState::DM* functions
//
//
// AUTHOR:  Peter Denhoed, Updated DcL-
//
// Change history:
//
// POS22826 Work Request:16656 Name:Matt Condra Date:November 28, 2011
// POS294076 and POS270827 Work Request:76936 Name: Jangala Sunny Date:November 10, 2016
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include "Common.h"
#include "Common.h"
#include "SMState.h"
#include "SMOperatorPasswordState.h"
#include "SMSecOperatorPWState.h"
#include "RCMKeyValue.h"

#include "RemoteMessageAPI.h" // External library ...\platform\RemoteMessage
#include "..\AfxTransBroker\CustomerTransBroker\CustomerTBSharedDefs.h"

#include "NCRPtr.h"
#include "dmx\printer.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmStateDM")

long SMState::m_lCutPercentage = 100;

static SMState * m_pSMState;
UINT_PTR SMState::m_scannerMsgTimer = NULL;
CString SMState::m_scannerMsgTarget = _T("LaunchPadNet.exe");
CString SMState::m_biometMsgTarget = _T("BioDevMgr.exe");
CString SMState::m_currentProcessName = _T("SCOTAPPU.EXE");
CRITICAL_SECTION SMState::m_scannerMsgTargetLock = {0};
int SMState::m_scannerMsgTimeout = 5000;
bool SMState::m_waitingForScannerMsgResponse = false;
bool SMState::m_scannerSharingEnabled = false;
bool SMState::m_scannerLoaded = false;
bool SMState::m_cashTakenInSecState = false;
bool SMState::m_timeoutInTakeChangeState = false;
bool SMState::m_bCashDispensed = false;

CWinThread * SMState::m_remoteMessageListenerThread = NULL;
HANDLE SMState::m_remoteMessageListenerExit = NULL;
CDMPrinterUtils myDMPrinter;
DLLEXPORT CDMPrinterUtils &printerUtils = myDMPrinter;

bool SMState::ReceiptPrintSuccessful()
{
    return SMStateBase::ReceiptPrintSuccessful();
}

void SMState::ResetMLBCValues(void)
{
    SMStateBase::ResetMLBCValues();
}

CString SMState::GetPrinterCharSet()
{
    return SMStateBase::GetPrinterCharSet();
}

void SMState::SendNewEscSeq2Printer()
{
    SMStateBase::SendNewEscSeq2Printer();
}

void SMState::SendOrigEscSeq2Printer()
{
    SMStateBase::SendOrigEscSeq2Printer();
}

long SMState::DMConfigureScaleDisplay(void)
{
    return SMStateBase::DMConfigureScaleDisplay();
}

void SMState::DMConfigureScaleParams(long &divsize, long &constant)
{
    SMStateBase::DMConfigureScaleParams(divsize, constant);
}

CString SMState::ConvertPrinterString(CString csData) const
{
    return SMStateBase::ConvertPrinterString(csData);
}

CString SMState::FormatDeviceInfoInput(long lErrorCode)
{
    return SMStateBase::FormatDeviceInfoInput(lErrorCode);
}

CString SMState::DMGetDeviceErrorHTML(int DeviceClass,
                                      CString& sDeviceModel,
                                      CString& sDeviceSubModel,
                                      int StatusEventId,
                                      long ExtendedResultCode,
                                      int nDataLength,
                                      void* sData, 
                                      bool* bCheckHealth,
                                      int nContext,
                                      CString sLCID)
{
    return SMStateBase::DMGetDeviceErrorHTML(DeviceClass, sDeviceModel, sDeviceSubModel, StatusEventId,
        ExtendedResultCode, nDataLength, sData, bCheckHealth, nContext, sLCID);
}

bool SMState::getNextHopperStatusLine(CString& currencyMap,
                                       CString& hopperStatusList,
                                       CString& hopperStatusLine,
                                       bool bDenominationOnly)
{
    return SMStateBase::getNextHopperStatusLine(currencyMap, hopperStatusList, hopperStatusLine, bDenominationOnly);
}

void SMState::getHopperStatus(const CString denominationCode,
                               CString& denominationStatus)
{
    SMStateBase::getHopperStatus(denominationCode, denominationStatus);
}

CString SMState::getHopperStatusString(void)
{
    return SMStateBase::getHopperStatusString();
}

CString SMState::getHopperDenominations(long& lErrorCode)
{
    return SMStateBase::getHopperDenominations(lErrorCode);
}

bool SMState::isOnlyHopperSubsApprovalRequired(void)
{
    return SMStateBase::isOnlyHopperSubsApprovalRequired();
}

SMStateBase * SMState::DMParse( const MessageElement* me)
{
	trace(L6, _T("+SMState::DMParse"));
    SMStateBase * pRet = STATE_NULL;
    // Intercept any custom DM messages for biometric data
    if( (me->dmInfo.Msg == DM_DATA) &&
        (me->dmInfo.Class == DMCLASS_REMOTEMESSAGE) )
    {
        switch(me->dmInfo.DeviceId)
        {
        case DM_REMOTEMESSAGE_RELEASESCANNER:
            {
                ProcessReleaseScannerRequest();
            }
            break;
        case DM_REMOTEMESSAGE_SCANNERAVAILABLE:
            {
                ProcessResponseScannerAvailable();
            }
            break;
        case DM_REMOTEMESSAGE_SCANNEROPENFAILED:
            {
                ProcessResponseScannerOpenFailed();
            }
            break;
        case DM_REMOTEMESSAGE_PROCESSBIOMETRICDATA:
            {
                if( me->dmInfo.lCodeOrLength )
                {
                    CString encodedData = (LPTSTR)me->dmInfo.vDPtr;
                    delete [] me->dmInfo.vDPtr;
                    pRet = ProcessBiometricData(encodedData);
                }
            }
            break;
        default:
            {
				trace(L6, _T("Unknown DM_REMOTEMESSAGE custom event %d"), me->dmInfo.DeviceId);
            }
        };
    }
    else
    {
        if ( ((me->dmInfo.Msg == DM_STATUS) && (me->dmInfo.Class == DMCLASS_CASHACCEPTOR) && (me->dmInfo.lCodeOrLength == DM_CASHACCEPTOR_STATUS_INSERTED)) ||
			((me->dmInfo.Msg == DM_DATA) && (me->dmInfo.Class == DMCLASS_COINACCEPTOR) && (me->dmInfo.lCodeOrLength > 0))
			)
        {
            // POS22826: Cash/Coin has been inserted but not processed yet. It cannot be processed while
            // in Assist Mode, so make sure RAP Assist Mode button is disabled. This also allows the
            // customer and attendant to see cash amounts on the screen if Assist Mode is entered later.
            // (Note: There is still a small window here for the attendant to press the RAP Assist
            // Mode button prior to the button being disabled. We will cross that bridge if it is a 
            // problem in the future.)
            trace(L6, _T("%s inserted; Disabling RAP Assist Mode button"), ((me->dmInfo.Class == DMCLASS_CASHACCEPTOR)?_T("Cash"):_T("Coin")));
            ra.RequestEnableAssistMode(false);
        }
		
        pRet = SMStateBase::DMParse(me);
    }
	
    //
    // DM errors for FastLane devices should probably all be suppressed
    // while in remote assist mode. This is not currently the case and 
    // RemoteSystemMessage will always pop for FastLane and RAP device errors
    // Following is some device specific code to suppress the system msgs although
    // a more generic approach would be best, maybe even an attribute 
    // during device configuration to set 'suppress this device's errors during AM'.
    //
    if( 
        ((me->dmInfo.Msg == DM_ERROR) || (me->dmInfo.Msg == DM_STATUS)) && 
        !fStateAssist && 
        ps.RemoteMode() &&
        (pRet != STATE_NULL) 
		)
    {
        switch(me->dmInfo.Class)
        {
        case DMCLASS_SIGCAP:
        case DMCLASS_KEYLOCK:
        case DMCLASS_CASHACCEPTOR:
        case DMCLASS_COINACCEPTOR:
        case DMCLASS_CASHRECYCLER:
        case DMCLASS_CASHCHANGER:
        case DMCLASS_SCALE:
        case DMCLASS_MOTION_SENSOR:
        case DMCLASS_EASDEACTIVATOR:
        case DMCLASS_MISC:
            {
                CString csState = pRet->GetRuntimeClass()->m_lpszClassName; 
                if( -1 != csState.Find(_T("SystemMessage")) )
                {
                    fStateAssist = true;
                    trace(L6, _T("Suppressing FastLane device system messages while in remote assist mode."));
                }
            }
        }
    }
    trace(L6, _T("-SMState::DMParse"));
    return pRet;
    //return SMStateBase::DMParse(me);
}

SMStateBase * SMState::DMSetScaleWeight(bool &fCheckHealth)
{
    return SMStateBase::DMSetScaleWeight(fCheckHealth);
}

bool SMState::DMHookNotifyTenderOver(void)
{
    return SMStateBase::DMHookNotifyTenderOver();
}

bool SMState::DMCashRecyclerShouldReject(const CString &csStateName)
{
    return SMStateBase::DMCashRecyclerShouldReject(csStateName);
}

bool SMState::DMCashRecyclerIsTenderOver(const CString &csStateName)
{
    return SMStateBase::DMCashRecyclerIsTenderOver(csStateName);
}

bool SMState::DMUnlockNoteCover(void)
{
    return SMStateBase::DMUnlockNoteCover();
}

long SMState::DMUnInitialize(void)
{
    trace(L6,_T("+SMState::DMUnInitialize"));
	
    m_waitingForScannerMsgResponse = false;
    trace(L6, _T("Signaling Remote Message Listener thread to end and waiting for it to terminate."));
    SetEvent(m_remoteMessageListenerExit);
	
    if( m_scannerSharingEnabled )
    {
        ClearScannerMsgTimer();
        DeleteCriticalSection(&m_scannerMsgTargetLock);
        CString msg;
        msg.Format(_T("%s=%s;"), eventRapOperation, ExitRemoteMessageListener);
        trace(L6, _T("SendProcessMessage(%s, %s)"), m_scannerMsgTarget, msg);
        SendProcessMessage(m_currentProcessName, msg);
    }
    JoinThreadOrTerminate(m_remoteMessageListenerThread, 3000);
    m_remoteMessageListenerThread = NULL;
    CloseHandle(m_remoteMessageListenerExit);
    m_remoteMessageListenerExit = NULL;
	
    m_pSMState = NULL;
    long rc = SMStateBase::DMUnInitialize();
    m_scannerLoaded = false;
    trace(L6,_T("-SMState::DMUnInitialize"));
    return rc;
}

long SMState::DMInitialize(void)
{
    trace(L6,_T("+SMState::DMInitialize"));
	
    // Initialize the scanner sharing variables
    m_scannerMsgTimer = NULL;
    m_scannerMsgTarget = _T("LaunchPadNet.exe");
    m_biometMsgTarget = _T("BioDevMgr.exe");
    m_scannerMsgTimeout = 5000;
    m_waitingForScannerMsgResponse = false;
    // device is opened and claimed in base DMInitialize
    m_scannerLoaded = true;  
    m_pSMState = this;
	
    // Get the current process name
    TCHAR procName[MAX_PATH];
    if( GetModuleFileName(NULL, procName, MAX_PATH-1) )
    {
        m_currentProcessName = procName;
        // Remove path from filename
        int pos = m_currentProcessName.ReverseFind(_T('\\'));
        if( pos > -1 )
        {
            m_currentProcessName = m_currentProcessName.Mid(pos+1);
        }
    }
    else
    {
        trace(L0, _T("Failed to retrieve the current process name."));
    }
	
    // Initialize the RemoteMessage listener
    m_remoteMessageListenerExit = CreateEvent(NULL, FALSE, FALSE, NULL);
    ASSERT(m_remoteMessageListenerExit);
	
    m_remoteMessageListenerThread = AfxBeginThread((AFX_THREADPROC)RemoteMessageListenerProc, (LPVOID)this, THREAD_PRIORITY_NORMAL,0,0,NULL); 
    ASSERT(m_remoteMessageListenerThread);
	
    // Determine whether we're enabling scanner sharing
    DetectAndEnableScannerSharing();
	
    if( m_scannerSharingEnabled )
    {
        InitializeCriticalSection(&m_scannerMsgTargetLock);
		
        if( IsScannerMsgTargetRunning() )
        {
            trace(L6, _T("Tell LaunchPad to let go of the scanner if it has it open..."));
            CString msg;
            msg.Format(_T("%s=%s;"), eventRapOperation, ReleaseScanner);
            trace(L6, _T("SendProcessMessage(%s, %s)"), m_scannerMsgTarget, msg);
            SendProcessMessage(m_scannerMsgTarget, msg);
        }
    }
	
    trace(L6,_T("-SMState::DMInitialize"));
	
    long rc = SMStateBase::DMInitialize();
	
    return rc;
}

CString SMState::DMGetDevicesInError(void)
{
    return SMStateBase::DMGetDevicesInError();
}

SMStateBase * SMState::DMScanner(void)
{
    return SMStateBase::DMScanner();
}

SMStateBase * SMState::DMScannerEncrypted(void)
{
    return SMStateBase::DMScannerEncrypted();
}

SMStateBase * SMState::DMReceiptPrinter(DWORD dwEvent)
{
    return SMStateBase::DMReceiptPrinter(dwEvent);
}

SMStateBase * SMState::DMJournalPrinter(DWORD dwEvent)
{
    return SMStateBase::DMJournalPrinter(dwEvent);
}

SMStateBase * SMState::DMSlipPrinter(DWORD dwEvent)
{
    return SMStateBase::DMSlipPrinter(dwEvent);
}

SMStateBase * SMState::DMMICR(DWORD dwEvent)
{
    return SMStateBase::DMMICR(dwEvent);
}

SMStateBase * SMState::DMKeyLock(DWORD dwEvent)
{
    return SMStateBase::DMKeyLock(dwEvent);
}

SMStateBase * SMState::DMCashDrawer(DWORD dwEvent)
{
    return SMStateBase::DMCashDrawer(dwEvent);
}

SMStateBase * SMState::DMAcceptor(void)
{
    return SMStateBase::DMAcceptor();
}

SMStateBase * SMState::DMCardReader(void)
{
    return SMStateBase::DMCardReader();
}

SMStateBase * SMState::DMCardRemoved(void)
{
    return SMStateBase::DMCardRemoved();
}

SMStateBase * SMState::DMCashRemoved(void)
{
    return SMStateBase::DMCashRemoved();
}

SMStateBase * SMState::DMDispenserPositionEvent( long lStat)
{
    return SMStateBase::DMDispenserPositionEvent(lStat);
}

void SMState::DMSlipPrinterPrint(LPCTSTR szPrintData)
{
    SMStateBase::DMSlipPrinterPrint(szPrintData);
}

long SMState::DMSlipPrinterBarcode(CString csBarcode,long lBCType, 
      long lBCHeight, long lBCAlign, bool fPrintNumbers)
{
    return SMStateBase::DMSlipPrinterBarcode(csBarcode, lBCType, 
       lBCHeight, lBCAlign, fPrintNumbers);
}

long SMState::DMSlipPrinterPrintBitmap(CString csBitmap, long lBmpAlign)
{
    return SMStateBase::DMSlipPrinterPrintBitmap(csBitmap, lBmpAlign);
}

bool SMState::DMSlipPrinterIsSlipInserted()
{
    return SMStateBase::DMSlipPrinterIsSlipInserted();
}

bool SMState::DMSlipPrinterIsSlipNearEnd()
{
    return SMStateBase::DMSlipPrinterIsSlipNearEnd();
}

void SMState::DMSlipPrinterInsertSlip(bool bInsert)
{
    SMStateBase::DMSlipPrinterInsertSlip(bInsert);
}

long SMState::DMSlipPrinterHome()
{
    return SMStateBase::DMSlipPrinterHome();
}

long SMState::DMSlipPrinterFeedLines(int iLines)
{
    return SMStateBase::DMSlipPrinterFeedLines(iLines);
}

void SMState::PrintEndorsementChit(LPCTSTR szEndorsement) const
{
    trace(L6, _T("+SMState::PrintEndorsementChit()"));
    
    // clear the current endorsement since we're printing it out
    fInEndorsementState = ENDORSE_NONE;

	CString csDataToPrint = szEndorsement;

	CString csRotate = _T("\x1B\x12");

	if ( -1 != csDataToPrint.Find(csRotate) )
	{
		// This is rotated slip data.  Need TB to supply non-rotated.
		long lData = -42;
        LPTSTR szSlipDocumentDescription;
        LPTSTR szSlipDocumentFront;
        LPTSTR szSlipDocumentBack;

        szSlipDocumentDescription = NULL;
        szSlipDocumentFront = NULL;
    	szSlipDocumentBack = NULL;

		TBGetDocEndorsementInfo(&lData,
        &szSlipDocumentDescription,  &szSlipDocumentFront, &szSlipDocumentBack);
		
		csDataToPrint = szSlipDocumentBack;
        delete szSlipDocumentDescription;
	    delete szSlipDocumentFront;
    	delete szSlipDocumentBack;
	}
	else
	{
        // Chit formatting to maximize use of the receipt. Preserving double width formatting 
        // and alignment. Reformatting done in two passes.  The first pass determines which 
        // endorsement line is the closest to the left margin and the second pass will proceed
        // to shift the entire endorsement that many spaces to the left, taking into consideration
        // double wide character spacing.
        // New parser supports both EBT decline chits, regular check & traveler/rebate check chits successfully.
        
        const int MAXMARGIN = 45;
        const TCHAR cNewLine = _T('\x0A');
        const TCHAR cSpace = _T(' ');
		LPCTSTR szDblWide = _T("\x1B|2C");
        LPCTSTR szNormal = _T("\x1B|N");
        bool bDbl = false;


        //
        //
        // (1) Run a first pass through the receipt to determine the line that has the
        //     least amount leading whitespaces. This is the amount of spaces we'll be shifting left.
        //
        //
        int iMargin = MAXMARGIN;
        int iStart = 0;
        int iEnd = csDataToPrint.Find(cNewLine, iStart);
        while( -1 != iEnd )
        {
            // get the line data
            CString csLine = csDataToPrint.Mid(iStart, iEnd-iStart);
            
            // Find the first instance of a whitespace, dblwide and normal (if any)
            int iDbl = csLine.Find(szDblWide);
            int iNorm = csLine.Find(szNormal);
            
            // Count the number of whitespaces before characters start
            int iCount = 0;
            int iSpace = csLine.Find(cSpace);

            while( -1 != iSpace )
            {
                // is there a double wide formatter before the space?
                if( !bDbl && (-1 != iDbl) && (iDbl < iSpace) )
                {
                    bDbl = true;
                }
                // is there a normal string formatter before the space and after the double wide?
                if( bDbl && (-1 != iNorm) && (iNorm > iDbl) && (iNorm < iSpace) )
                {
                    bDbl = false;
                }

                if( bDbl )
                {
                    iCount+=2;
                }
                else
                {
                    iCount++;
                }

                // process next char
                if( csLine.GetLength() > (iSpace+1) )
                {
                    // is the next character a whitespace?
                    if( csLine.Mid(iSpace+1,1) == cSpace )
                    {
                        iSpace++;
                    }
                    else
                    {
                        iSpace = -1;
                    }
                }
                else
                {
                    // no characters to print in this line 
                    // so don't consider it
                    iCount = MAXMARGIN;
                    iSpace = -1;
                }
            }

            // do we have a new margin?
            if( iCount < iMargin )
            {
                iMargin = iCount;
            }
            iStart = iEnd+1;
            iEnd = csDataToPrint.Find(cNewLine, iStart);
        }

        //
        //
        // (2) if we have a valid new margin proceed to truncate the whitespace characters
        //     shifting every line left
        //
        //
        if( (0 < iMargin) && (iMargin < MAXMARGIN) )
        {
            bDbl = false;
            iStart = 0;
            iEnd = csDataToPrint.Find(cNewLine, iStart);
            while( -1 != iEnd )
            {
                // get the line data
                CString csLine = csDataToPrint.Mid(iStart, iEnd-iStart);

                // Find the first instance of a whitespace, dblwide and normal (if any)
                int iDbl = csLine.Find(szDblWide);
                int iNorm = csLine.Find(szNormal);
                int iSpace = csLine.Find(cSpace);


                // is there a double wide formatter before the space?
                if( !bDbl && (-1 != iDbl) && (iDbl < iSpace) )
                {
                    bDbl = true;
                }
                // is there a normal string formatter before the space and after the double wide?
                if( bDbl && (-1 != iNorm) && (iNorm > iDbl) && (iNorm < iSpace) )
                {
                    bDbl = false;
                }

                int iDelCnt = bDbl ? iMargin/2 : iMargin;

                // don't bother if it's an empty string line
                if( !((csLine.GetLength() - iSpace) < iDelCnt) )
                {
                    csDataToPrint.Delete(iStart + iSpace, iDelCnt);
                    // move the end pointer back the number of spaces
                    iEnd -= iDelCnt;
                }

                iStart = iEnd+1;
                iEnd = csDataToPrint.Find(cNewLine, iStart);
            }
        }
    }

	SMStateBase::PrintEndorsementChit( csDataToPrint );

    trace(L6, _T("-SMState::PrintEndorsementChit()"));
}

bool SMState::InRemoteDeviceMode(void) const
{
    return SMStateBase::InRemoteDeviceMode();
}

bool SMState::IsHopperSubstitutionUsed(bool & bEmpty, bool &bChuteJammed)
{
    return SMStateBase::IsHopperSubstitutionUsed(bEmpty, bChuteJammed);
}

bool SMState::DMWasCashTaken(void)
{
    return SMStateBase::DMWasCashTaken();
}

void SMState::DMSetCashTaken(void)
{
    SMStateBase::DMSetCashTaken();
}

SMStateBase * SMState::DMScale(void)
{
    return SMStateBase::DMScale();
}

void SMState::DMReceiptReprint(void)
{
    SMStateBase::DMReceiptReprint();
}

void SMState::DMJournalReprint(void)
{
    SMStateBase::DMJournalReprint();
}

CString SMState::GetGLOBALAddThe128Prefix()
{
    return SMStateBase::GetGLOBALAddThe128Prefix();
}

void SMState::DMReceiptLine(const CString& csPrintLine)
{
    CString csDesc = dm.GetDescription(DMCLASS_PRINTER,0);
	CString strOptionName = _T("ReceiptPartialCut");
	CString strOptionValue;
	SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &strOptionValue);

    if (csDesc.Find(_T("NCR K5xx Kiosk POS Printer")) == 0 && strOptionValue != _T("0"))
    {
        trace(L6, _T("Receipt Description:[%s]"),csDesc);
        SetCutPercentage(90);
        CString csPartialCutMode = _T("\x1D\x55");
        dm.ReceiptLine(csPartialCutMode);
		
        CString csReceiptCut = _T("\x1B|P");
        CString csNewCut = _T("\x1B|90P");
        CString csNewPrintLine = csPrintLine;
        csNewPrintLine.Replace(csReceiptCut, csNewCut);
        SMStateBase::DMReceiptLine(csNewPrintLine);
    }
    else
    {
        SMStateBase::DMReceiptLine(csPrintLine);
    }
}

void SMState::DMReceiptCut()
{
    trace(L6,_T("+ReceiptCut - m_lCutPercentage = %d"), m_lCutPercentage);
    _TCHAR msg[] = T_ID _T("::ReceiptCut");		
	
	CString strOptionName = _T("ReceiptPartialCut");
	CString strOptionValue;
	SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &strOptionValue);

    if (m_lCutPercentage < 100 && strOptionValue != _T("0"))
    {
        CPrintStation* pPrinter = dm.GetPrintStation( DMCLASS_RECEIPT );
        pPrinter->CutPaper( m_lCutPercentage );
        ThrowDmException( DMCLASS_RECEIPT, pPrinter->GetResultCode(), pPrinter->GetResultCodeExtended() );        
    }
    else
    {
        SMStateBase::DMReceiptCut();
    }	    
}

void SMState::DMReceiptBitMap(const CString csBmpFile)
{
    SMStateBase::DMReceiptBitMap(csBmpFile);
}

void SMState::DMReceiptPrintBarcode(LPCTSTR szBarcode, long BCType)
{
    SMStateBase::DMReceiptPrintBarcode(szBarcode, BCType);
}

void SMState::DMJournalLine(const CString& csPrintLine)
{
    SMStateBase::DMJournalLine(csPrintLine);
}

void SMState::DMReceiptPrintBarcode(CString csBarcode,
                                    long     lBCType, 
                                    long     lBCHeight, 
                                    long     lBCAlign , 
                                    bool     fPrintNumbers, 
                                    int      nDeviceClass )
{
    SMStateBase::DMReceiptPrintBarcode(csBarcode, lBCType, lBCHeight, lBCAlign , fPrintNumbers, nDeviceClass );
}

SMStateBase * SMState::DMHandleStartupError(void)
{
    return SMStateBase::DMHandleStartupError();
}

long SMState::DMPrintLine(int nDeviceClass,  const CString& csPrintLine)
{
    return SMStateBase::DMPrintLine(nDeviceClass,  csPrintLine);
}

long SMState::DMPrintBitmap(CString csBitmap, long lBmpAlign, int nDeviceClass)
{
    return SMStateBase::DMPrintBitmap(csBitmap, lBmpAlign, nDeviceClass );
}

long SMState::DMPrintBarcode(CString csBarcode,long lBCType, long lBCHeight, long lBCAlign, 
        bool fPrintNumbers, int nDeviceClass)
{
    return SMStateBase::DMPrintBarcode(csBarcode,lBCType, lBCHeight, lBCAlign, fPrintNumbers, nDeviceClass);
}

long SMState::DMProcessPrintLine(int nDeviceClass, const CString& csPrintLine)
{
    return SMStateBase::DMProcessPrintLine(nDeviceClass, csPrintLine);
}

long SMState::DMProcessPrintTokens(int nDeviceClass, CString& CurrentPrintLine)
{
    return SMStateBase::DMProcessPrintTokens(nDeviceClass, CurrentPrintLine);
}

long SMState::DMDispenseChange(const long lAmt, CString& csCashCountsStatus, long *lDispenseErrorCode)
{
    return SMStateBase::DMDispenseChange(lAmt,  csCashCountsStatus, lDispenseErrorCode);
}

long SMState::DMDispenseChange(CString& csChangeInDenomination, long *lDispenseErrorCode)
{
    return SMStateBase::DMDispenseChange(csChangeInDenomination, lDispenseErrorCode);
}

long SMState::DMReportDispensedCashList()
{
    return SMStateBase::DMReportDispensedCashList();
}

long SMState::DMPrintCashManagementReceipt( CArray<int,int>& caValueList, CArray<int,int>& caChangeInCount )
{
    return SMStateBase::DMPrintCashManagementReceipt( caValueList, caChangeInCount );
}

long SMState::DMPrintCashManagementReceipt( CArray<CString,CString>& m_caCashAcceptorDenomList, 
                                           CArray<CString,CString>& m_caCoinAcceptorDenomList,  
                                           CArray<CString,CString>& m_caCashDispenserDenomList, 
                                           CArray<CString,CString>& m_caCoinDispenserDenomList,
                                           CArray<int,int>& m_caCashAcceptorCountList, 
                                           CArray<int,int>& m_caCoinAcceptorCountList,  
                                           CArray<int,int>& m_caCashDispenserCountList, 
                                           CArray<int,int>& m_caCoinDispenserCountList,
                                           CArray<int,int>& m_caCashAcceptorChangeInCountList,
                                           CArray<int,int>& m_caCoinAcceptorChangeInCountList,
                                           CArray<int,int>& m_caCashDispenserChangeInCountList,
                                           CArray<int,int>& m_caCoinDispenserChangeInCountList )
{
    return SMStateBase::DMPrintCashManagementReceipt( m_caCashAcceptorDenomList, m_caCoinAcceptorDenomList,
        m_caCashDispenserDenomList, m_caCoinDispenserDenomList, m_caCashAcceptorCountList, m_caCoinAcceptorCountList,
        m_caCashDispenserCountList, m_caCoinDispenserCountList, m_caCashAcceptorChangeInCountList, m_caCoinAcceptorChangeInCountList,
        m_caCashDispenserChangeInCountList, m_caCoinDispenserChangeInCountList );
}

CString SMState::DMPrintCashmanagementReceiptByDevice( CArray<CString,CString>& m_caDenomList,
                                                   CArray<int,int>& m_caCountList,
                                                   CArray<int,int>& m_caChangeInCountList)
{
    return SMStateBase::DMPrintCashmanagementReceiptByDevice(m_caDenomList, m_caCountList, m_caChangeInCountList);
}

long SMState::SendPrintDataToTB(CString *csAllData)
{
    return SMStateBase::SendPrintDataToTB(csAllData);
}

CString SMState::Justify( const CString csData )
{
    return SMStateBase::Justify( csData );
}

CString SMState::PositionData( CString csFirst, CString csMiddle, CString csLast )
{
    return SMStateBase::PositionData( csFirst, csMiddle, csLast );
}

CString SMState::ConvertValueToString( const int nDenom, bool bTrimDecimal )
{
    return SMStateBase::ConvertValueToString( nDenom, bTrimDecimal );
}

long SMState::DMSayAmount(const long lAmt)
{
    CString csProperty = _T("SuppressAmountAudio");
	CString csValue;
	SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csProperty, &csValue);
	
	csProperty = _T("IsResumeTrxInProgress");
	CString strPropValue;
	SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csProperty, &strPropValue);
	
	if (csValue == _T("true") || strPropValue == _T("1"))
	{
		return 0;
	}
	
	//fix for TAR#306843
	bool tempfStateSayPrices = co.fStateSayPrices;
	CString csPropNegativeMedia = _T("PropNegativeMedia");
	CString csNegativeMedia;
	SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csPropNegativeMedia, &csNegativeMedia);
	
	if(csNegativeMedia == _T("1"))
	{
		csNegativeMedia = _T("0");
		SendTBCommand(TBCMD_SET_HOOKPROPERTY, &csPropNegativeMedia, &csNegativeMedia);
		co.fStateSayPrices = false;
	}
	//
	
	//fix for TAR#306843
	long tempDMSayAmount = SMStateBase::DMSayAmount(lAmt);
	co.fStateSayPrices = tempfStateSayPrices;
	return tempDMSayAmount;
	//end of fix
}

long SMState::DMSayPhrase(LPCTSTR szWave)
{
	// start CRD 125377 don't display thank you if transaction canceled
	if 	((((m_bShopAndDropEnabled) && (m_bSNDmPerksCancel))	||
		((m_bShopAndDropEnabled) && (m_bSNDxCancelTrans)))		&&
		(_T("ThankYou") == ps.GetCurrentContext()))
    {
        trace( L6, _T("DMSaySecurity::Thank You don't due the audio"));
		return 0;
	}
    else
    {
    return SMStateBase::DMSayPhrase(szWave);
	}
	// end CRD 125377
}

long SMState::DMSayPhrase(const int nMsg)
{
	// start CRD 125377 don't display thank you if transaction canceled
	if 	((((m_bShopAndDropEnabled) && (m_bSNDmPerksCancel))	||
		((m_bShopAndDropEnabled) && (m_bSNDxCancelTrans)))		&&
		(_T("ThankYou") == ps.GetCurrentContext()))
    {
        trace( L6, _T("DMSaySecurity::Thank You don't due the audio"));
		return 0;
	}
    else
    {
    return SMStateBase::DMSayPhrase(nMsg);
	}
	// end CRD 125377
}

long SMState::DMSaySecurity(const long nMsg, LPCTSTR szWave, HANDLE hWaitEvent/*=0*/)
{
	// start CRD 125377 don't display thank you if transaction canceled
	if 	((((m_bShopAndDropEnabled) && (m_bSNDmPerksCancel))	||
		 ((m_bShopAndDropEnabled) && (m_bSNDxCancelTrans)))		&&
		 (_T("ThankYou") == ps.GetCurrentContext()))
    {
        trace( L6, _T("DMSaySecurity::Thank You don't due the audio"));
		return 0;
	}
    else
    {
    return SMStateBase::DMSaySecurity(nMsg, szWave, hWaitEvent);
	}
	// end CRD 125377
}

long SMState::DMCashAcceptorEnable(LPCTSTR szList) 
{
    return SMStateBase::DMCashAcceptorEnable(szList);
}

long SMState::DMAcceptorLight(const int lightState)
{
    return SMStateBase::DMAcceptorLight(lightState);
}

long SMState::DMCoinAcceptorLight(const int lightState)
{
    return SMStateBase::DMCoinAcceptorLight(lightState);
}

long SMState::DMCashAcceptorAccept(void)
{
    return SMStateBase::DMCashAcceptorAccept();
}

long SMState::DMCashAcceptorReject(void)
{
    return SMStateBase::DMCashAcceptorReject();
}

long SMState::DMCashAcceptorDisable(void)
{
    return SMStateBase::DMCashAcceptorDisable();
}

long SMState::DMCoinAcceptorEnable(void)
{
    return SMStateBase::DMCoinAcceptorEnable();
}

long SMState::DMCoinAcceptorAccept(void)
{
    return SMStateBase::DMCoinAcceptorAccept();
}

long SMState::DMCoinAcceptorReject(void)
{
    return SMStateBase::DMCoinAcceptorReject();
}

long SMState::DMCoinAcceptorDisable(void)
{
    return SMStateBase::DMCoinAcceptorDisable();
}

SMStateBase * SMState::DisplayCashDeviceError(void)
{
    return SMStateBase::DisplayCashDeviceError();
}

SMStateBase * SMState::GetUnloadedCashAcceptorError(void)
{
    return SMStateBase::GetUnloadedCashAcceptorError();
}

SMStateBase * SMState::GetUnloadedCoinAcceptorError(void)
{
    return SMStateBase::GetUnloadedCoinAcceptorError();
}

SMStateBase * SMState::GetUnloadedCashChangerError(void)
{
    return SMStateBase::GetUnloadedCashChangerError();
}

long SMState::DMScaleEnable(void)
{
    return SMStateBase::DMScaleEnable();
}

long SMState::DMScaleDisable(void)
{
    return SMStateBase::DMScaleDisable();
}

long SMState::DMMSREnable(void)
{
    return SMStateBase::DMMSREnable();
}

long SMState::DMMSRLight(const int lightState)
{
    return SMStateBase::DMMSRLight(lightState);
}

long SMState::DMMSREject(void)
{
    return SMStateBase::DMMSREject();
}

long SMState::DMMSRDisable(void)
{
    return SMStateBase::DMMSRDisable();
}

long SMState::DMMSRCapture(void)
{
    return SMStateBase::DMMSRCapture();
}

long SMState::DMCouponLight(const int lightState)
{
    return SMStateBase::DMCouponLight(lightState);
}

long SMState::DMScannerEnable(void)
{
    long rc = 0;
    trace(L6,_T("+SMState::DMScannerEnable()"));
    m_bCouldBeScannedLoyaltyCard = false;
    
    // Sharing aware scanner
    if( !dm.IsScannerEnabled() )
    {
        // Is the scanner device open? If it is, simply enable it.
        if( m_scannerLoaded )
        {
            rc = dm.ScannerEnable();
        }
        else // DEVICE IS NOT OPEN 
        {
            // Device is not open, request access to the scanner
            // function will determine the required action to take
            rc = RequestAccessToScanner();
        }
    }
    else
    {
        trace(L6, _T("The scanner is already open and enabled."));
    }
    trace(L6,_T("-SMState::DMScannerEnable() = %d"), rc);
    return rc;
}

long SMState::DMScannerDisable(void)
{
    return SMStateBase::DMScannerDisable();
}

void SMState::DMTakeawayBeltNotify(long Event)
{
    SMStateBase::DMTakeawayBeltNotify( Event);
}

void SMState::DMTakeawayBeltReset()
{
    SMStateBase::DMTakeawayBeltReset();
}

long SMState::DMEncryptorEnable(void)
{
    return SMStateBase::DMEncryptorEnable();
}

long SMState::DMEncryptorMaster(void)
{
    return SMStateBase::DMEncryptorMaster();
}

long SMState::DMEncryptorSetup(LPCTSTR szWorkingKey, 
                               TBPINBLOCKFORMAT nPinBlockFormat, 
                               TBPINPADSYSTEM   nPinpadSystem,
                               TBPINENCODING    nPinBlockEncoding)
{
    return SMStateBase::DMEncryptorSetup(szWorkingKey, nPinBlockFormat, nPinpadSystem, nPinBlockEncoding);
}

long SMState::DMEncryptorKeyTest()
{
    return SMStateBase::DMEncryptorKeyTest();
}

CString SMState::DMEncryptorGetPIN(void)
{
    return SMStateBase::DMEncryptorGetPIN();
}

long SMState::DMEncryptorDisable(void)
{
    return SMStateBase::DMEncryptorDisable();
}

long SMState::DMEncryptorCancelSecureMode(void)
{
    return SMStateBase::DMEncryptorCancelSecureMode();
}

long SMState::DMPrintVersion(void)
{
    return SMStateBase::DMPrintVersion();
}

void SMState::DMCashChangerPurgeBills(void)
{
    SMStateBase::DMCashChangerPurgeBills();
}

long SMState::DMCheckHealth(long nDevClass)
{
    return SMStateBase::DMCheckHealth(nDevClass);
}

CString SMState::DMCheckCashCounts(const bool fAdjust, CString& csHighCurrencies, CString& csCoinDispenserAlert, CString& csCashDispenserAlert)
{
    return SMStateBase::DMCheckCashCounts(fAdjust, csHighCurrencies, csCoinDispenserAlert, csCashDispenserAlert);
}

void SMState::BillOrCoinErrorStatus(bool *pCoin, bool* pBill)
{
    SMStateBase::BillOrCoinErrorStatus(pCoin, pBill);
}

CString SMState::CashChangerErrorMessage(long lChangeDue, CString addMsg)
{
    return SMStateBase::CashChangerErrorMessage(lChangeDue, addMsg);
}

CString SMState::GetCashChangerError(void)
{
    return SMStateBase::GetCashChangerError();
}

SMStateBase * SMState::DMEncryptorPinIsTooShort(void)
{
    return SMStateBase::DMEncryptorPinIsTooShort();
}

SMStateBase * SMState::DMEncryptorSecured(void)
{
    return SMStateBase::DMEncryptorSecured();
}

SMStateBase * SMState::DMEncryptorCanceled(void)
{
    return SMStateBase::DMEncryptorCanceled();
}

SMStateBase * SMState::DMEncryptorCompleted(void)
{
    return SMStateBase::DMEncryptorCompleted();
}

SMStateBase * SMState::DMSigcapStarted()
{
    return SMStateBase::DMSigcapStarted();
}

SMStateBase * SMState::DMSigcapEnded()
{
    return SMStateBase::DMSigcapEnded();
}

SMStateBase * SMState::DMTakeawayReadyForItem()
{
    return SMStateBase::DMTakeawayReadyForItem();
}

SMStateBase * SMState::DMTakeawayBagAreaBackup(bool isBackedUp)
{
    return SMStateBase::DMTakeawayBagAreaBackup(isBackedUp);
}

SMStateBase * SMState::DMTakeawaySafetyCoverOpen(bool isOpen)
{
    return SMStateBase::DMTakeawaySafetyCoverOpen(isOpen);
}

void SMState::DMReceiptRequestCompleted(long lRequestId)
{
    SMStateBase::DMReceiptRequestCompleted(lRequestId);
}

void SMState::DMJournalRequestCompleted(long lRequestId)
{
    SMStateBase::DMJournalRequestCompleted(lRequestId);
}

SMStateBase * SMState::DMSigcapFailure()
{
    return SMStateBase::DMSigcapFailure();
}

SMStateBase * SMState::DMCashChangerError(int nDMLastEvent)
{
    return SMStateBase::DMCashChangerError(nDMLastEvent);
}

void SMState::getPrinterStatusLine(const long lCode, CString& printerStatusLine)
{
    SMStateBase::getPrinterStatusLine(lCode, printerStatusLine);
}

void SMState::getCashAcceptorStatusLine(const long lCode, CString& printerStatusLine)
{
    SMStateBase::getCashAcceptorStatusLine(lCode, printerStatusLine);
}

void SMState::getCoinAcceptorStatusLine(const long lCode, CString& printerStatusLine)
{
    SMStateBase::getCoinAcceptorStatusLine(lCode, printerStatusLine);
}

void SMState::ProcessPrinterExtendedResultCode(long &ExtendedResultCode)
{
    SMStateBase::ProcessPrinterExtendedResultCode(ExtendedResultCode);
}

void SMState::AddMLBCRecord(MultiLabelBarcodeFormatRecord *pMLBFRecord)
{
    SMStateBase::AddMLBCRecord(pMLBFRecord);
}

CString SMState::GetItemCodeFromMLBC(void)
{
    return SMStateBase::GetItemCodeFromMLBC();
}

void SMState::TurnOnYellowTriColorLight(void)
{
    SMStateBase::TurnOnYellowTriColorLight();
}

int SMState::DMCheckForCoinDispenserLowSensor(void)
{
    return SMStateBase::DMCheckForCoinDispenserLowSensor();
}

SMStateBase * SMState::DMCashChangerInOperable()
{
    return SMStateBase::DMCashChangerInOperable();
}

SMStateBase * SMState::FillDMEvent2NotifyTB(const MessageElement* pEvent, sExtStateEventNotifyIn& sNotifyIn, sExtStateEventNotifyOut& sNotifyOut)
{
    return SMStateBase::FillDMEvent2NotifyTB(pEvent, sNotifyIn, sNotifyOut);
}

SMStateBase * SMState::HandleScannerData( const MessageElement* message)
{
    trace(L6,_T("+SMState::HandleScannerData()"));
    SMStateBase * pRetState = STATE_NULL;
    CString sBarcodeRaw = cDMLastData;
    CString sBarcode = _T("");
    trace(L6, _T("ScannerData: %s"), sBarcodeRaw);
    if( 
        (TB_SUCCESS_IMMEDIATE == SendTBCommand(TBCMD_IS_EMPLID_ENABLED)) &&
        dm.IsScannerEnabled() &&
        (OperPassStateName() == _T("SMOperatorPasswordState") || OperPassStateName() == _T("SMSecOperatorPWState") ) &&
        (TB_SUCCESS_IMMEDIATE == SendTBCommand(TBCMD_IS_MEIJER_BADGE_GET_BARCODE, &sBarcodeRaw, &sBarcode)) 
		)
    {
        // scanner should be disabled physically already, send this for bookkeeping and ensuring flags are set
        DMScannerDisable();
		
        if( ((SMOperatorPasswordState*)this)->IsPromptingForOperator() )
        {
            // We've received a badge scan process it, wedge the data into the user field and proceed.
            trace(L6, _T("Employee badge was detected, sending barcode to login screen: %s"), sBarcode);
            PSClearAllInput(true);
            for(int i = 0; i < sBarcode.GetLength(); i++)
            {
                PSAlphaNumericKey(sBarcode[i]);
            }
            pRetState = PSEnterKey();
        }
        else
        {
            trace(L0, _T("Employee badge scanned ignored: SCOTApp is not currently prompting for an operator entry."));
        }
        DMScannerEnable(); 
    }
    else
    {
        pRetState = SMStateBase::HandleScannerData(message);
    }
    trace(L6,_T("-SMState::HandleScannerData()"));
    return pRetState;
}

SMStateBase * SMState::DMRemoteScannerConnect(bool bConnect)
{
    return SMStateBase::DMRemoteScannerConnect(bConnect);
}

void SMState::DMHandleAcceptorHigh()
{
    SMStateBase::DMHandleAcceptorHigh();
}

SMStateBase * SMState::ClearAllStatesButLast(long lDeviceClass, SMStateBase *sReturnState)
{
    return SMStateBase::ClearAllStatesButLast(lDeviceClass, sReturnState);
}

void SMState::ClearAllStates(long lDeviceClass, SMStateBase *sReturnState)
{
    SMStateBase::ClearAllStates(lDeviceClass, sReturnState);
}

void SMState::CheckAndCleanStateStack(void)
{
    SMStateBase::CheckAndCleanStateStack();
}

void SMState::PrintFifoStack(void)
{
    SMStateBase::PrintFifoStack();
}

bool SMState::IsMINIHardware()
{
    return SMStateBase::IsMINIHardware();
}

bool SMState::IsSUPERMINI()
{
    return SMStateBase::IsSUPERMINI();
}

bool SMState::Is5XHardware()
{
    return SMStateBase::Is5XHardware();
}

bool SMState::IsCashDeviceConfigured()
{
    return SMStateBase::IsCashDeviceConfigured();
}

bool SMState::IsVerifoneConfigured()
{
    return SMStateBase::IsVerifoneConfigured();
}

void SMState::DisplayVerifoneForm(_bstr_t csForm)
{
    SMStateBase::DisplayVerifoneForm(csForm);
}

CString SMState::Get563xCassetteParms(long lcode, int nContext)
{
    return SMStateBase::Get563xCassetteParms(lcode, nContext);
}

void SMState::SetCashDiscrepancyStatus(bool bDiscrepancy)
{
    SMStateBase::SetCashDiscrepancyStatus(bDiscrepancy);
}

bool SMState::GetCashDiscrepancyStatus(void)
{
    return SMStateBase::GetCashDiscrepancyStatus();
}

bool SMState::LoadReceiptLogo(CString bitmapFilePath)
{
	trace(L0, _T("+SMState::LoadReceiptLogo()"));
	bool bRet = false;
	long lIndex = 1;
	long lWidth = -11;
	long lAlign = -2; //width and alignment are hard coded for optimization for now, but they are in the ACSOPTS.


	long lRet = 0;
	CString strResult;
	// SA250050: Call the new PrinterSetBitmap call that does not 
	//           translate and modify parameters based on the statically
	//           configured ChitBmp values. Also, width & alignment 
	//           parameters should be the original DM (read, non FL) values
	lRet = dm.PrinterSetBitmap(DMCLASS_RECEIPT, bitmapFilePath, lAlign, lIndex, lWidth);
	strResult = _T("Printer reported successful bitmap load.");
	if (lRet != 0)
		strResult.Format(_T("SetBitmap(%d,'%s',%d,%d,%d) failed with error %d."), DMCLASS_RECEIPT, bitmapFilePath, lAlign, lIndex, lWidth, lRet);

	trace(L0, _T("%s"), strResult);
	trace(L0, _T("-SMState::LoadReceiptLogo()"));

	return lRet == 0;
}

VOID CALLBACK ScannerMsgTimeoutProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    if( m_pSMState)
    {
        m_pSMState->ScannerMsgTimeout();
    }
}

UINT _cdecl SMState::RemoteMessageListenerProc(LPVOID pvParam)
{
    ((SMState*)pvParam)->RemoteMessageListener();
	return 0;
}

void SMState::JoinThreadOrTerminate(CWinThread * thread, DWORD timeout)
{
    trace(L6, _T("+JoinThreadOrTerminate()"));
    
    // Wait for thread to end or timeout.
    DWORD code = 0;
	DWORD ticks = GetTickCount();
	do
	{
		trace(L6, _T("Waiting for the thread to complete..."));
		GetExitCodeThread(thread->m_hThread, &code);
		Sleep(100);
	}
	while( STILL_ACTIVE == code && ((GetTickCount() - ticks) < timeout) );

    // If thread is still active, kill it
	/*if( STILL_ACTIVE == code )
	{
		trace(L0, _T("Thread did not die within %u ms as expected. TerminateThread is being called."), timeout);
		TerminateThread(thread->m_hThread,-1);
	}
	thread = NULL;*/

    trace(L6, _T("-JoinThreadOrTerminate()"));
}

void SMState::RemoteMessageListener()
{
    trace(L6, _T("RemoteMessageListener() -- thread started"));
    while( WaitForSingleObject(m_remoteMessageListenerExit, 0) == WAIT_TIMEOUT )
    {
        try
        {
            const int MSG_SIZE = 2048;
            TCHAR szSender[MSG_SIZE] = {0};
            TCHAR szMessage[MSG_SIZE] = {0};
            int nSenderLen = MSG_SIZE-1;
            int nMessageLen = MSG_SIZE-1;
            if( GetProcessMessage(szSender, nSenderLen, szMessage, nMessageLen) )
            {
                CString sender = szSender;
                CString message = szMessage;
                trace(L6, _T("+RemoteMessageListener() - GetProcessMessage()- Received from '%s', message '%s'"), sender, message);                
                if( !sender.CompareNoCase(m_scannerMsgTarget) )
                {
                    CString operation = ExtractRapEventData(eventRapOperation, message);
                    if( operation == _T("") )
                    {
                        trace(L0,_T("An operation was not included in the message. Discarding."));
                    }
                    else if( operation == ReleaseScanner )
                    {
                        mo.PostDM(DM_DATA, DMCLASS_REMOTEMESSAGE, DM_REMOTEMESSAGE_RELEASESCANNER,0,NULL);
                    }
                    else if( operation == ScannerAvailable )
                    {
                        mo.PostDM(DM_DATA, DMCLASS_REMOTEMESSAGE, DM_REMOTEMESSAGE_SCANNERAVAILABLE,0,NULL);
                    }
                    else if( operation == ScannerOpenFailed )
                    {
                        mo.PostDM(DM_DATA, DMCLASS_REMOTEMESSAGE, DM_REMOTEMESSAGE_SCANNEROPENFAILED,0,NULL);
                    }
                    else
                    {
                        trace(L0,_T("No FastLane handler for operation '%s'. Ignoring."), operation);
                    }
                }
                else if( !sender.CompareNoCase(m_biometMsgTarget) )
                {
                    CString operation = ExtractRapEventData(eventRapOperation, message);
                    if( operation == KEY_CT_OPERATION_BIOMET )
                    {
                        CString sEncodedData = ExtractRapEventData(KEY_CT_OPERATION_BIOMETDATA, message);
                        if( sEncodedData.GetLength() )
                        {
                            LPTSTR pszEncodedData = new TCHAR[sEncodedData.GetLength()+1];
                            _tcscpy(pszEncodedData, sEncodedData);
                            // fill manually because PostDM has a 1000 character limit on data buffer.
                            MessageElement * mePtr = new MessageElement; 
                            mePtr->id = ID_DM;
                            mePtr->dmInfo.Msg = DM_DATA;
                            mePtr->dmInfo.Class = DMCLASS_REMOTEMESSAGE;
                            mePtr->dmInfo.DeviceId = DM_REMOTEMESSAGE_PROCESSBIOMETRICDATA;
                            mePtr->dmInfo.lCodeOrLength = (sEncodedData.GetLength()+1)*sizeof(TCHAR);
                            mePtr->dmInfo.vDPtr = (LPVOID)pszEncodedData;
                            mo.PostIt(mePtr);
                        }
                        else
                        {
                            trace(L0, _T("Biometric message had no encoded data. Ignoring."));
                        }
                    }
                    else
                    {
                        trace(L0, _T("Unknown operation '%s' from source '%s'. Ignoring."), operation, sender);
                    }
                }
                else if( !sender.CompareNoCase(m_currentProcessName) )
                {
                    CString operation = ExtractRapEventData(eventRapOperation, message);
                    if( operation == _T("") )
                    {
                        trace(L0,_T("An operation was not included in the message. Discarding."));
                    }
                    else if( operation == ExitRemoteMessageListener )
                    {
                        trace(L0,_T("Received a request to exit the remote message listener. Exiting."));
                    }
                    else
                    {
                        trace(L0,_T("No FastLane handler for operation '%s'. Ignoring."), operation);
                    }
                }
                else
                {
                    trace(L0,_T("Received a message from an unexpected source: %s. Ignoring."), sender);
                }
                trace(L6, _T("-RemoteMessageListener() - GetProcessMessage() message handled. Waiting for next message."));
            }
        }
        catch(CException * e)
        {
            TCHAR szError[300];
            e->GetErrorMessage(szError, 300);
            trace(L0, _T("RemoteMessageListener() -- CException occured: %s"), szError);
            e->Delete();
        }
        catch(...)
        {
            DWORD err = GetLastError();
            trace(L0, _T("RemoteMessageListener() -- exception occured. GetLastError: %d"), err);
        }
    }
    trace(L6, _T("RemoteMessageListener() -- thread exiting"));
}

CString SMState::ExtractRapEventData(CString key, CString eventData)
{ 
    CString val = _T("");
    try
    {
        CString searchKey = key;
        searchKey += _T("=");
        int index = eventData.Find(searchKey);
        if( index > -1 )
        {
            int start = index + searchKey.GetLength();
            int end = eventData.Mid(start).Find(_T(";"));
            if( end > -1 )
            {
                end += start;
                val = eventData.Mid(start, end - start);
            }
            else
            {
                val = eventData.Mid(start);
            }
        }
    }
    catch(...)
    {
        trace(L0, _T("ExtractRapEventData() exception."));
        val = "";
    }
    return val;
}

void SMState::DetectAndEnableScannerSharing()
{
    trace(L6, _T("+SMState::DetectAndEnableScannerSharing()"));
    // Check if scanner sharing is enabled. For this we need to determine
    // which scanner class is being used by the LaunchPad.
    m_scannerSharingEnabled = false;
    _bstr_t configFile =  co.csConfigFilePath + _T("\\") + _T("LaunchPad.xml");
    try
    {
        MSXML2::IXMLDOMDocumentPtr spDoc(__uuidof( MSXML2::DOMDocument40));
        if( VARIANT_TRUE == spDoc->load(configFile) )
        {
            MSXML2::IXMLDOMNodePtr spClass = spDoc->selectSingleNode(OLESTR("//Options/ScannerClass"));
            if( spClass != NULL )
            {
                CString scanClass = (LPCTSTR)spClass->text;
                m_scannerSharingEnabled = (-1 != scanClass.Find(_T("ScannerShared")));
            }
        }
    }
    catch(_com_error & cex)
    {
        trace(L6, _T("Exception occurred trying to read from the %s file: %s"), (LPCTSTR)configFile, cex.ErrorMessage());
    }
    if( m_scannerSharingEnabled )
    {
        trace(L6, _T("Detected the LaunchPad is configured for Scanner Sharing - enabling it for FastLane."));
    }
    trace(L6, _T("-SMState::DetectAndEnableScannerSharing()"));
}

long SMState::OpenAndEnableScannerDirectly(bool forceEnable, bool sendMsgOnFail)
{
    trace(L6, _T("+SMState::OpenAndEnableScannerDirectly(%d,%d)"), forceEnable, sendMsgOnFail);
    long rc = OPOS_SUCCESS;
	
    if( !m_scannerLoaded )
    {
        rc = dm.OpenDevice(DMCLASS_SCANNER);
        m_scannerLoaded = true;
    }
	
    if( rc == OPOS_SUCCESS )
    {
        if( forceEnable || bEASReEnableScanner )
        {
            rc = dm.ScannerEnable();
        }
    }
    else
    {
        trace(L0, _T("Failed to open the scanner device"));
        if( sendMsgOnFail )
        {
            // Go ahead and send a failure remote message to originator.
            CString msg;
            msg.Format(_T("%s=%s;"), eventRapOperation, ScannerOpenFailed);
            trace(L6, _T("SendProcessMessage(%s, %s)"), m_scannerMsgTarget, msg);
            SendProcessMessage(m_scannerMsgTarget, msg);
        }
    }
    trace(L6, _T("-SMState::OpenAndEnableScannerDirectly() = %d"), rc);
    return rc;
}


void SMState::ProcessReleaseScannerRequest()
{
    trace(L6, _T("+SMState::ProcessReleaseScannerRequest()"));
    if( m_scannerSharingEnabled )
    {
        dm.ScannerDisable();
        long rc = dm.CloseDevice(DMCLASS_SCANNER);
        if( rc != OPOS_SUCCESS )
        {
            trace(L0, _T("Failed to close the scanner device."));
        }
        m_scannerLoaded = false;
        CString msg;
        msg.Format(_T("%s=%s;"), eventRapOperation, ScannerAvailable);
        //Adding below condition to skip the communication between LaunchPad and ScotApp 
        //regarding scanner while a transaction is going on
		//+POS294076 and POS270827
		if(fSAInTransaction==false)
		{
			trace(L6, _T("SendProcessMessage(%s, %s)"), m_scannerMsgTarget, msg);
			SendProcessMessage(m_scannerMsgTarget, msg);
		}
		//-POS294076 and POS270827		
    }
    else
    {
        trace(L0, _T("Scanner Sharing is not enabled for FL. Ignoring remote message."));    
    }
    trace(L6, _T("-SMState::ProcessReleaseScannerRequest()"));    
}


void SMState::ProcessResponseScannerAvailable()
{
    trace(L6, _T("+SMState::ProcessResponseScannerAvailable()"));
    if( m_scannerSharingEnabled )
    {
		m_waitingForScannerMsgResponse = false;
		ClearScannerMsgTimer();
        OpenAndEnableScannerDirectly(false, true);
    }
    else
    {
        trace(L0, _T("Scanner Sharing is not enabled for FL. Ignoring remote message."));
    }
    trace(L6, _T("-SMState::ProcessResponseScannerAvailable()"));
}

void SMState::ProcessResponseScannerOpenFailed()
{
    trace(L6, _T("+SMState::ProcessResponseScannerOpenFailed()"));
    if( m_scannerSharingEnabled )
    {
        OpenAndEnableScannerDirectly(false, true);
    }
    else
    {
        trace(L0, _T("Scanner Sharing is not enabled for FL. Ignoring remote message."));
    }
    trace(L6, _T("-SMState::ProcessResponseScannerOpenFailed()"));
}

SMStateBase * SMState::ProcessBiometricData(CString encodedData)
{
    trace(L6, _T("+SMState::ProcessBiometricData()"));
    SMStateBase * pRet = STATE_NULL;
    
    if( TB_SUCCESS_IMMEDIATE != SendTBCommand(TBCMD_IS_BIOMET_ENABLED) )
    {
        trace(L0, _T("Biometric data received but biometric option is disabled. Ignoring.")); 
        return STATE_NULL;
    }
	
    if( !SCOTIsVisible() || SCOTIsMinimized() )
    {
        trace(L0,_T("Biometric data received while the SCOT application is minimized or in the background (invisible). Ignoring."));
    }
    else if( OperPassStateName() == _T("SMOperatorPasswordState") ||
		OperPassStateName() == _T("SMSecOperatorPWState") )
    {
        SMOperatorPasswordState * pState = (SMOperatorPasswordState*)this;
        pState->m_sBiometData = encodedData;
        pRet = PSEnterKey();
        pState->m_sBiometData = _T("");
    }
    else
    {
        trace(L0,_T("Biometric data received in a state that does not handle it. Ignoring"));
    }
    
    trace(L6, _T("-SMState::ProcessBiometricData()"));
    return pRet;
}
// received biometric data, if applicable send it off for processing

// If biometrics are enabled AND we are on a Password entry screen 
// create a user/pass structure and send it off for validation.

long SMState::RequestAccessToScanner()
{
    trace(L6, _T("+SMState::RequestAccessToScanner()"));
    long rc = OPOS_SUCCESS;
	if( !dm.IsScannerEnabled() )
	{
		if( m_scannerSharingEnabled && IsScannerMsgTargetRunning() )
		{
            trace(L6, _T("Scanner sharing is enabled and LaunchPadNet is running, request scanner be released."));
			m_waitingForScannerMsgResponse = true;
            CString msg;
            msg.Format(_T("%s=%s;"), eventRapOperation, ReleaseScanner);
            trace(L6, _T("SendProcessMessage(%s, %s)"), m_scannerMsgTarget, msg);
            SendProcessMessage(m_scannerMsgTarget, msg);
			SetScannerMsgTimer();
        }
        else // open (if required) & enable device
        {
            rc = OpenAndEnableScannerDirectly(true, false);
        }
    }
	else
	{
        trace(L6, _T("The scanner is already enabled."));
	}
    trace(L6, _T("-SMState::RequestAccessToScanner() = %d"), rc);
    return rc;
}

void SMState::ScannerMsgTimeout()
{
    trace(L6, _T("+SMState::ScannerMsgTimeout()"));
    ClearScannerMsgTimer();
    if( m_waitingForScannerMsgResponse )
    {
        trace(L6, _T("Timeout occurred, did not receive response message from LaunchPadNet in time."));
        m_waitingForScannerMsgResponse = false;
        
        // We requested the scanner and didn't get a response. Try to enable it
        OpenAndEnableScannerDirectly(true, true);
    }
    trace(L6, _T("-SMState::ScannerMsgTimeout()"));
}

void SMState::SetScannerMsgTimer()
{
    trace(L6, _T("+SMState::SetScannerMsgTimer()"));
    EnterCriticalSection(&m_scannerMsgTargetLock);
    if( m_scannerMsgTimer ) 
    {
        ClearScannerMsgTimer();
    }
    trace(L6, _T("Creating timer for scanner message that will timeout in %d ms"), m_scannerMsgTimeout);
    m_scannerMsgTimer = SetTimer(NULL, 0, m_scannerMsgTimeout, (TIMERPROC)ScannerMsgTimeoutProc);
    if( !m_scannerMsgTimer )
    {
        m_waitingForScannerMsgResponse = false;
        trace(L0, _T("Failed to set the Scanner Message Timer. Request won't be protected by a timeout."));
	}
    LeaveCriticalSection(&m_scannerMsgTargetLock);
    trace(L6, _T("-SMState::SetScannerMsgTimer()"));
}

void SMState::ClearScannerMsgTimer()
{
    trace(L6, _T("+SMState::ClearScannerMsgTimer()"));
    EnterCriticalSection(&m_scannerMsgTargetLock);
    
    if( m_scannerMsgTimer )
    {
        trace(L6, _T("Killing scanner message timer."));
        KillTimer(NULL, m_scannerMsgTimer);
        m_scannerMsgTimer = NULL;
    }
    LeaveCriticalSection(&m_scannerMsgTargetLock);
    trace(L6, _T("-SMState::ClearScannerMsgTimer()"));
}

bool SMState::IsScannerMsgTargetRunning()
{
    return (0 != ::FindWindow(NULL, m_scannerMsgTarget));
}

void SMState::SetDMCashTakenInSecState(bool value)
{
    trace(L6, _T("SMState::SetDMCashTakenInSecState()"));
	
	m_cashTakenInSecState = value;
    
	trace(L6, _T("SetDMCashTakenInSecState value is %d"), (long)value);
}

bool SMState::GetDMCashTakenInSecState()
{
    trace(L6, _T("SMState::GetDMCashTakenInSecState()"));
	
    trace(L6, _T("GetDMCashTakenInSecState value is %d"), (long)m_cashTakenInSecState);
	
	return m_cashTakenInSecState;
}

void SMState::SetTimeoutInTakeChangeState(bool value)
{
    trace(L6, _T("SMState::SetTimeoutInTakeChangeState()"));
	
	m_timeoutInTakeChangeState = value;
    
	trace(L6, _T("SetTimeoutInTakeChangeState value is %d"), (long)value);
}

bool SMState::GetTimeoutInTakeChangeState()
{
    trace(L6, _T("SMState::GetTimeoutInTakeChangeState()"));
	
    trace(L6, _T("GetTimeoutInTakeChangeState value is %d"), (long)m_timeoutInTakeChangeState);
	
	return m_timeoutInTakeChangeState;
}

void SMState::SetChangeDispensedState(bool value)
{
    trace(L6, _T("SMState::SetTimeoutInTakeChangeState()"));
	
	m_bCashDispensed = value;
    
	trace(L6, _T("SetChangeDispensedState value is %d"), (long)value);
}

bool SMState::GetChangeDispensedState()
{
    trace(L6, _T("SMState::GetChangeDispensedState()"));	
	
	
	return m_bCashDispensed;
}

void SMState::SetCutPercentage(long lCutPercentage)
{
    m_lCutPercentage = lCutPercentage;
}

void SMState::ThrowDmException(long  nDeviceClass, long  lResult, long  nRcEx )
{
#define ANY_VALUE 0
    static const struct 
    {
        long  nDeviceClass;     // 0 means any
        long  nResult;          // 0 means any (when I'm not sure what the value is.) 
        long  nRcEx;            // 0 means any
        SCODE dmCode;
    } mapResultsToException[] =
        
        //SR672
    {     // Specific DM device status values may be defined in column #4
        DMCLASS_SCANNER,  OPOS_E_DISABLED,  ANY_VALUE,                 OPOS_E_OFFLINE,
            DMCLASS_SCANNER,  OPOS_E_NOHARDWARE,ANY_VALUE,                 OPOS_E_OFFLINE,
            DMCLASS_SCANNER,  OPOS_E_OFFLINE,   ANY_VALUE,                 OPOS_E_OFFLINE,
            ANY_VALUE,        OPOS_SUCCESS,     ANY_VALUE,                 0,
            DMCLASS_PRINTER,  OPOS_E_FAILURE,   NCR_EPTR_SWFAIL,           OPOS_E_FAILURE,
            DMCLASS_PRINTER,  ANY_VALUE,        NCR_EPTR_DE_PORTOPEN,      OPOS_E_FAILURE,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_STATION,          OPOS_E_ILLEGAL,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_NOCAP,            OPOS_E_ILLEGAL,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_ROTATEMODE,       OPOS_E_ILLEGAL,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_CP_PERCENTAGE,    OPOS_E_ILLEGAL,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_BC_SYMBOLOGY,     OPOS_E_ILLEGAL,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_BC_HEIGHT,        OPOS_E_ILLEGAL,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_BC_ALIGNMENT,     OPOS_E_ILLEGAL,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_BC_TEXTPOSITION,  OPOS_E_ILLEGAL,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_BM_BITMAPNUMBER,  OPOS_E_ILLEGAL,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_BM_WIDTH,         OPOS_E_ILLEGAL,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_BM_ALIGNMENT,     OPOS_E_ILLEGAL,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_BM_BITMAPTYPE,    OPOS_E_ILLEGAL,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_RP_NOTROTATEMODE, OPOS_E_ILLEGAL,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_RP_ROTATION,      OPOS_E_ILLEGAL,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_TP_TRANSMODE,     OPOS_E_ILLEGAL,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_TP_NOTTRANSMODE,  OPOS_E_ILLEGAL,
            DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_TP_CONTROL,       OPOS_E_ILLEGAL,
            DMCLASS_MICR,     OPOS_E_FAILURE,   NCR_EMICR_LINE_ERROR,      OPOS_E_FAILURE,
            DMCLASS_MICR,     OPOS_E_FAILURE,   NCR_EMICR_NOCHECK,         OPOS_E_FAILURE,
            DMCLASS_MICR,     OPOS_E_FAILURE,   NCR_EMICR_NO_DATA,         OPOS_E_FAILURE,
            DMCLASS_MICR,     OPOS_E_FAILURE,   NCR_EMICR_BAD_DATA,        OPOS_E_FAILURE,
            DMCLASS_MICR,     OPOS_E_FAILURE,   NCR_EMICR_CHECKJAM,        OPOS_E_FAILURE,
            DMCLASS_PRINTER,  OPOS_E_EXTENDED,  OPOS_EPTR_BADFORMAT,       OPOS_EPTR_BADFORMAT,
            //?   DMCLASS_PRINTER,  OPOS_E_EXTENDED,  OPOS_EPTR_TOOBIG,          DM_E_BITMAPTOOBIG, ?See Dm Code?
            ANY_VALUE,        OPOS_E_ILLEGAL,   ANY_VALUE,                 OPOS_E_ILLEGAL,
            ANY_VALUE,        OPOS_E_NOEXIST,   ANY_VALUE,                 OPOS_E_NOEXIST,
            ANY_VALUE,        OPOS_E_FAILURE,   ANY_VALUE,                 OPOS_E_FAILURE,
            ANY_VALUE,        OPOS_E_BUSY,      ANY_VALUE,                 OPOS_E_BUSY,
            ANY_VALUE,        OPOS_E_TIMEOUT,   ANY_VALUE,                 OPOS_E_TIMEOUT,
            DMCLASS_CASHRECYCLER, OPOS_E_OFFLINE, ANY_VALUE,				 OPOS_E_OFFLINE, // SafePay poweroff.
            DMCLASS_CASHRECYCLER, OPOS_E_DISABLED, ANY_VALUE,				 OPOS_E_DISABLED, // TAR 356035
    };  
    SCODE scException = OPOS_E_FAILURE;
    
    
    LPCTSTR sDesc = _T("Device failure");
    if ( nDeviceClass == DMCLASS_RECEIPT
        || nDeviceClass == DMCLASS_JOURNAL
        || nDeviceClass == DMCLASS_SLIP )
        nDeviceClass = DMCLASS_PRINTER;
    for ( int nIndex = 0; nIndex < sizeof( mapResultsToException )
        / sizeof( mapResultsToException[0] ); nIndex++ )
    {
        if ( mapResultsToException[ nIndex ].nDeviceClass == ANY_VALUE
            || mapResultsToException[ nIndex ].nDeviceClass == nDeviceClass )
        {
            if ( mapResultsToException[ nIndex ].nResult == lResult )
            {
                if ( mapResultsToException[ nIndex ].nRcEx == ANY_VALUE
                    || mapResultsToException[ nIndex ].nRcEx == nRcEx )
                {
                    scException = mapResultsToException[ nIndex ].dmCode;
                    break;
                }
            }
            // Allow extended result codes to be mapped regardless of ResultCode
            else if ( mapResultsToException[ nIndex ].nResult == ANY_VALUE
                && mapResultsToException[ nIndex ].nRcEx == nRcEx )
            {
                scException = mapResultsToException[ nIndex ].dmCode;
                break;
            }
        }
    }
    ThrowDmException( scException  );
    return;
}

/* static */
void SMState::ThrowDmException( SCODE scException )
{
    static const struct 
    {
        SCODE dmCode;
        LPCTSTR sDesc;
    } mapScToText[] =
    {
        OPOS_E_ILLEGAL,              _T("Function is illegal"),
            OPOS_E_NOEXIST,        _T("File not found"),
            OPOS_EPTR_BADFORMAT,   _T("Invalid file format"),
            OPOS_EPTR_TOOBIG,         _T("Bitmap too big for device"),
            OPOS_E_FAILURE,        _T("Device failure"), 
            OPOS_E_BUSY,                 _T("An asynchronous process is currently using device"),
            OPOS_E_TIMEOUT,              _T("Device timeout occurred"),
            DM_E_INVALIDDEVICECLASS,   _T("nDeviceClass parameter is invalid"),
            DM_E_INVALIDDEVICEID,      _T("nDeviceId parameter is invalid"),
            DM_E_DEVICENOTCONFIGURED,  _T("Device is not configured"),
            OPOS_E_DISABLED,   _T("Device is not available"),
            DM_E_INVALIDPARAMETER,     _T("Invalid parameter"),
            OPOS_E_OFFLINE,			 _T("Device offline"),
    };
    
    if ( scException != 0 )
    {
        LPCTSTR sDesc = _T("Unknown Exception Code");
        for ( int nIndex = 0; nIndex < sizeof( mapScToText ) / sizeof( mapScToText[0] );
        nIndex++ )
        {
            if ( mapScToText[ nIndex ].dmCode == scException )
            {
                sDesc = mapScToText[ nIndex ].sDesc;
                break;
            }
        }
        COleDispatchException* pExcept = new COleDispatchException( sDesc, -1, 0 );
        pExcept->m_scError = scException;
        throw pExcept;
    }
    return;
}


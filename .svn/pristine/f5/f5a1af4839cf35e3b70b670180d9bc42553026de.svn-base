//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    SMStateDMBase.CPP
//
// TITLE:   DM SMStateBase functions, implement all SMStateBase::DM* functions
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <winver.h>                 // version information header

#ifndef _CPPUNIT
#include "Common.h"                 // common includes
//SR672
#include "dmx\printer.h"
#include "oposbill.h"
#include "OPOSPtr.h"
#include "NCRPtr.h"
#include <ncrscan.h>
#include "NCRCashServicesSOEx.h"
#include "NCRBNRConsts.h"


#include "DMProcedures.h"           // DM procedures
//DLLEXPORT extern DMProcedures dm;             // global DM object
#include "DMCashCount.h"
DLLEXPORT extern DMCashCount  dmcc;    // Global DM object

//USSF Start
#include "SMUSSFManager.h"
#include "USSFCommon.h"
//USSF End

#include "SMTakeCard.h"             // take card state
#include "SMCardMisRead.h"          // mis-read card state
#include "SMSelectCardType.h"       // select card type
#include "SMCardProcessing.h"
#include "SMTakeLoyaltyCard.h"
#include "SMAcknowledgeSig.h"
#include "SMSmAuthorization.h"      // When scanning in supervisor barcodes, go to store mode
#include "SMSystemMessage1.h"       // When scanning in supervisor barcodes, go to store mode
#include "SCOTDevFactory.h"         // sensormatic devices
#include "Scotmot.h"
#include "opos_eas.h"
#include "MPProcedures.h"
#include "SMUnDeActivatedItemApproval.h"
#include "SMSystemMessage4.h"       // TAR197594
#include "SMSmMediaStatusBase.h"
#include "SMCustomMessage.h"
#include "GloryOpos.h"
#include "NCRRemoteSoEx.h"
#include "SMRemoteScannerConnect.h"
#include "SafePayC.h"               // SafePay cash recycler constants.
#include "dmx/ScaleWrapper.h"
#include "OposScal.h" 
#include "SMInvalidBarcode.h"
#include "SMSmAssistMenu.h"
#else
#include "FakeSMStateBase.h"
#endif //_CPPUNIT
#ifndef _CPPUNIT
#include "DeviceErrorLookUpAPI.h" //SR672
#include "CMInterface.h" //SR700
#endif //_CPPUNIT
extern CString csGLOBALAddThe128Prefix; //430804

#ifndef _CPPUNIT

#define _COIN_MEDIA_LOW_SENSOR _T("COINWITHMEDIALOWSENSOR")
#define CHARACTERLIMIT 40
#define LINEFORMAT _T("%s\n")


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SMdmBase")

// awk.12.1.2 Begin
#ifdef _DEBUG
#define MSGFMT _T("%s (%d)")
#else
#define MSGFMT _T("%s")
#endif

#define CASE_MSG(a)                 \
case DM_##a:                        \
  csWork.Format(MSGFMT,             \
    ps.GetPSText(MSG_##a),          \
    lCode);                         \
    break;
// awk.12.1.2 end

//SR672
#define CASE_MSGBILL(a)                 \
case  BILL_##a:                        \
  csWork.Format(MSGFMT,             \
    ps.GetPSText(MSG_BILL_##a),         \
    lCode);                         \
    break;

//SR672
#define CASE_MSGCOIN(a)                 \
case  COIN_##a:                        \
  csWork.Format(MSGFMT,             \
    ps.GetPSText(MSG_COINACCEPTOR_##a),         \
    lCode);                         \
    break;
#include "io.h"
#endif //_CPPUNIT
#define SIGRECEIPT_PATH             _T("%DATA_DRIVE%\\SCOT\\DATA\\") 
#define SIGRECEIPT_FILE_NAME      _T("S-RECEIPT.SCP") 
#define SIGRECEIPT_LOG_NAME       _T("S-RECEIPT.SCP.LOG") 
#define SIGRECEIPT_BACKUP_NAME  _T("S-RECEIPT.SCP.BAK") 
#define SIGRECEIPT_BADFILE_NAME _T("S-RECEIPT.SCP.BAD") 
#define DEVICEERRORXMLFILEPATH "%APP_DRIVE%\\scot\\data\\deviceerror\\"

//SR672+
const long NCR_BOTH_OVERDISPENSE = 1002;
const long NCR_BOTH_CANNOT_DISPENSE_CASH_MANAGEMENT = 1003;
const long NCR_BOTH_CANNOT_DISPENSE_HOPPER = 1004;
const long NCR_BOTH_CANNOT_DISPENSE = 1005;

//SR672-


//TAR 195179 & 191490 end

// DM last status
int  SMStateBase::nDMLastMsg = 0;       // last DM message type
int  SMStateBase::nDMLastDev = 0;       // last DM device
long SMStateBase::lDMLastCode = 0;      // last DM message code
long SMStateBase::lastDMReceiptCode = PTR_SUE_REC_PAPEROK; //SR672
long SMStateBase::lastDMJournalCode = PTR_SUE_JRN_PAPEROK; //SR672
long SMStateBase::sm_lastDMSlipCode = PTR_SUE_SLP_PAPEROK; //SR672
long SMStateBase::lastDMCashAcceptorCode = BILL_STATUS_OK; //SR672
long SMStateBase::lastDMCoinAcceptorCode = COIN_STATUS_OK; //SR672
int  SMStateBase::nDMLastLength = 0;    // last DM message data length
int  SMStateBase::nDMLastAcceptor = 0;  // last cash amt inserted in cents

int  SMStateBase::nDMLastCoinEvent = 0;  // last cash amt inserted in cents
int  SMStateBase::nDMLastBillEvent = 0;  // last cash amt inserted in cents

bool  SMStateBase::fDMErrorCashDue = 0;  // cash still to be returned Tar 214729
bool SMStateBase::fCancelPayment = false;

int  SMStateBase::nDMCashInserted = 0;  // accum cash amt inserted, not tendered
int  SMStateBase::nDMCashTendered = 0;  // accum cash amt tendered, not accepted
int  SMStateBase::nDMCashInDrawer = 0;  // total cash accepted by smkt
int  SMStateBase::nDMLastTrackLen = 0;  // last DM MSR track length

LPTSTR SMStateBase::cDMLastData = NULL;  // last DM data
LPTSTR SMStateBase::cDMLastTrack = NULL; // last DM MSR track data, only 1 track

long SMStateBase::lDMScaleWeight = 0;   // las DM scale weight or negative if broken
bool SMStateBase::fDMScaleChanged = 0;

CString SMStateBase::csDMLastBarCode = "";  // last DM bar code scanned

// BeginCodeLocaleUPC
CString SMStateBase::csDMLastBarLabel = ""; // last DM bar code scanned
long SMStateBase::csDMLastBarType = 0;  

BOOL SMStateBase::m_bLastScanDataWasMultiLabel = FALSE;  //last DM bar code scanned was part of a multi-label barcode
BOOL SMStateBase::m_bLastMntScanDataWasMultiLabel = FALSE;  //last DM bar code scanned was part of a multi-label barcode
int SMStateBase::m_nNumberOfPartsReturnedSoFar = 0; //The number of parts of a multi-label barcode that we have saved so far
int SMStateBase::m_nTotalNumberOfMLBParts = 0;      //The total number of parts of a multi-label barcode as configured by scotopts
#ifndef _CPPUNIT
CList <MultiLabelBarcodeFormatRecord, MultiLabelBarcodeFormatRecord&> SMStateBase::m_MLBarcodeList;
#endif _CPPUNIT
CArray <CString, CString> SMStateBase::m_MntMLBarcodeArray;
PSMultiLabelBarCodeDetails SMStateBase::m_psMLBarCodeDetails = NULL;
// EndCodeLocaleUPC  

CString SMStateBase::csDateCoinDispenserRefilled = "";   // TAR 140515
CString SMStateBase::csTimeCoinDispenserRefilled = "";   // TAR 140515

CString SMStateBase::csCardAccountNum = ""; // hold CardAccountNum

CString SMStateBase::csDMEncryptedPin = ""; // last DM pin

bool SMStateBase::fDMReceiptError = false;
bool SMStateBase::fDMJournalError = false;
bool SMStateBase::fDMScaleZeroed = true;    // a scale weight has been zeroed
bool SMStateBase::bgCoinDispenserIsEmpty = false;    // true when DM send CC_STATUS_EMPTY
int SMStateBase::nDeviceErrorContext = 0;
int SMStateBase::m_nCoinDispenserWithLowSensor = 0;

extern bool bGLOBALFromLaneClose;   // Tar 192351 RF012002

static long lLastDispense = 0;
bool SMStateBase::m_bCashRecyclerStopDeposit = 0;
long SMStateBase::m_nDMCashPaid = 0;
bool SMStateBase::m_bIsCashDrawerClosed= true;   //RAP Dispense Change  True - Closed, False - Open, default is True

long SMStateBase::m_nDMSavedDev = -1;  //277025
bool SMStateBase::bIgnoreScannedData; //dp185016 tar 286970
int SMStateBase::m_nCashRecyclerHardwareErrorCode = 0;

bool SMStateBase::bIsCashAcceptorEnabled = false;  //rfc330995
bool SMStateBase::bIsCoinAcceptorEnabled = false;  //rfc330995

int SMStateBase::m_nDevInitStatus = 0;
bool SMStateBase::fRecyclerNotReady = false; //dp185016 Recycler Platform Issue #12
int SMStateBase::m_nCashRecyclerFailedRejectAmt = 0;  // TAR 315616
CString SMStateBase::csBalance = _T("");
CString SMStateBase::csLoan = _T("");
CString SMStateBase::csPickUp = _T("");
CString SMStateBase::m_csPrinterCharSet = _T("");   //TAR378451
long SMStateBase::nDMMaximumWeight = 0;
long SMStateBase::nDMMaximumWeightTol = 0;
CString SMStateBase::sDeviceErrorMessageText = _T("");
bool SMStateBase::m_bDiscrepancy = false;   // Add Glory CM Support

#ifndef _CPPUNIT
extern MPProcedures mp;         // Instance

// these macros are used to handle all DM_ERROR and DM_STATUS events
#ifdef _DEBUG
#define ERRFMT _T("%s (%d,%d)")
#else
#define ERRFMT _T("%s")
#endif

#define CASE_IGNORE_NOW(a)          \
case DM_##a:                        \
    trace(L6,_T("-Parse ignore ")_T(#a));     \
   return STATE_NULL;

#define CASE_SYSMSG_NOW(a)          \
case DM_##a:                        \
  csWork.Format(ERRFMT,             \
    ps.GetPSText(MSG_##a,SCOT_LANGUAGE_PRIMARY),                        \
    nDMLastDev,                                             \
    lDMLastCode);                                           \
    fCaseHandled = true;            \
    break;


#define CASE_SYSMSG_L8R(a)          \
case DM_##a:                        \
  csWork.Format(ERRFMT,             \
    ps.GetPSText(MSG_##a,SCOT_LANGUAGE_PRIMARY),                        \
    nDMLastDev,                                             \
    lDMLastCode);                                           \
  fDoLater = true;                  \
    fCaseHandled = true;            \
    break;

#define CASE_HEALTH_L8R(a)          \
case DM_##a:                        \
  csWork.Format(ERRFMT,             \
    ps.GetPSText(MSG_##a,SCOT_LANGUAGE_PRIMARY),                        \
    nDMLastDev,                                             \
    nDMLastCode);                                           \
  ScotError(INFO,SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_DM_CHECK_HEALTH); \
  fCheckHealth = true;              \
  fDoLater = true;                  \
    fCaseHandled = true;            \
    break;

static char *szMsg3Name = NULL;

#define UNAVAILABLESIGBMP   _T("UnavailableSig.dat")


//FZ20020920 Error Handing

static long nDMLastDevId;
typedef struct  
{
int DeviceClass;
_TCHAR DeviceModel[40];
int StatusEventId;
int ExtendedResultCode;
int Context;
_TCHAR sErrorMnemonic[100];
}DeviceError;   

#endif //_CPPUNIT
#ifndef _CPPUNIT
///////////////////////////////
long SMStateBase::DMInitialize(void)
{
  long rc = 0;
  trace(L6,_T("+Init"));

    
  rc = dm.Initialize();
 
  //Check Coin enhanced low sensor
  if (rc >= 0)
  {
    DMCheckForCoinDispenserLowSensor();
    //+SR700
    if(! m_cmMgr.IsOpen()) 
    {
        // TAR 450162:  Pay attention to return of Open.
        if(! m_cmMgr.Open())    // TAR 449890:  Simplify CM IF.
        {
            trace(L4, _T("INFO:  Cash management module not loaded.."));
        }
    }
    //-SR700

    dmcc.Initialize(m_cmMgr); //TAR 448434
    DMCheckCashCounts(true);
  }
    m_csPrinterCharSet = GetPrinterCharSet();   //TAR378451
  trace(L6,_T("-Init %d"),rc);
  return rc;
}
/////////////////////////////////
long SMStateBase::DMUnInitialize(void)
{
  long rc = 0;
  trace(L6,_T("+UnInit"));
    
  rc = dm.UnInitialize();
  
  m_cmMgr.Close();
    
  if (szMsg3Name)
  {
    delete szMsg3Name;
    szMsg3Name = NULL;
  }

  if (cDMLastData) delete [] cDMLastData; //SSCOP-4165
  cDMLastData = NULL;

    if (cDMLastTrack) delete [] cDMLastTrack; //SSCOP-4165
  cDMLastTrack = NULL;
  // SR875
  if(IsVerifoneConfigured())
  {
      if (m_pVForm != NULL)
      {
        m_pVForm->Close();
        m_pVForm = NULL;
      }
  }
    
  trace(L6,_T("-UnInit %d"),rc);
  return rc;
}

/////////////////////////////////////////////////
SMStateBase *SMStateBase::DMParse(const MessageElement *me)
{
  bool fCheckHealth = false;
  bool fDoLater = false;
  bool fCaseHandled = false;
  static long lLastData = 0;
  struct ExtStateEventNotifyInType  sNotifyIn;
  sExtStateEventNotifyOut sNotifyOut;
  sNotifyIn.InitStruct();
  sNotifyOut.InitStruct();

// CString szCreditCardOwnerName ;  
  trace(L6,_T("+Parse"));
  //traceMem(L6);   
  PSTEXT nPsMsg = PS_NOTEXT;
    
  CString csWork;
  // save all the DM parameters
    
  nDMLastMsg = me->dmInfo.Msg;
  nDMLastDev = me->dmInfo.Class;
  long nDMLastDevId = me->dmInfo.DeviceId;
  ASSERT( nDMLastDevId == 0 || nDMLastDev == DMCLASS_MOTION_SENSOR
     || nDMLastDev == DMCLASS_EASDEACTIVATOR || nDMLastDev == DMCLASS_SCANNER);
  switch (nDMLastMsg)
  {
  case DM_DIRECTIO:
  case DM_ERROR:
  case DM_STATUS:
    lDMLastCode = me->dmInfo.lCodeOrLength;
    nDMLastLength = 0;
    break;
  case DM_DATA:
    lDMLastCode = 0;
    nDMLastLength = me->dmInfo.lCodeOrLength;
    break;
  default:
    ScotError(RETURN,SM_CAT_INPUT_PARAMETER, SCOTAPP_DM_MESSAGE,_T("%d"),nDMLastMsg);
    trace(L6,_T("-Parse"));
    return STATE_NULL;
  }
    
  trace(L6,_T("Parse DM evt %d, dev %d, code %d, length %d"),
    nDMLastMsg,
    nDMLastDev,
    lDMLastCode,
    nDMLastLength);
    
  // assume the worst
  csWork.Format(_T("DM Unsupported event %d, device %d, code %d length %d"),
    nDMLastMsg,
    nDMLastDev,
    lDMLastCode,
    nDMLastLength);
    
  // Use macros defined above.
  //
  // Immediate message required
  //  CASE_IGNORE_NOW     do nothing about this event
  //  CASE_SYSMSG_NOW     report this as an error, require operator intervention
  //  CASE_HEALTH_NOW     report this as an error, require operator intervention and
  //                      call CheckHealh after the operator presses continue
  // Wait for Attract to show message
  //  CASE_SYSMSG_L8R     report this as an error, require operator intervention
  //  CASE_HEALTH_L8R     report this as an error, require operator intervention and
  //                      call CheckHealh after the operator presses continue

  // BeginCodeLocaleUPC
  CString csData1;
  CString csData2;
  CString csData3;
  // EndCodeLocaleUPC
  
  //SSCOP-1020 - ignore error from same device
  if( (me->dmInfo.Msg == DM_ERROR) || (me->dmInfo.Msg == DM_STATUS) ) 
  {
      if ( dm.GetDegradedModeType() == DEGRADEDMODE_CARD_ONLY && IsCashDeviceError(nDMLastDev) )
      {
          ASSERT(co.IsDegradedModeOn());
          trace(L6, _T("SMStateBase::DMParse() - pretending cash devices are ok"));

          // Clean up currently unused data
          if (me->dmInfo.vDPtr)
              delete[] me->dmInfo.vDPtr;
 
          return STATE_NULL;
      }

      if ( dm.GetDegradedModeType() == DEGRADEDMODE_CASH_ONLY && IsCardDeviceError(nDMLastDev) )
      {
          ASSERT(co.IsDegradedModeOn());
          trace(L6, _T("SMStateBase::DMParse() - pretending card devices are ok"));

          // Clean up currently unused data
          if (me->dmInfo.vDPtr)
              delete[] me->dmInfo.vDPtr;
 
          return STATE_NULL;
      }
  }
  
  //SR672
  CString s1(_T("")), s2(_T("")), sSubModel(_T("")), sModelName(_T(""));
  CString csModel, csStateName;
  long lResultCode = EVENTID_ANY, lExtRetCode =  EXT_RESULT_ANY;
  switch(nDMLastMsg)
  {
  case DM_STATUS:
      {
          //+4.2 card dispenser
          if (DMCLASS_CARDDISPENSER == nDMLastDev)
          {
              return DMDispenserPositionEvent(  lDMLastCode );
          }
          //-4.2 card dispenser 
      }
  case DM_ERROR:
    {
      switch (nDMLastDev)
      {
            case DMCLASS_MICR:
                return DMMICR( lDMLastCode );
            case DMCLASS_KEYLOCK:
                return DMKeyLock( lDMLastCode );
      case DMCLASS_RECEIPT:
      case DMCLASS_JOURNAL:
      case DMCLASS_SLIP:
        {
          // save the status for the media status screen
          if (lDMLastCode != DM_PTR_REQUEST_COMPLETED)
          {
            // This is an interim fix to handle device errors in
            // remote mode.  The long-term fix is to break all status
            // and error and status events into virtuals that can be overridden
            // by the state handling the remoting.
            if (dm.IsDeviceRemote(nDMLastDev, 0))
            {
               SMStateBase* pHandlerState = STATE_NULL;
               if (nDMLastDev==DMCLASS_RECEIPT)
                  pHandlerState = DMReceiptPrinter( lDMLastCode );
               else if (nDMLastDev==DMCLASS_JOURNAL)
                  pHandlerState = DMJournalPrinter( lDMLastCode );
               else if (nDMLastDev==DMCLASS_SLIP)
                  return DMSlipPrinter( lDMLastCode );
               if (lastDMReceiptCode == lDMLastCode) //HDMerge-062502 Starts here
               {
                   // case handled
                   trace(L0, _T("Receipt/jounral event was handled in the derived object"));
                   return pHandlerState;
               }
               else
               {
                   // case not handled inside the current, active, screen object
                   // let it be handled by the default logic
                   trace(L0, _T("Receipt/journal event being handled by the default handler"));
               }

               // Need to check with Erick!!!!!!!!!!!!!!
/*
               if (fCaseHandled)
                  return pHandlerState;
               else
                  {
                  ms_bKeepRemoteDevices = true;
                  trace(L0, "Default handler is activated");
                  } */ //HDMerge-062502 ends here
            }
            else
            {
              if (nDMLastDev==DMCLASS_RECEIPT)
                  lastDMReceiptCode = lDMLastCode;
              else if (nDMLastDev==DMCLASS_JOURNAL)
                  lastDMJournalCode = lDMLastCode;

              else if (nDMLastDev==DMCLASS_SLIP)
                   return DMSlipPrinter( lDMLastCode );
            }
            /*  SR672
            if(lDMLastCode == DM_PTR_PAPEROK
              && nDMLastDev!=DMCLASS_SLIP
              && !dm.IsDeviceRemote(nDMLastDev, 0))*/
            if((lDMLastCode == PTR_SUE_REC_PAPEROK ||lDMLastCode == PTR_SUE_JRN_PAPEROK)
              && nDMLastDev!=DMCLASS_SLIP
              && !dm.IsDeviceRemote(nDMLastDev, 0))
            {
              if (!dm.GetCoinsOrBillsVeryLow())
              {
                  // Set the tricolor light to the correct state, either OFF if
                  // we are outside a txn or ON if we are in a TXN.
                  
                  // (*) SR898 ====================
                  DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                                     (fInSignedOn || fInSignedOff || fSAInTransaction) ? DM_TRICOLORLIGHT_ON : DM_TRICOLORLIGHT_OFF,
                                     DM_TRICOLORLIGHT_NORMALREQUEST);
                                     
                  trace(L7, _T("+ra.OnMediaLow, Message=%s"), ps.GetPSText(RA_MEDIA_NORMAL_ID, SCOT_LANGUAGE_PRIMARY));
                  ra.OnMediaLow(ps.GetPSText(RA_MEDIA_NORMAL_ID, SCOT_LANGUAGE_PRIMARY), false);  // TAR144557 & 152537 RAP not updating medio low status being fixed
                  trace(L7, _T("-ra.OnMediaLow"));
              } 
              DMCheckCashCounts(false);  // TAR 269759 - Ensure RAP media status is up to date
            }
            /* SR672
            else if((lDMLastCode == DM_PTR_NEAREMPTY || lDMLastCode == DM_PTR_EMPTY)
                        && nDMLastDev!=DMCLASS_SLIP
                    && !dm.IsDeviceRemote(nDMLastDev, 0))) */
            else if((lDMLastCode == PTR_SUE_JRN_NEAREMPTY || lDMLastCode == PTR_SUE_REC_NEAREMPTY
                    || lDMLastCode == PTR_SUE_JRN_EMPTY || lDMLastCode == PTR_SUE_REC_EMPTY) 
                    && nDMLastDev!=DMCLASS_SLIP
                    && !dm.IsDeviceRemote(nDMLastDev, 0))
            {
                // TAR#121450 Set Printer Paper status - if low, this will flash green tri-light 
                dm.SetPrinterPaperLow(true); //TAR 408696
                
                // (*) SR898 ====================
                DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                                   DM_TRICOLORLIGHT_BLINK_1HZ,
                                   DM_TRICOLORLIGHT_NORMALREQUEST);
                                   
                trace(L7, _T("+ra.OnMediaLow, Message=%s"), ps.GetPSText(RA_MEDIA_LOW_ID, SCOT_LANGUAGE_PRIMARY));
                ra.OnMediaLow(ps.GetPSText(RA_MEDIA_LOW_ID, SCOT_LANGUAGE_PRIMARY), true);  // Media Low is true, tell RAP
                trace(L7, _T("-ra.OnMediaLow"));
                DMCheckCashCounts(false);  // TAR 269759 - Ensure RAP media status is up to date
            } 
          }  // end if (lDMLastCode != DM_PTR_REQUEST_COMPLETED)
          else
          {
            if (nDMLastDev==DMCLASS_RECEIPT)
            {
               dm.nDMLastReceiptFunction = 0;
               lastDMReceiptCode = lDMLastCode;  // TAR 225852 - Update receipt printer status.
               if(hReceiptFile) 
               {
                  TBReleaseReceiptData(hReceiptFile);   //For synchronous printing
                  hReceiptFile = NULL;
               }
               // are you running on your PC?
               // with current emulator for journal, we are not getting DM_PTR_REQUEST_COMPLETED
               // so just enable the below #def
               // however, we are not giving SCOT source to many, fix should go with code
               // otherwise, app hangs on transaction end

            }
            // jounral data release
            if (nDMLastDev==DMCLASS_JOURNAL)
            {
               dm.nDMLastJournalFunction = 0;
               lastDMJournalCode = lDMLastCode;  // TAR 225852 - Update journal printer status.
               if(hJournalFile) 
               {
                  TBReleaseJournalData(hJournalFile);   //For synchronous printing
                  hJournalFile = NULL;
               }
            }

            if (dm.IsDeviceRemote(nDMLastDev, 0))
            {
               SMStateBase* pHandlerState = STATE_NULL; //HDMerge062802 starts here
               if (nDMLastDev==DMCLASS_RECEIPT)
                  pHandlerState = DMReceiptPrinter( lDMLastCode );
               else if (nDMLastDev==DMCLASS_JOURNAL)
                  pHandlerState = DMJournalPrinter( lDMLastCode );
               else if (nDMLastDev==DMCLASS_SLIP)
                  pHandlerState  = DMSlipPrinter( lDMLastCode );
               return pHandlerState; //HDMerge062802 ends here
            }
            if (m_bSignaturePrintingStatus)
            {
                m_bSignaturePrintingStatus = false;
                //Signature printing maintenance start
                WIN32_FIND_DATA FindFileData;
                HANDLE hFind;

                CString csSigbmps = TEMP_SIGNATURE_DIRECTORY;
                GET_PARTITION_PATH(TEMP_SIGNATURE_DIRECTORY, csSigbmps.GetBuffer(_MAX_PATH));
                csSigbmps.ReleaseBuffer();

                CString csFullFileName;
                csSigbmps += "*.*";
                hFind = FindFirstFile(csSigbmps, &FindFileData);
                long rc = 1;
                while ((hFind != INVALID_HANDLE_VALUE) && (rc))
                {
                    if ((FindFileData.cFileName != _T(".")) || (FindFileData.cFileName != _T("..")))
                    {
                        csFullFileName = TEMP_SIGNATURE_DIRECTORY;
                        GET_PARTITION_PATH(TEMP_SIGNATURE_DIRECTORY, csFullFileName.GetBuffer(_MAX_PATH));
                        csFullFileName.ReleaseBuffer();

                        csFullFileName += FindFileData.cFileName;
                        trace(L6,_T("Signature file deleted is: <%s>"), csFullFileName);
                        SCOTDeleteFile(csFullFileName);  //TAR 348946
                        rc = FindNextFile(hFind, &FindFileData);
                    }
                } 
                FindClose(hFind);
                WIN32_FIND_DATA fd;
                CString csLogFile, csBackupFile, csFileName;
                
                CString csSigRecPath = SIGRECEIPT_PATH;
                GET_PARTITION_PATH(csSigRecPath, csSigRecPath.GetBuffer(_MAX_PATH));
                csSigRecPath.ReleaseBuffer();

                csLogFile = csSigRecPath + CString(SIGRECEIPT_LOG_NAME) ;
                csBackupFile = csSigRecPath + CString(SIGRECEIPT_BACKUP_NAME) ;
                csFileName = csSigRecPath + CString(SIGRECEIPT_FILE_NAME);
            
                hFind = FindFirstFile(csLogFile, &fd);
                if (INVALID_HANDLE_VALUE == hFind)  // can not find the the file
                {
                    CFile::Rename(csFileName, csLogFile) ;
                }
                else
                {
                    FindClose(hFind); //must close handle
                    if ((hFind = FindFirstFile(csBackupFile, &fd)) != INVALID_HANDLE_VALUE)  // find the the file
                    {
                        FindClose(hFind); //must close handle
                        CFile::Remove(csBackupFile) ;
                    }
                    CFile::Rename(csLogFile, csBackupFile) ;
                    CFile::Rename(csFileName, csLogFile) ;
                }
                //Signature printing maintenance end
            }
            return STATE_NULL;
          }
            
          //SR672+
          sModelName=dm.GetDeviceModelName(nDMLastDev, nDMLastDevId); 
          switch (lDMLastCode)
          {
               //status events
               case PTR_SUE_COVER_OPEN:
               case PTR_SUE_COVER_OK:
               case PTR_SUE_JRN_EMPTY:
               case PTR_SUE_JRN_NEAREMPTY:
               case PTR_SUE_JRN_PAPEROK:
               case PTR_SUE_REC_EMPTY:
               case PTR_SUE_REC_NEAREMPTY:
               case PTR_SUE_REC_PAPEROK:
             
               case PTR_SUE_SLP_EMPTY:
               case PTR_SUE_SLP_NEAREMPTY:
               case PTR_SUE_SLP_PAPEROK:
                    lResultCode = lDMLastCode;
                    lExtRetCode = EXTENDED_RESULT_ANY;
                    break;
                //error events
               case OPOS_EPTR_COVER_OPEN:
               case OPOS_EPTR_REC_EMPTY:
               case OPOS_EPTR_JRN_EMPTY:
               case OPOS_EPTR_SLP_EMPTY:
                lResultCode = OPOS_E_EXTENDED;
                lExtRetCode = lDMLastCode;
                break;
               case NCR_EPTR_RPM_NOT_READY:
               case NCR_EPTR_CDS_DOOR_OPEN:
               case NCR_EPTR_RPM_MECH_ERROR:    
               case NCR_EPTR_RPM_PAPER_JAM: 
                    lResultCode = OPOS_E_FAILURE;
                    lExtRetCode = lDMLastCode;
                    break;
               case OPOS_E_NOHARDWARE:
               case OPOS_E_OFFLINE:
               case OPOS_E_FAILURE:
               case OPOS_E_TIMEOUT:
               case OPOS_E_BUSY:
                    lResultCode = lDMLastCode;
                    lExtRetCode = dm.GetResultCodeExtended(nDMLastDev, nDMLastDevId); 
                    break;
               default:
                    lResultCode = lDMLastCode;
                    lExtRetCode = EXTENDED_RESULT_ANY;
                    break;
                    
          }  // end switch
            csWork = DMGetDeviceErrorHTML(nDMLastDev, sModelName, sSubModel, lResultCode, lExtRetCode, 0, NULL, &fCheckHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
            if (csWork.Find(_T("Ignore")) !=-1)
                return STATE_NULL;
            fCaseHandled = true;  
            if (nDMLastDev==DMCLASS_RECEIPT)   
            {                               
                fDMReceiptError = true;                 
            }   
            else if (nDMLastDev==DMCLASS_JOURNAL)    
            {                                           
                fDMJournalError = true;                 
            }
        //SR672-
        }    // end case DMCLASS_RECEIPT, DMCLASS_JOURNAL, DMCLASS_SLIP
        break;
      case DMCLASS_MSR:
        {
          switch (lDMLastCode)
          {
          //SR672
          // NOTE** all the following #define must come from the NCRMSR.h when DMX replaces DM completely 
          // the CASE_HEALTH_NOW and CASE_IGNORE_NOW macro's will need to be changed also - LPM80202
          case NCR_BLANK_TRACKS:
          case NCR_READ_ERROR:
          case NCR_IOE_INVALID_CARD_INSERTED:
          case OPOS_EMSR_START:
          case OPOS_EMSR_END:
          case OPOS_EMSR_PARITY:
          case OPOS_EMSR_LRC: 
          //case DM_ERROR_MSR_READING: //TAR213352 CHU 08/30/02 so if it is this type of error, we don't go to SystemMessage
            waitingForCardRemoval = true;
            // +TAR 313995 - don't call the DMCheckHealth since it calls OnNORAPDataNeeded
            // which causes any immediate  intervention to be cleared
            dm.CheckHealth(nDMLastDev); // returns zero on success
            // -TAR 313995            trace(L6,_T("-Parse"));
            trace(L6,_T("-Parse"));         

            //TAR193038
            bBadCard = true;

            if (g_lDisplayFormRC != 0) //TAR 219971, 220074 BZ
            {
                /* 
                //Go to systemmessage4
                csWork = _T("DeviceErrorSigCap.xml:ERROR_SIGCAP_NO_COMM");  // Tar 227543
                if( _PINPAD_3X4X == dm.m_csPinpadRelease )
                {
                  csWork = _T("DeviceErrorSigCap.xml:ERROR_SIGCAP_NO_COMM_3X4X");   // Tar 310198
                }
                */
                //SR672
                bool bCkHealth = false;
                sModelName = dm.GetDeviceModelName(DMCLASS_SIGCAP, 0);
                csWork = DMGetDeviceErrorHTML(DMCLASS_SIGCAP, sModelName, sSubModel, ERROR_SIGCAP_NO_COMM, EXTENDED_RESULT_ANY, 0, NULL, &bCkHealth, 0, co.csPrimaryLanguageCode );
                // +TAR 313995
                //Notify RAP
                if (!sDeviceErrorTitle.IsEmpty())
                {
                    trace(L7, _T("+ra.OnRAPDataNeeded, ErrorMessage=%s"), sDeviceErrorTitle);
                    // write the error description on the receipt
                    ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + sDeviceErrorTitle);
                }
                // send a DataNeeded Approval
                cDataEntry.FromConfigData(DATAENTRYINIFILE, "DeviceFailure");
                m_View = DATANEEDED_DEVICEERROR;
                CString csSecInfo;
                if (!sDeviceErrorTitle.IsEmpty())
                    csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),sDeviceErrorTitle,ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), "", "", "", "");
                else
                    csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),_T("DeviceErrorSigCap.xml:ERROR_SIGCAP_NO_COMM"),ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), "", "", "", "");
                
                ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
                trace(L7, _T("-ra.OnRAPDataNeeded"));
                // -TAR 313995
                CREATE_AND_DISPENSE(SystemMessage4)(csWork,      // char *message
                                         PS_NOTEXT,
                                         PS_NOTEXT, // no prompt
                                         0,         // no value
                                         DMCLASS_SIGCAP,        // no device
                                         false, // no DM check health needed 
                                                    // since CheckHealth always return success          
                                                    // even the MSR is disconnected
                                         true);

            }
            else
            {
                if (co.fTenderIsMotorizedMSR)
                {
                  CREATE_AND_DISPENSE(CardMisRead)(MSG_MISREAD_MCRW, LTT_CARDMISREAD);
                }
                else
                {
                  CREATE_AND_DISPENSE(CardMisRead)(MSG_MISREAD_SLIDE, LTT_CARDMISREAD);
                }
            }
                   
            break;
          //ignored events
          case NCR_TRACKS_UNSUPPORTED:
          case NCR_CARD_PRESENT:
          case NCR_IOE_BIN_FULL:
          case NCR_IOE_BIN_REMOVED:
          case NCR_IOE_BIN_INSERTED:
          case NCR_IOE_CARD_INSERTED_STAGED:
          case NCR_IOE_CARD_DETECTED_ENTERING:
          case NCR_IOE_CARD_CAPTURED:
          case NCR_IOE_CARD_DETECTED:
          case NCR_IOE_DATA_AVAILABLE:
          case NCR_IOE_CARD_NOT_CAPTURED:
          case NCR_IOE_EJECT_CAPTURED:
          case MCR_GET_BACK_ONLINE:
              sModelName = dm.GetDeviceModelName(DMCLASS_MSR, 0);
                csWork = DMGetDeviceErrorHTML(DMCLASS_MSR, sModelName, sSubModel, lDMLastCode,EXT_RESULT_ANY, 0, NULL, &fCheckHealth, 0, co.csPrimaryLanguageCode );
                if (csWork.Find(_T("Ignored")) != -1)
                    return STATE_NULL;
                break;
            //Error events
          case OPOS_E_OFFLINE:
          case OPOS_E_FAILURE:
          case OPOS_E_TIMEOUT:
          case OPOS_E_NOHARDWARE:
              sModelName = dm.GetDeviceModelName(DMCLASS_MSR, 0);
                csWork = DMGetDeviceErrorHTML(DMCLASS_MSR, sModelName, sSubModel, lDMLastCode,EXT_RESULT_ANY, 0, NULL, &fCheckHealth, 0, co.csPrimaryLanguageCode );
                if (csWork.Find(_T("Ignored")) != -1)
                    return STATE_NULL;
                break;
          case NCR_SHUTTER_FAILURE:
          case NCR_IOE_DEVICE_INOP:
          case NCR_DEVICE_INOP:       
          case NCR_CARD_JAM:
          case NCR_NO_CARD:
          case MCR_GET_TEXTLEN_ERROR:
          case MCR_GET_CRCC_ERROR:
          //status events
          case NCR_IOE_SHUTTER_JAMMED_CLOSE:
          case MCR_GET_RESPONSE_TIMEOUT:
          case MCR_INITIALIZE_CMD_ERROR:
          case MCR_ENABLE_CMD_ERROR:
          case MCR_DISABLE_CMD_ERROR:
          case MCR_MAGTRACKREAD_CMD_ERROR:
          case MCR_STATUS_CMD_ERROR:
          case MCR_EJECT_CMD_ERROR:
          case MCR_CAPTURE_CMD_ERROR:
          case MCR_REVISION_CMD_ERROR:
          case MCR_RETRIEVE_CMD_ERROR:
            DMCheckHealth(nDMLastDev);
            if(NCR_CARD_JAM != lDMLastCode) //tar 266004 - Auto Eject of card after Jam can cause endless Jam Event / Eject loop
            {                               //will do the eject when continue is pressed on System Message screen
                DMMSREject();
            }
            sModelName = dm.GetDeviceModelName(DMCLASS_MSR, 0);
            csWork = DMGetDeviceErrorHTML(DMCLASS_MSR, sModelName, sSubModel,EVENTID_ANY, lDMLastCode, 0, NULL, &fCheckHealth, 0, co.csPrimaryLanguageCode );
            break;
          case NCR_IOE_CARD_REMOVED:
            trace(L6,_T("-Parse"));
            // notify any interested states that a card was removed
            // one state interested is the SMTakeCard state
            return DMCardRemoved();
          default:
            trace(L7, _T("Events %d ignored"), lDMLastCode );
            return STATE_NULL;
            break;
          }
        }
        break;
      case DMCLASS_CASHACCEPTOR:
        {
          // Tar 195273 save status before checking if lane closed
          // save the status for the media status screen
          lastDMCashAcceptorCode = lDMLastCode;
          if( bGLOBALFromLaneClose )    // Tar 192351 RF 012002
          {
              // Tar 279371+
              switch (lDMLastCode)
              {
               //SR672
              case BILL_STATUS_TAMPER_OUT:    
                  {
                      bLaneClosedTamperOut = true;
                      break;
                  }
              case BILL_STATUS_TAMPER_IN:
                  {
                      bLaneClosedTamperOut = false;
                      // +TAR 445735
                      // If this is a BNR, then the device has to be reset
                      // when the cashbox has been removed and reinserted.
                      if(dm.m_csCashDispenserModel == _CASHDISPENSER_BNR)
                      {
                          // If this health check fails, then the SO will send
                          // us another event.  So we should not need any 
                          // special handling if it doesn't pass.
                          (void)DMCheckHealth(DMCLASS_CASHACCEPTOR);
                      }
                      // -TAR 445735
                      break;
                  }
              default:
                  break;
              }
              // Tar 279371-
              return STATE_NULL;      // Stay in lane closed
          } 
          
          sModelName=dm.GetDeviceModelName(nDMLastDev, nDMLastDevId);
        
          //Tar 448255
          bool bIgnored = IsDeviceErrorIgnored(nDMLastDev, sModelName.GetBuffer(_MAX_PATH), sSubModel.GetBuffer(_MAX_PATH),lDMLastCode, EXT_RESULT_ANY);
          sModelName.ReleaseBuffer();
          sSubModel.ReleaseBuffer();
          

          if (!bIgnored)
          {
            long lExtCode=dm.GetResultCodeExtended(nDMLastDev, nDMLastDevId);
                
            csWork = DMGetDeviceErrorHTML(nDMLastDev, sModelName, sSubModel, lDMLastCode, lExtCode, 0, NULL, &fCheckHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
            if (csWork.Find(_T("Ignore")) !=-1)// TAR 451098
            {
                if (lExtCode == 51398)
                { 
                    (void)DMCheckHealth(DMCLASS_CASHACCEPTOR);
                }
                return STATE_NULL;
            }
          }
          else 
                return STATE_NULL;

          
          fCaseHandled = true;     
              
        }

        break;
      // Begin Coin Acceptor Modifcations
      case DMCLASS_COINACCEPTOR:
        {
          // Tar 195273 save status before checking if lane closed
          // save the status for the media status screen
          lastDMCoinAcceptorCode = lDMLastCode;
          if( bGLOBALFromLaneClose )    // Tar 192351 RF 012002
          {
                return STATE_NULL;      // Stay in lane closed
          } 

         //SR672 - Coin Acceptor
         sModelName=dm.GetDeviceModelName(nDMLastDev, nDMLastDevId);

        
         bool bIgnored =  IsDeviceErrorIgnored(nDMLastDev, sModelName.GetBuffer(_MAX_PATH), sSubModel.GetBuffer(_MAX_PATH),lDMLastCode, EXT_RESULT_ANY );
              sModelName.ReleaseBuffer();
              sSubModel.ReleaseBuffer();
         
         if (!bIgnored)
         {
            long lExtCode=dm.GetResultCodeExtended(nDMLastDev, nDMLastDevId, false); //Tar 445748
            csWork = DMGetDeviceErrorHTML(nDMLastDev, sModelName, sSubModel, lDMLastCode, lExtCode, 0, NULL, &fCheckHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
            if (csWork.Find(_T("Ignore")) !=-1)
                return STATE_NULL;
         }
          else
            return STATE_NULL;
          fCaseHandled = true; 
        
        }
        break;
        // End Coin Acceptor Modifications


      //begin CashRecycler FZ160000
        case DMCLASS_CASHRECYCLER:
        {
          /*// Tar 195273 save status before checking if lane closed
          // save the status for the media status screen
          lastDMCoinAcceptorCode = lDMLastCode;
          if( bGLOBALFromLaneClose )    // Tar 192351 RF 012002
          {
                return STATE_NULL;      // Stay in lane closed
          } */
            csStateName = EXTRACT_CLASSNAME(this); 
            //SR672
            if (dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_GLORY) != -1)
                csModel = _T("");
            else
                csModel = dm.m_csCashRecyclerModel;

            sModelName=dm.GetDeviceModelName(nDMLastDev, nDMLastDevId);

          switch (lDMLastCode)
          {

            case CHAN_STATUS_EMPTY: 
            case CHAN_STATUS_FULL:
                ScotError(INFO,SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_DM_CHECK_HEALTH);
                csWork = DMGetDeviceErrorHTML(nDMLastDev, sModelName, sSubModel, lDMLastCode, EXTENDED_RESULT_ANY, 0, NULL, &fCheckHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
                if (csWork.Find(_T("Ignore")) !=-1)
                    return STATE_NULL;
                fCaseHandled = true;            
                break;
            case OPOS_SUE_POWER_OFF:                        
            case OPOS_SUE_POWER_OFFLINE: 
              ScotError(INFO,SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_DM_CHECK_HEALTH);
              csWork = DMGetDeviceErrorHTML(nDMLastDev, sModelName, sSubModel, lDMLastCode, EXTENDED_RESULT_ANY, 0, NULL, &fCheckHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
                if (csWork.Find(_T("Ignore")) !=-1)
                    return STATE_NULL;
                fCaseHandled = true;            
                break;

            case OPOS_E_EXTENDED:   // 114
            case CHAN_STATUS_JAM:   //  31
            case OPOS_E_DISABLED:   // 105 TAR 356035

            case CHAN_DIEVT_DEPOSITERROR:  //3
            //case CHAN_DIEVT_DEPOSITRJ:       //4
            case CHAN_DIEVT_DEPOSITCASSETTEFULL:   //5
            case CHAN_DIEVT_DEPOSITSETERROR:   //6
            {
                 long lBillErrorCode = dm.CashRecyclerGetErrorCode(0x80);
                 CString csTempErrorCode;
                 if(lBillErrorCode)
                  {
                        csTempErrorCode.Format(_T("%d"),lBillErrorCode);
                        csTempErrorCode = csTempErrorCode.Left(2); // xml file only use first 2 digits
                        //csWork = GetDeviceErrorMnemonic(nDMLastDev, _BILLRECYCLER, 0, _ttol(csTempErrorCode), nDeviceErrorContext );
                        CString sSubModel(_T("Bill"));
                        csWork = DMGetDeviceErrorHTML(nDMLastDev, sModelName, sSubModel, EVENTID_ANY, _ttol(csTempErrorCode), 0, NULL, &fCheckHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
                        m_nCashRecyclerHardwareErrorCode = lBillErrorCode;

                  }
                  else 
                  {
                      CString sSubModel(_T("Coin"));
                      long lCoinErrorCode = dm.CashRecyclerGetErrorCode(0x01);
                      if(lCoinErrorCode)
                      {
                        csTempErrorCode.Format(_T("%d"),lCoinErrorCode);
                        csTempErrorCode = csTempErrorCode.Left(2); // xml file only use first 2 digits
                        //csWork = GetDeviceErrorMnemonic(nDMLastDev, _COINRECYCLER, 0,  _ttol(csTempErrorCode), nDeviceErrorContext );
                        
                        csWork = DMGetDeviceErrorHTML(nDMLastDev, sModelName, sSubModel, EVENTID_ANY, _ttol(csTempErrorCode), 0, NULL, &fCheckHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
                        m_nCashRecyclerHardwareErrorCode = lCoinErrorCode;
                      }
                      else
                      {
                          if(lDMLastCode == CHAN_STATUS_JAM)
                                //csWork = GetDeviceErrorMnemonic(nDMLastDev, _T(""), CHAN_STATUS_JAM, 0, nDeviceErrorContext );
                                csWork = DMGetDeviceErrorHTML(nDMLastDev, sModelName, sSubModel, CHAN_STATUS_JAM, EVENTID_ANY, 0, NULL, &fCheckHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
                      }
                  }

                  CString sParms;
                  sParms.Format(_T("%d"), m_nCashRecyclerHardwareErrorCode);
                  if (!csWork.IsEmpty() && !sParms.IsEmpty() )
                  {
                    ParseDeviceErrorHTML(csWork.GetBuffer(_MAX_PATH),  sParms.GetBuffer(_MAX_PATH), _T("%d"));
                    csWork.ReleaseBuffer();
                    sParms.ReleaseBuffer();
                  }
                  //fCheckHealth = true;              
                  fCaseHandled = true;  
                  break;
            }

            case CHAN_DIEVT_CASSETEWAIT: //1
                return OnMotionCashOutSensor();
            case CHAN_DIEVT_PULLOUT: //7
                return OnNoMotionCashOutSensor();

            case CHAN_STATUS_JAMOK:
                // TAR 315616
                if(m_nCashRecyclerFailedRejectAmt > 0)
                {
                    CString csCC;

                    trace(L6, _T("Dispensing currency amount %d that could not be rejected in an earlier attempt."),
                                m_nCashRecyclerFailedRejectAmt);
                    m_nCashRecyclerFailedRejectAmt -= DMDispenseChange(m_nCashRecyclerFailedRejectAmt,
                                                        csCC);

                    return STATE_NULL;
                }
                // TAR 315616

            default:
             trace(L6,_T("CashRecycler event ignored: %d"), lDMLastCode);
              return STATE_NULL;
            }
            // add the low currency info to the message in csWork
            //csWork = DMCheckCashCounts(false);

            //CString csHighCurrencies, csCoinAlert,csCashAlert;
            //add the cash and coin device alerts
            //DMCheckCashCounts(false, csHighCurrencies,csCoinAlert, csCashAlert);
            //csWork += csCoinAlert;
            //csWork += csCashAlert;
            //dp185016 support glory device - support currency is high
        }
        break;
       //End of Cash Recycler

      case DMCLASS_CASHCHANGER:
        {
          if( bGLOBALFromLaneClose )    // Tar 192351 RF 012002
          {
                return STATE_NULL;      // Stay in lane closed
          } 

          CString sMName=dm.GetDeviceModelName(nDMLastDev, nDMLastDevId);
          bool bIgnored =  IsDeviceErrorIgnored(nDMLastDev, sMName.GetBuffer(_MAX_PATH), _T(""),lDMLastCode, EXT_RESULT_ANY );
          sMName.ReleaseBuffer();
              
          if (bIgnored)
          {
              trace(L6,_T("Ignore"));
              return STATE_NULL;
          }
           long lExtCode=dm.GetResultCodeExtended(nDMLastDev, nDMLastDevId);
          switch (lDMLastCode)
          {
              // add the low currency info to the message in csWork
              csWork += DMCheckCashCounts(false);
            //SR672
              // Need to distinguish whether coin or bill error.   
                /*
               case DM_CC_STATUS_INOPERABLE:
               case DM_CC_STATUS_JAM:                        
              case DM_CC_STATUS_HOPPER_JAM:
              */
             
             
              case CASH_STATUS_INOPERABLE:
              case CASH_STATUS_JAM:                        
              case NCR_STATUS_HOPPER_JAM:
              {
                 long lCoinStatus = 0;
                 long lBillStatus = 0;
                 
                 dm.CashChangerGetStatusValues(&lCoinStatus, &lBillStatus);
                 trace(L6,_T("CashChangerGetStatusValues lCoinStatus=%d, lBillStatus=%d"), lCoinStatus, lBillStatus);
                 switch (lCoinStatus)
                 {  
                 case NCR_STATUS_HOPPER_JAM:
                        dm.m_csCoinDispenserModel.MakeUpper();
                        if (dm.m_csCoinDispenserModel.Find(_T("HOPPER")) != -1  )
                        {
                         if (dm.m_csCoinDispenserStage.IsEmpty() )  //Hopper tiny plan
                         {
                            trace(L6,_T("Ignore hopper jam status"));
                            return STATE_NULL;
                         }
                        }
                        s2=dm.GetDeviceModelName(nDMLastDev, nDMLastDevId);
                        sSubModel = _T("Coin");                     
                        csWork = DMGetDeviceErrorHTML(nDMLastDev, s2, sSubModel, lCoinStatus, lExtCode, 0, NULL, &fCheckHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
                        fCaseHandled = true;
                     break; 
                    case CASH_STATUS_JAM:
                    case CASH_STATUS_INOPERABLE: 
                        s2=dm.GetDeviceModelName(nDMLastDev, nDMLastDevId);
                        sSubModel = _T("Coin");
                        //GetDeviceErrorHTML(sHTML, nDMLastDev, s2.GetBuffer(_MAX_PATH), _T("Coin"), lCoinStatus, -99, 0, NULL, &fCheckHealth );
                        csWork = DMGetDeviceErrorHTML(nDMLastDev, s2, sSubModel, lCoinStatus, lExtCode, 0, NULL, &fCheckHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
                     fCaseHandled = true;
                     break;  
                 }
                 //+TAR 440763
                if ((lCoinStatus == NCR_STATUS_HOPPER_JAM || lCoinStatus == CASH_STATUS_JAM) 
                     && !s2.CompareNoCase(_T("BCR")))
                {       
                    CString sParms = FormatDeviceInfoInput(lExtCode);

                    if (!csWork.IsEmpty() && !sParms.IsEmpty())
                    {
                        TCHAR sHTML2[_MAX_PATH];
                        _tcscpy(sHTML2, csWork.GetBuffer(_MAX_PATH));
                        ParseDeviceErrorHTML(sHTML2, sParms.GetBuffer(_MAX_PATH), _T("%s"));
                        csWork.ReleaseBuffer();
                        csWork = sHTML2;
                        sParms.ReleaseBuffer();
                     }
                }//-TAR 440763
                 switch (lBillStatus)
                 {   
                   case NCR_STATUS_HOPPER_JAM:
                   case CASH_STATUS_JAM:
                     s2=dm.GetDeviceModelName(nDMLastDev, nDMLastDevId);
                     sSubModel = _T("Bill");
                     csWork = DMGetDeviceErrorHTML(nDMLastDev, s2, sSubModel, lBillStatus, lExtCode, 0, NULL, &fCheckHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
                     fCaseHandled = true;
                     break; 
                   case CASH_STATUS_INOPERABLE: 
                     if(nDMLastDev == m_nDMSavedDev )   // 277025 Ignore this error since there is already an error reported with the same device.
                     {
                        trace(L6,_T("-DMParse this error is ignored since there is already an error reported with the same device."));
                        return STATE_NULL;
                     }
                     else 
                         return DMCashChangerInOperable();  //TAR225069 CHU/Dale 061903
                     break;    
                 }
                //SR672
                 if( lCoinStatus != NCR_STATUS_HOPPER_JAM &&
                     lCoinStatus != CASH_STATUS_JAM &&
                     lCoinStatus != CASH_STATUS_INOPERABLE &&
                     lBillStatus != NCR_STATUS_HOPPER_JAM &&
                     lBillStatus != CASH_STATUS_JAM &&
                     lBillStatus != CASH_STATUS_INOPERABLE)  
                 {
                     trace(L6,_T("Ignore"));
                     return STATE_NULL;
                 }
                 break;
              }

              // notify any interested states that cash was removed
              // one state interested is the SMTakeChange state
              // another is the take money state
              //case DM_CC_STATUS_BILLS_TAKEN: 
              case CASH_STATUS_BILLS_TAKEN:
                  trace(L6,_T("-Parse"));
                  lLastDispense = 0;
                  return DMCashRemoved();
              default: 
                    CString s1=dm.GetDeviceModelName(nDMLastDev, nDMLastDevId);
                    sSubModel = _T("");
                    csWork = DMGetDeviceErrorHTML(nDMLastDev, s1, sSubModel, lDMLastCode, lExtCode, 0, NULL, &fCheckHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
                     
                    if (csWork.Find(_T("Ignore")) !=-1)
                        return STATE_NULL;
                    fCaseHandled = true; 
                    break;

          }  // End switch
          
        }
        break;
      case DMCLASS_SCANNER: //SR672
        {
          switch (lDMLastCode)
          {
              case NCR_ESCAN_DATALOSS:
              case NCR_ESCAN_BADTAGSUFFIX:
              case NCR_ESCAN_BADTAGPREFIX:
              case NCR_ESCAN_BADTAG:
              case NCR_ESCAN_SERIAL:
                    sModelName = dm.GetDeviceModelName(DMCLASS_SCANNER, 0);
                    csWork = DMGetDeviceErrorHTML(DMCLASS_SCANNER, sModelName, sSubModel, EVENTID_ANY, lDMLastCode, 0, NULL, &fCheckHealth, 0, co.csPrimaryLanguageCode );
                    break;
              case OPOS_E_OFFLINE:
              case OPOS_E_FAILURE:
              case OPOS_E_TIMEOUT:
              case OPOS_E_NOHARDWARE:
                sModelName = dm.GetDeviceModelName(DMCLASS_SCANNER, 0);
                csWork = DMGetDeviceErrorHTML(DMCLASS_SCANNER, sModelName, sSubModel,lDMLastCode, EVENTID_ANY, 0, NULL, &fCheckHealth, 0, co.csPrimaryLanguageCode );
                break;
              default:  // TAR 445296
                sModelName = dm.GetDeviceModelName(DMCLASS_SCANNER, 0);
                csWork = DMGetDeviceErrorHTML(DMCLASS_SCANNER, sModelName, sSubModel,lDMLastCode, EVENTID_ANY, 0, NULL, &fCheckHealth, 0, co.csPrimaryLanguageCode );
                if (csWork.Find(_T("Ignore")) !=-1)
                        return STATE_NULL;
                break;
          }
        }
        break;
      case DMCLASS_SCALE:
        trace(L6,_T("-Parse"));
        return DMSetScaleWeight(fCheckHealth);
      
      case DMCLASS_CASHDRAWER: //Ignore the open/close events of the cashdrawer awk.1
        {
            // Need to put some code in here to handle difference
            // between exit gate and cash drawer.
            return DMCashDrawer( lDMLastCode );
            
        }
        break;
      
      case DMCLASS_ENCRYPTOR: //SR672
        {
          switch (lDMLastCode)
          {
              /* Fix me later
                    CASE_IGNORE_NOW(STATUS_UNEXPECTED); //pd980715
                    CASE_HEALTH_NOW(ERROR_UNEXPECTED);
                    CASE_HEALTH_NOW(ERROR_GENERALFAILURE);
                    */
                        
          case DM_ENCRYPT_SHORT_PIN:
            trace(L6,_T("-Parse CASE_VAL(ENCRYPTOR, ENCRYPT_SHORT_PIN"));
            return this->DMEncryptorPinIsTooShort();
          }
        }
        break;
        //Yafu

      case DMCLASS_SIGCAP:
        {
            switch (lDMLastCode) 
            {
                case DM_SIGCAP_STARTED:
                    // TAR: 126645 : ICI: At Sig Needed, 'touch Ok after signing, time out returns to wrong state
                    return DMSigcapStarted();
                case DM_SIGCAP_ENDED:
                    return DMSigcapEnded();
                case DM_SIGCAP_FAILURE: //TAR182614  LPM THP migrated from 2.1.1 E with differences in .h files  101101
                    return DMSigcapFailure();
            }
        }
        break;
      case DMCLASS_MOTION_SENSOR:  //SR672
        {
            switch (lDMLastCode)
               {
               case MOT_MOTION:
                  if ( nDMLastDevId == 2 ) //Cash Out sensor motion
                  {
                     trace(L6,_T("DM Cash Out Sensor detected motion"));
                     trace(L6,_T("-Parse"));
                     return OnMotionCashOutSensor();
                  }
                  else
                  {
                      trace(L6,_T("DM Unknown Sensor - motion"));
                      trace(L6,_T("-Parse"));
                  }
                  return STATE_NULL;
               case MOT_NO_MOTION:
                  if ( nDMLastDevId == 0 )
                  {
                     trace(L6,_T("DM EAS MotionSensor detected no motion"));
                     trace(L6,_T("-Parse"));
                     return DMdf_EASNoMotion();
                  }
                  else if ( nDMLastDevId == 1 ) //coupon sensor
                  {
                     trace(L6,_T("DM Coupon Sensor detected coupon slip"));
                     trace(L6,_T("-Parse"));
                     return OnCouponSensor();
                  }
                  else if ( nDMLastDevId == 2 ) //Cash Out sensor
                  {
                     trace(L6,_T("DM Cash Out Sensor detected no motion"));
                     trace(L6,_T("-Parse"));
                     return OnNoMotionCashOutSensor();
                  }
                  else
                  {
                      trace(L6,_T("DM Unknown Sensor - no motion"));
                      trace(L6,_T("-Parse"));
                  }

                  return STATE_NULL;

                case MOT_STATUS_FAILURE:  //TAR246799
                    if ( nDMLastDevId == 0 )
                    {
                         trace(L6,_T("DM EAS MotionSensor status failure"));
                         trace(L6,_T("-Parse"));
                        return STATE_NULL;
                    }
                    else if ( nDMLastDevId == 1 ) //coupon sensor
                     {
                        trace(L6,_T("DM Coupon Sensor status failure"));
                        //+dp185016 tar 277604
                        //csWork.Format(ERRFMT, ps.GetPSText(MSG_COUPONSENSOR_FAILURE,SCOT_LANGUAGE_PRIMARY),                     \
                        //                            DMCLASS_MOTION_SENSOR, lDMLastCode);                                          \
                        //
                        //csWork = _T("DeviceErrorMotionSensor.xml:NCR_DIGITAL_INTERFACE_ERROR");
                        //-dp185016 tar 277604
                        sModelName = dm.GetDeviceModelName(DMCLASS_MOTION_SENSOR, 0);
                        csWork = DMGetDeviceErrorHTML(DMCLASS_MOTION_SENSOR, sModelName, sSubModel, NCR_DIGITAL_INTERFACE_ERROR, EXTENDED_RESULT_ANY, 0, NULL, &fCheckHealth, 0, co.csPrimaryLanguageCode );
                        fCaseHandled = true;
                        break;
                        //go to SystemMessage4 without CheckHealth
                    }
                    else if ( nDMLastDevId == 2 ) //Cash Out sensor
                      {
                        trace(L6,_T("DM Cash Out Sensor status failure"));
                        trace(L6,_T("-Parse"));
                        return STATE_NULL;
                      }
                    else
                    {
                        trace(L6,_T("DM Unknown Sensor - status failure"));
                        trace(L6,_T("-Parse"));
                        return STATE_NULL;
                    }

                case MOT_BLOCKED:
                    if ( nDMLastDevId == 1 )        //coupon sensor 
                    {
                        //parsing error message MOT_BLOCKED
                        trace(L6,_T("DM Coupon Sensor status blocked"));
                        // +SSCOADK-3539: Incorrect way to get screen text
						// csWork = _T("DeviceErrorMotionSensor.xml:NCR_MOTION_SENSOR_BLOCKED");
                 		sModelName = dm.GetDeviceModelName(DMCLASS_MOTION_SENSOR, 0);
						csWork = DMGetDeviceErrorHTML(DMCLASS_MOTION_SENSOR, sModelName, sSubModel, NCR_MOTION_SENSOR_BLOCKED, EXTENDED_RESULT_ANY, 0, NULL, &fCheckHealth, 0, co.csPrimaryLanguageCode );

						// -SSCOADK-3539 fCheckHealth = true;
                        fCaseHandled = true;
                        break;
                    }
                    else
                    {
                        trace(L6,_T("DM Unknown Sensor"));
                        return STATE_NULL;
                    }
    
                case MOT_EXCEED_MAX:
                    if ( nDMLastDevId == 1 )        //coupon sensor
                    {
                        trace(L6,_T("DM Coupon Sensor status motion exceeds maximum cycles being set."));
                        //post error message to the RAP Lane receipt 
                        ra.OnNormalItem(RAP_RECEIPT_LINE_HIGHLIGHT + ps.GetPSText(MSG_COUPONSENSOR_EXCEED_TEXT, SCOT_LANGUAGE_PRIMARY)); 
                        return STATE_NULL;
                    }
                    else
                    {
                        trace(L6,_T("DM Unknown Sensor"));
                        return STATE_NULL;
                    }   

               default:
                   trace(L6,_T("DM Unknown Motion Sensor Status"));   //tar 245719
                   trace(L6,_T("-Parse"));

                   return STATE_NULL;

                

               }
        }
        break;
      case DMCLASS_EASDEACTIVATOR:
         {
         if ( lDMLastCode == EAS_STATUS_DEACTIVATED 
            && nDMLastDevId == 1 )
            {
             return OnUnDeActivatedTheftTag();
            }
         return STATE_NULL;
         break;
         }
      case DMCLASS_MISC:      //SR672  // Tar 208394
          {   // if in Assist Mode, don't attempt to report MiscIF error
              if(!fStateAssist)
              {
                 //+dp185016 tar 277608
                 //csWork = _T("OnTrak System Control Interface\n"); //+dp185016 tar # 277608
                 //csWork += ps.GetPSText(MSG_UNKNOWN_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY); //+dp185016 tar # 277608
                 //csWork = _T("DeviceErrorMisc.xml:NCR_TRILIGHT_ERROR"); 
                 //-dp185016
                sModelName = dm.GetDeviceModelName(DMCLASS_MISC, 0);
                csWork = DMGetDeviceErrorHTML(DMCLASS_MISC, sModelName, sSubModel, NCR_TRILIGHT_ERROR, EXTENDED_RESULT_ANY, 0, NULL, &fCheckHealth, 0, co.csPrimaryLanguageCode );
                 //Notify RAP
                if (!sDeviceErrorTitle.IsEmpty())
                {
                    trace(L7, _T("+ra.OnRAPDataNeeded, ErrorMessage=%s"), sDeviceErrorTitle);
                    // write the error description on the receipt
                    ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + sDeviceErrorTitle);
                }
                 // save the previous view
                 m_PrevView = m_View;
                 // send a DataNeeded Approval
                 cDataEntry.FromConfigData(co.csDataEntryINIFile, "DeviceFailure");
                 m_View = DATANEEDED_DEVICEERROR;
                 CString csSecInfo;
                 if (!sDeviceErrorTitle.IsEmpty())
                    csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),sDeviceErrorTitle,ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), "", "", "", "");
                 else
                    csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),_T("DeviceErrorMisc.xml:NCR_TRILIGHT_ERROR"),ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), "", "", "", "");
                 
                 ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
                 trace(L7, _T("-ra.OnRAPDataNeeded"));
                 CREATE_AND_DISPENSE(SystemMessage4)(csWork,           // char *message
                        PS_NOTEXT,        // no canned message
                        PS_NOTEXT,        // no prompt
                        0,                // no value
                        nDMLastDev,       // device reporting the error or -1
                        true,    // T/F need to issue CheckHealth after operator clears problem
                        true);
              }
              else
              {   // be sure to turn this flag off for assist mode
                  dm.fPendingMiscIFError = false;
              }

          break;
          }
    default:
        {
            /* fix me later
          switch (lDMLastCode)
          {
                    CASE_HEALTH_NOW(STATUS_UNEXPECTED);
                    CASE_HEALTH_NOW(ERROR_UNEXPECTED);
                    CASE_HEALTH_NOW(ERROR_GENERALFAILURE);
          }*/
        }
        break;
      }
    }
    break;
   case DM_DIRECTIO:
      {
          switch (nDMLastDev)
          {
          case DMCLASS_SCANNER:
              {
                  if (me->dmInfo.lDirectIONumber == NCRDIOE_SCANNER_ACTIVE)
                  {
                      trace(L7,_T("ScannerDirectIO"));
                      if ( (me->dmInfo.lDirectIONumData == 1)/* && co.IsRemoteScannerConnected() */)
                          return DMRemoteScannerConnect(true);
                      else
                          return DMRemoteScannerConnect(false);
                  }
              }
              break;

          case DMCLASS_CASHRECYCLER:    // Only relevant to SafePay for now.
              trace(L6, _T("Ignoring cash recycler DM_DIRECTIO event [%d]"), lDMLastCode); 
              return STATE_NULL;              

          default:
              break;
          }
          // Clean up currently unused data
          if (me->dmInfo.vDPtr)
              delete[] me->dmInfo.vDPtr;
          fCaseHandled = true;
      }
      break;
  case DM_DATA:
    // the data from DM can be binary or text
    // it is passed to here as a length and a
    // void *buffer new'd by PostDM
    // here we keep our own buffer and delete
    // the PostDM buffer
    // to avoid excessive new's and delete's
    // we keep one buffer and grow it only if needed.
    // if our buffer is too small, 'delete' it and
    // 'new' a bigger one
    if (me->dmInfo.vDPtr)
    {
      /*if ( nDMLastLength > 1000 )
      {
        trace(L6,_T("-Parse DM_DATA too large"));
        return STATE_NULL;          // too big
      }*/  //SR 93.4 Removed the 1000 byte limit
            
      if ( nDMLastLength < 1 )
      {
        trace(L6,_T("-Parse DM_DATA too small"));
        return STATE_NULL;          // too small
      }
            
      // void * data was passed
      /* Merge fix
      if (nDMLastLength+1>nDMMaxLength)
      {
        if (cDMLastData) delete cDMLastData;
        nDMMaxLength = nDMLastLength+1;
        cDMLastData = new char[nDMMaxLength];
        trace(L8,"Getting bigger DM data buffer %d",nDMMaxLength);
      }
      */
      if (cDMLastData) delete [] cDMLastData; //SSCOP-4165
      cDMLastData = new TCHAR [nDMLastLength+1];
      // End Merge

      // copy the new data to our buffer
      // the MsgObject graciously allocates 1 byte more and
      // tacks on a 0x00 so we can copy len + 1
      memcpy(cDMLastData, me->dmInfo.vDPtr, nDMLastLength * sizeof(_TCHAR));
      cDMLastData[nDMLastLength] = NULL;    // set last character as a NULL
      lLastData = *(long *)cDMLastData;
        
      // now we MUST delete the PostDM new'd buffer
      delete[] me->dmInfo.vDPtr;
            
      //Tar219151 don't set to L6 unless it's an emergency...it
      //will expose customer data.
      // TAR 310164 Don't trace customer sensitive data 
      if (nDMLastDev == DMCLASS_MSR)
      {
          trace(L6,_T("DM Data MSR Length %d"), nDMLastLength);
      }
      else
      {
          if (co.csUserExitString5 == _T("OmitCustSensitiveDataInTraces"))
             to.HexTrace(L7,_T("DM Data"),cDMLastData,nDMLastLength); //Tar219151
          else
            to.HexTrace(L6,_T("DM Data"),cDMLastData,nDMLastLength);  
      }

    }
        
    {
    switch (nDMLastDev)
    {
      case DMCLASS_CASHACCEPTOR:
    {
        csDMEncryptedPin = EMPTY_STRING;
        nDMLastAcceptor = 0;
        // BeginCodeLocale
        // if (memcmp(cDMLastData,"USD",3)!=0)
        // {
        //   ScotError(RETURN,"Invalid Cash inserted <%s>",cDMLastData);
        //   DMCashAcceptorReject();  // Coin Acceptor Modifications
        //   trace(L6,"-Parse cash not USD");
        //   return STATE_NULL;
        // }
        // EndCodeLocale

        nDMLastAcceptor = _ttol(cDMLastData+3);  // save amt of cash
        if (nDMLastAcceptor<=0)
        {
          ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_DM_CASH,_T("%d"),nDMLastAcceptor);
          DMCashAcceptorReject();  // Coin Acceptor Modifications
          nDMLastAcceptor = 0;
          trace(L6,_T("-Parse"));
          return STATE_NULL;
        }
        //+dp185016 Recycler Platform Issue #16
        csStateName = this->GetRuntimeClass()->m_lpszClassName;
        if (DMCashRecyclerShouldReject(csStateName))    
        {
            nDMLastBillEvent = nDMLastAcceptor;             // TAR 311332
            //Added error handling
            long rc = DMCashAcceptorReject();  //just reject the bills
            if (rc < 0)         //  most likely out of change
            {
                return DMCashChangerError(nDMLastBillEvent);   //RFC 330995
            }
            else if (nDMLastBillEvent > 0)                     //RFC 330995
            {
                nDMLastBillEvent = 0;                           // TAR 311332
                trace(L6,_T("In %s screen, reject bills."), csStateName);
                trace(L6,_T("-Parse"));
            }
            return STATE_NULL;
        }
        //-dp185016 Recycler Platform Issue #16
        
        trace(L6,_T("-Parse"));
                
        if (nDMLastAcceptor>0)
        {
            nDMLastBillEvent = nDMLastAcceptor;
            nDMLastCoinEvent = 0;
            if(!m_bCashRecyclerStopDeposit &&
               !dm.IsPauseCountsActive(DMCLASS_CASHRECYCLER) &&
               !rp.IsCashManagementInProgress())
            {
              // If IsPauseCountsActive, then we are in cash management and the
              // currency device is a cash recycler.
                nDMCashInserted += nDMLastAcceptor;
            }
            trace(L7, _T("+rp.CurrencyAccepted"));
            CString csDen;
            if (nDMLastBillEvent)
            {
                rp.m_bBill = true;
                csDen = (LPTSTR) cDMLastData;
                csDen.Delete(0,3);
            }
            rp.CurrencyAccepted(rp.m_bBill,csDen,1,nDMLastBillEvent);
            
            if(dm.m_csCashRecyclerModel.IsEmpty())
            {
                //quantity is always one because it will parse data every cash insert
                dmcc.CurrencyAccepted(rp.m_bBill, cDMLastData, 1); //TAR 448434
                DMHandleAcceptorHigh();
            }

            trace(L7, _T("-rp.CurrencyAccepted"));
            
            // Inserted cash in CM should is not considered as tender so 
            // showing the "Begin Cash Tender" in the e-receipt such in 
            // SMAuthorization, RAP, or even customer mode is not appropriate.
            if(!m_cmMgr.IsInCashManagement())   //SSCOP-2258
            {
                SendCurrencyAcceptedToRAP(nDMLastBillEvent);
            }
            
            if(dm.m_csCashRecyclerModel.IsEmpty() )
                DMCashAcceptorAccept();  // Coin Acceptor Modifications
        }
        // prenotify event to TB begin
        if(!m_bCashRecyclerStopDeposit)
        {
            // set event data to the value of the inserted amount
            CString csCashValue;
            csCashValue.Format(_T("%d"), nDMLastAcceptor);
            sNotifyIn.SetEventData(_T("CASHACCEPTOR_DATA"), csCashValue, csCashValue.GetLength());
            SMStateBase* pCashTBState = PreNotifySCOTEvent2TB(me, sNotifyIn, sNotifyOut);
            sNotifyIn.CleanUp();
           if (!sNotifyOut.bAllowDefaultProcessing)
            {
              trace(L4, _T("PreNotifyDMEvent2TB() returns to a new state"));
              return pCashTBState;
            }
        }
        // prenotify event to TB end
              //Glory 

          CString csDepositCashList;

          if(DMCashRecyclerIsTenderOver(csStateName))                   
          {
              long nCashInsertFromCashRecycler;       
              if(DMHookNotifyTenderOver())  
              {
                //SSCOP-2400
                ps.ShowAmountDueAmountPaid(lBalanceDue, lTotalPaid + nDMCashInserted);

                nCashInsertFromCashRecycler = dm.CashRecyclerStopDeposit(csDepositCashList) - m_nDMCashPaid;
                nDMCashInserted = (nDMCashInserted >= nCashInsertFromCashRecycler)? nDMCashInserted: nCashInsertFromCashRecycler;

                m_bCashRecyclerStopDeposit = true;

                return DMAcceptor();        // cash amount
              }
          }
        if(!m_bCashRecyclerStopDeposit || 
          (!dm.m_csCashRecyclerModel.IsEmpty() && csStateName.Find(_T("CashManagement")) != -1)) //dp185016 tar 307726
            return DMAcceptor();        // cash amount
        else
        {
            trace(L6, _T("For CashRecycler, ignore data event for %d after stopping deposit"), nDMLastAcceptor );           
            return STATE_NULL;
        }
    }

      // Begin Coin Acceptor Modifications
      case DMCLASS_COINACCEPTOR:
    {
        csDMEncryptedPin = EMPTY_STRING;
        nDMLastAcceptor = 0;
        // BeginCodeLocale
        // if (memcmp(cDMLastData,"USD",3)!=0)
        // {
        //   ScotError(RETURN,"Invalid Coin inserted <%s>",cDMLastData);
        //   DMCoinAcceptorReject();
        //   trace(L6,"-Parse cash not USD");
        //   return STATE_NULL;
        // }
        // EndCodeLocale

        nDMLastAcceptor = _ttol(cDMLastData+3);  // save amt of cash
        if (nDMLastAcceptor<=0)
        {
          ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_DM_COIN,_T("%d"),nDMLastAcceptor);
          DMCoinAcceptorReject();
          nDMLastAcceptor = 0;
          trace(L6,_T("-Parse"));
          return STATE_NULL;
        }
        //+dp185016 Recycler Platform Issue #16
        csStateName = this->GetRuntimeClass()->m_lpszClassName;
        if (DMCashRecyclerShouldReject(csStateName))
        {
            nDMLastCoinEvent = nDMLastAcceptor;             // TAR 311332
            //Added error handling
            long rc = DMCoinAcceptorReject();  //just reject the coins
            if (rc < 0)         //  most likely out of change
            {
                return DMCashChangerError(nDMLastCoinEvent);   //RFC 330995
            }
            else if (nDMLastCoinEvent > 0)                     //RFC 330995
            {

                nDMLastCoinEvent = 0;                               // TAR 311332
                trace(L6,_T("In %s screen, reject coins."), csStateName);
                trace(L6,_T("-Parse"));
            }
            return STATE_NULL;
        }
        //-dp185016 Recycler Platform Issue #16

        trace(L6,_T("-Parse"));
                
        if (nDMLastAcceptor>0)
        {
          nDMLastCoinEvent = nDMLastAcceptor;
          nDMLastBillEvent = 0;
          if(!m_bCashRecyclerStopDeposit && 
             !dm.IsPauseCountsActive(DMCLASS_CASHRECYCLER) &&
             !rp.IsCashManagementInProgress())
          {
              // If IsPauseCountsActive, then we are in cash management and the
              // currency device is a cash recycler.
                nDMCashInserted += nDMLastAcceptor;
          }

          trace(L7, _T("+rp.CurrencyAccepted"));
          CString csDen;

          if (nDMLastCoinEvent)
          {
              rp.m_bBill = false;
              csDen      = (LPTSTR)(cDMLastData);
              csDen.Delete(0,3);

          }
          rp.CurrencyAccepted(rp.m_bBill,csDen,1,nDMLastCoinEvent);
        if(dm.m_csCashRecyclerModel.IsEmpty() )
           {
              //quantity is always one because it will parse data every 
              // coin insert but we need to pass the whole string(“USD10”for example).
              dmcc.CurrencyAccepted(rp.m_bBill, cDMLastData, 1); //TAR 448434
              DMHandleAcceptorHigh();
           }
          trace(L7, _T("-rp.CurrencyAccepted"));
          // TAR 320363 start
          //SR672 
          if (COIN_STATUS_REJECTED == lastDMCoinAcceptorCode)
                lastDMCoinAcceptorCode = COIN_STATUS_OK;
          // TAR 320363 end
          
          // Inserted cash in CM should is not considered as tender so 
          // showing the "Begin Cash Tender" in the e-receipt such in 
          // SMAuthorization, RAP, or even customer mode is not appropriate.
          if(!m_cmMgr.IsInCashManagement()) //SSCOP-2258
          {
              SendCurrencyAcceptedToRAP(nDMLastCoinEvent);
          }
          
        }

        if(!m_bCashRecyclerStopDeposit)
        {
            // prenotify event to TB begin
            // set event data to the value of the inserted amount
            CString csCashValue;
            csCashValue.Format(_T("%d"), nDMLastAcceptor);
            sNotifyIn.SetEventData(_T("CASHACCEPTOR_DATA"), csCashValue, csCashValue.GetLength());
            SMStateBase* pTBState = PreNotifySCOTEvent2TB(me, sNotifyIn, sNotifyOut);
            sNotifyIn.CleanUp();
            if (!sNotifyOut.bAllowDefaultProcessing)
            {
              trace(L4, _T("PreNotifyDMEvent2TB() returns to a new state"));
              return pTBState;
            }
            // prenotify event to TB end
        }
              //Glory 

          CString csDepositCashList;          
          if(DMCashRecyclerIsTenderOver(csStateName))                   
          {
              long nCashInsertFromCashRecycler;       
              if(DMHookNotifyTenderOver())  
              {
                //SSCOP-2400
                ps.ShowAmountDueAmountPaid(lBalanceDue, lTotalPaid + nDMCashInserted);

                nCashInsertFromCashRecycler = dm.CashRecyclerStopDeposit(csDepositCashList) - m_nDMCashPaid; 
                nDMCashInserted = (nDMCashInserted >= nCashInsertFromCashRecycler)? nDMCashInserted: nCashInsertFromCashRecycler;

                m_bCashRecyclerStopDeposit = true;
                return DMAcceptor();        // cash amount          
              }
          }

    if(!m_bCashRecyclerStopDeposit ||
       (!dm.m_csCashRecyclerModel.IsEmpty() && csStateName.Find(_T("CashManagement")) != -1)) //dp185016 tar 307726
            return DMAcceptor();        // cash amount
        else
        {
            trace(L6, _T("For CashRecycler, ignore data event for %d after stopping deposit"), nDMLastAcceptor );           
            return STATE_NULL;
        }

    }

        
      case DMCLASS_SCANNER:
          {
              //USSF Start
              //SMStateBase* pState = HandleScannerData(me);
              USSFDATA(_T("DMLASTDATA"), cDMLastData);
              USSFHOOK(_T("DMXRawScan"), nextState = HandleScannerData(me), return nextState);
              //USSF End
          }
          break;
      case DMCLASS_SCALE:
        lDMLastCode = * (int *)cDMLastData;
        trace(L6,_T(" Weight %d "), lDMLastCode);
        trace(L6,_T("-Parse"));
        return DMSetScaleWeight(fCheckHealth);
                
      case DMCLASS_MSR:
        {
        CString sTrackData;
        {
         // The overall purpose of this first section is to reformat the data from being
         // a long string with the track data separated by field separators into a string
         // without the field separators.  Track lengths are provided in an array.

         // The structure definition here is misleading.  The structure as used is the 
         // 3 track lengths followed by the data from the three tracks, regardless
         // of how long that is.  cData just gives a place to attach the data.
         typedef struct 
            {
            short nTrackLengths[ 3 ];
            TCHAR  cData[ 1 ];
            } DM2MO_DATA;

         CString dmData( cDMLastData );
         short nTrackLengthTotals;


         // Calculate the length of the new structure to allocate as the length of all tracks
         // plus the length of the three track lengths.  
         nDMLastLength = sizeof(DM2MO_DATA) + dmData.GetLength() * sizeof(_TCHAR) + sizeof(_TCHAR);
         DM2MO_DATA *value = (DM2MO_DATA*)new TCHAR[ nDMLastLength + 1 ];

         // Get track 1 length and put it into the final string
         value->nTrackLengths[ 0 ] = (short)dmData.Find( (_TCHAR)DM_FS );
         nTrackLengthTotals = value->nTrackLengths[ 0 ];
         sTrackData = dmData.Left( value->nTrackLengths[ 0 ] ); 
         
         // Reposition to the start of track 2 and get the track 2 data and add it in
         dmData = dmData.Mid( value->nTrackLengths[ 0 ]  + 1 );
         value->nTrackLengths[ 1 ] = (short)dmData.Find( (_TCHAR)DM_FS );
         nTrackLengthTotals += value->nTrackLengths[ 1 ];
         sTrackData += dmData.Left( value->nTrackLengths[ 1 ] ); 

         // Reposition.  What is left is the track 3 data
         // NOTE: Previous implementation did not pass track 3 data.  Always sent 0 length
         // instead
         dmData = dmData.Mid( value->nTrackLengths[ 1 ]  + 1 );
         sTrackData += dmData;
         value->nTrackLengths[ 2 ] = (short)dmData.GetLength();
         nTrackLengthTotals += value->nTrackLengths[ 2 ];

         // Copy the compressed string to our structure
         _tcscpy( value->cData, sTrackData );
/*

         TCHAR *token;
         // Calculate the length of the new structure to allocate as the length of all tracks
         // plus the length of the three track lengths.  
         nDMLastLength = sizeof(DM2MO_DATA) + _tcsclen(cDMLastData) + 1;
         DM2MO_DATA *value = (DM2MO_DATA*)new TCHAR[ nDMLastLength + 1 ];

         TCHAR *dmData = new TCHAR[ nDMLastLength + 1 ];
         TCHAR *sTrackData = new TCHAR[ nDMLastLength + 1 ];

         _tcscpy(dmData, cDMLastData);

         token = _tcstok( dmData, (LPCTSTR)DM_FS );

         int nIndx = 0;
         
         while(token != NULL)
         {
             nIndx++;
             value->nTrackLengths[nIndx - 1] = _tcsclen(token);
             if(1 == nIndx)
                 _tcscpy(sTrackData, token);
             else
                 _tcscat(sTrackData, token);
             token = _tcstok( NULL, (LPCTSTR)DM_FS);
         }
         // Copy the compressed string to our structure
         _tcscpy( value->cData, sTrackData );
*/
         // Delete the original string passed in and pass our structure value along instead
         delete [] cDMLastData;
         cDMLastData = (LPTSTR)value;

         // release the current memory and allocate new
         // memory for the msr track data
         delete [] cDMLastTrack; //SSCOP-4165
         nDMLastTrackLen = nDMLastLength;
         cDMLastTrack = new _TCHAR [nDMLastTrackLen+1];
         memcpy(cDMLastTrack,cDMLastData,nDMLastTrackLen * sizeof(_TCHAR));
         //      szCreditCardOwnerName = CustomerName(cDMLastTrack);
         csCardAccountNum = GetAccountNum(cDMLastTrack) ;
         cDMLastTrack[nDMLastTrackLen] = NULL;   // make it a NULL terminated
         // erase any exisitng pin data
         csDMEncryptedPin = EMPTY_STRING;
         nPINTries = 0;
         waitingForCardRemoval = true; //TAR260141
         // The 5945 can return no data and no error on a card misread
         if (0 == nTrackLengthTotals)
         {
            // card data is non existent
            // handle it is a card-misread
            if (co.fTenderIsMotorizedMSR)
            {
                CREATE_AND_DISPENSE(CardMisRead)(MSG_MISREAD_MCRW, LTT_CARDMISREAD);
            }
            else
            {
                CREATE_AND_DISPENSE(CardMisRead)(MSG_MISREAD_SLIDE, LTT_CARDMISREAD);
            }
         }
        }
        // release the current memory and allocate new
        // memory for the msr track data
            /*
        if (cDMLastTrack)
          delete cDMLastTrack;
        nDMLastTrackLen = nDMLastLength;
        cDMLastTrack = new char[nDMLastTrackLen+1];
        memcpy(cDMLastTrack,cDMLastData,nDMLastTrackLen);
        csCardAccountNum = GetAccountNum(cDMLastTrack) ;
        cDMLastTrack[nDMLastTrackLen] = NULL;   // make it a NULL terminated
        // erase any exisitng pin data
        csDMEncryptedPin = EMPTY_STRING;
        nPINTries = 0;
        End Merge */
        trace(L6,_T("-Parse DMCLASS_MSR "));
        {
            // prenotify event to TB begin
            SMStateBase* pMSRState = PreNotifySCOTEvent2TB(me, sNotifyIn, sNotifyOut);
            if (!sNotifyOut.bAllowDefaultProcessing)
            {
                trace(L4, _T("PreNotifyDMEvent2TB() returns to a new state"));
                return pMSRState;
            }
            // prenotify event to TB end
        }
        // Loyalty card begin
        if ((co.fStateAllowLoyaltyCard) && (!fInATMTransaction))
        {
        // TAR 275219 start
            if (!bIsTransactionStarted && co.fStateForceTrxStartOnSwipe)
            {
                fLoyaltyCardAwaitVerification = true;  //   verify card after TB_READY return
            }
             else  //TAR 275219 end
             {
                TBLOYALTYCARDTYPE nLCProcessState = tb.VerifyLoyaltyCard(cDMLastTrack,nDMLastTrackLen);
                trace(L4,_T("tb.VerifyLoyaltyCard returns %d"),nLCProcessState);//TAR 440288

                switch (nLCProcessState)
                {
                    case LC_CARDDATA_MISSING:
                        {
                            // card data is not complete w.r.t TB
                            // handle it is a card-misread
                            if (co.fTenderIsMotorizedMSR)
                            {
                                CREATE_AND_DISPENSE(CardMisRead)(MSG_MISREAD_MCRW, LTT_CARDMISREAD);
                            }
                            else
                            {
                                CREATE_AND_DISPENSE(CardMisRead)(MSG_MISREAD_SLIDE, LTT_CARDMISREAD);
                            }
                        }
                    break;
                    case LC_INVALID:
                        {
                        // inserted  card is not a loyalty card
                        // let current state determine what to do
                        // +Fix TAR246479
                        // fInLoyaltyCardProcessing = true; //TAR235030 CHU 042103
                            trace(L0, _T("TB: NOT A LOYALTY CARD"));
                            fInLoyaltyCardProcessing =false;
                        // -Fix TAR246479
                        }
                    break;
                    case LC_VALID:
                        {
                            // TAR 307842
                            if(!fSAInTransaction) 
                                SASendSCOTInput( ITEMIZATION );
                            // end TAR 307842
                            fInLoyaltyCardProcessing = true;
                            RETURNSTATE(CardProcessing)
                         }
                    break;
                    case LC_INSERTED_BEFORE:
                        {
                            // TAR 307842
                            if(!fSAInTransaction) 
                                SASendSCOTInput( ITEMIZATION );
                            // end TAR 307842
                            CREATE_AND_DISPENSE(TakeLoyaltyCard)(LC_INSERTED_BEFORE);   
                        }
                    break;
                }

         }
            
         }
        // Loyalty card End

        // REQ 20060728-986 Start Special Transaction
        {
            bool bAllowDefaultProcessing = true;
            tb.PreprocessData(sTrackData, _T("MSR_DATA"), &bAllowDefaultProcessing);
            if (!bAllowDefaultProcessing)
            {
                // Enable MSR after start special transaction
                trace(L2, _T("Enable MSR after start special transaction."));
                waitingForCardRemoval = false;
                DMMSREject();
                trace(L4, _T("PreprocessData() processing returns STATE_NULL"));
                return STATE_NULL;
            }
        }
        
        if ((getStateName(this) != _T("SMAttract") && bIsTransactionStarted))
        {
            //If data matches personalization filter, do a loyalty lookup
            //SMAttract has specialized handling in place in DMCardReader
            LoyaltyCheck(_T("Swipe"), GetAccountDataFromMSRSwipe(cDMLastTrack));
        }

        //USSF Start
        USSFDATA(_T("DMLASTTRACK"), cDMLastTrack);
        USSFHOOK(_T("DMXCardRead"), nextState = DMCardReader(), return nextState;);
        //return DMCardReader();      // call the derived class for this state if one exists else call this base case
        //USSF End
        }
      case DMCLASS_MOTION_SENSOR:
        switch (lLastData)
        {
          case MOT_MOTION:

            if ( nDMLastDevId == 2 ) //Cash Trough sensor motion
            {
                trace(L6,_T("DM Cash Out Sensor detected motion"));
                trace(L6,_T("-Parse"));
                return OnMotionCashOutSensor();
            }
            else
            {
                trace(L6,_T("DM EAS MotionSensor detected motion"));
                trace(L6,_T("-Parse"));
            }
            return STATE_NULL;
            break;
          case MOT_NO_MOTION:
          if ( nDMLastDevId == 0 )
          {
            trace(L6,_T("DM EAS MotionSensor detected no motion"));
            trace(L6,_T("-Parse"));
            return DMdf_EASNoMotion();
          }
          else if ( nDMLastDevId == 1 ) //coupon sensor
          {
            trace(L6,_T("DM Coupon Sensor detected coupon slip"));
            trace(L6,_T("-Parse"));
            return OnCouponSensor();
          }
          else if ( nDMLastDevId == 2 ) //Cash Out
          {
            trace(L6,_T("DM Cash Out Sensor detected"));
            trace(L6,_T("-Parse"));
            return OnMotionCashOutSensor();
          }

          break;
        }
        trace(L6,_T("-Parse"));
        return STATE_NULL;

        case DMCLASS_TAKEAWAYBELT:
            switch (lLastData)
            {
            case tabStatusSendScaleStatus:
                trace(L6, _T("DM Takeaway Belt requests scale status"));
                trace(L6,_T("-Parse"));
                return DMTakeawaySendScaleStatus();
            default:
                trace(L6,_T("DM Takeaway Belt detected unknown status %04lX"), lLastData);
                break;
            }
            trace(L6, _T("-Parse"));
            return STATE_NULL;  // just stay in same state (ignore) for now

        /******cm150000: Added In Merged Code******/
        case DMCLASS_MICR:
            return DMMICR( 0 );
            break;
        /******cm150000: End Merged Code***********/

      case DMCLASS_ENCRYPTOR:
        if (nDMLastLength!=4)
        {
          trace(L6,_T("-Parse"));
          return STATE_NULL;
        }
        switch (lLastData)
        {
          case DM_ENCRYPT_SECURE_MODE:
            trace(L6,_T("DM Encryptor SecureMode"));
            trace(L6,_T("-Parse"));
            return DMEncryptorSecured();
          case DM_ENCRYPT_CANCELED:
            trace(L6,_T("DM Encryptor Cancelled"));
            trace(L6,_T("-Parse"));
            return DMEncryptorCanceled();
          case DM_ENCRYPT_COMPLETE:
            trace(L6,_T("DM Encryptor Secure Complete"));
            trace(L6,_T("-Parse"));
            return DMEncryptorCompleted();
        }
        trace(L6,_T("-Parse"));
        return STATE_NULL;

      }                             // end switch (nDMLastDev)
    }
    break;
  }                
    
/* SR672 - csWork will be the HTML file name
  switch(nDMLastDev)
  {
    case DMCLASS_RECEIPT:
        csWork = ps.GetPSText(TXT_RECEIPT_PRINTER) + ": " + csWork;
        break;
    case DMCLASS_JOURNAL:
        csWork = ps.GetPSText(TXT_JOURNAL_PRINTER) + ": " + csWork;
        break;
    default:
        break;
  }
  */
  

  if(fCaseHandled == false)
  {
    ScotError(INFO,SM_CAT_INTERNAL_ERROR, SCOTAPP_DM_CSWORK,_T("%d|%d|%d|%d"),
      nDMLastMsg,
      nDMLastDev,
      lDMLastCode,
      nDMLastLength);
  }
  else
  {
    ScotError(INFO,SM_CAT_INTERNAL_ERROR, SCOTAPP_DM_EVENT,_T("%s"), csWork);
  }

    trace(L6,_T("-Parse %s"), csWork);

    // +TAR 397457:  Ignore errors from devices that have not successfully
    //               loaded.  The health check will fail unless the 
    //               device status == DM_DA_LOADED.
    if(! dm.IsDeviceConfigured(nDMLastDev, nDMLastDevId))
    {
        trace(L6, _T("-DMParse:  Ignoring error from device class %d / id: %d since device is not loaded."), nDMLastDev, nDMLastDevId); 
        return STATE_NULL;
    }
    // -TAR 397457

    // 277025{
    if(nDMLastDev == m_nDMSavedDev )   // 277025 Ignore this error since there is already an error reported with the same device.
    {
        trace(L6,_T("-DMParse this error is ignored since there is already an error reported with the same device."));
        return STATE_NULL;
    }
    m_nDMSavedDev =  nDMLastDev;
    //}277025


   SMStateBase* sExit =
    createLevel1Message(csWork,           // char *message
                        PS_NOTEXT,        // no canned message
                        PS_NOTEXT,        // no prompt
                        0,                // no value
                        nDMLastDev,       // device reporting the error or -1
                        fCheckHealth,    // T/F need to issue CheckHealth after operator clears problem
                        true);  //display HTML //SR672


    //Notify RAP
    trace(L7, _T("+ra.OnRAPDataNeeded, ErrorMessage=%s, DeviceError=%d"), csWork, nDMLastDev);
    // write the error description on the receipt
    CString csSecInfo;
    //SR672
    if(!csWork.IsEmpty() &&!sDeviceErrorTitle.IsEmpty()) //Tar 438366
    {
        ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + ps.ConvertToEscapeSequence(sDeviceErrorTitle, _T("\\n"), _T("\n")) ); // TAR 288199
    }
    
    // save the previous view
    if(m_PrevView == DATANEEDED_NOVIEW)
    {
        m_PrevView = m_View; // save the previous view
    }

    // send a DataNeeded Approval
    cDataEntry.FromConfigData(co.csDataEntryINIFile, "DeviceFailure");
    m_View = DATANEEDED_DEVICEERROR;

    if(!csWork.IsEmpty() && !sDeviceErrorTitle.IsEmpty() ) //Tar 438366
    {
        csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),sDeviceErrorTitle,ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), "", "", "", "");
    }

    //429111+
    CString csTempStateName = this->GetRuntimeClass()->m_lpszClassName;
    if ((csTempStateName.Find(_T("Attract")) != -1 ) || !isAnySoldVoidedItems() )
    {
        rp.TransactionStart(rp.m_csBusinessDate);
    }
    //429111-

    ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
    trace(L7, _T("-ra.OnRAPDataNeeded"));

  //If in HostOffline state, delay the display of system message
  //and display the message when exit this state
  if (!fDoLater && bIsFromHostOffline)
  {
    fDoLater = true;
  }
    
  if (fDoLater)                                       // delay presention of this message until in Attract state
  {
    if (!szMsg3Name)                              // need to save the sysmsg3 state name for later comparison
    {
      szMsg3Name = new char[strlen(sExit->GetRuntimeClass()->m_lpszClassName)+1];
      strcpy(szMsg3Name,sExit->GetRuntimeClass()->m_lpszClassName);
    }

    // trace this push event LPM072901 TAR176732
    CString csNextStateName = sExit->GetRuntimeClass()->m_lpszClassName;
    trace(L6,_T("Pushing state %s onto stack"),csNextStateName);
    sExit = PushStateFifoStack(sExit); // try to push if on stack for later
  }
   
  if (!fDoLater && ps.RemoteMode()) //HDMerge062802 starts here
  {
      // when we merge this fix to 105 or later build
      // Please remove 'SMState::' since it is a member of SMStateBase in build:105 or later
      ms_bKeepRemoteDevices = true;
      if (fStateAssist)
      {
        // allow state transition from Assistmode to system message
        //fStateAssist = false; //TAR408719
        trace(L6, _T("transitioning from Assistmode to SystemMessage"));
      }
  } //HDMerge062802 ends here

 
 // +TAR369813
 if(((me->dmInfo.Msg == DM_ERROR) || (me->dmInfo.Msg == DM_STATUS)) /*&& !fStateAssist*/&& 
        ps.RemoteMode() && ( sExit != STATE_NULL)) //TAR408719
    {
      if(!IsRemoteErrorAllowed(me->dmInfo.Class))
       {
                CString csState = sExit->GetRuntimeClass()->m_lpszClassName; 
                if( -1 != csState.Find(_T("SystemMessage")) )
                {
                    ms_bKeepRemoteDevices = false; //TAR389687
                    //fStateAssist = true; 
                    trace(L6,_T("Pushing state %s onto stack"),csState);
            sExit = PushStateFifoStack(sExit);
                    trace(L6, _T("Suppressing FastLane device system messages while in remote assist mode."));
                    return sExit; //sExit should be STATE_NULL after PushStateFifoStack
        }
            
        }
    }
  // -TAR369813


  return sExit;                                       // report error, now or later
}

bool SMStateBase::DMHookNotifyTenderOver(void)
{
    return true;
}

/**
 * Function:        DMCashRecyclerShouldReject
 * Purpose:         Return true if the recycler should reject the currency.  false otherwise.
 * Parameters:      csStateName:  Constant reference to CString that contains the 
 *                  name of the current state.
 * Returns:         true if the recycler should reject the currency.  false otherwise.
 * Calls:           dm.CashRecyclerGetEnabled(); isAnyApprovalRequired(); CString::Find();
 * Side Effects:
 * Notes:           Created to eliminate two ugly if statements in DMParse()
 */
bool SMStateBase::DMCashRecyclerShouldReject(const CString &csStateName)
{
    if(m_bCashRecyclerStopDeposit==true)  // Rejection is up to recycler if true.
    {                                     // FastLane will still process any new 
        return false;                     // currency events.
    }

    // From this point on is specific to cash recyclers only.
    if(dm.m_csCashRecyclerModel.IsEmpty())
    {
        return false;
    }

    if(dm.CashRecyclerGetEnabled() == FALSE &&
        (!isAnyApprovalRequired() ||                        // TAR 314457
         fOtherPayment ))                                   // TAR 315344
    {
        return true;
    }

    //+RFC 330995
    // TAR 318784 - Move this if block to before recycler model check.
    // Reject will not be just for Glory recycler.
    if(csStateName.Find(_T("MediaStatus")) != -1   ||
       csStateName.Find(_T("ConfirmVoid")) != -1   ||       // TAR 311332
       csStateName.Find(_T("VoidItem"))    != -1 )          // TAR 318047
    {
        return true;
    }
    //-RFC 330995

    return false;
}

/**
 * Function:        DMCashRecyclerIsTenderOver
 * Purpose:         Japan customers prefer to receive large denominations in change rather
 *                  than many of smaller denominations.  For example:  If the change due
 *                  to the customer is 4000 yen, the customer will insert an extra 1000 yen
 *                  bill in order to receive a single 5000-yen bill in change rather 
 *                  than 4 1000-yen bills.  This method determines when that condition
 *                  is met.
 *                  If true is returned, then deposits to the recycler will be disabled
 *                  and the total amount deposited will be retrieved from the recycler.
 * Parameters:      csStateName:  Constant reference to CString that contains the name of the 
 *                  current state.
 * Returns:         true if an amount > amount due has been inserted.  false otherwise.
 * Calls:           isAnyApprovalRequired();  CString::Find();
 * Side Effects:
 * Notes:           Created to eliminate two ugly if statements in DMParse()
 */
bool SMStateBase::DMCashRecyclerIsTenderOver(const CString &csStateName)
{
    // This method is only applicable to cash recyclers.
    if(dm.m_csCashRecyclerModel.IsEmpty())
    {
        return false;
    }

    if(nDMCashInserted >= lBalanceDue && 
        lBalanceDue && 
        !m_bCashRecyclerStopDeposit && 
        csStateName.Find(_T("CashManagement")) == -1 &&
        csStateName.Find(_T("InProgress")) == -1
      )             // TAR 314457, 319971
    {
        return true;
    }
    
    return false;
}

/**
 * Function:        DMUnlockNoteCover
 * Purpose:         Unlock SafePay note cover.  This can only be done through
 *                  software.
 * Parameters:      NONE
 * Returns:         true if successful.  false otherwise.
 * Calls:           
 * Side Effects:
 * Notes:           Wrapper for DMProcedures::UnlockNoteCover.      
 */
bool SMStateBase::DMUnlockNoteCover(void)
{
    long rc;

    if(dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_SAFEPAY) == -1 ) //SR672
    {
        return false;
    }

    rc = dm.UnlockNoteCover(DMCLASS_CASHRECYCLER, 0);   
    if(rc != OPOS_SUCCESS)
    {
        trace(L4, 
              _T("DMCashRecyclerUnlockNoteCover> ERROR: Unlatch note cover. rc: %d"),
              rc);
        return rc;
    }
    else
    {
        rp.ModeChange(rp.m_lEntryID++, MODE_NOTECOVER, _T("UNLOCKED"));
    }
    return(rc == OPOS_SUCCESS);
}

/////////////////////////////////////
CString SMStateBase::DMGetDevicesInError(void)
{
  return dm.GetDevicesInError();
}

/////////////////////////////////////
long SMStateBase::DMCheckHealth(long lDevClass) // call CheckHealth
{
  trace(L6,_T("+DMCheckHealth %d"),lDevClass);
  long rc = dm.CheckHealth(lDevClass); // returns zero on success

  if(lDevClass <= DMCLASS_PRINTER && rc == OPOS_SUCCESS && dm.IsDeviceConfigured(lDevClass))
  {
        CPrintStation* pPrinter = dm.GetPrintStation(lDevClass);

        if ( pPrinter != NULL && pPrinter->GetState() != OPOS_S_BUSY )
        {
            rc = pPrinter->RetryOutput();
            if ( rc == OPOS_SUCCESS)
            {
                mo.PostDM(DM_STATUS, lDevClass, 0, DM_PTR_REQUEST_COMPLETED, NULL);
            }
        }
    }

   //SR742+
   if ( dm.GetDegradedModeType() == DEGRADEDMODE_CARD_ONLY && IsCashDeviceError(lDevClass) )
   {
      ASSERT(co.IsDegradedModeOn());
      trace(L6, _T("SMStateDM::DMCheckHealth() - pretending cash devices are ok"));
      return OPOS_SUCCESS;
   }

   if ( dm.GetDegradedModeType() == DEGRADEDMODE_CASH_ONLY && IsCardDeviceError(lDevClass) )
   {
      ASSERT(co.IsDegradedModeOn());
      trace(L6, _T("SMStateDM::DMCheckHealth() - pretending card devices are ok"));
      return OPOS_SUCCESS;
   }
   //SR742-
  
  // tell RAP only if device actually cleared the error
  if (!rc) // rc is zero on success
  {
    // clear the Intervention related to this device error
    // Begin TAR 192405, 192461
    trace(L6, _T("ra.OnNoRAPDataNeeded()"));
    ra.OnNoRAPDataNeeded(); 
    // End TAR 192405, 192461
  }
  
  trace(L6,_T("-DMCheckHealth"));
  return rc;
}

/////////////////////////////////////
SMStateBase *SMStateBase::DMEncryptorPinIsTooShort(void) // The returned pin was to short.
{
  trace(L6,_T("+DMEncryptorPinIsTooShort"));

  gpSCOTEncryptor->CancelPINEntry();

  trace(L6,_T("-DMEncryptorPinIsTooShort"));
  return STATE_NULL;
}
#endif
/////////////////////////////
SMStateBase *SMStateBase::DMScale(void)
{
#ifndef _CPPUNIT
  trace(L6,_T("+Scale"));
    
  // if we must select tare, don't prime virtual scale,
  // we must wait for the ENTER WEIGHT prompt to test
  // if we need to select a tare, weights will be keyed in
  // if not tare, then we can prime the virtual scale
  // update the on-screen weight.
  // if the current state is frozen, i.e. assist mode
  // then prime the scale now
/*  
  if (lDMScaleWeight>0)                       // if good weight
  {
    TSSetScale(lDMScaleWeight);
  }
  else                                                        // error, unstable or zero
  {
    TSSetScale(0);                                // set weight to 0
  }
*/
    if (lDMScaleWeight==0)
        fDMScaleZeroed = true;                  // scale has been zeroed, used by S&B

    if(lDMScaleWeight > (nDMMaximumWeight + nDMMaximumWeightTol))
    {
        lDMScaleWeight = DM_ERROR_SCALE_OVERWEIGHT;
        trace(L6,_T("!Scale OverWeight! Weight: %d; Limit: %d"), lDMScaleWeight, 
                nDMMaximumWeight + nDMMaximumWeightTol);
    }
   
  if (lDMScaleWeight>=0)
  {
    ps.Weight(lDMScaleWeight);
  }
  else switch (lDMScaleWeight)
  {
  case DM_ERROR_GENERALFAILURE:       // -1
    ps.Weight(PS_SCALE_OVERWEIGHT); // --.--
    break;
  case DM_ERROR_SCALE_OVERWEIGHT:   // -2
    ps.Weight(PS_SCALE_OVERWEIGHT); // --.--
    break;
  case DM_ERROR_SCALE_UNDERZERO:    // -3
    ps.Weight(PS_SCALE_UNDERZERO);  // blank
    break;
  case DM_ERROR_SCALE_UNSTABLE:     // -4
    ps.Weight(PS_SCALE_UNSTABLE);   // blank
        lDMScaleWeight = 0;                         // not an error
        fDMScaleChanged = TRUE;
    break;
  case DM_ERROR_SCALE_WEIGHTUNCHANGED:// -5
    break;
  default:                          // other
    ps.Weight(PS_SCALE_OVERWEIGHT); // --.--
    break;
  }
    
  trace(L6,_T("-Scale: lDMScaleWeight = %d"), lDMScaleWeight);
#endif
  return STATE_NULL;
}
#ifndef _CPPUNIT
///////////////////////////////
SMStateBase *SMStateBase::DMScanner(void)
{
  //+TAR208818 use system msg 1 (instead of 4) to display do not scan message without 
  // requiring login.  Also added better tracing LPM070902
  trace(L6,_T("DMScanner System Message - %s"), ps.GetPSText(MSG_DONOTSCAN) );
  //+dp185016 tar 286970
  trace(L6,_T("bIgnoreScannedData : %d"), bIgnoreScannedData );
  if( bIgnoreScannedData )
  {
    return STATE_NULL;
  }
  bIgnoreScannedData = true;
  //-dp185016
  trace(L6,_T("DMScanner System Message - %s"), ps.GetPSText(MSG_DONOTSCAN) );
  CREATE_AND_DISPENSE(SystemMessage1)(  NULL,           // no char *message
                                        MSG_DONOTSCAN,  // canned message
                                        PS_NOTEXT,      // no prompt
                                        0,              // no prompt value
                                        -1,             // no device
                                        false);         // no DM check health needed

}


/////////////////////////////////////
// SR93.4
// DMScannerEncrypted
// 
// Purpose: Handles 2D barcode types.
//          Encrypted barcode types are listed in the registry as:
//
//          [HKEY_LOCAL_MACHINE\SOFTWARE\NCR\SCOT\CurrentVersion\SCOTAPP]
//          "EncryptedBarcodes"="201,202,203,204,205,206,207"
//
///////////////////////////////////////
// +SR93.4
SMStateBase *SMStateBase::DMScannerEncrypted(void)
{
    if (co.IsInAssistMenus() || fStateAssist)
    {
        SMSmAssistMenu::SetAssistMessage(LTT_INVALIDBARCODESCAN, SETITEMASIDECONTINUEWITHPURCHASES);
	    bEASReEnableScanner = true;
        RETURNSTATE(SmAssistMenu)
    } 
    else
    {
        nStateBeforeInvalidBarcode =  getAnchorState();
        RETURNSTATE(InvalidBarcode);
    }
}
// -SR93.4

////////////////////////////////
SMStateBase *SMStateBase::DMAcceptor(void)
{
  long rc;
  CString csStateName = EXTRACT_CLASSNAME(this);
  SMStateBase *retVal = STATE_NULL;

  trace(L6,_T("DMAcceptor"));
  
  // return the cash!

  // Tar 155302 SCOT don't need return inserted cash all the time even if the current screen doesn't allow cash insert. 
  // Sometime the cash is inserted in the previous screen but is processed in current screen. If BalanceDue is larger than 0, 
  // SCOT just keep the moeny instead of returning it.
  //+SSCOADK-836
  CString csCurrentStateName; 
  try
  {
	csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
  }
  catch(...)
  {
    trace(L1, _T("this->GetRuntimeClass() threw Exception!"));
  };

  int iErrorState = csCurrentStateName.Find(_T("SystemMessage"));
  if (lBalanceDue >0 || (iErrorState != -1))
  {
      // whenever the app goes back to finish state, it will eventually call TBEnterTender 
	  if (iErrorState != -1)
	     trace(L6, _T("SCOT is in SystemMessage. Returning STATE_NULL.")); // SSCOP-1692

	  nDMLastCoinEvent=0;
      return STATE_NULL;
  }    
  //-SSCOADK-836

  // TAR 335219 - SafePay and Glory cash recyclers.
  // If device is in deposit mode, then accept the currency.
  // This may occur if a device error occurs in cash management or if 
  // currency is inserted when logging in as head cashier 
  // (in SMSmCashierPassword).  
  if(! dm.m_csCashRecyclerModel.IsEmpty() && 
       dm.IsPauseCountsActive(DMCLASS_CASHRECYCLER) &&
      !rp.IsCashManagementInProgress())
  {
      trace(L6, _T("Accepted currency in state: %s"), (LPCTSTR)csStateName);
      return STATE_NULL;
  }
  
  // Tar 214729 RJF 101002  Need to pay attention to return code
  // Race condition, customer manages to get extra cash inserted before device disabled, 
  // but when we attempt to reject cash, we encounter an ScotError(probably out of change),
  // able to reproduce coin situation like at store, but couldn't reproduce bill equivalent;
  // as failsafe adding same code for bill
  if (!bIsCashAcceptorEnabled )             //rfc 330995
  {

      rc = DMCashAcceptorReject();  // Coin Acceptor Modifications
      if (rc < 0)           //  most likely out of change
        retVal = DMCashChangerError(nDMLastBillEvent);
	  //sscoadk-7308+
      else if ( rc == OPOS_E_ILLEGAL )
      {
        trace(L6, _T("Unable to dispense the inserted money at this moment, will dispense later "));
        nDMLastBillEvent = 0;
        return STATE_NULL; 
      }
      //sscoadk-7308-
      else if (nDMLastBillEvent > 0 && !rp.IsCashManagementInProgress())    //SSCOP-413
      {
        nDMCashInserted -= nDMLastBillEvent;
        
        //additional checking. We should not let this var go below zero, or else we had problems.
        if(nDMCashInserted < 0)     //SSCOP-413
            nDMCashInserted = 0;
            
        nDMLastBillEvent = 0;
      }
  }

  if (!bIsCoinAcceptorEnabled )            //rfc 330995
  {

      rc = DMCoinAcceptorReject();  // Coin Acceptor Modifications

      if (rc < 0)           // Tar 214729 RJF 101002 most likely out of change
      {
          // TAR 398436 release object if not used
          if (!((retVal >= STATE_NULL) && (retVal <= STATE_ERROR)))
          {
              delete retVal;
          }
          
          retVal = DMCashChangerError(nDMLastCoinEvent);
      }
      else if (nDMLastCoinEvent > 0 && !rp.IsCashManagementInProgress())    //SSCOP-413
      {
        nDMCashInserted -= nDMLastCoinEvent;
        
        //additional checking. We should not let this var go below zero, or else we had problems.
        if(nDMCashInserted < 0)     //SSCOP-413
            nDMCashInserted = 0;
            
        nDMLastCoinEvent = 0;
      }
  }

  trace(L6,_T("SMStateBase::DMAcceptor; can't accept coin/bill in state [%s]."), (LPCTSTR)csStateName);
  if (!Is5XHardware() && !Is6XHardware())  // Tar 403042
  {
       //+TAR 400007
      //+RFC 330995
      // TAR 398436 release object if not used
      if (!((retVal >= STATE_NULL) && (retVal <= STATE_ERROR)))
      {
          delete retVal;
      }
      
      //SR672
      bool fCheckHealth = false;
      CString sModelName=dm.GetDeviceModelName(DMCLASS_CASHCHANGER, 0);
      CString csWork = DMGetDeviceErrorHTML(DMCLASS_CASHCHANGER, sModelName, CString(_T("")), 1007, -99, 0, NULL, &fCheckHealth, 0, co.csPrimaryLanguageCode );
      
      //CString csWork = _T("DeviceErrorCashChanger.xml:DM_CASHCHANGER_STATUS_DISPENSE");
      CREATE_AND_DISPENSE(SystemMessage1)(csWork,   // no char *message 
                        PS_NOTEXT,  // canned message
                        PS_NOTEXT,  // no prompt
                        0,          // no prompt value
                        -1,         // no device
                        false, true);     // no DM check health needed 
      //-RFC 330995
      //-TAR 400007
  }
   
  return retVal;
  
}

//////////////////////////////////
SMStateBase *SMStateBase::DMCardReader(void)
{
  // should never get here since MSR is only enabled
  // if a card insert is allowed but just in case...
  trace(L6,_T("DMCardReader"));

  if ( !cDMLastTrack )  //TAR393636
  {
    DMMSREject();                     // eject DcL-
  }

  DMMSRDisable();                   // disable
  // NN 013103 Merging the fix to 3.1 branch
  // TAR 218568 NN 103102 Changing this from a SysMsg4 to SysMsg1 since this message is just
  // informational and do not require operator login 
/*  CREATE_AND_DISPENSE(SystemMessage1)(NULL,       // no char *message
                             co.fTenderIsMotorizedMSR ? MSG_DONOTINSERTCARD : MSG_DONOTSWIPECARD, // canned message //TAR#119738
                             PS_NOTEXT,  // no prompt
                             0,          // no prompt value
                             -1,         // no device
                             false);     // no DM check health needed
                             */
  
  CustomMessageObj.Reset();
  CustomMessageObj.csLeadThruText = ps.GetPSText(TXT_SYSTEMMESSAGE); //TAR313651
  CustomMessageObj.csScreenText = ps.GetPSText(co.fTenderIsMotorizedMSR ? MSG_DONOTINSERTCARD : MSG_DONOTSWIPECARD);
  RETURNSTATE(CustomMessage)
}


///////////////////////////////////
//SMStateBase *SMStateBase::DMDeviceNowOK(const long lDev)// this is overriden in the system message 3 state
//{
//  trace(L6,"DMDeviceNowOk %d",lDev);
  // here we want to examine the FIFO state stack to remove any pending states
  // that are errors on this device lDev.  Presumably the error has been
  // cleared and we need not present the error anymore.
  // if any SystemMessage3 states have been stacked, we will have
  // set the szMsg3Name above, we need this so we can skip non-sysmsg3 stacked states
//  if (szMsg3Name)                   // some sysmsg3 have been issued
//  {
//    for (int i=0;i<MAX_FIFO_STACK;i++) // scan the whole stack
//    {
      // set work pointer to state
//      SMStateBase *sWork = sStateFifo[i];
//          
//      if (sWork!=STATE_NULL)
//      {
        // if a stacked sysmsg3 state
//        if (strcmp(sWork->GetRuntimeClass()->m_lpszClassName,szMsg3Name)==0)
//        {
//          if (sWork->lDevice==lDev) // yes same device, we now remove this state
//          {
//            delete sWork;           // delete that state and dequeue the pointer
//            sStateFifo[i] = STATE_NULL;
//          }
//        }
//      }
//    }
//  }
//  return STATE_NULL;
//}

///////////////////////////////////
SMStateBase *SMStateBase::DMCardRemoved(void)
{
  trace(L6,_T("DMCardRemoved"));
  waitingForCardRemoval = false;
  return STATE_NULL;
}

///////////////////////////////////
SMStateBase *SMStateBase::DMCashRemoved(void)// this is overriden in the take change, take money states
{
  trace(L6,_T("DMCashRemoved"));
  DMSetCashTaken();
  return STATE_NULL;
}

/////////////////////////////
long SMStateBase::DMPrintVersion()
{
  long rc = 0;
//fiscal printer start
  if(co.fOperationsTBControlsPrinter)
    {
        trace(L6,_T("+DMPrintVersion"));
        trace(L6,_T("No Version printed because Fiscal printer configured"));
        trace(L6,_T("-DMPrintVersion"));
        return rc;
    }
//fiscal printer end
  trace(L6,_T("+DMPrintVersion"));
    
  // This application is able to extract information from its Version Information structure
  // which is embedded into the executable file.
  // The following methodology is what works and the following information is able to be extracted.
  // CompanyName FileDescription FileVersion InternalName LegalCopyright OriginalFilename ProductName ProductVersion
  //
  CString AppName = AfxGetAppName();
  AppName += _T(".exe");
  DWORD rzero;  // what in the world is this for ?
  DWORD VersionSize = GetFileVersionInfoSize( (LPTSTR)(LPCTSTR)AppName, &rzero );
  LPVOID lpData = new TCHAR[VersionSize]; // pointer to buffer to receive filenegativersion info.
  int rc2 = GetFileVersionInfo( (LPTSTR)(LPCTSTR)AppName, 0, VersionSize, lpData );
  rc2 = GetLastError();                       // handle errors if needed
/*  LPVOID lpBuffer;                                  // address of buffer for version pointer
  unsigned int dwBytes;                       // address of version-value length buffer
  rc2 = VerQueryValue(lpData,
        TEXT("\\StringFileInfo\\040904b0\\ProductVersion"),
        &lpBuffer,
        &dwBytes);
  rc2 = GetLastError();*/

  // Tar 163262 Change the logic that show the version of the software on the receipt.
  // Before we read the info from scotapp.rc file, now we read from two sub keys of the regedit.
  // They are Release Version and Patch Version.
  _TUCHAR  szReleaseVersion[_MAX_PATH + 1]={0}; 
  _TUCHAR  szPatchVersion[_MAX_PATH + 1]={0};
  unsigned long  uSize = _MAX_PATH;


  //Read the Release version key from registry
  LONG     lRC;
  HKEY     hKey;
  //DWORD dwDataSize;

  CString ReleaseSubKey(_T("Release Version"));
  CString PatchSubKey(_T("Patch Version"));

  CString csReleaseVersionInfo, csPatchVersionInfo;            
   
  lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP"), 
               0, KEY_QUERY_VALUE, &hKey);

  // ASSERT(lRC==ERROR_SUCCESS); 
  if (lRC==ERROR_SUCCESS)
  {
      uSize = _MAX_PATH;

      lRC = RegQueryValueEx(hKey, ReleaseSubKey, NULL, NULL, 
                   (LPBYTE) szReleaseVersion, &uSize);

      // ASSERT(lRC==ERROR_SUCCESS); 

      //read the Patch Version key from registry
      uSize = _MAX_PATH;
      lRC = RegQueryValueEx(hKey, PatchSubKey, NULL, NULL, 
                   (LPBYTE) szPatchVersion, &uSize);

      // ASSERT(lRC==ERROR_SUCCESS); 
   
      ::RegCloseKey(hKey);
  }

        
  TCHAR Tnumber[25];
  long TerminalNumber = TBGetTerminalNumber();  // Get the terminal number of this system.
  _ltot( TerminalNumber, Tnumber, 10 );

  CTime ctTime = CTime::GetCurrentTime();
  CString csWork;
  CString csTemp;

  csWork += _T("            NCR SelfServ Checkout\n");  
  csWork += _T("       SCOTApp System Initialization");
  csWork += _T("\n\n");
    
  csWork += _T("Current Date/Time: ");
  csWork += ctTime.Format( _T("%Y/%m/%d %H:%M:%S") );   // format for the time stamp string
  csWork += csTemp;
  csWork += _T("\n\n");
    
  csReleaseVersionInfo = szReleaseVersion;
  if (!csReleaseVersionInfo.IsEmpty())
  {
      csWork += _T("Release Version ");
      csWork += csReleaseVersionInfo;
      csWork += _T(" ");
  }
  csPatchVersionInfo = szPatchVersion;
  if (!csPatchVersionInfo.IsEmpty())
  {
    csWork += csPatchVersionInfo;
  }
#ifdef _DEBUG
  csWork += _T(" (DEBUG MODE)");
#endif
  csWork += _T("\n\n");
  csWork += ps.GetPSText(MSG_TERMINALNUMBER);
  csWork += _T(" "); // TAR 392601
  csWork += Tnumber;
  csWork += _T("\n\n");
    
  CFileFind ft;
  CTime tt;
  TCHAR szExeName[_MAX_PATH],*szN;
    
  // try to get the current EXE date, time and size
  if (SearchPath(NULL,AfxGetApp()->m_pszExeName,_T(".EXE"),_MAX_PATH,szExeName,&szN)>0)
  {
    if (ft.FindFile(szExeName)!=0)  // EXE found
    {
      ft.FindNextFile();            // get date of 'to' file
      ft.GetLastWriteTime(tt);
      ULONG sz = (ULONG)ft.GetLength();
      int yr = tt.GetYear();
      int mo = tt.GetMonth();
      int dy = tt.GetDay();
      int hr = tt.GetHour();
      int mn = tt.GetMinute();
      int se = tt.GetSecond();
      csWork += _T(" Current Program File:\n  ");
      csWork += szExeName;
      csWork += _T("\n");
      csTemp.Format(_T(" File Size:   %10d"),sz);
      csWork += csTemp;
      csWork += _T("\n");
      csTemp.Format(_T(" File Date:   %d/%02d/%02d"),yr,mo,dy);
      csWork += csTemp;
      csWork += _T("\n");
      csTemp.Format(_T(" File Time:   %2d:%02d:%02d"),hr,mn,se);
      csWork += csTemp;
    }
  }
    csWork += _T("\n\n");

  csTemp = TBName();                    // get TS compnent(s) name(s)
  csWork += csTemp;
  csWork += _T("\n");

  ScotError(INFO,SM_CAT_INTERNAL_ERROR, SCOTAPP_DM_EVENT,csWork);                 // log to error log as INFO
    
  csWork = _T("--------------------.-------------------\n\n") +
        csWork +
        _T("\n\n--------------------'-------------------\n");

    fDMReceiptError = false;
#ifdef _UNICODE   //SSCOP-137 (migrate fix of TAR459459)
  CString csData=csWork;
  csWork=ConvertPrinterString(csData);
#endif
  dm.ReceiptLine(csWork);
  dm.ReceiptCut();

  fDMJournalError=false;   //Journal Printer
  dm.JournalLine(csWork);
    
    
  delete [] lpData;
    
  trace(L6,_T("-DMPrintVersion %d"),rc);
  return rc;
}

//+dp185016 support glory device

CString SMStateBase::Justify( const CString csData )
{
  trace(L6,_T("+Justify %s"),csData);
  CString csRet = _T("");

  if( !csData.IsEmpty() ) //SR834 - no need to justify if data is empty
  {
      int nLeadingSpaces = ( CHARACTERLIMIT - csData.GetLength() ) / 2;

      for( int i = 0; i < nLeadingSpaces; i++ )
      {
        csRet += _T(" ");
      }

      csRet += csData;
    //  csRet += _T("\n");

      trace(L6,_T("-Justify %s"),csRet);
  }

  return csRet;

}

// format of parameters e.g. :
// iChangeValue : 10
// m_bBill : 0, coins
// then change in value is $.10
CString SMStateBase::ConvertValueToString( const int nDenom, bool bTrimDecimal )
{
  trace(L6,_T("+ConvertValueToString : %d"), nDenom);

  // let's convert the denomination to string, plus add the currency sign
  CString csDenom = _T("%d");
  int iChange = abs( nDenom );

  //tar 299311
    CString csYEN(_T(""));

  //dp185016 tar 307463 - modify japanese formatting
  //if( co.csLocaleCurrencySymbol.Find( csYEN ) != -1 )
  //{ // just don't use the psprocedures fmtdollar yen formatting here. e.g. 1 yen -> .01 yen
  //  csDenom.Format( (CString) csDenom, (int)iChange );
  //  if( csDenom.GetLength() == 1 && iChange != 0 )
  //  {
  //   csDenom.Insert( 0, '0' );
  //  }
  //
  //  if( iChange != 0 )
  //  {
  //    csDenom.Insert( csDenom.GetLength() - 2, '.' ); // insert a period
  //  }
  //  if( csDenom.Right( 3 ) == _T(".00") )
  //  {
  //    csDenom = csDenom.Left( csDenom.GetLength() - 3 );
  //  }

  //  csDenom = co.csLocaleCurrencySymbol + csDenom;
  //  if( nDenom < 0 )
  //    {csDenom = _T("-") + csDenom;}
  //}
  //else
  //{
    csDenom = ps.FmtDollar( nDenom, ps.SP_OTHER, ps.CASHMANAGEMENTFORMAT );
  //}
  //-tar 299311

  if( bTrimDecimal )
  {
    int nDecimal = csDenom.Find(_T("."));
    if( nDecimal != -1 )// && csDenom.Right( csDenom.GetLength() - nDecimal ) == _T(".00") )
    {
      csDenom = csDenom.Left( nDecimal );
    }
  
  }

  trace(L6,_T("-ConvertValueToString : %s"), csDenom);
  return csDenom;
}



CString SMStateBase::PositionData( CString csFirst, CString csMiddle, CString csLast )
{
  CString csRet = csFirst;

  int nPosition =  ( CHARACTERLIMIT - csMiddle.GetLength() ) / 2;

  nPosition = 23; 

  if( co.csDataMessageFile.Find( _T("0411") ) != -1 || co.csDataMessageFile.Find( _T("0412") ) != -1
      || co.csDataMessageFile.Find( _T("0804") ) != -1 || co.csPrimaryLanguageCode.CompareNoCase(_T("0C04")) == 0)
  {
    if( csFirst == ps.GetPSText( MSG_TOTAL ) )
      nPosition -= 2;
    else if( csFirst == ps.GetPSText( MSG_TOTAL_VALUE ) )
      nPosition -= 1;
    else if( csFirst == ps.GetPSText( MSG_LOCKBOX ) )
      nPosition -= 1;
  }
//  if( csFirst == ps.GetPSText( MSG_LOCKBOX ))
//    nPosition = 23;
//    nPosition = 17;

  trace(L6,_T("Denomination : %s, Position of changeInCount : %d "), csFirst, nPosition);

  int i;

  //tar 298899
//  for( i = csFirst.GetLength(); i < nPosition; i++ )
  for( i = csFirst.GetLength(); i < nPosition - csMiddle.GetLength(); i++ )
  {
    csRet += _T(" ");
  }

  csRet += csMiddle;
  nPosition = CHARACTERLIMIT - csLast.GetLength();
//  nPosition =  31;

  trace(L6,_T("csFirst Left : %s"), csFirst.Left( (ps.GetPSText( MSG_LOCKBOX )).GetLength() ));
//  if( csFirst.Left( (ps.GetPSText( MSG_LOCKBOX )).GetLength() ) == ps.GetPSText( MSG_LOCKBOX ))
//    nPosition = 25;
  if( co.csDataMessageFile.Find( _T("0411") ) != -1 || co.csDataMessageFile.Find( _T("0412") ) != -1
      || co.csDataMessageFile.Find( _T("0804") ) != -1 || co.csPrimaryLanguageCode.CompareNoCase(_T("0C04")) == 0)
  {
    if( csFirst == ps.GetPSText( MSG_TOTAL ) )
      nPosition -= 2;
    else if( csFirst == ps.GetPSText( MSG_TOTAL_VALUE ) )
      nPosition -= 1;
    else if( csFirst == ps.GetPSText( MSG_LOCKBOX ) )
      nPosition -= 1;
  }

  trace(L6,_T("Position of changeInValue : %d "), nPosition);

  trace(L6,_T("ChangeInValue : %s "), csLast);

  for( i = csRet.GetLength(); i < nPosition; i++ )
  {
    csRet += _T(" ");
  }

  csRet += csLast;

  trace(L6,_T("-PositionData : %s "), csRet );

  return csRet;
}

long SMStateBase::DMPrintCashManagementReceipt( 
    CArray<CString,CString>& m_caCashAcceptorDenomList, 
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
  trace(L6,_T("+DMPrintCashManagementReceipt"));
  long rc = 0;
    
  CString csHeader = LINEFORMAT;
  //+SR834
  csHeader +=        _T("%s");   // Store name
  csHeader +=        _T("%s");   // Store number
  //-SR834
  csHeader +=        _T("----------------------------------------\n" );
  csHeader +=        LINEFORMAT; // Cash Management Report
  csHeader +=        LINEFORMAT; // Date. this will be assigned with the value of csDayDate below.
  csHeader +=        LINEFORMAT; // terminal number
  csHeader +=        LINEFORMAT; // time

  long TerminalNumber = TBGetTerminalNumber();;
  CString csTerminalDataString = _T("%s %d");
  csTerminalDataString.Format((CString) csTerminalDataString, ps.GetPSText( MSG_TERMINALNUMBER ), TerminalNumber);

  //+ get the date
  COleDateTime dt = COleDateTime::GetCurrentTime();

    TCHAR sz[255];

    SYSTEMTIME st;

    st.wYear = (WORD)dt.GetYear();
    st.wMonth = (WORD)dt.GetMonth();
    st.wDayOfWeek = (WORD)dt.GetDayOfWeek();
    st.wDay = (WORD)dt.GetDay();
    st.wHour = dt.GetHour();
    st.wMinute = dt.GetMinute();
    st.wSecond = 0;
    st.wMilliseconds = 0;

    GetDateFormat(LOCALE_USER_DEFAULT, 
          // locale for which date is to be formatted
        DATE_LONGDATE, 
          // flags specifying function options
        &st,  // date to be formatted
        NULL, // date format string
        sz,   // buffer for storing formatted string
        254   // size of buffer
    );

    CString csDate = (CString) sz;
    //- get the date
  

    GetTimeFormat(LOCALE_USER_DEFAULT, //TAR 301310
          // locale for which date is to be formatted
        TIME_NOSECONDS, 
          // flags specifying function options
        &st,  // date to be formatted
        NULL, // use the locale setting to format TAR 301310
        sz,   // buffer for storing formatted string
        254   // size of buffer
    );

  CString csTime = (CString) sz; // get tiime

  //+SR834
  CString csStoreName = _T("");
  CString csStoreNumber = _T("");
  if(co.fLocaleShowStoreOnReport)
  {
      //get store name
      csStoreName = co.csLocaleStoreDescriptor;
      if (!csStoreName.IsEmpty())
          csStoreName += _T("\n");

      //get store number
      csStoreNumber = co.csLocaleStoreName;
      if (!csStoreNumber.IsEmpty())
          csStoreNumber.Format(_T("%s %s\n"), ps.GetPSText(MSG_STORE_NUMBER), csStoreNumber);
  }
  //-SR834

  trace(L6,_T("Store Name : %s, Store Number : %s, Terminal Number : %d, Date : %s, Time : %s"), 
        co.csLocaleStoreDescriptor, co.csLocaleStoreName, TerminalNumber, csDate, csTime);

  csHeader.Format( (CString)csHeader, 
    Justify( ps.GetPSText( MSG_FASTLANEREPORT ) ),
    Justify( csStoreName ), // SR834
    Justify( csStoreNumber ), // SR834
    Justify( ps.GetPSText( MSG_CASH_MANAGEMENTREPORT ) ),
    Justify( csDate ),
    Justify( csTerminalDataString ),
    Justify( csTime ) );  

  csHeader += _T("\n");


  CString csBillAcceptor, csBillDispenser, csCoinAcceptor, csCoinDispenser;

  //add "CashAcceptor=" for balance and pickup data
  csBalance += CASHACCEPTOR;
  csPickUp += CASHACCEPTOR;

  trace( L6, _T("Setting Bill Acceptor Data") );
  csBillAcceptor = ps.GetPSText( MSG_BILL_ACCEPTOR ) + _T("\n\n") + 
                      DMPrintCashmanagementReceiptByDevice(m_caCashAcceptorDenomList, 
                        m_caCashAcceptorCountList, m_caCashAcceptorChangeInCountList);

  if (csBillAcceptor.Find(_T("-")) == -1) //no pickup data
      csPickUp.Replace(CASHACCEPTOR, _T("")); //remove text "CashAcceptor="

  //add "CashDispenser=" for balance, loan and pickup data
  csBalance += CASHDISPENSER;
  csLoan += CASHDISPENSER;
  csPickUp += CASHDISPENSER;

  trace( L6, _T("Setting Bill dispenser Data") );
  csBillDispenser = ps.GetPSText( MSG_BILL_DISPENSER ) + _T("\n\n")  + 
                      DMPrintCashmanagementReceiptByDevice(m_caCashDispenserDenomList, 
                        m_caCashDispenserCountList, m_caCashDispenserChangeInCountList);

  if (csBillDispenser.Find(_T("+")) == -1) //no loan data
      csLoan.Replace(CASHDISPENSER, _T("")); //remove text "CashDispenser="
  if (csBillDispenser.Find(_T("-")) == -1) //no pickup data
      csPickUp.Replace(CASHDISPENSER, _T("")); //remove text "CashDispenser="

  //add "CoinAcceptor=" for balance and pickup data
  csBalance += COINACCEPTOR;
  csPickUp += COINACCEPTOR;

  trace( L6, _T("Setting Coin Acceptor Data") );
  csCoinAcceptor = ps.GetPSText( TXT_COIN_ACCEPTOR ) + _T("\n\n")  + 
                      DMPrintCashmanagementReceiptByDevice(m_caCoinAcceptorDenomList, 
                        m_caCoinAcceptorCountList, m_caCoinAcceptorChangeInCountList);

  if(csCoinAcceptor.Find(_T("-")) == -1) //no pickup data
      csPickUp.Replace(COINACCEPTOR, _T("")); //remove text "CoinAcceptor="

  //add "CoinDispenser=" for balance, loan and pickup data
  csBalance += COINDISPENSER;
  csLoan += COINDISPENSER;
  csPickUp += COINDISPENSER;

  trace( L6, _T("Setting Coin dispenser Data") );
  csCoinDispenser = ps.GetPSText( MSG_COIN_DISPENSER ) + _T("\n\n")  +   
                      DMPrintCashmanagementReceiptByDevice(m_caCoinDispenserDenomList, 
                        m_caCoinDispenserCountList, m_caCoinDispenserChangeInCountList);

  if(csCoinDispenser.Find(_T("+")) == -1) //no loan data
      csLoan.Replace(COINDISPENSER, _T("")); //remove text "CoinDispenser"
  if(csCoinDispenser.Find(_T("-")) == -1) //no pickup data
      csPickUp.Replace(COINDISPENSER, _T("")); //remove text "CoinDispenser"

  CString csAllData = _T("%s%s%s%s%s");

  trace(L6,_T("Adding all Receipt Data" ) );
  csAllData.Format((CString) csAllData, csHeader, csBillAcceptor, csBillDispenser,
    csCoinAcceptor, csCoinDispenser);
  
  if(co.fOperationsTBControlsPrinter)//TAR341375
  {
    return SendPrintDataToTB(&csAllData);
  }
  else
  {
//+SSCOP-137 (migrate fix of TAR459459)
    fDMReceiptError = false;
#ifdef _UNICODE
    CString csData=csAllData;
    csAllData=ConvertPrinterString(csData);
#endif
    dm.ReceiptLine(csAllData);
    dm.ReceiptCut();
  }
//-SSCOP-137 (migrate fix of TAR459459)
  fDMJournalError=false;   //Journal Printer
  dm.JournalLine(csAllData);

  trace(L6,_T("-DMPrintCashManagementReceipt %d"), rc);

  return rc;
}

CString SMStateBase::ConvertPrinterString(CString csData) const //SSCOP-137 (migrate fix of TAR459459)
{
    char* pBuf;
    int nLen;
    BOOL bDef;

#ifdef _UNICODE   //SSCOP-137 (migrate fix of TAR459459)

    unsigned int nCodePage = _ttoi(m_csPrinterCharSet);
    trace(L6, _T("Default Character Set is %s"), m_csPrinterCharSet);

    if ( nCodePage != 0 )
    {
        trace( L6, _T("Attempting conversion to code page %d"), nCodePage );
        nLen = WideCharToMultiByte( nCodePage, 0, csData, csData.GetLength(), NULL, 0,  NULL, &bDef );
        trace( L6, _T("Conversion needs %d bytes: %d"), nLen, ::GetLastError() );

        if ( nLen )
        {
            int nRealLen;
            pBuf = new char[nLen+1];
            memset((void*)pBuf, 0, nLen+1 );
            nRealLen = WideCharToMultiByte( nCodePage, 0, csData, csData.GetLength(), pBuf, nLen+1,NULL, &bDef );

            trace( L6, _T("Conversion returns %d bytes: %d"), nLen, ::GetLastError() );
            if ( nRealLen )
            {
                CString csANSI( pBuf, nRealLen );
                delete[] pBuf;
                return csANSI;
            }
            else
            {
                delete[] pBuf;
            }
        }
    }

#endif

    return csData;
}

CString SMStateBase::DMPrintCashmanagementReceiptByDevice( CArray<CString,CString>& m_caDenomList,
    CArray<int,int>& m_caCountList,
    CArray<int,int>& m_caChangeInCountList)
{
  trace(L6,_T("+DMPrintCashmanagementReceiptByDevice") );

  trace(L6,_T("DenomList size : %d. CountList Size : %d. Change In CountList Size : %d."),
    m_caDenomList.GetSize(), m_caCountList.GetSize(), m_caChangeInCountList.GetSize() );

  CString csAllData; 
  //+show header - bin    count   changeincount
  CString csHeader;

  if( co.fLocaleCashManagementDisplayCount )
  {
    csHeader.Format(ps.GetPSText(MSG_RECEIPTFORMAT_GENERICHEADERCOUNT), ps.GetPSText(MSG_BINS), 
      ps.GetPSText( MSG_COUNT ), 
      ps.GetPSText( MSG_CHANGE_IN_COUNT) );
  }
  else
  {
    csHeader.Format(ps.GetPSText(MSG_RECEIPTFORMAT_GENERICHEADERVALUE), ps.GetPSText(MSG_BINS), 
      ps.GetPSText( MSG_VALUE ), 
      ps.GetPSText( MSG_CHANGEINVALUE) );
  }
  //  csHeader = PositionData( ps.GetPSText(MSG_BINS), ps.GetPSText( MSG_COUNT ), ps.GetPSText( MSG_CHANGE_IN_COUNT ) );
  csHeader = ps.ConvertToEscapeSequence(csHeader, _T("\\n"), _T("\n"));
  csAllData += _T("\n");
  csAllData += csHeader;
  csAllData += _T("\n\n");
  //-show header - bin    count   changeincount

  CString csTotalCount, csTotalChangeInCount, csTotalValue, csTotalChangeInValue;
  int nTotalCount = 0, nTotalChangeInCount = 0, nTotalValue = 0, nTotalChangeInValue = 0;
  
  for( int i = 0; i < m_caDenomList.GetSize(); i++ )
  {
    CString csDenom;
    CString csCount;
    CString csChangeInCount;

    csBalance += m_caDenomList[i];//add denom on balance
    csBalance += _T(":");
    if( m_caChangeInCountList[i] > 0 )//has loan data
    {
        csLoan += m_caDenomList[i];//add denom on loan
        csLoan += _T(":");
    }
    if( m_caChangeInCountList[i] < 0 )//has pickup data
    {
        csPickUp += m_caDenomList[i];//add denom on pickup
        csPickUp += _T(":");
    }

    if( _ttol( m_caDenomList[i] ) == 0 && m_caDenomList[i] != _T("0") )
    {
      csDenom = m_caDenomList[i];
    }
    else
    {
      csDenom = ConvertValueToString(abs(_ttol(m_caDenomList[i])), false);
    }

    // +TAR 370144:  Move this block out of check for
    //               co.fLocaleCashManagementDisplayCount
      csCount.Format( _T("%d"), m_caCountList[i] );
      csChangeInCount.Format( _T("%d"), m_caChangeInCountList[i] );

      csBalance += csCount; //add count data on balance
      if (i < (m_caDenomList.GetSize() - 1))//do not add "," on the last item
    {
        csBalance += _T(",");
    }
     
      if( m_caChangeInCountList[i] > 0 )
        {
          csLoan += csChangeInCount;//add count data on loan

          //+append plus sign if positive
          csChangeInCount = _T("+") + csChangeInCount;
          //-append plus sign if positive

          if (i < (m_caDenomList.GetSize() - 1))//do not add "," on the last item 
            csLoan += _T(",");
        }

      if( m_caChangeInCountList[i] < 0 )
      {
          csPickUp += csChangeInCount;//add count data on pickup
          if (i < (m_caDenomList.GetSize() - 1))//do not add "," on the last item 
            csPickUp += _T(",");
      }
    // -TAR 370144
      
    if( co.fLocaleCashManagementDisplayCount )
    {
      trace(L6,_T("denomination %s, count %s, and ChangeInCount %s"), csDenom, csCount, csChangeInCount );    
      csAllData += PositionData( csDenom, csCount, csChangeInCount );
    }
    else
    {
      CString csValue, csChangeInValue;
      csValue.Format( _T("%s"), 
        ConvertValueToString( abs( _ttol(m_caDenomList[i]) ) * m_caCountList[i],false ) );
      csChangeInValue.Format( _T("%s"), 
        ConvertValueToString( abs( _ttol(m_caDenomList[i]) ) * m_caChangeInCountList[i],false ) );
      trace(L6,_T("denomination %s, value %s, and ChangeInValue %s"), csDenom, csValue, csChangeInValue );

      //+append plus sign if positive
      if( m_caChangeInCountList[i] > 0 )
        {csChangeInValue = _T("+") + csChangeInValue;}
      //-append plus sign if positive
      csAllData += PositionData( csDenom, csValue, csChangeInValue );
    
    }
      
    csAllData += _T("\n");

    nTotalCount += m_caCountList[i];
    nTotalChangeInCount += m_caChangeInCountList[i];
    nTotalValue += abs( _ttol(m_caDenomList[i]) ) * m_caCountList[i];
    nTotalChangeInValue += abs( _ttol(m_caDenomList[i]) ) * m_caChangeInCountList[i];
    trace(L6,_T("Running TotalCount %d, TotalChangeInCOunt %d, TotalValue %d, TotalChangeInValue %d"),
      nTotalCount, nTotalChangeInCount, nTotalValue, nTotalChangeInValue);
  }

  trace(L6,_T("REAL TotalCount %d, TotalChangeInCOunt %d, TotalValue %d, TotalChangeInValue %d"),
      nTotalCount, nTotalChangeInCount, nTotalValue, nTotalChangeInValue);
  
  csAllData += _T("\n");

  //+display total count and total change in count
  csTotalCount.Format(_T("%d"), nTotalCount);
  csTotalChangeInCount.Format(_T("%d"), nTotalChangeInCount);

  if( nTotalChangeInCount > 0 )
    {csTotalChangeInCount = _T("+") + csTotalChangeInCount;}
  csAllData += PositionData( ps.GetPSText( MSG_TOTAL ), csTotalCount, csTotalChangeInCount );
  trace(L6,_T("Total Count : %s, Total Change in Count : %s"), csTotalCount, csTotalChangeInCount );
  //-display total count and total change in count

  csAllData += _T("\n");

  //+display total value and total change in value
  csTotalValue = ConvertValueToString(nTotalValue, false);
  csTotalChangeInValue = ConvertValueToString(nTotalChangeInValue, false);
 
  if( nTotalChangeInValue > 0 )
    {csTotalChangeInValue = _T("+") + csTotalChangeInValue;}

  csAllData += PositionData( ps.GetPSText( MSG_TOTAL_VALUE ), csTotalValue, csTotalChangeInValue );
  trace(L6,_T("Total Value : %s, Total Change in value : %s"), csTotalValue, csTotalChangeInValue );
  //-display total value and total change in value

  csAllData += _T("\n\n\n");

  csBalance += _T(";");//add seperator
  csLoan.TrimRight(_T(","));//remove extra ","
  csPickUp.TrimRight(_T(","));
  csPickUp.Remove(_T('-'));
  if (csAllData.Find(_T("+") ) != -1)//has loan
    csLoan +=_T(";");//add seperator
  if (csAllData.Find(_T("-") ) != -1)//has pickup
    csPickUp += _T(";");//add seperator

  trace(L6, _T("csLoan = %s, csPickUp = %s, csBalance = %s"), csLoan, csPickUp, csBalance);

  trace(L6,_T("-DMPrintCashmanagementReceiptByDevice %s"), csAllData );
  return csAllData;
}


long SMStateBase::DMPrintCashManagementReceipt( CArray<int,int>& caValueList, CArray<int,int>& caChangeInCount )
{
  trace(L6,_T("+DMPrintCashManagementReceipt size of valuelist, and changeincount list : %d,   %d"), caValueList.GetSize(), caChangeInCount.GetSize());

  long rc = 0;
    
  CString csHeader = LINEFORMAT;
  //+SR834
  csHeader +=        _T("%");    // Store name
  csHeader +=        _T("%");    // Store number
  //-SR834
  csHeader +=        _T("----------------------------------------\n" );
  csHeader +=        LINEFORMAT; // Cash Management Report
  csHeader +=        LINEFORMAT; // Date. this will be assigned with the value of csDayDate below.
  csHeader +=        LINEFORMAT; // terminal number
  csHeader +=        LINEFORMAT; // time

  long TerminalNumber = TBGetTerminalNumber();;
  CString csTerminalDataString = _T("%s %d");
  csTerminalDataString.Format((CString) csTerminalDataString, ps.GetPSText( MSG_TERMINALNUMBER ), TerminalNumber);

  //+ get the date
  COleDateTime dt = COleDateTime::GetCurrentTime();

    TCHAR sz[255];

    SYSTEMTIME st;

    st.wYear = (WORD)dt.GetYear();
    st.wMonth = (WORD)dt.GetMonth();
    st.wDayOfWeek = (WORD)dt.GetDayOfWeek();
    st.wDay = (WORD)dt.GetDay();
    st.wHour = dt.GetHour();
    st.wMinute = dt.GetMinute();
    st.wSecond = 0;
    st.wMilliseconds = 0;

    GetDateFormat(LOCALE_USER_DEFAULT, 
          // locale for which date is to be formatted
        DATE_LONGDATE, 
          // flags specifying function options
        &st,  // date to be formatted
        NULL, // date format string
        sz,   // buffer for storing formatted string
        254   // size of buffer
    );

    CString csDate = (CString) sz;
    //- get the date
  

    GetTimeFormat(LOCALE_USER_DEFAULT, // TAR 301310
          // locale for which date is to be formatted
        TIME_NOSECONDS, 
          // flags specifying function options
        &st,  // date to be formatted
        NULL, // use the locale setting to format TAR 301310
        sz,   // buffer for storing formatted string
        254   // size of buffer
    );

  CString csTime = (CString) sz; // get tiime

  //+SR834
  CString csStoreName = _T("");
  CString csStoreNumber = _T("");
  if(co.fLocaleShowStoreOnReport)
  {
      //get store name
      csStoreName = co.csLocaleStoreDescriptor;
      if (!csStoreName.IsEmpty())
          csStoreName += _T("\n");

      //get store number
      csStoreNumber = co.csLocaleStoreName;
      if (!csStoreNumber.IsEmpty())
          csStoreNumber.Format(_T("%s %s\n"), ps.GetPSText(MSG_STORE_NUMBER), csStoreNumber);
  }
  //-SR834

  trace(L6,_T("Store Name : %s, Store Number : %s, Terminal Number : %d, Date : %s, Time : %s"), 
         co.csLocaleStoreDescriptor, co.csLocaleStoreName, TerminalNumber, csDate, csTime);

  csHeader.Format( (CString)csHeader, 
    Justify( ps.GetPSText( MSG_FASTLANEREPORT ) ),
    Justify( csStoreName ),   //SR834
    Justify( csStoreNumber ), //SR834
    Justify( ps.GetPSText( MSG_CASH_MANAGEMENTREPORT ) ),
    Justify( csDate ),
    Justify( csTerminalDataString ),
    Justify( csTime ) );  


  csHeader += _T("\n");
  // header for coin and bill bins
  CString csCoinsHeader = LINEFORMAT; // the BILLS word
  csCoinsHeader = _T("\n") + csCoinsHeader;
  
  csCoinsHeader += _T("\n%s\n"); // Bins, Change In COunt, and Change In Value header
  CString csTemp = ps.GetPSText( MSG_RECEIPTFORMAT_GLORYHEADER ); // to justify the bins, change in count, and change in value header
  csTemp = ps.ConvertToEscapeSequence(csTemp, _T("\\n"), _T("\n"));
  csTemp += _T("\n");
  csTemp.Format((CString) csTemp, ps.GetPSText(MSG_BINS), 
    ps.GetPSText( MSG_CHANGE_IN_COUNT), 
    ps.GetPSText( MSG_CHANGEINVALUE) );

  trace( L6, _T("Size of Bin, ChangeInCount, ChangeInValue is : %d"), csTemp.GetLength() );

  csCoinsHeader.Format( (CString)csCoinsHeader, ps.GetPSText(MSG_COINS), csTemp);

  CString csBillsHeader = LINEFORMAT;
  csBillsHeader += _T("\n%s\n"); // Bins, Change In COunt, and Change In Value header

  csBillsHeader.Format( (CString)csBillsHeader, ps.GetPSText(MSG_BILLS), csTemp);



  CString csCoinData, csBillData;
  bool bLoanBill = false;// a flag decide when to add "Cash=" text on loan data
  bool bPickupBill = false; //a flag decide when to add "Cash=" text on pickup data

  for( int i = 0; i < caChangeInCount.GetSize(); i++ )
  {

    trace(L6,_T("Processing ChangeInCount for location %i of ChangeInCount."), i );

    CString csDenomination, csChangeInValue, csChangeInCount= _T("%d");
    bool m_bBill = true;;

    if( i == caValueList.GetSize() )
    { // purge count
      trace(L6,_T("This is a lock box data") );
      m_bBill = true;
      csDenomination = ps.GetPSText(MSG_LOCKBOX);
      csChangeInValue = _T("--");
    }
    else
    {
      if( caValueList[i] < 0 )
        {m_bBill = false;}

      trace(L6,_T("IsBill : %d"), m_bBill );
    //format denomination;ConvertChangeInDenominationToString( int iChangeInValue, bool m_bBill )
      
      trace(L6,_T("Processing ChangeInValue for denomination %d, with ChangeInCount : %d"), caValueList[i], caChangeInCount[i] );
      
      csDenomination = ConvertValueToString( abs(caValueList[i]), false );
      int iChangeInValue = (m_bBill) ? caChangeInCount[i] * caValueList[i] : caChangeInCount[i] * caValueList[i]  * -1;
      csChangeInValue = ConvertValueToString( iChangeInValue, false );
      if( iChangeInValue > 0 )
      {
        csChangeInValue = _T("+") + csChangeInValue;
      }

      CString csDenom;
      csDenom.Format(_T("%d"), caValueList[i]);//collect denom data

      if (iChangeInValue > 0)//loan
      {
          if ((m_bBill) && (!bLoanBill))//start to have bill denom
          {
              csLoan.TrimRight(_T(','));//remove the last ","
              csLoan += _T(";Cash=");//add "Cash="
              bLoanBill = true;//only add once
          }
          csLoan += csDenom;
          csLoan += _T(":");
      }
      if (iChangeInValue < 0)//pickup
      {   
    
          if ((m_bBill) && (!bPickupBill))
          {
              csPickUp.TrimRight(_T(','));
              csPickUp += _T(";Cash=");
              bPickupBill = true;
          }
          csPickUp += csDenom;
          csPickUp += _T(":");
      }
    }
    
    //+format iChangeInCount
    csChangeInCount.Format( (CString) csChangeInCount, caChangeInCount[i] );
    

    if( caChangeInCount[i] > 0 )//loan
    {
        csLoan += csChangeInCount;//add count data to loan
        if (i < (caChangeInCount.GetSize() -1))//do not add "," on last item
            csLoan += _T(",");
        csChangeInCount = _T("+") + csChangeInCount;
        //-format iChangeInCount
    }

    if( caChangeInCount[i] < 0 )//pickup
    {
        csPickUp += csChangeInCount;//add count data to pickup
        if (i < (caChangeInCount.GetSize() -1))
            csPickUp += _T(",");
    }
    

    //add whether to bills or coin string
    CString *csCoinOrBillData = &csCoinData;
    if( m_bBill )
    {
      csCoinOrBillData = &csBillData;
      //put to csCoinsData list
    }

    trace(L6,_T("ChangeInValue : %s."), csChangeInValue );
    CString csStr = PositionData( csDenomination, csChangeInCount, csChangeInValue );
    *csCoinOrBillData += csStr;
    *csCoinOrBillData += _T("\n");

    trace(L6,_T("Added to %d(0 for coin) data, %s."), m_bBill, csStr );

  }
    csLoan.TrimLeft(_T(";"));//remove extra ";" on left
    csPickUp.TrimLeft(_T(";"));
    if (csLoan.Find(_T("-")) != -1)//has coin denom data
        csLoan = _T("Coin=") + csLoan;//add "Coin="
    if (csPickUp.Find(_T("-")) != -1)//has coin denom data
        csPickUp = _T("Coin=") + csPickUp;
    csLoan.Remove(_T('-'));//remove "-" on denom and count data
    csPickUp.Remove(_T('-'));

  CString csAllData = _T("%s%s%s%s%s");

  trace(L6,_T("Adding all Receipt Data" ) );
  csAllData.Format((CString) csAllData, csHeader, 
                   csBillsHeader, csBillData,
                   csCoinsHeader, csCoinData );
  
  if(co.fOperationsTBControlsPrinter)//TAR341375
  {
    return SendPrintDataToTB(&csAllData);
  }
  else
  {
    fDMReceiptError = false;
#ifdef _UNICODE   //SSCOP-137 (migrate fix of TAR459459)
    CString csData=csAllData;
    csAllData=ConvertPrinterString(csData);
#endif
    dm.ReceiptLine(csAllData);
    dm.ReceiptCut();
  }

  fDMJournalError=false;   //Journal Printer
  dm.JournalLine(csAllData);
    
  trace(L6,_T("-DMPrintVersion %d"),rc);
  return rc;
}//-dp185016

/////////////////////////////////////////
void SMStateBase::DMReceiptLine(const CString& csPrintLine)
{
    dm.ReceiptLine(csPrintLine);
}

/////////////////////////////////////////
void SMStateBase::DMReceiptBitMap(const CString csBmpFile)
{
    dm.ReceiptBitMap(csBmpFile);
}

/////////////////////////////////////////
void SMStateBase::DMReceiptCut(void)
{
    dm.ReceiptCut();
}

//////////////////////////////////////////////////////////////
void SMStateBase::DMReceiptReprint(void)
{
    fDMReceiptError = false;
    dm.ReceiptReprint();
}


//////////////////////////////////////////////////////////////
void SMStateBase::DMJournalReprint(void)
{
    fDMJournalError = false;
    dm.JournalReprint();
}

/////////////////////////////////////////
void SMStateBase::DMJournalLine(const CString& csPrintLine)
{
    fDMJournalError = false;
    dm.JournalLine(csPrintLine);
}

//Begin RFC 367092
long SMStateBase::DMPrintLine(int nDeviceClass,  const CString& csPrintLine)
{
    return dm.PrintLine(nDeviceClass, csPrintLine);
}
long SMStateBase::DMPrintBitmap(CString csBitmap, long lBmpAlign, int nDeviceClass)
{
    return dm.PrintBitmap(csBitmap, lBmpAlign, nDeviceClass);
}
long SMStateBase::DMPrintBarcode(CString csBarcode,long lBCType, long lBCHeight, long lBCAlign, bool fPrintNumbers, int nDeviceClass )
{
    return dm.PrintBarcode(csBarcode,lBCType, lBCHeight, lBCAlign, fPrintNumbers, nDeviceClass );
}

long SMStateBase::DMProcessPrintLine(int nDeviceClass, const CString& csPrintLine)
{
    return printerUtils.ProcessPrintLine(nDeviceClass, csPrintLine);
}
long SMStateBase::DMProcessPrintTokens(int nDeviceClass, CString& CurrentPrintLine)
{
    return printerUtils.ProcessPrintTokens(nDeviceClass, CurrentPrintLine);
}
//End RFC 367092

#endif // _CPPUNIT
/////////////////////////////////////////
long SMStateBase::DMSayAmount(const long lAmt)
{
  long rc = 0;
  trace(L6,_T("+DMSayAmount %d, g_bInRecallMode=<%d>"), lAmt, g_bInRecallMode);
  if (g_bInRecallMode)
      return rc;

  if (co.GetfStateSayPrices())
  {
      if (co.GetfStateAllowTBAudioControl())
      {
#ifndef _CPPUNIT
          if (TBIsAllowed(TB_FT_PRICE_AUDIO))
          {
              if ( ( co.fStateSayPricesDisableInAttendantMode )
                            && ( co.IsAttendantModeOn() ) )
              {
                  trace(L6, _T("Disabled SayPrice %d in AttendantMode"), lAmt );
              }
              else
              {
                  rc = dm.SayAmount(lAmt);
              }
          }
          else
          {
              trace(L6,_T("TB disallowed price audio"));
          }
#endif // _CPPUNIT
      }
      else if ( ( co.GetfStateSayPricesDisableInAttendantMode() )
                && ( co.IsAttendantModeOn() ) )
      {
          trace(L6, _T("Disabled SayPrice %d in AttendantMode"), lAmt );
      }
      else
      {
          rc = dm.SayAmount(lAmt);
      }
  }
    
  trace(L6, _T("-DMSayAmount %d"), rc);
  return rc;
}

///////////////////////////////////////

///////////////////////////////////////
long SMStateBase::DMSayPhrase(LPCTSTR szWave)
{
  long rc = 0;

  CString csW = szWave;
  trace(L6,_T("+DMSayPhrase <%s>"),szWave);
    
  if (co.GetfStateSayPhrases())
  {
      if (co.GetfStateAllowTBAudioControl())
      {
#ifndef _CPPUNIT
          if (TBIsAllowed(TB_FT_PHRASE_AUDIO))
          {
              rc = dm.SayPhrase(csW);
          }
          else
          {
              trace(L6,_T("TB disallowed phrase audio"));
          }
#endif
      }
      else if (co.GetfStateDisableAudioInAttendantMode())
      {
          trace(L6,_T("Audio disabled in attendant mode."));
      }
      else
      {
          rc = dm.SayPhrase(csW);
      }
  }
  trace(L6,_T("-DMSayPhrase %d"),rc);
  return rc;
}

/////////////////////////////////////////
long SMStateBase::DMSayPhrase(const int nMsg)
{
  long rc = 0;
  CString csW = ps.GetSoundName(nMsg);
  trace(L6,_T("+DMSayPhrase <%d>"),nMsg);
  trace(L6,_T("g_bInRecallMode <%d>, lNotForSaleItems<%d>, lUnknownItemsKeyed<%d>"),
      nMsg, lNotForSaleItems, lUnknownItemsKeyed);

  if ( g_bInRecallMode )
  {
      if ( nMsg == APPROVALNEEDED ||
          lNotForSaleItems || 
          lUnknownItemsKeyed)
          return rc;
  }
    if ( nMsg == SELECTLANGUAGE || nMsg == TOUCHSTART )
        return rc;

  if (co.GetfStateSayPhrases())
  {
      if (co.GetfStateAllowTBAudioControl())
      {
#ifndef _CPPUNIT
          if (TBIsAllowed(TB_FT_PHRASE_AUDIO))
          {
              rc = dm.SayPhrase(csW);
          }
          else
          {
              trace(L6,_T("TB disallowed phrase audio"));
          }
#endif
      }
      else if (co.GetfStateDisableAudioInAttendantMode())
      {
          trace(L6,_T("Audio disabled in attendant mode."));
      }
      else
      {
          rc = dm.SayPhrase(csW);
      }
  }
  trace(L6,_T("-DMSayPhrase %d"),rc);

  return rc;
}


long SMStateBase::DMSaySecurity(const long nMsg, LPCTSTR szWave, HANDLE hWaitEvent/*=0*/)
{
    long rc = 0;

    trace(L6,_T("+DMSaySecurity <%d> <%s> <hWaitEvent=0x%x>, g_bInRecallMode=<%d>"),
        nMsg, szWave, hWaitEvent,g_bInRecallMode);
    
    if (g_bInRecallMode)
        return rc;

    if (co.GetfStateAllowTBAudioControl())
    {
#ifndef _CPPUNIT
        if (TBIsAllowed(TB_FT_SECURITY_AUDIO))
        {
            rc = dm.SaySecurity(nMsg, szWave);
        }
        else
        {
            trace(L6,_T("TB disallowed security audio"));
        }
#endif // _CPPUNIT
    }
    else if (co.GetfStateDisableAudioInAttendantMode())
    {
        trace(L6,_T("Audio disabled in attendant mode."));
    }
    else
    {
        rc = dm.SaySecurity(nMsg, szWave, hWaitEvent);
    }

    trace(L6,_T("-DMSaySecurity %d"),rc);
    return rc;
}


//////////////////////////////////
long SMStateBase::DMScannerEnable(void)
{
  long rc = 0;
  trace(L6,_T("+DMScannerEnable"));
  m_bCouldBeScannedLoyaltyCard = false;
  rc = dm.ScannerEnable();
  trace(L6,_T("-DMScannerEnable %d"),rc);
  return rc;
}

///////////////////////////////////
long SMStateBase::DMScannerDisable(void)
{
  long rc = 0;
  trace(L6,_T("+DMScannerDisable"));
    
  rc = dm.ScannerDisable();
    
  trace(L6,_T("-DMScannerDisable %d"),rc);
  return rc;
}


//////////////////////////////////
void SMStateBase::DMTakeawayBeltNotify(long Event)
{
#ifndef _CPPUNIT
    trace(L6, _T("+DMTakeawayBeltNotify"));

    if (dm.fStateTakeawayBelt && gpSCOTTakeawayBelt) //tar 225991
    {
        trace(L6, _T("Takeaway Belt present: sending app event 0x%04lX"), Event);
        gpSCOTTakeawayBelt->OnAppEvent(Event);
    }
    else
    {
        trace(L7, _T("Takeaway Belt not present"));
    }

    trace(L6, _T("-DMTakeawayBeltNotify"));
#endif // _CPPUNIT
}
#ifndef _CPPUNIT

void SMStateBase::DMTakeawayBeltReset()
{
    trace(L6, _T("+DMTakeawayBeltReset"));

    if (dm.fStateTakeawayBelt && gpSCOTTakeawayBelt) //tar 225991
    {
        if (co.IsSecurityBypassOn())
        {
            trace(L6, _T("Ignoring reset while Security bypass is on"));
        }
        else
        {
            trace(L6, _T("Takeaway Belt present: resetting takeaway belt state"));
            gpSCOTTakeawayBelt->OnAppEvent(tabEventReset);
        }
    }
    else
    {
        trace(L7, _T("Takeaway Belt not present"));
    }

    trace(L6, _T("-DMTakeawayBeltReset"));
}
#endif //_CPPUNIT
///////////////////////////////////
long SMStateBase::DMCashAcceptorEnable(LPCTSTR szList)       // Coin Acceptor Modifications
{
  long rc = 0;
  // TAR 310659: For Glory cash recycler, if this flag is set then
  // do not reenable the recycler.
  
   if (!DMKeepCurrencyAcceptorsDisabled())
   {
       if(m_bCashRecyclerStopDeposit)
      {
         trace(L6, _T("DMCashAcceptorEnable:  Device not re-enabled after after stopping deposit."));
         return rc;
      }
      if(!bIsCashAcceptorEnabled) //SSCOADK-836
      {

        trace(L6,_T("+DMCashAcceptorEnable"));                        // Coin Acceptor Modifications
    
        if (co.fTenderCashAllowed && dm.IsCashAcceptorAvailable())
        {
           // DMAcceptorLight(DM_MISC_STATE_ON);now done state by state
           rc = dm.CashAcceptorEnable();
           if (rc>=0)
           {
               rc = dm.SetCashAcceptorCurrencyAcceptList(szList);

                // +SafePay cash recycler.
                // Handle error condition if BeginDeposit fails.
                if(rc == OPOS_E_EXTENDED && 
             dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_SAFEPAY) != -1 && //SR672
                   EXTRACT_CLASSNAME(this).Find(_T("CashManagement")) == -1)  // Don't show error if in cash mgmnt.
                {
                   trace(L4, _T("ERROR:  -DMCashAcceptorEnable FAILED.  Posting error for handling in DMParse."));
                   mo.PostDM(DM_ERROR, DMCLASS_CASHRECYCLER, 0, rc, NULL);
                }// -SafePay
                else
                    bIsCashAcceptorEnabled = true;         //RFC330995
           }
        }
      }
      else //SSCOADK-836
      {
          trace(L6,_T("Cash Acceptor is already enabled")); 
      }
   }
   trace(L6,_T("-DMCashAcceptorEnable %d"),rc);                  // Coin Acceptor Modifications
   return rc;
}


///////////////////////////////////
long SMStateBase::DMCashAcceptorReject(void)                // Coin Acceptor Modifications
{
  long rc = 0;
  trace(L6,_T("+DMCashAcceptorReject"));                        // Coin Acceptor Modifications
#ifndef _CPPUNIT    
  if (co.fTenderCashAllowed && dm.IsCashAcceptorAvailable())
  {
    if (nDMLastBillEvent)
    {
        
//SSCOP-4230      rc = dm.CashAcceptorEnable();
//SSCOP-4230      if (rc>=0)
//SSCOP-4230      {
        rc = dm.CashAcceptorReject(nDMLastBillEvent);

        // TAR 315616
        if(rc != OPOS_SUCCESS && dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_GLORY) != -1) //SR672
        {
            m_nCashRecyclerFailedRejectAmt += nDMLastBillEvent;

            trace(L6, _T("Failed to reject amount %d; Total failed amt: %d"), 
                nDMLastBillEvent, m_nCashRecyclerFailedRejectAmt);
        }
        // TAR 315616

        if (rc>=0)
        {
          DMReportDispensedCashList(); // TAR 373026
//SSCOP-4230  dm.CashAcceptorDisable();
        }
//SSCOP-4230      }
    }
  }
#endif _CPPUNIT
  trace(L6,_T("-DMCashAcceptorReject %d"),rc);                  // Coin Acceptor Modifications
  return rc;
}

///////////////////////////////////
long SMStateBase::DMCashAcceptorAccept(void)            // Coin Acceptor Modifications
{
  long rc = 0;
  trace(L6,_T("+DMCashAcceptorAccept"));                    // Coin Acceptor Modifications
#ifndef _CPPUNIT 
  if (co.fTenderCashAllowed && dm.IsCashAcceptorAvailable())
  {
    if (nDMLastAcceptor)
    {
        nDMLastAcceptor = 0;
//SSCOP-4230      rc = dm.CashAcceptorEnable();
//SSCOP-4230      if (rc>=0)
//SSCOP-4230      {
        rc = dm.CashAcceptorAccept();
//SSCOP-4230        if (rc>=0)
//SSCOP-4230        {
//SSCOP-4230          rc = dm.CashAcceptorDisable();
//SSCOP-4230        }
//SSCOP-4230      }
    }
  }
#endif _CPPUNIT
  trace(L6,_T("-DMCashAcceptorAccept %d"),rc);              // Coin Acceptor Modifications
  return rc;
}

////////////////////////////////////////////
long SMStateBase::DMAcceptorLight(const int lightState)
{
  long rc = 0;
  
#ifndef _CPPUNIT
  trace(L6,_T("+DMAcceptorLight %d"),lightState);

  CString sModelName = dm.GetDeviceModelName(DMCLASS_CASHACCEPTOR, 0);
  CString sMdoelNameRecycler = dm.GetDeviceModelName(DMCLASS_CASHRECYCLER, 0);

  if (sModelName.Find(_T("CashFlow")) !=-1 || sModelName.Find(_T("BNR")) !=-1 || sMdoelNameRecycler.Find(_T("Glory")) !=-1 )
      trace(L6,_T("No need to set note acceptor light for cashflow,BNR and Glory models"));
  else
  {
    
      if (co.fTenderCashAllowed && dm.IsCashAcceptorAvailable())
      {
        rc = dm.DepositoryLight(lightState);
      }
  }
#endif //_CPPUNIT
        
    
  trace(L6,_T("-DMAcceptorLight %d"),rc);
  
  return rc;
}

////////////////////////////////////////////
long SMStateBase::DMCoinAcceptorLight(const int lightState)
{
  long rc = 0;

#ifndef _CPPUNIT
  trace(L6,_T("+DMCoinAcceptorLight %d"),lightState);
  if (!Is5XHardware())
  {    
      if (co.fTenderCashAllowed && dm.IsCoinAcceptorAvailable())
      {
        rc = dm.CoinAcceptorLight(lightState);             // Tar 139796 Coin Acceptor Light
      }
  }
  else
  {
      int state = lightState;
      // With the BCR, the LEDs are connected to the device so it can control the lights
	  // We just need to handle the LEDs for the Glory device on 5X
      if(dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_GLORY) != -1) // +SSCOADK-4200 
      {
	     // if state is not "off" then it should "on". The LED only has 2 states 
	     if (state != DM_MISC_STATE_OFF)
		 {
            state = DM_MISC_STATE_ON;
		 }	
         rc = dm.MiscSetIndicator(DM_MISC_I_MIS_LED_2, state); 
         trace(L6,_T("For Glory device, set coin acceptor's LED, state = %d."), state);
      }                                                        // -SSCOADK-4200   
  }  
  trace(L6,_T("-DMCoinAcceptorLight %d"),rc);
  #endif //_CPPUNIT
  return rc;
}

#ifndef _CPPUNIT
////////////////////////////////////////////
long SMStateBase::DMCoinDispenserLight(const int lightState)  // +SSCOP-5501 
{
  long rc = 0;
  trace(L6,_T("+DMCoinDispenserLight %d"), lightState);
  if (Is5XHardware() && (dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_GLORY) != -1))   
  {
     trace(L6,_T("Change Coin dispenser LED light for the Glory's."));
     rc = dm.MiscSetIndicator(DM_MISC_I_MIS_LED_3, lightState); 
  }                                                         
  trace(L6,_T("-DMCoinDispenserLight: rc=%d"),rc);
  return rc;                                                 // -SSCOP-5501      
}
  #endif //_CPPUNIT
////////////////////////////////////
long SMStateBase::DMCashAcceptorDisable(void)           // Coin Acceptor Modifications
{
  long rc = 0;
  trace(L6,_T("+DMCashAcceptorDisable"));                   // Coin Acceptor Modifications
    
  if (co.fTenderCashAllowed && dm.IsCashAcceptorAvailable())
  {
    rc = DMAcceptorLight(DM_MISC_STATE_OFF);
    if (rc>=0)
    {
      bIsCashAcceptorEnabled = false;       //RFC330995
      rc = dm.CashAcceptorDisable();
    }
  }
    
  trace(L6,_T("-DMCashAcceptorDisable %d"),rc);             // Coin Acceptor Modifications
  return rc;
}

// Begin Coin Acceptor Modifications
///////////////////////////////////
long SMStateBase::DMCoinAcceptorEnable(void)
{
   long rc = 0;
   // TAR 310659: For Glory cash recycler, if this flag is set then 
   // do not reenable the recycler.
   //SR742 - if were in card only mode, we disable cash devices. Don't need to enable it.
   if (!DMKeepCurrencyAcceptorsDisabled())
   {
      if(m_bCashRecyclerStopDeposit)
      {
         trace(L6, _T("DMCoinAcceptorEnable:  Device not re-enabled after after stopping deposit."));
         return rc;
      }
      trace(L6,_T("+DMCoinAcceptorEnable"));
    
      if (co.fTenderCashAllowed && dm.IsCoinAcceptorAvailable())
      {
         rc = dm.CoinAcceptorEnable();
         bIsCoinAcceptorEnabled = true;     //RFC330995
         if (rc>=0)
         {
            rc = dm.SetCoinAcceptorEnableCoins(true);
         }
         // End Coin Acceptor Modifications
      }
   }

   trace(L6,_T("-DMCoinAcceptorEnable %d"),rc);
   return rc;
}
#ifndef _CPPUNIT

///////////////////////////////////
long SMStateBase::DMCoinAcceptorReject(void)
{
  long rc = 0;
  trace(L6,_T("+DMCoinAcceptorReject"));
    
  if (co.fTenderCashAllowed && dm.IsCoinAcceptorAvailable())
  {
    if (nDMLastCoinEvent)
    {
        rc = dm.CoinAcceptorReject(nDMLastCoinEvent);

        // TAR 315616
        if(rc != OPOS_SUCCESS && dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_GLORY) != -1) //SR672
        {
            m_nCashRecyclerFailedRejectAmt += nDMLastCoinEvent;

            trace(L6, _T("Failed to reject amount %d; Total failed amt: %d"), 
                nDMLastCoinEvent, m_nCashRecyclerFailedRejectAmt);
        }
        // TAR 315616
        rc=DMReportDispensedCashList(); 
    }
  }
    
  trace(L6,_T("-DMCoinAcceptorReject %d"),rc);
  return rc;
}

///////////////////////////////////
long SMStateBase::DMCoinAcceptorAccept(void)
{
  long rc = 0;
  trace(L6,_T("+DMCoinAcceptorAccept"));
    
  if (co.fTenderCashAllowed && dm.IsCoinAcceptorAvailable())
  {
    if (nDMLastAcceptor)
    {
      nDMLastAcceptor = 0;
//tar256688      rc = dm.CoinAcceptorEnable();
//tar256688      if (rc>=0)
//tar256688      {
        rc = dm.CoinAcceptorAccept();
//tar256688        if (rc>=0)
//tar256688        {
//tar256688          rc = dm.CoinAcceptorDisable();
//tar256688        }
//tar256688      }
    }
  }
    
  trace(L6,_T("-DMCoinAcceptorAccept %d"),rc);
  return rc;
}
#endif //_CPPUNIT

////////////////////////////////////
long SMStateBase::DMCoinAcceptorDisable(void)
{
  long rc = 0;
  trace(L6,_T("+DMCoinAcceptorDisable"));
    
  if (co.fTenderCashAllowed && dm.IsCoinAcceptorAvailable())
  {
    rc = DMCoinAcceptorLight(DM_MISC_STATE_OFF);
    if (rc>=0)
    {
      bIsCoinAcceptorEnabled = false;    //RFC330995
      rc = dm.CoinAcceptorDisable();  // Fixed problem found during unit test
    }
  }
    
  trace(L6,_T("-DMCoinAcceptorDisable %d"),rc);
  return rc;
}
// End Coin Acceptor Modifications

bool SMStateBase::DMKeepCurrencyAcceptorsDisabled()
{
    bool bKeepDisabled = false;
    if (dm.GetDegradedModeType() == DEGRADEDMODE_CARD_ONLY )
    {
        //SR742 - if were in card only mode, we disable cash devices. Don't need to enable it.
        bKeepDisabled = true;
    }
    else if (co.fLocaleDisableCashAcceptorsDuringItemization &&
            !bEnterTenderState && 
            !helpMode() && !storeMode())
    {
        bKeepDisabled = true;
    }
    if (bKeepDisabled)
    {
        if (bIsCoinAcceptorEnabled)
        {
            DMCoinAcceptorDisable();
        }
        if (bIsCashAcceptorEnabled)
        {
            DMCashAcceptorDisable();
        }
    }
    return bKeepDisabled;
}

#ifndef _CPPUNIT

//////////////////////////////////////////
long SMStateBase::DMCouponLight(const int lightState)
{
  long rc = 1; // SSCOADK-3006 needs to be non-zero 
  trace(L6,_T("+DMCouponLight %d"),lightState);
  
  //Coupon Light not part of 3x or 4x hardware. TAR 415538
  if( !_tcsicmp(co.csHWType, _T("SCOT5")) || !_tcsicmp(co.csHWType, _T("SCOT6")))   // SSCOADK-3006
  {
      rc = dm.PassbookLight(lightState);
  }
  else
  {
	  trace(L6,_T("Coupon light doesn't exist"));
  }  
  trace(L6,_T("-DMCouponLight rc = %d"),rc);
  return rc;
}

//////////////////////////////////////////////
long SMStateBase::DMDispenseChange(const long lAmt, CString& csCashCountsStatus, long *lDispenseErrorCode)
{
  long rc = 0;
  long lGloryRet = 0;   // Return code for glory device.
  long lDsp = 0; //TAR 373026
  trace(L6,_T("+DMDispenseChange %d"),lAmt);
      
  bIsCashAcceptorEnabled=false;      
  lLastDispense = 0;
  bool bDispense = true;
  if (TrainingModeObj.IsTMOn() && !co.nOperationsAllowDispenseInTrainingMode) // in training mode and option set to not dispense
  {
    lLastDispense = lAmt;
    rc = lAmt; // pretend that amount is dispensed but actually we don't since option is set to do not suspend
    bDispense = false;
  }
    
  if (bDispense && (lAmt>0))
  {
    rc = dm.CashChangerDispense(lAmt);

    // Since this is such a long function, save the return code
    // in a Glory-specific variable (see glory notes below) in case rc is
    // overwritten somewhere.
    lGloryRet = rc;

    // TAR 314536 - Begin.  Make dispense error code available to caller.
    if(lDispenseErrorCode != NULL)      
    {
        *lDispenseErrorCode = rc;
    }
    // TAR 314536 - End

    // Glory was busy for all dispense attempts.
    // Japan states that the return of any of the following errors guarantees
    // that nothing was dispensed.
    // If nothing was dispensed, then CashChangerGetDispensedCashList() (using DirectIO()
    // call) will return the last amount that was DEPOSITED.
    // Therefore, do not call CashChangerGetDispensedCashList(). 
    if(dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_GLORY) != -1) //SR672
    {
        switch(rc)
        {
        case OPOS_ECHAN_CASSETTEWAIT:
        case OPOS_ECHAN_BUSY:
            trace(L6,_T("-DMdispenseChange [Glory busy] error code: %d"), rc);
            lLastDispense = 0;
            return 0;
        case OPOS_E_ILLEGAL:
        case OPOS_ECHAN_SETERROR:
        case OPOS_ECHAN_IMPOSSIBLE:
        case OPOS_ECHAN_ERROR:
        case OPOS_ECHAN_DEPOSIT:
        case OPOS_ECHAN_PAUSEDEPOSIT:
        case OPOS_ECHAN_IFERROR:
        case OPOS_E_NOHARDWARE:
        case OPOS_ECHAN_OVER:
        case OPOS_ECHAN_OVERDISPENSE:
          trace(L6,_T("-DMdispenseChange ERROR [Glory] %d"), rc);
          lLastDispense = 0;
          return 0;
        }
    }

    /*
    CString CashChangerDispensedCashList();
    Description
    Returns a string specifying the amount dispensed both in bills and coins
    ie
    25:0,5:0,1:0;100:0,500:0,1000:0,2000:0
    for the last dispense operation.
    This operation is particularly useful for erratic dispense operations,
    such as PARTIAL_DISPENSE_BILLS, PARTIAL_DISPENSE_COINS, PARTIAL_DISPENSE_UNKNOWN.
    */
    //SR672
    dm.m_csCoinDispenserModel.MakeUpper();
    if (rc != OPOS_SUCCESS && (!( dm.m_csCoinDispenserModel.Find(_T("HOPPER")) != -1 ) && (rc == OPOS_E_FAILURE)))
    {
          trace(L6,_T("-DMdispenseChange %d"),rc);
          return 0;
    }
    // Code Removed from here for TAR 373026
    
    lDsp = DMReportDispensedCashList(); // TAR 373026
        
    lLastDispense = lDsp;                     // save it for Purgebills to check
    csCashCountsStatus = DMCheckCashCounts(true);
    rc = lDsp;                                // return amount actually dispensed

    // For Glory, if rc == OPOS_SUCCESS, then do not use values extracted from
    // dm.CashChangerGetDispensedCashList() for the return code of this function.  
    // Simply return lAmt instead.  
    // The values from dm.CashChangerGetDispensedCashList() will only be used
    // to update the Reports database and DMCC.
    // 
    // There is a test case where if a coin is inserted while another coin is 
    // being dispensed, the string returned from 
    // dm.CashChangerGetDispensedCashList() is not correct.  This is 
    // a Glory issue.  For this test case, the amounts used to update the 
    // Reports database will not be correct.  Japan is a aware of this and 
    // requested this workaround.
    if((dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_GLORY) != -1) && (lGloryRet==OPOS_SUCCESS))
    {
        rc = lAmt;
        lLastDispense = lAmt;

        if(lDsp != lAmt)
        {
            trace(L6, _T("DISPENSE WARNING: DirectIO() amount [%d] != requested amount [%d]."),
                lDsp, lAmt);
        }
    }

    dm.CashRecyclerUpdateDispensed(lLastDispense);  // TAR 311471
  }
    
  trace(L6,_T("-DMdispenseChange %d"),rc);
  return rc;
}

//+dp185016 support glory device
long SMStateBase::DMDispenseChange(CString& csChangeInDenomination,
                                   long *lDispenseErrorCode)
{
  long rc = 0;
  long lDsp = 0; //TAR 373026
  trace(L6,_T("+DMDispenseChangeByCount %s"),csChangeInDenomination);  

  bIsCashAcceptorEnabled=false;      
  rc = dm.CashChangerDispense( csChangeInDenomination );

    if(lDispenseErrorCode != NULL)      
    {
        *lDispenseErrorCode = rc;
    }

  //+dp185016 Recycler Platform Issue #9 - Currency Dispensed Reporting issue
    // Code Removed from here for TAR 373026
    
    lDsp = DMReportDispensedCashList(); // TAR 373026
        
    lLastDispense = lDsp;                     // save it for Purgebills to check
        
    rc = lDsp;    
  //-dp185016 Recycler Platform Issue #9 - Currency Dispensed Reporting issue

  trace(L6,_T("-DMDispenseChangeByCount returns %d"), rc);  

  return rc;
}
//-dp185016
//////////////////////////////////////////
// +TAR 373026
// Base on Johh Bush fix on Wal-Mart.
// Codes are base on DMDispenseChange method
long SMStateBase::DMReportDispensedCashList()
{
    trace(L7,_T("+SMStateBase::DMReportDispensedCashList"));

    int i = -1;
    int j = -1;
    int n = 0;
    long lDsp = 0;
    CString csCoin, csBill;
    CString csList = dm.CashChangerGetDispensedCashList();
    while(csList == ";" && n<15)  //device is not ready to give count, wait for 15 second
    {
          trace(L6,_T("dm.CashChangerGetDispensedCashList() not ready n=%d."),n);
          Sleep(1000);
          csList = dm.CashChangerGetDispensedCashList();
          n++;
    }
    if (n==15)
    {
          trace(L0,_T("SMStateBase::DMDispenseChange::CashChangerGetDispensedCashList() failed. "));
    }

    //We need to pass the whole string of data to be dispensed
    //example("1:4,5:2,10:1") XM's :CurrencyDispensed will handle this one
    dmcc.CurrencyDispensed(csList); //TAR 448434

    csList += ","; // append one last comma
    i = csList.Find(';');
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Change being Dispensed is Notes and Coins
/////////////////////////////////////////////////////////////////////////////////////////////////////
  if (i != -1)
  {
    csCoin = csList.Left(i);
    csCoin += ","; 
    csBill = csList.Right(csList.GetLength()-i-1);
    if (i >= 0) csCoin.SetAt(i,',');
    while (true)
    {
      int i = csCoin.Find(',');
      if (i<0) break;
      CString csWork = csCoin.Left(i);
      CString csDen;
      if (csWork != EMPTY_STRING)
      { 
        int i = csWork.Find(':');
        if(i<0) break;
        csDen = csWork.Left(i);         
      }
               
      csCoin = csCoin.Mid(i+1);
      i = csWork.Find(':');     // TAR 390025 - Removed extra space.
      
      if (i>0)
      {
        long lVal = _ttoi(csWork.Left(i));
        long lNum = _ttoi(csWork.Mid(i+1));
        long lRptAmt = (lVal * lNum);
        if (lNum > 0)
        {
            rp.m_bBill = false;
            trace(L7, _T("+rp.CurrencyDispensed"));
            rp.CurrencyDispensed(false,csDen,lNum,lRptAmt);
            trace(L7, _T("-rp.CurrencyDispensed<note:false>"));
            lDsp += (lVal * lNum);
        }
      }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Change being Dispensed is Notes 
/////////////////////////////////////////////////////////////////////////////////////////////////////
    if (csBill != EMPTY_STRING)
    while (true)
    {
      int j = csBill.Find(',');
      if (j<0) break;
      CString csWork = csBill.Left(j);
      CString csDen;
      if (csWork != EMPTY_STRING)
      { 
        int j = csWork.Find(':');
        if(j<0) break;
        csDen = csWork.Left(j);         
      }
               
      csBill = csBill.Mid(j+1);
      j = csWork.Find(':');
      
      if (j>0)
      {
        long lVal = _ttoi(csWork.Left(j));
        long lNum = _ttoi(csWork.Mid(j+1));
        long lRptAmt = (lVal * lNum);
        if (lNum > 0)
        {
            rp.m_bBill = true;
            trace(L7, _T("+rp.CurrencyDispensed"));
            rp.CurrencyDispensed(true, csDen,lNum,lRptAmt);
            trace(L7, _T("-rp.CurrencyDispensed<note:true>" ));
            lDsp += (lVal * lNum);
        }
      }
    }
  }
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Change being Dispensed is either only Coins or only Bills
/////////////////////////////////////////////////////////////////////////////////////////////////////
  else
  {
      CString csList2;
      bool    bBills = false;
      bool    bFound = false;
      csList2 = csList;
      // Find out whether we're dealing with coins or bills
      while (true)
      {
          // Go through the list until we get a definitive answer
          bFound = false;
          int i = csList2.Find(','); // Find next entry
          if (i<0) break;           // None found, exit
          CString csWork = csList2.Left(i);
                   
          csList2 = csList2.Mid(i+1);
          i = csWork.Find(':');
          
          if (i>0)  // Better be one
          {
              long lVal = _ttoi(csWork.Left(i));
              int nConfigCashListSize = co.nCashValueList.GetSize();

              for (j=0; j < nConfigCashListSize; j++)
              {
                  if (lVal == co.nCashValueList[j])        
                  {
                      // Found as bill
                      if (bFound)  // If we already found it as a coin
                      {
                          bFound = false;  // We still don't know
                      }
                      else
                      {
                          bFound = true;
                          bBills = true;
                      }

                  }
                  else if (   (co.nCashValueList[j] < 0) 
                           && ((0 - lVal) == co.nCashValueList[j]))
                  {
                      // Found as coin
                      bFound = true;
                      bBills = false;
                  }
              }
              if (bFound)
                  break;
          }
      }
    while (true)
    {
      int i = csList.Find(',');
      if (i<0) break;
      CString csWork = csList.Left(i);
      CString csDen;
      if (csWork != EMPTY_STRING)
      { 
        int i = csWork.Find(':');
        if(i<0) break;
        csDen = csWork.Left(i);         
      }
               
      csList = csList.Mid(i+1);
      i = csWork.Find(':');
      
      if (i>0)
      {
        long lVal = _ttoi(csWork.Left(i));
        long lNum = _ttoi(csWork.Mid(i+1));
        long lRptAmt = (lVal * lNum);
        rp.m_bBill = bBills;
        trace(L7, _T("+rp.CurrencyDispensed"));
        rp.CurrencyDispensed(bBills,csDen,lNum,lRptAmt);
        trace(L7, _T("-rp.CurrencyDispensed<note:false>"));
        lDsp += (lVal * lNum);
      }
    }
  }
  trace(L7,_T("-SMStateBase::DMReportDispensedCashList %d"), lDsp);
  return lDsp;
}
// -TAR 373026
//////////////////////////////////////////
void SMStateBase::DMCashChangerPurgeBills(void)
{
  trace(L6,_T("+DMCashChangerPurgeBills"));
    
//  if (lLastDispense>99)   // TAR 127340 // only required if bills dispensed i.e. >99 cents
    if (!DMWasCashTaken())  // TAR 127340 required if there is money still in dispenser
  {
    dm.CashChangerPurgeBills();
  }
    
  lLastDispense = 0;
    
  trace(L6,_T("-DMCashChangerPurgeBills"));
}

// Begin TAR 127340
//////////////////////////////////////////
bool SMStateBase::DMWasCashTaken(void)
{
    return (dm.GetCashChangerCashTaken() != 0) ;
}
// End TAR 127340

// Begin TAR 127340
//////////////////////////////////////////
void SMStateBase::DMSetCashTaken(void)
{
    dm.SetCashChangerCashTaken(true);
}
// End TAR 127340

////////////////////////////////
#endif
long SMStateBase::DMScaleEnable(void)
{
  long rc = 0;
  trace(L6,_T("+DMScaleEnable"));
    
  rc = dm.ScaleEnable();    // enable the scale
    
  trace(L6,_T("-DMScaleEnable %d"),rc);
  return rc;
}
#ifndef _CPPUNIT
/////////////////////////////////
long SMStateBase::DMScaleDisable(void)
{
  long rc = 0;
  trace(L6,_T("+DMScaleDisable"));
    
  rc = dm.ScaleDisable();   // disable the scale
    
  trace(L6,_T("-DMScaleDisable %d"),rc);
  return rc;
}

//////////////////////////////
long SMStateBase::DMMSREnable(void)
{
   long rc = 0;
   trace(L6,_T("+DMMSREnable"));
   // SR742 - don't enable the MSR device if we are in Cash Only mode. Swide card has no effect in this mode.
   if ( dm.GetDegradedModeType() != DEGRADEDMODE_CASH_ONLY ||
       co.fOperationsEnableMSRDegradedModeCashOnly )
   {
      // if credit or debit tender is defined (allowed) then
      // enable the MSR
      //if (co.fTenderCreditAllowed || co.fTenderDebitAllowed || co.fStateAllowLoyaltyCard)  Tar 166621
 
      rc = dm.MSREnable();                      // enable the MSR
      if (rc>=0)
      {
         // Unfortunately, just for the case of the 5945, the SCOTEncryptor
         // class needs to know whether or not the MSR is enabled.  This is because
         // if a prompt must be displayed when the MSR is enabled, we have to update
         // the prompt table entry for "Please Slide Card", but if the msr is disabled, 
         // we just have to display the string.  Kludgy, but then so is the 5945
         // 3/08/2000 - ncr_barn

         gpSCOTEncryptor->SetMSREnabled(true);

         // No need to do this every time, just do it once when we 
         // open and claim the device in DMInterface::Initialize
         //rc = dm.SetMSRTracksToRead(DM_MSR_TRACK_1_2);   // set tracks to read
         //if (rc>=0)
         {
            rc = DMMSRLight(DM_MISC_STATE_ON); // turn on MEI light
         }
      }
   }
  
  trace(L6,_T("-DMMSREnable %d"),rc);
  return rc;
}

///////////////////////////////////////
long SMStateBase::DMMSRLight(const int lightState)
{
  long rc = 0;
  if (co.fStateHasCardMEI)
  {
    trace(L6,_T("+DMMSRLight %d"),lightState);

    if (co.fTenderCreditAllowed || co.fTenderDebitAllowed || co.fStateAllowLoyaltyCard)
      {
          rc = dm.CardReaderLight(lightState);
      }
    trace(L6,_T("-DMMSRLight %d"),rc);
  }
  return rc;
}

/////////////////////////////
long SMStateBase::DMMSREject(void)
{
  long rc = 0;
  trace(L6,_T("+DMMSREject"));
    
  if (cDMLastTrack)                 // if there is a card inserted
  {
    delete [] cDMLastTrack; //SSCOP-4165
    cDMLastTrack = NULL;
    nDMLastTrackLen = 0;
    csDMEncryptedPin = EMPTY_STRING;        
  }

  if (co.fTenderIsMotorizedMSR)
    rc = dm.MSREjectCard();
    
  trace(L6,_T("-DMMSREject %d"),rc);
  return rc;
}

///////////////////////////////
long SMStateBase::DMMSRCapture(void)
{
  long rc = 0;
  trace(L6,_T("+DMMSRCapture"));

  // Depending on the exact state we're in, there may or
  // may not still be last track data.
  if (cDMLastTrack)
  { // if there is, delete it
    delete [] cDMLastTrack; //SSCOP-4165
    cDMLastTrack = NULL;
    nDMLastTrackLen = 0;
    csDMEncryptedPin = EMPTY_STRING;
  }
  waitingForCardRemoval = false; //TAR260141
  rc = dm.MSRCaptureCard();
    
  trace(L6,_T("-DMMSRCapture %d"),rc);
  return rc;
}

///////////////////////////////
long SMStateBase::DMMSRDisable(void)
{
  long rc = 0;
  trace(L6,_T("+DMMSRDisable"));
    
  ///DcL-PR#022598-8rc = DMMSREject();
  ///DcL-if (rc>=0)
  {
    rc = DMMSRLight(DM_MISC_STATE_OFF);
    // The light is not of primary concern with regard to disabling the MSR.
    // Also, for Scot 2.0 we still have a buggy MISCIF control where the lights
    // fail to work sometimes and then we fail to disable - this is bad!!!
    // So until all the bugs get worked out, we will just disable and not care
    // about the Light return code.  ncr_barn - 01/27/2000
    //if (rc>=0)
    {
      rc = dm.MSRDisable();               // disable the MSR

      // Unfortunately, just for the case of the 5945, the SCOTEncryptor
      // class needs to know whether or not the MSR is enabled.  This is because
      // if a prompt must be displayed when the MSR is enabled, we have to update
      // the prompt table entry for "Please Slide Card", but if the msr is disabled, 
      // we just have to display the string.  Kludgy, but then so is the 5945
      // 3/08/2000 - ncr_barn
      
      if (gpSCOTEncryptor)
          gpSCOTEncryptor->SetMSREnabled(false);
    }
  }
    
  trace(L6,_T("-DMMSRDisable %d"),rc);
  return rc;
}

////////////////////////////////////
long SMStateBase::DMEncryptorEnable(void)
{
  long rc = -1;
  trace(L6,_T("+DMEncryptorEnable"));
    
  if (gpSCOTEncryptor)
    rc = gpSCOTEncryptor->Enable();   // enable the encryptor
    
  trace(L6,_T("-DMEncryptorEnable %d"),rc);
  return rc;
}

////////////////////////////////////
long SMStateBase::DMEncryptorMaster(void)
{
  long rc = -1;
  trace(L6,_T("+DMEncryptorMaster"));
    
    // see comments in DMEncryptorSetup(void)
  if (gpSCOTEncryptor)
      rc = gpSCOTEncryptor->StoreMasterKeyFromPinpad();
    
  trace(L6,_T("-DMEncryptorMaster %d"),rc);
  return rc;
}

///////////////////////////////////
long SMStateBase::DMEncryptorSetup(LPCTSTR szWorkingKey, 
                                   TBPINBLOCKFORMAT nPinBlockFormat, 
                                   TBPINPADSYSTEM   nPinpadSystem,
                                   TBPINENCODING    nPinBlockEncoding)
{
  long rc = -1;
  trace(L6,_T("+DMEncryptorSetup"));

    // we need to get the right 12 digits not counting the last digit
    //    CString csA = szChargeAccount;
  //
  // Actually, now that we are in the world of other encryptors besides
  // BAPE, we will pass the whole acct# and let the BAPE derived class
  // do the following lines of code.  2/15/2000 - ncr_barn
    //CString csA = csChargeAccount;
    //csA = csA.Right(13);
    //csA = csA.Left(12);
  if (gpSCOTEncryptor)
      rc = gpSCOTEncryptor->DoPINTransaction(csChargeAccount, 
                                           szWorkingKey, 
                                           nPinBlockFormat, 
                                           nPinpadSystem,
                                           nPinBlockEncoding);
  trace(L6,_T("-DMEncryptorSetup %d"),rc);
  return rc;
}

///////////////////////////////////
long SMStateBase::DMEncryptorKeyTest()
{
  long rc = -1;
  trace(L6,_T("+DMEncryptorKeyTest"));
  if (gpSCOTEncryptor)
    rc = gpSCOTEncryptor->ValidateMasterKey();
  trace(L6,_T("-DMEncryptorKeyTest %d"),rc);
  return rc;
}



///////////////////////////////////////
CString SMStateBase::DMEncryptorGetPIN(void)
{
  CString csRet;
  trace(L6,_T("+DMEncryptorGetPIN"));
    
  if (gpSCOTEncryptor)
    csRet = gpSCOTEncryptor->GetEncryptedPIN();
    
  trace(L6,_T("-DMEncryptorgetPIN <%s>"),csRet);
  return csRet;
}

/////////////////////////////////////
SMStateBase *SMStateBase::DMEncryptorSecured(void)// usually overridden in EnterPin state
{
  trace(L6,_T("+DMEncryptorSecured"));
  trace(L6,_T("-DMEncryptorSecured"));
  return STATE_NULL;
}

/////////////////////////////////////
SMStateBase *SMStateBase::DMEncryptorCanceled(void)// usually overridden in EnterPin state
{
  trace(L6,_T("+DMEncryptorCanceled"));
  trace(L6,_T("-DMEncryptorCanceled"));
  return STATE_NULL;
}

/////////////////////////////////////
SMStateBase *SMStateBase::DMEncryptorCompleted(void)// usually overridden in EnterPin state
{
  trace(L6,_T("+DMEncryptorCompleted"));
  trace(L6,_T("-DMEncryptorCompleted"));
  return STATE_NULL;
}

/////////////////////////////////////
long SMStateBase::DMEncryptorDisable(void)
{
  long rc = -1;
  trace(L6,_T("+DMEncryptorDisable"));
    
  if (gpSCOTEncryptor)
    rc = gpSCOTEncryptor->Disable();              // disable the encryptor
    
  trace(L6,_T("-DMEncryptorDisable %d"),rc);
  return rc;
}

long SMStateBase::DMEncryptorCancelSecureMode(void)
{
  long rc = -1;
  trace(L6,_T("+DMEncryptorCancelSecureMode"));

  if (gpSCOTEncryptor)
    rc = gpSCOTEncryptor->CancelPINEntry();

  trace(L6,_T("-DMEncryptorCancelSecureMode"));
  return rc;
}

//////////////////////////////////////////////////////////
// DMCheckCashCounts
// at startup and after each cash dispense, check the bill and coin counts.
// If they are below the configured low level, set the green light to flash
// Used to count the amount of currency in the hopper once the low was signalled,
// Now this functionality is in the device control (TAR#133269)
//
// fAdjust used to denote whether or not we should adjust these cash counts, now it 
// is not used. It was left in in case the autocount functionality is ever put back in.
//
// csHighCurrencies contains the bins with a high/nearfull status.
// csCoinDispenserAlert - returns the alerts for the coin dispenser
// csCashDispenserAlert - returns the alerts for the cash dispenser
//
// Returns a string describing the low bins.
/////////////////////////////////////////////////
CString SMStateBase::DMCheckCashCounts(const bool fAdjust, CString& csHighCurrencies, CString& csCoinDispenserAlert, CString& csCashDispenserAlert)
{
  bool bNoCash = false;                   // TAR 310136
  if(m_nCoinDispenserWithLowSensor == 2)  // if device is not ready to check if there is an enhanced media low sensor last time
                                          // check it again. //tar 246539
  {
      DMCheckForCoinDispenserLowSensor();
  }

  CString csList = "";
    
  // If we are a) not accepting cash, and b) not dispensing cash,
  // then we don't need to do this since we aren't using the cash
  // changer at all. So we will do a short-circuit check
  // here to see if we really need to do this.
  if (!co.fTenderCashAllowed    && 
      !co.fTenderCreditCashBack && 
      !co.fTenderDebitCashBack  &&
      !co.fTenderEBTCBCashBack)
  {
    bNoCash = true;
  }

  int i,j;
    

  // BeginCodeLocale
  // These values are now read from configobject
  // -----------------------------------------------
  // const int MAX_LISTVALUE = 8;       
  // Possible Cash Value of hopper and cassette
  // !!! Coins are negative value
  // int CashValueList[MAX_LISTVALUE] = {  -1, -5, -10, -25, 100, 500, 1000, 2000}; 
  // minimum thresholds provided by Rafael Torres NCR
  // int CashSafeList[MAX_LISTVALUE] = { 101, 60, 101,  50,  26,  26,   26,   26};
  // low thresholds for the green blinking
  // int CashLowList[MAX_LISTVALUE] = {  10, 10,  10,  10,  20,  20,   20,   20};


  // -----------------------------------------------
  // EndCodeLocale
  
    
  BOOL bDiscrepancy = true;
  CString csCashCounts;
    
  if (!bNoCash)
  {
    BSTR pCashCounts = SysAllocStringByteLen(NULL,200);
    *pCashCounts = 0;   //Tar 246679
    dm.CashChangerGetCashCounts(&pCashCounts, &bDiscrepancy);

    SetCashDiscrepancyStatus(bDiscrepancy); // Add Glory CM support
     
    csCashCounts = pCashCounts;   
    csCashCounts += ',';      // suffix a comma, so later Find(',') works correctly
    
    SysFreeString(pCashCounts);             // free the BSTR

    if (csCashCounts == ",") // TAR 310136 - Limited processing if no cash counts  
    {
        bNoCash = true;
    }
  }
  // parse the cstring to get the values and counts
  CArray<int,int> nCashValue;
  CArray<int,int> nCashCount;
  int nTimes = -1;                  // coins are negative
  int nSemiColonIndex;
  int nStrIndex;

  if (!bNoCash)
  {
     if( !dm.m_csCashRecyclerModel.IsEmpty() || !co.fLocaleCashManagementScreen)  //tar 312039
     {
        trace(L6,_T("Cash Counts <%s>"),csCashCounts);  //395250
       if (csCashCounts.GetAt(0) == ';')     //TAR#125460 if no coin hoppers set up, 
       {                                     //need to skip the beginning semicolon
           csCashCounts = csCashCounts.Mid(1);
           nTimes = 1;
       }
  
       while (!csCashCounts.IsEmpty())
       {
           nStrIndex = csCashCounts.Find(':');
           if (nStrIndex <= 0) break;
           nCashValue.Add( _ttol(csCashCounts) * nTimes );
           nCashCount.Add( _ttol(csCashCounts.Mid(nStrIndex+1)) );
           nStrIndex = csCashCounts.Find(',');
           if (nTimes == -1)                 // still processing coins
           {
               nSemiColonIndex = csCashCounts.Find(';'); // coin and bill separator
               // if we ever get a $1.00 coin...
               // make all coin negative for to separate $1.00 coin from $1.00 bill
               if (nSemiColonIndex >= 0 && nSemiColonIndex < nStrIndex)
               {
                   nStrIndex = nSemiColonIndex;
                   nTimes = 1;   // if past the semi-colon the rest are positive bills
               }
           }
           if (nStrIndex) 
               csCashCounts = csCashCounts.Mid(nStrIndex+1);
       }
     }
     else
     {
       //+dp185016 generic cash management - base our data from DMCashCounts class
       CArray<CString,CString> caCashAcceptorDenomList, caCoinAcceptorDenomList, caCashDispenserDenomList, caCoinDispenserDenomList;
       trace(L6,_T("Getting Currency Denominations."));
   //    SMSmCashManagementBase::TestData(&caCashAcceptorDenomList, &caCoinAcceptorDenomList, &caCashChangerDenomList, &caCoinChangerDenomList);
       dmcc.GetCashDenoms(&caCashAcceptorDenomList, &caCoinAcceptorDenomList, &caCashDispenserDenomList, &caCoinDispenserDenomList);
       trace( L6, _T("denom Sizes : Cash Acceptor : %d. CoinAcceptor : %d. cashdispenser: %d. coindispenser : %d"),
         caCashAcceptorDenomList.GetSize(), caCoinAcceptorDenomList.GetSize(), 
         caCoinAcceptorDenomList.GetSize(), caCoinDispenserDenomList.GetSize() );
  
       for( int i = 0; i < caCoinDispenserDenomList.GetSize(); i++ )
       {
         nCashValue.Add( _ttol( caCoinDispenserDenomList[i] ) * -1 );
       }
       for( i = 0; i < caCashDispenserDenomList.GetSize(); i++ )
       {
         nCashValue.Add( _ttol( caCashDispenserDenomList[i] ) );
       }
  
       CArray<int,int> caCashAcceptorCountList, caCoinAcceptorCountList, caCashDispenserCountList, caCoinDispenserCountList;
       trace(L6,_T("Getting Currency counts."));
   //    SMSmCashManagementBase::TestCount(&caCashAcceptorCountList, &caCoinAcceptorCountList, &caCashDispenserCountList, &caCoinDispenserCountList);
       dmcc.GetCashCounts(&caCashAcceptorCountList, &caCoinAcceptorCountList, &caCashDispenserCountList, &caCoinDispenserCountList);
       trace( L6, _T("Sizes : Cash Dispenser Count List : %d. Coin Dispenser Count List : %d"),
         caCashDispenserCountList.GetSize(),caCoinDispenserCountList.GetSize());

       nCashCount.Copy( caCoinDispenserCountList );
       nCashCount.Append( caCashDispenserCountList );
  
     }
  }

  dm.SetCoinsOrBillsVeryLow(false);  //TAR#144557 Don't set this here!  LPM061900
  dm.SetCoinsVeryLow(false);         //TAR 279497
  dm.SetBillsVeryLow(false);         //TAR 279497

  CString csCoinDispenserRAPList = _T("");  
  CString csCashDispenserRAPList = _T("");

  if (!bNoCash)
  {
     ASSERT(co.nCashValueList.GetSize() == co.nCashLowList.GetSize());
     ASSERT(nCashValue.GetSize() == nCashCount.GetSize());

     int nCashValueSize = nCashValue.GetSize();
     int nConfigCashListSize = co.nCashValueList.GetSize();

     //For each currency value given by the dm, find the low value specified in the scotopts
     //file (the value when the green light should blink and notify the currency is low

     CString *csDispenserRAPList;


       CString *csDispenserList;
     for ( i=0; i < nCashValueSize; i++ ) 
     {
         for (j=0; j < nConfigCashListSize; j++)
         {
             // if cash value = -1 means coin
             if (nCashValue[i] == co.nCashValueList[j])        
             {
                   double nPercentage = (((double)nCashCount[i] / co.nCashCapacity[j]) * 100 );
                   CString csPercentage, csFormat = _T("%.0f%%");
                   if( nPercentage > 100 )
                   {
                     nPercentage = 100;
                   }
                   else if( nPercentage > 0 && nPercentage < 0.95 )
                     {csFormat = _T("%.1f%%");} // display just one digit to show not zero
                     trace( L6, _T("Count Percentage : %f. denom : %d. count %d. capacity %d.low level: %d"), nPercentage, 
                     nCashValue[i], nCashCount[i], co.nCashCapacity[j], co.nCashLowList[j]);
            
                   csPercentage.Format((CString)csFormat, nPercentage);
                 //if the count is lower than the scotopts low setting
                 if (nCashCount[i] <= co.nCashLowList[j] && co.nCashLowList[j] != 0)   
                 {
                     CString csWork;
                     PSTEXT msgCashType;
                     int nCurrencyLow;
                  
                     // see if cash or coin to set up the system message screen msg
                     if (nCashValue[i] < 0 )
                     {
                         msgCashType = MSG_COIN;
                         nCurrencyLow = 0-nCashValue[i];
                         dm.SetCoinsVeryLow(true);  // TAR 279497 
                         if ( csCoinDispenserRAPList.GetLength() == 0) 
                         {
                             //+dp185016 tar 305024
                             csCoinDispenserRAPList = ps.GetPSText(MSG_COIN_DISPENSER_STATUS, SCOT_LANGUAGE_PRIMARY);
                         }
                           csDispenserList = &csCoinDispenserAlert;
                           csDispenserRAPList = &csCoinDispenserRAPList;//dp185016 tar 305024
                         int nCurrencyValueTmp= nCurrencyLow/co.nLocalePriceCentUnit;  //Tar 322840
                         if ( nCurrencyValueTmp >= 1)  //Tar 322840
                         {
                             nCurrencyLow=nCurrencyValueTmp;
                             msgCashType = MSG_BILL;
                         }

                     }
                     else
                     {
                         msgCashType = MSG_BILL;
                         nCurrencyLow = nCashValue[i] / co.nLocalePriceCentUnit;
                         dm.SetBillsVeryLow(true);  // TAR 279497 
                         if ( csCashDispenserRAPList.GetLength() == 0) 
                         {
                             //+dp185016 tar 305024
                             csCashDispenserRAPList = ps.GetPSText(MSG_CASH_DISPENSER_STATUS, SCOT_LANGUAGE_PRIMARY);
                         }
                           csDispenserList = &csCashDispenserAlert;
                           csDispenserRAPList = &csCashDispenserRAPList; //dp185016 tar 305024
                     }

                     if( co.fLocaleCashManagementScreen )
                     { // tar 298689 - if cashmanagementscreen is turned on, then show percentage

                       if( nPercentage <= 0 )
                       { //dp185016 tar 301330
                                csWork.Format(_T("   %3d %s %s"),nCurrencyLow,ps.GetPSText(msgCashType),
                                   ps.GetPSText(MSG_BINISEMPTY, SCOT_LANGUAGE_PRIMARY));
                          
                       }
                       else
                       {
                         csWork.Format(_T("   %3d %s %s"),nCurrencyLow,ps.GetPSText(msgCashType),
                                 ps.GetPSText(MSG_CURRENCYLOWPERCENTAGE, SCOT_LANGUAGE_PRIMARY));
                         csWork.Format((CString) csWork, csPercentage);
                       }

                     }
                     else
                     {
						 if (nCashCount[i] <=0) //SSCOADK-4369
                         {
							 csWork.Format(_T("   %3d %s %s"),nCurrencyLow,ps.GetPSText(msgCashType, SCOT_LANGUAGE_PRIMARY),
                                   ps.GetPSText(MSG_BINISEMPTY, SCOT_LANGUAGE_PRIMARY));
                         }
						 else
                         {
                             csWork.Format(_T("   %3d %s %s"),nCurrencyLow,ps.GetPSText(msgCashType),
                                   ps.GetPSText(MSG_CURRENCYISLOW, SCOT_LANGUAGE_PRIMARY));
                         }
                     }
                     csList += "\n" + csWork;
                       *csDispenserList += "\n" + csWork;
                        //*csDispenserRAPList += csWork + _T(";");//dp185016 tar 305024
                  *csDispenserRAPList += "\n" + csWork;

                 } 

                //+dp185016 support glory device - support currency is high
                if( dm.m_csCashRecyclerModel.IsEmpty() == FALSE)
                {
                  trace( L6, _T("Currency High : %d, %d, %d"), nCashValue[i], nCashCount[i], co.nCashHighList[j]  );
                  if (nCashCount[i] >= co.nCashHighList[j] )   
                  {
                      CString csWork;
                      PSTEXT msgCashType;
                      int nCurrencyHigh;
                              dm.SetCoinsOrBillsVeryLow(true); // we could use this variable to indicate high also?
                  
                      // see if cash or coin to set up the system message screen msg
                      if (nCashValue[i] < 0 )
                      {
                          msgCashType = MSG_COIN;
                          nCurrencyHigh = 0-nCashValue[i];
                          csDispenserList = &csCoinDispenserAlert;
                          csDispenserRAPList = &csCoinDispenserRAPList;//dp185016 tar 305024
                          int nCurrencyValueTmpHigh= nCurrencyHigh/co.nLocalePriceCentUnit;  //Tar 322840
                          if ( nCurrencyValueTmpHigh >= 1) //Tar 322840
                          {
                             nCurrencyHigh=nCurrencyValueTmpHigh;
                             msgCashType = MSG_BILL;
                          }
                      }
                      else
                      {
                          msgCashType = MSG_BILL;
                          nCurrencyHigh = nCashValue[i] / co.nLocalePriceCentUnit;
                          csDispenserList = &csCashDispenserAlert;
                          csDispenserRAPList = &csCashDispenserRAPList;//dp185016 tar 305024
                      }
                      if( co.fLocaleCashManagementScreen )
                      { // tar 298689 - if cashmanagementscreen is turned on, then show percentage
                        csWork.Format(_T("   %3d %s %s"),nCurrencyHigh,ps.GetPSText(msgCashType),
                                     ps.GetPSText(MSG_CURRENCYISHIGH, SCOT_LANGUAGE_PRIMARY));
                        csWork.Format((CString) csWork, csPercentage);
                      }
                      else
                      {
                        csWork.Format(_T("   %3d %s %s"),nCurrencyHigh,ps.GetPSText(msgCashType),
                                     ps.GetPSText(MSG_CURRENCYHIGH, SCOT_LANGUAGE_PRIMARY));
                      }
                      trace( L6, _T("Added to High : %s"), csWork);
                      csHighCurrencies += "\n" + csWork;
                      *csDispenserList += "\n" + csWork;
                      *csDispenserRAPList += csWork + _T(";");//dp185016 tar 305024
                      //*csDispenserRAPList += "\n" + csWork;

                  } 
                }
                //-dp185016

                break;  //break out of innermost loop since we found the currency config values       
            }
        }
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////////
  //awk.11.12.2 - Get Printer Status/Cash List/And Acceptor status and send it to RAP
  // This change was for the Lane Status functionality on RAP.

  CString csWork1,csWork2, csRAPMediaInfo;
  //dp185016 support glory device - support currency is high
  if (!bNoCash)
  {
      //   if (csCoinDispenserRAPList.IsEmpty() && csCashDispenserRAPList.IsEmpty()) //dp185016 tar 305024
      if (csList.IsEmpty() && csHighCurrencies.IsEmpty()) //TAR 329383
          csRAPMediaInfo += (ps.GetPSText(MSG_CURRENCYALLNORMAL, SCOT_LANGUAGE_PRIMARY) + "\n"); 
      else
          csRAPMediaInfo += (csCoinDispenserRAPList + "\n" + csCashDispenserRAPList + "\n" );//dp185016 tar 305024
  }

  if (!dm.GetPrinterPaperLow()) //tar 255562
  {
    getPrinterStatusLine(lastDMReceiptCode,csWork1);
  }
  else //tar 255562
  {
    getPrinterStatusLine(PTR_SUE_REC_NEAREMPTY,csWork1); //TAR 450978
  }
  csWork2 = ps.GetPSText(TXT_RECEIPT_PRINTER, SCOT_LANGUAGE_PRIMARY);
  csWork2 += _T(": ");
  csWork2 += csWork1;
  csRAPMediaInfo += (csWork2 + "\n");
  CString csRAPReceipt, csRAPJournal, csRAPCoin, csRAPCash; 
  csRAPReceipt = csWork2;

  if (dm.IsDeviceConfigured( DMCLASS_JOURNAL, 0) )
  {
        getPrinterStatusLine(lastDMJournalCode,csWork1);
        csWork2 = ps.GetPSText(TXT_JOURNAL_PRINTER, SCOT_LANGUAGE_PRIMARY);
        csWork2 += ": ";
        csWork2 += csWork1;
        csRAPJournal = csWork2 ;
        csRAPMediaInfo += (csWork2 + "\n");
  }
  
  getCashAcceptorStatusLine(lastDMCashAcceptorCode, csWork1);
  csRAPMediaInfo += (csWork1 + "\n");
  csRAPCash = csWork1 ;
  
  getCoinAcceptorStatusLine(lastDMCoinAcceptorCode, csWork1);
  csRAPMediaInfo += (csWork1 + "\n");
  csRAPCoin = csWork1;
  
  CString csStateName = this->GetRuntimeClass()->m_lpszClassName;
  if( csStateName.Find( _T("MediaStatus") ) == -1 )
  {
    //fix mediastatus screen "medialist" issue(got messed up). if mediastatus context,
    //just let mediastatus state add to the medialist datasource
    trace( L6, _T("Tell rap of Media Info. %s"), csStateName );
    ps.ResetMediaList();

    CString csLowCurrencies = csRAPMediaInfo;
    bool bDisplayedHeader = false;

    bool bCurrencyNormal = true; //dp185016 support glory device - support currency is high


    CString *csFirstGroup, *csSecondGroup;
    // grouped by low, and high currencies, OR
    // grouped by coin dispenser and cash dispenser
    PSTEXT psFirstMsg, psSecondMsg;
    
    if( dm.m_csCashRecyclerModel.IsEmpty() == FALSE )
    {
      csFirstGroup = &csCoinDispenserAlert;
      csSecondGroup = &csCashDispenserAlert;
    }
    else
    {
      csFirstGroup = &csLowCurrencies;
      csSecondGroup = &csHighCurrencies;
    }
    trace( L6, _T("csFirstGroup. %s"), csFirstGroup->Mid(0) );
    trace( L6, _T("csSecondGroup. %s"), csSecondGroup->Mid(0) );

    
    
    // Trim leading and trailing whitespace
        //csFirstGroup->TrimLeft();     //TAR 355345 Commented to set the Cash & Coin dispenser status messages aligned in RAP screen.
    csFirstGroup->TrimRight();
    //csSecondGroup->TrimLeft();    //TAR 355345 Commented to set the Cash & Coin dispenser status messages aligned in RAP screen.
    csSecondGroup->TrimRight();
    if (!csFirstGroup->IsEmpty())
      { 
        if( !dm.m_csCashRecyclerModel.IsEmpty() )
        {
            psFirstMsg = MSG_COIN_DISPENSER_STATUS;
        }
        else
        {
            psFirstMsg = MSG_CURRENCYLOWSTATUS;
        }
          while (!csFirstGroup->IsEmpty())
          { 
              if (!bDisplayedHeader)
              {
                  ps.AddMediaList(ps.GetPSText(psFirstMsg, SCOT_LANGUAGE_PRIMARY));
                  bDisplayedHeader = true;
              }

              nStrIndex = csFirstGroup->Find(_T('\n'));
              if (nStrIndex == -1)
              {
                  ps.AddMediaList(*csFirstGroup);
                  csFirstGroup->Empty();
              }
              else
              {
                  ps.AddMediaList(csFirstGroup->Left(nStrIndex));
                  *csFirstGroup = csFirstGroup->Mid(nStrIndex+1);
              }
          }
          bCurrencyNormal = false;
      }

      bDisplayedHeader = false;
      if (!csSecondGroup->IsEmpty())
      { 
          if( !dm.m_csCashRecyclerModel.IsEmpty() )
          {
            psSecondMsg = MSG_CASH_DISPENSER_STATUS;
          }
          else
          {
            psSecondMsg = MSG_CURRENCYHIGHSTATUS;
          }
          while (!csSecondGroup->IsEmpty())
          { 
              if (!bDisplayedHeader)
              {
                  ps.AddMediaList(ps.GetPSText(psSecondMsg, SCOT_LANGUAGE_PRIMARY));
                  bDisplayedHeader = true;
              }

              nStrIndex = csSecondGroup->Find(_T('\n'));
              if (nStrIndex == -1)
              {
                  ps.AddMediaList(*csSecondGroup);
                  csSecondGroup->Empty();
              }
              else
              {
                  ps.AddMediaList(csSecondGroup->Left(nStrIndex));
                  *csSecondGroup = csSecondGroup->Mid(nStrIndex+1);
              }
          }
          bCurrencyNormal = false;
      }


   if( bCurrencyNormal && !bNoCash )
      {ps.AddMediaList(ps.GetPSText(MSG_CURRENCYALLNORMAL, SCOT_LANGUAGE_PRIMARY));}

    //ra.OnAddOtherEvent("COMMAND=MEDIA" + csRAPMediaInfo); 
    //ra.OnAddOtherEvent("COMMAND=MEDIA" + csRAP2);
    //  ps.AddMediaList(csRAPReceipt);
    //  ps.AddMediaList(csRAPJournal);
    //  ps.AddMediaList(csRAPCoin);
    //  ps.AddMediaList(csRAPCash);
     

  }
  //////////////////////////////////////////////////////////////////////////////////////////////
  //awk.11.12.2 End

  return csList;
}

/////////////////////////////////////////////////
SMStateBase *SMStateBase::DMSetScaleWeight(bool &fCheckHealth)
{
  CString csWork;
  bool fCaseHandled = false;
    
  switch (lDMLastCode)
  {
      /* Fix me later
    CASE_IGNORE_NOW(STATUS_UNEXPECTED);
        
    CASE_HEALTH_NOW(ERROR_UNEXPECTED);
    */
  // +TAR  299060
  case DM_ERROR_GENERALFAILURE:       // -1
    lDMScaleWeight = lDMLastCode;   // set reason for DMScale()
    trace(L6,_T("-Parse scale general failure"));
    return DMScale();
  // -TAR  299060

  case DM_ERROR_SCALE_OVERWEIGHT:   // -2
  case DM_ERROR_SCALE_UNDERZERO:    // -3
    // a checkhealth will be issued after the scale is reset
    // from SMScaleBroken if no valid weights arrive before then
    // ScaleBroken is activated from Attract or a WeightRequired item
    lDMScaleWeight = lDMLastCode;   // save error reason -2 or -3
    trace(L6,_T("-Parse scale code %d"),lDMScaleWeight);
    return DMScale();
  case DM_ERROR_SCALE_UNSTABLE:       // -4
    lDMScaleWeight = lDMLastCode;   // set reason for DMScale()
    trace(L6,_T("-Parse scale unstable"));
    fDMScaleChanged = TRUE;
    return DMScale();
  case DM_ERROR_SCALE_WEIGHTUNCHANGED: // -5
    trace(L6,_T("-Parse scale unchanged"));
    return STATE_NULL;
  default:
    if (lDMLastCode>=0) 
    {
      // If we are in lbs, then we want to get rid of any effect of the
      // thousandths.  However, since all the weight formatting code is set
      // up to expect the thousandths, we will just do the rounding to the nearest
      // hundredth here and leave the thousandths place on (as a 0)
      if (!co.fOperationsScaleMetric)
      {
        int remainder = lDMLastCode % 10;
        if (remainder >= 5)
        {
          lDMLastCode = lDMLastCode + (10 - remainder);
        }
        else
        {
          lDMLastCode = lDMLastCode - remainder;
        }
      }
      
      lDMScaleWeight = lDMLastCode;
    }
  }
  trace(L6,_T("Scale weight = %d"),lDMScaleWeight);
    
    /* We now work with 1/1000th for lb and kg
  // if (lDMScaleWeight>0)
  // {
  //   if (!co.fOperationsScaleMetric)// if not 1/1000ths KG then
  //   {
  //    lDMScaleWeight /= 10;                 // convert to 1/100ths lbs
  //     trace(L6,"Scale weight/10 = %d",lDMScaleWeight);
  //   }
  // }
    */
    
  return DMScale();                               // prime VIRTUAL scale
}

///////////////////////////////////////////////
bool SMStateBase::ReceiptPrintSuccessful()
{
  return dm.nDMLastReceiptFunction == 0;
}


///////////////////////////////////////////////
void SMStateBase::DMReceiptPrintBarcode(LPCTSTR szBarcode, long lBCType)
{
    dm.ReceiptPrintBarcode(szBarcode, lBCType);
}

#endif //_CPPUNIT

CString SMStateBase::GetGLOBALAddThe128Prefix() {
    return csGLOBALAddThe128Prefix;
}

#ifndef _CPPUNIT
void SMStateBase::DMReceiptPrintBarcode(CString  csBarcode,
   long     lBCType, 
   long     lBCHeight, 
   long     lBCAlign , 
   bool     fPrintNumbers, 
   int      nDeviceClass ) //Tar 430804
{
    dm.PrintBarcode(csBarcode, lBCType, lBCHeight, lBCAlign, fPrintNumbers, nDeviceClass);
}

SMStateBase *SMStateBase::DMHandleStartupError(void)
{
    // Don't process anything unless we have gotten a -3 from DM startup.
    if(m_nDevInitStatus != DMProcedures::DM_DEVICE_ERROR)
    {
       return STATE_NULL;
    }

    // RFC 445940:  This mess below was existing code.  Just moved it to a 
    //              dedicated method.  This code really needs to be refactored.

      CString csWork;
      CString sModel;
      CString sSubModel;
      bool bCkHealth = false;
      int DeviceClass = -1;

      int lExtCode = 0;
      int lRetCode = EVENTID_ANY;
      if (dm.m_nStartDeviceErrorDeviceClass > -1)  
      {
        // tar 277605//no ResultCodeExtended for DMCLASS_MISC and DMCLASS_MOTION_SENSOR
        if(    dm.m_nStartDeviceErrorDeviceClass == DMCLASS_MISC ) // tar 277605
        {
            //csWork = _T("DeviceErrorMisc.xml:NCR_TRILIGHT_ERROR");
            //SR672
            sModel = dm.GetDeviceModelName(DMCLASS_MISC, 0);
            lRetCode = NCR_TRILIGHT_ERROR;
            csWork = DMGetDeviceErrorHTML(DMCLASS_MISC, sModel, sSubModel, lRetCode, EXTENDED_RESULT_ANY, 0, NULL, &bCkHealth, 0, co.csPrimaryLanguageCode );
        }
        else if( dm.m_nStartDeviceErrorDeviceClass == DMCLASS_MOTION_SENSOR ) // tar 277605
        {
            //csWork = _T("DeviceErrorMotionSensor.xml:NCR_DIGITAL_INTERFACE_ERROR");
            //SR672
            sModel = dm.GetDeviceModelName(DMCLASS_MOTION_SENSOR, 0);
            lRetCode = NCR_DIGITAL_INTERFACE_ERROR;
            csWork = DMGetDeviceErrorHTML(DMCLASS_MOTION_SENSOR, sModel, sSubModel, lRetCode, EXTENDED_RESULT_ANY, 0, NULL, &bCkHealth, 0, co.csPrimaryLanguageCode );
        }
        else if(dm.m_nStartDeviceErrorDeviceClass == DMCLASS_CASHRECYCLER )
        {
            // Note that the input parameter to CashRecyclerGetErrorCode
            // is not used for the SafePay device.
            // The hard-coded arguments below are from existing Glory recycler
            // code.  0x80 specifies the Glory note recycler while 0x01
            // specifies the Glory coin recycler.
             long lBillErrorCode = dm.CashRecyclerGetErrorCode(0x80);
             CString csTempErrorCode;
             CString sSubModel;  //SR672
             if(lBillErrorCode)
              {
                    //Bill Hardware errors
                    csTempErrorCode.Format(_T("%d"),lBillErrorCode);

                    if(dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_GLORY) != -1) //SR672
                    {
                        csTempErrorCode = csTempErrorCode.Left(2); // xml file only use first 2 digits
                        /*
                          csWork = GetDeviceErrorMnemonic(dm.m_nStartDeviceErrorDeviceClass, 
                            _BILLRECYCLER, 0, _ttol(csTempErrorCode), 
                            nDeviceErrorContext ); */
                        sSubModel =  _T("Bill");
                        csWork = DMGetDeviceErrorHTML(dm.m_nStartDeviceErrorDeviceClass, dm.m_csCashRecyclerModel, sSubModel, 0, _ttol(csTempErrorCode), 0, NULL, &bCkHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
                    }
                    else if(dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_SAFEPAY) != -1)  //SR672
                    {
                        // SafePay - If an extended result code was read on init,
                        // then use it.
                        if(dm.m_nStartDeviceErrorResultCodeExtended)
                        {
                            lBillErrorCode = dm.m_nStartDeviceErrorResultCodeExtended;
                        }
                        /*
                          csWork = GetDeviceErrorMnemonic(dm.m_nStartDeviceErrorDeviceClass, 
                            dm.m_csCashRecyclerModel, 0, lBillErrorCode, 
                            nDeviceErrorContext );*/
                        sSubModel = _T("");
                        csWork = DMGetDeviceErrorHTML(dm.m_nStartDeviceErrorDeviceClass, dm.m_csCashRecyclerModel, sSubModel, 0, lBillErrorCode, 0, NULL, &bCkHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
                    }
                    m_nCashRecyclerHardwareErrorCode = lBillErrorCode;

              }
              else 
              {
                  long lCoinErrorCode = dm.CashRecyclerGetErrorCode(0x01);
                  if(lCoinErrorCode)
                  {
                    //Coin Hardware errors
                    csTempErrorCode.Format(_T("%d"),lCoinErrorCode);
                    csTempErrorCode = csTempErrorCode.Left(2); // xml file only use first 2 digits
                    //SR672
                      //csWork = GetDeviceErrorMnemonic(dm.m_nStartDeviceErrorDeviceClass, _COINRECYCLER, 0,  _ttol(csTempErrorCode), nDeviceErrorContext );
                    sSubModel =  _T("Coin");
                    csWork = DMGetDeviceErrorHTML(dm.m_nStartDeviceErrorDeviceClass, dm.m_csCashRecyclerModel, sSubModel, 0, _ttol(csTempErrorCode), 0, NULL, &bCkHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
                    m_nCashRecyclerHardwareErrorCode = lCoinErrorCode;
                  }
                  else
                  {
                    //csWork = GetDeviceErrorMnemonic(dm.m_nStartDeviceErrorDeviceClass, _T(""), CHAN_STATUS_JAM, 0, nDeviceErrorContext );
                      sSubModel = _T("");
                      csWork = DMGetDeviceErrorHTML(dm.m_nStartDeviceErrorDeviceClass, dm.m_csCashRecyclerModel, sSubModel, CHAN_STATUS_JAM, 0, 0, NULL, &bCkHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
                  }
              }
        
            DeviceClass = dm.m_nStartDeviceErrorDeviceClass; 
        }
        else
        {    
            //SR672
            //App starts, Cash Change Error or Scanner Error
            //csWork = GetDeviceErrorMnemonic(dm.m_nStartDeviceErrorDeviceClass, dm.GetDeviceName(dm.m_nStartDeviceErrorDeviceClass, 0), EVENTID_ANY, dm.m_nStartDeviceErrorResultCodeExtended, 0 );
            CString sSubModel = _T("");
            bCkHealth=false;
            CString sModel;
            sModel = dm.GetDeviceModelName(dm.m_nStartDeviceErrorDeviceClass, 0);
            lExtCode = dm.m_nStartDeviceErrorResultCodeExtended; //Tar 440771

            csWork = DMGetDeviceErrorHTML(dm.m_nStartDeviceErrorDeviceClass, sModel, sSubModel, EVENTID_ANY, lExtCode, 0, NULL, &bCkHealth, 0, co.csPrimaryLanguageCode );
            dm.m_nStartDeviceErrorResultCodeExtended = 0;  //Reset
            DeviceClass = dm.m_nStartDeviceErrorDeviceClass; 
        }
        
        //SR742 - Don't reset device error covered on this feature on startup while in degraded mode.
        if ( ! ( co.IsDegradedModeOn() 
            && ( IsCashDeviceError (dm.m_nStartDeviceErrorDeviceClass)  
            || IsCardDeviceError (dm.m_nStartDeviceErrorDeviceClass ) ) ) )
        {
            dm.m_nStartDeviceErrorDeviceClass = -1;
        }

      }
      //SSCOP-1103
      if (csWork.Find(_T("Ignore")) != -1)
      {
          trace(L6, _T("ERROR:  Invalid error at startup: %s"), (LPCTSTR)csWork);
          return STATE_NULL;
       }

      
      if(csWork.IsEmpty() || csWork.Find(_T("DeviceErrorUnknown")) != -1)  //Not handling by the extended result code
      {
        CString sParms;
         sParms = ps.GetPSText(MSG_DEVICESERROR); // code from below replaced here
         sParms += DMGetDevicesInError(); // code from below replaced here

         if(sParms.Find(_T("Signature Capture"))!=-1)  //It is sig cap device error
         {
            DeviceClass = DMCLASS_SIGCAP;
         }         
         else if(sParms.Find(_T("Scale"))!=-1)  //It is Scale device error
         {
            DeviceClass = DMCLASS_SCALE;
            TCHAR sScaleHTML2[_MAX_PATH];
            _tcscpy(sScaleHTML2, csWork.GetBuffer(_MAX_PATH));

            CString csScaleTitle = ps.GetPSText(MSG_SCANNER_SCALE_ERROR);  
            if(!UpdateDeviceErrorHTMLTitle(sScaleHTML2, csScaleTitle.GetBuffer(_MAX_PATH))) 
            {
                trace(L6,_T("UpdateDeviceErrorHTMLTitle() FAILED."));
            }
            csWork.ReleaseBuffer();
            csWork = sScaleHTML2;

         }
         else if(sParms.Find(_T("OnTrak"))!=-1)  //It is MISC device error
         {
            DeviceClass = DMCLASS_MISC;
         }
         else if(sParms.Find(_T("ZT1"))!=-1  || sParms.Find(_T("CashFlow"))!=-1)  //It is CASHACCEPTOR device error
         {
            DeviceClass = DMCLASS_CASHACCEPTOR;
         }
         else if(sParms.Find(_T("5945"))!=-1)  //It is MSR device error
         {
            DeviceClass = DMCLASS_MSR;
         }
         else if(sParms.Find(_T("Journal"))!=-1)  //It is JOURNAL device error
         {
            DeviceClass = DMCLASS_JOURNAL;
         }

         //SR672
        if (!csWork.IsEmpty() && !sParms.IsEmpty())
        {
            TCHAR sHTML2[_MAX_PATH];
            _tcscpy(sHTML2, csWork.GetBuffer(_MAX_PATH));

            //SSCOB-517
            if(dm.GetDevicesInErrorCountAtStartup() > 1 )
            {
                CString csTitle = ps.GetPSText(TXT_SYSTEMMESSAGE);  
                if(!UpdateDeviceErrorHTMLTitle(sHTML2, csTitle.GetBuffer(_MAX_PATH))) 
                {
                    trace(L6,_T("UpdateDeviceErrorHTMLTitle() FAILED."));
                }
                
                CString csSubTitle = ps.GetPSText(LTT_DEVICESINERROR); 
                if(!UpdateDeviceErrorHTMLSubTitle(sHTML2, csSubTitle.GetBuffer(_MAX_PATH)))
                {
                    trace(L6,_T("DeviceErrorHTMLSubTitle() FAILED."));
                }
            }

            UpdateDeviceErrorHTMLText(sHTML2, sParms.GetBuffer(_MAX_PATH));
            csWork.ReleaseBuffer();
            csWork = sHTML2;
            sParms.ReleaseBuffer();
        }

      }
      
      return createLevel1Message(csWork,      // char *message
                               PS_NOTEXT,     // no canned message
                               PS_NOTEXT,     // no prompt
                               0,             // no prompt value
                               DeviceClass,   // -1 = no device
                               bCkHealth,     // whether DM check health needed
                               true);  //display HTML //SR672
}

#endif //_CPPUNIT
///////////////////////////////////////////////
SMStateBase *SMStateBase::DMSigcapStarted()
{
    // Base functionality does nothing
  return STATE_NULL;
}

///////////////////////////////////////////////
SMStateBase *SMStateBase::DMSigcapEnded()
{
    // Base functionality does nothing
  return STATE_NULL;
}

///////////////////////////////////////////////
SMStateBase* SMStateBase::DMTakeawayReadyForItem()
{
    g_bReadyForItem = true; //TAR 363023
    // Base functionality does nothing
    return STATE_NULL;
}

///////////////////////////////////////////////
SMStateBase* SMStateBase::DMTakeawayBagAreaBackup(bool isBackedUp)
{
#ifndef _CPPUNIT
    // Set global backup flag, and remain in current state.
    dm.fTakeawayBeltBackup = isBackedUp;

#endif //_CPPUNIT
    return STATE_NULL;
}

///////////////////////////////////////////////
SMStateBase* SMStateBase::DMTakeawaySafetyCoverOpen(bool isOpen)
{
#ifndef _CPPUNIT
    // Base functionality does nothing
    //return STATE_NULL;
    
    //TAR197594+
    if (isOpen)  //cover open
    {
        trace(L6,_T("+DMTakeawaySafetyCoverOpen: true"));       
        if (!fTABSafetyCoverOpenApprovalPending)  // for // TAR 201268 TAR 202442 TAR 201239
        {
            //Tell RAP about this device error
            CString csSecInfo, csErrorMessage;
            csErrorMessage = ps.GetPSText(RA_MSG_SAFETYCOVEROPEN, 
                                      SCOT_LANGUAGE_PRIMARY);
            trace(L7, _T("+ra.OnRAPDataNeeded, ErrorMessage=%s, DeviceClass=%d"), 
                  csErrorMessage, DMCLASS_TAKEAWAYBELT);
            // write the error description on the receipt
            ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csErrorMessage);    
            // send a DataNeeded Approval
            cDataEntry.FromConfigData(co.csDataEntryINIFile, "DeviceFailure");
            // save the previous view
            m_PrevView = m_View;
            m_View = DATANEEDED_DEVICEERROR;
            csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), 
                             csErrorMessage,
                             ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), 
                             "", "", "", "");
            ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
            trace(L7, _T("-ra.OnRAPDataNeeded "));

            fTABSafetyCoverOpenApprovalPending = true;
        

            trace(L6,_T("-DMTakeawaySafetyCoverOpen"));

            DMTakeawayBeltNotify(tabEventAppRequestStopBelt); //TAR:203166: Tell TAB to stop belts

            //Go to systemmessage4
            CREATE_AND_DISPENSE(SystemMessage4)(NULL,      // char *message
                                                RA_MSG_SAFETYCOVEROPEN,  //"Safety Cover Open"
                                                PS_NOTEXT,  // no prompt
                                                0,                  // no value
                                                DMCLASS_TAKEAWAYBELT, // DM device
                                                true);     // DM check health after
        }
        else // already approval sent, don't do anything else
        {
            return STATE_NULL;
        }


    }
    else        //cover closed 
    {
        trace(L6,_T("+DMTakeawaySafetyCoverOpen: false"));
        trace(L6,_T("-DMTakeawaySafetyCoverOpen"));
        return STATE_NULL;
    }
    //TAR197594-
#else
    return STATE_NULL;
#endif //_CPPUNIT
}
#ifndef _CPPUNIT

///////////////////////////////////////////////
/* Moved to SmStateBase
SMStateBase* SMStateBase::DMTakeawaySendScaleStatus()
{
    // Base functionality does nothing
    return STATE_NULL;
}
*/

//////////////////////////////////////////////////   TINA
void SMStateBase::BillOrCoinErrorStatus(bool *pCoin, bool* pBill)
{
  long coin = 0;
  long bill = 0;

  trace(L6, _T("+BillOrCoinErrorStatus"));
  // Query DM for status.
  dm.CashChangerGetStatusValues(&coin, &bill);

  trace(L6, _T("Coin status is %d"), coin);
  trace(L6, _T("Bill status is %d"), bill);
  *pCoin = true;
  *pBill = true;
    //SR672+
    
  //Tar 450295
   if ( (bill == CASH_STATUS_NEAREMPTY ||
                  bill == CASH_STATUS_EMPTYOK   ||
                  bill == CASH_STATUS_EMPTY   ||
                  bill == CASH_STATUS_FULL ||
                  bill == CASH_STATUS_NEARFULL || 
                  bill == CASH_STATUS_JAMOK ||
                  bill == CASH_STATUS_TAMPER ))
      *pBill = false; 

   if ( (coin == CASH_STATUS_NEAREMPTY ||
                  coin == CASH_STATUS_EMPTY ||
                  coin == CASH_STATUS_EMPTYOK   ||
                  coin == CASH_STATUS_FULL ||
                  coin == CASH_STATUS_NEARFULL ||
                  coin == CASH_STATUS_JAMOK ||
                  coin == CASH_STATUS_TAMPER ))
      *pCoin = false; 
    //SR672-
  trace(L6, _T("Coin error is %d"), *pCoin);
  trace(L6, _T("Bill error is %d"), *pBill);
  trace(L6, _T("-BillOrCoinErrorStatus"));
}

//////////////////////////////////////////////////   TINA
CString SMStateBase::CashChangerErrorMessage(long lChangeDue, CString addMsg)
{
  // + TAR  296345 - use new style & the same error message for both coin & bill
  CString csWork;
  //SR672
  
  CString sSubModel;

  bool coin = false;
  bool bill = false;
  bgCoinDispenserIsEmpty = true;
  //+-SSCOADK-6319
  long lErrorCode = -99;

  BillOrCoinErrorStatus(&coin, &bill);

  if (coin)
            sSubModel = _T("Coin");
  if (bill)
            sSubModel = _T("Bill");

  //+SSCOADK-6319
  if (dm.m_csCashRecyclerModel.IsEmpty())
  {
    lErrorCode = dm.GetResultCodeExtended(DMCLASS_CASHCHANGER, 0);  //SSCOP-1971 & SSCOP-2531 
  }
  //-SSCOADK-6319

  CString s1=dm.GetDeviceModelName(DMCLASS_CASHCHANGER, 0, sSubModel); //SSCOP-1971 & SSCOP-2531 

  bool fCheckHealth = false;
  csWork = DMGetDeviceErrorHTML(DMCLASS_CASHCHANGER, s1, sSubModel, EVENTID_ANY, lErrorCode, 0, NULL, &fCheckHealth, CONTEXT_ANY, co.csPrimaryLanguageCode );
                     

if (csWork.Find(_T("DeviceErrorUnknown")) != -1 || csWork.Find(_T("Ignore")) !=-1)
{ 
   //+RFC 332822
  if(lChangeDue < 0)
  {
      //csWork=  _T("DeviceErrorCashChanger.xml:NCR_BOTH_OVERDISPENSE"); 
      lErrorCode = NCR_BOTH_OVERDISPENSE;
  }
  //SR672
  dm.m_csCoinDispenserModel.MakeUpper();
  //-RFC 332822
    if(co.fLocaleCashManagementScreen)
    {
        //csWork=  _T("DeviceErrorCashChanger.xml:NCR_BOTH_CANNOT_DISPENSE_CASH_MANAGEMENT"); 
        lErrorCode = NCR_BOTH_CANNOT_DISPENSE_CASH_MANAGEMENT;
    }
    else if(dm.m_csCoinDispenserModel.Find(_T("HOPPER")) != -1 && !dm.m_csCoinDispenserStage.IsEmpty() )
    {
        // csWork=  _T("DeviceErrorCashChanger.xml:NCR_BOTH_CANNOT_DISPENSE_HOPPER");
        lErrorCode = NCR_BOTH_CANNOT_DISPENSE_HOPPER;
    }
    else
    {
        // csWork=  _T("DeviceErrorCashChanger.xml:NCR_BOTH_CANNOT_DISPENSE"); 
        lErrorCode = NCR_BOTH_CANNOT_DISPENSE;
    }

  csWork = DMGetDeviceErrorHTML(DMCLASS_CASHCHANGER, s1, sSubModel, lErrorCode, EXTENDED_RESULT_ANY, 0, NULL, &fCheckHealth, CONTEXT_ANY, co.csPrimaryLanguageCode );
}
  
  if(dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_SAFEPAY) != -1)
  {
      lErrorCode = NCR_BOTH_CANNOT_DISPENSE;
      csWork = DMGetDeviceErrorHTML(DMCLASS_CASHCHANGER, s1, sSubModel, lErrorCode, EXTENDED_RESULT_ANY, 0, NULL, &fCheckHealth, CONTEXT_ANY, co.csPrimaryLanguageCode );
      return csWork;

  }


  BillOrCoinErrorStatus(&coin, &bill);

  if ((coin && !bill) || (bill && !coin))
  {
    if (bill)
    {
      bgCoinDispenserIsEmpty = false;
    }
  }

  // - TAR 296345

  // This message is real long indicating status of all
  // currency values so it is not appropriate to show it at this time.

  /*addMsg.TrimLeft();
   if (addMsg != "")
   {
    csWork += "\n";
    csWork += addMsg;
   }*/

  return csWork;
}

///////////////////////////////////////////////
SMStateBase *SMStateBase::DMSigcapFailure()
{ // TAR182614  LPM, THP, and AB 101101
  if ( isAnySoldVoidedItems() ) // if the sig capture failure event occurs make sure there are items purchased (or voided)  
  {
    BSTR pData= NULL;
    //When SigCap device is failure, open unavailablesig.dat file.  
    // Read the file, convert the string in the file to BSTR.
    HANDLE  m_hFile;
    CString csName;

    csName.Format(co.csConfigFilePath + _T("\\") UNAVAILABLESIGBMP);
    m_hFile = ::CreateFile( csName,                 // pointer to name of the file
                            GENERIC_READ,           // access read mode
                            0,                      // share mode
                            NULL,                   // pointer to security attributes
                            OPEN_EXISTING,          // how to create
                            FILE_ATTRIBUTE_NORMAL,  // file attributes
                            NULL    );              // handle to file with attributes to copy

    BOOL bResult;
    CString csSigData;
    unsigned long lDataLength , lBytesRead;

    bResult = ::ReadFile(m_hFile, &lDataLength , sizeof(lDataLength), &lBytesRead, NULL) ; 
    bResult = ::ReadFile(m_hFile, csSigData.GetBuffer(lDataLength) , lDataLength, &lBytesRead, NULL) ;
    csSigData.ReleaseBuffer(lDataLength) ;

#ifdef _UNICODE
    pData = SysAllocStringLen( csSigData, lDataLength);
#else
    pData = SysAllocStringByteLen(csSigData, lDataLength);
#endif


    ::CloseHandle(m_hFile) ;
    m_hFile = INVALID_HANDLE_VALUE ;

    CString csFailureMsg = " Signature Capture failure!!!";
    trace(L6, csFailureMsg);

    //Send signature data to TB
    if(co.fOperationsSendSignatureDataToTB)
        TBSendSigData(CustomerName(cDMLastTrack),(LPCTSTR) pData,(DWORD)(SysStringByteLen(pData)));

    //ra.OnNormalItem("[highlight]" + csFailureMsg );   // maybe do later
    trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_TENDER_COMPLETE, SCOT_LANGUAGE_PRIMARY));
    ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + 
                        ps.GetPSText(RA_TENDER_COMPLETE, SCOT_LANGUAGE_PRIMARY));
    trace(L7, _T("-ra.OnNormalItem"));

    EndOfTransactionPrint(); //SR697

    // +TAR 390646
    if (co.m_bOperationsPrintSignatureReceipt)
    {
        SaveSigReceiptData(pData);
    }
    // -TAR 390646

    SysFreeString(pData);
    CREATE_AND_DISPENSE(TakeCard)(true) ; //TAR#132072 -- Proceed SigCap after tendering
  }
  return STATE_NULL;
}


SMStateBase *SMStateBase::DMSlipPrinter(DWORD dwEvent)
   {
   trace (L6, _T("+SMState::DMSlipPrinter(0x%08.8x <%s>)"), dwEvent, dm.GetEventDescription(dwEvent));
   trace (L6, _T("-SMState::DMSlipPrinter()"));
   return STATE_NULL;
   }

SMStateBase *SMStateBase::DMMICR(DWORD dwEvent)
   {
   trace (L6, _T("+SMState::DMMICR(0x%08.8x <%s>)"), dwEvent, dm.GetEventDescription(dwEvent));
   trace (L6, _T("-SMState::DMMICR()"));
   return STATE_NULL;
   }

// this function is called by DMParse() in remote mode
// to allow current screen object to handle any receipt-printer events
// except DM_PTR_REQUEST_COMPLETED event
// lDMLastReceiptCode has to be set to dwEvent in order to not invoke
// default handler in SMStateBase::DMParse()
SMStateBase *SMStateBase::DMReceiptPrinter(DWORD dwEvent)
   {
   trace (L6, _T("+SMState::DMReceiptPrinter(0x%08.8x <%s>)"), dwEvent, dm.GetEventDescription(dwEvent));
   trace (L6, _T("-SMState::DMReceiptPrinter()"));
   return STATE_NULL;
   }

// this function is called by DMParse() in remote mode
// to allow current screen object to handle any journal-printer events
// except DM_PTR_REQUEST_COMPLETED event
// lDMLastReceiptCode has to be set to dwEvent in order to not invoke
// default handler in SMStateBase::DMParse()
SMStateBase *SMStateBase::DMJournalPrinter(DWORD dwEvent)
   {
   trace (L6, _T("+SMState::DMJournalPrinter(0x%08.8x <%s>)"), dwEvent, dm.GetEventDescription(dwEvent));
   trace (L6, _T("-SMState::DMJournalPrinter()"));
   return STATE_NULL;
   }

SMStateBase *SMStateBase::DMCashDrawer(DWORD dwEvent)
   {
   trace (L6, _T("+SMState::DMCashDrawer(0x%08.8x)"), dwEvent);
   tb.SetCashDrawerState(DM_DRAWER_OPEN  == dwEvent ? 1 : 0);
   
   if(DM_DRAWER_OPEN == dwEvent)  //RAP Dispense Change  
   {
       m_bIsCashDrawerClosed = false;  //True - Closed, False - Open, default is True
   }
   else
   {
       m_bIsCashDrawerClosed = true;
   }

   trace (L6, _T("-SMState::DMCashDrawer()"));
    return STATE_NULL;
   }

SMStateBase *SMStateBase::DMKeyLock(DWORD dwEvent)
   {
   trace(L6,_T("+SMState::DMKeylock(0x%08.8x <%s>)"), dwEvent, dm.GetEventDescription(dwEvent));

   long nExpectedLevel;
   long nCurrentLevel; 
   long nNewLevel =       
      dwEvent == DM_KEYLOCK_L ? 1 :
      dwEvent == DM_KEYLOCK_N ? 2 :
      dwEvent == DM_KEYLOCK_S ? 3 :
      dwEvent == DM_KEYLOCK_X ? 4 : 0;

   tb.GetExpectedSecurityLevel(&nExpectedLevel);
   tb.GetCurrentSecurityLevel(&nCurrentLevel);

   trace(L6,_T("SMState::DMKeylock() TBExpectedLevel=%d TBCurrentLevel=%d NewLevel=%d"), 
   nExpectedLevel, nCurrentLevel, nNewLevel);
   tb.SetSecurityLevel(nNewLevel);

   trace(L6,_T("-SMState::DMKeylock()"));
   return STATE_NULL;
   }

void SMStateBase::PrintEndorsementChit(LPCTSTR szEndorsement) const
   {
   trace (L6, _T("+SMState::PrintEndorsementChit()"));
   CString sChitHeader;
   sChitHeader = ps.GetPSText(PRN_ENDORSEMENT_CHIT_HEADER);
   int iFind = sChitHeader.Find(_T("\\n"));
   while (iFind != -1)
      {
      sChitHeader.SetAt(iFind,'\r');
      sChitHeader.SetAt(iFind + 1, '\n');
      iFind = sChitHeader.Find(_T("\\n"));
      }
  CString csData;
#ifdef _UNICODE   //SSCOP-137 (migrate fix of TAR459459)
  csData=sChitHeader;
  sChitHeader=ConvertPrinterString(csData);
#endif
   dm.ReceiptLine(sChitHeader);
#ifdef _UNICODE   //SSCOP-137 (migrate fix of TAR459459)
  csData=szEndorsement;
  csData=ConvertPrinterString(csData);
#endif
   dm.ReceiptLine(csData);
   dm.ReceiptCut();
   trace (L6, _T("-SMState::PrintEndorsementChit()"));
   }


bool SMStateBase::InRemoteDeviceMode(void) const
   {
   return (
      dm.IsDeviceRemote(DMCLASS_MICR, 0) &&
      dm.IsDeviceRemote(DMCLASS_KEYLOCK, 0) &&
      dm.IsDeviceRemote(DMCLASS_CASHDRAWER, 0) &&
      dm.IsDeviceRemote(DMCLASS_RECEIPT, 0) &&
      dm.IsDeviceRemote(DMCLASS_SLIP, 0));
   }


///////////////////////////////////////////////
// Handler function for Items found on 2nd (Take Away) belt which failed
// to be deactivated
SMStateBase *SMStateBase::OnUnDeActivatedTheftTag(void)
{
    trace(L7, _T("+SMStateBase::OnUnDeActivatedTheftTag"));
    // in store mode ignore it
    if (storeMode() || fInSuspendTrans || fInVoidTrans) 
    {
        return STATE_NULL;
    }

    if  (bIsInFinalization) //in one of the Finalization state
    {
        if (!fBeyondFinishAndPayState) //if we have NOT entered tendering by selecting 
                                // a payment type
        {
             DMTakeawayBeltNotify(tabEventAppRequestStopBelt);  // TAR244694
             CREATE_AND_DISPENSE(UnDeActivatedItemApproval)(getAnchorState(), getAnchorState());
        }
        // else simply ignore the event.
    }
    else 
    {
        if (co.csOperationsHandleUnDeActivatedItemAs == "I")
        {
            DMTakeawayBeltNotify(tabEventAppRequestStopBelt);   // TAR244694
            CREATE_AND_DISPENSE(UnDeActivatedItemApproval)(getAnchorState(), getAnchorState());
        }
        else if (co.csOperationsHandleUnDeActivatedItemAs == "D")
        {
            CString sDescription;
            if (!fUnDeActivatedItem) // this is the 1st Undeactivated item
            {  
                 //Make light yellow 
                 
                 // (*) SR898 ====================
                 DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                                    DM_TRICOLORLIGHT_ON,
                                    DM_TRICOLORLIGHT_NORMALREQUEST);
                                    
                 sDescription = ps.GetPSText(RA_TC_UNDEACTIVATEDITEM, SCOT_LANGUAGE_PRIMARY);
                 //Send a delayed approval to RAP for EAS item active.
                 CString csAnchorStateName = getSCOTStateName();//Tar 247393
                 CString csInstruction;
                 csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), 
                                      sDescription, 
                                      ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY),//Lane # Button Text
                                      csAnchorStateName, co.IsSecurityDisableOn()==true || co.GetSecurityDisableSubCheckOn() ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), 
                                      ps.GetPSText(RA_THIRDLINE_UNDEACTIVATED, SCOT_LANGUAGE_PRIMARY)
                                     );
                 trace(L7, _T("+ra.OnUnDeActivatedItem, Message=%s"), csInstruction);           
                 ra.OnUnDeActivatedItem(csInstruction);
                 trace(L7, _T("-ra.OnUnDeActivatedItem"));
                 //Update RAP receipt with EAS item refernce line message
                 sDescription = ps.GetPSText(RA_RECEIPT_MSG_EASITEM_REFERENCE, SCOT_LANGUAGE_PRIMARY);
                 ra.OnNormalItem(RAP_RECEIPT_LINE_HIGHLIGHT + sDescription);
                 fUnDeActivatedItem = true;
                 fNewItemScannedSinceLastUnDeActivatedItem = false;
            }
            else 
            {
                //if (an item has been scanned after the previous EAS item active event)
                // else the event is for the same active item stuck on roller, so 
                // ignore it.       
                sDescription = ps.GetPSText(RA_RECEIPT_MSG_EASITEM_REFERENCE, SCOT_LANGUAGE_PRIMARY);
                if (fNewItemScannedSinceLastUnDeActivatedItem)
                {
                    ra.OnNormalItem(RAP_RECEIPT_LINE_HIGHLIGHT + sDescription);
                    fNewItemScannedSinceLastUnDeActivatedItem = false;
                }
            } // end else 
        } // end else if (co.csOperationsHandleUnDeActivatedItemAs == "D")
    } // end outer else
    trace(L7, _T("-SMStateBase::OnUnDeActivatedTheftTag"));
    return STATE_NULL;

}
SMStateBase *SMStateBase::DMCashChangerError(int nDMLastEvent)
{
    // Error condition of we didn't give all change due...
    // Need to distinguish coin or bill error.   TINA
    CString csWork = CashChangerErrorMessage(nDMLastEvent, "");
    trace(L6, _T("cash changer error: %s"), csWork);
    // this will let systemmessage4::uninitialize rerun this function to dispense after changer refilled
    fDMErrorCashDue = true;

    //Notify RAP
    //+SSCOADK-836
    trace(L6, _T("+ra.OnRAPDataNeeded, ErrorMessage=%s"), csWork);
    CString csTmp;
    csTmp = ps.GetPSText(LTT_NCR_E5633_FEED_FAILURE, SCOT_LANGUAGE_PRIMARY);
	if(csTmp.Find(_T("%s")))
		csTmp.Format(csTmp, ps.FmtDollar(nDMLastEvent));
    ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csTmp);
    //-SSCOADK-836
    // send a DataNeeded Approval
    cDataEntry.FromConfigData(co.csDataEntryINIFile, "DeviceFailure");
    m_View = DATANEEDED_DEVICEERROR;
    CString csSecInfo;
    csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),csTmp,ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), "", "", "", ""); //SSCOADK-836
    ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
    trace(L6, _T("-ra.OnRAPDataNeeded")); //SSCOADK-836

    //Go to systemmessage4
    CREATE_AND_DISPENSE(SystemMessage4)(csWork,      // char *message
                            PS_NOTEXT,  // no canned message
                            PS_NOTEXT,  // no prompt
                            0,          // no value
                            DMCLASS_CASHCHANGER, // DM device
                            true);     // DM check health after
}

//awk.11.12.2 Begin Moved methods. Moved from MediaStatusBase
//SR672
void SMStateBase::getPrinterStatusLine(const long lCode, CString& csPrinterStatus)
{
  CString csWork;
        switch ( lCode )
      {
        case PTR_SUE_COVER_OK:
        case PTR_SUE_JRN_PAPEROK:
        case PTR_SUE_REC_PAPEROK:
        case PTR_SUE_SLP_PAPEROK:
        case DM_PTR_REQUEST_COMPLETED:
            csWork.Format(MSGFMT,ps.GetPSText(TXT_NORMAL),lCode);
            break;
        case PTR_SUE_COVER_OPEN:
            csWork.Format(MSGFMT, ps.GetPSText(MSG_PTR_COVER_OPEN), lCode);
            break;
        case PTR_SUE_JRN_EMPTY:
        case PTR_SUE_REC_EMPTY:
        case PTR_SUE_SLP_EMPTY:
            csWork.Format(MSGFMT, ps.GetPSText(MSG_PTR_EMPTY), lCode);
            break;
        case PTR_SUE_JRN_NEAREMPTY:
        case PTR_SUE_REC_NEAREMPTY:
        case PTR_SUE_SLP_NEAREMPTY:
            csWork.Format(MSGFMT, ps.GetPSText(MSG_PTR_NEAREMPTY), lCode);
            break;
        case OPOS_EPTR_COVER_OPEN:
            csWork.Format(MSGFMT, ps.GetPSText(MSG_PTR_ERR_COVER_OPEN), lCode);
            break;
        case OPOS_EPTR_JRN_EMPTY:
        case OPOS_EPTR_REC_EMPTY:
        case OPOS_EPTR_SLP_EMPTY:
            csWork.Format(MSGFMT, ps.GetPSText(MSG_PTR_ERR_EMPTY), lCode);
            break;
            
        case OPOS_E_TIMEOUT:
        case OPOS_E_OFFLINE:
        case OPOS_E_BUSY:
            csWork.Format(MSGFMT, ps.GetPSText(MSG_PTR_ERR_TIME_OUT), lCode);
            break;
      default:
        csWork.Format(MSGFMT,ps.GetPSText(TXT_UNKNOWN),lCode);
        break;
      }

    
    csPrinterStatus = csWork;
}

void SMStateBase::getCashAcceptorStatusLine(const long lCode, CString& csCashAccepStatus)
{
  
  CString csWork;

  if (co.fTenderCashAllowed)
  {
    switch ( lCode )
    {
    
      //SR672
      case BILL_STATUS_OK:
      case BILL_STATUS_RESET:   //Tar 223001
      case BILL_STATUS_INSERTED:
      case BILL_STATUS_REMOVED:
      case BILL_STATUS_ASYNC:
      //case OPOS_SUCCESS:
      case BILL_STATUS_TAMPER_IN:
          csWork.Format(MSGFMT,ps.GetPSText(TXT_CASSETTE_NORMAL, SCOT_LANGUAGE_PRIMARY),lCode);
          break;
      case BILL_STATUS_TAMPER_OUT:
          csWork.Format(MSGFMT,ps.GetPSText(TXT_CASSETTE_OUT, SCOT_LANGUAGE_PRIMARY),lCode);
          break;
      case OPOS_E_FAILURE:
          csWork.Format(MSGFMT,ps.GetPSText(MSG_ERROR_GENERALFAILURE, SCOT_LANGUAGE_PRIMARY),lCode);
          break;
      CASE_MSGBILL(STATUS_JAM);
      CASE_MSGBILL(STATUS_REJECTED);
      CASE_MSGBILL(STATUS_FAILURE);
      CASE_MSGBILL(STATUS_FULL);
      CASE_MSGBILL(STATUS_NEARFULL);
      CASE_MSGBILL(STATUS_CHEAT);
      //CASE_MSG2(ERROR_UNEXPECTED);
      //CASE_MSG2(ERROR_GENERALFAILURE);
      //CASE_MSG2(STATUS_UNEXPECTED);

    default:
      csWork.Format(MSGFMT,ps.GetPSText(TXT_UNKNOWN, SCOT_LANGUAGE_PRIMARY),lCode);
      break;
    }
  }
  else
  {
    csWork.Format(MSGFMT,ps.GetPSText(TXT_NOT_PRESENT, SCOT_LANGUAGE_PRIMARY),0);
  }
    
  csCashAccepStatus = ps.GetPSText(TXT_CASH_ACCEPTOR, SCOT_LANGUAGE_PRIMARY) + ": " + csWork;
}

void SMStateBase::getCoinAcceptorStatusLine(const long lCode, CString& csCoinAccepStatus)
{
  //------------------------------------------------------
  // Coin Acceptor Status
  //------------------------------------------------------

  CString csWork;

  if (co.fTenderCashAllowed) // check if coin acceptor installed
  {
    switch ( lCode )
    {
      case COIN_STATUS_OK:
      case COIN_STATUS_ASYNC:
      //case OPOS_SUCCESS:
      case COIN_STATUS_TAMPER_IN:
          csWork.Format(MSGFMT,ps.GetPSText(TXT_CASSETTE_NORMAL, SCOT_LANGUAGE_PRIMARY),lCode);
          break;
      case COIN_STATUS_TAMPER_OUT:
      case OPOS_ECOIN_TAMPER_OUT:
          csWork.Format(MSGFMT,ps.GetPSText(TXT_CASSETTE_OUT, SCOT_LANGUAGE_PRIMARY),lCode);
          break;
      case OPOS_ECOIN_CHEAT:
          csWork.Format(MSGFMT,ps.GetPSText(MSG_COINACCEPTOR_STATUS_CHEAT, SCOT_LANGUAGE_PRIMARY),lCode);
          break;
      CASE_MSGCOIN(STATUS_JAM);
      CASE_MSGCOIN(STATUS_REJECTED);
      CASE_MSGCOIN(STATUS_RESET);
      CASE_MSGCOIN(STATUS_FAILURE);
      CASE_MSGCOIN(STATUS_FULL);
      CASE_MSGCOIN(STATUS_NEARFULL);
      //CASE_MSGCOIN(STATUS_CHEAT);
      CASE_MSGCOIN(STATUS_RETURNED);
      CASE_MSGCOIN(STATUS_CLOSED);
      CASE_MSGCOIN(STATUS_INOPERABLE);
      CASE_MSGCOIN(STATUS_ESCROW_FULL);

      //CASE_MSG(ERROR_UNEXPECTED);
      //CASE_MSG(ERROR_GENERALFAILURE);
      //CASE_MSG(STATUS_UNEXPECTED);

      default:
      csWork.Format(MSGFMT,ps.GetPSText(TXT_UNKNOWN, SCOT_LANGUAGE_PRIMARY),lCode);
      break;
    }
  }
  else
  {
      csWork.Format(MSGFMT,ps.GetPSText(TXT_NOT_PRESENT, SCOT_LANGUAGE_PRIMARY),0);
  }
    
  csCoinAccepStatus = ps.GetPSText(TXT_COIN_ACCEPTOR, SCOT_LANGUAGE_PRIMARY) + ": " + csWork;
}
//awk.11.12.2 end moved methods for rap media status.


void SMStateBase::ProcessPrinterExtendedResultCode(long &ExtendedResultCode)
{
    if (ExtendedResultCode & PTRSTAT_FLAG)   // NCR supported status
        ExtendedResultCode &= 0x7FFFFFFF;
    return;
}



void SMStateBase::ResetMLBCValues(void)
{
    if(m_nNumberOfPartsReturnedSoFar)
    {
        if(m_psMLBarCodeDetails)
        {
            for(int i=0;i<m_nNumberOfPartsReturnedSoFar;i++)
            {
                if(m_psMLBarCodeDetails[i].sBarCode)
                    delete [] m_psMLBarCodeDetails[i].sBarCode;
            }
            delete [] m_psMLBarCodeDetails;
            m_psMLBarCodeDetails = NULL;//RFC 241710
        }
        m_nNumberOfPartsReturnedSoFar = 0;
    }
    m_bLastScanDataWasMultiLabel = FALSE;//RFC 241710
}


//add record to list in barcode position order
void SMStateBase::AddMLBCRecord(MultiLabelBarcodeFormatRecord *pMLBFRecord)
{
    POSITION pos = m_MLBarcodeList.GetHeadPosition();
    if(pos==NULL)
    {
        m_MLBarcodeList.AddHead(*pMLBFRecord);
    }
    else
    {
        do
        {
            if(pMLBFRecord->csDMData == m_MLBarcodeList.GetAt(pos).csDMData ||  //Check that same label is not scanned twice 
               pMLBFRecord->nIndex != m_MLBarcodeList.GetAt(pos).nIndex )   //Check that different multi-label is not scanned
            {
                m_bLastScanDataWasMultiLabel = FALSE; // will cause app to ignore last scan and throw away all saved labels
                return;
            } 
            if(pMLBFRecord->nPosition < m_MLBarcodeList.GetAt(pos).nPosition)
            {
                m_MLBarcodeList.InsertBefore(pos, *pMLBFRecord);
                break;
            }
            m_MLBarcodeList.GetNext(pos);
        } while(pos);
        if(pos==NULL)
        {
            m_MLBarcodeList.AddTail(*pMLBFRecord);
        }
    }
    m_bLastMntScanDataWasMultiLabel = TRUE;
    m_bLastScanDataWasMultiLabel = TRUE;
    m_nNumberOfPartsReturnedSoFar++;
}

CString SMStateBase::GetItemCodeFromMLBC(void)
{
    CString csCode;
    int nLength = 0;
    int nCount=0;
 
    m_MntMLBarcodeArray.RemoveAll();
    csCode.Empty();
    m_psMLBarCodeDetails = new SMultiLabelBarCodeDetails[m_nTotalNumberOfMLBParts];
    
    POSITION pos = m_MLBarcodeList.GetHeadPosition();
    //++TAR 266075 -- Add option to prepend item code with prefix of first label
    if(pos!=NULL && co.fMultiLabelBarCodesPrependFirstPrefixToItemCode) 
    {
        csCode = m_MLBarcodeList.GetAt(pos).csPrefix;
    }
    //--TAR 266075
    while(pos!=NULL)
    {
        m_MntMLBarcodeArray.Add(m_MLBarcodeList.GetAt(pos).csLabel);
        nLength = m_MLBarcodeList.GetAt(pos).csBarcode.GetLength();
        m_psMLBarCodeDetails[nCount].sBarCode = new _TCHAR[nLength + 1];
        memcpy((m_psMLBarCodeDetails[nCount].sBarCode), m_MLBarcodeList.GetAt(pos).csBarcode, nLength * sizeof(_TCHAR));
        m_psMLBarCodeDetails[nCount].sBarCode[nLength] = _T('\0');
        m_psMLBarCodeDetails[nCount].lBarCodeType = m_MLBarcodeList.GetAt(pos).nSymbology;
        
        if(m_MLBarcodeList.GetAt(pos).nItemCodePosition != -1) //contains at least part of item code
        {
            csCode += m_MLBarcodeList.GetAt(pos).csLabel.Mid(m_MLBarcodeList.GetAt(pos).nItemCodePosition,m_MLBarcodeList.GetAt(pos).nItemCodeLength);
            io.cScanType = csDMLastBarType = m_MLBarcodeList.GetAt(pos).nSymbology;
        }
        
        m_MLBarcodeList.GetNext(pos);
        ++nCount;
    }
    m_bLastScanDataWasMultiLabel = FALSE;
    return csCode;
}

#endif // _CPPUNIT
//Check if Coin Dispenser has a enhanced low sensor
int SMStateBase::DMCheckForCoinDispenserLowSensor(void)
{
#ifndef _CPPUNIT
   CString csCoinDesc;
   if ( dm.IsDeviceAvailable(DMCLASS_CASHCHANGER) )
   {
       csCoinDesc = dm.GetDescription(DMCLASS_CASHCHANGER,0);
   }
   trace(L6,_T("GetDeviceDescription = %s"),csCoinDesc);

   if(csCoinDesc.Find(_T("NOTCLAIM")) != -1) // SO is not ready for
                                              // device description call
   {
      m_nCoinDispenserWithLowSensor = 2;   //tar 246539
   }
    //Device Description contains "CoinMediaLowSensor" text if Low Sensor is available.
    else if(csCoinDesc.Find(_T("CoinMediaLowSensor"))!=-1)
    {
        m_nCoinDispenserWithLowSensor = 1;
        co.nCashValueList.RemoveAll();
        co.nCashLowList.RemoveAll();
    //+dp185016 support glory device. just add parameters.
    co.LoadCashValueLists(co.csLocaleCashValueList, co.csLocaleCashLowListForNewEnhancedCoinLowSensor,
//      _T(""), _T(""), _T(""));   //-dp185016
      co.csLocaleCashHighList, co.csLocaleCashCapacity, co.csLocaleCashRemoveAmount ); //refresh all options
        trace(L6,_T("SMStateBase::DMCheckForCoinDispenserLowSensor(), use CashLowListForNewEnhancedCoinLowSensor = %s"),co.csLocaleCashLowListForNewEnhancedCoinLowSensor);

    }
    else
    {
       m_nCoinDispenserWithLowSensor = 0;
    }
   trace(L6,_T("SMStateBase::DMCheckForCoinDispenserLowSensor = %d"),m_nCoinDispenserWithLowSensor);

#endif // _CPPUNIT
   return  m_nCoinDispenserWithLowSensor;
}
#ifndef _CPPUNIT

//TAR225069+ CHU/DALE
SMStateBase *SMStateBase::DMCashChangerInOperable()
{
    CString csWork;

    //SR672
    
    CString sSubModel = _T("Bill");
    bool fCheckHealth = false;
    long lcode = dm.GetResultCodeExtended(DMCLASS_CASHCHANGER, 0);
    CString sModelName =dm.GetDeviceModelName(DMCLASS_CASHCHANGER, 0);

    csWork = DMGetDeviceErrorHTML(DMCLASS_CASHCHANGER, sModelName, sSubModel, CASH_STATUS_INOPERABLE, lcode, 0, NULL, &fCheckHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
    /*csWork.Format(ERRFMT, ps.GetPSText(MSG_CC_STATUS_INOPERABLE_EX,SCOT_LANGUAGE_PRIMARY),                     \
                  DMCLASS_CASHCHANGER, DM_CC_STATUS_INOPERABLE);  */                                        \
    ScotError(INFO,SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_DM_CHECK_HEALTH); 
    //We save the following so we can handle CashChange event while we are at SystemMessae
    m_lSavedCashChangerStatusCode = CASH_STATUS_INOPERABLE;


     SMStateBase* sExit =
    createLevel1Message(csWork,                 // char *message
                        PS_NOTEXT,              // no canned message
                        PS_NOTEXT,              // no prompt
                        0,                      // no value
                        DMCLASS_CASHCHANGER,    // device reporting the error or -1
                        true, true);    // T/F need to issue CheckHealth after operator clears problem

    //Notify RAP
    trace(L7, _T("+ra.OnRAPDataNeeded, ErrorMessage=%s, DeviceError=%d"), csWork, nDMLastDev);
    // write the error description on the receipt
    if (!sDeviceErrorTitle.IsEmpty())
        ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + sDeviceErrorTitle); //Tar 450308 and 450430
    // save the previous view
    m_PrevView = m_View;
    // send a DataNeeded Approval
    cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("DeviceFailure"));
    m_View = DATANEEDED_DEVICEERROR;
    CString csSecInfo;
    if (!sDeviceErrorTitle.IsEmpty())
        csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),sDeviceErrorTitle,ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""), _T(""), _T(""));

    ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);

    trace(L7, _T("-ra.OnRAPDataNeeded"));
 
    return sExit;
}
//TAR225069-


SMStateBase* SMStateBase::HandleScannerData(const MessageElement* message)
{
    SMStateBase* pRetState = STATE_NULL;
    struct ExtStateEventNotifyInType  sNotifyIn;
    sExtStateEventNotifyOut sNotifyOut;
    sNotifyIn.InitStruct();
    sNotifyOut.InitStruct();
    //TAR238515+

    

    if (!dm.IsScannerEnabled())
    {
        //We want to ignore this scan data since Scanner is in the process of disabling
        trace(L2, _T("::DMParse, DMCLASS_SCANNER - bScannerIsEnabled is set to false, scan is ignored."));
        trace(L6,_T("-Parse"));
        DMSayPhrase(PLEASERESCANITEM);
        return STATE_NULL;
    }
    //TAR238515-

    
    // The scanner should be disabled physically (autodisable),
    // but issue this for bookkeeping
  {  // begin of block A
    DMScannerDisable();
    // Add intercept for scan of special operators card
    // to immediately go to store mode       
    // the function that checks if this bar code is a special operator
    // bar code should be in the UserExit module since this is customer
    // specific

    // notity TB the scanner event and request further direction
    // prenotify event to TB begin


    bool bNeed2NotifyTB = false;
    bool bIsValidOperator = isBarcodeValidOperatorPassword(cDMLastData);
    if (EXTRACT_CLASSNAME(this).Find(_T("SMExtensionState")) != -1)
    {
        // DO NOT call TBValidOperator but extract 
        // operator id and password based on the scan data
    

        if (!(csOperator.IsEmpty() && csPassword.IsEmpty()))
        {
            // operator related barcode
                // let TB decide the flow
            // prenotify event to TB begin
            CString csEventData;
            ADD_SCOTEVENT_DATA_FIRST(csEventData, "SCAN_RAW", cDMLastData);
            ADD_SCOTEVENT_DATA(csEventData,_T("OPERATOR_ID"), csOperator);
            ADD_SCOTEVENT_DATA(csEventData,_T("OPERATOR_PASSWORD"), csPassword);
            sNotifyIn.SetEventData(_T("SCANNER_DATA"), csEventData, csEventData.GetLength());
            SMStateBase* pTBState = PreNotifySCOTEvent2TB(message, sNotifyIn, sNotifyOut);
            sNotifyIn.CleanUp();
            if (!sNotifyOut.bAllowDefaultProcessing)
            {
              trace(L4, _T("PreNotifyDMEvent2TB() returns to a new state"));
              return pTBState;
            }
            // prenotify event to TB end
        }
        else
        {
            bNeed2NotifyTB = true; // notify TB before calling DMScanner() for normal scans
        }
    }

    // BeginCodeLocaleUPC  // SR93.4 code block moved because global variables are needed by DMScannerEncrypted()

    // 3 Fields: LastData|0xe1|Label|0xe1|Type
    int iFind;
    CString csData1;
    CString csData2;
    CString csData3;
    csData1 = cDMLastData;
    csData2 = "";
    csData3 = "";

    // Get First Field
    iFind=csData1.Find((_TCHAR)DM_FS);
    ASSERT (iFind != -1);
    csDMLastBarCode = csData1.Left(iFind);


    // Get Second Field
    csData2 = csData1.Mid(iFind+1);
    ASSERT (csData2.GetLength()>0);
    iFind=csData2.Find((_TCHAR)DM_FS);
    ASSERT (iFind != -1);
    csDMLastBarLabel = csData2.Left(iFind);

    // Get Third Field
    csData3 = csData2.Mid(iFind+1);
    ASSERT (csData3.GetLength()>0);
    csDMLastBarType = _ttol(csData3);

    // EndCodeLocaleUPC



    if (bIsValidOperator)      
    { 
        //+SR819
        //Validate head cashier if configured to use head cashier context in SCOTOPTS.
        //or Validate Head Cashier if configured to use context sensitive menus    
        if ( co.fStateContextSensitiveMenus || (!co.csOperationsHeadCashierContext.IsEmpty() && 
             (co.csOperationsHeadCashierContext != _T("SmAuthorization"))) )
        {
            g_bValidHeadCashier = TBValidateHeadCashier(csOperator, csPassword);
        }
        //-SR819

        m_bLastScanDataWasMultiLabel = FALSE;
        if (!storeMode())  // only valid in customer mode or help mode
        {
            //+TAR 343053 
            bool bMakeXMLNow = false;
            if (!rp.IsTransactionInProgress() )
            {
                bMakeXMLNow = true;
                rp.TransactionStart(rp.m_csBusinessDate);
            }
            rp.SetOperator(csOperator);
            rp.OperatorLogin(rp.m_lOperatorLoginID, ATTIN, SCOTAPP);
            rp.m_lOperatorLoginID += 1;
            if (bMakeXMLNow)
            {
                rp.TransactionEnd();
            }
            //-TAR 343053
            if(co.IsInAssistMenus())
            {
                // Not in Store Mode but we know that SSCO came from the assist menus which is a store mode
                trace(L5,_T("Ignoring Operator Barcode since we are already in Store Mode. assistMenus=%d"),co.IsInAssistMenus());
                DMScannerEnable();
                return STATE_NULL;
            }
            if (!helpMode())
            {
				//sscoadk-5605+ if operator barcode is scanned at ScanAndBag, we need to tell SM about exiting ScanAndBag
				CString csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
				if (csCurrentStateName.Find(_T("ScanAndBag")) != -1)
				{
					if (g_bAllowLateBagging)
					{
						CKeyValueParmList parmList;
						CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_CLEAR_ALLOW_LATE_BAGGING);
						_bstr_t bstrParmList((const _TCHAR *)parmList);
						BSTR bstrResult;
						bstrResult = NULL;
						SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
						SysFreeString(bstrResult);
					}
					TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag"));
				}
				//sscoadk-5605-

                RETURNSTATE(SmAuthorization);
            }
            else
            {
                // Help mode -- Should be SMOperatorPassword, but we don't know where to 
                // go from there so we will just pass along the OP and PASS and let the 
                // system continue.
                DMScannerEnable(); 
                fNewItemScannedSinceLastUnDeActivatedItem = true;
                //USSF Start
                USSFDATA(_T("DMLASTBARCODE"), csDMLastBarCode);
                USSFDATA(_T("DMLASTBARLABEL"), csDMLastBarLabel);
                USSFDATA(_T("DMLASTBARTYPE"), csDMLastBarType);
                USSFHOOK(_T("DMXOperator"), nextState = DMScanner(), return nextState);
                //return DMScanner();         // barcode
                //USSF End
            }
        }
        else
        {
            // in store mode or assist mode this barcode has no useful meaning
            DMScannerEnable(); 
            return STATE_NULL;
        }
    }
    else
    {
        if (helpMode())  // TAR 253541
        {
            DMScannerEnable(); 
            fNewItemScannedSinceLastUnDeActivatedItem = true;
            //USSF Start
            USSFDATA(_T("DMLASTBARCODE"), csDMLastBarCode);
            USSFDATA(_T("DMLASTBARLABEL"), csDMLastBarLabel);
            USSFDATA(_T("DMLASTBARTYPE"), csDMLastBarType);
            // +SR93.4
            if ( dm.IsBarcodeTypeEncrypted(csDMLastBarType) )
            {
                USSFHOOK(_T("DMXScanRead"), nextState = DMScannerEncrypted(), return nextState);
            }
            else
            {
                USSFHOOK(_T("DMXScanRead"), nextState = DMScanner(), return nextState);
            }
            // -SR93.4
            //return DMScanner();         // barcode
            //USSF End
        }

        if ((storeMode() && !(fStateAssist || co.IsInAssistMenus())))
        {
            if (!bSecMaintMode)     //new for Security Maintenance Mode
            {
                // in help mode ONLY manager barcode allowed
                // nothing scanned in store mode (unless we are in assist mode) is allowed
                trace(L6, _T("BARCODE not valid operator/password. Ignoring scan due to either HelpMode=%d, or StoreMode=%d and AssistMode=%d."), helpMode(), storeMode(), fStateAssist);
                DMScannerEnable(); 
                m_bLastScanDataWasMultiLabel = FALSE;
                return STATE_NULL;
            }
        }
        trace(L6, _T("BARCODE not valid operator/password. Not ignoring scan due to either HelpMode=%d, or StoreMode=%d and AssistMode=%d."), helpMode(), storeMode(), fStateAssist);
    }
   

    MultiLabelBarcodeFormatRecord MLBFRecord;
    m_bLastMntScanDataWasMultiLabel = FALSE;
    BOOL bIsPartofMultiBarcode = (g_MultiLabelBarcodes.IsPartOfMultiLabel(cDMLastData, &MLBFRecord));
    if(bIsPartofMultiBarcode)
    {
        if(!m_bLastScanDataWasMultiLabel)
        {
            //set MultiLabel related variables
            ResetMLBCValues(); //reset Multi-Label Barcode variable if necessary
            m_nTotalNumberOfMLBParts = g_MultiLabelBarcodes.GetTotalNumberOfParts(MLBFRecord.nIndex);
            m_MLBarcodeList.RemoveAll();
        }
        AddMLBCRecord(&MLBFRecord);
        if(!m_bLastScanDataWasMultiLabel)
        {
            trace(L6,_T("-Parse (MULTI_LABEL) Ignoring Scan and Returning STATE_NULL Because label was scanned twice or different barcode type"));
            ResetMLBCValues();
            DMScannerEnable(); //RFC 241710
            return STATE_NULL;  
        }
        if(m_nNumberOfPartsReturnedSoFar == m_nTotalNumberOfMLBParts)
        {
            //assemble the code to send to security and the full code to send to TB
            csDMLastBarCode = GetItemCodeFromMLBC();
            csDMLastBarLabel = csDMLastBarCode;

            trace(L6,_T("-Parse (MULTI_LABEL) Returning DMScanner() After receiving all barcodes"));
            fNewItemScannedSinceLastUnDeActivatedItem = true;
            //USSF Start
            USSFDATA(_T("DMLASTBARCODE"), csDMLastBarCode);
            USSFDATA(_T("DMLASTBARLABEL"), csDMLastBarLabel);
            USSFDATA(_T("DMLASTBARTYPE"), csDMLastBarType);
            USSFHOOK(_T("DMXScanRead"), nextState = DMScanner(), return nextState);
            //return DMScanner();         // barcode
            //USSF End
        }

        trace(L6,_T("-Parse (MULTI_LABEL) Returning STATE_NULL to wait for next barcode"));
        DMScannerEnable(); //RFC 241710
        return STATE_NULL;  
    }
    else
    {
        m_bLastScanDataWasMultiLabel = FALSE;
        ResetMLBCValues();
    }

    // dont let the scanner be re-enabled until the EAS and/or motion sensor are tripped
    trace(L2, _T("::DMParse, DMCLASS_SCANNER - bEASReEnableScanner is set to false."));
    bEASReEnableScanner = false;    // TAR199297 moved til after operator barcode verified LPM032002

    if (fItemIsWaiting)
    {
      // Beep(500,500);
      trace(L6,_T("-Parse"));
      return STATE_NULL;
    }
               
    //Need to know if Code 39 early 
    io.cScanType = csDMLastBarType;  


    if (InvalidCouponPrefix(csDMLastBarCode))  //TAR 269159
        {
            dm.SaySecurity(SEC_NONE, ps.GetSoundName(COUPONSNOTACCEPTED));
            dm.ScannerEnable();
            return STATE_NULL;
        }

    /*   RFC 412256 - support GS1 barcode, taking out temp fix
    //This is a temp fix for RSS barcode since the current OPOS doesn't support RSS barcode scantype.
    //So OPOS doesn't return the correct scan type for RSS in csDMLastBarType. It also contains the "]e0" prefix
    //in the barcode. The "]e0" prefix should be extracted from the RSS barcode. Tar 247406
    //After OPOS and SO fix the scan type issue, the following code need be removed.
    //RSS temp fix starts
    if(csDMLastBarCode.Find(_T("]e0")) != -1 )
    {
        //csDMLastBarLabel  = csDMLastBarLabel.Mid(3);    //TAR 380552
        io.cScanType = SCAN_SDT_RSS;
        csDMLastBarType = SCAN_SDT_RSS;
    }
    //RSS temp fix ends
    */

    trace(L6,_T("-Parse"));
    fNewItemScannedSinceLastUnDeActivatedItem = true;
    if (bNeed2NotifyTB)
    {
        CString csEventData;
        ADD_SCOTEVENT_DATA_FIRST(csEventData,_T("SCAN_RAW"), cDMLastData);
        ADD_SCOTEVENT_DATA(csEventData,_T("SCAN_CODEWITHTYPE"), csDMLastBarLabel);
        ADD_SCOTEVENT_DATA(csEventData,_T("SCAN_CODEWITHOUTTYPE"), csDMLastBarCode);
        CString csLabelType;
        csLabelType.Format(_T("%d"), io.cScanType);
        ADD_SCOTEVENT_DATA(csEventData,_T("SCAN_TYPE"), csLabelType);
        SMStateBase* pTBState = PreNotifySCOTEvent2TB(message, sNotifyIn, sNotifyOut);
        sNotifyIn.CleanUp();
        if (!sNotifyOut.bAllowDefaultProcessing)
        {
          trace(L4, _T("PreNotifyDMEvent2TB() returns to a new state"));
          pRetState = pTBState;
        }
    }
    // REQ 20060728-986 Start Special Transaction
    if ((!bNeed2NotifyTB) || (sNotifyOut.bAllowDefaultProcessing))
    {
        CString csDataString;
        ADD_SCOTEVENT_DATA_FIRST(csDataString,_T("SCAN_RAW"), cDMLastData);
        ADD_SCOTEVENT_DATA(csDataString,_T("SCAN_CODEWITHTYPE"), csDMLastBarLabel);
        ADD_SCOTEVENT_DATA(csDataString,_T("SCAN_CODEWITHOUTTYPE"), csDMLastBarCode);
        CString csLabelType;
        csLabelType.Format(_T("%d"), io.cScanType);
        ADD_SCOTEVENT_DATA(csDataString,_T("SCAN_TYPE"), csLabelType);

        static long rc;  // TB return value
        rc = tb.PreprocessData(csDataString, _T("SCANNER_DATA"), &sNotifyOut.bAllowDefaultProcessing);
        if (!sNotifyOut.bAllowDefaultProcessing)
        {
            // Enable scanner after start special transaction
            trace(L2, _T("Enable scanner after start special transaction."));
            bEASReEnableScanner = true;
            SAWLDBScannerEnable();
            // If we are in the attract screen, start a transaction by going to scan and bag.
            if (getAnchorState() == BES_START)
            {
                pRetState=setAndCreateAnchorState(BES_SCANANDBAG);
            }
        }
    }

    if (sNotifyOut.bAllowDefaultProcessing)
    {
        //+SR815 - Personalization
        if ((getStateName(this) != _T("SMAttract") && bIsTransactionStarted))
        {
            //If data matches personalization filter, do a loyalty lookup
            //SMAttract has specialized handling in place in DMCardReader
            LoyaltyCheck(_T("Scan"), csDMLastBarLabel);
        }
        //-SR815 - Personalization

        //USSF Start
        USSFDATA(_T("DMLASTBARCODE"), csDMLastBarCode);
        USSFDATA(_T("DMLASTBARLABEL"), csDMLastBarLabel);
        USSFDATA(_T("DMLASTBARTYPE"), csDMLastBarType);
        // +SR93.4
        if ( dm.IsBarcodeTypeEncrypted(csDMLastBarType) )
        {
            USSFHOOK(_T("DMXScanRead"), pRetState = DMScannerEncrypted(), ;);
        }
        else
        {
            USSFHOOK(_T("DMXScanRead"), pRetState = DMScanner(), ;);
        }
        // +SR93.4
        //pRetState = DMScanner();         // barcode
        //USSF End
    }
  }    // end of block A     
  return pRetState;
}  // HandleScannerData



//#ifdef ENABLE_FillDMEvent2NotifyTB
SMStateBase* SMStateBase::FillDMEvent2NotifyTB(const MessageElement* pEvent, sExtStateEventNotifyIn& sNotifyIn, sExtStateEventNotifyOut& sNotifyOut)
{
    // reserved for future usage
    return STATE_NULL;
}
//#endif // ENABLE_FillDMEvent2NotifyTB

//Tar 275064
bool SMStateBase::IsMINIHardware()   // SSCOADK-3006: Update to match CADD
{    
    bool bRet = false;

    // The right one to check is 'HWSpec'
	if( !_tcsicmp(co.csHWSpec, _T("MINI")))
 	   bRet = true;
    trace(L6, _T("SMStateBase::IsMINIHardware()= %d"), bRet);
    return bRet;

}

//tar 397356
bool SMStateBase::IsSUPERMINI() // SSCOADK-3006: Update to match CADD
{    
    bool bRet = false;

     // For HWSpec is the one to check
    if( !_tcsicmp(co.csHWSpec, _T("SUPERMINI")))   
		bRet = true;		
    trace(L6, _T("SMStateBase::IsSUPERMINI()= %d"), bRet);
	return bRet;
}


//Tar 403042
bool SMStateBase::Is5XHardware() // SSCOADK-3006: Update to match CADD
{    
	bool bRet = false;

    // The most reliable setting to check is 'HWType', not 'Type'
	if( !_tcsicmp(co.csHWType, _T("SCOT5")))   
        bRet = true;
    return bRet;
}

bool SMStateBase::Is6XHardware(void) const
{
	if( !_tcsicmp(co.csHWType, _T("SCOT6")))
	{
		return true;
	}
	
	return false;
}

//Tar 452490
//Check if the system has cash device configured to install
bool SMStateBase::IsCashDeviceConfigured() 
{ 
    bool bRet = false;
    if ((dm.IsDeviceConfigured(DMCLASS_CASHACCEPTOR,0)) ||
        (dm.IsDeviceConfigured(DMCLASS_COINACCEPTOR,0)) ||
        (dm.IsDeviceConfigured(DMCLASS_CASHCHANGER,0)) ||
        (dm.IsDeviceConfigured(DMCLASS_CASHRECYCLER,0)))
    bRet = true;
    trace(L6, _T("+-SMStateBase::IsCashDeviceConfigured: Return Code=%d"), bRet);
    return bRet;
}

// SR875 +
bool SMStateBase::IsVerifoneConfigured() 
{ 
    bool bRet = false;
    _TCHAR szValue[10];       
    LONG   lRC;
    HKEY   hKey;
    DWORD  dwDataSize = sizeof(szValue);

    lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\DeviceManagerEx\\Devices\\Local\\MSR\\Verifone"), 
               0, KEY_QUERY_VALUE, &hKey);

    memset(szValue,0, sizeof(szValue));

    lRC = RegQueryValueEx(hKey, _T("Configure"), NULL, NULL,(LPBYTE)szValue, &dwDataSize);
    ::RegCloseKey(hKey);
    if(lRC == ERROR_SUCCESS)
    {
        if( !_tcsicmp(szValue, _T("Yes")))
        {
            bRet = true;
        }
        else
        {
            bRet =false;
        }
    }
    trace(L6, _T("+-SMStateBase::IsVerifoneConfigured: Return Code=%d"), bRet);
    return bRet;
}

void SMStateBase::DisplayVerifoneForm(_bstr_t csForm)
{
    if(IsVerifoneConfigured())
    {
        if (m_pVForm == NULL)
        {
            HRESULT hr = S_OK;
            hr= m_pVForm.CreateInstance("OPOS.Form.1"); 
            if (m_pVForm)
            {
                m_pVForm->Open(_T("Verifone Mx"));
                m_pVForm->Claim(5000);
                m_pVForm->PutDeviceEnabled(1);
            }

            m_pVForm->InitForm(csForm);
            m_pVForm->ShowForm();
        }
        else
        {
            m_pVForm->InitForm(csForm);
            m_pVForm->ShowForm();           
        }
    }
}
// SR875 -
#endif //_CPPUNIT

SMStateBase* SMStateBase::DMRemoteScannerConnect(bool bConnect)
{
    trace(L6, _T("SMStateBase::DMRemoteScannerConnect(%d)"), bConnect);
    if (bConnect)
    {
        dm.ScannerRemoteAccess(true);
        CREATE_AND_DISPENSE(RemoteScannerConnect)(storeMode());
    }
    else
    {
        //Save current value of Attendant mode,
        // Save orig Attendant mode setting for comparision 
        bool bAttendantMode = co.IsAttendantModeOn();
        //Cancel connection accepted in DMParse
        co.SetRemoteScannerConnected(false);
        //Check if Attendant mode is now false because it was entered using HH Scanner.
        if (!co.IsAttendantModeOn() == bAttendantMode)  
        {
            trace(L6, _T("Remote scanner disconnect exiting attendant mode"));
            ps.SMButton(_T("SMButton7"), MSG_LEAVE_ATTENDANT_MODE, true);
            SetAttendantModeStateInSM(false); //SSCOADK-544
            SetNextGenUIMode();

            rp.SetOperator(csOperator);
            co.SetAttendantID(EMPTY_STRING);
            rp.ModeChange(rp.m_lAttendantEntryID, ATTENDANT, EXIT);
            DMTakeawayBeltNotify(tabEventAssistModeStop);
            rp.m_lAttendantEntryID += 1;
        }
        ps.ShowTrainingModeOnScreen(TrainingModeObj.IsTMOn());
        return STATE_NULL;
    }
}

//PW500001

// If the acceptor cash or count levels reach the co.nLocaleCashAcceptorHighThreshold or

// co.nLocaleCoinAcceptorHighThreshold then flash the green tri-light
#ifndef _CPPUNIT
void SMStateBase::DMHandleAcceptorHigh()
{
 
    CArray<int,int> caCashAcceptorCount, caCoinAcceptorCount, caCashDispenserCount, caCoinDispenserCount;
    
    dmcc.GetCashCounts(&caCashAcceptorCount, &caCoinAcceptorCount, &caCashDispenserCount, &caCoinDispenserCount);
    
    int iCashArrayMax, iCoinArrayMax = 0;

    iCashArrayMax = caCashAcceptorCount.GetSize() - 1;
    iCoinArrayMax = caCoinAcceptorCount.GetSize() - 1;

    bool bRetVal = false;

    long lCashTotal, lCoinTotal, lRunningTotal = 0;

    int iCount = 0;

    for (iCount = 0; iCount <= iCashArrayMax; iCount++)
        lRunningTotal += caCashAcceptorCount.GetAt(iCount);

    lCashTotal = lRunningTotal;
    lRunningTotal = 0;

    for (iCount = 0; iCount <= iCoinArrayMax; iCount++)
        lRunningTotal += caCoinAcceptorCount.GetAt(iCount);

    lCoinTotal = lRunningTotal;
    lRunningTotal = 0;

    long lCashHighThreshold, lCoinHighThreshold = 0;

    lCashHighThreshold = co.nLocaleCashAcceptorHighThreshold;
    lCoinHighThreshold = co.nLocaleCoinAcceptorHighThreshold;

    //dp185016 tar 300547 add checking before setting state of tricolorlight
    //if color of tricolorlight is red, or yellow, then don't override the color
    if( dm.getTriColorLightState() == DM_TRICOLORLIGHT_OFF ||
      !(dm.getTriColorLightColor() == DM_TRICOLORLIGHT_RED || 
      dm.getTriColorLightColor() == DM_TRICOLORLIGHT_YELLOW) ) 
    {
      if( (lCashTotal >= lCashHighThreshold) || (lCoinTotal >= lCoinHighThreshold) )
      {

          // (*) SR898 ====================
          DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                             DM_TRICOLORLIGHT_BLINK_1HZ,
                             DM_TRICOLORLIGHT_NORMALREQUEST);
      }
      else
      {
        // (*) SR898 ====================
        DMTurnOffTriColorLight();
        
        // (*) SR898 ====================
        DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                           DM_TRICOLORLIGHT_ON,
                           DM_TRICOLORLIGHT_NORMALREQUEST);
      }
    }
    return;
}

//+4.2 card dispenser
//************************************************************************ 
//
// Function:        DMDispenserPositionEvent
// Purpose:         This is called when an event from the card dispenser is 
//                  detected.  SSF override this method. 
// Parameters:      long lStat - Card position reported by the event
// Returns:         State
// Calls:
// Side Effects:
// Notes:           
//
//************************************************************************
SMStateBase* SMStateBase::DMDispenserPositionEvent( long lStat)
{
    return STATE_NULL;
}
//-4.2 card dispenser

//=====================================================================
// getHopperStatus
//   Converts a device manager cash changer hopper status code to a phrase
//   and returns it. Doesn't return any status message if bin is normal or low
//   (low is displayed in currency section, because we don't care if one
//   $1 bin is low, only if the total # of $1 bills is low). 
//=====================================================================
void SMStateBase::getHopperStatus(const CString denominationCode,
                                          CString& denominationStatus)
{
    //SR672
  dm.m_csCoinDispenserModel.MakeUpper();
  switch (*denominationCode)
  {
    case _T('0'):
        denominationStatus.Empty();  //Don't display any text if bin is normal,
        break;                       //only bins in error
    case _T('1'):
        denominationStatus.Empty();  //Don't check for low currency here, but in
                               //initializeCurrencyList() instead
        
        //denominationStatus = ps.GetPSText(MSG_BINISLOW);
        
        break;
    case _T('2'):
        //dp185016 tar 299314
        if (dm.m_csCoinDispenserModel.Find(_T("HOPPER")) != -1 && !dm.m_csCoinDispenserStage.IsEmpty() ) //SR672
        {
                denominationStatus = ps.GetPSText(MSG_BINISEMPTYORJAMMED,SCOT_LANGUAGE_PRIMARY );
        }
        else if( !co.fLocaleCashManagementScreen )
        {
                denominationStatus = ps.GetPSText(MSG_BINISEMPTY, SCOT_LANGUAGE_PRIMARY);
        }
        else
            denominationStatus = ps.GetPSText(MSG_BINISEMPTY, SCOT_LANGUAGE_PRIMARY);
        break;
    case _T('3'):
        if (dm.m_csCoinDispenserModel.Find(_T("HOPPER")) != -1 && (dm.m_csCoinDispenserStage.IsEmpty()) ) //SR672
        {            
        denominationStatus = ps.GetPSText(MSG_BINISEMPTYORJAMMED, SCOT_LANGUAGE_PRIMARY);
        }
        else
        {
            denominationStatus = ps.GetPSText(MSG_BINISJAMMED, SCOT_LANGUAGE_PRIMARY);
        }
        break;
    case _T('4'):
        denominationStatus = ps.GetPSText(TXT_CASSETTE_OUT, SCOT_LANGUAGE_PRIMARY);
        break;
    case _T('9'):
        denominationStatus = ps.GetPSText(TXT_TRANSPORT_JAM, SCOT_LANGUAGE_PRIMARY);
        break;
    default:
        denominationStatus = ps.GetPSText(MSG_BINPROBLEM, SCOT_LANGUAGE_PRIMARY);
  }
}

//=========================================================================
// getNextCurrencyStatusLine
//
//   Returns one entire currencyStatusLine for the next currency.  Each
//   currency type (from map) and its associated status (from status list)
//   are parsed (one per invocation).  The last currency type and status
//   are deleted from their strings.  e.g. if currencyMap is
//   "USD:500,USD:1000,USD:2000" and currencyStatusList is "1,2,0", they
//   get returned as "USD:1000,USD:2000" and "2,0", and currencyStatusLine
//   is "    5 dollar bin is low" (in case of a 1 dollar coin, this would. 
//   dinstinguish it as the "100 cent bin" as opposed to the "1 dollar bin"
//=========================================================================
bool SMStateBase::getNextHopperStatusLine(CString& currencyMap,
                                                  CString& hopperStatusList,
                                                  CString& hopperStatusLine, bool bDenominationOnly)
{
  CString csCurrencyType, csDenominationStatusCode, csDenominationStatus;
  int colonIndex, commaIndex = 0;
  int nDenomination;

  hopperStatusLine.Empty(); // SSCOP-2804
    
  if ((0 == currencyMap.Find(_T(';')) ) && ( 0 == hopperStatusList.Find(_T(';')) ))
  {


     // This is a cash dispenser only system with no coins
     currencyMap = currencyMap.Mid(1);
     hopperStatusList = hopperStatusList.Mid(1);
  }

  // Find the first currency denomination in the list
  // There will be a colon in every valid invocation
  if ( (colonIndex = currencyMap.Find(_T(':'))) > -1 )
  {
    commaIndex = currencyMap.FindOneOf(_T(",;")); // separators in currency map
    if (commaIndex == -1)
      commaIndex = currencyMap.GetLength();

    // get denomination value, e.g. 5 (nickels) or 1000 ($10 bills)
    nDenomination = _ttol(currencyMap.Mid(colonIndex + 1,commaIndex - colonIndex - 1));

    if (currencyMap.Find(_T(';')) != -1) //we're still in the coins
    {
        csCurrencyType = ps.GetPSText(MSG_COIN, SCOT_LANGUAGE_PRIMARY);
        //tar 344120
        int nDenomTemp = nDenomination / co.nLocalePriceCentUnit;
        if (nDenomTemp >= 1)
        {
            csCurrencyType = ps.GetPSText(MSG_BILL, SCOT_LANGUAGE_PRIMARY); 
            nDenomination = nDenomination / co.nLocalePriceCentUnit;
        }
    }
    else
    {
       csCurrencyType = ps.GetPSText(MSG_BILL, SCOT_LANGUAGE_PRIMARY); 
       nDenomination = nDenomination / co.nLocalePriceCentUnit;
    }
    
    // remove the part parsed
    if (commaIndex < currencyMap.GetLength())
        currencyMap = currencyMap.Right(currencyMap.GetLength() - commaIndex - 1);
    else
        currencyMap = _T("");                         // final time, delete string
    
    // Now find the corresponding status of that denomination
    commaIndex = hopperStatusList.FindOneOf(_T(",;"));
    if (commaIndex == -1)
        commaIndex = hopperStatusList.GetLength();
    
    // get status code, e.g. "0" or "9"
    csDenominationStatusCode = hopperStatusList.Left(commaIndex);
    
        // remove the parsed part
    if ( commaIndex < hopperStatusList.GetLength() )
        hopperStatusList = hopperStatusList.Mid(commaIndex + 1);
    else
        hopperStatusList = _T("");
        
    // convert the status number to a phrase to output
    getHopperStatus(csDenominationStatusCode, csDenominationStatus);

    if (!csDenominationStatus.IsEmpty() && nDenomination != 0 )   //if the bin isn't normal
    {
        
        // format the line from the denomination and status
        if (bDenominationOnly )
        {  
            if (csDenominationStatusCode==_T("3"))  //only jam status case
                hopperStatusLine.Format(_T("%3d %s"),   nDenomination, csCurrencyType) ;
        }
        else
            hopperStatusLine.Format(_T("   %3d %s %s"),
                                nDenomination,
                                csCurrencyType,
                                csDenominationStatus) ;
    }
    else
    {
      // TAR 211509 
        //if (nDenomination == 10 && *csDenominationStatusCode == _T('1'))
          //hopperStatusLine.Format(_T("   %3d %s %s"),
                               // nDenomination,
                               // csCurrencyType,
                               // ps.GetPSText(MSG_BINISLOW)) ;
        
      
      
      hopperStatusLine.Empty(); // Need to clear line if status OK
    }
    
    if (!hopperStatusLine.IsEmpty())
        trace(L6,_T("SMStateBase::getNextHopperStatusLine: %s"), hopperStatusLine );
        
    return true;
  }

  return false; // invalid syntax or empty string
}
bool SMStateBase::IsHopperSubstitutionUsed(bool &bEmpty, bool &bChuteJammed)
{
  bool bRet = false;
  bEmpty = false;
  bChuteJammed = false;
  //SR672
  dm.m_csCoinDispenserModel.MakeUpper();
  if(dm.m_csCoinDispenserModel.Find(_T("HOPPER")) != -1 && !dm.m_csCoinDispenserStage.IsEmpty() )
  {
        CString hopperStatusList = dm.CashChangerGetLowIndicators();
        CString sStatusCode;

        int idx = hopperStatusList.Find(_T(';'));
        if (idx != -1)
        {
            sStatusCode=hopperStatusList.Left(idx);

        }
        if (sStatusCode.Find(_T("2")) !=-1)  //empty
        {
            bRet = true;
            bEmpty=true;
        }

        if (sStatusCode.Find(_T("3")) !=-1) //jam
        {
        //  bRet = true;
            bChuteJammed = true;
        }       
  }
  return bRet;

}

CString SMStateBase::getHopperStatusString(void)
{
    CString hopperStatusLine, sHopperStatus;
    hopperStatusLine.Empty();
    sHopperStatus.Empty();
    CString hopperStatusList = dm.CashChangerGetLowIndicators();
    CString currencyMap = dm.CashChangerGetCurrencyMap();

    while ( getNextHopperStatusLine(currencyMap, hopperStatusList, sHopperStatus, false) )
    {
      if (!sHopperStatus.IsEmpty()) 
      {
          hopperStatusLine += _T("\\n-") + sHopperStatus;
      }
    }
    return hopperStatusLine;

}

CString SMStateBase::getHopperDenominations(long& lErrorCode)
{
    CString hopperStatusLine, sHopperStatus;
    hopperStatusLine.Empty();
    sHopperStatus.Empty();
    if (lErrorCode == NCR_ECOINDISP_PAYOUT_HOPPER_1 ||
        lErrorCode == NCR_ECOINDISP_PAYOUT_HOPPER_2 ||
        lErrorCode == NCR_ECOINDISP_PAYOUT_HOPPER_3 ||
        lErrorCode == NCR_ECOINDISP_PAYOUT_HOPPER_4 ||
        lErrorCode == NCR_ECOINDISP_PAYOUT_HOPPER_5 ||
        lErrorCode == NCR_ECOINDISP_PAYOUT_HOPPER_6 ||

        lErrorCode == NCR_ECOINDISP_LEVEL_HOPPER_1 ||
        lErrorCode == NCR_ECOINDISP_LEVEL_HOPPER_2 ||
        lErrorCode == NCR_ECOINDISP_LEVEL_HOPPER_3 ||
        lErrorCode == NCR_ECOINDISP_LEVEL_HOPPER_4 ||
        lErrorCode == NCR_ECOINDISP_LEVEL_HOPPER_5 ||
        lErrorCode == NCR_ECOINDISP_LEVEL_HOPPER_6 ||

        lErrorCode == NCR_ECOINDISP_SHORT_HOPPER_1 ||
        lErrorCode == NCR_ECOINDISP_SHORT_HOPPER_2 ||
        lErrorCode == NCR_ECOINDISP_SHORT_HOPPER_3 ||
        lErrorCode == NCR_ECOINDISP_SHORT_HOPPER_4 ||
        lErrorCode == NCR_ECOINDISP_SHORT_HOPPER_5 ||
        lErrorCode == NCR_ECOINDISP_SHORT_HOPPER_6 ||

        lErrorCode == NCR_ECOINDISP_OVERCUR_HOPPER_1 ||
        lErrorCode == NCR_ECOINDISP_OVERCUR_HOPPER_2 ||
        lErrorCode == NCR_ECOINDISP_OVERCUR_HOPPER_3 ||
        lErrorCode == NCR_ECOINDISP_OVERCUR_HOPPER_4 ||
        lErrorCode == NCR_ECOINDISP_OVERCUR_HOPPER_5 ||
        lErrorCode == NCR_ECOINDISP_OVERCUR_HOPPER_6 ||

        lErrorCode == NCR_ECOINDISP_EEPROM_HOPPER_1 ||
        lErrorCode == NCR_ECOINDISP_EEPROM_HOPPER_2 ||
        lErrorCode == NCR_ECOINDISP_EEPROM_HOPPER_3 ||
        lErrorCode == NCR_ECOINDISP_EEPROM_HOPPER_4 ||
        lErrorCode == NCR_ECOINDISP_EEPROM_HOPPER_5 ||
        lErrorCode == NCR_ECOINDISP_EEPROM_HOPPER_6 ||

        lErrorCode == NCR_ECOINDISP_WRITE_FAIL_HOPPER_1 ||
        lErrorCode == NCR_ECOINDISP_WRITE_FAIL_HOPPER_2 ||
        lErrorCode == NCR_ECOINDISP_WRITE_FAIL_HOPPER_3 ||
        lErrorCode == NCR_ECOINDISP_WRITE_FAIL_HOPPER_4 ||
        lErrorCode == NCR_ECOINDISP_WRITE_FAIL_HOPPER_5 ||
        lErrorCode == NCR_ECOINDISP_WRITE_FAIL_HOPPER_6 
        )
    {
        CString hopperStatusList = dm.CashChangerGetLowIndicators();
        CString currencyMap = dm.CashChangerGetCurrencyMap();

        while ( getNextHopperStatusLine(currencyMap, hopperStatusList, sHopperStatus, true) )
        {
          if (!sHopperStatus.IsEmpty()) 
          {
              hopperStatusLine += sHopperStatus;
          }
        }
    }

    return hopperStatusLine;
    
}

bool SMStateBase::isOnlyHopperSubsApprovalRequired(void)
{
        bool fUpdateSA= false;
        bool fReturnVal = false;
        
        if (lUnapprovedHopperSubstitution> 0)
        {
            fReturnVal = true;
        }

        if (isRestrictedItemApprovalRequired() ||
            isVisualItemApprovalRequired() ||
            isCouponRatioApprovalRequired(fUpdateSA) ||
            isSecurityApprovalRequired())
        {
            fReturnVal = false;
            
        }

        if ((lNotForSaleItems>0)            ||
            (lUnknownItems>0)               ||
            (lPriceRequiredItems>0)         ||
            (lRestrictedNotAllowedItems>0)  ||
            (lTimeRestrictedItems>0)        ||
            (bSuspendFailure)               ||
            (lCouponsInAssist>0)            ||      //TAR#102019
            (lQuantityRestrictedItems>0)    ||
            (fOtherPayment)                 ||      // for outstanding approvals printing
            (lUnapprovedVoidedItems>0))
        {
            fReturnVal = false;
        }

        if (fCantScanCoupon || fRequestForHelp ||
            (bDropoffRequired && !bDropoffApproved) ||
            (bSuspendRequested && !bSuspendApproved) ||
            fUnDeActivatedItem 
            )
        {
            fReturnVal = false;
        }
        return fReturnVal;
}

//TAR341375
long SMStateBase::SendPrintDataToTB(CString *csAllData)
{
  long rc=0;
  CString csFileToPrint=_T("RPTDATA.CAS");
  SCOTDeleteFile(csFileToPrint);  //TAR 348946
  //put print data into file
  int nOpenMode = CFile::modeWrite| CFile::typeText | CFile::modeCreate;
  CStdioFile csFile;
  
  try
  {
    csFile.Open(csFileToPrint,nOpenMode);
  }
  catch( CFileException* e )
  {
    CString csError;
    csError.Format(_T("File Error:%s could not be opened. Cause:%s \n"), csFileToPrint, e->m_cause);
    trace(L0, _T("%s"), csError); // log file
    e->Delete();
    return rc;
  }
  try
  {
      CString &csData = *csAllData; //tar 378898
      csFile.WriteString(csData);
  }
  catch(CFileException *e)
  {
    CString csError;
    csError.Format(_T("File Error:%s during WriteSting(). Cause:%s \n"), csFileToPrint, e->m_cause);
    trace(L0, _T("%s"), csError); // log file
    e->Delete();
    return rc;
  }
  
  csFile.Close();
  
  //send file name to TB for printing
  int nFileToPrintLength = csFileToPrint.GetLength();
  LPTSTR szFileToPrint=csFileToPrint.GetBuffer(nFileToPrintLength);
  trace(L6, _T("+tb.Print, file name=%s"), szFileToPrint);
  rc = tb.Print(szFileToPrint);
  csFileToPrint.ReleaseBuffer(); //408230
  trace(L6, _T("-tb.Print"));
  return rc;
}


//+307389

void SMStateBase::CheckAndCleanStateStack()
{
    trace (L6, _T("+SMStateBase::CheckAndCleanStateStack"));
    long lDeviceClassesChecked[MAX_FIFO_STACK];
    long rc=0;
    int iStackInc=0;
    try{
        while (iStackInc < MAX_FIFO_STACK)
        {
            SMStateBase  *sReturnState = PopStateFifoStack();
            if(sReturnState != STATE_NULL)
            {
                CString csStateName = sReturnState->GetRuntimeClass()->m_lpszClassName;
                trace (L6, _T("popped state is %s"), csStateName);
                if(csStateName.Find(_T("SystemMessage")) != -1)
                {
                    long lDeviceClass;
                    if(csStateName.Find(_T("SystemMessage4")) != -1
                        || csStateName.Find(_T("SystemMessage5")) != -1
                        || csStateName.Find(_T("SystemMessage6")) != -1
                    )
                    {
                        lDeviceClass = ((SMSystemMessageOpPassBase *)sReturnState)->GetDeviceClass();
                    }
                    else if(csStateName.Find(_T("SystemMessage1")) != -1
                        || csStateName.Find(_T("SystemMessage2")) != -1
                        || csStateName.Find(_T("SystemMessage3")) != -1
                    )
                    {
                         lDeviceClass = ((SMSystemMessageBase *)sReturnState)->GetDeviceClass();
                    }
                    else 
                    {
                        lDeviceClass = ((SMSystemMessageBase *)sReturnState)->GetDeviceClass();
                    }

                    bool bSkip = false;
                    for (int i=0;i<MAX_FIFO_STACK;i++)
                    {
                        if (lDeviceClassesChecked[i] == lDeviceClass)
                        {
                            bSkip = true;
                            break;
                        }
                    }

                    if(!bSkip)
                    {

                        rc = DMCheckHealth(lDeviceClass); 
                        trace (L6, _T("checking device class %d returned %d"),lDeviceClass, rc);
                        lDeviceClassesChecked[iStackInc] = lDeviceClass;
                    }
                    else
                    {
                        rc=1;
                    }
                    
                    if (rc) 
                    {
                        ClearAllStatesButLast(lDeviceClass,sReturnState);
                    }
                    else
                    {
                        ClearAllStates(lDeviceClass,sReturnState);
                    }

                    

                }
                else
                {
                    PushStateFifoStack(sReturnState);
                }
            }
            iStackInc++;

        
        }
    }
    catch (...)
    {
        trace (L6, _T("CheckAndCleanStateStack did not complete"));
    }
    trace (L6, _T("-SMStateBase::CheckAndCleanStateStack"));

}

void SMStateBase::ClearAllStates(long lDeviceClass, SMStateBase *sReturnState)
{
    trace (L6, _T("+SMStateBase::ClearAllStates"));
    PrintFifoStack();
    int iStateInc = 0;
    try{
        //deleting original device state popped from stack
        delete getRealStatePointer(sReturnState);
        //delete remaining device states on stack
        while (iStateInc < MAX_FIFO_STACK)
        {
            if(sStateFifo[iStateInc] != STATE_NULL)
            {
                CString csStateName = sStateFifo[iStateInc]->GetRuntimeClass()->m_lpszClassName;
                if(csStateName.Find(_T("SystemMessage")) != -1)
                {
                    if(csStateName.Find(_T("SystemMessage4")) != -1
                        || csStateName.Find(_T("SystemMessage5")) != -1
                        || csStateName.Find(_T("SystemMessage6")) != -1
                    )
                    {
                        if(((SMSystemMessageOpPassBase *)sStateFifo[iStateInc])->GetDeviceClass() == lDeviceClass)
                        {
                            trace (L6, _T("setting state '%s' to STATE_NULL"),csStateName);
                            delete getRealStatePointer(sStateFifo[iStateInc]);
                            sStateFifo[iStateInc] = STATE_NULL;
                        }
                        
                    }
                    else if(csStateName.Find(_T("SystemMessage1")) != -1
                        || csStateName.Find(_T("SystemMessage2")) != -1
                        || csStateName.Find(_T("SystemMessage3")) != -1
                    )
                    {
                        if(((SMSystemMessageBase *)sStateFifo[iStateInc])->GetDeviceClass() == lDeviceClass)
                        {
                            trace (L6, _T("setting state '%s' to STATE_NULL"),csStateName);
                            delete getRealStatePointer(sStateFifo[iStateInc]);
                            sStateFifo[iStateInc] = STATE_NULL;
                        }
                        
                    }
                    else
                    {
                        if(((SMSystemMessageBase *)sStateFifo[iStateInc])->GetDeviceClass() == lDeviceClass)
                        {
                            trace (L6, _T("setting state '%s' to STATE_NULL"),csStateName);
                            delete getRealStatePointer(sStateFifo[iStateInc]);
                            sStateFifo[iStateInc] = STATE_NULL;
                        }
                    }
                    
                }
            }
            iStateInc++;
            
        }
    }
    catch (...)
    {
        trace (L6, _T("ClearAllStates did not complete"));
    }
    PrintFifoStack();
    trace (L6, _T("-SMState::ClearAllStates"));
}

SMStateBase *SMStateBase::ClearAllStatesButLast(long lDeviceClass, SMStateBase *sReturnState)
{
    trace (L6, _T("+SMStateBase::ClearAllStatesButLast"));
    PrintFifoStack();
    int iStateInc = 0;
    SMStateBase *sStateToKeep = sReturnState;
    try{
        
        while (iStateInc < MAX_FIFO_STACK)
        {
            if(sStateFifo[iStateInc] != STATE_NULL)
            {
                
                CString csStateName = sStateFifo[iStateInc]->GetRuntimeClass()->m_lpszClassName;
                if(csStateName.Find(_T("SystemMessage")) != -1)
                {

                    long lFifoClassRet;
                    if(csStateName.Find(_T("SystemMessage4")) != -1
                        || csStateName.Find(_T("SystemMessage5")) != -1
                        || csStateName.Find(_T("SystemMessage6")) != -1
                    )
                    {
                         lFifoClassRet = ((SMSystemMessageOpPassBase *)sStateFifo[iStateInc])->GetDeviceClass();
                    }
                    else if(csStateName.Find(_T("SystemMessage1")) != -1
                        || csStateName.Find(_T("SystemMessage2")) != -1
                        || csStateName.Find(_T("SystemMessage3")) != -1
                    )
                    {
                        lFifoClassRet = ((SMSystemMessageBase *)sStateFifo[iStateInc])->GetDeviceClass();
                    }
                    else
                    {
                        lFifoClassRet = ((SMSystemMessageBase *)sStateFifo[iStateInc])->GetDeviceClass();
                    }
                    if(lFifoClassRet == lDeviceClass)
                    {
                        delete getRealStatePointer(sStateToKeep);
                        sStateToKeep = sStateFifo[iStateInc];
                        trace (L6, _T("setting state to keep %s"),csStateName);
                        sStateFifo[iStateInc] = STATE_NULL;
                    
                    }
                    else
                    {
                        trace (L6, _T("Device Class did not match %d found, current class %d"),lFifoClassRet,lDeviceClass);
                    }
                }
            }
            iStateInc++;
        }
        
        trace (L6, _T("Pushing state to keep %s"),sStateToKeep->GetRuntimeClass()->m_lpszClassName);
        PushStateFifoStack(sStateToKeep);
    }
    catch (...)
    {
        trace (L6, _T("ClearAllStatesButLast did not complete"));
    }
    PrintFifoStack();
    trace (L6, _T("-SMState::ClearAllStatesButLast"));
    return sStateToKeep;
    
}


void SMStateBase::PrintFifoStack()
{
    trace (L7, _T("+SMState::PrintFifoStack"));
    int iStateInc = 0;
    try{
        while (iStateInc < MAX_FIFO_STACK)
        {
            if(sStateFifo[iStateInc] != STATE_NULL)
            {
                CString csStateName = sStateFifo[iStateInc]->GetRuntimeClass()->m_lpszClassName;
                trace (L7, _T("State in %i is %s - address is 0x%x - aStateFifo is %i and uStateFifo is %i"),iStateInc,csStateName,sStateFifo[iStateInc],aStateFifo,uStateFifo);
            }
            else
            {
                trace (L7, _T("State in %i is %s"),iStateInc,_T("STATE_NULL"));
            }
            iStateInc++;
            
        }
    }
    catch (...)
    {
        trace (L6, _T("PrintFifoStack did not complete"));
    }
    trace (L6, _T("-SMState::PrintFifoStack"));
}


//-307389


//TAR378451+
CString SMStateBase::GetPrinterCharSet()
{
    _TCHAR szCharSet[8];
    LONG   lRC;
    HKEY   hKey;
    DWORD  dwDataSize = sizeof(szCharSet);
    lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\DeviceManagerEx\\Devices\\Local\\Printer\\SCOTRec"), 
               0, KEY_QUERY_VALUE, &hKey);

    lRC = RegQueryValueEx(hKey, _T("DefaultCharacterSet"), NULL, NULL,(LPBYTE)szCharSet, &dwDataSize);
    ::RegCloseKey(hKey);
    
    CString csPrinterCharSet(_T(""));
    if ( lRC == ERROR_SUCCESS )
    {
        csPrinterCharSet = szCharSet;   
    }
    else
    {
        trace(L6, _T("SMSmRunReportsBase::GetPrinterCharSet() - query registry failed") );  
    }
    return csPrinterCharSet;
}

void SMStateBase::SendNewEscSeq2Printer()
{ 
  CString csEscSeq;
  if ( m_csPrinterCharSet == _T("858") || m_csPrinterCharSet == _T("437") || m_csPrinterCharSet == _T("CP865") )
  {
      csEscSeq.Format( _T("\x1b|3E\x1bR\x08")); //for code page 1252
      DMReceiptLine(csEscSeq); //Report requires the printer's code page to be set to 1252
  }
}

void SMStateBase::SendOrigEscSeq2Printer()
{  
  CString csEscSeq;
  if ( m_csPrinterCharSet == _T("437") )
  {
      csEscSeq.Format( _T("\x1b|3E\x1bR\x00")); //for code page 473
  }
  else if ( m_csPrinterCharSet == _T("858") )
  {
      csEscSeq.Format( _T("\x1b|3E\x1bR\x06")); //for code page 858
  }
  else if ( m_csPrinterCharSet == _T("CP865") ) //TAR389946
  {
      csEscSeq.Format( _T("\x1b|3E\x1bR\x05")); //for code page 865
  }
  DMReceiptLine(csEscSeq); //Set printer back to the original code page
}
//TAR378451-


void SMStateBase::DMConfigureScaleParams(long &divsize, long &constant)
{
    trace(L6, _T("+DMConfigureScaleParams"));
    LONG lRC;
    HKEY hKey; 

    CString MaxWeightKey(_T("MaxWeight"));
    CString DivisionSizeKey(_T("DivisionSize"));
    CString ConstantKey(_T("Constant"));
    CString MaxWeightTolKey(_T("MaxWeightTol"));
    LPCTSTR RegBaseKey(_T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\ScaleDisplay"));

    TCHAR MaxWeight[MAX_PATH + 1]={0};
    TCHAR DivisionSize[MAX_PATH + 1]={0};
    TCHAR Constant[MAX_PATH + 1]={0};
    TCHAR MaxWeightTol[MAX_PATH + 1]={0};
    DWORD optsz=0;

    lRC = RegOpenKeyEx(HKEY_LOCAL_MACHINE,RegBaseKey,  0, KEY_READ, &hKey);

    if (lRC==ERROR_SUCCESS)
    {
        optsz= MAX_PATH;
        //read the MaxWeight key from registry
        lRC=RegQueryValueEx(hKey, MaxWeightKey, NULL, NULL,  (BYTE*)&MaxWeight, &optsz);

        if (lRC==ERROR_SUCCESS)
        {
            //check if data less than or equal zero or if data is empty
            if((_tcstol(MaxWeight,NULL,10) <= 0) || (optsz == 2))
            {
                trace(L6,_T("Override value for MaximumWeight is invalid."));
            }
            else
            {
                //Convert unsigned short MaxWeight to long and assign to nDMMaximumWeight
                nDMMaximumWeight=_tcstol(MaxWeight,NULL,10);

                //Trace the Overriden value
                trace(L6, _T("Using MaximumWeight: [%d] obtained from registry.")
                    ,nDMMaximumWeight);
            }
        }
        else
        {
            trace(L6,_T("WARNING:  Could not access registry key [%s\\%s].")
                 ,RegBaseKey,MaxWeightKey);
        }
        optsz= MAX_PATH;
        //read the DivisionSize key from registry
        lRC = RegQueryValueEx(hKey, DivisionSizeKey, NULL, NULL, 
                        (BYTE*)&DivisionSize, &optsz); 
    
        if (lRC==ERROR_SUCCESS)
        {
            //check if data less than or equal zero or if data is empty
            if((_tcstol(DivisionSize,NULL,10) <= 0) || (optsz == 2))
            {
                trace(L6,_T("Override value for DivisionSize is invalid."));
            }
            else
            {
                //Convert unsigned short DivisionSize to long and assign to divsize
                divsize=_tcstol(DivisionSize,NULL,10);

                //Trace the Overriden value
                trace(L6, _T("Using DivisionSize: [%d] obtained from registry.")
                    ,divsize);
            }
        }
        else
        {
            trace(L6,_T("WARNING:  Could not access registry key [%s\\%s].")
                ,RegBaseKey,DivisionSizeKey);
        }
        optsz= MAX_PATH;
        //read the Constant key from registry
        lRC = RegQueryValueEx(hKey, ConstantKey, NULL, NULL, 
                    (BYTE*)&Constant, &optsz);
        if (lRC==ERROR_SUCCESS)
        {
            //check if data less than or equal zero or if data is empty
            if((_tcstol(Constant,NULL,10) <= 0) || (optsz == 2))
            {
                trace(L6,_T("Override value for Constant is invalid."));
            }
            else
            {
                //Convert unsigned short Constant to long and assign to constant
                constant=_tcstol(Constant,NULL,10);

                //Trace the Overriden value
                trace(L6, _T("Using Constant: [%d] obtained from registry."),constant);
            }
        }
        else
        {
            trace(L6,_T("WARNING:  Could not access registry key [%s\\%s].")
                ,RegBaseKey,ConstantKey);
        }
        optsz= MAX_PATH;
        //read the MaxWeightTol key from registry
        lRC = RegQueryValueEx(hKey, MaxWeightTolKey, NULL, NULL, 
                   (BYTE*)&MaxWeightTol, &optsz);
   
        if (lRC==ERROR_SUCCESS)
        {
            //check if data less than zero or if data is empty
            if((_tcstol(MaxWeightTol,NULL,10) < 0) || (optsz == 2))
            {   
                trace(L6,_T("Override value for MaxWeightTolerance is invalid."));
            }
            else
            {
                //Convert unsigned short MaxWeightTol to long and assign to nDMMaximumWeightTol
                nDMMaximumWeightTol=_tcstol(MaxWeightTol,NULL,10);

                //Trace the Overriden value
                trace(L6, _T("Using MaximumWeightTolerance: [%d] obtained from registry.")
                    ,nDMMaximumWeightTol);
            }
        }
        else
        {
            trace(L6,_T("WARNING:  Could not access registry key [%s\\%s].")
                 ,RegBaseKey,MaxWeightTolKey);
        }
        RegCloseKey(hKey);
    }  
    else
    {
        trace(L6,_T("WARNING:  Could not access registry key [%s]."),RegBaseKey);
    }

    trace(L6, _T("-DMConfigureScaleParams"));
}


//SR672 - Enhanced device error display

CString SMStateBase::DMGetDeviceErrorHTML(int DeviceClass, CString&  sDeviceModel, CString&  sDeviceSubModel,
                    int StatusEventId, long ExtendedResultCode, int nDataLength, void* sData, bool* bCheckHealth, int nContext, CString sLCID )
{
    CString csWork;
    csWork.Empty();
    //TCHAR sHTML[_MAX_PATH];

    trace(L6, _T("+SMStateBase::DMGetDeviceErrorHTML() DeviceClass=%d, Model=%s, SubModel=%s, Status/Error=%d, ExtCode=%d, Context=%d, LCID=%s"), 
        DeviceClass, sDeviceModel,sDeviceSubModel, StatusEventId, ExtendedResultCode, nContext, sLCID  );

    //+SSCOADK-836
    long lAmountDue = lChangeDue; // Any failed dispense amount.
    if (lAmountDue==0 && nDMLastCoinEvent>0)
	   lAmountDue = nDMLastCoinEvent;
    //-SSCOADK-836

    bool bErrorHTML = GetDeviceErrorHTML(/*sHTML*/csWork.GetBuffer(_MAX_PATH), DeviceClass, sDeviceModel.GetBuffer(_MAX_PATH), sDeviceSubModel.GetBuffer(_MAX_PATH), 
        StatusEventId, ExtendedResultCode, nDataLength, sData, bCheckHealth, nContext, sLCID.GetBuffer(_MAX_PATH) );
    
    //csWork = sHTML;

    sDeviceErrorTitle.Empty();
    GetCurrentTitleText(sDeviceErrorTitle.GetBuffer(_MAX_PATH));
    sDeviceErrorTitle.ReleaseBuffer();

    sDeviceErrorTitle2.Empty();
    GetCurrentTitle2Text(sDeviceErrorTitle2.GetBuffer(_MAX_PATH));
    sDeviceErrorTitle2.ReleaseBuffer();
    

    SMSystemMessageBase::g_csButtonText.Empty();
    GetCurrentButton1Text(SMSystemMessageBase::g_csButtonText.GetBuffer(_MAX_PATH));
    SMSystemMessageBase::g_csButtonText.ReleaseBuffer();

    sDeviceModel.ReleaseBuffer();
    sDeviceSubModel.ReleaseBuffer();
    sLCID.ReleaseBuffer();
    csWork.ReleaseBuffer();

    if (!csWork.IsEmpty())
        trace(L6, _T("Device error html: %s"), csWork);


    //Tar 447941
    if (DeviceClass == DMCLASS_CASHCHANGER || DeviceClass == DMCLASS_COINACCEPTOR || DeviceClass == DMCLASS_CASHACCEPTOR )
    {
        CString sParms;
        sParms.Empty();
        if (sDeviceModel.Find(_COINRECYCLER_BCR) != -1)
            sParms = FormatDeviceInfoInput(ExtendedResultCode);
        else if (sDeviceModel.Find(_CASHDISPENSER_BNR) != -1)
            sParms = FormatDeviceInfoInput(ExtendedResultCode);
        else if (sDeviceModel.CompareNoCase(_COINDISPENSER_TELEQUIPHOPPER) == 0 )   
        {
            if (ExtendedResultCode == NCR_STATUS_HOPPER_JAM)
            {
                CString sHopperStatusLine = getHopperStatusString();
                if (!sHopperStatusLine.IsEmpty())
                            sHopperStatusLine = sHopperStatusLine + _T("\n");
  
                sParms = sHopperStatusLine ;
            }

            if( ExtendedResultCode >= NCR_ECASH_JAMMED_BIN_1 &&  
                ExtendedResultCode <= NCR_ECASH_JAMMED_BIN_8 )
            {
                CString sHopperStatusLine = getHopperStatusString();

                // Check if startup error
                if(IsCashDeviceError(dm.m_nStartDeviceErrorDeviceClass))
                {
                    sParms.Format(_T("%s"), sHopperStatusLine );
                }
                else
                {
                    sParms.Format(_T("%s&%s"), ps.FmtDollar(lChangeDue),sHopperStatusLine );
                }
            }
        }
        else if (sDeviceModel.Find(_T("F53")) != -1  || sDeviceModel.Find(_CASHDISPENSER_563X) != -1 
            || sDeviceModel.Find(_CASHDISPENSER_5633) != -1 || sDeviceModel.Find(_T("ESP")) != -1)    // SSCOP-3348
        {
            sParms = Get563xCassetteParms(ExtendedResultCode,nContext);     
        }
        if (csWork.Find(_T("1003")) != -1 ||  csWork.Find(_T("1004")) != -1 || csWork.Find(_T("1005")) != -1 )
        {
            CString sHopperStatusLine = getHopperStatusString();
            sParms.Format(_T("%s&%s&%s"), ps.FmtDollar(lAmountDue), ps.FmtDollar(lAmountDue), sHopperStatusLine ); ////SSCOADK-836
        }

          if ( !sParms.IsEmpty() && !csWork.IsEmpty() && csWork.Find(_T("DeviceErrorUnknown"))== -1 )
          {
                TCHAR sHTML2[_MAX_PATH];
                _tcscpy(sHTML2, csWork.GetBuffer(_MAX_PATH));
                ParseDeviceErrorHTML(sHTML2, sParms.GetBuffer(_MAX_PATH), _T("%s"));
                csWork.ReleaseBuffer();
                csWork = sHTML2;
                sParms.ReleaseBuffer();
          }
         
    }
        

        //Tar 448083
    sDeviceErrorMessageText.Empty();
    int nBufferLength = GetCurrentMessageTextLength();
    if (nBufferLength > 0)
    {
        GetCurrentMessageText(sDeviceErrorMessageText.GetBuffer(nBufferLength+1));
        sDeviceErrorMessageText.ReleaseBuffer();
    }
    

    trace(L6, _T("-SMStateBase::DMGetDeviceErrorHTML()"));
    return csWork;
}

//SR672 - Enhanced device error display
CString SMStateBase::Get563xCassetteParms(long lcode , int nContext)
{
    trace(L6, _T("+Get563xCassetteParms(%ld,%d)"), lcode, nContext);
              int nCashChangeCassOutNumber=0, nCashChangePickFailureNumber=0, nCashChangePickSensorNumber=0;
                
            
              CString sParms;
              sParms.Empty();

			  //Numeric error codes are for NCR_ESP support
              //Get Cass Out type 1, 2, 3, or 4
              if(lcode == NCR_E5633_CASS_OUT_ONE || lcode == 101) //Get type 1, 2, 3, or 4
                  nCashChangeCassOutNumber = 1;
              else if(lcode == NCR_E5633_CASS_OUT_TWO || lcode == 102)
                  nCashChangeCassOutNumber = 2;
              else if(lcode == NCR_E5633_CASS_OUT_THREE || lcode == 103)
                  nCashChangeCassOutNumber = 3;
              else if(lcode == NCR_E5633_CASS_OUT_FOUR || lcode == 104)
                  nCashChangeCassOutNumber = 4;

              //Get Pick failure type 1, 2, 3, or 4
              else if(lcode == NCR_E5633_FEED_FAILURE_ONE || lcode == 251 || lcode == 301) //Get type 1, 2, 3, or 4
                  nCashChangePickFailureNumber = 1;
              else if(lcode == NCR_E5633_FEED_FAILURE_TWO || lcode == 252 || lcode == 302)
                  nCashChangePickFailureNumber = 2;
			  else if(lcode == NCR_E5633_FEED_FAILURE_THREE || lcode == 253 || lcode == 303)
                  nCashChangePickFailureNumber = 3;
              else if(lcode == NCR_E5633_FEED_FAILURE_FOUR || lcode == 254 || lcode == 304)
                  nCashChangePickFailureNumber = 4;
              
              //Get Pick failure type 1, 2, 3, or 4
              else if(lcode == NCR_E5633_PICK_SENSOR_ONE || lcode ==  401 || lcode == 501 || lcode == 506 || lcode == 4901) //Get type 1, 2, 3, or 4
                  nCashChangePickSensorNumber = 1;
              else if(lcode == NCR_E5633_PICK_SENSOR_TWO || lcode == 402 || lcode ==  502 || lcode == 507 || lcode == 4902)
                  nCashChangePickSensorNumber = 2;
              else if(lcode == NCR_E5633_PICK_SENSOR_THREE || lcode == 403 || lcode ==  503 || lcode == 508 || lcode == 4903)
                  nCashChangePickSensorNumber = 3;
              else if(lcode == NCR_E5633_PICK_SENSOR_FOUR || lcode == 404 || lcode ==  504 || lcode == 509 || lcode == 4904)
                  nCashChangePickSensorNumber = 4;




            if(nCashChangeCassOutNumber)  // Process cass type 1, 2, 3, 4 out, need get $ amount 
            {
                //$DMCashChanger_1137$ and $DMCashChanger_1117$
                sParms.Format(_T("%s&%s&%s"), ps.FmtDollar(dm.m_nBillDispenserType[nCashChangeCassOutNumber-1]), ps.FmtDollar(dm.m_nBillDispenserType[nCashChangeCassOutNumber-1]), ps.FmtDollar(dm.m_nBillDispenserType[nCashChangeCassOutNumber-1]));
                nCashChangeCassOutNumber = 0; //Reset
            }
            else if(nCashChangePickFailureNumber)
            {

                sParms.Format(_T("%s&%s&%s&%s"), ps.FmtDollar(lChangeDue), ps.FmtDollar(dm.m_nBillDispenserType[nCashChangePickFailureNumber-1]), ps.FmtDollar(dm.m_nBillDispenserType[nCashChangePickFailureNumber-1]),ps.FmtDollar( dm.m_nBillDispenserType[nCashChangePickFailureNumber-1]));
                nCashChangePickFailureNumber = 0; //Reset
            }
            else if(nCashChangePickSensorNumber)
            {
                if(nContext == 0) 
                    sParms.Format(_T("%s&%s"), ps.FmtDollar(dm.m_nBillDispenserType[nCashChangePickSensorNumber-1]), ps.FmtDollar(dm.m_nBillDispenserType[nCashChangePickSensorNumber-1]));
                else
                    sParms.Format(_T("%s&%s&%s"), ps.FmtDollar(lChangeDue), ps.FmtDollar(dm.m_nBillDispenserType[nCashChangePickSensorNumber-1]), ps.FmtDollar(dm.m_nBillDispenserType[nCashChangePickSensorNumber-1]));
                
                nCashChangePickSensorNumber = 0; //Reset
            }

            if (lcode == NCR_E5633_BILL_JAM ||lcode == NCR_E5633_PURGE_OUT || lcode == NCR_E5633_TIMING_WHEEL_FAILED ||
                lcode == NCR_E5633_EXIT_SENSOR || lcode == NCR_E5633_DIVERTER_ERROR || lcode == 600 || lcode == 601 || lcode == 1800 || lcode == 4906 ||
                lcode == 700 || lcode == 800 || lcode == 6200 )
                {
                    sParms.Format(_T("%s"), ps.FmtDollar(lChangeDue));
                }

            if (!sParms.IsEmpty())
                trace(L6, _T("-Get563xCassetteParms() =%s"), sParms);
            else
                trace(L6, _T("-Get563xCassetteParms()"));
            return sParms;

}

long SMStateBase::DMConfigureScaleDisplay(void)
{
    trace(L6, _T("+DMConfigureScaleDisplay"));
    CScaleWrapper *pDevice = NULL;
    long wgtunit = 0;       // Scale weight units.
    long constant = 20;     // Constant to calculate min.
    long divsize = 10;      // Scale division size.
    const long DIV_SIZE_TOLERANCE = 9;  // Multiplier used to calculate max tolerance.
    
    CString stxt_cm;        // Format string for CMScaleImage.
    CString stxt_sm;        // Format string for SMScaleImage.
    CString simage_txt;

    float ds;               // Calculate displayed division size.
    float maxw;             // Calculate displayed maximum wght.
    float minw;             // Calculate displayed minimum wght.
    float nmax;             // Calculate displayed Nmax.
    

    //TAR 439226 - use scotopts value to set weight unit if scale not configured
    //             Start with some sane values no matter what.
    if(co.fOperationsScaleMetric)
    {               
        ps.SetWeightUnits(ps.GetPSText(SCALE_KG));
    }
    else
    {   
        ps.SetWeightUnits(ps.GetPSText(SCALE_LB));
    }
    //end 439226

    if(! co.fStateDisplayWeightsAndMeasures || 
        !dm.IsDeviceConfigured(DMCLASS_SCALE))
    {
        trace(L6,_T("-DMConfigureScaleDisplay:  Weights&Measures display disabled or no scanner/scale configured. "));
        return -1;
    }
    
    pDevice = dm.GetScaleWrapper();

    if(! pDevice)
    {
        // Trace error.
        trace(L6,_T("-DMConfigureScaleDisplay:  Device Error. "));
        return MSG_SCANNER_SCALE_ERROR;
    }
    
    nDMMaximumWeight = pDevice->GetMaximumWeight();
    wgtunit = pDevice->GetWeightUnit();
   
    switch(wgtunit)
    {       
    case SCAL_WU_KILOGRAM:
        //SSCOADK-5989 + reformat weights and measures string
        if ((co.fStateDualLanguage && m_customerLanguage != SCOT_LANGUAGE_PRIMARY && 
            !storeMode() && !(fStateAssist || co.IsInAssistMenus())) || g_bCustomerLanguageChanged)
        {
            stxt_cm = ps.GetPSText(MSG_CM_KGSCALEIMAGE, m_customerLanguage);
        }
        else
        {
            stxt_cm = ps.GetPSText(MSG_CM_KGSCALEIMAGE, SCOT_LANGUAGE_PRIMARY);
        }

        stxt_sm = ps.GetPSText(MSG_SM_KGSCALEIMAGE, SCOT_LANGUAGE_PRIMARY);
        // If co.fOperationsScaleMetric is false, then log an error to the 
        //        event log.
        if(! co.fOperationsScaleMetric)
        {
            ScotError(RETURN,SM_CAT_CONFIGURATION, SCALEMETRIC_ERROR1);
        }
        divsize = 5;                // 0.005 kg
        if ((co.fStateDualLanguage && m_customerLanguage != SCOT_LANGUAGE_PRIMARY && 
            !storeMode() && !(fStateAssist || co.IsInAssistMenus())) || g_bCustomerLanguageChanged)
        {
            ps.SetWeightUnits(ps.GetPSText(SCALE_KG, m_customerLanguage)); //SSCOADK-5989
        }
        else
        {
            ps.SetWeightUnits(ps.GetPSText(SCALE_KG, SCOT_LANGUAGE_PRIMARY)); 
        }
        break;
    case SCAL_WU_POUND:
        //SSCOADK-5989 + reformat weights and measures string
        if ((co.fStateDualLanguage && m_customerLanguage != SCOT_LANGUAGE_PRIMARY && 
            !storeMode() && !(fStateAssist || co.IsInAssistMenus())) || g_bCustomerLanguageChanged)
        {
            stxt_cm = ps.GetPSText(MSG_CM_SCALEIMAGE, m_customerLanguage);
        }
        else
        {
            stxt_cm = ps.GetPSText(MSG_CM_SCALEIMAGE, SCOT_LANGUAGE_PRIMARY);
        }

        stxt_sm = ps.GetPSText(MSG_SM_SCALEIMAGE, SCOT_LANGUAGE_PRIMARY);
        // If co.fOperationsScaleMetric is true, then log an error to the 
        //        event log.
        if(co.fOperationsScaleMetric)
        {
            ScotError(RETURN,SM_CAT_CONFIGURATION, SCALEMETRIC_ERROR2);
        }
        divsize = 10;               // 0.01 lb
        if ((co.fStateDualLanguage && m_customerLanguage != SCOT_LANGUAGE_PRIMARY && 
            !storeMode() && !(fStateAssist || co.IsInAssistMenus())) || g_bCustomerLanguageChanged)
        {
            ps.SetWeightUnits(ps.GetPSText(SCALE_LB, m_customerLanguage)); //SSCOADK-5989
        }
        else
        {
            ps.SetWeightUnits(ps.GetPSText(SCALE_LB, SCOT_LANGUAGE_PRIMARY)); 
        }

        break;
    case SCAL_WU_OUNCE:
    case SCAL_WU_GRAM:
    default:
        trace(L0, _T("ERROR:  Unsupported weight unit [%d]"), wgtunit);
        return MSG_SCANNER_SCALE_WTUNIT_UNSUPPORTED;
    }

    nDMMaximumWeightTol = DIV_SIZE_TOLERANCE * divsize;

    // Trace nDMMaximumWeight, nDMMaximumWeightTol, wgtunit, divsize, 
            //               constant etc. ....
    trace(L6,_T("MaximumWeight: [%d]"),nDMMaximumWeight);
    trace(L6,_T("MaximumWeightTolerance: [%d]"),nDMMaximumWeightTol);
        trace(L6,_T("WeightUnit: [%d]"),wgtunit);
    trace(L6,_T("DivisionSize: [%d]"),divsize);
    trace(L6,_T("Constant: [%d]"),constant);

    DMConfigureScaleParams(divsize, constant);  // Check for registry overrides.

    // Format display strings for store and customer mode.
    ds = 1.0*divsize/1000.0;
    maxw = 1.0*nDMMaximumWeight/1000.0;
    minw = 1.0*(constant*divsize)/1000.0;
    nmax = nDMMaximumWeight/divsize;

    stxt_cm = ps.ConvertToEscapeSequence(stxt_cm, _T("\\n"), _T("\n"));
    simage_txt.Format(stxt_cm, ds, maxw, minw, nmax);
    if( IsRCError(
        m_pAppPSX->SetTransactionVariable( _T("CMScaleText"), simage_txt)) )
    {
        // Trace error.
        trace(L6,_T("-DMConfigureScaleDisplay:  Error Setting Transaction Variable in CMScaleText. "));
        return MSG_SCANNER_SCALE_WTPARAM_FAILURE;
    }

    stxt_sm.Format(_T("           %s"), stxt_sm);
    stxt_sm = ps.ConvertToEscapeSequence(stxt_sm, _T("\\n"), _T("\n"));
    simage_txt.Format(stxt_sm, ds, maxw, minw, nmax);
    
    if( IsRCError(
        m_pAppPSX->SetTransactionVariable( _T("SMScaleText"), simage_txt)) )
    {
        // Trace error.
        // Store mode failure is not as severe as customer mode, so may not need
        // to return false here.
        trace(L6,_T("-DMConfigureScaleDisplay:  Error Setting Transaction Variable in SMScaleText. "));
    }
    
    //SSCOADK-5989 +
    if (co.fStateDualLanguage)
    {
        DMScale(); 
        g_bCustomerLanguageChanged = false; 
    }
    //SSCOADK-5989 -

    trace(L6, _T("-DMConfigureScaleDisplay"));
    return -1;
}

CString SMStateBase::FormatDeviceInfoInput(long lErrorCode)
{
    CString sParm;
    sParm.Empty();

    //+SSCOADK-836
    long lAmountDue = lChangeDue; // Any failed dispense amount.
    if (lAmountDue==0 && nDMLastCoinEvent>0) 
	  lAmountDue = nDMLastCoinEvent;
    //-SSCOADK-836

    //BCR errors
    if (lErrorCode == NCR_ECOINDISP_PAYOUT_HOPPER_1 ||
        lErrorCode == NCR_ECOINDISP_PAYOUT_HOPPER_2 ||
        lErrorCode == NCR_ECOINDISP_PAYOUT_HOPPER_3 ||
        lErrorCode == NCR_ECOINDISP_PAYOUT_HOPPER_4 ||
        lErrorCode == NCR_ECOINDISP_PAYOUT_HOPPER_5 ||
        lErrorCode == NCR_ECOINDISP_PAYOUT_HOPPER_6 )
    {
        //Fill $DMBCR_262_267_T2$ and $DMBCR_262_267_TEXT$ - 2 %s
        CString sDenom=getHopperDenominations(lErrorCode);
        CString sHopperStatusLine = getHopperStatusString();
        sParm.Format(_T("%s&%s"),sDenom, sHopperStatusLine);        

        return sParm;
    }
    else if (lErrorCode == NCR_ECOINDISP_LEVEL_HOPPER_1 ||
        lErrorCode == NCR_ECOINDISP_LEVEL_HOPPER_2 ||
        lErrorCode == NCR_ECOINDISP_LEVEL_HOPPER_3 ||
        lErrorCode == NCR_ECOINDISP_LEVEL_HOPPER_4 ||
        lErrorCode == NCR_ECOINDISP_LEVEL_HOPPER_5 ||
        lErrorCode == NCR_ECOINDISP_LEVEL_HOPPER_6 
    )
    {   
        //$DMBCR_268_273_T2$
        CString sDenom=getHopperDenominations(lErrorCode);
        sParm.Format(_T("%s"),sDenom);
        return sParm;

    }
    else if (lErrorCode == NCR_ECOINDISP_OVERCUR_HOPPER_1 ||
        lErrorCode == NCR_ECOINDISP_OVERCUR_HOPPER_2 ||
        lErrorCode == NCR_ECOINDISP_OVERCUR_HOPPER_3 ||
        lErrorCode == NCR_ECOINDISP_OVERCUR_HOPPER_4 ||
        lErrorCode == NCR_ECOINDISP_OVERCUR_HOPPER_5 ||
        lErrorCode == NCR_ECOINDISP_OVERCUR_HOPPER_6 )
    {
        //$DMBCR_307_312_T2$ and $DMBCR_307_312_TEXT$
        CString sDenom=getHopperDenominations(lErrorCode);
        sParm.Format(_T("%s&%s&%s"),sDenom, sDenom,sDenom);
        return sParm;
    }
    //BNR errors
    else if (lErrorCode == BUNDLER_JAM ||
        lErrorCode == BUNDLER_ERROR ||
        lErrorCode == BUNDLER_FULL ||
        lErrorCode == MAIN_MODULE_OUTLET_JAM ||
        lErrorCode == MAIN_MODULE_BOTTOM_TRANSPORT_JAM )
    {
        sParm.Format(_T("%s&%s"), ps.FmtDollar(lAmountDue),ps.FmtDollar(lAmountDue)); //SSCOADK-836
        return sParm;
    }
    else if (lErrorCode == RECYCLER3_STOR_EMPTY ||
        lErrorCode == RECYCLER4_STOR_EMPTY ||
        lErrorCode == RECYCLER5_STOR_EMPTY ||
        lErrorCode == RECYCLER6_STOR_EMPTY)
    {
        CString sDenom=getHopperDenominations(lErrorCode);
        sParm.Format(_T("%s&%s&%s"), ps.FmtDollar(lAmountDue),sDenom, sDenom); //SSCOADK-836
        return sParm;
    }
    return sParm;

}
//+SR742 //Tar 450827  
SMStateBase  *SMStateBase::DisplayCashDeviceError(void)
{
    long lCode = 0;
    //Tar 410364
     //display cash acceptor error
     if (dm.IsDeviceConfigured(DMCLASS_CASHACCEPTOR,0)  )
     {
            lCode = dm.GetCashAcceptorDeviceStatus();

            // Sure, it SAYS it's OK, but it's not to be trusted!
            // (i.e. not all errors reported by above function are correct at this time.)
            //SR672+
            if (lCode == BILL_STATUS_OK)
            {
                if ( co.IsDegradedModeOn() && dm.GetDegradedModeType() == DEGRADEDMODE_CASH_ONLY ) //SSCOP-2255
                    lCode = lastDMCashAcceptorCode;
            }
            if (lCode == BILL_STATUS_TAMPER_OUT ||
                lCode == BILL_STATUS_FAILURE    ||
                lCode == BILL_STATUS_JAM ||
                lCode == BILL_STATUS_FULL  ||
                lCode == BILL_STATUS_CHEAT)
            //SR672-
           {
              mo.PostDM(DM_ERROR, DMCLASS_CASHACCEPTOR, 0, lCode, NULL);
              return STATE_NULL;
           }
     }
     else if (dm.IsDeviceConfiguredNotLoaded(DMCLASS_CASHACCEPTOR,0))
     {
        SMStateBase *pRet = GetUnloadedCashAcceptorError();                
        if(pRet != STATE_NULL)
        {
            return pRet;
        }
     }

       //display coin acceptor
     if (dm.IsDeviceConfigured(DMCLASS_COINACCEPTOR,0) )
     {
           lCode = dm.GetCoinAcceptorDeviceStatus();
           
            // +AR418069 If the current status is inoperable (according to the DM property)
            // then it could be for a lot of reasons.  The Coin Acceptor spec says
            // it will set the property to inoperable for lots of things, but the last
            // status event will tell us what the real reason is.  So we will get the
            // last event.
           //SR672+
           if (   lCode == COIN_STATUS_INOPERABLE ) //TAR418069
           {
                lCode = lastDMCoinAcceptorCode;
           }

           // +SSCOP-3372:  Remap TAMPER_OUT to JAM because
           //                TAMPER_OUT is ignored.
           if(lCode == DM_COINACCEPTOR_STATUS_TAMPER_OUT)
           {
               lCode = DM_COINACCEPTOR_STATUS_JAM;
           }
           // -SSCOP-3372:
           
           if(lCode == COIN_STATUS_JAM ||
               lCode == COIN_STATUS_FAILURE ||
               lCode == COIN_STATUS_FULL ||
               lCode == COIN_STATUS_TAMPER_OUT)
            //SR672-
           {
              mo.PostDM(DM_ERROR, DMCLASS_COINACCEPTOR, 0, lCode, NULL);
              return STATE_NULL;
           }
     }
     else if (dm.IsDeviceConfiguredNotLoaded(DMCLASS_COINACCEPTOR,0))
     {
        SMStateBase *pRet = GetUnloadedCoinAcceptorError();
        if(pRet != STATE_NULL)
        {
            return pRet;
        }
     }
    
   //bool bChanger = IsDeviceConfigured(DMCLASS_CASHCHANGER,0);
   //display coin/bill dispenser error
   if(dm.m_csCashRecyclerModel.IsEmpty())
   {
       //SR672
      CString csWork, s1, s2(_T(""));
      bool bLocalCheckHealth = false;
      

      if (dm.IsDeviceConfigured(DMCLASS_CASHCHANGER,0))
      {
          long lCheckCashChangerError = dm.CheckHealthOnlyWithLevel(DMCLASS_CASHCHANGER, 0, 1);
          if(lCheckCashChangerError)
          {
             trace(L6,_T("Try calling checkhealth lvl 2. Level 1 might have overlooked that the device was already fixed."));
             lCheckCashChangerError = dm.CheckHealth(DMCLASS_CASHCHANGER);    //SSCOP-416           

             if(lCheckCashChangerError)
             {
                 lCheckCashChangerError = 0;
                 //mo.PostDM(DM_ERROR, DMCLASS_CASHCHANGER, 0, EVENTID_ANY, NULL);

                 //Tar 446431
                 long lcode = dm.GetResultCodeExtended(DMCLASS_CASHCHANGER, 0 );
                 s1=dm.GetDeviceModelName(DMCLASS_CASHCHANGER, 0);

                 csWork = DMGetDeviceErrorHTML(DMCLASS_CASHCHANGER, s1, s2, EVENTID_ANY, lcode, 0, NULL, &bLocalCheckHealth, CONTEXT_ANY, co.csPrimaryLanguageCode );
                 if (csWork.Find(_T("Ignore")) !=-1)
                        return STATE_NULL;

                 //CString csWork = GetDeviceErrorMnemonic(DMCLASS_CASHCHANGER, dm.GetDeviceName(DMCLASS_CASHCHANGER, 0), EVENTID_ANY, dm.GetResultCodeExtended(DMCLASS_CASHCHANGER, 0 ) , 0 );
                  return createLevel1Message(csWork,            // char *message
                                       PS_NOTEXT,         // no canned message
                                       PS_NOTEXT,         // no prompt
                                       0,                 // no prompt value
                                       DMCLASS_CASHCHANGER,       // -1 = no device
                                       true,        // whether DM check health needed
                                       true);       //use web browser
             }
          }

      }
      else
      {
          return GetUnloadedCashChangerError();
      }
   }
   //diplay recycler error
   else if(dm.m_csCashRecyclerModel == _CASHDISPENSER_GLORY || 
                     dm.m_csCashRecyclerModel == _CASHDISPENSER_SAFEPAY )
   {
      CString csCheckHealthText = dm.CashRecyclerGetCheckHealthText();
      if( (csCheckHealthText.Find(_T(":Full")) != -1 ) || (csCheckHealthText.Find(_T(":Jam")) != -1 ))
      {
         mo.PostDM(DM_ERROR, DMCLASS_CASHRECYCLER, 0, CHAN_STATUS_JAM, NULL);
         return STATE_NULL;
      }
   }
      
  bCashDeviceErrorToDispaly = false;  //415172
  return STATE_NULL;
}
//-SR742  

CString SMStateBase::GetCashChangerError(void)
{
    trace(L6, _T("SMStateBase::GetCashChangerError"));

    CString csSubModel, csModelName, csWork;
    bool bCoin = false;
    bool bBill = false;
    bool bCheckHealth = false;
    long lCode;
  
    BillOrCoinErrorStatus(&bCoin, &bBill);
      
    if(bCoin)
        csSubModel = _T("Coin");
    if(bBill)
        csSubModel = _T("Bill");

    lCode = dm.GetResultCodeExtended(DMCLASS_CASHCHANGER, 0, false);
    csModelName = dm.GetDeviceModelName(DMCLASS_CASHCHANGER, 0, csSubModel);
    csWork = DMGetDeviceErrorHTML(DMCLASS_CASHCHANGER, csModelName, csSubModel, EVENTID_ANY, 
        lCode, 0, NULL, &bCheckHealth, 0, co.csPrimaryLanguageCode );

    //+TAR 398739
    if (lCode == NCR_E5633_FEED_FAILURE || 
        lCode == NCR_E5633_FEED_FAILURE_ONE || 
        lCode == NCR_E5633_FEED_FAILURE_TWO || 
        lCode == NCR_E5633_FEED_FAILURE_THREE || 
        lCode == NCR_E5633_FEED_FAILURE_FOUR)
    {
        DMCheckCashCounts(false);
        if(co.fLocaleCashManagementScreen && 
            dm.GetBillsVeryLow())
        {
            csWork = DMGetDeviceErrorHTML(DMCLASS_CASHCHANGER, csModelName, csSubModel, 
                EVENTID_ANY, 1003, 0, NULL, &bCheckHealth, 0, co.csPrimaryLanguageCode );
        }
    }
    //-TAR 398739

    return csWork;
}

// Add Glory CM Support
void SMStateBase::SetCashDiscrepancyStatus(bool bDiscrepancy)
{
    trace(L7, _T("SMStateBase::SetCashDiscrepancyStatus [%d]"), bDiscrepancy);

    m_bDiscrepancy = bDiscrepancy;
}


/**
 * SSCOP-4169:  Break DisplayCashDeviceError() into smaller methods so that
 *              errors for individual devices can be determined.
 */
SMStateBase *SMStateBase::GetUnloadedCashAcceptorError(void)
{
     if (dm.IsDeviceConfiguredNotLoaded(DMCLASS_CASHACCEPTOR,0))
     {
          CString csWork, csHTML;
          csWork = ps.GetPSText(MSG_DEVICESERROR); // code from below replaced here
          csWork += _T("\n") + dm.GetDescription( DMCLASS_CASHACCEPTOR, 0 );

          //SR672
          CString sSubModel = _T("");
          bool bLocalCheckHealth=false;
          long lExtCode=dm.GetResultCodeExtended(DMCLASS_CASHACCEPTOR, 0);
          CString sModel = dm.GetDeviceModelName(DMCLASS_CASHACCEPTOR, 0);
          csHTML = DMGetDeviceErrorHTML(DMCLASS_CASHACCEPTOR, sModel, sSubModel, EVENTID_ANY, lExtCode, 0, NULL, &bLocalCheckHealth, 0, co.csPrimaryLanguageCode );
            
                
            if (!csHTML.IsEmpty() && !csWork.IsEmpty())
            {        
                UpdateDeviceErrorHTMLText(csHTML.GetBuffer(_MAX_PATH), csWork.GetBuffer(_MAX_PATH));
                csWork.ReleaseBuffer();        
                csHTML.ReleaseBuffer();
            }
          
          return createLevel1Message(csHTML,            // char *message
                               PS_NOTEXT,         // no canned message
                               PS_NOTEXT,         // no prompt
                               0,                 // no prompt value
                               DMCLASS_CASHACCEPTOR,          // -1 = no device
                               true,  // whether DM check health needed
                               true);  //display html      
     }

     return STATE_NULL;
}


/**
 * SSCOP-4169:  Break DisplayCashDeviceError() into smaller methods so that
 *              errors for individual devices can be determined.
 */
SMStateBase *SMStateBase::GetUnloadedCoinAcceptorError(void)
{
    if (dm.IsDeviceConfiguredNotLoaded(DMCLASS_COINACCEPTOR,0))
    {
        //SR672
        CString csWork,sParm, s1, s2(_T(""));
        bool bLocalCheckHealth = false;
        long lExtCode = dm.GetResultCodeExtended(DMCLASS_COINACCEPTOR, 0);
        s1=dm.GetDeviceModelName(DMCLASS_COINACCEPTOR, 0);

        csWork = DMGetDeviceErrorHTML(DMCLASS_COINACCEPTOR, s1, s2, EVENTID_ANY, lExtCode, 0, NULL, &bLocalCheckHealth, CONTEXT_ANY, co.csPrimaryLanguageCode );

         /*  
         
          CString csWork = GetDeviceErrorMnemonic(DMCLASS_COINACCEPTOR, dm.GetDeviceName(DMCLASS_COINACCEPTOR, 0),
                            lastDMCoinAcceptorCode,dm.GetResultCodeExtended(DMCLASS_COINACCEPTOR, 0), 0);
                            */

          if( csWork.IsEmpty() || csWork.Find(_T("DeviceErrorUnknown")) != -1)
          {
                  sParm = ps.GetPSText(MSG_DEVICESERROR); // code from below replaced here
                sParm += _T("\n") + dm.GetDescription( DMCLASS_COINACCEPTOR, 0 );

                if (!csWork.IsEmpty() && !sParm.IsEmpty())
                {                
                    UpdateDeviceErrorHTMLText(csWork.GetBuffer(_MAX_PATH), sParm.GetBuffer(_MAX_PATH));
                    csWork.ReleaseBuffer();                
                    sParm.ReleaseBuffer();
                }                

          }
          return createLevel1Message(csWork,      // char *message
                               PS_NOTEXT,         // no canned message
                               PS_NOTEXT,         // no prompt
                               0,                 // no prompt value
                               DMCLASS_COINACCEPTOR,          // -1 = no device
                               true, // whether DM check health needed
                               true);        //display html

    }

    return STATE_NULL;
}

/**
 * SSCOP-4169:  Break DisplayCashDeviceError() into smaller methods so that
 *              errors for individual devices can be determined.
 */
SMStateBase *SMStateBase::GetUnloadedCashChangerError(void)
{
    CString csWork, s1, s2(_T(""));
    bool bLocalCheckHealth = false;
    /*
    CString csWork = GetDeviceErrorMnemonic(DMCLASS_CASHCHANGER, dm.GetDeviceName(DMCLASS_CASHCHANGER, 0),
                    EVENTID_ANY,dm.GetResultCodeExtended(DMCLASS_CASHCHANGER, 0), 0);  
            */
    long lExtCode = dm.GetResultCodeExtended(DMCLASS_CASHCHANGER, 0 );
    s1=dm.GetDeviceModelName(DMCLASS_CASHCHANGER, 0);    
    csWork = DMGetDeviceErrorHTML(DMCLASS_CASHCHANGER, s1, s2, EVENTID_ANY, lExtCode, 0, NULL, &bLocalCheckHealth, CONTEXT_ANY, co.csPrimaryLanguageCode );
    if (csWork.Find(_T("Ignore")) !=-1)
                return STATE_NULL;

    CString sParms;
    if( csWork.Find(_T("DeviceErrorUnknown")) != -1)
    {
        sParms = ps.GetPSText(MSG_DEVICESERROR); // code from below replaced here
        sParms += _T("\n") + dm.GetDescription( DMCLASS_CASHCHANGER, 0 );
        
        if (!csWork.IsEmpty() && !sParms.IsEmpty())
        {                
            UpdateDeviceErrorHTMLText(csWork.GetBuffer(_MAX_PATH), sParms.GetBuffer(_MAX_PATH));
            csWork.ReleaseBuffer();                
            sParms.ReleaseBuffer();
        }

    }
    return createLevel1Message(csWork,            // char *message
                       PS_NOTEXT,         // no canned message
                       PS_NOTEXT,         // no prompt
                       0,                 // no prompt value
                       DMCLASS_CASHCHANGER,          // -1 = no device
                       true, true);        // whether DM check health needed
}
#endif //_CPPUNIT

void SMStateBase::TurnOnYellowTriColorLight(void)
{
    trace (L6, _T("+SMStateBase::TurnOnYellowTriColorLight()"));

    DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                       DM_TRICOLORLIGHT_OFF,
                       DM_TRICOLORLIGHT_NORMALREQUEST);

    DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                       DM_TRICOLORLIGHT_ON,
                       DM_TRICOLORLIGHT_NORMALREQUEST);

    dm.saveTriColorLightStatus();
}



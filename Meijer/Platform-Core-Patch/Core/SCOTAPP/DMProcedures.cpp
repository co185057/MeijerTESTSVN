//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  FILE:    DMProcedures.cpp
//
//  TITLE:   Device Manager (NCR API) Interface
//
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"  
#include "Common.h"
#include "CommonNCR.h"
#include "CommonServices.h"
#include "dmx\printer.h"
#include "dmx\CashDrawerWrapper.h"
#include "dmx\KeylockWrapper.h"
#include "dmx\MICRWrapper.h"
#include "dmx\EASWrapper.h"
#include "dmx\MotionSensorWrapper.h"
#include "dmx\ScannerWrapper.h"
#include  "winuser.h"   //TAR 271776
#include "NCRRemoteSoEx.h"
#include "dmx\CashRecyclerWrapper.h"
#include "GloryOpos.h" //dp185016 Recycler Platform Issue #21
#include "SafePayC.h"	// SafePay cash recycler constants.
//+4.2 card dispenser
#include "dmx\CardDispenser.h"
#include "NCRSankyoPCRW.h"
#include "SankyoCPConst.h"
//-4.2 card dispenser

#include "MPProcedures.h"    //TAR 408696

#define DEVICEID0 0
#define DEVICEID1 1
#define WM_DISABLE_DEPOSIT      (WM_USER + 2)

// TAR 355933:  msec time lag before real disable of recycler.
#define RECYCLER_DISABLE_TIMEOUT 1750	

#define COMP_ID ID_DM							  // used by the trace() calls to identify this component
#define T_ID    _T("DMp")
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DLLEXPORT extern MPProcedures mp; //TAR 408696

static int g_nTimesInSetTriColorLight = 0;
static BOOL g_bCanDumpStack = true;
static _TCHAR szWork[255];
static long rc;

extern LPCTSTR g_sClassNames[];
/*
static LPCTSTR g_sClassNames[] = 
   {
   _T("Receipt"),
   _T("Journal"),
	_T("Slip"),
	_T("Printer"),
   _T("CashDrawer"),
	_T("CashChanger"),
	_T("Keylock"),
	_T("LineDisplay"),
	_T("MICR"),
	_T("MSR"),
	_T("Scale"),
	_T("Scanner"),
	_T("SignatureCapture"),
	_T("MISC"),
	_T("Encryptor"),
	_T("CashAcceptor"),
	_T("CoinAcceptor"),
	_T("AuxDisplay"),
	_T("MotionSensor"),
	_T("ElectronicArticleSurveillant"),
	_T("CashRecycler"),     // Standard OPOS CashChanger - not Scot CashChanger
	_T("PointCardRW")		//4.2 card dispenser
   };
*/
extern long GetURLName(CString& csFileName); // defined and used in this file

long MapDmBarcodeAlignToOpos( 
   long nAlign )
   {
   switch(nAlign)
      {
      case DM_ALIGN_LEFT:
         nAlign = PTR_BC_LEFT;
         break;
      case DM_ALIGN_RIGHT:
         nAlign = PTR_BC_RIGHT;
         break;
      case DM_ALIGN_CENTER:
         nAlign = PTR_BC_CENTER;
         break;
      default:
         ASSERT( FALSE );
         // Throw an error to the application indicating invalid parameter.
         CDeviceManagerEx::ThrowDmException( DM_E_INVALIDPARAMETER );
         break;
      }
   return nAlign;
   }


long MapDmBitmapAlignToOpos( 
   long nAlign )
   {
   switch(nAlign)
      {
      case DM_ALIGN_LEFT:
         nAlign = PTR_BM_LEFT;
         break;
      case DM_ALIGN_RIGHT:
         nAlign = PTR_BM_RIGHT;
         break;
      case DM_ALIGN_CENTER:
         nAlign = PTR_BM_CENTER;
         break;
      default:
         ASSERT( FALSE );
         // Throw an error to the application indicating invalid parameter.
         CDeviceManagerEx::ThrowDmException( DM_E_INVALIDPARAMETER );
         break;
      }
   return nAlign;
   }

long MapDmBarcodeTypeToOpos(
   long BarcodeType ) 
   {
   switch(BarcodeType)
      {
      case DM_BCS_UPCA:
         BarcodeType = PTR_BCS_UPCA;
         break;
      case DM_BCS_UPCE:
         BarcodeType = PTR_BCS_UPCE;
         break;
      case DM_BCS_JAN8:
         BarcodeType = PTR_BCS_JAN8;
         break;
      case DM_BCS_JAN13:
         BarcodeType = PTR_BCS_JAN13;
         break;
      case DM_BCS_ITF:
         BarcodeType = PTR_BCS_ITF;
         break;
      case DM_BCS_CODABAR:
         BarcodeType = PTR_BCS_Codabar;
         break;
      case DM_BCS_CODE39:
         BarcodeType = PTR_BCS_Code39;
         break;
 	  case DM_BCS_CODE128:	//BZ 06/12/02 TAR 204658: Recognize Code 128 barcodes
         BarcodeType = PTR_BCS_Code128;
         break;
      default:
         ASSERT( FALSE );
         // Throw an error to the application indicating invalid parameter.
         CDeviceManagerEx::ThrowDmException( DM_E_INVALIDPARAMETER );
         break;
      }
   return BarcodeType;
   }


typedef  enum
{
	PRN_BITMAPTOK,
	PRN_BARCODETOK,
	PRN_NOTOKEN
} PRNOBJECTTOKENS;

#define TOKEN_END		_T('\x0F')
#define BARCODETOK_START	_T('\x04')
#define BARCODETOK_END		TOKEN_END
#define BMPTOK_START		_T('\x05')
#define BMPTOK_END			TOKEN_END

static _TCHAR szTokenList[] = {BARCODETOK_START, BMPTOK_START, BMPTOK_END,_T('\0')};

PRNOBJECTTOKENS FindPrnTokenType(_TCHAR chTokenPrefix);


// macro to handle exception processing using multiple catch{}'s
// pass return value if exception caught
#define PROCESS_EXCEPTIONS_EX(i,n)                           \
	catch(COleDispatchException * e)                            \
{                                                              \
  rc = OleDispatchException(e,i,n,msg,__LINE__);                 \
  e->Delete();                                                 \
}                                                              \
	catch(COleException* e)                                     \
{                                                              \
  rc = -2;                                                     \
  OleException(e,i,n,msg,__LINE__);                              \
  e->Delete();                                                 \
}                                                              \
	catch(CException* e)                                        \
{                                                              \
  rc = -2;                                                     \
  Exception(e,i,n,msg,__LINE__);                                 \
  e->Delete();                                                 \
}                                                              \
	catch(...)                                                  \
{                                                              \
  rc = -2;                                                     \
  long le = GetLastError();                                    \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DMINTERFACE, _T("%s|%d"),msg,le);\
}
// Same but with DeviceId assumed to be zero
#define PROCESS_EXCEPTIONS(i) PROCESS_EXCEPTIONS_EX(i,0)

////////////////////////// pd980416
// some OLEDispatchExceptions can be ignored since they
// are followed by a DM_STATUS or DM_ERROR event
// and we will handle that event
//
//  Event                             Exception to ignore
//  DM_CC_STATUS_EMPTY                DM_E_CC_SHORTAGE & DM_E_CC_SHORTAGE_EX
//  DM_CC_STATUS_JAM                  DM_E_JAMMED & DM_E_JAMMED_EX
//  DM_CC_STATUS_TRANSPORT_JAM_CASH   DM_E_CC_TRANSPORT_JAM_CASH
//  DM_CC_STATUS_TRANSPORT_JAM_COIN   DM_E_CC_TRANSPORT_JAM_COIN
//  DM_CC_STATUS_HOPPER_JAM           DM_E_CC_HOPPER_JAM

static BOOL ReportException(COleDispatchException *e)// return true to report this exception
{
  switch (e->m_scError)
  {
  case DM_E_JAMMED:
  case DM_E_CC_SHORTAGE:
  case DM_E_CC_TRANSPORT_JAM_CASH:
  case DM_E_CC_TRANSPORT_JAM_COIN:
  case DM_E_CC_HOPPER_JAM:
  case DM_E_NO_CARD:				// capturecard and msrejectcard cause problems without this case
  case DM_E_CC_PARTIAL_DISP_COINS:
  case DM_E_CC_PARTIAL_DISP_BILLS:
  case DM_E_CC_PARTIAL_DISP_UNKNOWN:
  case DM_E_DEVICEFAILURE:			//When the sigcap device failed, dm throw an exception with this error
  case DM_E_DEVICENOTCONFIGURED:	// calling a DM method for an unconfigured device
  case CTL_E_FILENOTFOUND:			// it can a bitmap file, in which case DM logs it the error in event log and app logs it in trace file
    return false;
  }
  return true;
}

//////////////////////////////
long DMProcedures::OleDispatchException(
   COleDispatchException *e,
   const long nDeviceClass,
   long        nDeviceId,
   LPCTSTR szMsg,
   const int nLine )
   {
   // comment from DM Design doc...
   //    MessageBox(c->m_strDescription, "DM Container", MB_OK);
   //    CString s;
   //    s.Format("SCODE is %08x", c->m_scError);
   //    MessageBox(s, "DM Container", MB_OK);
   //    c->Delete();
   if (e->m_scError == 0x80010005) // This is the message filter error we are getting
      {
      trace(L6, _T("!!Got Message Filter Error!!"));
      traceStack(L6);
      }

   	if(nDeviceClass==DMCLASS_MICR && e->m_scError == DM_E_INVALIDDEVICEID)
	{
	   trace(L6, _T("*DMProcedures::OleDispatchException nDeviceClase==DMCLASS_MICR Killing Timer"));
	   Enable(DMCLASS_MICR, DEVICEID0, false);

	}

   //TAR 192563 NN and Chu 022602 - Status of -2146827284 from a scanner failure
   //If the above error comes from the scanner then do not ignore it but 
   //throw a system message
   bool bReportException = false;
   if ((e->m_scError == DM_E_DEVICEFAILURE && nDeviceClass == DMCLASS_SCANNER ) ||
     (e->m_scError == 0x800A03EC && nDeviceClass == DMCLASS_SCANNER ))

   {
     //Do not ignore this even though this exception do not have a status/error 
     //Since a Serial Error already exists, then use it.  As this device failure
     //is NOT known already to SCOT, and gives customer a feeling that something
     //is wrong with the scanner, otherwise it's invisible.
      bReportException = true;
      e->m_scError = 0x80010022;  //-34 error in hex
      trace(L6, _T("ERTeam: Treating scanner device failure as an existing serial error"));

   }
   else 
   {
	   bReportException = !!(ReportException(e)); // convert return value from BOOL to bool
   }
   //TAR 192563 end NN and Chu 022602 

   //bool bReportException = ReportException(e); //NN and Chu 022602   

   if (!bReportException)
      {
      // trace(L6, "Ignoring this return code: 0x%0xd to process, further status/error events with details", e->m_scError);
      }
   if (bReportException && m_bDMOpened)
      {
		ScotError(RETURN, SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DMINTERFACE_OLE,
         _T("%s|%s|%x|%s|%d"),
         szMsg, e->m_strDescription,
         e->m_scError,
         m_devmgr.GetDeviceStatistics( nDeviceClass, nDeviceId ).csDescription,
         nDeviceClass );
      }
   return e->m_scError;
   }

//////////////////////////////
void DMProcedures::OleException(COleException *e,
   const long nDeviceClass,
   long        nDeviceId,
   LPCTSTR szMsg,
   const int nLine)
{
  if (e->m_sc == 0x80010005) // This is the message filter error we are getting
  {
    trace(L6, _T("!!Got Message Filter Error!!"));
    traceStack(L6);
  }

  e->GetErrorMessage( szWork, sizeof( szWork ) );
  ScotError(FATAL,
    SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DMINTERFACE_OLE_FATAL,
    _T("%s|%s|%x|%s|%d"),
    szMsg,
    szWork,
    e->m_sc,
    m_devmgr.GetDeviceStatistics( nDeviceClass, nDeviceId ).csDescription,
    nDeviceClass);
}

//////////////////////////////
void DMProcedures::Exception(CException *e,
   const long nDeviceClass,
   long        nDeviceId,
   LPCTSTR szMsg,
   const int nLine)
{

  e->GetErrorMessage( szWork, sizeof( szWork ) );
  eo.Error(COMP_ID,T_ID,nLine,C_FATAL,
    SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DMINTERFACE_EXCEPTION,
    _T("%s|%s|%s|%d"),
    szMsg,
    szWork,
    m_devmgr.GetDeviceStatistics( nDeviceClass, nDeviceId ).csDescription,
    nDeviceClass);
}



////////////////////////////
// DMProcedures

DMProcedures::DMProcedures(
   void )
   : 
   m_bDMOpened(false),
//   m_fMSRIsMotorized(true),
   m_fIsCoinsOrBillsVeryLow(false),
   m_fIsCoinsVeryLow(false),     // TAR 279497
   m_fIsBillsVeryLow(false),     // TAR 279497
   m_bCoinDispenserWithLowSensor(false), // TAR 283924
   m_bPrinterPaperLow(false), //TAR 408696
   nDMLastReceiptFunction( 0 ),	// 0-none, 1-line, 2-BMP, 11-cut line, 12-cut BMP
   nDMLastJournalFunction( 0 ),		// 0-none, 1-line
   m_nCoinAcceptorLightState( 0 ),
   bScannerIsEnabled(false),			// RFC 219283 DE/RF 7401 Enables Motion Sensor on 7883 scanner
   m_csCashRecyclerModel( EMPTY_STRING ), //dp185016 generic cash management
   m_bIgnoreDMEvents(false),		// TAR 309097
   m_csPinpadRelease( EMPTY_STRING ),   //tar 310198
   m_csCoinDispenserStage(EMPTY_STRING)
   {
   

   fStateMotionSensor = false;          // RFC192983 RF012502
   fStateTakeawayBelt = false;          // Does the system have a Takeaway Belt?
   fTakeawayBeltBackup = false;         // Is the Takeaway Belt bagging area backed up?
   fStartUpDevFactoryLoading = true;		// TAR154447 LPM101200
   }

/////////////////////////////
DMProcedures::~DMProcedures()
   {
   }


////////////////////////////////////////////////////
// Cash Changer Functions
long DMProcedures::CashChangerDispense( long Amount)
   {
   trace(L6, _T("+CashChangerDispense - Amount: %d"), Amount);
   _TCHAR msg[] = T_ID _T("::CashChangerDispense");
   //TAR193723 Check to see if it is able to enable
   rc = Enable(DMCLASS_CASHCHANGER, DEVICEID0, true);     
   if ( rc == 0 )
      {
      try
         {
         m_devmgr.CashChangerDispense(DEVICEID0, Amount);	// 6075 = $60.75
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHCHANGER);	
      }
   else
      {
      try
      {
           CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
           //+dp185016 Recycler Platform Issue #26 - add retry
           int n = 0;
		   rc = pDevice->DispenseChange( Amount );
		   rc = rc == OPOS_E_EXTENDED ? pDevice->GetResultCodeExtended() : rc;

		   while(CashRecyclerDispenseRetry(rc) &&		// TAR 310660
			   n < co.nOperationsGloryDispenseRetries)
		   {
			   trace( L7, _T("Retrying dispense - rc: %d; n: %d"), rc, n); // Tracing for TAR 315345
			   Sleep(co.getTimeOutGloryUnitDispense());
			   rc = pDevice->DispenseChange( Amount );
			   rc = rc == OPOS_E_EXTENDED ? pDevice->GetResultCodeExtended() : rc;
			   n++;
           }

           if (co.nOperationsGloryDispenseRetries == n)
           {
             //just trace for now
             trace( L6, _T("Failed to dispense. Device is busy after %d msecs."),
				 co.nOperationsGloryDispenseRetries * co.getTimeOutGloryUnitDispense());
           }
           //-dp185016 Recycler Platform Issue #26
      }
      PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);	
      }
   trace(L6, _T("-CashChangerDispense 0x%x"),rc);
   return rc;
   }


long DMProcedures::CashChangerDispense( LPCTSTR sDispenseList)
   {
   trace(L6, _T("+CashChangerDispense - sDispenseList: %s"), sDispenseList);
   _TCHAR msg[] = T_ID _T("::CashChangerDispense");
   //TAR193723 Check to see if it is able to enable
   rc = Enable(DMCLASS_CASHCHANGER, DEVICEID0, true);     
   if ( rc == 0 )
      {
      try
         {
         // DM Does not expose this now
//         m_devmgr.CashChangerDispenseByCount(DEVICEID0, sDispenseList);	
         trace(L6, _T("Dispense through cash changer"));
         m_devmgr.CashChangerDispense(DEVICEID0, sDispenseList);	

         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHCHANGER);	
      }
   else
      {
      try
         {
           CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
           //+dp185016 Recycler Platform Issue #26 - add retry
           int n = 0;
		     trace(L7, _T("right before calling pDevice->DispenseCash  - sDispenseList: %s"), sDispenseList);
		     rc = pDevice->DispenseCash( sDispenseList );
			rc = rc == OPOS_E_EXTENDED ? pDevice->GetResultCodeExtended() : rc;
			
			while(CashRecyclerDispenseRetry(rc) &&		// TAR 310660
				n < co.nOperationsGloryDispenseRetries)
			{
				trace( L7, _T("Retrying dispense - rc: %d; n: %d"), rc, n); // Tracing for TAR 315345
				Sleep(co.getTimeOutGloryUnitDispense());
				rc = pDevice->DispenseCash( sDispenseList );
				rc = rc == OPOS_E_EXTENDED ? pDevice->GetResultCodeExtended() : rc;
				n++;
			}
           if (co.nOperationsGloryDispenseRetries == n)
           {
             //just trace for now
             trace( L6, _T("Failed to dispense. Device is busy after %d msecs."),
				 co.nOperationsGloryDispenseRetries * co.getTimeOutGloryUnitDispense());
           }
           //-dp185016 Recycler Platform Issue #26

      }
      PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);	
      }
   trace(L6, _T("-CashChangerDispense 0x%x"),rc);
   return rc;
   }


// Begin TAR 127340 
//////////////////////////////////////////////////
long DMProcedures::SetCashChangerCashTaken( bool bNewValue )
   {
   trace(L6, _T("+SetCashChangerCashTaken <%d>"), bNewValue);
   _TCHAR msg[] = T_ID _T("::SetCashChangerCashTaken");
   rc = Enable(DMCLASS_CASHCHANGER, DEVICEID0, true);    //TAR193723 Check to see if it is able to enable
   if ( rc == 0 )
      {
      try
         {
         m_devmgr.SetCashChangerCashTaken(DEVICEID0, bNewValue);
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHCHANGER);	
      }
   trace(L6, _T("-SetCashChangerCashTaken 0x%x"),rc);
   return rc == 0;
   }
// End TAR 127340


/////////////////////////////////////////////////////////////////////
long DMProcedures::CashChangerGetCashCounts( BSTR* pCashCounts, BOOL* pDiscrepancy)
   {
   trace(L6, _T("+CashChangerGetCashCounts"));
   _TCHAR msg[] = T_ID _T("::CashChangerGetCashCounts");
   rc = Enable(DMCLASS_CASHCHANGER, DEVICEID0, true);   //TAR193723 Check to see if it is able to enable
   if ( rc == 0 )
      {
      try
         {
         m_devmgr.CashChangerGetCashCounts(DEVICEID0, pCashCounts, pDiscrepancy);
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHCHANGER);  
      }
   else
      {
		if(!m_csCashRecyclerModel.IsEmpty())
		{
			try
			{
				CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
				rc = pDevice->ReadCashCounts( pCashCounts, pDiscrepancy );
			}
			PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);	
		}
	  }

   trace(L6, _T("-CashChangerGetCashCounts 0x%x"),rc);
   return rc;
   }

/////////////////////////////////////////////////////////////////////
long DMProcedures::CashChangerReplenishCoins(
   long* plData, 
   BSTR* pbsData )
   {
   _TCHAR msg[] = T_ID _T("::CashChangerReplenishCoins");
   trace(L6, _T("+CashChangerReplenishCoins"));
   rc = Enable(DMCLASS_CASHCHANGER, DEVICEID0, true);  //TAR193723 Check to see if it is able to enable
   if ( rc == 0 )
      {
      try
         {
         m_devmgr.CashChangerReplenishCoins(DEVICEID0, plData, pbsData);
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHCHANGER);  
      }
   trace(L6, _T("-CashChangerReplenishCoins"));
   return rc;
   }

/////////////////////////////////////////////////////////////////////
void DMProcedures::CashChangerGetStatusValues(

   long* plCoinStatus, 
   long* plBillStatus)
   {
   _TCHAR msg[] = T_ID _T("::CashChangerGetStatusValues");
   trace(L6, _T("+CashChangerGetStatusValues"));
   try
      {
      m_devmgr.CashChangerGetStatusValues(DEVICEID0, plCoinStatus, plBillStatus);	
      }
   PROCESS_EXCEPTIONS(DMCLASS_CASHCHANGER);  
   trace(L6, _T("-CashChangerGetStatusValues"));
   return;
   }


//////////////////////////////////////////////////
long DMProcedures::GetCashChangerCashTaken( void )
   {
   _TCHAR msg[] = T_ID _T("::GetCashChangerCashTaken");
   trace(L6, _T("+GetCashChangerCashTaken"));
   //TAR193723 Check to see if it is able to enable
   rc = Enable(DMCLASS_CASHCHANGER, DEVICEID0, true);  
   if ( rc == 0 )
      {
      try
         {
         return m_devmgr.GetCashChangerCashTaken(DEVICEID0);
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHCHANGER);  
      }
   trace(L6, _T("-GetCashChangerCashTaken 0x%x"),rc);
   return rc;
   }

//////////////////////////////////////////////
long DMProcedures::CashChangerPurgeBills(void)
   {
   _TCHAR msg[] = T_ID _T("::CashChangerPurgeBills");
   trace(L6, _T("+CashChangerPurgeBills"));
   rc = Enable(DMCLASS_CASHCHANGER, DEVICEID0, true); 
   //TAR193723 Check to see if it is able to enable
   if ( rc == 0 )
      {
      try
         {
         // puts the bills not taken into the purge bin.
         m_devmgr.ThrowDmException( DM_E_ILLEGAL );	
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHCHANGER);
      }
   trace(L6, _T("-CashChangerPurgeBills 0x%x"),rc);
   return rc;
   }


////////////////////////////////////////////////////////////////////////////////////////////////
// DMProcedures CheckHealth
long DMProcedures::CheckHealth(
   long nDeviceClass, 
   long nDeviceId /* = 0 */ ) // call CheckHealth
   {
   trace(L6,_T("+CheckHealth %d"),nDeviceClass);
	_TCHAR msg[] = T_ID _T("::CheckHealth");
   CDeviceStatistics dvs = m_devmgr.GetDeviceStatistics( nDeviceClass, nDeviceId );
	BOOL bDeviceToBeDisabled = dvs.fIsEnabled == FALSE;
   rc = 0;
	if ( bDeviceToBeDisabled  != false )
	   {
		trace(L6, _T("Enabling device before check health"));
		rc = Enable(nDeviceClass, DEVICEID0, true);
	   }
	try
	   {
      if ( nDeviceClass <= DMCLASS_PRINTER )
         {
         if ( dvs.cStatus == DM_DA_LOADED )
            {
               CPrintStation* pPrinter = m_devmgr.GetPrintStation( nDeviceClass, nDeviceId );
               // return success if the printer is busy, - it must be healthy
               rc = OPOS_SUCCESS;
			   //The logic is, if the printer is busy, don't even try to check the health.
			   //But, once it is NOT busy, try to check the health again, and if the health is good,
			   //try to reprint what ever was being printed when the error occurred.
               if ( pPrinter->GetState() != OPOS_S_BUSY )
			   {
                  rc = pPrinter->CheckHealth( 2 );
                  //if ( rc == 0 )                     //Tar 218582
                     //rc = pPrinter->RetryOutput();
                  //if ( rc == 0 )
                     //nStatusEventScotdevicemanager1( DM_PTR_REQUEST_COMPLETED, nDeviceClass, DEVICEID0 );
				  m_devmgr.ThrowDmException( pPrinter ); 
			   }
			}
		   else
            {
            ASSERT( nDeviceClass != DMCLASS_RECEIPT); // Slip CheckHealth??
			   trace(L6, _T("Not doing health check on Journal Printer."));
            }
         }
      else if ( rc == 0 )  // level 2 check health
			m_devmgr.CheckHealth( nDeviceClass, nDeviceId, 2 );      
      else if ( nDeviceClass == DMCLASS_CASHCHANGER 
         || nDeviceClass == DMCLASS_CASHACCEPTOR 
         || nDeviceClass == DMCLASS_COINACCEPTOR )
         {
         rc = 0;
         m_devmgr.CheckHealth( nDeviceClass = DMCLASS_CASHRECYCLER, nDeviceId, 2 );      
         }
      }
   PROCESS_EXCEPTIONS(nDeviceClass );

   long lReturn = rc;   //Tar 228587

   if (bDeviceToBeDisabled)
      {
	   trace(L6, _T("Disabling device after check health"));
	   Enable(nDeviceClass, DEVICEID0, false);
	   bDeviceToBeDisabled = false;
	   }
   trace(L6,_T("-CheckHealth 0x%x"), rc);

   if(lReturn && (nDeviceClass == DMCLASS_COINACCEPTOR || nDeviceClass == DMCLASS_CASHACCEPTOR || nDeviceClass == DMCLASS_CASHCHANGER) )  ////Tar 228587 don't disable device if coinacceptor disconnect //tar 228685
   {
	   rc = lReturn;   // rc got lost in Enable(), restore it.
   }

   return rc;
   }

////////////////////////////////////////////////////////////////////////////////////////////////
// DMProcedures get currency map
CString DMProcedures::CashChangerGetCurrencyMap(void)
   {
   _TCHAR msg[] = T_ID _T("::CashChangerGetCurrencyMap");
   CString csRet;
   trace(L6,_T("+CashChangerGetCurrencyMap"));
   rc = Enable(DMCLASS_CASHCHANGER, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         csRet = m_devmgr.GetCashChangerCurrencyContainerMap(DEVICEID0);
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHCHANGER );
      }
   else
      {
      try
         {
         CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
         csRet = pDevice->GetCurrencyContainerMap();
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHCHANGER );
      }

   trace(L6,_T("-CashChangerGetCurrencyMap: %s"), csRet);
   return csRet;
   }

////////////////////////////////////////////////////////////////////////////////////////////////
// DMProcedures get dispensed list
CString DMProcedures::CashChangerGetDispensedCashList(void)
   {
   _TCHAR msg[] = T_ID _T("::CashChangerGetDispensedCashList");
   CString csRet;
   trace(L6,_T("+CashChangerGetDispensedCashList"));
   rc = Enable(DMCLASS_CASHCHANGER, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         csRet = m_devmgr.GetCashChangerDispensedCashList( DEVICEID0 );
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHCHANGER );
      }
   else
      {
      try
         {
         CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
         csRet = pDevice->GetDispensedCashList();
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHCHANGER );
      }
   trace(L6,_T("-CashChangerGetDispensedCashList: %s"), csRet);
   return csRet;
   }

////////////////////////////////////////////////////////////////////////////////////////////////
// DMProcedures get low currency indicators
CString DMProcedures::CashChangerGetLowIndicators(void)
   {
   _TCHAR msg[] = T_ID _T("::GetCashChangerLowIndicatorList");
   CString csRet;
   trace(L6,_T("+CashChangerGetLowIndicators"));
   rc = Enable(DMCLASS_CASHCHANGER, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         csRet = m_devmgr.GetCashChangerLowIndicatorList( DEVICEID0 );
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHCHANGER );
      }
   else
      {
      try
         {
         CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
         csRet = pDevice->GetLowIndicatorList();
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHCHANGER );
      }
   trace(L6,_T("-GetCashChangerLowIndicatorList: %s"), csRet);
   return csRet;
   }

////////////////////////////////////////////////////////
// Sound related functions
bool DMProcedures::SoundInitPath(LPCTSTR ProductRegPath)
{
  if (_tcsclen (ProductRegPath)==0) return false;
	
  // Registry entry for sound file.
#define  SOUNDFILE_REG_KEY _T("\\SCOTSOUND")
#define  SCOT_SOUND_ENTRY  _T("SoundFilePath")
#define  SCOT_DEFAULT_LANGUAGE _T("DefaultLanguage")
	
  unsigned long nIntBufferLen = _MAX_PATH;
  _TCHAR  strSubKey[_MAX_PATH+1];
  _TCHAR  strBuffer[_MAX_PATH+1];
	
  HKEY  hKey;
  long  retVal;

  _tcscpy (strSubKey, ProductRegPath);
  _tcscat (strSubKey, SOUNDFILE_REG_KEY);

  //get main sound file path	
  retVal = RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey, 0, KEY_QUERY_VALUE, &hKey);
  ASSERT(retVal == ERROR_SUCCESS);
	
  retVal = RegQueryValueEx(hKey, SCOT_SOUND_ENTRY, NULL, NULL, (LPBYTE) strBuffer, &nIntBufferLen);
  /// fix later, the error object is not present
  ASSERT (retVal == ERROR_SUCCESS);

  if (retVal != ERROR_SUCCESS) return false;

  // Set the global variable for this module
  ApplicationRegistryRootPath = strBuffer;
  if (ApplicationRegistryRootPath.Right(1) != _T("\\"))
      ApplicationRegistryRootPath += _T('\\');

  //get default language sound subdirectory
  retVal = RegQueryValueEx(hKey, SCOT_DEFAULT_LANGUAGE, NULL, NULL, (LPBYTE) strBuffer, &nIntBufferLen);
  ASSERT (retVal == ERROR_SUCCESS);
  m_csSoundDefaultLanguageCode = strBuffer;
	
  RegCloseKey(hKey);
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Copy the sound files dirrectory path string to the char pointer parameter.
bool DMProcedures::SoundFindSoundFilePath(LPTSTR pszPathName, unsigned long nIntBufferLen)
{
  if (_tcsclen(ApplicationRegistryRootPath) == 0 )
    return false;

  CString csWork;
  if (co.fStateDualLanguage)
  {
      csWork = ApplicationRegistryRootPath  + co.language[ps.GetLanguage()].szCode;
  }
  else
  {
      csWork = ApplicationRegistryRootPath + m_csSoundDefaultLanguageCode;
  }
  _tcscpy (pszPathName, csWork);
  return true;
}

///////////////////////////////////////////
// Stated the money amount passed in the parameter, negitive numbers allowed.
long DMProcedures::SayAmount( long amount )
{
  if(co.fStateDualLanguage)
	SCOTPriceSoundSetLanguage(co.language[ps.GetLanguage()].szCode, co.language[ps.GetLanguage()].csPriceSoundConfigFile);
  BOOL bRet = SCOTPriceSoundSayPrice(amount);
  trace(L6,_T("SCOTPriceSoundSayPrice 0x%x"),amount);

  if(!bRet)
  {
    ScotError( RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_DMPROCEDURES_SAYAMOUNT);

    return -1;
  }
  return 0;	// success
}

///////////////////////////////////////////////
// Play the associated wav file.
long DMProcedures::SayPhrase( LPCTSTR WaveFile )
{
  TCHAR  pszwholeFileName[512];
  BOOL bRet = SoundFindSoundFilePath(pszwholeFileName, sizeof(pszwholeFileName));
  CString sWholeName = pszwholeFileName;
  CString sWaveFile = WaveFile;
	
  if (bRet)
  {
    if (sWaveFile.Find(_T("\\")) == -1)
       {
          if (sWholeName.Right(1) != _T("\\"))
            //Append the "\\" character to the end.
            sWholeName += _T("\\");
          sWholeName += sWaveFile;
       }
    else
       sWholeName = sWaveFile;

    //Play the wav file.
	TRACE(_T("!!!!SCOTPriceSound: Playing this wave file: %s !!!!\n"), (LPCTSTR)sWholeName);
    bRet = SCOTPriceSoundSayMessage((LPCTSTR)sWholeName);
    if(!bRet)
    {
      ScotError( RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_DMPROCEDURES_SAYPHRASE);

      return -1;
    }    
  }
  return 0;
}

/////////////////////////////////////////////////////////////
// Play a security alert tone and the associated wav file
// make sure NOT to call this with a space for the wave file name!
long DMProcedures::SaySecurity( long level, LPCTSTR wavfile, HANDLE hWaitEvent /*=0*/ )
{
  trace(L6, _T("+SaySecurity, %s."), wavfile);
  _TCHAR  pszwholeFileName[512];
	
  BOOL bRet = SoundFindSoundFilePath(pszwholeFileName, sizeof(pszwholeFileName));
	
  if (bRet)
  {
    //Append the "\\" character to the end.
		
    // hokey bypass for abend access violation at 0a0000           pd
    int i = _tcsclen(pszwholeFileName); // pd
    ASSERT(i>0);  
	
	// pd
    if (pszwholeFileName[i-1] != _T('\\')) // pd
      //if (pszwholeFileName[strlen(pszwholeFileName)-1] != '\\')// pd
      _tcscat (pszwholeFileName, _T("\\"));
		
    if (_tcsclen(wavfile) > 0)		//TAR#125687 do not attempt to play missing wav files	LPM110899
	{
		_tcscat (pszwholeFileName, wavfile );							
		// Let the Dll know the value of the interrupt option LPM
		bRet = SCOTSaySecurityAllowInterrupt(co.fStateAllowInterruptWavFiles);
		if(!bRet)
		{
		  ScotError( RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_DMPROCEDURES_SAYSECURITY);
		  trace(L6, _T("-SaySecurity: internal error, %s."), wavfile);
		  return -1;
		}
		//Play the wav file.
		//if (strlen(wavfile)==0) strcpy(pszwholeFileName,"");			//TAR#125687
		bRet = SCOTPriceSoundSaySecurity(level, pszwholeFileName, hWaitEvent);
		if(!bRet)
		{
		  ScotError( RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_DMPROCEDURES_SAYSECURITY);
		  trace(L6, _T("-SaySecurity: internal error, %s."), wavfile);
		  return -1;
		}
		// Tar 222910 start: Changes to read SaySecurity Sleep value from Registry
		if (co.bIgnoreSleep == false)
		{
		  Sleep (co.lSaySecSleepVal);
		}
		//Tar 222910 end
	}
  }
  trace(L6, _T("-SaySecurity, %s."), wavfile);
  return 0;
}

//////////////////////////////////////////
// Play a security alert tone only
long DMProcedures::SaySignal( long level )
{
  if( level == 0 )
    return 0;
	
  BOOL bRet = SCOTPriceSoundSaySignal(level);
	
  if(!bRet)
  {
    ScotError( RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_DMPROCEDURES_SAYSIGNAL);

    return -1;
  }
  return 0;
}


////////////////////////////////////
// MSReader Methods
long DMProcedures::MSREnable( void )
   {
   trace(L6, _T("+MSREnable"));
   rc = Enable(DMCLASS_MSR, DEVICEID0, true );
   trace(L6,_T("-MSREnable 0x%x"),rc);
   return rc;
   }

/////////////////////////////////////
long DMProcedures::MSRDisable( void )
   {
   trace(L6,_T("+MSRDisable"));
   rc = Enable(DMCLASS_MSR, DEVICEID0, false );
   trace(L6,_T("-MSRDisable 0x%x"),rc);
   return rc;
   }


///////////////////////////////////////
long DMProcedures::MSRCaptureCard(void)
   {
   trace(L6,_T("+MSRCaptureCard"));
   _TCHAR msg[] = T_ID _T("::MSRCaptureCard");
   rc = Enable(DMCLASS_MSR, DEVICEID0, true);
   CString csMSRcaptureInfo = EMPTY_STRING;
   long pData = 0;
   if ( rc == 0 )
      {
	  BSTR pString = csMSRcaptureInfo.AllocSysString();
      try
         {
         CMSRWrapper* pMotorizedMSR = m_devmgr.GetMSR( DEVICEID0 );
         rc = pMotorizedMSR->DirectIO(NCR_IOM_CAPTURE, &pData, &pString);
		 csMSRcaptureInfo = pString;
		 SysFreeString(pString);
         }
      PROCESS_EXCEPTIONS(DMCLASS_MSR);
      }
   trace(L6,_T("-MSRCaptureCard 0x%x, %s"),rc, csMSRcaptureInfo);
   return rc;
   }

/////////////////////////////////////
long DMProcedures::MSREjectCard(void)
   {
   trace(L6,_T("+MSREjectCard"));
   _TCHAR msg[] = T_ID _T("::MSREjectCard");
   rc = Enable(DMCLASS_MSR, DEVICEID0, true);
   CString csMSRejectInfo = EMPTY_STRING;
   long pData = 0;
   if ( rc == 0 )
      {
	  BSTR pString = csMSRejectInfo.AllocSysString();
      try
         {
         CMSRWrapper* pMotorizedMSR = m_devmgr.GetMSR( DEVICEID0 );
         rc = pMotorizedMSR->DirectIO(NCR_IOM_EJECT, &pData, &pString);
		 csMSRejectInfo = pString;
		 SysFreeString(pString);
        }
      PROCESS_EXCEPTIONS(DMCLASS_MSR);
      }
   trace(L6,_T("-MSREjectCard 0x%x, %s"),rc, csMSRejectInfo);
   return rc;
   }

///////////////////////////////////
bool DMProcedures::MSRIsMotorized()
   {
   return co.fTenderIsMotorizedMSR;
   }

///////////////////////////////////
long DMProcedures::SetMSRTracksToRead(long nNewValue)
{
  _TCHAR msg[] = T_ID _T("::SetMSRTracksToRead");
	
   rc = Enable(DMCLASS_MSR, DEVICEID0, true);
   long pData = 0;
   if ( rc == 0 )
   {
    try
    {
         CMSRWrapper* pMotorizedMSR = m_devmgr.GetMSR( DEVICEID0 );
         pMotorizedMSR->SetTracksToRead(nNewValue);
    }
      PROCESS_EXCEPTIONS(DMCLASS_MSR);
   }
   trace(L6,_T("-MSRCaptureCard 0x%x"),rc);
    return 0;
}


///////////////////////////////////
CString DMProcedures::MSRGetDeviceName(void)
{
  _TCHAR msg[] = T_ID _T("::MSRGetDeviceName");
   CString csDeviceName;	
   rc = Enable(DMCLASS_MSR, DEVICEID0, true);
   if ( rc == 0 )
   {
    try
    {
         CMSRWrapper* pMotorizedMSR = m_devmgr.GetMSR( DEVICEID0 );
         csDeviceName = pMotorizedMSR->GetDeviceName();
    }
      PROCESS_EXCEPTIONS(DMCLASS_MSR);
   }
   trace(L6,_T("-MSRGetDeviceName %s"),csDeviceName);
    return csDeviceName;
}

/////////////////////////////////// awk.1
//Support for the exit gate////////
long DMProcedures::CashDrawerOpen(void)
   {
   trace(L6,_T("+ExitGateOpen"));
   _TCHAR msg[] = T_ID _T("::CashDrawerOpen");
   rc = Enable(DMCLASS_CASHDRAWER, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         CCashDrawerWrapper* pCashDrawer = m_devmgr.GetCashDrawer( DEVICEID0 );
         rc = pCashDrawer->OpenDrawer();
         m_devmgr.ThrowDmException( pCashDrawer );
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHDRAWER);
      }
   trace(L6,_T("-ExitGateOpen 0x%x"),rc);
   return rc;
   }

/////////////////////////////////// 
//Support for the exit gate////////
CString DMProcedures::CashDrawerGetDeviceName(void)
   {
   trace(L6,_T("+CashDrawerGetDeviceName"));
   _TCHAR msg[] = T_ID _T("::CashDrawerGetDeviceName");
   CString csDeviceName;
   rc = Enable(DMCLASS_CASHDRAWER, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         CCashDrawerWrapper* pCashDrawer = m_devmgr.GetCashDrawer( DEVICEID0 );
         csDeviceName = pCashDrawer->GetDeviceName();
         m_devmgr.ThrowDmException( pCashDrawer );
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHDRAWER);
      }
   trace(L6,_T("-CashDrawerGetDeviceName 0x%x, %s"),rc, csDeviceName );
   return csDeviceName;
   }


//////////////////////////////////////////////
// Printer functions
long DMProcedures::ReceiptLine(const CString& csPrintLine) // print a line
   {
   trace(L6,_T("+ReceiptLine"));
   csDMLastReceiptLine = csPrintLine;
   nDMLastReceiptFunction = 1;
   rc = ProcessPrintLine(DMCLASS_RECEIPT,csPrintLine);
   trace(L6,_T("-ReceiptLine 0x%x"),rc);
   return rc;
   }

//////////////////////////////////////////////////
long DMProcedures::ReceiptBitMap(const CString csBmpFile) // print a BMP file PD write logo
   {
   trace(L6,_T("+ReceiptBitMap %s"),csBmpFile);
   csDMLastReceiptBmpFile = csBmpFile;
   nDMLastReceiptFunction = 2;
   rc = PrintBitmap(csBmpFile, DM_ALIGN_LEFT, DMCLASS_RECEIPT );
   trace(L6,_T("-ReceiptBitMap %d"),rc);
   return rc;
   }


///////////////////////////////
long DMProcedures::ReceiptCut()		  // cut the paper
   {
   trace(L6,_T("+ReceiptCut"));
   _TCHAR msg[] = T_ID _T("::ReceiptCut");
   if ( nDMLastReceiptFunction < 10 )
      nDMLastReceiptFunction += 10;
   rc = Enable(DMCLASS_RECEIPT, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         // 1.1 Printer SO was always doing an advance, even though
         // we sent CUT_NOW.  For 2.0, CUT_NOW really does a CUT NOW,
         // so we need to send an explicit AUTOADVANCE.
         // 03/01/2000 - ncr_barn - TAR# 133943
         CPrintStation* pPrinter = m_devmgr.GetPrintStation( DMCLASS_RECEIPT );
         pPrinter->CutPaper( 100 );
         m_devmgr.ThrowDmException( pPrinter );
         }
      PROCESS_EXCEPTIONS(DMCLASS_RECEIPT );	
      }
   trace(L6,_T("-ReceiptCut 0x%x"),rc);
   return rc;
   }

///////////////////////////////
void DMProcedures::ReceiptReprint()
{
  trace(L6,_T("+ReceiptReprint"));
    // dmi.ReceiptLine(csDMLastReceiptLine);  // old code
	// Allow SCOT App to process failed print line and regenerate the entire print request queue
	// While processing, any bitmap or barcode requests embedded in the receipt line are executed
	// again.
  switch (nDMLastReceiptFunction)
  {
  case 1:
//    ReceiptLine(csDMLastReceiptLine);
//    break;
//Tar 139633 start
  case 11:
  case 12:
//Tar 139633 end
  case 2:
      ReceiptBitMap(csDMLastReceiptBmpFile);
    break;
  case 10:
//Tar 139633  dmi.ReceiptCut();
    break;
//Tar 139633 start
//  case 11:
//  ReceiptLine(csDMLastReceiptLine);
//  dmi.ReceiptCut();
//	rq.PrintAll();
//    break;
//  case 12:
//  dmi.ReceiptBitMap(csDMLastReceiptBmpFile);
//  dmi.ReceiptCut();
//	rq.PrintAll();
//    break;
//Tar 139633 end
  }
  trace(L6,_T("-ReceiptReprint"));
}



long DMProcedures::JournalLine(const CString& csPrintLine) // print a line
   {
   trace(L6,_T("+JournalLine"));
   csDMLastJournalLine = csPrintLine;
   nDMLastJournalFunction = 1;   //0: none; 1: print line
   _TCHAR msg[] = T_ID _T("::JournalLine");
   // the journal is an optional device
   // honor its config status and allow
   // it to be absent
   CDeviceStatistics dvs = m_devmgr.GetDeviceStatistics( DMCLASS_JOURNAL );
   if ( dvs.cStatus == DM_DA_NOT_CONFIGURED)
      return 0;
   rc = Enable( DMCLASS_JOURNAL, DEVICEID0, true );
   if ( rc == 0 )
      {
      try
         {
         CPrintStation* pPrinter = m_devmgr.GetPrintStation( DMCLASS_JOURNAL );
         pPrinter->PrintNormal( csPrintLine );
         m_devmgr.ThrowDmException( pPrinter );
         }
      PROCESS_EXCEPTIONS(DMCLASS_JOURNAL);	
      }
   trace(L6,_T("-JournalLine 0x%x"),rc);
   return rc;
   }

///////////////////////////////
void DMProcedures::JournalReprint()    //Journal Printer
{
  trace(L6,_T("+JournalReprint"));
  switch (nDMLastJournalFunction)
  {
  case 1:
    JournalLine(csDMLastJournalLine);
    break;
  
  }
  trace(L6,_T("-JournalReprint"));
}


/////////////////////////////////////////////
// Cash Acceptor Methods
long DMProcedures::CashAcceptorEnable( void )
{
  trace(L6,_T("+CashAcceptorEnable"));
  rc = Enable(DMCLASS_CASHACCEPTOR, DEVICEID0, true );
  KillTimer(WM_DISABLE_DEPOSIT);
  if ( rc != 0 )
     {
      _TCHAR msg[] = T_ID _T("::CashAcceptorEnable");
     try
         {
         CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
         rc = 0;
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);	
      }
  trace(L6,_T("-CashAcceptorEnable 0x%x"),rc);
  return rc;
}

////////////////////////////////////////////
long DMProcedures::CashAcceptorDisable(void)
{
  trace(L6,_T("+CashAcceptorDisable"));
  rc = Enable(DMCLASS_CASHACCEPTOR, DEVICEID0, false );

  // +TAR 355933:  Increase time lag for SafePay.  Changing this value
  //			   could lead to timing issues, so leave Glory value the 
  //			   same for now.
  if(dm.m_csCashRecyclerModel == _CASHDISPENSER_SAFEPAY)
  {
  	  SetTimer( WM_DISABLE_DEPOSIT, RECYCLER_DISABLE_TIMEOUT, NULL );
  }
  else	// Use regular setting.
  {
  	  SetTimer( WM_DISABLE_DEPOSIT, 1500, NULL );
  }
  // -TAR 355933

#if 0
   try
      {
      CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
      rc = pDevice->EndDeposit( CHAN_DEPOSIT_NOCHANGE );
      }
   PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);	
#endif

  trace(L6,_T("-CashAcceptorDisable 0x%x"),rc);
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
long DMProcedures::CashAcceptorReadBillCounts(BSTR* pCashCounts)
   {
   trace(L6,_T("+CashAcceptorReadBillCounts"));
   _TCHAR msg[] = T_ID _T("::CashAcceptorReadBillCounts");
	
   rc = Enable(DMCLASS_CASHACCEPTOR, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         m_devmgr.CashAcceptorReadBillCounts(DEVICEID0, pCashCounts);
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHACCEPTOR);
      }
   else
      {
      try
         {
         CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
         BOOL bDiscrepancy = FALSE;
         rc = pDevice->ReadDropBoxCounts( pCashCounts, &bDiscrepancy );
         CString sData( *pCashCounts );
         ::SysFreeString( *pCashCounts );
         if ( sData.Find( _T(';') ) != -1 )
            sData = sData.Mid( sData.Find( _T(';') ) );
         *pCashCounts = sData.AllocSysString();
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);	
      }

   trace(L6,_T("+CashAcceptorReadBillCounts 0x%x"),rc);
   return rc;
   }

//////////////////////////////////////////////////////////////////////////
long DMProcedures::SetCashAcceptorCurrencyAcceptList(LPCTSTR lpszNewValue)
   {
   trace(L6,_T("+SetCashAcceptorCurrencyAcceptList"));
   _TCHAR msg[] = T_ID _T("::SetCashAcceptorCurrencyAcceptList");
   rc = Enable(DMCLASS_CASHACCEPTOR, DEVICEID0, true);

   if ( rc == 0 )
      {
      try
         {
         m_devmgr.SetCashAcceptorCurrencyAcceptList(DEVICEID0, lpszNewValue);
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHACCEPTOR);	
      }
   else
      {
      try
         {
         CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
         if ( *lpszNewValue != 0 )
		 {
            rc = pDevice->BeginDeposit();
            if (SMStateBase::fRecyclerNotReady)
            { //dp185016 Recycler Platform Issue #12
              //we just log this in trace for now.
              trace( L6, _T("Timedout while waiting for recycler to be ready on BeginDeposit()."));
              SMStateBase::fRecyclerNotReady = false; //reset
            }
			trace(L6,_T("+SetCashAcceptorCurrencyAcceptList pDevice->BeginDeposit() rc=0x%x "), rc);
		 }
         else
            {
            rc = pDevice->EndDeposit( CHAN_DEPOSIT_NOCHANGE );
			trace(L6,_T("+SetCashAcceptorCurrencyAcceptList pDevice->EndDeposit() rc=0x%x "), rc);
            }
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);	
      }
   trace(L6,_T("-SetCashAcceptorCurrencyAcceptList 0x%x"),rc);
   return rc;
   }

///////////////////////////////////////////
long DMProcedures::CashAcceptorReject(long nAmount)
   {
   trace(L6,_T("+CashAcceptorReject"));
   _TCHAR msg[] = T_ID _T("::CashAcceptorReject");
   rc = Enable(DMCLASS_CASHACCEPTOR, DEVICEID0, true);
   CString csDen;
   csDen.Format(_T("%d"),nAmount);
   if ( rc == 0 )
   {
      try
      {

         if ( m_devmgr.GetCashAcceptorCapEscrow(DEVICEID0) != FALSE )
            m_devmgr.CashAcceptorReject(DEVICEID0);
         else if (nAmount > 0)
         {
            rc = CashChangerDispense( nAmount );
            if (rc == 0)
            {
               //Code Removed from here for TAR 373026
			   CashRecyclerUpdateDispensed(nAmount);	// TAR 311471, 312429
            }
         }
      }
      PROCESS_EXCEPTIONS(DMCLASS_CASHACCEPTOR);
   }
   else if (nAmount > 0)
   {
      rc = CashChangerDispense( nAmount );
      if (rc == 0)
      {
         //Code Removed from here for TAR 373026
		 CashRecyclerUpdateDispensed(nAmount);	// TAR 311471, 312429
      }
   }
   trace(L6,_T("-CashAcceptorReject 0x%x"),rc);
   return rc;
   }

///////////////////////////////////////////
long DMProcedures::CashAcceptorAccept(void)
   {
   trace(L6,_T("+CashAcceptorAccept"));
   _TCHAR msg[] = T_ID _T("::CashAcceptorAccept");
   rc = Enable(DMCLASS_CASHACCEPTOR, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         if ( m_devmgr.GetCashAcceptorCapEscrow(DEVICEID0) != FALSE )
            m_devmgr.CashAcceptorAccept( DEVICEID0 );
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHACCEPTOR);
      }
   trace(L6,_T("-CashAcceptorAccept 0x%x"),rc);
   return rc;
   }

////////////////////////////////////////////////////
long DMProcedures::GetCashAcceptorDeviceStatus(void)
   {
   trace (L6,_T("+GetCashAcceptorDeviceStatus"));
   _TCHAR msg[] = T_ID _T("::CashAcceptorDeviceStatus");
   long lResult = rc = -1;
   // Check if device is configured
   CDeviceStatistics dvs = m_devmgr.GetDeviceStatistics( DMCLASS_CASHACCEPTOR );
   if ( DM_DA_NOT_CONFIGURED != dvs.cStatus )  
      {
      // Leave the cash acceptor in the same state we found it
      bool saveEnableStatus = dvs.fIsEnabled;
      lResult = rc = Enable(DMCLASS_CASHACCEPTOR, DEVICEID0, true);
      try
         {
         lResult = m_devmgr.GetCashAcceptorDeviceStatus(DEVICEID0);
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHACCEPTOR);	
      Enable(DMCLASS_CASHACCEPTOR, DEVICEID0, saveEnableStatus);
      }
   if (rc == DM_CASHACCEPTOR_STATUS_OK)
	   trace (L6,_T("-GetCashAcceptorDeviceStatus ok!,rc = 0x%x, Status:%d"),rc, lResult);
   else
      trace (L4,_T("-GetCashAcceptorDeviceStatus rc = 0x%x, Status:%d"),rc, lResult);
   return lResult;
   }

// Begin Coin Acceptor Modifications
/////////////////////////////////////////////
// Coin Acceptor Methods
long DMProcedures::CoinAcceptorEnable( void )
   {
   trace(L6,_T("+CoinAcceptorEnable"));
   rc = Enable(DMCLASS_COINACCEPTOR, DEVICEID0, true );
   KillTimer( WM_DISABLE_DEPOSIT );
  if ( rc != 0 )
     {
      _TCHAR msg[] = T_ID _T("::CoinAcceptorEnable");
     try
         {
         CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
         rc = 0;
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);	
      }
   trace(L6,_T("-CoinAcceptorEnable 0x%x"),rc);
   return rc;
   }

////////////////////////////////////////////
long DMProcedures::CoinAcceptorDisable(void)
   {
   trace(L6,_T("+CoinAcceptorDisable"));
   rc = Enable(DMCLASS_COINACCEPTOR, DEVICEID0, false );

   // +TAR 355933:  Increase time lag for SafePay.  Changing this value
   //			   could lead to timing issues, so leave Glory value the 
   //			   same for now.
   if(dm.m_csCashRecyclerModel == _CASHDISPENSER_SAFEPAY)
   {
	   SetTimer( WM_DISABLE_DEPOSIT, RECYCLER_DISABLE_TIMEOUT, NULL );
   }
   else	// Use regular setting.
   {
	   SetTimer( WM_DISABLE_DEPOSIT, 1500, NULL );
   }
   // -TAR 355933

#if 0
   try
      {
      CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
      rc = pDevice->EndDeposit( CHAN_DEPOSIT_NOCHANGE );
      }
   PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);	
#endif
   trace(L6,_T("-CoinAcceptorDisable 0x%x"),rc);
   return rc;
   }

////////////////////////////////////////////////////////////////////////////////
long DMProcedures::CoinAcceptorReadBillCounts(BSTR* pCoinCounts)
   {
   trace(L6,_T("+CoinAcceptorReadCoinCounts"));
   _TCHAR msg[] = T_ID _T("::CoinAcceptorReadBillCounts");
   rc = -1;
   CDeviceStatistics dvs = m_devmgr.GetDeviceStatistics( DMCLASS_COINACCEPTOR );
   if (DM_DA_NOT_CONFIGURED != dvs.cStatus)  // Check if device is configured
      {
      rc = Enable(DMCLASS_COINACCEPTOR, DEVICEID0, true);
      if ( rc == 0 )
         {
         try
            {
            m_devmgr.CoinAcceptorReadCoinCounts(DEVICEID0, pCoinCounts);
            }
         PROCESS_EXCEPTIONS(DMCLASS_COINACCEPTOR);	
         }
      }
   else
      {
      try
         {
         CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
         BOOL bDiscrepancy = FALSE;
         rc = pDevice->ReadDropBoxCounts( pCoinCounts, &bDiscrepancy );
         CString sData( *pCoinCounts );
         ::SysFreeString( *pCoinCounts );
         if ( sData.Find( _T(';') ) != -1 )
            sData = sData.Left( sData.Find( _T(';') ) );
         *pCoinCounts = sData.AllocSysString();
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);	
      }
   trace(L6,_T("+CoinAcceptorReadCoinCounts 0x%x"),rc);
   return rc;
   }

//////////////////////////////////////////////////////////////////////////
long DMProcedures::SetCoinAcceptorEnableCoins(bool bNewValue)
   {
   trace(L6,_T("+SetCoinAcceptorEnableCoins"));
   _TCHAR msg[] = T_ID _T("::SetCoinAcceptorEnableCoins");
   rc = -1;
   CDeviceStatistics dvs = m_devmgr.GetDeviceStatistics( DMCLASS_COINACCEPTOR );
   if (DM_DA_NOT_CONFIGURED != dvs.cStatus)  // Check if device is configured
      {
      rc = Enable(DMCLASS_COINACCEPTOR, DEVICEID0, true);
      if ( rc == 0 )
         {
         try
            {
            m_devmgr.SetCoinAcceptorEnableCoins(DEVICEID0, bNewValue);
            }
         PROCESS_EXCEPTIONS(DMCLASS_COINACCEPTOR);	
         }
      }
   else
      {
      try
         {
			 CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
			 if ( bNewValue != 0 )
			 {
				rc = pDevice->BeginDeposit();
        if (SMStateBase::fRecyclerNotReady)
        { //dp185016 Recycler Platform Issue #12
          //we just log this in trace for now.
          trace( L6, _T("Timedout while waiting for recycler to be ready on BeginDeposit()."));
          SMStateBase::fRecyclerNotReady = false; //reset
        }

				trace(L6,_T("+SetCoinAcceptorEnableCoins pDevice->BeginDeposit() rc=0x%x "), rc);
			 }
			 else
			 {
				rc = pDevice->EndDeposit( CHAN_DEPOSIT_NOCHANGE );
				trace(L6,_T("+SetCoinAcceptorEnableCoins pDevice->EndDeposit() rc=0x%x "), rc);
			 }
         }
      PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);	
      }
   trace(L6,_T("-SetCoinAcceptorEnableCoins 0x%x"),rc);
   return rc;
   }

///////////////////////////////////////////
long DMProcedures::CoinAcceptorReject(long nAmount)
   {
   trace(L6,_T("+CoinAcceptorReject"));
   _TCHAR msg[] = T_ID _T("::CoinAcceptorReject");
   rc = Enable( DMCLASS_COINACCEPTOR, DEVICEID0, true);
	CString csDen;
	csDen.Format(_T("%d"),nAmount);
   if ( rc == 0 )
   {
      try
      {
         if ( m_devmgr.GetCoinAcceptorCapEscrow(DEVICEID0) != FALSE )
           m_devmgr.CashAcceptorReject(DEVICEID0);
         else if (nAmount > 0)
         {
            rc = CashChangerDispense( nAmount );
            //Code Removed from here for TAR 373026
         }
      }
      PROCESS_EXCEPTIONS(DMCLASS_COINACCEPTOR);
   }
   else
   {
	   try
	   {
		   CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
		   int n = 0;

		   if (nAmount > 0)
		   {
			   rc = pDevice->DispenseChange( nAmount );
			   rc = rc == OPOS_E_EXTENDED ? pDevice->GetResultCodeExtended() : rc;
			   
			   while(CashRecyclerDispenseRetry(rc) &&		// TAR 310660
				   n < co.nOperationsGloryDispenseRetries)
			   {
				   trace( L7, _T("Retrying dispense - rc: %d; n: %d"), rc, n); // Tracing for TAR 315345
				   Sleep(co.getTimeOutGloryUnitDispense());
				   rc = pDevice->DispenseChange(nAmount);
				   rc = rc == OPOS_E_EXTENDED ? pDevice->GetResultCodeExtended() : rc;
				   n++;
			   }
			   
			   if (rc == 0)
			   {
				   //Code Removed from here for TAR 373026
				   CashRecyclerUpdateDispensed(nAmount);	// TAR 311471, 312429	
			   }
		   }
	   }
	   PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);	
   }
   trace(L6,_T("-CoinAcceptorReject 0x%x"),rc);
   return rc;
   }

//SR742+
long DMProcedures::ReloadCoinAcceptor(void)
   {
   trace(L6,_T("+ReloadCoinAcceptor"));
   _TCHAR msg[] = T_ID _T("::ReloadCoinAcceptor");
   try
   {
	   m_devmgr.CoinAcceptorOpenDevice(DEVICEID0);
	   m_devmgr.CheckHealth(DMCLASS_COINACCEPTOR,DEVICEID0,2);
   }
   PROCESS_EXCEPTIONS(DMCLASS_COINACCEPTOR);
   trace(L6,_T("-ReloadCoinAcceptor"));
   return OPOS_SUCCESS;
   }


long DMProcedures::ReloadCashAcceptor(void)
   {
   trace(L6,_T("+ReloadCashAcceptor"));
   _TCHAR msg[] = T_ID _T("::ReloadCashAcceptor");
   try
   {
	   m_devmgr.CashAcceptorOpenDevice(DEVICEID0);
	   m_devmgr.CheckHealth(DMCLASS_CASHACCEPTOR,DEVICEID0,2);
   }
   PROCESS_EXCEPTIONS(DMCLASS_CASHACCEPTOR);
   trace(L6,_T("-ReloadCashAcceptor"));
   return OPOS_SUCCESS;
   }


long DMProcedures::ReloadCashChanger(void)
   {
   trace(L6,_T("+ReloadCashChanger"));
   _TCHAR msg[] = T_ID _T("::ReloadCashChanger");
   try
   {
       m_devmgr.CashChangerCloseDevice();
	   m_devmgr.CashChangerOpenDevice();
	   m_devmgr.CheckHealth(DMCLASS_CASHCHANGER,DEVICEID0,2);
   }
   PROCESS_EXCEPTIONS(DMCLASS_CASHCHANGER);
   trace(L6,_T("-ReloadCashChanger"));
   return OPOS_SUCCESS;
   }
//SR742-


///////////////////////////////////////////
//new version for TAR315293 to help prevent "coin swallowing" 
long DMProcedures::CoinAcceptorAccept(void)
   {
   trace(L6,_T("+CoinAcceptorAccept"));
   _TCHAR msg[] = T_ID _T("::CoinAcceptorAccept");
   try
   {
     if ( m_devmgr.GetCoinAcceptorCapEscrow( DEVICEID0 ) != FALSE )
         m_devmgr.CoinAcceptorAccept( DEVICEID0 );
   }
   PROCESS_EXCEPTIONS(DMCLASS_COINACCEPTOR);
   trace(L6,_T("-CoinAcceptorAccept"));
   return 1;
   }
/*
long DMProcedures::CoinAcceptorAccept(void)
   {
   trace(L6,_T("+CoinAcceptorAccept"));
   _TCHAR msg[] = T_ID _T("::CoinAcceptorAccept");
   rc = Enable( DMCLASS_COINACCEPTOR, DEVICEID0, true );
   if ( rc == 0 )
      {
      try
         {
         if ( m_devmgr.GetCoinAcceptorCapEscrow( DEVICEID0 ) != FALSE )
            m_devmgr.CoinAcceptorAccept( DEVICEID0 );
         }
      PROCESS_EXCEPTIONS(DMCLASS_COINACCEPTOR);
      }
   trace(L6,_T("-CoinAcceptorAccept 0x%x"),rc);
   return rc;
   }
*/
////////////////////////////////////////////////////
long DMProcedures::GetCoinAcceptorDeviceStatus(void)
   {
   trace (L6,_T("+GetCoinAcceptorDeviceStatus"));
   _TCHAR msg[] = T_ID _T("::GetCoinAcceptorDeviceStatus");
   long lResult = rc = -1;
   // Check if device is configured
   CDeviceStatistics dvs = m_devmgr.GetDeviceStatistics( DMCLASS_COINACCEPTOR );
   if ( DM_DA_NOT_CONFIGURED != dvs.cStatus )  
      {
      // Leave the coin acceptor in the same state we found it
      bool saveEnableStatus = dvs.fIsEnabled;
      lResult = rc = Enable(DMCLASS_COINACCEPTOR, DEVICEID0, true);
      try
         {
         lResult = rc = m_devmgr.GetCoinAcceptorDeviceStatus(DEVICEID0);
         }
      PROCESS_EXCEPTIONS(DMCLASS_COINACCEPTOR);	
      Enable(DMCLASS_COINACCEPTOR, DEVICEID0, saveEnableStatus);
      }
   trace (L6,_T("-GetCoinAcceptorDeviceStatus 0x%x"),rc);
   return lResult;
   }
// End Coin Acceptor Modifications

// Begin Signature Capture Modifications
////////////////////////////////////////////////////
long DMProcedures::SigCapBeginCapture(LPCTSTR pFormName)
   {
   trace (L6,_T("+SigCapBeginCapture"));
   _TCHAR msg[] = T_ID _T("::SigCapBeginCapture");
   CDeviceStatistics dvs = m_devmgr.GetDeviceStatistics( DMCLASS_SIGCAP );
   if (DM_DA_NOT_CONFIGURED != dvs.cStatus)  // Check if device is configured
      {
      // Leave sigcap device in the same state we found it
      BOOL bSigCapEnableStatus = dvs.fIsEnabled;
      rc = Enable(DMCLASS_SIGCAP, DEVICEID0, true);  
      // enable it(opening is done automatically)
      if ( rc == 0 )
         {
         try
            {
            m_devmgr.SetSigCapNotifyStyle(DEVICEID0, 1);
            m_devmgr.SigCapBeginCapture(DEVICEID0, pFormName);   // begin to capture signature
            }
         PROCESS_EXCEPTIONS(DMCLASS_SIGCAP);	
         if ( rc != 0 
            && bSigCapEnableStatus == false )
            Enable(DMCLASS_SIGCAP, DEVICEID0, false);
         }
      }
   trace (L6,_T("-SigCapBeginCapture 0x%x"),rc);
   return rc;
   }

////////////////////////////////////////////////////
long DMProcedures::SigCapEndCapture()
   {
   trace (L6,_T("+SigCapEndCapture"));
   _TCHAR msg[] = T_ID _T("::SigCapEndCapture");

   // assume the device is configured, since SigCapBeginCapture has been called succesfully
   // if it is not, it is programing error.
   CDeviceStatistics dvs = m_devmgr.GetDeviceStatistics( DMCLASS_SIGCAP );
   ASSERT(DM_DA_NOT_CONFIGURED != dvs.cStatus)  ;
   try
      {
      rc = 0;
      m_devmgr.SigCapEndCapture(DEVICEID0);
      }
   PROCESS_EXCEPTIONS(DMCLASS_SIGCAP);
   Enable(DMCLASS_SIGCAP, DEVICEID0, false);
   trace (L6,_T("-SigCapEndCapture 0x%x"),rc);
   return rc;
   }

////////////////////////////////////////////////////
long DMProcedures::SigCapSetWindowHandle(long* pHWnd)
   {
   trace (L6,_T("+SigCapSetWindowHandle"));
   _TCHAR msg[] = T_ID _T("::SigCapSetWindowHandle");
   CDeviceStatistics dvs = m_devmgr.GetDeviceStatistics( DMCLASS_SIGCAP );
   if (DM_DA_NOT_CONFIGURED != dvs.cStatus)  // Check if device is configured
      {
	   try

	      {
         rc = 0;
		   m_devmgr.SigCapSetWindowHandle(DEVICEID0, pHWnd);   // end capturing signature
	      }
	   PROCESS_EXCEPTIONS(DMCLASS_SIGCAP);	
      }
   trace (L6,_T("-SigCapSetWindowHandle 0x%x"),rc);
   return rc;
   }

////////////////////////////////////////////////////
long DMProcedures::SigCapEnable()
   {
   trace (L6,_T("+SigCapEnable"));
   rc = Enable(DMCLASS_SIGCAP, DEVICEID0, true);  // enable it(opening is done automatically)
   trace (L6,_T("-SigCapEnable 0x%x"),rc);
   return rc;
   }

////////////////////////////////////////////////////
long DMProcedures::SigCapDiable()
   {
   trace (L6,_T("+SigCapDiable"));
   rc = Enable(DMCLASS_SIGCAP, DEVICEID0, false);  // enable it(opening is done automatically)
   trace (L6,_T("-SigCapDiable 0x%x"),rc);
   return rc;
   }

////////////////////////////////////////////////////
long DMProcedures::GetSigCapNotifyStyle()
   {
   trace (L6,_T("+GetSigCapNotifyStyle"));
   _TCHAR msg[] = T_ID _T("::GetSigCapNotifyStyle");
   CDeviceStatistics dvs = m_devmgr.GetDeviceStatistics( DMCLASS_SIGCAP );
   if (DM_DA_NOT_CONFIGURED != dvs.cStatus)  // Check if device is configured
      {
      try
         {
         rc = m_devmgr.GetSigCapNotifyStyle(DEVICEID0);   // 
         }
      PROCESS_EXCEPTIONS(DMCLASS_SIGCAP);	
      }
   trace (L6,_T("-GetSigCapNotifyStyle 0x%x"),rc);
   return rc;
   }

////////////////////////////////////////////////////
long DMProcedures::SigCapDataToBitmap(LPCTSTR pFormName)
   {
   trace (L6,_T("+SigCapDataToBitmap"));
   _TCHAR msg[] = T_ID _T("::SigCapDataToBitmap");

   CDeviceStatistics dvs = m_devmgr.GetDeviceStatistics( DMCLASS_SIGCAP );
   if (DM_DA_NOT_CONFIGURED != dvs.cStatus)  // Check if device is configured
      {
      try
         {
         m_devmgr.SigCapDataToBitmap(DEVICEID0, pFormName);   // 
         }
      PROCESS_EXCEPTIONS(DMCLASS_SIGCAP);
      }
   trace (L6,_T("-SigCapDataToBitmap 0x%x"),rc);
   return rc;
   }

////////////////////////////////////////////////////
long DMProcedures::SigCapDataTo5991Format(BSTR *pbsData)
   {
   trace (L6,_T("+SigCapDataTo5991Format"));
   _TCHAR msg[] = T_ID _T("::SigCapDataTo5991Format");

   CDeviceStatistics dvs = m_devmgr.GetDeviceStatistics( DMCLASS_SIGCAP );
   if (DM_DA_NOT_CONFIGURED != dvs.cStatus)  // Check if device is configured
      {
      try
         {
         m_devmgr.SigCapDataTo5991Format(DEVICEID0, pbsData);
         }
      PROCESS_EXCEPTIONS(DMCLASS_SIGCAP);
      }
   trace (L6,_T("-SigCapDataTo5991Format 0x%x"),rc);
   return rc;
   }
// End Signature Capture Modifications

///////////////////////////////////////
// PinPad in non-encrypt mode functions is like a the keyboard
long DMProcedures::PinPadEnable( void )
   {
   trace(L6,_T("+PinPadEnable"));
   rc = Enable(DMCLASS_ENCRYPTOR, DEVICEID0, true );
   trace(L6,_T("-PinPadEnable 0x%x"),rc);
   return rc;
   }

//////////////////////////////////////
long DMProcedures::PinPadDisable(void)
   {
   trace(L6,_T("+PinPadDisable"));
   rc = Enable(DMCLASS_ENCRYPTOR, DEVICEID0, false );
   trace(L6,_T("-PinPadDisable 0x%x"),rc);
   return rc;
   }

///////////////////////////////////////////
// KeyLock operations
long DMProcedures::GetKeylockPosition(void)
{
   trace(L6,_T("+GetKeylockPosition"));
   _TCHAR msg[] = T_ID _T("::GetKeylockPosition");
   rc = Enable(DMCLASS_KEYLOCK, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         CKeylockWrapper* pKeylock = m_devmgr.GetKeylock( DEVICEID0 );
         rc = pKeylock->GetKeyPosition();

         long lPosition =       
            rc == 1 ? DM_KEYLOCK_L  :
            rc == 2 ? DM_KEYLOCK_N  :
            rc == 3 ? DM_KEYLOCK_S  :
            rc == 4 ? DM_KEYLOCK_X  : 0;

         rc = lPosition;
         }

      PROCESS_EXCEPTIONS(DMCLASS_KEYLOCK);	
      }
   trace(L6,_T("-GetKeylockPosition 0x%x"),rc);
   return rc;
}

//////////////////////////////////////
// Scale operations
long DMProcedures::ScaleEnable( void )
   {
   trace(L6,_T("+ScaleEnable"));
   rc = Enable(DMCLASS_SCALE, DEVICEID0, true );
   trace(L6,_T("-ScaleEnable"));
   return rc;
   }

/////////////////////////////////////
long DMProcedures::ScaleDisable(void)
   {
   trace(L6,_T("+ScaleDisable"));
   rc = Enable(DMCLASS_SCALE, DEVICEID0, false );
   trace(L6,_T("-ScaleDisable"));
   return rc;
   }

///////////////////////////////////////
long DMProcedures::GetScaleWeight(void)// proper way to obtain weight
   {
   trace(L6,_T("+GetScaleWeight"));
   _TCHAR msg[] = T_ID _T("::GetScaleWeight");
   rc = Enable( DMCLASS_SCALE, DEVICEID0, true );
   if ( rc == 0 )
      {
      try
         {
         CScaleWrapper* pScale = m_devmgr.GetScaleWrapper( DEVICEID0 );
ASSERT( FALSE );
//         rc = m_devmgr.GetScaleWeight( DEVICEID0 );
         }
      PROCESS_EXCEPTIONS(DMCLASS_SCALE);
      }
   trace(L6,_T("-GetScaleWeight : %d"), rc);
   return rc;
   }

////////////////////////////////////////
bool DMProcedures::IsScaleEnable(void)
   {
   trace(L6, _T("+IsScaleEnable"));
   trace(L6, _T("-IsScaleEnable"));
   return m_devmgr.GetDeviceStatistics( DMCLASS_SCALE  ).cStatus == DM_DA_LOADED;
   }

////////////////////////////////////////
bool DMProcedures::IsCashAcceptorAvailable(void)
   {
   trace(L6, _T("+IsCashAcceptorAvailable"));
   trace(L6, _T("-IsCashAcceptorAvailable"));
   return m_devmgr.GetDeviceStatistics( DMCLASS_CASHACCEPTOR ).cStatus == DM_DA_LOADED
      || m_devmgr.GetDeviceStatistics( DMCLASS_CASHRECYCLER).cStatus == DM_DA_LOADED;
   }

////////////////////////////////////////
bool DMProcedures::IsCoinAcceptorAvailable(void)
   {
   trace(L6, _T("+IsCoinAcceptorAvailable"));
   trace(L6, _T("-IsCoinAcceptorAvailable"));
   return m_devmgr.GetDeviceStatistics( DMCLASS_COINACCEPTOR ).cStatus == DM_DA_LOADED
      || m_devmgr.GetDeviceStatistics( DMCLASS_CASHRECYCLER).cStatus == DM_DA_LOADED;
   }

//////////////////////////////////////// awk.1
bool DMProcedures::IsCashDrawerAvailable(void)
   {
	trace(L6, _T("+/-IsCashDrawerAvailable"));
   return m_devmgr.GetDeviceStatistics( DMCLASS_CASHDRAWER ).cStatus == DM_DA_LOADED;
   }

////////////////////////////////////////
// Scanner operations
long DMProcedures::ScannerEnable( void )
   {
   trace(L6,_T("+ScannerEnable"));
   if (co.IsRemoteScannerConnected())
   {
       rc = Enable(DMCLASS_SCANNER, DEVICEID1, true ); 
       if (rc)
       {
           trace(L6,_T("Error %x enabling remote scanner"), rc);
       }
   }
   rc = Enable(DMCLASS_SCANNER, DEVICEID0, true );
   bScannerIsEnabled = true;		// RFC 219283 DE/RF 7401 Enables Motion Sensor on 7883 scanner
   // No need to do this every time, we will just do it in 
   // DMProceduresInitialize once.
   //if (rc>=0) rc = dmi.SetScannerAutoDisable(true);
   trace(L6,_T("-ScannerEnable"));
   return rc;
   }

/////////////////////////////////////////
long DMProcedures::ScannerDisable( void )
   {
   trace(L6,_T("+ScannerDisable"));
   if (co.IsRemoteScannerConnected())
   {
       rc = Enable(DMCLASS_SCANNER, DEVICEID1, false );
       if (rc)
       {
           trace(L6,_T("Error %x enabling remote scanner"), rc);
       }
   }
   rc = Enable(DMCLASS_SCANNER, DEVICEID0, false );
   bScannerIsEnabled = false;		// RFC 219283 DE/RF 7401 Enables Motion Sensor on 7883 scanner
   trace(L6,_T("-ScannerDisable"));
   return rc;
   }

// RFC 219283 DE/RF 7401 Enables Motion Sensor on 7883 scanner
bool DMProcedures::IsScannerEnabled(void)
{
	trace(L7,_T("IsScannerEnabled (%s)"),bScannerIsEnabled ? _T("TRUE") : _T("FALSE"));
	return bScannerIsEnabled;
}

////////////////////////////////////////////////////
long DMProcedures::GetScannerPacesetterEnabled(void)
   {
   trace(L6,_T("+GetScannerPacesetterEnabled"));
   _TCHAR msg[] = T_ID _T("::GetScannerPacesetterEnabled");
   rc = Enable(DMCLASS_SCANNER, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         CScannerWrapper* pScanner = m_devmgr.GetScanner(DEVICEID0);
         rc = pScanner->GetPacesetterEnabled();
         }
      PROCESS_EXCEPTIONS(DMCLASS_SCANNER);	
      }
   trace(L6,_T("-GetScannerPacesetterEnabled"));
   return rc;
   }

//////////////////////////////////////////////////////////////
long DMProcedures::SetScannerPacesetterEnabled(
   bool bNewValue)
   {
   trace(L6,_T("+SetScannerPacesetterEnabled"));
   _TCHAR msg[] = T_ID _T("::SetScannerPacesetterEnabled");
   rc = Enable(DMCLASS_SCANNER, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         CScannerWrapper* pScanner = m_devmgr.GetScanner( DEVICEID0 );
         rc = pScanner->SetScannerPacesetterEnabled( bNewValue );
         m_devmgr.ThrowDmException( pScanner );
         }
      PROCESS_EXCEPTIONS(DMCLASS_SCANNER);
      }
   trace(L6,_T("-SetScannerPacesetterEnabled"));
   return rc;
   }



void DMProcedures::SetScannerAutoDisable(
   bool bNewValue)
   {
   trace(L6,_T("+SetScannerAutoDisable"));
   _TCHAR msg[] = T_ID _T("::SetScannerPacesetterEnabled");
   rc = Enable(DMCLASS_SCANNER, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         CScannerWrapper* pScanner = m_devmgr.GetScanner( DEVICEID0 );
         pScanner->SetAutoDisable( bNewValue );
         m_devmgr.ThrowDmException( pScanner );
         }
      PROCESS_EXCEPTIONS(DMCLASS_SCANNER);
      }
   trace(L6,_T("-SetScannerAutoDisable"));
   return ;
   }

//BeginLocaleCode	
long DMProcedures::SetScannerDecodeData(bool bNewValue)
   {
   trace(L6,_T("+SetScannerDecodeData"));
   _TCHAR msg[] = T_ID _T("::SetScannerDecodeData");
   rc = Enable(DMCLASS_SCANNER, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         CScannerWrapper* pScanner = m_devmgr.GetScanner( DEVICEID0 );
         pScanner->SetDecodeData( bNewValue );
         m_devmgr.ThrowDmException( pScanner );
         }
      PROCESS_EXCEPTIONS(DMCLASS_SCANNER);	
      }
   trace(L6,_T("-SetScannerDecodeData"));  
   return rc;
   }

#if 0 

CString DMProcedures::GetScannerDataLabel(void)
   {
   trace(L6,_T("+GetScannerDataLabel"));
   _TCHAR msg[] = T_ID _T("::GetScannerDataLabel");
   CString csRet;
   try
      {
      CScannerWrapper* pScanner = m_devmgr.GetScanner( DEVICEID0 );
      csRet = pScanner->GetScanDataLabel( );
      }
   PROCESS_EXCEPTIONS( DMCLASS_SCANNER ); 
   trace(L6,_T("-GetScannerDataLabel"));
   return csRet;
   }

long DMProcedures::GetScannerDataType(void)
   {
   trace(L6,_T("+GetScannerDataType"));
   _TCHAR msg[] = T_ID _T("::GetScannerDataType");
   try
      {
      CScannerWrapper* pScanner = m_devmgr.GetScanner( DEVICEID0 );
      rc = pScanner->GetScanDataType( );
      }
   PROCESS_EXCEPTIONS(DMCLASS_SCANNER ); 
   trace(L6,_T("-GetScannerDataType"));
   return rc;
   }

#endif
//EndLocaleCode 

long DMProcedures::ScannerRemoteAccess( BOOL bAcquire )
   {
   trace(L6,_T("+ScannerRemoteAccess"));
   _TCHAR msg[] = T_ID _T("::ScannerRemoteAccess");
   try
      {
      CScannerWrapper* pScanner = m_devmgr.GetScanner( DEVICEID1 );
      BSTR bstrData = NULL;
      rc = pScanner->DirectIO( NCRDIO_SCANNER_CONNECT, (long*)&bAcquire, &bstrData );
      ::SysFreeString( bstrData );
      }
   PROCESS_EXCEPTIONS( DMCLASS_SCANNER ); 

   trace(L6,_T("-ScannerRemoteAccess"));
   return rc;
   }


CString DMProcedures::ScannerGetDeviceName(void)
   {
   trace(L6,_T("+ScannerGetDeviceName"));
   _TCHAR msg[] = T_ID _T("::ScannerGetDeviceName");
   CString csDeviceName;
   try
      {
      CScannerWrapper* pScanner = m_devmgr.GetScanner( DEVICEID0 );
      csDeviceName = pScanner->GetDeviceName( );
      }
   PROCESS_EXCEPTIONS(DMCLASS_SCANNER ); 
   trace(L6,_T("-ScannerGetDeviceName %s"), csDeviceName );
   return csDeviceName;
   }

//Begin RFC 356020: Support Check payment on lane using printer 7167
CString DMProcedures::MICRGetDeviceName(void)
   {
   trace(L6,_T("+MICRGetDeviceName"));
   _TCHAR msg[] = T_ID _T("::MICRGetDeviceName");
   CString csDeviceName;
   try
      {
      CMICRWrapper* pMicr = m_devmgr.GetMICR( DEVICEID0 );
      csDeviceName = pMicr->GetDeviceName( );
      }
   PROCESS_EXCEPTIONS(DMCLASS_SCANNER ); 
   trace(L6,_T("-MICRGetDeviceName %s"), csDeviceName );
   return csDeviceName;
   }
//End RFC 356020

/////////////////////////////////////////////
// Methods for the MISC - SCOT Related Lights

////////////////////////////////////////
void DMProcedures::saveLaneLightStatus()
   {
   trace(L6,_T("+saveLaneLight"));
   m_nLaneLightStatusSaved	= m_nLaneLightStatus;
   trace(L6,_T("-saveLaneLight"));
   }

/////////////////////////////////////
void DMProcedures::restoreLaneLight()
   {
   trace(L6,_T("+restoreLaneLight"));
   m_nLaneLightStatus = m_nLaneLightStatusSaved;
   RemoteRelay(m_nLaneLightStatus);
   trace(L6,_T("-restoreLaneLight"));
   }

/////////////////////////////////////////////////////////////////
void DMProcedures::setTriColorLight(
   DMTriColorLightColor color,
   DMTriColorLightState state,
   DMTriColorLightRequestType request )
   {
   trace(L6,_T("+setTriColorLight, color = %d state = %d request = %d"),
      color, state, request );
   _TCHAR msg[] = T_ID _T("::setTriColorLight");
   if ( g_nTimesInSetTriColorLight++ > 0
      && g_bCanDumpStack ) // only dump stack once if we re-enter
      {
      // We re-entered this function which is a problem we are trying to figure out.
      // Therefore, let's dump the stack.
      trace(L6, _T("!!Re-entered setTriColorLight!!"));
      traceStack(L6);
      g_bCanDumpStack = false;
      }
   if ( color == DM_TRICOLORLIGHT_GREEN 
      && state == DM_TRICOLORLIGHT_ON
      && (m_fIsCoinsOrBillsVeryLow || (GetPrinterPaperLow() && IsDeviceRemote(DMCLASS_RECEIPT, 0)))) 
      state = DM_TRICOLORLIGHT_BLINK_1HZ;				
   rc = Enable(DMCLASS_MISC, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         m_theTriColorLight.set(color, state, request, m_devmgr, DEVICEID0);
         }
      PROCESS_EXCEPTIONS(DMCLASS_MISC);
	     if((rc != 0) && !fPendingMiscIFError)   // Tar 208394
		 {
            trace(L6, _T("setTriColorLight PostDmEventToApplication rc=%x"),rc); 
            fPendingMiscIFError = true;
            mo.PostDM(DM_ERROR, DMCLASS_MISC, 0, rc, NULL);
		 }
      }
   // The above line will return to here even if we catch and process exceptions,
   // so we are safe using a static int to signify if we ever re-enter this function.
   if ( --g_nTimesInSetTriColorLight == 0)
      g_bCanDumpStack = true;
   trace(L6,_T("-setTriColorLight, color = %d state = %d, request = %d"),
      color, state, request);
   return;
   }

/////////////////////////////////////////
void DMProcedures::turnOffTriColorLight()
   {
   trace(L6,_T("+turnOffTriColorLight") );
   _TCHAR msg[] = T_ID _T("::turnOffTriColorLight");
   rc = Enable(DMCLASS_MISC, DEVICEID0, true);
	
   if ( rc == 0 )
      {
      try
         {
         m_theTriColorLight.off(m_devmgr, DEVICEID0);
         }
      PROCESS_EXCEPTIONS(DMCLASS_MISC);
	     if((rc != 0) && !fPendingMiscIFError)   // Tar 208394
		 {
            trace(L6, _T("turnOffTriColorLight PostDmEventToApplication rc=%x"),rc); 
            fPendingMiscIFError = true;
            mo.PostDM(DM_ERROR, DMCLASS_MISC, 0, rc, NULL);
		 }
      }
   trace(L6,_T("-turnOffTriColorLight"));
   }
////////////////////////////////////////////
void DMProcedures::TriColorLightEndTransaction()
{  
   trace(L6,_T("+TriColorLightEndTransaction") );
   // Sets end of transaction for pager functions which, for some
   // strange reason, are implemented through the tri light
   m_theTriColorLight.endTransaction();
   trace(L6,_T("-TriColorLightEndTransaction"));
}

////////////////////////////////////////////
void DMProcedures::saveTriColorLightStatus()
   {
   trace(L6,_T("+backupTriColorLight") );
   m_theTriColorLight.backup();
   trace(L6,_T("-backupTriColorLight"));
   }

////////////////////////////////////////////
void DMProcedures::restoreTriColorLight()
   {
  trace(L6,_T("+restoreTriColorLight") );
   _TCHAR msg[] = T_ID _T("::restoreTriColorLight");
   rc = Enable(DMCLASS_MISC, DEVICEID0, true);

   if ( rc == 0 )
      {
      try
         {
         m_theTriColorLight.restore(m_devmgr, DEVICEID0);
         }
      PROCESS_EXCEPTIONS(DMCLASS_MISC);
	     if((rc != 0) && !fPendingMiscIFError)   // Tar 208394
		 {
            trace(L6, _T("restoreTriColorLight PostDmEventToApplication rc=%x"),rc); 
            fPendingMiscIFError = true;
            mo.PostDM(DM_ERROR, DMCLASS_MISC, 0, rc, NULL);
		 }
      }
   trace(L6,_T("-restoreTriColorLight"));
   }

///////////////////////////////////////////
long DMProcedures::RemoteRelay( int state )
{
  trace(L6,_T("+RemoteRelay, state = %d "), state );
   _TCHAR msg[] = T_ID _T("::RemoteRelay");
   rc = Enable(DMCLASS_MISC, DEVICEID0, true);

   if ( rc == 0)
      {
      try
         {
         m_devmgr.MiscSetIndicator(DEVICEID0,                 // DeviceID
            DM_MISC_REMOTE_RELAY,	     // Lane Light
            state);								     // State
         // TAR# 134878 - self-checkout light works in conjunction with lane light
         m_devmgr.MiscSetIndicator(DEVICEID0,                 // DeviceID
            DM_MISC_SELFCHECKOUT_MODE,	 // Self-Checkout Light
            state);								     // State
         m_nLaneLightStatus = state;
         }
      PROCESS_EXCEPTIONS(DMCLASS_MISC);
	     if((rc != 0) && !fPendingMiscIFError)   // Tar 208394
		 {
            trace(L6, _T("RemoteRelay PostDmEventToApplication rc=%x"),rc); 
            fPendingMiscIFError = true;
            mo.PostDM(DM_ERROR, DMCLASS_MISC, 0, rc, NULL);
		 }
      }
   trace(L6,_T("-RemoteRelay, state = %d "), state );
   return rc;
   }

///////////////////////////////////////////////
long DMProcedures::CardReaderLight( int state )
   {
   trace(L6,_T("+CardReaderLight, state = %d "), state );
   _TCHAR msg[] = T_ID _T("::CardReaderLight");
   rc = Enable(DMCLASS_MISC, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         m_devmgr.MiscSetIndicator(DEVICEID0,// DeviceID
            DM_MISC_MEDIA_INDICATOR1,   // Indicator
            state);										  // State
         }
      PROCESS_EXCEPTIONS(DMCLASS_MISC);
	     if((rc != 0) && !fPendingMiscIFError)   // Tar 208394
		 {
            trace(L6, _T("CardReaderLight PostDmEventToApplication rc=%x"),rc); 
            fPendingMiscIFError = true;
            mo.PostDM(DM_ERROR, DMCLASS_MISC, 0, rc, NULL);
		 }
      }
   trace(L6,_T("-CardReaderLight, state = %d "), state );
   return rc;
   }

///////////////////////////////////////////////
long DMProcedures::DepositoryLight( int state )
   {
   trace(L6,_T("+DepositoryLight, state = %d "), state );
   if( state == m_nDepositoryLightState )
      return 0;
   m_nDepositoryLightState = DM_MISC_STATE_UNKNOWN;
   _TCHAR msg[] = T_ID _T("::DepositoryLight");
   rc = Enable(DMCLASS_MISC, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         m_devmgr.MiscSetIndicator(DEVICEID0,// DeviceID
            DM_MISC_MEDIA_INDICATOR2,	  // Indicator
            state);										  // State
         }
      PROCESS_EXCEPTIONS(DMCLASS_MISC);
	     if((rc != 0) && !fPendingMiscIFError)   // Tar 208394
		 {
            trace(L6, _T("DepositoryLight PostDmEventToApplication rc=%x"),rc); 
            fPendingMiscIFError = true;
            mo.PostDM(DM_ERROR, DMCLASS_MISC, 0, rc, NULL);
		 }
      m_nDepositoryLightState = state;
      }
   trace(L6,_T("-DepositoryLight, state = %d "), state );
   return rc;
   }

/////////////////////////////////////////////
long DMProcedures::PassbookLight( int state )
   {
   trace(L6, _T("+PassbookLight"));
   _TCHAR msg[] = T_ID _T("::PassbookLight");
   rc = Enable(DMCLASS_MISC, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         m_devmgr.MiscSetIndicator(DEVICEID0,// DeviceID
            DM_MISC_MEDIA_INDICATOR3,	  // Indicator
            state);										  // State
         }
      PROCESS_EXCEPTIONS(DMCLASS_MISC);
	     if((rc != 0) && !fPendingMiscIFError)   // Tar 208394
		 {
            trace(L6, _T("PassbookLight PostDmEventToApplication rc=%x"),rc); 
            fPendingMiscIFError = true;
            mo.PostDM(DM_ERROR, DMCLASS_MISC, 0, rc, NULL);
		 }
      }
   trace(L6, _T("-PassbookLight 0x%x"),rc);
   return rc;
   }

/////////////////////////////////////////////
long DMProcedures::CoinAcceptorLight( int state )
   {
   trace(L6,_T("+CoinAcceptorLight, state = %d "), state );
   _TCHAR msg[] = T_ID _T("::CoinAcceptorLight");
   if( state == m_nCoinAcceptorLightState  )
      return 0;
   m_nCoinAcceptorLightState = DM_MISC_STATE_UNKNOWN;
   rc = Enable(DMCLASS_MISC, DEVICEID0, true);
   if (rc == 0)
      {
      try
         {
         m_devmgr.MiscSetIndicator(DEVICEID0,// DeviceID
            DM_MISC_MEDIA_INDICATOR4,	  // Indicator
            state);										  // State
         }
      PROCESS_EXCEPTIONS(DMCLASS_MISC);
	     if((rc != 0) && !fPendingMiscIFError)   // Tar 208394
		 {
            trace(L6, _T("CoinAcceptorLight PostDmEventToApplication rc=%x"),rc); 
            fPendingMiscIFError = true;
            mo.PostDM(DM_ERROR, DMCLASS_MISC, 0, rc, NULL);
		 }
      m_nCoinAcceptorLightState = state;
      }
   trace(L6,_T("-CoinAcceptorLight, state = %d "), state );
   return rc;
   }

/////////////////////////////////////////////
CString DMProcedures::GetDevicesInError(void)
   {
   trace(L6, _T("+/-GetDevicesInError"));
   return m_csDevicesInError;
   }

///////////////////////////////////////////
bool DMProcedures::GetCoinsOrBillsVeryLow()
   {
   trace(L6, _T("+/-GetCoinsOrBillsVeryLow (%s)"), m_fIsCoinsOrBillsVeryLow? _T("true") : _T("false"));   // TAR330965
   return m_fIsCoinsOrBillsVeryLow || m_fIsCoinsVeryLow || m_fIsBillsVeryLow;
   }

/////////////////////////////////////////////////////
void DMProcedures::SetCoinsOrBillsVeryLow(bool value)
   {
   trace(L6, _T("+SetCoinsOrBillsVeryLow"));
   m_fIsCoinsOrBillsVeryLow = value;  
   trace(L6, _T("-SetCoinsOrBillsVeryLow (%s)"), m_fIsCoinsOrBillsVeryLow? _T("true") : _T("false"));  // TAR330965
   return; 
   }

///////////////////////////////////////////
bool DMProcedures::GetCoinsVeryLow()
   {
   trace(L6, _T("+/-GetCoinsVeryLow (%s)"), m_fIsCoinsVeryLow? _T("true") : _T("false"));  // TAR330965
   return m_fIsCoinsVeryLow;
   }

/////////////////////////////////////////////////////
void DMProcedures::SetCoinsVeryLow(bool value)
   {
   trace(L6, _T("+SetCoinsVeryLow"));
   if (value)
   { //dp185016 tar 310480
     SetCoinsOrBillsVeryLow(value);
   }
   m_fIsCoinsVeryLow = value;  
   trace(L6, _T("-SetCoinsVeryLow (%s)"), m_fIsCoinsVeryLow? _T("true") : _T("false"));  // TAR330965
   return; 
   }

///////////////////////////////////////////
bool DMProcedures::GetBillsVeryLow()
   {
   trace(L6, _T("+/-GetBillsVeryLow (%s)"), m_fIsBillsVeryLow? _T("true") : _T("false"));  // TAR330965
   return m_fIsBillsVeryLow;
   }

/////////////////////////////////////////////////////
void DMProcedures::SetBillsVeryLow(bool value)
   {
   trace(L6, _T("+SetBillsVeryLow"));
   if (value)
   { //dp185016 tar 310480
     SetCoinsOrBillsVeryLow(value);
   }
   m_fIsBillsVeryLow = value;  
   trace(L6, _T("-SetBillsVeryLow (%s)"), m_fIsBillsVeryLow? _T("true") : _T("false"));  // TAR330965
   return; 
   }

///////////////////////////////////////////
// Added for TAR 283924
bool DMProcedures::GetCoinDispenserWithLowSensor()
   {
   trace(L6, _T("+/-GetCoinDispenserWithLowSensor"));
   return m_bCoinDispenserWithLowSensor;
   }

//TAR408696+
///////////////////////////////////////////
void DMProcedures::SetPrinterPaperLow(bool value)
   {
   trace(L6, _T("+SetPrinterPaperLow"));
   m_bPrinterPaperLow = value;  
   trace(L6, _T("-SetPrinterPaperLow (%s)"), m_bPrinterPaperLow? _T("true") : _T("false"));  
   return; 
   }
//TAR408696-

bool DMProcedures::GetPrinterPaperLow()
   {
   //trace(L6, _T("+GetPrinterPaperLow"));
   trace(L4,_T("+GetPrinterPaperLow on thread 0x%08.8x"), GetCurrentThreadId()); //408696

   //TAR408696+
   CString csCurrentStateName = mp.sCurr->GetRuntimeClass()->m_lpszClassName;
   if ( !(    ( csCurrentStateName.Find(_T("SMAttract")) != -1 )
	       || ( csCurrentStateName.Find(_T("SMSm")) != -1 ) 
		   || ( csCurrentStateName.Find(_T("SystemMessage")) != -1 )
		   || ( csCurrentStateName.Find(_T("OutOfService")) != -1 )
		   || co.IsInAssistMenus()
		 ) 
	  )
   {
	   trace(L6, _T("Not in store mode and not at Attract, return m_bPrinterPaperLow %d"), m_bPrinterPaperLow);
	   return m_bPrinterPaperLow;
   }
   //TAR408696-
   
   _TCHAR msg[] = T_ID _T("::GetPrinterPaperLow");
   bool bRetVal = false;
   try
   	{
		if ( IsDeviceConfigured( DMCLASS_RECEIPT, DEVICEID0 ) != false )
		{
			CPrintStation* pPrinter = m_devmgr.GetPrintStation( DMCLASS_RECEIPT );
			if(pPrinter != NULL && !co.fOperationsTBControlsPrinter)
			{
				bRetVal = pPrinter->GetPaperNearEmpty() || pPrinter->GetPaperEmpty();
				SetPrinterPaperLow(bRetVal);  //TAR408696
			}
		}
	}
	PROCESS_EXCEPTIONS(DMCLASS_RECEIPT);  // macro with 4 catch{}'s, do not disable

   trace(L6, _T("-GetPrinterPaperLow returning %s"), bRetVal ? _T("true") : _T("false"));
   return bRetVal;  
   }

/////////////////////////////////////
BEGIN_MESSAGE_MAP(DMProcedures, CWnd)
//{{AFX_MSG_MAP(DMProcedures)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BEGIN_EVENTSINK_MAP(DMProcedures, CWnd)
//{{AFX_EVENTSINK_MAP(DMProcedures)
ON_EVENT(DMProcedures, IDC_SCOTDEVICEMANAGER1, 1 /* StatusEvent */, OnStatusEventScotdevicemanager1, VTS_I4 VTS_I4 VTS_I4)
ON_EVENT(DMProcedures, IDC_SCOTDEVICEMANAGER1, 2 /* InputEvent */, OnInputEventScotdevicemanager1, VTS_I4 VTS_I4 VTS_I4 VTS_VARIANT)
//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


////////////////////////////////////////////////////////////////////////////////////////////////
// DMProcedures message handlers

///////////////////////////////////////////////////////
long DMProcedures::ReceiptPrintBarcode(LPCTSTR szBarcode, long     lBCType )
   {
   trace(L6,_T("+ReceiptPrintBarcode %s"), szBarcode);
  // rc = PrintBarcode(szBarcode, DM_BCS_JAN13, 4, DM_ALIGN_CENTER,false, DMCLASS_RECEIPT );
   rc = PrintBarcode(szBarcode, lBCType, 4, DM_ALIGN_CENTER,false, DMCLASS_RECEIPT ); //NN 080202 Added by FZ for RFC 187993

   trace(L6,_T("-ReceiptPrintBarcode 0x%x"), rc);
   return rc;
   }

bool DMProcedures::IsOpen()
   {
   return m_bDMOpened;
   }

//Begin RFC 356020: Support Check payment on lane using printer 7167
bool DMProcedures::IsCheckPaymentAtLaneAvailable()
{
	bool bRet = false;
	//bRet = co.fOperationsPayCheckOnLane && dm.IsDeviceAvailable(DMCLASS_SLIP) && dm.IsDeviceAvailable(DMCLASS_MICR);
	bRet = dm.IsDeviceAvailable(DMCLASS_SLIP) && dm.IsDeviceAvailable(DMCLASS_MICR)  && !ps.RemoteMode();
	return bRet;
}
//End RFC 356020

long DMProcedures::PrinterSetBitmap(
   long     nDeviceClass, 
   CString& csBitmapName, 
   long     nAlign, 
   int      nBmpIndex)
   {
	_TCHAR msg[] = T_ID _T("::SetBitMap");
   rc = -1;
	if ( m_devmgr.GetDeviceStatistics( nDeviceClass ).cStatus==DM_DA_LOADED )
	   {
   	rc = Enable( nDeviceClass,  DEVICEID0, true );
      if ( rc == 0 )
         {
			// Resolve 5-partition path if necessary.
			GET_PARTITION_PATH(csBitmapName, csBitmapName.GetBuffer(_MAX_PATH));
			csBitmapName.ReleaseBuffer();

         try
   		   {
            CPrintStation* pPrinter = m_devmgr.GetPrintStation( DMCLASS_RECEIPT );
            nAlign = MapDmBitmapAlignToOpos ( nAlign );
            long lWidth = PTR_BM_ASIS;
            
			// Start TAR 271776
			SBmpPrintObject tmpPrintObject = co.PrintBmpObjectList[nBmpIndex - 1];
            float sf = tmpPrintObject.scalingFactor;

            if (sf != 1)
            {
				lWidth = GetBitmapWidth(csBitmapName, sf);
						
				if (lWidth == 0) 
				{
					lWidth = PTR_BM_ASIS;
				}
			}  // End TAR 271776
			else if (csBitmapName.Find(_T("WIDTH:")) == 0)
            { 
						 // the caller wants to set bitmap width
						// Until this function is modified to support
						// setting up of width of the bitmap, this work around should be supported
						// When a clean solution is in place, please contact Koteswara -01/16/02
	
			    _TCHAR szBmp[300] = {0};
                _stscanf((LPCTSTR) csBitmapName,_T("WIDTH:%d:%s"), &lWidth, szBmp);
                csBitmapName = szBmp;
             }
			// Fix 354727 TAR When LAN disconnected bitmap fails to print due to URL filename
			if (IsDeviceRemote(nDeviceClass, DEVICEID0))
			{
				// file name should not have local drive references to get our remote so working with files
				GetURLName(csBitmapName);
			}
            rc = pPrinter->SetBitmap( nBmpIndex, csBitmapName, lWidth, nAlign );

			// TAR 307991
			// If running as a non-admin user, the C$ share is not available.
			// For non-admin, the SCOT\\IMAGE folder should be shared as IMAGE.
			if(rc != 0 && GetNonAdminURLName(csBitmapName))
			{
				// Try to set the bitmap again.
				rc = pPrinter->SetBitmap( nBmpIndex, csBitmapName, lWidth, nAlign );
			}
			// end TAR 307991

            m_devmgr.ThrowDmException( pPrinter );
	   	   }
		   PROCESS_EXCEPTIONS(nDeviceClass);  
         }
      }
   return rc;
   }
// TAR 271776

// SA250050 - Core patch to support dynamic SetBitmap calls
//          - Alignment & Width should not be translated from original DM
//            (i.e. PTR_BM_ASIS, PTR_BM_LEFT, PTR_BM_RIGHT, PTR_BM_CENTER)
long DMProcedures::PrinterSetBitmap(
        long     nDeviceClass, 
        CString& csBitmapName, 
        long     nAlign, 
        int      nBmpIndex,
        int      nWidth)
{
    _TCHAR msg[] = T_ID _T("::PrinterSetBitmap");
    rc = -1;
    if ( m_devmgr.GetDeviceStatistics( nDeviceClass ).cStatus==DM_DA_LOADED )
	   {
        rc = Enable( nDeviceClass,  DEVICEID0, true );
        if ( rc == 0 )
        {
            try
            {
                CPrintStation* pPrinter = m_devmgr.GetPrintStation( DMCLASS_RECEIPT );
                
                // file name should not have local drive references to get our remote so working with files
                GetURLName(csBitmapName);
                rc = pPrinter->SetBitmap( nBmpIndex, csBitmapName, nWidth, nAlign );
                
                // TAR 307991
                // If running as a non-admin user, the C$ share is not available.
                // For non-admin, the SCOT\\IMAGE folder should be shared as IMAGE.
                if(rc != 0 && GetNonAdminURLName(csBitmapName))
                {
                    // Try to set the bitmap again.
                    rc = pPrinter->SetBitmap( nBmpIndex, csBitmapName, nWidth, nAlign );
                }
                // end TAR 307991
                
                m_devmgr.ThrowDmException( pPrinter );
            }
            PROCESS_EXCEPTIONS(nDeviceClass);  
        }
    }
    return rc;
}

long DMProcedures::GetBitmapWidth(CString& csBitmapName, float scalingFactor )
{

    long width = 0;

	HBITMAP phBitmap = (HBITMAP) LoadImage( NULL, csBitmapName , IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  
    if (phBitmap )
    {

		BITMAP bmp = {0};
		GetObject(phBitmap, sizeof(BITMAP), (LPSTR)&bmp );
		
		if (bmp.bmWidth!= 0)
		{
		   trace(L6, _T("DMProcedures::GetBitmapWidth( ) - the original bitmap(%s) width is %d"),csBitmapName, bmp.bmWidth);
		   
		   CPrintStation* pPrinter = m_devmgr.GetPrintStation( DMCLASS_RECEIPT );
		  
		   long LineWidth = pPrinter->GetLineWidth();
           long LineDots = 576;         //  Maximum 576 dots per line for 80mm wide receipt
		   if (LineWidth == 585 )       
		   {
			   LineDots = 592;		    //  Maximum 592 dots per line for 82mm wide receipt
           }
		   else if (LineDots == 832 ) 
           { 
			   LineDots = 832;          //  Maximum 832 dots per line for 114mm wide receipt
           } 
		   
		   width = (long)( bmp.bmWidth * scalingFactor * LineWidth/LineDots);
		   trace(L6, _T("DMProcedures::GetBitmapWidth( ) - the width after calculation is %d"),width);

		   if (width > LineWidth) //if bigger than printable zone
		   {
			   width = 0; 
			   trace(L6, _T("DMProcedures::GetBitmapWidth( ) - the bitmap can not print out in printable zone."));
		   }
		   
		}
		else
		{
			trace(L6, _T("DMProcedures::GetBitmapWdith( ) - Can't get bitmap width "));
		}
		
		DeleteObject(phBitmap); 

	}
	
	return width;
}


long DMProcedures::LoadPrintObjects(void)
   {
	SBmpPrintObject tmpPrintObject = co.PrintBmpObjectList[0];
	if (!tmpPrintObject.csBmpName.IsEmpty())
	   {
		PrinterSetBitmap(DMCLASS_RECEIPT, tmpPrintObject.csBmpName, tmpPrintObject.nAlignment,1);
	   }
	////////////////// this is commented because DM doesn't allow to set index and set second bitmap ///////////////
   tmpPrintObject = co.PrintBmpObjectList[1];
	if (!tmpPrintObject.csBmpName.IsEmpty())
	   {
		PrinterSetBitmap(DMCLASS_RECEIPT, tmpPrintObject.csBmpName, tmpPrintObject.nAlignment,2);
	   }
	return 0;
   }

bool DMProcedures::PrinterSlipGetInserted(
   void )
   {
   trace(L6,_T("+PrinterSlipGetInserted"));
   LPTSTR msg=T_ID _T("::PrinterGetPaperEmpty");
   bool bStatus = FALSE;
	try
   	{
      CPrintStation* pPrinter = m_devmgr.GetPrintStation( DMCLASS_SLIP );
      bStatus = !pPrinter->GetPaperEmpty( );
	   }
	PROCESS_EXCEPTIONS(DMCLASS_SLIP);  
	trace(L6, _T("-PrinterSlipGetInserted %d"), bStatus );
	return bStatus;
   }

bool DMProcedures::PrinterSlipGetNearEnd()
   {
   trace(L6,_T("+PrinterSlipGetNearEnd"));
   LPTSTR msg=T_ID _T("::PrinterGetPaperNearEmpty");
   bool bStatus = FALSE;
	try
   	{
      CPrintStation* pPrinter = m_devmgr.GetPrintStation( DMCLASS_SLIP );
      bStatus = (bool)pPrinter->GetPaperNearEmpty( );
	   }
	PROCESS_EXCEPTIONS(DMCLASS_SLIP);  
	trace(L6, _T("-PrinterSlipGetNearEnd %d"), bStatus );
	return bStatus;
   }

void DMProcedures::PrinterSlipInsert(
   bool bInsert)
   {
   trace(L6,_T("+PrinterSlipInsert"));
   LPTSTR msg=T_ID _T("::PrinterSlipInsert");
	rc = Enable( DMCLASS_SLIP, DEVICEID0, true );
	if ( rc == 0 )
	   {
		try
		   {
         CPrintStation* pPrinter = m_devmgr.GetPrintStation( DMCLASS_SLIP );
         if ( bInsert != false )
            rc = pPrinter->BeginInsertion( 0 );
         else 
            rc = pPrinter->BeginRemovalForced( 0 );
		   }
		PROCESS_EXCEPTIONS(DMCLASS_SLIP);	
	   }
	trace(L6, _T("-PrinterSlipInsert"));
	return ;
   }

long DMProcedures::PrinterSlipHome(
   void )
   {
   trace(L6,_T("+PrinterSlipHome"));
   LPTSTR msg=T_ID _T("::PrinterSlipHome");
	rc = Enable( DMCLASS_SLIP, DEVICEID0, true );
	if ( rc == 0 )
	   {
		try
		   {
         CPrintStation* pPrinter = m_devmgr.GetPrintStation( DMCLASS_SLIP );
         rc = pPrinter->EndInsertion( );
         m_devmgr.ThrowDmException( pPrinter );
		   }
		PROCESS_EXCEPTIONS(DMCLASS_SLIP);	
	   }
	trace(L6, _T("-PrinterSlipHome %d"), rc );
	return rc;
   }

long DMProcedures::PrinterSlipFlip(void)
{
   trace(L6,_T("+PrinterSlipFlip"));
   LPTSTR msg=T_ID _T("::PrinterSlipFlip");
   BSTR bstrValue = NULL;
   long lValue = 0;

   long rc = Enable( DMCLASS_SLIP, DEVICEID0, true);
   if ( rc == 0 )
   {
       try
       {
          CPrintStation* pPrinter = m_devmgr.GetPrintStation( DMCLASS_SLIP );
#if 1
          if( pPrinter->GetPaperNearEmpty() || pPrinter->GetPaperEmpty() )
          {
            trace(L6, _T("Reverse feed 8 points since slip is near empty before flip."));
            rc = PrinterFeedLines(DMCLASS_SLIP, -8);
          }
          rc = pPrinter->ChangePrintSide(PTR_PS_OPPOSITE);
#else
          // ***** DirectIO IMPLEMENTATION (START) *****
          // This section is for when OposVer17Compliance=F and ScanFlipSlipTOF=F 

          // After a slip print, the slip may be all completely in the printer
          // and no longer seen by the sensor.  If that's the case bring it back
          // out a bit and check again.
          if( pPrinter->GetPaperEmpty() )
          {
             int iLines = -10;
             trace(L6,
                 _T("SlipFlip requested but tray appears empty, sending reverse ")
                 _T("feed of %d lines in case slip is buried in printer passed the sensor."), iLines);
             rc = PrinterFeedLines(DMCLASS_SLIP, iLines);
          }

          // flip and reverse feed to reposition the slip for printing
          trace(L6, _T("Executing Slip Flip DirectIO Command..."));
          rc = pPrinter->GetPrinter().DirectIO(NCRDIO_PTR_SLIP_FLIP, &lValue, &bstrValue);
          if( rc == OPOS_SUCCESS )
          {
             int iLines = -32;
             trace(L6, _T("Reverse feeding %d lines after flip to reposition slip..."), iLines);
             rc = PrinterFeedLines(DMCLASS_SLIP, iLines);
          }
          // ***** DirectIO IMPLEMENTATION (END) *****
#endif
          m_devmgr.ThrowDmException( pPrinter );
       }
       PROCESS_EXCEPTIONS(DMCLASS_SLIP);
   }
   trace(L6, _T("-PrinterSlipFlip 0x%x"), rc );
   return rc;
}

long DMProcedures::PrinterSlipSetRotateSpecial(long lRotateProperty)
{
   trace(L6,_T("+PrinterSlipSetRotateSpecial(%d)"), lRotateProperty);
   LPTSTR msg=T_ID _T("::PrinterSlipSetRotateSpecial");

   long rc = Enable( DMCLASS_SLIP, DEVICEID0, true);
   if ( rc == 0 )
   {
       try
       {
          CPrintStation* pPrinter = m_devmgr.GetPrintStation( DMCLASS_SLIP );

          pPrinter->GetPrinter().SetRotateSpecial(lRotateProperty);
          m_devmgr.ThrowDmException( pPrinter );
       }
       PROCESS_EXCEPTIONS(DMCLASS_SLIP);
   }
   trace(L6, _T("-PrinterSlipSetRotateSpecial 0x%x"), rc );
   return rc;
}


long DMProcedures::PrinterSlipGetRotateSpecial(long & lRotateProperty)
{
   trace(L6,_T("+PrinterSlipGetRotateSpecial()"));
   LPTSTR msg=T_ID _T("::PrinterSlipGetRotateSpecial");

   long rc = Enable( DMCLASS_SLIP, DEVICEID0, true);
   if ( rc == 0 )
   {
       try
       {
          CPrintStation* pPrinter = m_devmgr.GetPrintStation( DMCLASS_SLIP );

          lRotateProperty = pPrinter->GetPrinter().GetRotateSpecial();
          m_devmgr.ThrowDmException( pPrinter );
       }
       PROCESS_EXCEPTIONS(DMCLASS_SLIP);
   }
   trace(L6, _T("-PrinterSlipGetRotateSpecial 0x%x"), rc );
   return rc;
}



long DMProcedures::PrinterFeedLines(
   long nDeviceClass, 
   int iLines)
   {
	trace(L6, _T("+PrinterFeedLines"));
   rc = 0;
   if ( iLines != 0 )
      {
      rc = Enable( nDeviceClass, DEVICEID0, true );
      if ( rc == 0 )
         {
         CString sEscSeq;
         sEscSeq.Format( _T("\x1b|%d%cF"), abs( iLines ), iLines > 0 ? _T('l') : _T('r'));
         rc = PrintLine(nDeviceClass, sEscSeq);
		   }
      }
	trace(L6, _T("-PrinterFeedLines %d"), rc );
   return rc;
   }

   

long DMProcedures::PrintBarcode(
   CString  csBarcode,
   long     lBCType, 
   long     lBCHeight, 
   long     lBCAlign, 
   bool     fPrintNumbers, 
   int      nDeviceClass )
   {
	trace(L6,_T("+PrintBarcode %s"), csBarcode);
   	//TAR 211913 BZ: for use of printing 128-C type barcodes
	CString csEncodedBarcode;
   	bool bBinaryConversion = false; 
	long lBinaryConversion = 0;	//defaut to OPOS_BC_NONE with no conversion
	//TAR 211913 end 

   _TCHAR msg[] = T_ID _T("::PrintBarcode");
   rc = Enable( nDeviceClass, DEVICEID0, true );
   if ( rc == 0)
      {
      try
         {
         CPrintStation* pPrinter = m_devmgr.GetPrintStation( DMCLASS_RECEIPT );
		 if (lBCType < 100)  //Tar 237528, only do mapping if it is not defined in OPosPtr.h
			lBCType = MapDmBarcodeTypeToOpos( lBCType );
         lBCAlign = MapDmBarcodeAlignToOpos( lBCAlign );

         lBCHeight *= pPrinter->GetLineHeight();
         long nWidth = 20 * pPrinter->GetLineWidth() / pPrinter->GetLineChars(); 
 		 
         //TAR 211913 BZ: encode 128-C type barcodes as required by OPOS binary conversion
 		 if (lBCType == 110 && csBarcode.Left(2) == _T("{C")) 
		 {
			csEncodedBarcode = GetEncoded128CBarcode(csBarcode);
			csBarcode = csEncodedBarcode;
			if (!csBarcode.IsEmpty()){
				lBinaryConversion = pPrinter->m_rPtr.GetBinaryConversion();	
				pPrinter->m_rPtr.SetBinaryConversion(1);	//Set BinaryConversion to OPOS_BC_NIBBLE
				bBinaryConversion = true;
			}
		 }
		 trace(L6, _T("pPrinter->PrintBarcode: csBarcode=%s lBCType=%d lBCHeight=%d nWidth=%d lBCAlign=%d"), 
				csBarcode, lBCType, lBCHeight, nWidth, lBCAlign);
 		 //TAR 211913 end
         rc = pPrinter->PrintBarcode(
            csBarcode, lBCType, 
            lBCHeight, nWidth,  // barcode height, width (zero width causes DMx to scale like DM)
            lBCAlign, fPrintNumbers ? PTR_BC_TEXT_BELOW : PTR_BC_TEXT_NONE );
		 //TAR 211913 BZ: restore the original value of BinaryConversion
		 if (bBinaryConversion) {

			 pPrinter->m_rPtr.SetBinaryConversion(lBinaryConversion);
		 }
		 //TAR 211913 end

         m_devmgr.ThrowDmException( pPrinter );
         }
      PROCESS_EXCEPTIONS(nDeviceClass);
      }
   trace(L6,_T("-PrintBarcode 0x%x"), rc);
   return rc;
   }

//TAR 211913 BZ: this routine is used to encode 128-c type barcodes as required by OPOS binary convesion
CString DMProcedures::GetEncoded128CBarcode(CString csBarcodeData)
{
	trace(L6,_T("+GetEncoded128CBarcode %s"), csBarcodeData);

	long lBarcodeDataLength;	//The number of characters embedded in the barcode
	CString csEncodedData = _T("");

	csEncodedData = csBarcodeData;
	//remove the leading characters "{C" - this is just for barcode type
	csEncodedData =  csEncodedData.Mid(2);
	lBarcodeDataLength = csEncodedData.GetLength();

    if (lBarcodeDataLength%2 != 0)
	{
		trace(L6,_T("-GetEncoded128CBarcode: original barcode is not in pairs %s"), csBarcodeData);
		return csBarcodeData;
	}

	TCHAR *pData = new TCHAR[lBarcodeDataLength + 1];
    memset(pData,0,(lBarcodeDataLength + 1) * sizeof(_TCHAR));


	//parse the original barcode data in pairs, each pair takes two characters
	//and translate the pair to the printable format required by OPOS binary conversion
	for (int i=0; i<lBarcodeDataLength/2; i++)
	{
		TCHAR a[8] = {0};
        _stprintf(a, _T("%02x"), _ttoi(csEncodedData.Mid(i*2, 2)));
		pData[i*2]=a[0];
		pData[i*2+1]=a[1];
	}
	//replace unprintable characters according to ASCII table
	for (i=0; i<lBarcodeDataLength;i++)	
	{
		if (pData[i]>='A' && pData[i]<='F')
		{
			pData[i] -= 7;
		}
		if (pData[i]>='a' && pData[i]<='f')
		{
			pData[i] -= 39;
		}
	}
    pData[lBarcodeDataLength] = '\0';
    
	csEncodedData = pData;	//done with encoding 
	delete [] pData;

    if (csEncodedData.GetLength() != (csBarcodeData.GetLength() - 2))
	{
		trace(L6,_T("-GetEncoded128CBarcode: barcode was mistakenly truncated, new barcode is %s"), csEncodedData);
	}
	else
		trace(L6,_T("-GetEncoded128CBarcode: barcode %s was successfully encoded."), csEncodedData);

	return csEncodedData;
}
//TAR 211913 end

long DMProcedures::PrintBitmap(
   CString  csBitmap, 
   long     nAlign, 
   int      nDeviceClass)
   {
	trace(L6,_T("+PrintBitmap %s"), csBitmap);
	
	//TAR 195179 & 191490 start

	CString csSavedBitmap =  TEMP_SIGNATURE_DIRECTORY; //"%DATA_DRIVE%\\scot\\data\\tempprint\\";
	GET_PARTITION_PATH(TEMP_SIGNATURE_DIRECTORY, csSavedBitmap.GetBuffer(_MAX_PATH));
	csSavedBitmap.ReleaseBuffer();

	// Fix 354727 TAR When LAN disconnected bitmap fails to print due to URL filename
	if (IsDeviceRemote(nDeviceClass, DEVICEID0))
	{
		// file name should not have local drive references to get our remote so working with files
		GetURLName(csSavedBitmap); // TAR:230644
		GetURLName(csBitmap);      // TAR:230644
	}
	rc = CreateDirectory(csSavedBitmap, NULL);

	// TAR 308999 - Begin
	if(rc == 0 && GetLastError() == ERROR_ACCESS_DENIED)// CreateDirectory() failed.
	{
		if(GetNonAdminURLName(csSavedBitmap))			// Are we in non-admin mode?
		{
			rc = CreateDirectory(csSavedBitmap, NULL);	// Try again.
		}
	}
	// TAR 308999 - End

	time_t systime;
	time(&systime);

	static long adtoname = 0;

	if (adtoname == 99999)
		adtoname = 0;

	adtoname++;

	_TCHAR TimeInMS[100] = {0};

	_stprintf(TimeInMS,_T("%d%d"),systime,adtoname);

	csSavedBitmap += TimeInMS;
	csSavedBitmap += _T("ncr.bmp");
	rc = CopyFile(csBitmap, csSavedBitmap,false);

	// TAR 308999 - Begin
	if(rc == 0 && GetNonAdminURLName(csBitmap))			// CopyFile() failed.
	{
		GetNonAdminURLName(csSavedBitmap);				// May have already been done above.
		rc = CopyFile(csBitmap, csSavedBitmap,false);	// Try again.
	}
	// TAR 308999 - End

	csBitmap = csSavedBitmap;
	//TAR 195179 & 191490 end

	_TCHAR msg[] = T_ID _T("::PrintBitMap");
	rc = Enable( nDeviceClass, DEVICEID0, true );
	if ( rc == 0 )
	   {
		try
		   {
         CPrintStation* pPrinter = m_devmgr.GetPrintStation( nDeviceClass );
         rc = pPrinter->PrintBitmap( csBitmap, PTR_BM_ASIS, MapDmBitmapAlignToOpos( nAlign ) );
         m_devmgr.ThrowDmException( pPrinter );

		   }
		PROCESS_EXCEPTIONS(nDeviceClass);
      }
	trace(L6,_T("-PrintBitmap 0x%x"), rc);
	return rc;
   }


long DMProcedures::ProcessPrintLine(int nDeviceClass, const CString& csPrintLine)
{
	CString csWork = csPrintLine;
	int nPrintLineLen = csWork.GetLength();
	if (!nPrintLineLen)
	{
		// no work??? 
		return OPOS_SUCCESS;
	}
	long rc = OPOS_E_FAILURE;
	int nCurrentTokenStartPos, nCurrentTokenEndPos;
	nCurrentTokenStartPos = -1;
	nCurrentTokenEndPos = -1;
	bool bContinueParsing = false;
/*	
	nCurrentTokenStartPos = csWork.FindOneOf(szTokenList);

	if (nCurrentTokenStartPos == -1)
	{
		// no tokens found
		bContinueParsing = false;
	}

	// start parsing the line for token processing
	while (bContinueParsing && (nCurrentTokenStartPos >=0))
	{
		if (nCurrentTokenStartPos > 0)
		{
			rc = PrintLine(nDeviceClass, csWork.Left(nCurrentTokenStartPos));
			csWork = csWork.Mid(nCurrentTokenStartPos); // include start token

		}
		rc = ProcessPrintTokens(nDeviceClass, csWork);
		nCurrentTokenStartPos = csWork.FindOneOf(szTokenList);
	}
*/
	TCHAR ch;
	for(nCurrentTokenStartPos = 0; nCurrentTokenStartPos < csWork.GetLength(); nCurrentTokenStartPos++)
	{
		ch = csWork.GetAt(nCurrentTokenStartPos);
		if(ch == _T('\x04') || ch == _T('\x05'))
		{
			bContinueParsing = true;
			break;
		}
	}
	

	while (bContinueParsing)
	{
	
		rc = PrintLine(nDeviceClass, csWork.Left(nCurrentTokenStartPos));
		csWork = csWork.Mid(nCurrentTokenStartPos); // include start token
		
		rc = ProcessPrintTokens(nDeviceClass, csWork);
		//nCurrentTokenStartPos = csWork.FindOneOf(szTokenList);
		bContinueParsing = false;
		for(nCurrentTokenStartPos = 0; nCurrentTokenStartPos < csWork.GetLength(); nCurrentTokenStartPos++)
		{
			ch = csWork.GetAt(nCurrentTokenStartPos);
			if(ch == _T('\x04') || ch == _T('\x05'))
			{
				bContinueParsing = true;
				break;
			}
		}
		
	}


	// now print the last piece of normal data buffer
	if (!csWork.IsEmpty())
	{
		rc = PrintLine(nDeviceClass, csWork);
	}
	return rc;
}



//****************************************************************************************
//  TB appends bitmap or barcode token to the receipt file and SCOT App instructs
//  printer to print those objects. If any paramter is set to zero, SCOT App uses appropriate 
//  default values.
//
// Bitmap TokenFormat:
//  ____________________________________________________________
// |			  |	       |        |              |            |
// |  BMPTOKSTART |  WIDTH |  ALIGN |   BMPNAME    |  BMPTOKEND | 
// |_____________ |________|________|______________|____________|
//
// Note that the bitmap name should include the complete path to print the bitmap
//
//
// Barcode TokenFormat:
//  ____________________________________________________________________________________
// |			 |	    |        |        | print numbers    |             |            |
// |  BCTOKSTART | TYPE | HEIGHT |  ALIGN |   at the bottom  | BarcodeData |  BCTOKEND  | 
// |_____________|______|________|________|__0 or 1__________|_____________|____________|
//
//////////////////////////////////////////////////////////////////////////////////////////////
long DMProcedures::ProcessPrintTokens(int nDeviceClass, CString& CurrentPrintLine)
{
	PRNOBJECTTOKENS CurrentTokenType;
	int nTokenEndPos = -1;
	long rc = OPOS_E_FAILURE;

	CString csWork;
	int nPrintLineLength;
	
	// Try this
	if (!CurrentPrintLine.IsEmpty())
	{
			CurrentTokenType = FindPrnTokenType(CurrentPrintLine[0]);
			//	process tokens
			csWork = CurrentPrintLine.Mid(1);
			nPrintLineLength = csWork.GetLength();
	}
	else
		CurrentTokenType = PRN_NOTOKEN;
	

	switch (CurrentTokenType)
	{
		case PRN_BITMAPTOK:
		{
			nTokenEndPos = 0;
			LPTSTR szPrintLine = csWork.GetBuffer(nPrintLineLength);
			while ((nTokenEndPos < nPrintLineLength) && 
			       (szPrintLine[nTokenEndPos] != BMPTOK_END))
			{
				++nTokenEndPos;
			}
			if ((nTokenEndPos == nPrintLineLength) || (szPrintLine[nTokenEndPos] != BMPTOK_END))
			{
				nTokenEndPos = -1;
			}
			long nAlignment,nWidth;
			CString csBmpName,csPrintLineWithoutToken;
			if (nTokenEndPos != -1)
			{
				_TCHAR szField1[sizeof(long)+1] = {NULL};
				_TCHAR szField2[sizeof(long)+1] = {NULL};
				_TCHAR szField3[500] = {NULL};
				int nLongSize = sizeof(long);
				int nField3Size = (nTokenEndPos - 2*nLongSize);
				int nTokenLen = nField3Size + 2*nLongSize + 1; // no starting token prefix
				int nPrnLnWithoutTokenLen = nPrintLineLength - nTokenLen;
				LPTSTR szField4 = (LPTSTR ) new _TCHAR[nPrnLnWithoutTokenLen];
				memcpy(szField1, szPrintLine, nLongSize * sizeof(_TCHAR));
				memcpy(szField2, szPrintLine+nLongSize, nLongSize * sizeof(_TCHAR));
				memcpy(szField3, szPrintLine+2*nLongSize, nField3Size * sizeof(_TCHAR));
				memcpy(szField4, szPrintLine+nTokenLen, nPrnLnWithoutTokenLen * sizeof(_TCHAR));
				CString csTmpRestOfTheLine(szField4, nPrnLnWithoutTokenLen);
				delete [] szField4;
				nWidth     = _ttol(szField1);
				nAlignment = _ttol(szField2);

				if ((nAlignment != DM_ALIGN_LEFT) && 
					(nAlignment != DM_ALIGN_RIGHT) && 
					(nAlignment != DM_ALIGN_CENTER))
				{
					nAlignment = DM_ALIGN_CENTER;
				}
				csBmpName = szField3;
				csPrintLineWithoutToken = csTmpRestOfTheLine; // rest of the print line without token
			}
			CurrentPrintLine.ReleaseBuffer();
			// if (!csBmpName.IsEmpty()) // if bmp is null, DM uses PrinterSetBitmap value
			{
				// ****** DM control currently doesn't allow to set width of a bitmap ****** ///
				ASSERT(csBmpName.GetLength());
				rc = PrintBitmap(csBmpName, nAlignment,nDeviceClass);
				if (rc == CTL_E_FILENOTFOUND)
				{
					trace(L6, _T("Print Error: PrintBitmap: bitmap:%s doesnt exist"), csBmpName);
				}
			}
			CurrentPrintLine = csPrintLineWithoutToken;
			break;
		}
		case PRN_BARCODETOK:
		{
			long nType, nHeight, nAlignment;
			bool bPrintNumbers;
			CString csBarcodeData,csPrintLineWithoutToken;

			// find bar code token's end
			nTokenEndPos = 0;
			LPTSTR szPrintLine = csWork.GetBuffer(nPrintLineLength);
			while ((nTokenEndPos < nPrintLineLength) && 
			       (szPrintLine[nTokenEndPos] != BARCODETOK_END))
			{
				++nTokenEndPos;
			}
			if ((nTokenEndPos == nPrintLineLength) || (szPrintLine[nTokenEndPos] != BARCODETOK_END))
			{
				nTokenEndPos = -1;
			}

			// if token end is found, process bar code data
			if (nTokenEndPos != -1)
			{
				_TCHAR szField1[sizeof(long)+1] = {NULL};
				_TCHAR szField2[sizeof(long)+1] = {NULL};
				_TCHAR szField3[sizeof(long)+1] = {NULL};
				_TCHAR cField4 = DM_DA_NOT_CONFIGURED;
				int nLongSize = sizeof(long);
				int nField5Size = (nTokenEndPos - 3*nLongSize - 1);
				int nTokenLen = 3*nLongSize + 1 + nField5Size + 1; // no starting token prefix
				int nPrnLnWithoutTokenLen = nPrintLineLength - nTokenLen;
				LPTSTR szField5 = (LPTSTR ) new _TCHAR[nField5Size+1];
				LPTSTR szField6 = (LPTSTR ) new _TCHAR[nPrnLnWithoutTokenLen];
				memset(szField5,0,(nField5Size+1)*sizeof(_TCHAR));
				memset(szField6,0,nPrnLnWithoutTokenLen*sizeof(_TCHAR));

				// extract buffers for fields
				memcpy(szField1, szPrintLine, nLongSize*sizeof(_TCHAR));
				memcpy(szField2, szPrintLine+nLongSize, nLongSize*sizeof(_TCHAR));
				memcpy(szField3, szPrintLine+2*nLongSize, nLongSize*sizeof(_TCHAR));
				cField4 = *(szPrintLine+3*nLongSize);
				memcpy(szField5, szPrintLine+3*nLongSize+1, nField5Size*sizeof(_TCHAR));
				memcpy(szField6, szPrintLine+nTokenLen, nPrnLnWithoutTokenLen*sizeof(_TCHAR));
				CString csTmpBCData(szField5, nField5Size);
				CString csTmpRestOfTheLine(szField6, nPrnLnWithoutTokenLen);
				delete [] szField5;
				delete [] szField6;

				// save extracted field's value
				nType      = _ttol(szField1);
				nHeight    = _ttol(szField2);
				nAlignment = _ttol(szField3);
				if ((nAlignment != DM_ALIGN_LEFT) && 
					(nAlignment != DM_ALIGN_RIGHT) && 
					(nAlignment != DM_ALIGN_CENTER))
				{
					nAlignment = DM_ALIGN_CENTER;
				}
				bPrintNumbers = (cField4 == DM_DA_LOADED) ? 1:0 ;
				csBarcodeData = csTmpBCData;
				csPrintLineWithoutToken = csTmpRestOfTheLine; // rest of the print line without current barcode token
				// test data
				int nTmpLen;
				nTmpLen = csBarcodeData.GetLength();
				nTmpLen = csPrintLineWithoutToken.GetLength();
			}
			CurrentPrintLine.ReleaseBuffer();
			if (!csBarcodeData.IsEmpty())
			{
				int nDataLen = csBarcodeData.GetLength();
				//LPCTSTR szBarCode = csBarcodeData.GetBuffer(nDataLen);
				// rc = PrintBarcode(szBarCode,nType, nHeight, nAlignment,bPrintNumbers, nDeviceClass);
				rc = PrintBarcode(csBarcodeData,nType, nHeight, nAlignment,bPrintNumbers, nDeviceClass);
				//csBarcodeData.ReleaseBuffer();
				if (rc == CTL_E_OUTOFMEMORY)
				{
					trace(L6, _T("Print Error: PrintBarcode: out of memory error from DM"));
				}
			}
			CurrentPrintLine = csPrintLineWithoutToken;
			break;
		}
		case PRN_NOTOKEN:
		default:
		{
		//	ASSERT(0);
			break;
		}
	}
	// test line to see the remaining length
	int ntmp = CurrentPrintLine.GetLength();
	return rc;
}

// this function has to be tested before using it, it is meant for future usage only -AKR
bool DMProcedures::GetDeviceStatusInfo(const int nDeviceClass, bool& bIsEnabled, CDeviceStatistics* pDeviceInfo)
{
	bool bRetValue = false;

	if ((nDeviceClass >= DMCLASS_START) && (nDeviceClass < m_devmgr.GetMaxClasses()))
	{
	   CDeviceStatistics LocalInfo = m_devmgr.GetDeviceStatistics( nDeviceClass );
		if (pDeviceInfo)
		{
			*pDeviceInfo = LocalInfo;
			//pDeviceInfo.cStatus = LocalInfo.cStatus;
			//pDeviceInfo.csDescription = LocalInfo.csDescription;
			//pDeviceInfo.fIsEnabled = LocalInfo.fIsEnabled;
			//LocalInfo.fDoEnable   = false;
		}
		bIsEnabled = LocalInfo.fDoEnable;
		bRetValue = true;
	}
	return bRetValue;
}


PRNOBJECTTOKENS FindPrnTokenType(_TCHAR chTokenPrefix)
{
	PRNOBJECTTOKENS nTokenType = PRN_NOTOKEN;
	_TCHAR cTmp = (_TCHAR) BARCODETOK_START;
	cTmp = (_TCHAR) BMPTOK_START;
	if (chTokenPrefix == (_TCHAR) BMPTOK_START)
	{
		nTokenType = PRN_BITMAPTOK;
	}
	else if (chTokenPrefix == (_TCHAR) BARCODETOK_START)
	{
		nTokenType = PRN_BARCODETOK;
	}
	ASSERT(nTokenType != PRN_NOTOKEN);
	return nTokenType;
}


///////////////////////////////////////////////////////////////////////
// Enable or disable class device
long DMProcedures::Enable(
   int   nDeviceClass,
   long  nDeviceId,
   bool  fEnable )
   {
   _TCHAR msg[] = T_ID _T("::Enable");
   long lResult = 0;
   // if the device is already in the required state, just return
   CDeviceStatistics dvs = m_devmgr.GetDeviceStatistics( nDeviceClass, nDeviceId );
   if (   DMCLASS_CASHCHANGER == nDeviceClass) 
   {
		if (dvs.csDescription.Find(_T("Short")) != -1) 
		  {
			  m_csCoinDispenserStage= _T("ShortTermPlan");
		  }
		  else if (dvs.csDescription.Find(_T("Long")) != -1)
		  {
			  m_csCoinDispenserStage= _T("LongTermPlan");
		  }
   }
		  
   if ( dvs.fIsEnabled == fEnable ) 
      lResult = 0;   // zero is device is correctly enabled/disabled
   else if ( dvs.cStatus != DM_DA_LOADED )  // if the status is other than 1 the device
   {                         // is in error or unavailable
      if ( dvs.cStatus != DM_DA_NOT_CONFIGURED )
         {     // Log an error event only if the device is in error
         ScotError(INFO,SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_DMINTERFACE_DEVICE,_T("%s|%s|%d|%c"),
            (fEnable?_T("enable"):_T("disable")), dvs.csDescription, nDeviceClass,
            dvs.cStatus );
         }
      lResult = fEnable ? -1 : 0;  // return good if device is disabled, else bad
      // TAR 283924 start - determine low sensor even if device has error
      if (   DMCLASS_CASHCHANGER == nDeviceClass) 
	  {
          if (dvs.csDescription.Find(_T("CoinMediaLowSensor")) != -1) 
		   {
		   m_bCoinDispenserWithLowSensor = true;
		   }  
	  }
   }
      // TAR 283924 end
   else
      {
      trace(L4, _T("+%s %d %d "),msg,nDeviceClass,fEnable);
      if (dvs.fDoEnable )
         {
         try
            {
            rc = 0;
            m_devmgr.Enable(nDeviceId, nDeviceClass, fEnable);
            }
         PROCESS_EXCEPTIONS(nDeviceClass);
         lResult = rc;
         }
      trace(L4, _T("-%s %d %d "),msg, nDeviceClass, fEnable);
      }
	if ( m_bDMOpened != FALSE
      && lResult != 0 )
	{
	  if (dvs.cStatus == DM_DA_NOT_CONFIGURED ) //TAR351593
	  {
		  trace(L6, _T("Device is not configured.")); //TAR351239 no need to log it in event log
	  }
	  else
	  {
          // +TAR 410407
          if(dvs.cStatus == DM_DA_CONFIGURED_BUT_NOT_LOADED)
          {
              trace(L6, _T("Device is configured but not loaded."));
          }
          else
          {
              ScotError( INFO, SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DMINTERFACE2, _T("%s|%d"), msg,__LINE__);              \
          }
          // -TAR 410407
	  }
	}
   return lResult;
   }


/**
 * Method:      EnableCouponProcessIng
 * Description:  
 * Parameters:  bFlag - If true, forward coupon sensor events to the base                    
 *                      application.  
 *                      If false, ignore coupon sensor events.
 * Returns:
 * Notes:       Added for SR631.1 - FL Receive coupon events only when coupons   
 *              expected
 */

void DMProcedures::EnableCouponProcessing (bool bIsNeeded)
{
	trace(L6,_T("+EnableCouponProcessing - bIsNeeded: %d"), bIsNeeded);
	 _TCHAR msg[] = T_ID _T("::EnableCouponProcessing ");
	if(!IsDeviceConfigured( DMCLASS_MOTION_SENSOR, DEVICEID1 ))
	{
		trace(L4, _T("Coupon Sensor not configured!"));
		return;
	}
	try
	{	
	    CMotionSensorWrapper* motion = m_devmgr.GetMotionSensor(DEVICEID1);	
		if(!motion)
		{
			trace(L4, _T("Error on the coupon sensor."));
			return;
		}
		else
		{
			//access the EnableCouponProcessing() in the class wrapper
			motion->EnableCouponProcessing(bIsNeeded);
		}
	}
	PROCESS_EXCEPTIONS(DMCLASS_MOTION_SENSOR);	
	trace(L6,_T("-EnableCouponProcessing "));
}


//////////////////////////////////////////
// INIT and UNINIT Methods
// 'Create' this window (dmp) then the (dmi) window object,
//  load OLE control and update device manager status...
long DMProcedures::Initialize(CWnd * pWnd)
{
	long rc = -3;
    _TCHAR msg[] = T_ID _T("::Initialize");

	try
	{
	  trace(L0, _T("+")_T(__TIMESTAMP__));
	  trace(L4,_T("+Initialize"));
	  m_nStartDeviceErrorResultCodeExtended = 0; //Error Handling Cash
	  m_nStartDeviceErrorDeviceClass = -1;
	  m_lDeviceClassThatWasInError.clear();		 //TAR410767
		
	  if( m_hWnd == NULL )
		 Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), 
			pWnd == NULL ? AfxGetMainWnd() : pWnd, IDW_DMPWINDOW );

	  bScannerIsEnabled = false;		// RFC 219283 DE/RF 7401 Enables Motion Sensor on 7883 scanner
  
	  ReadBillDispenserTypes();    //Tar 227532

	  rc = DMIInitialize();
	  if(rc == 0)
		LoadPrintObjects(); // preload any bitmaps listed in SCOTOPTS.000 under PrintObjects section

	  // Initialize the remote aspect of the price sound object
	  try
	  {
		SCOTPriceSoundInitializeRemote(false);
	  }
	  catch(...)
	  {
		  trace(L6,_T("SCOTPriceSoundInitializeRemote threw an exception"));
	  }

	  m_csReceiptPrinterModel = ReadReceiptPrinterModel();     //Device Error
	  ReadDevicesModel();
		
	  trace(L4,_T("-Initialize %d"),rc);
	}
    PROCESS_EXCEPTIONS(DM_ALL_DEVICECLASSES);
	
  return rc;
}

/////////////////////////////////
long DMProcedures::UnInitialize()
{
    trace(L4,_T("+DMProcedures::UnInitialize()"));
    
    trace(L4,_T("calling SCOTPriceSoundUninitializeRemote()..."));
    SCOTPriceSoundUninitializeRemote();
    
    trace(L4,_T("calling KillTimer(WM_DISABLE_DEPOSIT)..."));
    // +TAR 343389 - Migrated fix for 339541.
    KillTimer(WM_DISABLE_DEPOSIT); // TAR 339541
    // -TAR 343389 - Migrated fix for 339541.
    
    DMIUnInitialize();
    
    if ( m_hWnd != NULL )
    {
        trace(L4,_T("calling DestroyWindow()..."));
        DestroyWindow();
    }
    
    trace(L4,_T("-DMProcedures::UnInitialize()"));
    
    return 0;
}


CString DMProcedures::GetDescription(
   const int nDeviceClass,
   long nDeviceId /* = 0 */ )
   {
   _TCHAR msg[] = T_ID _T("::GetDescription");
   trace(L7, _T("GetDescription"));
   CString desc = m_devmgr.GetDeviceDescription(nDeviceId,nDeviceClass);

   // TAR fix: 241019: sometimes missing device description.
   // CMal062403 - noted that Device Description doesn ot always work, at least try 
   //              to grab the name.  Might be that only DMX works well with the above.
   if (desc.IsEmpty())
   {
	   desc = m_devmgr.GetDeviceName(nDeviceClass, nDeviceId);
   }
   // CMal062403 - end 241019

   // Default description is both coin and bill.
   if(nDeviceClass == DMCLASS_CASHCHANGER)
      {
      long coin = 0;
      long bill = 0;
      CashChangerGetStatusValues(&coin, &bill);
      trace(L7, _T("****Coin status from DM is %ld"), coin);
      trace(L7, _T("****Bill status from DM is %ld"), bill);

      // If only one of them is in error, then which one?
      BOOL coinError = true;
      BOOL billError = true;

      if( coin == DM_CC_STATUS_NEAREMPTY ||

         coin == DM_CC_STATUS_EMPTYOK   ||
         coin == DM_CC_STATUS_PURGEFULL ||
         coin == DM_CC_STATUS_PURGENEARFULL)
         coinError = false; 

      if (bill == DM_CC_STATUS_NEAREMPTY ||
         bill == DM_CC_STATUS_EMPTYOK   ||
         bill == DM_CC_STATUS_PURGEFULL ||
         bill == DM_CC_STATUS_PURGENEARFULL)
         billError = false; 
      trace(L7, _T("Coin error is %d"), coinError);
      trace(L7, _T("Bill error is %d"), billError);

      if ((coinError && !billError) || (billError && !coinError))
         {
         CString device;
         int i = desc.Find(_T("and"));
         if (coinError)
            device = desc.Left(i);
         if (billError)
            device = desc.Right(desc.GetLength()-(i+3));
         device.TrimLeft();
         desc = device;
          trace(L7, _T("*****One cash changer device in error: %s"), desc);
         }
      else if ((coinError && billError)) // CMal062403 TAR fix: 241035 giving msg when nothing wrong
         trace(L7, _T("*****Both cash changer devices in error: %s"), desc);
      } 
   return desc;
   }

long DMProcedures::DMIInitialize(void)
   {
   LPTSTR msg=T_ID _T("::Initialize");

   trace(L4,_T("%s"),msg);

   // +TAR 373658/374133: Migrated changes from E5
   if (m_bDMOpened)
   {
       trace(L4, _T("If DevMgr is open, close it")) ;
       m_bDMOpened = false ;
       m_devmgr.Close() ;
   }
   // -TAR 373658/374133: Migrated changes from E5

   // In-Store Message Support
   m_theTriColorLight.Initialize();

   // Initialize Device State variables to Unknown
   m_nCoinAcceptorLightState = DM_MISC_STATE_UNKNOWN;
   m_nDepositoryLightState = DM_MISC_STATE_UNKNOWN;

   if (m_devmgr.m_hWnd == NULL)

      {
      if(!m_devmgr.Create(NULL, NULL, CRect(0,0,0,0), this, IDC_SCOTDEVICEMANAGER1))
         return -1;
      trace(L4,_T("devmgr Created"));
      }
  // now check each device class for number of available devices
  // we need, and expect one device in each class
  // device IDs in each class are numbered 0,1,2,3
  // the DeviceAvailability method returns a CString whose length is the
  // number of device IDs available
  // each char in the CString is a status of that device
  // a 1 means OK, a 2 means in error and a check health is required
  // if any devices have a status of 2 then
  // ... issue a check health and
  // ... set a flag to indicate a device was in error and
  // ... continue with rest of devices
  // if the flag is set indicating one or more errors then
  // ... close DM
  // ... open DM
  // ... repeat DeviceAvailability test for all devices and
  // ... ... if a device is in error still then set a flag
  // ... if the flag is set popup a PS screen showning device(s) in error
  // ... let operator correct the problem
  // ... wait for continue button
  // ... repeat this whole process
	
#define DM_OPEN_RETRIES 1
   long nInError = -1;
   m_bIgnoreDMEvents = false;		// TAR 309097 - reset.
   try
   {
      for ( int nIndex = 0; nIndex < DM_OPEN_RETRIES + 1 /*TAR 373658/374133: && m_bDMOpened == false*/; nIndex++ )
      {
         nInError = -1;
         m_devmgr.Open();
         m_bDMOpened = true;
         trace(L4,_T("devmgr Opened"));
         DWORD dwOptions = LEVEL2_HC_ON_CASHCHANGER;
         nInError = DMICheckDevices( dwOptions );
         if ( nInError != 0 )
         {
            if ( nIndex < DM_OPEN_RETRIES && m_bIgnoreDMEvents == false) // if not last attempt
            {
               ScotError(RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_DMINTERFACE_DEVICE_ERROR2,_T("%d"),nInError);
               
               // TAR 288207 start - reset flags from previous time thru loop
               m_nStartDeviceErrorResultCodeExtended = 0;
               m_nStartDeviceErrorDeviceClass = -1;
               // TAR 288207 end
            }

            // +TAR 373658/374133
            if (nIndex < DM_OPEN_RETRIES)
            {
                trace(L6,_T("shut down devmgr before re-check"));
                m_bDMOpened = false;
                m_devmgr.Close();
            }
         }
         else
         {
             trace(L4, _T("DMICheckDevices returns no error")) ;
             break ;
         }
		 // TAR 309097 - Only go through once.
		 // Note that m_bIgnoreDMEvents is set in DMICheckDevices().
		 if(m_bIgnoreDMEvents == true)		
			 break;
      }
   }
   PROCESS_EXCEPTIONS(DM_ALL_DEVICECLASSES);
   return m_bDMOpened == false || nInError != 0 ? -3 : 0;
}

//SR742+
long DMProcedures::ReloadMSR()
{
	trace(L6,_T("+ReloadMSR"));
	LPTSTR msg=T_ID _T("::ReloadMSR");
	
	long lReturn = OPOS_SUCCESS;
	trace(L6, _T("+DMProcedures::ReloadMSR"));
	
	//Tar 418070
	try
	{
		m_devmgr.ReloadMSR();
		lReturn = CheckHealth( DMCLASS_MSR);
	}
	PROCESS_EXCEPTIONS(DMCLASS_MSR);
	trace(L6,_T("-ReloadMSR"));
	return lReturn;

}
//SR742-

long DMProcedures::DMIUnInitialize(void)
{
    LPTSTR msg=T_ID _T("::UnInitialize");
    
    trace(L4,_T("+DMIUnInitialize::UnInitialize()"));
    
    // clear out device structure
    for(int x = DMCLASS_RECEIPT; x < m_devmgr.GetMaxClasses(); x++)
    {
        try // GetDeviceAvailability can throw.  We don't want it uncaught
        {
            trace(L6,_T("+DMIUnit Device:%d"), x ); 
            CString csWork = m_devmgr.GetDeviceAvailability( x );
            for ( int nDeviceId = 0; nDeviceId < csWork.GetLength(); nDeviceId++ )
            {
                if ( csWork[ nDeviceId ] == DM_DA_LOADED )
                {
                    trace(L6,_T("Device:%d Id:%d loaded; disabling it..."), x, nDeviceId);
                    Enable(x, nDeviceId, false);
                }
            }
            trace(L6,_T("-DMIUnit Device:%d"), x ); 
        }
        PROCESS_EXCEPTIONS(DM_ALL_DEVICECLASSES);
    }
    rc = 0;
    trace(L4,_T("Disabled all devices, attempting to close device manager..."));
    try
    {
        if (m_bDMOpened)
        {
            trace(L4,_T("Device manager open.  Closing..."));
            m_devmgr.Close();							  // close the device manager
            m_bDMOpened = false;
        }
        if ( m_devmgr.m_hWnd != NULL )
        {
            trace(L4,_T("Destroying device manager window..."));
            m_devmgr.DestroyWindow();
        }
    }
    PROCESS_EXCEPTIONS(DM_ALL_DEVICECLASSES);
    trace(L4,_T("-DMIUnInitialize::UnInitialize()"));
    return rc;
}



void DMProcedures::OnStatusEventScotdevicemanager1(long StatusType, long DeviceClass, long DeviceID)
{
  // Post a DM StatusEvent message to the message object.
  // SCOTAPP doesn't make use of the keylock (supervisor switch),
  // so ignore those messages.  (A switch application, if running
  // along SCOTAPP, will make use of keylock.)
  if (DeviceClass != DMCLASS_KEYLOCK)
    mo.PostDM(DM_STATUS, DeviceClass, DeviceID, StatusType, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DMProcedures::OnInputEventScotdevicemanager1(long InputType, long DeviceClass, long DeviceID, const VARIANT FAR& DeviceData)
{
  if(InputType==DM_ERROR)// errorcode
  {
    mo.PostDM(DM_ERROR, DeviceClass, DeviceID, DeviceData.lVal, NULL);
  }
	
  if(InputType==DM_DATA)// Data value
  {
	
    if(DeviceClass==DMCLASS_ENCRYPTOR)
    {
      mo.PostDM(DM_DATA, DeviceClass, DeviceID, sizeof(VT_I4), (void*)&DeviceData.lVal);

      if(DeviceData.lVal==DM_ENCRYPT_COMPLETE)
      {
        CString ed = m_devmgr.GetEncryptorEncryptedPIN(DEVICEID0);
        mo.PostDM(DM_DATA, DeviceClass, DeviceID, 8, (void*)((LPCTSTR)ed));
      }
    }
		
    if(DeviceClass==DMCLASS_CASHACCEPTOR)
    {
      CString Sd(_T(" "), SysStringByteLen(DeviceData.bstrVal)+1);
      for(ULONG x = 0; x < SysStringLen(DeviceData.bstrVal); x++)
        Sd.SetAt(x, (_TCHAR)(DeviceData.bstrVal)[x]);
      Sd.SetAt(x, 0);
      mo.PostDM(DM_DATA, DeviceClass, DeviceID, Sd.GetLength(), (void*)LPCTSTR(Sd));
    }
// Begin Coin Acceptor Modifications
    if(DeviceClass==DMCLASS_COINACCEPTOR)
    {
      CString Sd(_T(" "), SysStringByteLen(DeviceData.bstrVal)+1);
      for(ULONG x = 0; x < SysStringLen(DeviceData.bstrVal); x++)
        Sd.SetAt(x, (_TCHAR)(DeviceData.bstrVal)[x]);
      Sd.SetAt(x, 0);
      mo.PostDM(DM_DATA, DeviceClass, DeviceID, Sd.GetLength(), (void*)LPCTSTR(Sd));
    }
// End Coin Acceptor Modifications
    /// handle unknowns later
  }
}



long DMProcedures::PrintLine(
   int            nDeviceClass, 
   const CString& csPrintLine)
   {
   LPTSTR msg=T_ID _T("::PrintLine");
   try 
      {
      CPrintStation* pPrinter = m_devmgr.GetPrintStation( nDeviceClass );
	  //Tar219151
	  //don't trace slip data at level 6.
		if ((nDeviceClass == DMCLASS_SLIP) && (co.csUserExitString5 == _T("OmitCustSensitiveDataInTraces")))
	  {
        to.HexTrace(L7,_T("DMProcedures::PrintLine"),csPrintLine, csPrintLine.GetLength() );
	  }
	  else
	  {
		  // default trace
        to.HexTrace(L6,_T("DMProcedures::PrintLine"),csPrintLine, csPrintLine.GetLength() );
	  }
	  //-Tar219151

#ifdef UNICODE
		rc = pPrinter->PrintNormal( csPrintLine );	
	
#else
		//TAR 266885 - Support compressed printing
	  	int firstpos = -1;
		CString csTemp("\x1b\x16\x01");    //Compressed pitch for a print line
		firstpos = csPrintLine.Find(csTemp);     


		if (firstpos == -1)
		{
	   	   rc = pPrinter->PrintNormal( csPrintLine );
		} 
		else
		{
			//+dp185016 tar 275927
			int csPrintLineLength = csPrintLine.GetLength();

			int lineCount = pPrinter->GetLineChars(); // get RecLineChars property
			
			int compressLineCount;
			if (lineCount == 44) compressLineCount = 56;
			else if (lineCount == 45) compressLineCount = 59;
			else if (lineCount == 64) compressLineCount = 83;
			
			int printPos = -1;
			
			while (firstpos >= 0)
			{
				if ((firstpos > 0) && (printPos < (firstpos - 1 ))) 
				{
					rc = pPrinter->PrintNormal(csPrintLine.Mid((printPos + 1), (firstpos - printPos -1) ));
				}
				printPos = firstpos;
				
				pPrinter->SetLineChars(compressLineCount);
				
				int aPos = csPrintLine.Find('\x0a',printPos);
				if (aPos == -1) 
				{
					aPos = csPrintLineLength;
				}
				
				rc = pPrinter->PrintNormal(csPrintLine.Mid(printPos, (aPos - printPos + 1)));
				printPos = aPos;
				
				pPrinter->SetLineChars(lineCount);  // set property back
				
				firstpos = csPrintLine.Find(csTemp,printPos); // search for Compressed pitch   
				
			}
			
			if (printPos < csPrintLineLength) 
			{
				rc = pPrinter->PrintNormal(csPrintLine.Mid((printPos+1),(csPrintLineLength - printPos +1 )));
			}	
	//-dp185016
    }


#endif
      m_devmgr.ThrowDmException( pPrinter );
      }
   PROCESS_EXCEPTIONS( nDeviceClass );
   return rc;
   }

static void DoCodeThatShouldNotBeHere( 
   long              nDeviceClass,
   CDeviceStatistics* pDvs )
   {
   switch( nDeviceClass )
      {
      case DMCLASS_SCALE:
			//co.fStateSubstCkPriceEmbeddedWeights = false;	
			co.fStateForceWeighingOFPriceEmbedded = false;
			co.fStateDisplayWeightsAndMeasures = false;
			ps.ShowTrainingModeOnScreen(TrainingModeObj.IsTMOn());
			ScotError(INFO,SM_CAT_CONFIGURATION, SCOTAPP_FORCED_TO_OVERRIDE_OPTION, _T("%s"), _T("ForceWeighingOFPriceEmbedded")); // event log
			trace(L2, _T("Scot forced to override ForceWeighingOFPriceEmbedded option = ."));
         break;
      case DMCLASS_CASHACCEPTOR:
			trace(L2, _T("Cash Acceptor is not available."));
         break;
      case DMCLASS_COINACCEPTOR:
			trace(L2, _T("Coin Acceptor is not available."));
         break;
      case DMCLASS_MOTION_SENSOR:
			trace(L2, _T("Sensormatic Motion Sensor is not available."));
         dm.fStateMotionSensor = false;
         break;
      case DMCLASS_EASDEACTIVATOR:
			trace(L2, _T("Sensormatic Deactivator is not available."));
         break;
#if 0
      case DMCLASS_MSR:
         {
         ASSERT( FALSE );
         CString MSRDesc = pDvs->csDescription;
	      MSRDesc.MakeLower();
	      m_fMSRIsMotorized = (BOOL)(MSRDesc.Find(_T("motor")) != -1);
         }
#endif
      default:
			trace(L2, _T("DeviceClass %d is not available."), nDeviceClass);
         break;
      }
   return;
   }

long DMProcedures::DMICheckDevices( DWORD dwOptions /* =0 */)
   {
   int nInError = 0;
   // if there were any errors, close DM, open DM and re-check
   m_csDevicesInError.Empty();
   for ( int nIndex = 0; nIndex < m_devmgr.GetMaxClasses(); nIndex++)
      { // init structure
	   // ------------------------------------------------------------------------
	   // TAR fix: 241038 during Device Init, not clear which load ok.  CMal062403
	   //                 moved some statements, but mainly added traces to 
	   //                 allow someone to follow this logic in a trace.
	   // ------------------------------------------------------------------------

	   // -------------------------------------------------------------------
	   // This returns a 'digit' for each configured device.  Therefore only
	   // a few deviceClasses return multiple digits.  e.g. Device 3 Printers
	   // Note:  0=NotConfiguredinReg 1=Loaded(OK) 2=ConfiguredButNotLoaded
	   //        and blank means NotConfigured too
	   // Known: DMX works the above mentioned way
	   // Unknown: if DevMgr works that way
	   // -------------------------------------------------------------------
      CString csWork = m_devmgr.GetDeviceAvailability( nIndex );
	  CString csDevName;	// TAR 309097

	  // CMal060903 Note: Need to check for a valid deviceid from GetDeviceAvailability.  
	  //                  And if this device is configured, then go thru the 'for'
	  //                  loop for all the deviceids.  Note: csWork is "", 1, 2, ..
	  
	  int iNumPossibleDev = csWork.GetLength();

	  if (iNumPossibleDev > 0) // CMal062403: added new 'if' to ensure correct working
	  {
 		 //long lDeviceCnt = m_devmgr.GetAvailableDevices(nIndex); // Count em this way CMal060903
		 trace(L6,_T("Check1A for deviceClass %d with %d configured devices"), nIndex, iNumPossibleDev); 

		 for ( int nDeviceId = 0; nDeviceId < iNumPossibleDev/*csWork.GetLength() CMal062403*/ ; nDeviceId++ )
         {
          //DeviceError  Tar 226224
          if((nIndex == DMCLASS_CASHCHANGER) && ( dwOptions & LEVEL2_HC_ON_CASHCHANGER ) && !m_nStartDeviceErrorResultCodeExtended)
          {  
	           CDeviceStatistics Stats = m_devmgr.GetDeviceStatistics( nIndex, nDeviceId );
        	   if( Stats.cStatus == DM_DA_NOT_CONFIGURED )
			   {
			   }
			   else
			   {
				   CheckHealth( nIndex, nDeviceId ); // level 2 check health
				   m_nStartDeviceErrorResultCodeExtended = GetResultCodeExtended(nIndex, nDeviceId );
				   if(m_nStartDeviceErrorResultCodeExtended >0)  //There is error
				   {
					   m_csDevicesInError += _T("\n") + GetDescription( nIndex, nDeviceId ); //Tar 230829
    				   nInError++;
					   m_nStartDeviceErrorDeviceClass = nIndex;
					   break;
				   }
			   }		       			                    	  
		  }
  
		 		 
         CDeviceStatistics Stats = m_devmgr.GetDeviceStatistics( nIndex, nDeviceId );

		 // begin TAR 309097
		 // See if this is a Glory device and it is configured in the registry.
		 csDevName = Stats.csDescription;
		 csDevName.MakeUpper();
		 if(csDevName.Find(_T("GLORY"))==0 && Stats.cStatus!=DM_DA_NOT_CONFIGURED)
		 {
			m_bIgnoreDMEvents = true;
		 }
		 // end TAR 309097
		 if( Stats.cStatus == DM_DA_NOT_CONFIGURED )
		 {
			 // TAR fix: 241038 CMal062403
			trace(L6,_T("Check1B device %s availability: DeviceClass %d  DeviceID %d - NOT CONFIGURED in Registry"), Stats.csDescription, nIndex, nDeviceId  );
            DoCodeThatShouldNotBeHere( nIndex, &Stats );
		 }
         else 
            {
            CString sDesc = GetDescription( nIndex, nDeviceId );
			if ( Stats.cStatus == DM_DA_CONFIGURED_BUT_NOT_LOADED )
		       {
				  m_nStartDeviceErrorDeviceClass = nIndex;  //tar 277605
				  // TAR fix: 241038 CMal062403
				  trace(L6,_T("Check1B device %s availability: DeviceClass %d  DeviceID %d - configured in Registry, But NOT LOADED"), Stats.csDescription, nIndex, nDeviceId  );

				  if (-1 == sDesc.Find(_T("This is supposed to come from")))
				  {
					m_csDevicesInError += _T("\n") + sDesc;
				  }
				  else
				  {
                      CString sDevice, sDeviceID;
					  sDevice = g_sClassNames[nIndex];
					  sDeviceID.Format(_T("(DeviceId %d)"),nDeviceId);
                      m_csDevicesInError += _T("\n") + sDesc + _T(" ") + sDevice + sDeviceID;	
				  }
			      ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_DMINTERFACE_DEVICE_LOAD,_T("%s|%d|%d"),
				      sDesc, nIndex, nDeviceId );
				  ////////////////////////////////////////////
				  /////////   ALLOW REMOTE SESSION WITH OPTIONAL DEVICES IN ERROR STATE ALSO TAR:220593
				  if (( (nIndex == DMCLASS_RECEIPT) ||
						(nIndex == DMCLASS_SLIP) ||
						(nIndex == DMCLASS_PRINTER) ||
						(nIndex == DMCLASS_MICR) ||
						(nIndex == DMCLASS_CASHDRAWER)
					  ) && (co.csUserExitString5 == _T("OmitCustSensitiveDataInTraces")) // TAR:220593
				      && (IsDeviceRemote( nIndex, nDeviceId )) ) // TAR 223498/224782/225005
				  {
					// these devices are not stopping the transaction to complete
					// as of today, slip and receipt output is directed to fastlane
					// in case the remote devices are not available
					// There will be a separate RFC to address optional devices,
					// communicating the error state to user
					// until that RFC is implemented, do not consider optional device errors
				  }
				  else
				  {
					nInError++;
				  }
				   CheckHealth( nIndex, nDeviceId ); // level 2 check health
				  //Error hangling 
				  if(nIndex == DMCLASS_CASHCHANGER  || nIndex == DMCLASS_SCANNER || 
					 nIndex == DMCLASS_COINACCEPTOR || nIndex == DMCLASS_CASHRECYCLER)
				  {
				    m_nStartDeviceErrorResultCodeExtended = GetResultCodeExtended(nIndex, nDeviceId );
					m_nStartDeviceErrorDeviceClass = nIndex;
				  }
               }
			   // CMal062403 - show more tracing so we know what happened.  TAR fix: 241038
			   else if (Stats.cStatus == DM_DA_LOADED)
			   {	    
					trace(L6,_T("Check1B device %s availability: DeviceClass %d  DeviceID %d - LOADED"), Stats.csDescription,nIndex, nDeviceId  );
			   }
			   else
   			   {
					trace(L6,_T("Check1B device %s availability: DeviceClass %d  DeviceID %d - Unknown Status %c"), Stats.csDescription, nIndex, nDeviceId, Stats.cStatus);
			   }
			   // CMal062403 - end: TAR fix: 241038
	         }//3
         
			//SR742+ Check if there are devices in error at start-up and check if that devices are 
		    // covered on this feature. If so, the we will by-pass the error and continue  loading as  
		    // if the device error has been corrected.
			if ( nInError && co.IsDegradedModeOn() )
			{	
				//if device error on startup, check if it is degraded mode devices.
				if ( m_nStartDeviceErrorDeviceClass == DMCLASS_CASHACCEPTOR ||  
					 m_nStartDeviceErrorDeviceClass == DMCLASS_COINACCEPTOR || 
					 m_nStartDeviceErrorDeviceClass == DMCLASS_CASHRECYCLER ||
	 			     m_nStartDeviceErrorDeviceClass == DMCLASS_CASHCHANGER  || 
					 m_nStartDeviceErrorDeviceClass == DMCLASS_MSR )
				{
					m_lDeviceClassThatWasInError.insert(m_nStartDeviceErrorDeviceClass);

					if (  dm.GetDegradedModeType() != DEGRADEDMODE_OFF ) 
					{
						trace(L6, _T("Get into degraded mode. Bypass device error %d."), m_nStartDeviceErrorDeviceClass);
						m_nStartDeviceErrorResultCodeExtended = 0;
						nInError--;

						if ( dm.GetDegradedModeType() == DEGRADEDMODE_CARD_ONLY)
						{
							co.fTenderCashAllowed = false;
						}
					}
				}
			}//SR742-
         }
	  }
	  // CMal060903 - say something when nothing there too TAR fix: 241038
	  else
	  {
		  if(nIndex == DMCLASS_RECEIPT)
		  {
				m_nStartDeviceErrorResultCodeExtended = 259;   //Display Device Not Available error
				m_nStartDeviceErrorDeviceClass = nIndex;
		  }
		  trace(L6,_T("Check1A for deviceClass %d - Not a configured device"), nIndex ); // CMal060903
	  }
	  // CMal060903 - end else TAR fix: 241038
// Application expects that csWork is not empty because DM has a set number
// of devices per class, but we do not. Empty is fine and indicates 
// that no devices were configured. (like "00" from DM)
//      else 
//         ScotError( RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_DMINTERFACE_DEVICE_AVAI,"%d", nIndex );
      }
   if (nInError)
      ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_DMINTERFACE_DEVICE_ERROR,_T("%d"),nInError);
   else
      {
      m_devmgr.ClearInput(DM_ALL_DEVICECLASSES); // clear all buffers on all devices
      m_devmgr.SetInputEventEnabled(true);	// turn on input event logging
        // Now that we have the corresponding DM fix to allow
        // data events to come in with all possible card data,
        // change the core app. to read all possible tracks.
        // TAR# 132962 - ncr_barn - March 14, 2000
      fStateMotionSensor = m_devmgr.GetDeviceStatistics( 
         DMCLASS_MOTION_SENSOR, DEVICEID0 ).cStatus == DM_DA_LOADED;
      }
   return nInError;
   }


long DMProcedures::MICREnable( 
   void )
   {
   trace(L6, _T("+MICREnable"));
   _TCHAR msg[] = T_ID _T("::MICREnable");
   rc = Enable(DMCLASS_MICR, DEVICEID0, true);
   if ( rc == 0 )
      {
      try
         {
         CMICRWrapper* pMicr = m_devmgr.GetMICR( DEVICEID0 );
         ASSERT( pMicr != NULL );
		 rc = pMicr->ClearInput(  ); 
         rc = pMicr->BeginInsertion( 0 );
         m_devmgr.ThrowDmException( DMCLASS_MICR, rc, pMicr->GetResultCodeExtended() );
         }
      PROCESS_EXCEPTIONS(DMCLASS_MICR);	
      }
   trace(L6, _T("-MICREnable 0x%x"), rc);
   return rc;
   }


long DMProcedures::MICRDisable( 
   bool bEject )
   {
   trace(L6, _T("+MICRDisable"));
   _TCHAR msg[] = T_ID _T("::MICRDisable");
   try
      {
      CMICRWrapper* pMicr = m_devmgr.GetMICR( DEVICEID0 );
      ASSERT( pMicr != NULL );
      if( bEject )
      {
          pMicr->BeginRemoval(0);
          pMicr->EndRemoval();
      }
	  if (ps.RemoteMode() )  //RFC 356020
	  {
		  Enable(DMCLASS_MICR, DEVICEID0, false);
		  pMicr->SetDataEventEnabled( FALSE );
	  }
      rc = pMicr->ClearInput(  );
      m_devmgr.ThrowDmException( pMicr );
      }
   PROCESS_EXCEPTIONS(DMCLASS_MICR);	
   trace(L6, _T("-MICRDisable 0x%x"), rc);
   return rc;
   }


long DMProcedures::MICRGetCheckData( 
   CString&    sRawData, 
   CString&    sAcctNumber,
   CString&    sAmount, 
   CString&    sBankNumber, 
   CString&    sEPC,
   CString&    sSerialNumber, 
   CString&    sTransitNumber, 
   long*       pnCheckType, 
   long*       pnCountryCode )
   {
   trace(L6, _T("+MICRGetCheckData") );
   _TCHAR msg[] = T_ID _T("::MICRGetCheckData");
   try
      {
      rc = 0;
      CMICRWrapper* pMicr = m_devmgr.GetMICR( DEVICEID0 );
      ASSERT( pMicr != NULL );
      sRawData          = pMicr->GetRawData();
      sAcctNumber       = pMicr->GetAccountNumber();
      sAmount           = pMicr->GetAmount(); 
      sBankNumber       = pMicr->GetBankNumber(); 
      sEPC              = pMicr->GetEpc(); 
      sSerialNumber     = pMicr->GetSerialNumber (); 
      sTransitNumber    = pMicr->GetTransitNumber(); 
      *pnCheckType      = pMicr->GetCheckType(); 
      *pnCountryCode    = pMicr->GetCountryCode(); 
      }
   PROCESS_EXCEPTIONS(DMCLASS_MICR);	
   trace(L6, _T("-MICRGetCheckData 0x%x"), rc );
   return rc;
   }

//+4.2 card dispenser
//******************************************************
//	Method:	 CardDispenserDisable
//  Purpose: to allow the application to issue the enable command to the Card Dispenser control
//  Parameters:  
//  Return Values: long - OPOS_SUCCESS if successfull
//******************************************************
long DMProcedures::CardDispenserEnable()
{
   _TCHAR msg[] = T_ID _T("::CardDispenserEnable");

   trace(L6, _T("+CardDispenserEnable") );
   try
   {
      rc = 0;
      CCardDispenser* pCardDispenser = m_devmgr.GetCardDispenser( DEVICEID0 );
      ASSERT( pCardDispenser != NULL );

	  pCardDispenser->SetDeviceEnabled(TRUE);
	  rc = pCardDispenser->GetResultCode();
      trace(L7, _T("Data Event Enabled Status %d"), pCardDispenser->GetResultCode());
   }
   PROCESS_EXCEPTIONS(DMCLASS_CARDDISPENSER);	
   trace(L6, _T("-CardDispenserEnable 0x%x"), rc );
   return rc;
}

//******************************************************
//	Method:	 CardDispenserDisable
//  Purpose: To allow the application to call and disable the 
//			 Card Dispenser device when necessary.
//  Parameters:  
//  Return Values: long - OPOS_SUCCESS if successfull
//******************************************************
long DMProcedures::CardDispenserDisable()
{
   _TCHAR msg[] = T_ID _T("::CardDispenserDisable");

   trace(L6, _T("+CardDispenserDisable") );
   try
   {
      rc = 0;
      CCardDispenser* pCardDispenser = m_devmgr.GetCardDispenser( DEVICEID0 );
      ASSERT( pCardDispenser != NULL );

      pCardDispenser->SetDeviceEnabled(FALSE);
	  rc = pCardDispenser->GetResultCode();
      trace(L7, _T("Data Event Enabled Status %d"), pCardDispenser->GetResultCode());
   }
   PROCESS_EXCEPTIONS(DMCLASS_CARDDISPENSER);	
   trace(L6, _T("-CardDispenserDisable 0x%x"), rc );
   return rc;
}

//******************************************************
//	Method:	 CardDispenserGetCardPosition
//  Purpose: This method returns the current position of a 
//           card within the card dispenser
//  Parameters:  
//  Return Values: long - Card Position
//******************************************************
long DMProcedures::CardDispenserGetCardPosition()
{
   BSTR bstrValue = NULL;
   long lValue = 0;
   long nStatus;
   _TCHAR msg[] = T_ID _T("::CardDispenserGetCardPosition");

   trace(L6, _T("+CardDispenserGetCardPosition") );
   try
   {
      rc = 0;
      CCardDispenser* pCardDispenser = m_devmgr.GetCardDispenser( DEVICEID0 );
      ASSERT( pCardDispenser != NULL );

	  if(pCardDispenser->DirectIO(NCRDIO_CHAN_CARD_POSITION,&lValue,&bstrValue) == OPOS_SUCCESS)
	  {
		  switch (lValue)
			{
				case SANKYO_USER_MT_CRD_STAT_1:
					nStatus = DM_CD_CARD_REMOVED;
					break;
				case SANKYO_USER_MT_CRD_STAT_2:
					nStatus = DM_CD_CARD_AT_GATE;
					break;
				case SANKYO_USER_MT_CRD_STAT_3:
					nStatus =  DM_CD_CARD_IN_TRANSPORT;
					break;
				default:
					nStatus = -1;
					break;
			}
		    rc = nStatus;
	  }
	  else 
	  {
		  rc = -1;
	  }
   }
   PROCESS_EXCEPTIONS(DMCLASS_CARDDISPENSER);	
   trace(L6, _T("-CardDispenserGetCardPosition 0x%x"), rc );
   return rc;
}

//******************************************************
//	Method:	 CardDispenserDispense
//  Purpose: To allow the application to issue a dispense command to the card dispenser, 
//			  which will load a card into the transport.  
//			  The application will receive a status event when the card arrives in the transport.  
//			  This method will be responsible for decrementing the current count of cards 
//			   in the bin and saving it to the registry.
//  Parameters:  
//  Return Values: long - OPOS_SUCCESS if successfull
//******************************************************
long DMProcedures::CardDispenserDispense()
{
   BSTR bstrValue = NULL;
   long lValue = 0;
   _TCHAR msg[] = T_ID _T("::CardDispenserDispense");

   HKEY hKey;
   long lBinCount = 0;	//number of cards in the bin
   unsigned long lLength = sizeof( lBinCount );
   trace(L6, _T("+CardDispenserDispense") );

   try
   {
      rc = 0;
      CCardDispenser* pCardDispenser = m_devmgr.GetCardDispenser( DEVICEID0 );
      ASSERT( pCardDispenser != NULL );

	  long rc = pCardDispenser->DirectIO(NCRDIO_CHAN_DISPENSE_CARD,&lValue,&bstrValue);

	  //+write to registry the number of cards in the bin
	  if ( ERROR_SUCCESS ==  RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\"), \
				0, KEY_ALL_ACCESS, &hKey ) )
	  {
			if ( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("CardDispenserBinCount"), NULL, NULL, \
					(LPBYTE)&lBinCount, &lLength  ) )
			{
				lBinCount--;

				if (lBinCount < 0 || OPOS_SUCCESS != rc ) //if lBinCount less than zero 
				{
					lBinCount = 0;						  // set lBinCount to zero 	
				}

				RegSetValueEx(hKey, _T("CardDispenserBinCount"), 0L, REG_DWORD, \
						     (LPBYTE)&lBinCount, sizeof(DWORD) ) ;
			}

			
		}
	  RegCloseKey( hKey );
	  //-write to registry the number of cards in the bin

   }
   PROCESS_EXCEPTIONS(DMCLASS_CARDDISPENSER);	
   trace(L6, _T("-CardDispenserDispense 0x%x"), lBinCount );
   return rc;
}

//******************************************************
//	Method:	 CardDispenserRead
//  Purpose: To provide a way for the application to read the card currently loaded into 
//			 the transport.  If the read is successful, an event is posted to the application, 
//			 identical to that which would be received from the MSR.
//  Parameters:  
//  Return Values: long - OPOS_SUCCESS if successfull
//******************************************************
long DMProcedures::CardDispenserRead()
{
   BSTR bstrValue = NULL;
   long lValue = 0;
   CString csEmpty;
   CString sData;
   csEmpty.Format(_T("%c%c"), DM_FS, DM_FS);
   _TCHAR msg[] = T_ID _T("::CardDispenserRead");

   trace(L6, _T("+CardDispenserRead") );
   try
   {
      rc = 0;
      CCardDispenser* pCardDispenser = m_devmgr.GetCardDispenser( DEVICEID0 );
      ASSERT( pCardDispenser != NULL );

	  rc = pCardDispenser->DirectIO(NCRDIO_CHAN_FORCED_READ,&lValue,&bstrValue);
	  if(rc == OPOS_SUCCESS)
	  {
		   // If we succeed, we're faking an MSR read message
		   long nTrackToRead = pCardDispenser->GetTracksToRead();
		   sData.Format( _T("%s%c%s%c%s"), 
			  nTrackToRead & DM_MSR_TRACK_1 ? pCardDispenser->GetTrack1Data() : _T(""), DM_FS, 
			  nTrackToRead & DM_MSR_TRACK_2 ? pCardDispenser->GetTrack2Data() : _T(""), DM_FS, 
			  nTrackToRead & DM_MSR_TRACK_3 ? pCardDispenser->GetTrack3Data() : _T("") );
		   if (sData == csEmpty)
		   {
			   rc =  -1;
		   }
		   else
		   {
			   //send sData to DMParse
		       mo.PostDM(DM_DATA, DMCLASS_MSR, 0, sData.GetLength(), (void*)LPCTSTR(sData));
		   }
	  }
	

   }
   PROCESS_EXCEPTIONS(DMCLASS_CARDDISPENSER);	
   trace(L6, _T("-CardDispenserRead 0x%x "), rc);
   return rc;

}

//******************************************************
//	Method:	 CardDispenserMoveToReject
//  Purpose: Issues a command to the card dispenser to take a card currently in the 
//			 transport to the reject bin.
//			  A status event indicating the card is removed should be received by 
//			  the application when the reject has completed.
//  Parameters:  
//  Return Values: long - OPOS_SUCCESS if successfull
//******************************************************
long DMProcedures::CardDispenserMoveToReject()
{
   BSTR bstrValue = NULL;
   long lValue = 0;
   _TCHAR msg[] = T_ID _T("::CardDispenserMoveToReject");

   trace(L6, _T("+CardDispenserMoveToReject") );
   try
   {
      rc = 0;
      CCardDispenser* pCardDispenser = m_devmgr.GetCardDispenser( DEVICEID0 );
      ASSERT( pCardDispenser != NULL );

	  long rc = pCardDispenser->DirectIO(NCRDIO_CHAN_TREJECT_STACKER,&lValue,&bstrValue);
   }
   PROCESS_EXCEPTIONS(DMCLASS_CARDDISPENSER);	
   trace(L6, _T("-CardDispenserMoveToReject 0x%x"), rc );
   return rc;
}

//******************************************************
//	Method:	 CardDispenserMoveToGate
//  Purpose: Called by the application to move the card currently in the transport 
//			 to the gate, where the customer may retrieve the card.  
//			 A status event indicating the card is removed should be received 
//			 when the customer takes the card.
//  Parameters:  
//  Return Values: long - OPOS_SUCCESS if successfull
//******************************************************
long DMProcedures::CardDispenserMoveToGate()
{
   BSTR bstrValue = NULL;
   long lValue = 0;
   _TCHAR msg[] = T_ID _T("::CardDispenserMoveToGate");

   trace(L6, _T("+CardDispenserMoveToGate") );
   try
   {
      rc = 0;
      CCardDispenser* pCardDispenser = m_devmgr.GetCardDispenser( DEVICEID0 );
      ASSERT( pCardDispenser != NULL );

	  // The timeout on the next command isn't used anyway.
	  long rc = pCardDispenser->BeginRemoval(0);
	  if (rc == OPOS_E_EXTENDED)
	  {
		  rc = pCardDispenser->GetResultCodeExtended();
	  }
   }
   PROCESS_EXCEPTIONS(DMCLASS_CARDDISPENSER);	
   trace(L6, _T("-CardDispenserMoveToGate 0x%x"), rc );
   return rc;
}

//******************************************************
//	Method:	 CardDispenserCaptureCard
//  Purpose: Allow the application to retrieve a card, currently at the gate, back to the transport. 
//           A status event indicating the card is in the transport should be received 
//			 when the operation has been successfully completed. 
//  Parameters:  
//  Return Values: long - OPOS_SUCCESS if successfull
//******************************************************
long DMProcedures::CardDispenserCaptureCard()
{
   BSTR bstrValue = NULL;
   long lValue = 0;
   _TCHAR msg[] = T_ID _T("::CardDispenserCaptureCard");

   trace(L6, _T("+CardDispenserCaptureCard") );
   try
   {
      rc = 0;
      CCardDispenser* pCardDispenser = m_devmgr.GetCardDispenser( DEVICEID0 );
      ASSERT( pCardDispenser != NULL );

	  long rc = pCardDispenser->DirectIO(NCRDIO_CHAN_RETRY,&lValue,&bstrValue);
   }
   PROCESS_EXCEPTIONS(DMCLASS_CARDDISPENSER);	
   trace(L6, _T("-CardDispenserCaptureCard 0x%x"), rc );
   return rc;
}

//******************************************************
//	Method:	 CardDispenserSetTracksToRead
//  Purpose:  May be called to specify which tracks the application wants to 
//			  attempt to read from the card.  The application may select from 
//			  tracks one to three in any combination by setting the 1 bit for track 1, 
//			  the 2 bit for track 2 and the 3rd bit (with a value of 4) for track 3.  
//			  These bits are anded together to specify which tracks should be read.  
//			  A value of 3 would be sent to read tracks 1 and 2.
//  Parameters:  
//  Return Values: long - OPOS_SUCCESS if successfull
//******************************************************
long DMProcedures::CardDispenserSetTracksToRead(long nNewValue)
{
   BSTR bstrValue = NULL;
   long lValue = 0;
   _TCHAR msg[] = T_ID _T("::CardDispenserSetTracksToRead");

   trace(L6, _T("+CardDispenserSetTracksToRead") );
   try
   {
      rc = OPOS_SUCCESS;
      CCardDispenser* pCardDispenser = m_devmgr.GetCardDispenser( DEVICEID0 );
      ASSERT( pCardDispenser != NULL );

	  // The timeout on the next command isn't used anyway.
	  pCardDispenser->SetTracksToRead(nNewValue);
   }
   PROCESS_EXCEPTIONS(DMCLASS_CARDDISPENSER);	
   trace(L6, _T("-CardDispenserSetTracksToRead 0x%x"), rc );
   return rc;
}

//******************************************************
//	Method:	 GetCardDispenserVeryLow
//  Purpose:  Returns a true if the card dispenser sensor is showing a low condition, 
//			  which is typically fewer than 20 cards remaining.  
//			  It returns false if the sensor does not indicate a low condition.
//  Parameters:  
//  Return Values: bool - true/false
//******************************************************
bool DMProcedures::GetCardDispenserVeryLow()
{
   bool bReturn = false;
   BSTR bstrValue = NULL;
   long lValue = 0;
   _TCHAR msg[] = T_ID _T("::GetCardDispenserVeryLow");

   trace(L6, _T("+GetCardDispenserVeryLow") );
   try
   {
      rc = 0;
      CCardDispenser* pCardDispenser = m_devmgr.GetCardDispenser( DEVICEID0 );
      ASSERT( pCardDispenser != NULL );

	  long rc = pCardDispenser->DirectIO(NCRDIO_CHAN_GET_HOPPER_STATUS,&lValue,&bstrValue);
	  // If the hopper is low or empty, return true
	  if ((lValue == SANKYO_USER_MT_CRD_HP_STAT_6) || (lValue == SANKYO_USER_MT_CRD_HP_STAT_7))
	  {
		  bReturn = true;
	  }
   }
   PROCESS_EXCEPTIONS(DMCLASS_CARDDISPENSER);	
   trace(L6, _T("-GetCardDispenserVeryLow 0x%x 0x%x"), bReturn, lValue );
   return bReturn;
}

//******************************************************
//	Method:	 GetCardDispenserEmpty
//  Purpose:  returns true if no cards are remaining in the hopper, otherwise it returns false.
//  Parameters:  
//  Return Values: bool - true/false
//******************************************************
bool DMProcedures::GetCardDispenserEmpty()
{
   bool bReturn = false;
   BSTR bstrValue = NULL;
   long lValue = 0;
   _TCHAR msg[] = T_ID _T("::GetCardDispenserEmpty");

   trace(L6, _T("+GetCardDispenserEmpty") );
   try
   {
      rc = 0;
      CCardDispenser* pCardDispenser = m_devmgr.GetCardDispenser( DEVICEID0 );
      ASSERT( pCardDispenser != NULL );

	  long rc = pCardDispenser->DirectIO(NCRDIO_CHAN_GET_HOPPER_STATUS,&lValue,&bstrValue);
	  // If the hopper is empty, return true
	  if (lValue == SANKYO_USER_MT_CRD_HP_STAT_7)
	  {
		  //set SOFTWARE\NCR\SCOT\CurrentVersion\SCOTAPP\CardDispenserBinCount to 0 if the hopper is empty
		  DWORD lBinCount = 0;
		  HKEY hKey;
		  long lRetVal = RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\"), \
		  					0, KEY_ALL_ACCESS, &hKey ) ;
	  	  if ( ERROR_SUCCESS == lRetVal )
		  {
			 RegSetValueEx(hKey, _T("CardDispenserBinCount"), 0, REG_DWORD, \
			 						     (LPBYTE)&lBinCount, sizeof(lBinCount) ) ;
		  }
		  RegCloseKey( hKey );
		  bReturn = true;
	  }
   }
   PROCESS_EXCEPTIONS(DMCLASS_CARDDISPENSER);	
   trace(L6, _T("-GetCardDispenserEmpty 0x%x 0x%x"), bReturn, lValue );
   return bReturn;
}
//******************************************************
//	Method:	 GetCardDispenserGateOpen
//  Purpose: Returns true if the gate next to the transport on the 
//			 gift card dispenser has been left open.
//  Parameters:  
//  Return Values: bool - true/false
//******************************************************
bool DMProcedures::GetCardDispenserGateOpen()
{
   bool bReturn = false;
   BSTR bstrValue = NULL;
   long lValue = 0;
   _TCHAR msg[] = T_ID _T("::GetCardDispenserGateOpen");

   trace(L6, _T("+GetCardDispenserGateOpen") );
   try
   {
      rc = 0;
      CCardDispenser* pCardDispenser = m_devmgr.GetCardDispenser( DEVICEID0 );
      ASSERT( pCardDispenser != NULL );

	  long rc = pCardDispenser->DirectIO(NCRDIO_CHAN_GET_HOPPER_STATUS,&lValue,&bstrValue);
	  // If the hopper is empty, return true
	  if (lValue == SANKYO_USER_MT_CRD_ERR_27)
	  {
		  bReturn = true;
	  }
   }
   PROCESS_EXCEPTIONS(DMCLASS_CARDDISPENSER);	
   trace(L6, _T("-GetCardDispenserGateOpen 0x%x 0x%x"), bReturn, lValue );
   return bReturn;
}

//******************************************************
//	Method:	 GetAdjustCardDispenserBinCounts
//  Purpose: Returns the current count of cards in the main bin and in the reject bin.
//			 It also accepts an adjustment value as a 3rd parameter. 
//  Parameters:  [out] long& lCountBin - cards in the main bin 
//				 [out] long& lCountReject - cards in the reject bin
//				 [in] long lAdjValue - The adjustment value, if not 0, 
//									   is used to increase or decrease the 
//									   current card dispenser bin count.
//  Return Values: 
//******************************************************
void DMProcedures::GetAdjustCardDispenserBinCounts(long& lCountBin, long& lCountReject, const long lAdjValue)
{
  BSTR bstrValue = NULL;
  _TCHAR msg[] = T_ID _T("::GetAdjustCardDispenserBinCounts");
  long lRetVal;
  long lRejectCount = 0;		//number of cards in the reject bin
  long lBinCount = 0;			//number of cards in the bin
  unsigned long lLength = sizeof(lBinCount );

   trace(L6, _T("+GetAdjustCardDispenserBinCounts") );

    //+get lCountBin
	HKEY hKey;
	lRetVal = RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\"), \
		0, KEY_ALL_ACCESS, &hKey ) ;
	if ( ERROR_SUCCESS == lRetVal )
	{

		lRetVal = RegQueryValueEx(hKey, _T("CardDispenserBinCount"), NULL, NULL, \
		     (LPBYTE)&lBinCount , &lLength  ) ;
		if ( ERROR_SUCCESS == lRetVal )
		{
			if ( 0 != lAdjValue )
			{
				lBinCount  += lAdjValue ;
				if ( lBinCount  < 0 )
				{
					lBinCount = 0;
				}
				RegSetValueEx(hKey, _T("CardDispenserBinCount"), 0, REG_DWORD, \
						     (LPBYTE)&lBinCount, sizeof(lBinCount) ) ;
			}
			lCountBin = lBinCount;
		}
	}
	RegCloseKey( hKey );
	//-get lCountBin
	//+get lCountReject
   try
   {
      rc = 0;
      CCardDispenser* pCardDispenser = m_devmgr.GetCardDispenser( DEVICEID0 );
      ASSERT( pCardDispenser != NULL );
	  
	  long rc = pCardDispenser->DirectIO(NCRDIO_CHAN_GET_REJECT_STACKER, &lRejectCount,&bstrValue);
	  //set lCountReject to -1 if unsuccessful
	  if (!(rc == OPOS_SUCCESS ))
	  {
		  lCountReject = -1;  
	  }
	  lCountReject = lRejectCount;
   }
   PROCESS_EXCEPTIONS(DMCLASS_CARDDISPENSER);	
   
   //-get lCountReject
   trace(L6, _T("-GetAdjustCardDispenserBinCounts 0x%x 0x%x"), lCountBin, lCountReject);
}
//******************************************************
//	Method:	 CardDispenserWrite
//  Purpose: Allow the application to write to a card
//  Parameters:  [in] long lTracksToWrite - which has values 
//											DM_MSR_TRACK_1, DM_MSR_TRACK_2, 
//											and DM_MSR_TRACK_3 logically ored together 
//											as desired to specify the tracks to be written.     
//				 [in] CString csTrack1Data - data for Track1
//				 [in] CString csTrack2Data - data for Track2
//				 [in] CString csTrack3Data - data for Track3
//  Return Values: bool - true if write operation is successfull
//******************************************************
bool DMProcedures::CardDispenserWrite( const long lTracksToWrite, const CString& csTrack1Data, \
		const CString& csTrack2Data, const CString& csTrack3Data )
{
   _TCHAR msg[] = T_ID _T("::CardDispenserWrite");
   bool bReturn = false;

   trace(L6, _T("+CardDispenserWrite") );
	try
	{
		CCardDispenser* pCardDispenser = m_devmgr.GetCardDispenser( DEVICEID0 );
		ASSERT( pCardDispenser != NULL );

		
		pCardDispenser->SetTracksToWrite ( lTracksToWrite );

		if ( lTracksToWrite & DM_MSR_TRACK_1 )		//TRACK 1
		{
			pCardDispenser->SetWrite1Data( csTrack1Data );
		}
		if ( lTracksToWrite & DM_MSR_TRACK_2 )		//TRACK 2
		{
			pCardDispenser->SetWrite2Data( csTrack2Data );
		}
		if ( lTracksToWrite & DM_MSR_TRACK_3 )		//TRACK 3
		{
			pCardDispenser->SetWrite3Data( csTrack3Data );
		}
		
		if (pCardDispenser->PrintWrite(2,0,0,_T("")) == OPOS_SUCCESS )
		{
			bReturn = true;
		}
	}
	PROCESS_EXCEPTIONS(DMCLASS_CARDDISPENSER);
	trace(L6, _T("-CardDispenserWrite") );
	return bReturn;
}
//******************************************************
//	Method:  CardDispenserEnableInsertion(
//  Purpose: This method control card insertion.
//  Parameters:     [in] bool bEnable 
//							true - enable card insertion
//							false - disable card insertion	
//  Return Values: 
//******************************************************
void DMProcedures::CardDispenserEnableInsertion(bool bEnable )
{
   _TCHAR msg[] = T_ID _T("::CardDispenserEnableInsertion");
   trace(L6, _T("+CardDispenserEnableInsertion") );
	try
	{
		CCardDispenser* pCardDispenser = m_devmgr.GetCardDispenser( DEVICEID0 );
		ASSERT( pCardDispenser != NULL );
		bEnable ? pCardDispenser->BeginInsertion( 0 ):pCardDispenser->EndInsertion();
	}
   PROCESS_EXCEPTIONS(DMCLASS_CARDDISPENSER);
   trace(L6, _T("-CardDispenserEnableInsertion") );
}
//-4.2 card dispenser

void DMProcedures::OnTimer(UINT nIDEvent) 
   {
	// TODO: Add your message handler code here and/or call default
   if ( nIDEvent == WM_DISABLE_DEPOSIT )
      {
      _TCHAR msg[] = T_ID _T("::OnTimer(WM_DISABLE_DEPOSIT)");
      KillTimer(WM_DISABLE_DEPOSIT);
      if ( IsDeviceConfigured( DMCLASS_CASHRECYCLER, DEVICEID0 ) != false )
		{
	      try
	         {
			 trace(L6, _T("+OnTimer()"));
	         CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
	         rc = pDevice->EndDeposit( CHAN_DEPOSIT_NOCHANGE );
	         }
	      PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);	
		}
      }
	CWnd::OnTimer(nIDEvent);
   return;
   }



BOOL DMProcedures::SoundConnectToServer(int iFocus, int iActive)
{
	BOOL rc = (BOOL) SCOTPriceSoundConnectToServer((LPCTSTR)ps.GetRemoteServerName(),
		                                            iActive,
																  iFocus);
	return rc;

}

BOOL DMProcedures::SoundDisconnectFromServer(BOOL bSendDisconnectMsg)
{
	BOOL rc = (BOOL) SCOTPriceSoundDisconnectFromServer(bSendDisconnectMsg);
	return rc;
}



long DMProcedures::BeginRemote( 
   LPCTSTR sRemoteKey /* = _T("Remote") */ )
   {
   trace(L6, _T("+BeginRemote") );
   _TCHAR msg[] = T_ID _T("::BeginRemote");
   long nInError = -1;
   try
      {
      rc = 0;
      m_devmgr.OpenRemote( sRemoteKey );
      nInError = DMICheckDevices( );
      if ( nInError != 0 )
         {
         ScotError(RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_DMINTERFACE_DEVICE_ERROR2,_T("%d"),nInError);
         trace(L6,_T("CloseRemote!!"));
         m_devmgr.CloseRemote();
         rc = -3;
         }
      }
   PROCESS_EXCEPTIONS(DM_ALL_DEVICECLASSES);
   trace(L6, _T("-BeginRemote 0x%x"), rc );
   return rc;
   }

long DMProcedures::EndRemote( 
   void )
   {
   trace(L6, _T("+EndRemote") );
   _TCHAR msg[] = T_ID _T("::EndRemote");
   long nInError = -1;
   try
      {
      rc = 0;
      m_devmgr.CloseRemote();
      nInError = DMICheckDevices( );
      if ( nInError != 0 )
         {
         ScotError(RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_DMINTERFACE_DEVICE_ERROR2,
            _T("%d"),nInError);
         rc = -3;
         }
      }
   PROCESS_EXCEPTIONS(DM_ALL_DEVICECLASSES);
   trace(L6, _T("-EndRemote 0x%x"), rc );
   return rc;
   }
bool DMProcedures::IsDeviceRemote(long nDeviceClass, long nDeviceId)
   {
   bool bResult = false;
   _TCHAR msg[] = T_ID _T("::IsDeviceRemote");
   trace(L6, _T("+IsDeviceRemote(nDeviceClass = <%d>, nDeviceId = <%d>)"), 
      nDeviceClass, nDeviceId );
   try
      {
      bResult = !!m_devmgr.IsDeviceRemote( nDeviceClass, nDeviceId );
      }
   PROCESS_EXCEPTIONS(nDeviceClass);

   trace(L6, _T("-IsDeviceRemote returning %s"), bResult ? _T("true") : _T("false"));
   return bResult;
   }
static struct 
   {
   DWORD dwEvent;
   LPCTSTR sEvent;
   } s_aEventDescriptionMap [] =
   {
      DM_DRAWER_OPEN,_T("DM_DRAWER_OPEN"),
      DM_DRAWER_CLOSED,_T("DM_DRAWER_CLOSED"),
      DM_PTR_COVER_OPEN,_T("DM_PTR_COVER_OPEN"),
      DM_PTR_COVER_CLOSED,_T("DM_PTR_COVER_CLOSED"),
      DM_PTR_EMPTY,_T("DM_PTR_EMPTY"),
      DM_PTR_NEAREMPTY,_T("DM_PTR_NEAREMPTY"),
      DM_PTR_PAPEROK,_T("DM_PTR_PAPEROK"),
      DM_PTR_ERR_COVER_OPEN,_T("DM_PTR_ERR_COVER_OPEN"),
      DM_PTR_ERR_EMPTY,_T("DM_PTR_ERR_EMPTY"),
      DM_PTR_ERR_FAULT,_T("DM_PTR_ERR_FAULT"),
      DM_PTR_REQUEST_COMPLETED,_T("DM_PTR_REQUEST_COMPLETED"),
      DM_PTR_ERR_REQUEST_SEQUENCE,_T("DM_PTR_ERR_REQUEST_SEQUENCE"),
      DM_PTR_ERR_HEAD_JAM,_T("DM_PTR_ERR_HEAD_JAM"),
      DM_PTR_ERR_KNIFE_JAM,_T("DM_PTR_ERR_KNIFE_JAM"),
      DM_PTR_ERR_PAPER_JAM,_T("DM_PTR_ERR_PAPER_JAM"),
      DM_PTR_ERR_PAPER_NOT_LOADED_JAM,_T("DM_PTR_ERR_PAPER_NOT_LOADED_JAM"),
      DM_PTR_ERR_COMMS_ERROR,_T("DM_PTR_ERR_COMMS_ERROR"),
      DM_PTR_ERR_PRINTER_OPEN,_T("DM_PTR_ERR_PRINTER_OPEN"),
      DM_PTR_ERR_REPLACE_HEAD,_T("DM_PTR_ERR_REPLACE_HEAD"),
      DM_PTR_ERR_SIDEWAYS_COMMS_ERROR,_T("DM_PTR_ERR_SIDEWAYS_COMMS_ERROR"),
      DM_PTR_ERR_EJECT_JAM,_T("DM_PTR_ERR_EJECT_JAM"),
      DM_PTR_ERR_BLACKMARK_ERROR,_T("DM_PTR_ERR_BLACKMARK_ERROR"),
      DM_PTR_ERR_TIME_OUT,_T("DM_PTR_ERR_TIME_OUT"),
      DM_KEYLOCK_L,_T("DM_KEYLOCK_L"),
      DM_KEYLOCK_N,_T("DM_KEYLOCK_N"),
      DM_KEYLOCK_S,_T("DM_KEYLOCK_S"),
      DM_KEYLOCK_X,_T("DM_KEYLOCK_X"),
      DM_MSR_CAPBIN_FULL,_T("DM_MSR_CAPBIN_FULL"),
      DM_MSR_CAPBIN_REMOVED,_T("DM_MSR_CAPBIN_REMOVED"),
      DM_MSR_CAPBIN_INSERTED,_T("DM_MSR_CAPBIN_INSERTED"),
      DM_MSR_CARD_REMOVED,_T("DM_MSR_CARD_REMOVED"),
      DM_MSR_CARD_INSERTED_STAGED,_T("DM_MSR_CARD_INSERTED_STAGED"),
      DM_MSR_CARD_DETECTED_ENTERING,_T("DM_MSR_CARD_DETECTED_ENTERING"),
      DM_MSR_DEVICE_INOPERABLE,_T("DM_MSR_DEVICE_INOPERABLE"),
      DM_MSR_INVALID_CARD_INSERTED,_T("DM_MSR_INVALID_CARD_INSERTED"),
      DM_MSR_SHUTTER_JAMMED_CLOSE,_T("DM_MSR_SHUTTER_JAMMED_CLOSE"),
      DM_MSR_CARD_CAPTURED,_T("DM_MSR_CARD_CAPTURED"),
      DM_CASHACCEPTOR_STATUS_OK,_T("DM_CASHACCEPTOR_STATUS_OK"),
      DM_CASHACCEPTOR_STATUS_INSERTED,_T("DM_CASHACCEPTOR_STATUS_INSERTED"),
      DM_CASHACCEPTOR_STATUS_REMOVED,_T("DM_CASHACCEPTOR_STATUS_REMOVED"),
      DM_CASHACCEPTOR_STATUS_REJECTED,_T("DM_CASHACCEPTOR_STATUS_REJECTED"),
      DM_CASHACCEPTOR_STATUS_RESET,_T("DM_CASHACCEPTOR_STATUS_RESET"),
      DM_CASHACCEPTOR_STATUS_JAM,_T("DM_CASHACCEPTOR_STATUS_JAM"),
      DM_CASHACCEPTOR_STATUS_FAILURE,_T("DM_CASHACCEPTOR_STATUS_FAILURE"),
      DM_CASHACCEPTOR_STATUS_FULL,_T("DM_CASHACCEPTOR_STATUS_FULL"),
      DM_CASHACCEPTOR_STATUS_NEARFULL,_T("DM_CASHACCEPTOR_STATUS_NEARFULL"),
      DM_CASHACCEPTOR_STATUS_TAMPER_IN,_T("DM_CASHACCEPTOR_STATUS_TAMPER_IN"),
      DM_CASHACCEPTOR_STATUS_TAMPER_OUT,_T("DM_CASHACCEPTOR_STATUS_TAMPER_OUT"),
      DM_CASHACCEPTOR_STATUS_ASYNC,_T("DM_CASHACCEPTOR_STATUS_ASYNC"),
      DM_CASHACCEPTOR_STATUS_CHEAT,_T("DM_CASHACCEPTOR_STATUS_CHEAT"),
      DM_CASHACCEPTOR_SUCCESS,_T("DM_CASHACCEPTOR_SUCCESS"),
      DM_MSR_CARD_DETECTED,_T("DM_MSR_CARD_DETECTED"),
      DM_MSR_DATA_AVAILABLE,_T("DM_MSR_DATA_AVAILABLE"),
      DM_MSR_CARD_NOT_CAPTURED,_T("DM_MSR_CARD_NOT_CAPTURED"),
      DM_MSR_EJECT_CAPTURE,_T("DM_MSR_EJECT_CAPTURE"),
      DM_MSR_POSSIBLE_CUST_TAMPERING,_T("DM_MSR_POSSIBLE_CUST_TAMPERING"),
      DM_SIGCAP_STARTED,_T("DM_SIGCAP_STARTED"),
      DM_COINACCEPTOR_STATUS_OK,_T("DM_COINACCEPTOR_STATUS_OK"),
      DM_COINACCEPTOR_STATUS_ASYNC,_T("DM_COINACCEPTOR_STATUS_ASYNC"),
      DM_COINACCEPTOR_STATUS_RETURNED,_T("DM_COINACCEPTOR_STATUS_RETURNED"),
      DM_COINACCEPTOR_STATUS_REJECTED,_T("DM_COINACCEPTOR_STATUS_REJECTED"),
      DM_COINACCEPTOR_STATUS_RESET,_T("DM_COINACCEPTOR_STATUS_RESET"),
      DM_COINACCEPTOR_STATUS_JAM,_T("DM_COINACCEPTOR_STATUS_JAM"),
      DM_COINACCEPTOR_STATUS_FAILURE,_T("DM_COINACCEPTOR_STATUS_FAILURE"),
      DM_COINACCEPTOR_STATUS_FULL,_T("DM_COINACCEPTOR_STATUS_FULL"),
      DM_COINACCEPTOR_STATUS_NEARFULL,_T("DM_COINACCEPTOR_STATUS_NEARFULL"),

      DM_COINACCEPTOR_STATUS_TAMPER_IN,_T("DM_COINACCEPTOR_STATUS_TAMPER_IN"),
      DM_COINACCEPTOR_STATUS_TAMPER_OUT,_T("DM_COINACCEPTOR_STATUS_TAMPER_OUT"),
      DM_COINACCEPTOR_STATUS_ESCROW_FULL,_T("DM_COINACCEPTOR_STATUS_ESCROW_FULL"),
      DM_COINACCEPTOR_STATUS_CLOSED,_T("DM_COINACCEPTOR_STATUS_CLOSED"),
      DM_COINACCEPTOR_STATUS_CHEAT,_T("DM_COINACCEPTOR_STATUS_CHEAT"),
      DM_COINACCEPTOR_SUCCESS,_T("DM_COINACCEPTOR_SUCCESS"),
      DM_COINACCEPTOR_STATUS_INOPERABLE,_T("DM_COINACCEPTOR_STATUS_INOPERABLE"),
      DM_SIGCAP_ENDED,_T("DM_SIGCAP_ENDED"),
      DM_SIGCAP_FAILURE,_T("DM_SIGCAP_FAILURE"),
      DM_STATUS_UNEXPECTED,_T("DM_STATUS_UNEXPECTED"),
      0,_T("")
   };

CString DMProcedures::GetEventDescription(DWORD dwEvent)
   {
   int iIndex = 0;
   while (s_aEventDescriptionMap[iIndex].dwEvent)
      {
      if (s_aEventDescriptionMap[iIndex].dwEvent == dwEvent)
         return s_aEventDescriptionMap[iIndex].sEvent;
      iIndex++;
      }
   trace(L0, _T("There is not a description for event 0x%08.8x in the event description map.  Please add one!"));
   ASSERT(FALSE);
   return _T("Unknown!");
   }


long DMProcedures::EASDeactivatorEnable( 
   long nEnableMode )
   {
   ASSERT( nEnableMode == EASEnable );
   _TCHAR msg[] = T_ID _T("::EASDeactivatorEnable");
   trace(L6, _T("+EASDeactivatorEnable()") );
   CString csWork = m_devmgr.GetDeviceAvailability( DMCLASS_EASDEACTIVATOR );
   for ( int nDeviceId = 0; nDeviceId < csWork.GetLength(); nDeviceId++ )
      {
      if ( csWork[ nDeviceId ] == DM_DA_LOADED )
         {
         CEASWrapper* pDevice = m_devmgr.GetEASWrapper( nDeviceId );
         ASSERT( pDevice != NULL );
         Enable( DMCLASS_EASDEACTIVATOR, nDeviceId, true );
         pDevice->SetTagDeactivateMode(nEnableMode);
         }
      }  
   trace(L6, _T("-EASDeactivatorEnable()") );
   return 0;
   }


long DMProcedures::EASDeactivatorDisable( 
   void )
   {
   _TCHAR msg[] = T_ID _T("::EASDeactivatorDisable");
   trace(L6, _T("+EASDeactivatorDisable()") );
   CString csWork = m_devmgr.GetDeviceAvailability( DMCLASS_EASDEACTIVATOR );
   for ( int nDeviceId = 0; nDeviceId < csWork.GetLength(); nDeviceId++ )
      {
      if ( csWork[ nDeviceId ] == DM_DA_LOADED )
         {
         CEASWrapper* pDevice = m_devmgr.GetEASWrapper( nDeviceId );
         ASSERT( pDevice != NULL );

         Enable( DMCLASS_EASDEACTIVATOR, nDeviceId, false );
         }
      }  
   trace(L6, _T("-EASDeactivatorDisable()") );
   return 0;
   }

bool DMProcedures::EASDeactivatorPresent( 
   void )
   {
   CString csWork = m_devmgr.GetDeviceAvailability( DMCLASS_EASDEACTIVATOR );
   return csWork.Find( DM_DA_LOADED ) != -1;
   }


bool DMProcedures::IsCouponSensorAvailable(
	void )
	{
	trace(L6, _T("+/-IsCouponSensorAvailable"));
    return m_devmgr.GetDeviceStatistics( DMCLASS_MOTION_SENSOR, 1 ).cStatus == DM_DA_LOADED;
	}

/*
long DMProcedures::CouponSensorEnable( void )
   {
   trace(L6, "+CouponSensorEnable");
   rc = Enable(DMCLASS_MOTION_SENSOR, 1, true );
   trace(L6,"-CouponSensorEnable 0x%x",rc);
   return rc;
   }
*/

bool DMProcedures::IsDeviceConfigured( 
   long nDeviceClass, 
   long nDeviceId /* = 0 */) // TAR199516
   {
   return m_devmgr.GetDeviceStatistics( nDeviceClass, nDeviceId ).cStatus == DM_DA_LOADED;
   }

long DMProcedures::GetResultCodeExtended(   long nDeviceClass, 
   long nDeviceId)
{
   trace(L6,_T("+GetResultCodeExtended nDeviceClass=%d nDeviceId=%d"),nDeviceClass, nDeviceId);
   	_TCHAR msg[] = T_ID _T("::GetResultCodeExtended");

	CCashRecyclerWrapper* pDevice = NULL;

   //First call checkhealth before get extended result code.
    CDeviceStatistics dvs = m_devmgr.GetDeviceStatistics( nDeviceClass, nDeviceId );
	BOOL bDeviceToBeDisabled = dvs.fIsEnabled == FALSE;
   rc = 0;
	if ( bDeviceToBeDisabled  != false )
	   {
		trace(L6, _T("Enabling device before check health"));
		rc = Enable(nDeviceClass, DEVICEID0, true);   //??? Feng
	   }
	try
	   {
      if ( nDeviceClass <= DMCLASS_PRINTER )
         {
         if ( dvs.cStatus == DM_DA_LOADED )
            {
               CPrintStation* pPrinter = m_devmgr.GetPrintStation( nDeviceClass, nDeviceId );
               // return success if the printer is busy, - it must be healthy
               rc = OPOS_SUCCESS;
               if ( pPrinter->GetState() != OPOS_S_BUSY )
			   {
                  rc = pPrinter->CheckHealth( 2 );
                /*  if ( rc != 0 )                     //TAR229356   //Tar 232725
                     rc = pPrinter->RetryOutput();
                  if ( rc == 0 )
                     OnStatusEventScotdevicemanager1( DM_PTR_REQUEST_COMPLETED, nDeviceClass, DEVICEID0 );
                  m_devmgr.ThrowDmException( pPrinter ); */
			   }
			}
		   else
            {
            ASSERT( nDeviceClass != DMCLASS_RECEIPT); // Slip CheckHealth??
			   trace(L6, _T("Not doing health check on Journal Printer."));
            }
         }
		else if ( rc == 0 )  // level 2 check health
		{
			if(nDeviceClass == DMCLASS_CASHCHANGER)  //for CashChange, need call internal checkhealth. 
			{
				m_devmgr.CheckHealth( nDeviceClass, nDeviceId, 1 ); 
			}
			else 
			{
				// I know that this is kludgy.  I don't like this, but if a health
				// check is called on the SafePay, then the extended result code is
				// wiped out. 
				if(m_csCashRecyclerModel != _CASHDISPENSER_SAFEPAY)
				{
					m_devmgr.CheckHealth( nDeviceClass, nDeviceId, 2 );      
				}
			}
		}
      }
   PROCESS_EXCEPTIONS(nDeviceClass );

   //Get ResultCodeExtended
   long rc;
	try
	   {
      if ( nDeviceClass <= DMCLASS_PRINTER )  //call dmx
         {         
               CPrintStation* pPrinter = m_devmgr.GetPrintStation( nDeviceClass, nDeviceId );
               rc = OPOS_SUCCESS;
               rc = pPrinter->GetResultCodeExtended( );
               //m_devmgr.ThrowDmException( pPrinter );   
        }
      else if(nDeviceClass == DMCLASS_CASHRECYCLER)
      {
            pDevice = m_devmgr.GetCashRecycler(DEVICEID0);
            if(pDevice)
            {
                rc = pDevice->GetResultCodeExtended();
            }
      }
      else
      {
            rc = m_devmgr.GetResultCodeExtended( nDeviceClass, nDeviceId );     
      }
    }
   PROCESS_EXCEPTIONS(nDeviceClass );
   
	//Diable device
      if (bDeviceToBeDisabled)
      {
	   trace(L6, _T("Disabling device after check health"));
	   Enable(nDeviceClass, DEVICEID0, false);
	   bDeviceToBeDisabled = false;
	   }

   trace(L6,_T("-GetResultCodeExtended rc =%d"), rc);
   return rc;
}

long DMProcedures::GetResultCode(   long nDeviceClass, 
   long nDeviceId)
{
   trace(L6,_T("+GetResultCode nDeviceClass=%d nDeviceId=%d"),nDeviceClass, nDeviceId);
   long rc;
	_TCHAR msg[] = T_ID _T("::GetResultCode");
	try
	   {
      if ( nDeviceClass <= DMCLASS_PRINTER )  //call dmx
         {         
               CPrintStation* pPrinter = m_devmgr.GetPrintStation( nDeviceClass, nDeviceId );
               rc = OPOS_SUCCESS;
               rc = pPrinter->GetResultCode( );
               m_devmgr.ThrowDmException( pPrinter ); 
		}
		            
      //else   //Currently only support GetResultCode for printers.
	//		rc = m_devmgr.GetResultCode( nDeviceClass, nDeviceId );     
      }
   PROCESS_EXCEPTIONS(nDeviceClass );

   trace(L6,_T("-GetResultCode rc =%d"), rc);
   return rc;
}


CString DMProcedures::GetDeviceName( long nDeviceClass, 
   long nDeviceId)
{
   trace(L6,_T("+GetDeviceName nDeviceClass=%d nDeviceId=%d"),nDeviceClass, nDeviceId);
	_TCHAR msg[] = T_ID _T("::GetDeviceName");

   CString retc;
	try
	   {
		  if ( nDeviceClass <= DMCLASS_PRINTER )  //call dmx
		  {         
			   CPrintStation* pPrinter = m_devmgr.GetPrintStation( nDeviceClass, nDeviceId );
           
			   CPrinterWrapper& cTempPrinterWrapper= pPrinter->GetPrinter();
			   retc = cTempPrinterWrapper.GetDeviceName();
			   m_devmgr.ThrowDmException( pPrinter ); 
		  }
		  else if(nDeviceClass == DMCLASS_SCANNER )   //270196
		  {
			  retc = ScannerGetDeviceName();
		  }
		  else if(nDeviceClass == DMCLASS_CASHDRAWER)   //270196
		  {
			  retc = CashDrawerGetDeviceName();
		  }
 		  else if(nDeviceClass == DMCLASS_MSR)   //270196
		  {
			  retc = MSRGetDeviceName();
		  }
		  //Begin RFC 356020: Support Check payment on lane using printer 7167
		  else if (nDeviceClass ==DMCLASS_MICR)
		  {
			  retc = MICRGetDeviceName();
		  }
		  else if(nDeviceClass == DMCLASS_CASHRECYCLER)
		  {
			  CCashRecyclerWrapper *pDevice = NULL;

			  retc = m_csCashRecyclerModel;
			  if(m_csCashRecyclerModel.IsEmpty())
			  {
				  pDevice = m_devmgr.GetCashRecycler(nDeviceId);				  
				  if(pDevice)
				  {
					  retc = pDevice->GetDeviceName();
					  retc.MakeUpper();
				  }
			  }			  			  
		  }
		  //End RFC 356020
 		  else   
				retc = m_devmgr.GetDeviceName( nDeviceClass, nDeviceId );     
      }
   PROCESS_EXCEPTIONS(nDeviceClass );

   trace(L6,_T("-GetDeviceName retc =%s"), retc);
   return retc;

}

CString DMProcedures::ReadReceiptPrinterModel(void)
{
    trace(L6, _T("+ReadReceiptPrinterModel()"));
	//Read Class Model 
	long retVal = 0; // Tar 260783  
	HKEY hKey;
	CString csPrinterModel, csWork;
	csPrinterModel = _K580;


	if( ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\ADD\\POSPrinterRec"),
				 0, KEY_QUERY_VALUE, &hKey))
	{
		DWORD dwValueSize=60;   //Tar 237697
		_TCHAR  szValue[61];
		if( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("DetectedDevice"), NULL, NULL, (LPBYTE)szValue, &dwValueSize))
		{
           csWork = szValue;
		   csWork.MakeUpper();
		   HKEY hKey2;
		   trace(L6, _T("ReadReceiptPrinterModel(). The model of RecPrinter in registry is %s."), csWork);
			if(csWork.Find(_T("K590")) != -1)
			{
				// K590, need look if it is Release 2 or Release 3
				if( ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\Installation\\platform"),
							 0, KEY_QUERY_VALUE, &hKey2))
				{
					// Tar 260783+
					dwValueSize = sizeof(szValue)-1; 
					retVal = RegQueryValueEx(hKey2, _T("Configure"), NULL, NULL, (LPBYTE)szValue, &dwValueSize);					
					// Get the configuration value		 										
					if(retVal == ERROR_SUCCESS)
					{
					    csWork = szValue;					 
					}										
					else
					{								
						trace(L2, _T("Error accessing data from \"Configure\" entry. Error Code Hex:%X Dec:%d"), retVal, retVal); 
					} 					
					// Tar 260783-

					if(ERROR_SUCCESS != ::RegCloseKey(hKey2))
					{
						trace(L6, _T("Error at ::RegCloseKey(hKey)"));
					}			
					// close registry if open succeeded
				}

				if(csWork.Find(_T("SCOT3")) != -1)
				{
					csPrinterModel = _K590REL3;
				}
				else
				{
					csPrinterModel = _K590REL2;
				}
			}
			else if(csWork.Find(_T("K580")) != -1)
			{
					csPrinterModel = _K580;
			}

			else if(csWork.Find(_T("F306")) != -1)
			{
				csPrinterModel = _PRINTER_F306;
			} 

		    else
			{
				trace(L6, _T("ReadReceiptPrinterModel() did not find a known model of Receipt Printer."));
			}
		}
	}
	else
	{
		trace(L6, _T("ReadReceiptPrinterModel()() did not find the registry key of Receipt Printer model."));
	}

	if(ERROR_SUCCESS != ::RegCloseKey(hKey))
	{
		trace(L6, _T("Error at ::RegCloseKey(hKey)"));
	}

    trace(L6, _T("-ReadReceiptPrinterModel(), csPrinterModel=%s"),csPrinterModel);
	return csPrinterModel;
}

//Read BillDispenserTypes from registry

void DMProcedures::ReadBillDispenserTypes(void)
{
	// Get a list of currency denominations in use from the currency map,
  // Example of a GetCashChangerCurrencyContainerMap string for US currency is:
  //
  // "USD:100,USD:500,USD:1000 -> Some Comments"
  // 
	CString csCurrencyMap;
	HKEY hKey;

	CString sProfile = _T("5633CurrDisp");

	if( ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\OLEforRetail\\ServiceOPOS\\CashChanger\\ScotCashChanger"),
				 0, KEY_QUERY_VALUE, &hKey))
	{
		DWORD dwValueSize;
		TCHAR*  szValue;
		if( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("BillSO"), NULL, NULL, NULL, &dwValueSize))
		{
			// dwValueSize is bytes.  We need characters.
			
			szValue = new TCHAR[ dwValueSize/sizeof(TCHAR) ];
			if( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("BillSO"), NULL, NULL, (LPBYTE)szValue, &dwValueSize))
			{
			   sProfile = szValue;
			}
			delete[] szValue;
		}
		::RegCloseKey(hKey);

		// close registry if open succeeded
	}

	if( ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\OLEforRetail\\ServiceOPOS\\CashChanger\\") + sProfile,
				 0, KEY_QUERY_VALUE, &hKey))
	{
		// Get the configuration value

		// TAR 353244:  dwValueSize is the number of BYTES, not characters.
		DWORD dwValueSize;  
		TCHAR*  szValue;
		if( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("CurrencyContainerMap"), NULL, NULL, NULL, &dwValueSize))
		{
			// dwValueSize is bytes.  We need characters.
			
			szValue = new TCHAR[ dwValueSize/sizeof(TCHAR) ];
			if( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("CurrencyContainerMap"), NULL, NULL, (LPBYTE)szValue, &dwValueSize))
			{
			   csCurrencyMap = szValue;
			}
			delete[] szValue;
		}
		::RegCloseKey(hKey);
		
		// close registry if open succeeded
	}

	int i =0, nIndex, colonIndex, commaIndex;
	if ( (nIndex = csCurrencyMap.Find(_T("->"))) != -1 ) //Remove comments
	{
		csCurrencyMap = csCurrencyMap .Left(nIndex);
	}


	// TAR 353244 - Added DMCASSETTE_COUNT check.  Incorrect registry setting
	//              could cause SCOTApp to crash.
    while(!csCurrencyMap.IsEmpty() && i < DMCASSETTE_COUNT)
	{
	  if ( (colonIndex = csCurrencyMap.Find(_T(":"))) != -1 )
	  {
		 if ( (commaIndex = csCurrencyMap.Find(_T(","))) != -1 )
		 {
			 m_nBillDispenserType [i] = _tcstoul(csCurrencyMap.Mid(colonIndex+1, commaIndex - colonIndex), 0, 10);
			 csCurrencyMap = csCurrencyMap.Mid(commaIndex+1);
			 i++;
		 }
		 else
		 {
			 m_nBillDispenserType [i] = _tcstoul(csCurrencyMap.Mid(colonIndex+1), 0, 10);
			 csCurrencyMap.Empty();
		 }
	  }
	  else
	  {
		csCurrencyMap.Empty();
	  }
	 }
	return;

}

long GetURLName(CString& csFileName)
{
	CString& csName = csFileName;
	long rc = -1;  // rc = -1 is unsuccessful,  rc = 0 or more is successful
	if (csName.IsEmpty())
	{
		return rc;
	}
	int nDriveLetterIndex = csName.Find(_T(":\\"));
	if (nDriveLetterIndex != -1)
	{
		// found non-URL path, resolve it to URL version
	    TCHAR szCompName[MAX_COMPUTERNAME_LENGTH + 1]; 
	    DWORD arraySize = sizeof(szCompName); 
	    GetComputerName(szCompName, &arraySize);
		csName.SetAt(nDriveLetterIndex, '$');
        CString csTBLogoFile;
		csTBLogoFile.Format(_T("%s%s%c%s"), _T("\\\\"), szCompName, '\\', csName);
		csName = csTBLogoFile;
		rc = 0;
	}
	TRACE(_T("GetURLName:%s"), csName);
	return rc;
}

/**
 * Function:		GetNonAdminURLName
 * Purpose:			Non-admin users cannot access the C$ administrative share.
 *		This function converts the filename to use a specific share of IMAGE or
 *		DATA rather than C$ (others can be added as necessary).
 *
 * Parameters:		csFileName - A CString of the form
 *		\\MACHINE\C$\SCOT\_DIRNAME_\file.xxx where _DIRNAME_ = image or data.
 *		csFileName is converted to \\MACHINE\_DIRNAME_\FILE.XXX if the method
 *		succeeds.
 * Returns:			true if successful.  false otherwise.  
 *					
 * Calls:			
 * Side Effects:	csFileName is not altered unless true is returned.
 * Notes:			Created for TARs 307991 and 308999.
 *					Internal method.
 */
bool DMProcedures::GetNonAdminURLName(CString &csFileName)
{
	int iDol;

	csFileName.MakeUpper();	// For uniform string search.

	iDol = csFileName.Find(_T("$\\SCOT\\IMAGE"));
	if(iDol == -1)
	{
		iDol = csFileName.Find(_T("$\\SCOT\\DATA"));
	}

	if(iDol <= 0)	// No potential network share directory found or invalid input.
	{
		return false;
	}

	// iDol > 0.  Must be greater than 0 because we are deleting from iDol-1.
	try
	{
		// Transform filename from
		// \\MACHINE\X$\SCOT\IMAGE\LOGO.BMP to
		//		     12345678
		// \\MACHINE\IMAGE\LOGO.BMP
		csFileName.Delete(iDol-1, 8);  // May throw CMemoryException
	}
	catch(CMemoryException *cme)
	{
		cme->Delete();
		trace(L6, _T("ERROR:  CMemoryException() thrown."));
		return false;
	}

	return true;
}

void DMProcedures::ReadDevicesModel(void)
{
	//Read Class Model 
	HKEY hKey;
	CString csWork;

	// ---------------------------------------------------------
	// Read CashAcceptor model
	// ---------------------------------------------------------
	if( ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\ADD\\CashAcceptor"),
				 0, KEY_QUERY_VALUE, &hKey))
	{
		DWORD dwValueSize=60;   //Tar 237697
		_TCHAR  szValue[61];
		if( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("DetectedDevice"), NULL, NULL, (LPBYTE)szValue, &dwValueSize))
		{
           csWork = szValue;
		   csWork.MakeUpper();
		   trace(L6, _T("ReadDevicesModel(). The model of Cash Acceptor in ADD registry is %s."), csWork);
		   if (csWork.Find(_T("CASHFLOW")) != -1)
		   {
			   m_csCashAcceptorModel = _CASHACCEPTOR_CASHFLOW;
		   }
		   else if(csWork.Find(_T("ARDAC")) != -1)

		   {
			   m_csCashAcceptorModel = _CASHACCEPTOR_ARDAC;
		   }
		   else if(csWork.Find(_T("ZT1000")) != -1)
		   {
			   m_csCashAcceptorModel = _CASHACCEPTOR_ZT1000;
		   }
		   else
		   {
				trace(L6, _T("ReadDevicesModel() did not find a known model of CashAcceptor."));
		   }
		}
	}
	else
	{
		trace(L6, _T("ReadDevicesModel() did not find the ADD registry key of CashAcceptor model."));
	}
	::RegCloseKey(hKey);


	// ---------------------------------------------------------
	// Read Coin Dispenser model
	// ---------------------------------------------------------
	if( ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\ADD\\CoinDispenser"),
				 0, KEY_QUERY_VALUE, &hKey))
	{
		DWORD dwValueSize=60;   //Tar 237697
		_TCHAR  szValue[61];
		if( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("DetectedDevice"), NULL, NULL, (LPBYTE)szValue, &dwValueSize))
		{
           csWork = szValue;
		   csWork.MakeUpper();
		   trace(L6, _T("ReadDevicesModel(). The model of Coin Dispenser in ADD registry is %s."), csWork);
		   if (csWork.Find(_T("TELEQUIPHOPPER")) != -1)
		   {
			   m_csCoinDispenserModel = _COINDISPENSER_TELEQUIP_HOPPER;
		   }
		   else if ( csWork.Find(_T("BCR")) != -1)	//SR742
		   {
			   m_csCoinDispenserModel = _COINRECYCLER_BCR;
		   }
		   else 
		   {
			   m_csCoinDispenserModel = _COINDISPENSER_TELEQUIP;
		   }

		}
	}
	else
	{
		trace(L6, _T("ReadDevicesModel() did not find the ADD registry key of CoinDispenser model."));
	}
	::RegCloseKey(hKey);
	// ---------------------------------------------------------
	// Read Bag Scale model
	// ---------------------------------------------------------
	if( ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\ADD\\BagScale"),
				 0, KEY_QUERY_VALUE, &hKey))
	{
		DWORD dwValueSize=60;   //Tar 237697
		_TCHAR  szValue[61];
		if( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("DetectedDevice"), NULL, NULL, (LPBYTE)szValue, &dwValueSize))
		{
           csWork = szValue;
		   csWork.MakeUpper();
		   trace(L6, _T("ReadDevicesModel(). The model of BagScale in ADD registry is %s."), csWork);

		   if (csWork.Find(_T("FLINTEC")) != -1)
		   {
			   m_csBagScaleModel = _BAGSCALE_FLINTEC;
		   }
		   else if(csWork.Find(_T("TEDEAHUNTLEIGH")) != -1)

		   {
			   m_csBagScaleModel = _BAGSCALE_TEDEAHUNTLEIGH;
		   }
		   else
		   {
				trace(L6, _T("ReadDevicesModel() did not find a known model of BagScale."));
		   }

		}
	}
	else
	{
		trace(L6, _T("ReadDevicesModel() did not find the ADD registry key of BagScale model."));
	}

	::RegCloseKey(hKey);
//+dp185016 tar 277590
	// ---------------------------------------------------------
	// Read Cash Dispenser Model
	// ---------------------------------------------------------
	if( ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\ADD\\BillDispenser"),
				 0, KEY_QUERY_VALUE, &hKey))
	{
		DWORD dwValueSize=60;   //Tar 237697
		_TCHAR  szValue[61];
		if( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("DetectedDevice"), NULL, NULL, (LPBYTE)szValue, &dwValueSize))
		{
           csWork = szValue;
		   csWork.MakeUpper();
		   trace(L6, _T("ReadDevicesModel(). The model of CashDispenser in ADD registry is %s."), csWork);
			
		   if (csWork.Find(_T("BNR")) != -1)
		   {
			   m_csCashDispenserModel = _CASHDISPENSER_BNR;
		   }
		   else if (csWork.Find(_T("5635")) != -1)
		   {
			   m_csCashDispenserModel = _CASHDISPENSER_5635;
		   }
		   else if(csWork.Find(_T("5633")) != -1)

		   {
			   m_csCashDispenserModel = _CASHDISPENSER_5633;
		   }
		   else
		   {
				trace(L6, _T("ReadDevicesModel() did not find a known model of CashDispenser in registry \\SOFTWARE\\NCR\\ADD\\BillDispenser"));
		   }

		}
	}
  //+dp185016 support glory device
	if( ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\ADD\\Recycler"),
		0, KEY_QUERY_VALUE, &hKey))
	{
		DWORD dwValueSize=60;   
		_TCHAR  szValue[61];
		if( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("DetectedDevice"), NULL, NULL, (LPBYTE)szValue, &dwValueSize))
		{
			csWork = szValue;
			csWork.MakeUpper();
			trace(L6, _T("ReadDevicesModel(). The model of CashRecycler in ADD registry is %s."), csWork);
			
			if(csWork.Find(_T("GLORY")) != -1)
			{
				m_csCashRecyclerModel = _CASHDISPENSER_GLORY;
			}
			else if(csWork.Find(_T("SAFEPAY")) != -1)
			{
				m_csCashRecyclerModel = _CASHDISPENSER_SAFEPAY;
			}
			else
			{
				trace(L6, _T("ReadDevicesModel() did not find a known model of CashRecycler."));
			}
			
		}
	}
  //-dp185016 support glory device
	else
	{
		trace(L6, _T("ReadDevicesModel() did not find the ADD registry key of CashRecycler model."));
	}

	::RegCloseKey(hKey);
  
//-dp185016 277590
	// ---------------------------------------------------------
	// Determine Coin Dispenser Model (Release)
	// ---------------------------------------------------------
	if( ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\Installation\\platform"),
				 0, KEY_QUERY_VALUE, &hKey))
	{
		DWORD dwValueSize=10;
		_TCHAR  szValue[11];
		// Get the configuration value
		if( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("Configure"), NULL, NULL, (LPBYTE)szValue, &dwValueSize))
		{
		   csWork = szValue;
		   trace(L6, _T("ReadDevicesModel() detected platform %s."), csWork);
		}
	   if(csWork.Find(_T("SCOTE")) != -1)
	   {
		   m_csCoinAcceptorRelease = _COINACCEPTOR_2x;
		   m_csPinpadRelease = _PINPAD_2X; //tar 310198
	   }
	   else
	   {
		   m_csCoinAcceptorRelease = _COINACCEPTOR_3x;
		   m_csPinpadRelease = _PINPAD_3X4X; //tar 310198
	   }
	}
	::RegCloseKey(hKey);
	return ;
}

bool DMProcedures::IsDeviceAvailable( 
   long nDeviceClass )
   {
   CString csWork = m_devmgr.GetDeviceAvailability( nDeviceClass );
   return csWork.Find( DM_DA_LOADED ) != -1;
   }

//dp185016 expose CCashRecyclerWrapper
CCashRecyclerWrapper* DMProcedures::GetCashRecycler()
{
   trace(L6,_T("+GetCashRecycler"));
   _TCHAR msg[] = T_ID _T("::GetCashRecycler");
   CCashRecyclerWrapper* pDevice = NULL;
   // TAR 309097 - begin
   // If device manager is closed, then return NULL pointer.
   if(m_bDMOpened == false)
   {
	   return NULL;
   }
   // TAR 309097 - end
  try
  {
	  pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
  }
  PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);
  trace(L6,_T("-GetCashRecycler"));
  return pDevice;
}

#define CHAN_DI_STATUSREAD 10
///////////////////////////////////////
long DMProcedures::CashRecyclerGetErrorCode(long pData)
{
	trace(L6,_T("+CashRecyclerGetErrorCode"));

	CCashRecyclerWrapper* pDevice = NULL;
	CString csErrorCode = EMPTY_STRING;
	BSTR pString; 
	long lErrorCode = 0; 
	long lCommand;
	_TCHAR msg[] = T_ID _T("::CashRecyclerGetErrorCode");

	ASSERT( ! m_csCashRecyclerModel.IsEmpty() );

	if(m_csCashRecyclerModel == _CASHDISPENSER_GLORY)
	{
		lCommand = CHAN_DI_STATUSREAD;
	}
	else if(m_csCashRecyclerModel == _CASHDISPENSER_SAFEPAY)
	{
		pData = 0;						
		lCommand = SP_DIO_GETJAMREASON;
	}
	else		// Not a valid recycler model.  Return.
	{
		trace(L4, 
			_T("-CashRecyclerGetErrorCode> ERROR: Invalid recycler model %s"),
			m_csCashRecyclerModel);

		// TAR 353244 - Changed return to OPOS_SUCCESS from OPOS_E_ILLEGAL
		//              in case this method is called on a system without
		//              a recycler.
		return OPOS_SUCCESS;  
	}

	// If here, we have a valid recyler model.
	rc = Enable(DMCLASS_CASHRECYCLER, DEVICEID0, true);
	if ( rc == 0 )
	{
		try
		{
			pString = csErrorCode.AllocSysString(); // throws CMemoryException 
			pDevice = m_devmgr.GetCashRecycler(DEVICEID0);

			if(pDevice != NULL)
			{
				rc = pDevice->DirectIO(lCommand, &pData, &pString);
				if(rc == 0)
				{	
					csErrorCode = pString;  		// Only used by Glory device.
				}
			}

			SysFreeString(pString);
		}
		PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);
	}

	if(lCommand == CHAN_DI_STATUSREAD)		// Glory.
	{
		csErrorCode = csErrorCode.Left(4);  //get error code
		lErrorCode =  _ttol(csErrorCode);
		trace(L6,_T("-CashRecyclerGetErrorCode 0x%x, %s"),rc, csErrorCode);
	}
	else									// SafePay
	{
		if(rc == OPOS_SUCCESS)
		{
			lErrorCode = pData;
		}
		else
		{
			trace(L4, _T("ERROR:  Could not get error code.  Device is offline?"));
			lErrorCode = OPOS_SUE_POWER_OFFLINE;
		}
		trace(L6,_T("-CashRecyclerGetErrorCode 0x%x, %d"),rc, lErrorCode);
	}
	return lErrorCode;
}


///////////////////////////////////////
CString DMProcedures::CashRecyclerGetCheckHealthText(void)
   {
   trace(L6,_T("+CashRecyclerGetCheckHealthText"));
   CString csCheckHealthText=_T("");
   rc = Enable(DMCLASS_CASHRECYCLER, DEVICEID0, true);

   if ( rc == 0 )
      {
			 CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
			 csCheckHealthText = pDevice->GetCheckHealthText();
		}
   trace(L6,_T("-CashRecyclerGetCheckHealthText %s"),csCheckHealthText);
   return csCheckHealthText;
   }

///////////////////////////////////////
long DMProcedures::CashRecyclerStopDeposit(CString& csDepositCashList)
   {
   trace(L6,_T("+CashRecyclerStopDeposit"));
   long lDepositAmount = 0;
   rc = Enable(DMCLASS_CASHRECYCLER, DEVICEID0, true);

   if ( rc == 0 )
      {
			 CCashRecyclerWrapper* pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
			 
			 rc = pDevice->EndDeposit(CHAN_DEPOSIT_NOCHANGE);
			 
			 if(rc==0)
			 {
				 lDepositAmount = pDevice->GetDepositAmount();
				 csDepositCashList = pDevice->GetDepositCounts();
			 }
		}
   trace(L6,_T("-CashRecyclerStopDeposit %d %s"),lDepositAmount, csDepositCashList);
   return lDepositAmount;
   }

void DMProcedures::CashRecyclerResetCounts(void)
{
	if(m_csCashRecyclerModel.IsEmpty())
	{
		return;
	}

	CCashRecyclerWrapper* pDevice = GetCashRecycler();
	pDevice->ResetCounts();
}

void DMProcedures::CashRecyclerUpdateDispensed(long nDisp)
{
	if(m_csCashRecyclerModel.IsEmpty())
	{
		return;
	}

	CCashRecyclerWrapper* pDevice = GetCashRecycler();
	pDevice->UpdateDispensed(nDisp);
}

void DMProcedures::CashRecyclerPauseCounts(bool bPause)
{
	if(m_csCashRecyclerModel.IsEmpty())
	{
		return;
	}

	CCashRecyclerWrapper* pDevice = GetCashRecycler();
	pDevice->PauseCounts(bPause);
}

/**
 * CashRecyclerGetEnabled()
 *
 * Returns TRUE if the recycler is accepting currency.  FALSE otherwise.
 */
BOOL DMProcedures::CashRecyclerGetEnabled()
{
   _TCHAR msg[] = T_ID _T("::CashRecyclerGetEnabled");
   CCashRecyclerWrapper* pDevice;
   bool bDepositState;

   if(m_csCashRecyclerModel.IsEmpty())
   {
		return FALSE;
   }

  try
  {
	  pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
	  bDepositState = pDevice->GetDepositState();		// TAR 313343

	  trace(L6, _T("+CashRecyclerGetEnabled; bDepositState: %d"), bDepositState);

	  if(bDepositState==true)
	  {
		  return TRUE;
	  }

	  return FALSE;
  }
  PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);

  return FALSE;
}

/**
 * Function:		CashRecyclerDispenseRetry
 * Purpose:			Returns true if the caller should attempt another dispense.
 *					false otherwise.
 * Parameters:		rc - The error code from the recycler to evaluate.
 * Returns:			true if the error code indicates that nothing was dispensed
 *					and that another dispense should be attempted.  false otherwise.
 * Calls:			
 * Side Effects:
 * Notes:			Created for TAR 310660		
 */
bool DMProcedures::CashRecyclerDispenseRetry(long rc)
{
	switch(rc)
	{
		case OPOS_ECHAN_BUSY:
		case OPOS_ECHAN_IMPOSSIBLE:
		case OPOS_ECHAN_IFERROR:
		case OPOS_ECHAN_CASSETTEWAIT:
			return true;
	}

	return false;
}

/**
 * Function:    CashRecyclerGetCurrencyCashList
 * Purpose:     Obtain the CurrencyCashList property (supported currency units)
 *              from the cash recycler.
 * Parameters:  csList - Reference to a CString that will receive the list 
 *              result.
 * Returns:     true if successful, false otherwise.
 * Calls:           
 * Side Effects:
 * Notes:       Used in SafePay cash management.        
 */
bool DMProcedures::CashRecyclerGetCurrencyCashList(CString &csList)
{
    _TCHAR msg[] = T_ID _T("::CashRecyclerGetCurrencyCashList");

    CCashRecyclerWrapper *pDevice = NULL;   
    try
    {
        pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
        
        if(pDevice == NULL)
        {
            return false;
        }
        
        csList = pDevice->GetCurrencyCashList();
        trace(L6, _T("CashRecyclerGetCurrencyCashList>  %s"), (LPCTSTR)csList);
        return true;
    }
    PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);
    return false;
}

/**
 * Function:    CashRecyclerGetDepositCounts
 * Purpose:     Obtain the DepositAmount property from the cash recycler.
 * Parameters:  csList - Reference to a CString that will receive the count list 
 *              result.
 * Returns:     true if successful, false otherwise.
 * Calls:           
 * Side Effects:
 * Notes:       This is required for SafePay cash management. ReadCashCounts cannot
 *              be used to obtain unfixed counts.
 */
bool DMProcedures::CashRecyclerGetDepositCounts(CString &csList)
{
    _TCHAR msg[] = T_ID _T("::CashRecyclerGetDepositCounts");

    CCashRecyclerWrapper *pDevice = NULL;   
    try
    {
        pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
        
        if(pDevice == NULL)
        {
            return false;
        }
        
        csList = pDevice->GetDepositCounts();       
        trace(L6, _T("CashRecyclerGetDepositCounts> %s"), (LPCTSTR)csList);
        return true;
    }
    PROCESS_EXCEPTIONS(DMCLASS_CASHRECYCLER);

    return false;
}

/**
 * Function:        UnlockNoteCover
 * Purpose:         Unlock SafePay note cover.  This can only be done through
 *                  software.
 * Parameters:      nDeviceClass - Class of the device 
 *                      (should be DMCLASS_CASHRECYCLER)
 *                  nDeviceId    - Device ID (0).
 * Returns:         OPOS_SUCCESS if successful.  SO error code otherwise.
 * Calls:           
 * Side Effects:
 * Notes:           Wrapper for CCashRecyclerWrapper::UnlockNoteCover.      
 */
long DMProcedures::UnlockNoteCover(const long nDeviceClass, long nDeviceId)
{
    trace(L6,_T("+UnlockNoteCover> nDeviceClass: %d; nDeviceId: %d"),
          nDeviceClass, nDeviceId);

    _TCHAR msg[] = T_ID _T("::UnlockNoteCover");

    long rc = OPOS_E_FAILURE;
    CCashRecyclerWrapper *pDevice = NULL;

    ASSERT(nDeviceClass == DMCLASS_CASHRECYCLER);

    try
    {
        pDevice = m_devmgr.GetCashRecycler( DEVICEID0 );
        if(pDevice == NULL)
        {
            trace(L6,_T("-UnlockNoteCover> ERROR: NULL device pointer returned"));
            return OPOS_E_FAILURE;
        }

        // The note cover cannot be unlatched if in a deposit cycle.
        pDevice->EndDeposit(CHAN_DEPOSIT_NOCHANGE);
        rc = pDevice->UnlockNoteCover();                // Unlock cover.        
    }
    PROCESS_EXCEPTIONS(nDeviceClass);

    trace(L6,_T("-UnlockNoteCover> rc: %d"), rc);
    return rc;
}

/**
 * Function:        IsUrgentMessagePending
 * Purpose:         Indicates whether there are pending SafePay urgent messages
 *                  to be displayed.
 * Parameters:      nDeviceClass - Class of the device 
 *                      (should be DMCLASS_CASHRECYCLER)
 *                  nDeviceId    - Device ID (0).
 * Returns:         true if a message is available, false otherwise.
 * Calls:           
 * Side Effects:
 * Notes:           Wrapper for CCashRecyclerWrapper::IsUrgentMessagePending.       
 */
bool DMProcedures::IsUrgentMessagePending(const long nDeviceClass, long nDeviceId)
{
    _TCHAR msg[] = T_ID _T("::IsUrgentMessagePending");
    CCashRecyclerWrapper *pDevice = NULL;

    ASSERT(nDeviceClass == DMCLASS_CASHRECYCLER);
    try
    {
        pDevice = m_devmgr.GetCashRecycler( nDeviceId );
        if(pDevice == NULL)
        {
            return false;
        }

        return pDevice->IsUrgentMessagePending();
    }
    PROCESS_EXCEPTIONS(nDeviceClass);

    return false;
}

/**
 * Function:        RetrieveUrgentMessage
 * Purpose:         Obtain the SafePay error message (if any) from the device SO.
 * Parameters:      cmsg (out) - Receives the ugent message.
 *                  nDeviceClass - Class of the device 
 *                      (should be DMCLASS_CASHRECYCLER)
 *                  nDeviceId    - Device ID (0).
 * Returns:         OPOS_SUCCESS if successful.  
 * Calls:           
 * Side Effects:
 * Notes:           Wrapper for CCashRecyclerWrapper::RetrieveUrgentMessage.        
 */
long DMProcedures::RetrieveUrgentMessage(CString &cmsg, const long nDeviceClass, 
                                         long nDeviceId)
{
    _TCHAR msg[] = T_ID _T("::RetrieveUrgentMessage");
    CCashRecyclerWrapper *pDevice = NULL;

    ASSERT(nDeviceClass == DMCLASS_CASHRECYCLER);

    try
    {
        pDevice = m_devmgr.GetCashRecycler( nDeviceId );
        if(pDevice == NULL)
        {
            return OPOS_E_FAILURE;
        }
        
        return pDevice->RetrieveUrgentMessage(cmsg);
    }
    PROCESS_EXCEPTIONS(nDeviceClass);
    
    return OPOS_E_FAILURE;
}

/**
 * Function:        ReadStatusBits
 * Purpose:         Retrieves SafePay error status bits from device.  Different bits
 *                  indicate different errors (see SP_Status class in SafePayAPI.h)
 * Parameters:      pData (out) - Receives the status bit values.
 *                  nDeviceClass - Class of the device 
 *                      (should be DMCLASS_CASHRECYCLER)
 *                  nDeviceId    - Device ID (0).
 * Returns:         true if successful.
 * Calls:           
 * Side Effects:
 * Notes:           Only valid for the SafePay cash recycler.
 *                  Wrapper for CCashRecyclerWrapper::ReadStatusBits
 */
bool DMProcedures::ReadStatusBits(long &pData, const long nDeviceClass, 
                                  long nDeviceId)
{
    _TCHAR msg[] = T_ID _T("::ReadStatusBits");
    CCashRecyclerWrapper *pDevice = NULL;

    ASSERT(nDeviceClass == DMCLASS_CASHRECYCLER);
    try
    {
        pDevice = m_devmgr.GetCashRecycler( nDeviceId );
        if(pDevice == NULL)
        {
            return false;
        }

        return pDevice->ReadStatusBits(pData);
    }
    PROCESS_EXCEPTIONS(nDeviceClass);

    return false;
}

/**
 * Function:        IsPauseCountsActive
 * Purpose:         Return the counting state of the recycler.  Internal counts are
 *                  "paused" while in cash management.  Currency inserted in the cash
 *                  management state is not added to inserted currency amount.
 * Parameters:      
 *                  nDeviceClass - Class of the device 
 *                      (should be DMCLASS_CASHRECYCLER)
 *                  nDeviceId    - Device ID (0).
 * Returns:         true if counts are paused. false if counting is resumed.
 * Calls:           
 * Side Effects:
 * Notes:           Used to test whether in cash management or not.  The current 
 *                  state is not an indicator of this.  If there is an error in 
 *                  cash management, then the state name will be different.
 *                  This method is a wrapper for CCashRecycler::PauseCounts(void).
 */
bool DMProcedures::IsPauseCountsActive(const long nDeviceClass, long nDeviceId)
{
    _TCHAR msg[] = T_ID _T("::IsPauseCountsActive");
    CCashRecyclerWrapper *pDevice = NULL;

    if(dm.m_csCashRecyclerModel.IsEmpty())
    {
        return false;   // This method only valid for cash recyclers.
    }

    try
    {
        pDevice = m_devmgr.GetCashRecycler( nDeviceId );
        if(pDevice == NULL)
        {
            return false;
        }

        return pDevice->PauseCounts();
    }
    PROCESS_EXCEPTIONS(nDeviceClass);
    return false;
}

/**
 * Function:        SetDeviceEnabledRaw
 * Purpose:         dm wrapper for CCashRecyclerWrapper::SetDeviceEnabledRaw
 * Parameters:      bEnable - If TRUE, enable the device, If FALSE, disable.
 *                  nDeviceClass - Class of the device 
 *                      (should be DMCLASS_CASHRECYCLER)
 *                  nDeviceId    - Device ID (0).
 * Returns:         true if successful.  false otherwise..
 * Calls:           
 * Side Effects:	Device is enabled / disabled.
 * Notes:           Created for TAR 356035.
 */
bool DMProcedures::SetDeviceEnabledRaw(BOOL bEnable, const long nDeviceClass, 
                                       long nDeviceId)
{
    _TCHAR msg[] = T_ID _T("::SetDeviceEnabledRaw");
    CCashRecyclerWrapper *pDevice = NULL;

    ASSERT(nDeviceClass == DMCLASS_CASHRECYCLER);
    try
    {
        pDevice = m_devmgr.GetCashRecycler( nDeviceId );
        if(pDevice == NULL)
        {
            return false;
        }

		return pDevice->SetDeviceEnabledRaw(bEnable);
    }
    PROCESS_EXCEPTIONS(nDeviceClass);

	return false;
}



//PW500001 Rel4 Patch C
long DMProcedures::GetCoinAcceptorCoinList(CString* pcsCoinCounts)
    {
       trace(L6,_T("+GetCoinAcceptorCoinList"));
       _TCHAR msg[] = T_ID _T("::GetCoinAcceptorCoinList");
	    
		rc = Enable(DMCLASS_COINACCEPTOR, DEVICEID0, true);
       if ( rc == 0 )
          {
          try
             {
             *pcsCoinCounts = m_devmgr.GetCoinAcceptorCoinList(DEVICEID0);
             }
          PROCESS_EXCEPTIONS(DMCLASS_CASHACCEPTOR);
          }
       trace(L6,_T("+GetCoinAcceptorCoinList 0x%x"),rc);
       return rc;
    }

//PW500001 Rel4 Patch C
long DMProcedures::GetCashAcceptorCurrencyCashList(CString* pcsCashCounts)
    {
       trace(L6,_T("+GetCashAcceptorCurrencyCashList"));
       _TCHAR msg[] = T_ID _T("::GetCashAcceptorCurrencyCashList");
	    
       rc = Enable(DMCLASS_CASHACCEPTOR, DEVICEID0, true);
       if ( rc == 0 )
          {
          try
             {
             *pcsCashCounts = m_devmgr.GetCashAcceptorCurrencyCashList(DEVICEID0);
             }
          PROCESS_EXCEPTIONS(DMCLASS_CASHACCEPTOR);
          }
       trace(L6,_T("+GetCashAcceptorCurrencyCashList 0x%x"),rc);
       return rc;
    }


long DMProcedures::CloseDevice( 
	long nDeviceClass, long nDeviceId  )
	{
    trace(L6,_T("+CloseDevice"));
    _TCHAR msg[] = T_ID _T("::CloseDevice");
	try
	{
		CGenericOposIF* pDevice = m_devmgr.GetDevice( nDeviceClass, nDeviceId );
      rc = pDevice->CloseDevice();
      CDeviceManagerEx::ThrowDmException( pDevice );
	}
	PROCESS_EXCEPTIONS(nDeviceClass);
   return rc;
	}

long DMProcedures::OpenDevice( 
	long nDeviceClass, long nDeviceId  )
	{
   trace(L6,_T("+OpenDevice"));
    _TCHAR msg[] = T_ID _T("::OpenDevice");
	try
	{
		CGenericOposIF* pDevice = m_devmgr.GetDevice( nDeviceClass, nDeviceId );
      rc = pDevice->OpenDevice();
      CDeviceManagerEx::ThrowDmException( pDevice );
	}
	PROCESS_EXCEPTIONS(nDeviceClass);
   return rc;
	}


long DMProcedures::ReadFirmwareVersion( 
   long     nDeviceClass, 
   long     nDeviceId, 
   CString& sVersion, 
   long*    lVersion )
   {
   trace(L6,_T("+ReadFirmwareVersion"));
    _TCHAR msg[] = T_ID _T("::ReadFirmwareVersion");
	try
	{
		CGenericOposIF* pDevice = m_devmgr.GetDevice( nDeviceClass, nDeviceId );
      rc = pDevice->ReadFirmwareVersion( sVersion, lVersion );
      CDeviceManagerEx::ThrowDmException( pDevice );
	}
	PROCESS_EXCEPTIONS(nDeviceClass);
   return rc;
   }

long DMProcedures::DirectIO( 
   long     nDeviceClass, 
   long     nDeviceId, 
   long     nCommand, 
   long*    plData, 
   CString& sData, 
   long*    pnRcEx )
   {
   trace(L6,_T("+DirectIO"));
    _TCHAR msg[] = T_ID _T("::DirectIO");
	try
	{
		CGenericOposIF* pDevice = m_devmgr.GetDevice( nDeviceClass, nDeviceId );
      BSTR bStrData = sData.AllocSysString( );
      rc = pDevice->DirectIO( nCommand, plData, &bStrData );
      *pnRcEx = pDevice->GetResultCodeExtended();
      sData = bStrData;
      ::SysFreeString( bStrData );
      bStrData = NULL;
      CDeviceManagerEx::ThrowDmException( pDevice );
	}
	PROCESS_EXCEPTIONS(nDeviceClass);
   return rc;
   }

///////////////////////////////////////
long DMProcedures::MSRGetCardPosition(void)
   {
   trace(L6,_T("+MSRGetCardPosition"));
   _TCHAR msg[] = T_ID _T("::MSRGetCardPosition");
    CString csMSRcaptureInfo = EMPTY_STRING;
   long pData = 0;
   BSTR strValue = NULL;
      try
         {
         CMSRWrapper* pMotorizedMSR = m_devmgr.GetMSR( DEVICEID0 );
         rc = pMotorizedMSR->DirectIO(NCR_IOM_POSITION, &pData, &strValue);

		if(rc  == OPOS_SUCCESS)
		{
		  switch(pData)
		  {
			//No card or card remove from gate
			case MCR_CARD_POSITION_NO_CARD_REMOVE:
			break;
			//Card at gate
			case MCR_CARD_POSITION_AT_GATE:
			break;
		   //Card inside the transport
		   case MCR_CARD_POSITION_AT_MCRW:
		   break;
		 }
         }
	  }
      PROCESS_EXCEPTIONS(DMCLASS_MSR);
     

   trace(L6,_T("-MSRGetCardPosition 0x%x, %d"),rc, pData);
   return pData;
   }
////////////////////////////
// +RFC 367092	 Function for to expose printer handle to SSF.
///////////////////////////
CPrintStation*  DMProcedures::GetPrintStation(long nDeviceClass) 
{
trace(L6,_T("+DMProcedures::GetPrintStation"));
   _TCHAR msg[] = T_ID _T("::GetPrintStation");
   CPrintStation*  pDevice = NULL;

   // If device manager is closed, then return NULL pointer.
   if(m_bDMOpened == false)
   {
	   return NULL;
   }
  try
  {
	  pDevice = m_devmgr.GetPrintStation( nDeviceClass);
  }
  PROCESS_EXCEPTIONS(nDeviceClass);
  trace(L6,_T("-DMProcedures::GetPrintStation"));
  return pDevice;
}

//Begin RFC 367092
long DMProcedures::PrinterGetBinaryConversion(long nDeviceClass)
{
	CPrintStation *pDevice= NULL;
	long lBinaryConversion = 0;
	pDevice = GetPrintStation(nDeviceClass);
	if (pDevice)
		lBinaryConversion = pDevice->m_rPtr.GetBinaryConversion();	
	return lBinaryConversion;
}
void DMProcedures::PrinterSetBinaryConversion(long nDeviceClass, long lBinaryVal)
{
	CPrintStation *pDevice= NULL;
	pDevice = GetPrintStation(nDeviceClass);
	if (pDevice)
		pDevice->m_rPtr.SetBinaryConversion(lBinaryVal);

}
//End RFC 367092

// -RFC 367092

CScaleWrapper* DMProcedures::GetScaleWrapper(long nDeviceId   /*=0 */)
{
	trace(L6,_T("+GetScale "));
	_TCHAR msg[] = T_ID _T("::GetScale ");
	CScaleWrapper* pDevice = NULL;
	
	// If device manager is closed, then return NULL pointer.
	if(m_bDMOpened == false)
	{
		return NULL;
	}
	try
	{
		pDevice = m_devmgr.GetScaleWrapper(nDeviceId);
	}
	PROCESS_EXCEPTIONS(DMCLASS_SCALE);
	trace(L6,_T("-GetScale "));
	return pDevice;
}

//SR742+
void DMProcedures::SetDegradedModeType(long lDegradedMode)
{
	m_lDegradedModeType = lDegradedMode;
}
//SR742-

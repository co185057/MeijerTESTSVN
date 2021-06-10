//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	 SMScanReceipt.CPP
//
// TITLE:	Class implementation for ScanReceipt State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"                 // MS MFC always needed first

#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMScanReceipt.h"
#include "SMAttract.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SMScanReceipt")

#define SCAN_RECEIPT_TIMEOUT 60

IMPLEMENT_DYNAMIC(SMScanReceipt, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP


//*************************************************************/
//*************************************************************/
SMScanReceipt::SMScanReceipt()
{
  IMPLEMENT_TIMESTAMP
}

//*************************************************************/
SMStateBase *SMScanReceipt::Initialize(void)
{
	// Tell security that we are entering ScanReceipt state
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ScanReceipt") );
	nTimeOut = SCAN_RECEIPT_TIMEOUT;
//	m_bResumeTrxStarted = false;
	m_bIsFromScanReceipt = true;
    ps.SetCMFrame(_T("ScanReceipt")); 
	ps.Message12(SSF_TXT_SCAN_RECEIPT, true);
	ps.ShowCMFrame();
    DMSayPhrase(_T("ScanYourReceipt"));
	DMScannerEnable();
    return STATE_NULL;
}

//*************************************************************/
SMStateBase *SMScanReceipt::DMScanner()
{
	  CString csBarcodeType;
	  CString csBarcode = csDMLastBarCode;
	  TrimSymbology(csBarcode);
	  trace(L6, _T("SMScanReceipt::DMScanner(), csBarcode: %s"), csBarcode);

	  if ( csBarcode.GetLength() == 12 )
	  {
		  csBarcodeType = csBarcode.Left(2);
	  }
	  else if( csBarcode.GetLength() == 13 )
	  {
		csBarcodeType = csBarcode.Left(3);
	  }
	  else if ( csBarcode.GetLength() == 14 )
	  {
		  csBarcodeType = csBarcode.Left(4);
		  if ( csBarcodeType != _T("0047") )
		  {
			  csBarcodeType = csBarcode.Left(5);
		  }
	  }

	  trace(L6, _T("SMScanReceipt::DMScanner(), csBarcodePrefix: %s, length: %d"), csBarcodeType, csBarcode.GetLength());

	  if ( csBarcodeType == _T("47") || csBarcodeType == _T("047") ||
		  csBarcodeType == _T("0047") || csBarcodeType == _T("00047"))
	  {
		//m_bResumeTrxStarted = true;
		m_bIsFromScanReceipt = false;
		csItemSent = csDMLastBarCode;
		return TBItemSale(false,0,true);
	  }
	  else
	  {
		  ps.Message12(SSF_TXT_INVALID_RECEIPT, true);
		  DMScannerEnable();
		  return STATE_NULL;
	  }  
}

//*************************************************************/
SMStateBase *SMScanReceipt::PSButtonGoBack(void)
{
	TBRC rc;
	fSAInTransaction = false;
	fInSignedOn = false;	
	fInSignedOff = true;
    rc = ::TBFinish();
	//rc=tb.Finish();
	return STATE_NULL;
} 

//*************************************************************/
void SMScanReceipt::UnInitialize(void)
{
	// Tell security that we are exiting form ScanReceipt state
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanReceipt") );
}

SMStateBase  *SMScanReceipt::TimedOut(void)
{
	nTimeOut = 0;
	return PSButtonGoBack();
}
// NCRTBDeviceSO.cpp : implementation file
//

#include "stdafx.h"

#include "NCRTBDevice.h"
#include "NCRTBDeviceSO.h"
#include "TBService.h"
#include "TBDevice.h"
#include "LineDisplay.h"
#include "CashDrawer.h"
#include "POSPrinter.h"
#include "Scale.h"
#include "FiscalPrinter.h"
#include "KeyLock.h"
#include "ToneIndicator.h"
#include "MICR.h"
#include "POSKeyBoard1.h"
//#include <sys/timeb.h>
//#include <time.h>
//#define TM_MODULE TM_MODULE_4
//#define T_ID "NCRTBDevice"
//#include "TraceSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


TOposDeviceMap<CTBDevice>   g_TBDevices;


/////////////////////////////////////////////////////////////////////////////
// CNCRTBDeviceSO

IMPLEMENT_DYNCREATE(CNCRTBDeviceSO, CCmdTarget)

// Note: we add support for IID_INCRTBDeviceSO to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.
static const IID IID_INCRTBDeviceSO =
{ 0x2b5e45bf, 0x37e8, 0x11d5, { 0x9c, 0x83, 0x0, 0x90, 0x27, 0x6f, 0xd2, 0x8d } };
//const IID guid = IID_INCRTBDeviceSO;


CNCRTBDeviceSO::CNCRTBDeviceSO()
: m_oService( _T("CNCRTBDeviceSO"), g_TBDevices, guid),
   m_bReallyClaimed( FALSE )

{
	EnableAutomation();
	
	// To keep the application running as long as an OLE automation 
	//	object is active, the constructor calls AfxOleLockApp.
	
	AfxOleLockApp();
}

CNCRTBDeviceSO::~CNCRTBDeviceSO()
{
	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.
	
	AfxOleUnlockApp();
}


void CNCRTBDeviceSO::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CNCRTBDeviceSO, CCmdTarget)
	//{{AFX_MSG_MAP(CNCRTBDeviceSO)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CNCRTBDeviceSO, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CNCRTBDeviceSO)
	DISP_FUNCTION(CNCRTBDeviceSO, "COFreezeEvents", COFreezeEvents, VT_I4, VTS_BOOL)
	DISP_FUNCTION(CNCRTBDeviceSO, "CheckHealth", CheckHealth, VT_I4, VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "Claim", Claim, VT_I4, VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "Close", Close, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "DirectIO", DirectIO, VT_I4, VTS_I4 VTS_PI4 VTS_PBSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "GetPropertyNumber", GetPropertyNumber, VT_I4, VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "GetPropertyString", GetPropertyString, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "OpenService", OpenService, VT_I4, VTS_BSTR VTS_BSTR VTS_DISPATCH)
	DISP_FUNCTION(CNCRTBDeviceSO, "Release", Release, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "SetPropertyNumber", SetPropertyNumber, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "SetPropertyString", SetPropertyString, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "ClearOutput", ClearOutput, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "ClearInput", ClearInput, VT_I4, VTS_NONE)
//line display
	DISP_FUNCTION(CNCRTBDeviceSO, "ClearDescriptors", ClearDescriptors, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "ClearText", ClearText, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "CreateWindow", CreateWindow, VT_I4, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "DestroyWindow", DestroyWindow, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "DisplayText", DisplayText, VT_I4, VTS_BSTR VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "DisplayTextAt", DisplayTextAt, VT_I4, VTS_I4 VTS_I4 VTS_BSTR VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "ScrollText", ScrollText, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "SetDescriptor", SetDescriptor, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "RefreshWindow", RefreshWindow, VT_I4, VTS_I4)
// cash drawer
	DISP_FUNCTION(CNCRTBDeviceSO, "OpenDrawer", OpenDrawer, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "WaitForDrawerClose", WaitForDrawerClose, VT_I4, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
// POS Printer or MICR
    DISP_FUNCTION(CNCRTBDeviceSO, "BeginInsertion", BeginInsertion, VT_I4, VTS_I4)
    DISP_FUNCTION(CNCRTBDeviceSO, "BeginRemoval", BeginRemoval, VT_I4, VTS_I4)
    DISP_FUNCTION(CNCRTBDeviceSO, "EndInsertion", EndInsertion, VT_I4, VTS_NONE)
    DISP_FUNCTION(CNCRTBDeviceSO, "EndRemoval", EndRemoval, VT_I4, VTS_NONE)
// POS Printer
    DISP_FUNCTION(CNCRTBDeviceSO, "CutPaper", CutPaper, VT_I4, VTS_I4)
    DISP_FUNCTION(CNCRTBDeviceSO, "PrintBarCode", PrintBarCode, VT_I4, VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
    DISP_FUNCTION(CNCRTBDeviceSO, "PrintBitmap", PrintBitmap, VT_I4, VTS_I4 VTS_BSTR VTS_I4 VTS_I4)
    DISP_FUNCTION(CNCRTBDeviceSO, "PrintImmediate", PrintImmediate, VT_I4, VTS_I4 VTS_BSTR)
    DISP_FUNCTION(CNCRTBDeviceSO, "PrintNormal", PrintNormal, VT_I4, VTS_I4 VTS_BSTR)
    DISP_FUNCTION(CNCRTBDeviceSO, "PrintTwoNormal", PrintTwoNormal, VT_I4, VTS_I4 VTS_BSTR VTS_BSTR)
    DISP_FUNCTION(CNCRTBDeviceSO, "RotatePrint", RotatePrint, VT_I4, VTS_I4 VTS_I4)
    DISP_FUNCTION(CNCRTBDeviceSO, "SetBitmap", SetBitmap, VT_I4, VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_I4)
    DISP_FUNCTION(CNCRTBDeviceSO, "SetLogo", SetLogo, VT_I4, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "TransactionPrint", TransactionPrint, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "ValidateData", ValidateData, VT_I4, VTS_I4 VTS_BSTR)
// Scale
	DISP_FUNCTION(CNCRTBDeviceSO, "ReadWeight", ReadWeight, VT_I4, VTS_PI4 VTS_I4)
// Fiscal Printer
	DISP_PROPERTY_EX(CNCRTBDeviceSO, "ErrorLevel", GetErrorLevel, SetErrorLevel, VT_I4)
	DISP_PROPERTY_EX(CNCRTBDeviceSO, "ErrorOutID", GetErrorOutID, SetErrorOutID, VT_I4)
	DISP_PROPERTY_EX(CNCRTBDeviceSO, "ErrorState", GetErrorState, SetErrorState, VT_I4)
	DISP_PROPERTY_EX(CNCRTBDeviceSO, "ErrorString", GetErrorString, SetErrorString, VT_BSTR)
	DISP_PROPERTY_EX(CNCRTBDeviceSO, "FlagWhenIdle", GetFlagWhenIdle, SetFlagWhenIdle, VT_BOOL)
	DISP_PROPERTY_EX(CNCRTBDeviceSO, "ErrorStation", GetErrorStation, SetErrorStation, VT_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "BeginFiscalDocument", BeginFiscalDocument, VT_I4, VTS_I4)
	//DISP_FUNCTION(CNCRTBDeviceSO, "BeginInsertion", BeginInsertion, VT_I4, VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "BeginFiscalReceipt", BeginFiscalReceipt, VT_I4, VTS_BOOL)
	DISP_FUNCTION(CNCRTBDeviceSO, "BeginFixedOutput", BeginFixedOutput, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "BeginItemList", BeginItemList, VT_I4, VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "BeginNonFiscal", BeginNonFiscal, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "BeginTraining", BeginTraining, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "ClearError", ClearError, VT_I4, VTS_NONE)
	//DISP_FUNCTION(CNCRTBDeviceSO, "BeginRemoval", BeginRemoval, VT_I4, VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "EndFiscalDocument", EndFiscalDocument, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "EndFiscalReceipt", EndFiscalReceipt, VT_I4, VTS_BOOL)
	DISP_FUNCTION(CNCRTBDeviceSO, "EndFixedOutput", EndFixedOutput, VT_I4, VTS_NONE)
	//DISP_FUNCTION(CNCRTBDeviceSO, "EndInsertion", EndInsertion, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "EndItemList", EndItemList, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "EndNonFiscal", EndNonFiscal, VT_I4, VTS_NONE)
	//DISP_FUNCTION(CNCRTBDeviceSO, "EndRemoval", EndRemoval, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "EndTraining", EndTraining, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "GetData", GetData, VT_I4, VTS_I4 VTS_PI4 VTS_PBSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "GetDate", GetDate, VT_I4, VTS_PBSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "GetTotalizer", GetTotalizer, VT_I4, VTS_I4 VTS_I4 VTS_PBSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "GetVatEntry", GetVatEntry, VT_I4, VTS_I4 VTS_I4 VTS_PI4)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintDuplicateReceipt", PrintDuplicateReceipt, VT_I4, VTS_NONE)
	//DISP_FUNCTION(CNCRTBDeviceSO, "PrintNormal", PrintNormal, VT_I4, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintFiscalDocumentLine", PrintFiscalDocumentLine, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintFixedOutput", PrintFixedOutput, VT_I4, VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintPowerLossReport", PrintPowerLossReport, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintRecItem", PrintRecItem, VT_I4, VTS_BSTR VTS_CY VTS_I4 VTS_I4 VTS_CY VTS_BSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintPeriodicTotalsReport", PrintPeriodicTotalsReport, VT_I4, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintRecItemAdjustment", PrintRecItemAdjustment, VT_I4, VTS_I4 VTS_BSTR VTS_CY VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintRecMessage", PrintRecMessage, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintRecNotPaid", PrintRecNotPaid, VT_I4, VTS_BSTR VTS_CY)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintRecSubtotal", PrintRecSubtotal, VT_I4, VTS_CY)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintRecRefund", PrintRecRefund, VT_I4, VTS_BSTR VTS_CY VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintRecTotal", PrintRecTotal, VT_I4, VTS_CY VTS_CY VTS_BSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintRecSubtotalAdjustment", PrintRecSubtotalAdjustment, VT_I4, VTS_I4 VTS_BSTR VTS_CY)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintRecVoid", PrintRecVoid, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintRecVoidItem", PrintRecVoidItem, VT_I4, VTS_BSTR VTS_CY VTS_I4 VTS_I4 VTS_CY VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintReport", PrintReport, VT_I4, VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintXReport", PrintXReport, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "ResetPrinter", ResetPrinter, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "SetDate", SetDate, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "PrintZReport", PrintZReport, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "SetStoreFiscalID", SetStoreFiscalID, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "SetTrailerLine", SetTrailerLine, VT_I4, VTS_I4 VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CNCRTBDeviceSO, "SetVatTable", SetVatTable, VT_I4, VTS_NONE)
	DISP_FUNCTION(CNCRTBDeviceSO, "SetHeaderLine", SetHeaderLine, VT_I4, VTS_I4 VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CNCRTBDeviceSO, "SetPOSID", SetPOSID, VT_I4, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "SetVatValue", SetVatValue, VT_I4, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "VerifyItem", VerifyItem, VT_I4, VTS_BSTR VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "CheckEntry", CheckEntry, VT_BOOL, VTS_I2 VTS_PBSTR)
	DISP_FUNCTION(CNCRTBDeviceSO, "ClaimDevice", ClaimDevice, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CNCRTBDeviceSO, "ReleaseDevice", ReleaseDevice, VT_EMPTY, VTS_NONE)
    // KeyLock
   	DISP_FUNCTION(CNCRTBDeviceSO, "WaitForKeylockChange", WaitForKeylockChange, VT_I4, VTS_I4 VTS_I4)
    // Tone Indicator
    DISP_FUNCTION(CNCRTBDeviceSO, "Sound", Sound, VT_I4, VTS_I4 VTS_I4)
    DISP_FUNCTION(CNCRTBDeviceSO, "SoundImmediate", SoundImmediate, VT_I4, VTS_NONE)

	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


BEGIN_INTERFACE_MAP(CNCRTBDeviceSO, CCmdTarget)
	INTERFACE_PART(CNCRTBDeviceSO, IID_INCRTBDeviceSO, Dispatch)
END_INTERFACE_MAP()

// {2B5E45C0-37E8-11D5-9C83-0090276FD28D}
MY_IMPLEMENT_OLECREATE(CNCRTBDeviceSO, "NCRTBDevice.NCRTBDeviceSO", 0x2b5e45c0, 0x37e8, 0x11d5, 0x9c, 0x83, 0x0, 0x90, 0x27, 0x6f, 0xd2, 0x8d)

/////////////////////////////////////////////////////////////////////////////
// CNCRTBDeviceSO message handlers


long CNCRTBDeviceSO::COFreezeEvents(BOOL bFreeze) 
{
	return m_oService.COFreezeEvents(bFreeze);
}

long CNCRTBDeviceSO::CheckHealth(long lLevel) 
{
	return m_oService.CheckHealth(lLevel);
}

long CNCRTBDeviceSO::Claim(long lTimeout) 
{
	long lResult = OPOS_E_CLOSED;
	if ( m_oService.IsOpen() != FALSE )
	{
		lResult = m_oService.Claim( lTimeout );
	}
	return lResult;
}

long CNCRTBDeviceSO::Close() 
{
    CString csClass = m_oService.GetService().m_sClassName;

    if ( csClass == OPOS_CLASSKEY_LOCK )
    {
        this->Release();
    }
	return m_oService.Close();;
}

long CNCRTBDeviceSO::DirectIO(long lCommand, long FAR* pData, BSTR FAR* pString) 
{
	return m_oService.DirectIO( lCommand, pData, pString );
}

long CNCRTBDeviceSO::GetPropertyNumber(long lPropIndex) 
{
    //if ( (PIDXFptr_DayOpened == lPropIndex) ||
    //     (PIDXFptr_PrinterState == lPropIndex) )
    //{
    //    // These property values must be obtained from the Fiscal printer
	//    CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	//    return pFPrinter->GetPropertyNumber(lPropIndex);
    //}
    //else
    //{
	    return m_oService.GetPropertyNumber(lPropIndex);
    //}
}

BSTR CNCRTBDeviceSO::GetPropertyString(long lPropIndex) 
{
	return m_oService.GetPropertyString(lPropIndex);
}

long CNCRTBDeviceSO::OpenService(LPCTSTR sClass, LPCTSTR sName, LPDISPATCH pDispatch) 
{  // The class is always what the Control says it is!
	m_oService.GetService().m_sClassName = sClass;
	long lResult = m_oService.OpenService( sClass, sName, pDispatch, sClass);

    CString csClass = sClass;
    if ( csClass == OPOS_CLASSKEY_LOCK )
    {
        this->Claim(30000);
    }
	return lResult;
}

long CNCRTBDeviceSO::Release() 
{
	long lResult = OPOS_E_CLOSED;
	if ( m_oService.IsOpen() != FALSE )
		lResult = m_oService.Release();
	return lResult;
}

void CNCRTBDeviceSO::SetPropertyNumber(long lPropIndex, long lProperty) 
{
	m_oService.SetPropertyNumber(lPropIndex, lProperty);
	m_oService.GetDevice()->ReportPropertyNumber( lPropIndex, lProperty );
	return;
}
  
void CNCRTBDeviceSO::SetPropertyString(long lPropIndex, LPCTSTR sProperty) 
{
	m_oService.SetPropertyString(lPropIndex, sProperty);
	return;
}

long CNCRTBDeviceSO::ClearOutput() 
{
    CString csClass = m_oService.GetService().m_sClassName;

    if ( OPOS_CLASSKEY_TONE == csClass )
    {
	    CToneIndicator* pTone = (CToneIndicator*)m_oService.GetDevice();
	    pTone->ClearOutput();
    }

   	return m_oService.ClearOutput();
}

long CNCRTBDeviceSO::ClearInput()
{
	return m_oService.ClearInput();
}


// Line Display Method Implementations
long CNCRTBDeviceSO::ClearText()
{
	CLineDisplay* pDisp = (CLineDisplay*)m_oService.GetDevice();
	return pDisp->ClearText();
}

long CNCRTBDeviceSO::CreateWindow(long lViewportRow, long lViewportColumn, long lViewportHeight, long lViewportWidth, long lWindowHeight, long lWindowWidth) 
{
	CLineDisplay* pDisp = (CLineDisplay*)m_oService.GetDevice();
	return pDisp->CreateWindow( lViewportRow, lViewportColumn, 
		lViewportHeight, lViewportWidth, lWindowHeight, lWindowWidth );
}

long CNCRTBDeviceSO::DestroyWindow()
{
	CLineDisplay* pDisp = (CLineDisplay*)m_oService.GetDevice();
	return pDisp->DestroyWindow( );
}

long CNCRTBDeviceSO::DisplayText(LPCTSTR sData, long lAttribute) 
{
	CLineDisplay* pDisp = (CLineDisplay*)m_oService.GetDevice();
	return pDisp->DisplayText( sData, lAttribute );
}

long CNCRTBDeviceSO::DisplayTextAt(long lRow, long lColumn, LPCTSTR sData, long lAttribute) 
{
	CLineDisplay* pDisp = (CLineDisplay*)m_oService.GetDevice();
	return pDisp->DisplayTextAt( lRow, lColumn, sData, lAttribute );
}


long CNCRTBDeviceSO::RefreshWindow(long lWindow)
{
	// Refresh implementation not required since it is for a real device
	return OPOS_SUCCESS;
}

long CNCRTBDeviceSO::ScrollText(long lDirection, long lUnits) 
{
	// Scrolling implementation not required since it is for a real device
	return OPOS_SUCCESS;
}

long CNCRTBDeviceSO::ClearDescriptors() 
{
	CLineDisplay* pDisp = (CLineDisplay*)m_oService.GetDevice();
	return pDisp->ClearDescriptors();
}

long CNCRTBDeviceSO::SetDescriptor(long lDescriptor, long lAttribute) 
{
	CLineDisplay* pDisp = (CLineDisplay*)m_oService.GetDevice();
	return pDisp->SetDescriptor( lDescriptor, lAttribute );
}

// CashDrawer method implementations

long CNCRTBDeviceSO::OpenDrawer()
{
	CCashDrawer* pCash = (CCashDrawer*)m_oService.GetDevice();
	return pCash->OpenDrawer();
}

long CNCRTBDeviceSO::WaitForDrawerClose(long BeepTimeout, long BeepFrequency, long BeepDuration, long BeepDelay)
{
	CCashDrawer* pCash = (CCashDrawer*)m_oService.GetDevice();
	return pCash->WaitForDrawerClose( BeepTimeout, BeepFrequency, BeepDuration, BeepDelay );
}

// POSPrinter method implementations

long CNCRTBDeviceSO::BeginInsertion(long Timeout)
{
    CString csClass = m_oService.GetService().m_sClassName;

    if ( OPOS_CLASSKEY_PTR == csClass )
    {
	    CPOSPrinter* pPrinter = (CPOSPrinter*)m_oService.GetDevice();
	    return pPrinter->BeginInsertion(Timeout);
    }

    if ( OPOS_CLASSKEY_FPTR == csClass )
    {
        CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
        return pFPrinter->BeginInsertion(Timeout);
    }

	if ( OPOS_CLASSKEY_MICR == csClass )
	{
		CMICR* pMicr = (CMICR*)m_oService.GetDevice();
		return pMicr->BeginInsertion(Timeout);
	}

    return OPOS_E_FAILURE;
}

long CNCRTBDeviceSO::BeginRemoval(long Timeout)
{
    CString csClass = m_oService.GetService().m_sClassName;

    if ( OPOS_CLASSKEY_PTR == csClass )
    {
	    CPOSPrinter* pPrinter = (CPOSPrinter*)m_oService.GetDevice();
	    return pPrinter->BeginRemoval(Timeout);
    }

    if ( OPOS_CLASSKEY_FPTR == csClass )
    {
        CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
        return pFPrinter->BeginRemoval(Timeout);
    }

	if ( OPOS_CLASSKEY_MICR == csClass )
	{
		CMICR* pMicr = (CMICR*)m_oService.GetDevice();
		return pMicr->BeginRemoval(Timeout);
	}

    return OPOS_E_FAILURE;
}

long CNCRTBDeviceSO::EndInsertion()
{
    CString csClass = m_oService.GetService().m_sClassName;

    if ( OPOS_CLASSKEY_PTR == csClass )
    {
	    CPOSPrinter* pPrinter = (CPOSPrinter*)m_oService.GetDevice();
	    return pPrinter->EndInsertion();
    }

    if ( OPOS_CLASSKEY_FPTR == csClass )
    {
        CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
        return pFPrinter->EndInsertion();
    }

	if ( OPOS_CLASSKEY_MICR == csClass )
	{
		CMICR* pMicr = (CMICR*)m_oService.GetDevice();
		return pMicr->EndInsertion();
	}

    return OPOS_E_FAILURE;
}

long CNCRTBDeviceSO::EndRemoval()
{
    CString csClass = m_oService.GetService().m_sClassName;

    if ( OPOS_CLASSKEY_PTR == csClass )
    {
	    CPOSPrinter* pPrinter = (CPOSPrinter*)m_oService.GetDevice();
	    return pPrinter->EndRemoval();
    }

    if ( OPOS_CLASSKEY_FPTR == csClass )
    {
        CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
        return pFPrinter->EndRemoval();
    }

	if ( OPOS_CLASSKEY_MICR == csClass )
	{
		CMICR* pMicr = (CMICR*)m_oService.GetDevice();
		return pMicr->EndRemoval();
	}

    return OPOS_E_FAILURE;
}

long CNCRTBDeviceSO::CutPaper(long Percentage)
{
	CPOSPrinter* pPrinter = (CPOSPrinter*)m_oService.GetDevice();
	return pPrinter->CutPaper(Percentage);
}

long CNCRTBDeviceSO::PrintBarCode(long Station, LPCTSTR Data, long Symbology, long Height, long Width, long Alignment, long TextPosition)
{
	CPOSPrinter* pPrinter = (CPOSPrinter*)m_oService.GetDevice();
	return pPrinter->PrintBarCode(Station, Data, Symbology, Height, Width, Alignment, TextPosition);
}

long CNCRTBDeviceSO::PrintBitmap(long Station, LPCTSTR FileName, long Width, long Alignment)
{
	CPOSPrinter* pPrinter = (CPOSPrinter*)m_oService.GetDevice();
	return pPrinter->PrintBitmap(Station, FileName, Width, Alignment);
}

long CNCRTBDeviceSO::PrintImmediate(long Station, LPCTSTR Data)
{
	CPOSPrinter* pPrinter = (CPOSPrinter*)m_oService.GetDevice();
	return pPrinter->PrintImmediate(Station, Data);
}

long CNCRTBDeviceSO::PrintNormal(long Station, LPCTSTR Data)
{
    CString csClass = m_oService.GetService().m_sClassName;

    if ( OPOS_CLASSKEY_PTR == csClass )
    {
	    CPOSPrinter* pPrinter = (CPOSPrinter*)m_oService.GetDevice();
	    return pPrinter->PrintNormal(Station, Data);
    }

    if ( OPOS_CLASSKEY_FPTR == csClass )
    {
        CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
        return pFPrinter->PrintNormal(Station, Data);
    }

    return OPOS_E_FAILURE;
}

long CNCRTBDeviceSO::PrintTwoNormal(long Stations, LPCTSTR Data1, LPCTSTR Data2)
{
	CPOSPrinter* pPrinter = (CPOSPrinter*)m_oService.GetDevice();
	return pPrinter->PrintTwoNormal(Stations, Data1, Data2);
}

long CNCRTBDeviceSO::RotatePrint(long Station, long Rotation)
{
	CPOSPrinter* pPrinter = (CPOSPrinter*)m_oService.GetDevice();
	return pPrinter->RotatePrint(Station, Rotation);
}

long CNCRTBDeviceSO::SetBitmap(long BitmapNumber, long Station, LPCTSTR FileName, long Width, long Alignment)
{
	CPOSPrinter* pPrinter = (CPOSPrinter*)m_oService.GetDevice();
	return pPrinter->SetBitmap(BitmapNumber, Station, FileName, Width, Alignment);
}

long CNCRTBDeviceSO::SetLogo(long Location, LPCTSTR Data)
{
	CPOSPrinter* pPrinter = (CPOSPrinter*)m_oService.GetDevice();
	return pPrinter->SetLogo(Location, Data);
}

long CNCRTBDeviceSO::TransactionPrint(long Station, long Control)
{
	CPOSPrinter* pPrinter = (CPOSPrinter*)m_oService.GetDevice();
	return pPrinter->TransactionPrint(Station, Control);
}

long CNCRTBDeviceSO::ValidateData(long Station, LPCTSTR Data)
{
	CPOSPrinter* pPrinter = (CPOSPrinter*)m_oService.GetDevice();
	return pPrinter->ValidateData(Station, Data);
}

// Scale Methods
long CNCRTBDeviceSO::ReadWeight( long* pWeightData, long Timeout )
{
	CScale* pScale = (CScale*)m_oService.GetDevice();
	return pScale->ReadWeight( pWeightData, Timeout );
}


// Fiscal Printer Properties
//    Taken directly from the Fiscal Printer SO
long CNCRTBDeviceSO::GetErrorLevel() 
{
	return 0;
}

void CNCRTBDeviceSO::SetErrorLevel(long nNewValue) 
{
}

long CNCRTBDeviceSO::GetErrorOutID() 
{
	return 0;
}

void CNCRTBDeviceSO::SetErrorOutID(long nNewValue) 
{
}

long CNCRTBDeviceSO::GetErrorState() 
{
	return 0;
}

void CNCRTBDeviceSO::SetErrorState(long nNewValue) 
{
}

BSTR CNCRTBDeviceSO::GetErrorString() 
{
	CString strResult;
	return strResult.AllocSysString();
}

void CNCRTBDeviceSO::SetErrorString(LPCTSTR lpszNewValue) 
{
}

BOOL CNCRTBDeviceSO::GetFlagWhenIdle() 
{
	return TRUE;
}

void CNCRTBDeviceSO::SetFlagWhenIdle(BOOL bNewValue) 
{
}

long CNCRTBDeviceSO::GetErrorStation() 
{
	return 0;
}

void CNCRTBDeviceSO::SetErrorStation(long nNewValue) 
{
}

// Fiscal Printer Methods
long CNCRTBDeviceSO::BeginFiscalDocument(long DocumentAmount)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->BeginFiscalDocument(DocumentAmount);
}

/*long CNCRTBDeviceSO::BeginInsertion(long Timeout)
{
    return OPOS_E_NOSERVICE;
}*/

long CNCRTBDeviceSO::BeginFiscalReceipt(BOOL PrintHeader)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->BeginFiscalReceipt(PrintHeader);
}

long CNCRTBDeviceSO::BeginFixedOutput(long Station, long DocumentType)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->BeginFixedOutput(Station, DocumentType);
}

long CNCRTBDeviceSO::BeginItemList(long VatID)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->BeginItemList(VatID);
}

long CNCRTBDeviceSO::BeginNonFiscal()
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->BeginNonFiscal();
}

long CNCRTBDeviceSO::BeginTraining()
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->BeginTraining();
}

long CNCRTBDeviceSO::ClearError()
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->ClearError();
}

/*long CNCRTBDeviceSO::BeginRemoval(long Timeout)
{
    return OPOS_E_NOSERVICE;
}*/

long CNCRTBDeviceSO::EndFiscalDocument()
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->EndFiscalDocument();
}

long CNCRTBDeviceSO::EndFiscalReceipt(BOOL PrintHeader)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->EndFiscalReceipt(PrintHeader);
}

long CNCRTBDeviceSO::EndFixedOutput()
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->EndFixedOutput();
}

/*long CNCRTBDeviceSO::EndInsertion()
{
    return OPOS_E_NOSERVICE;
}*/

long CNCRTBDeviceSO::EndItemList()
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->EndItemList();
}

long CNCRTBDeviceSO::EndNonFiscal()
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->EndNonFiscal();
}

/*long CNCRTBDeviceSO::EndRemoval()
{
    return OPOS_E_NOSERVICE;
}*/

long CNCRTBDeviceSO::EndTraining()
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->EndTraining();
}

long CNCRTBDeviceSO::GetData(long DataItem, long FAR* OptArgs, BSTR FAR* Data)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->GetData(DataItem, OptArgs, Data);
}

long CNCRTBDeviceSO::GetDate(BSTR FAR* Date)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->GetDate(Date);
}

long CNCRTBDeviceSO::GetTotalizer(long VatID, long OptArgs, BSTR FAR* Data)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->GetTotalizer(VatID, OptArgs, Data);
}

long CNCRTBDeviceSO::GetVatEntry(long VatID, long OptArgs, long FAR* VatRate)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->GetVatEntry(VatID, OptArgs, VatRate);
}

long CNCRTBDeviceSO::PrintDuplicateReceipt()
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintDuplicateReceipt();
}

/*long CNCRTBDeviceSO::PrintNormal(long Station, LPCTSTR Data)
{
    return OPOS_E_NOSERVICE;
}*/

long CNCRTBDeviceSO::PrintFiscalDocumentLine(LPCTSTR DocumentLine)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintFiscalDocumentLine(DocumentLine);
}

long CNCRTBDeviceSO::PrintFixedOutput(long DocumentType, long LineNumber, LPCTSTR Data)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintFixedOutput(DocumentType, LineNumber, Data);
}

long CNCRTBDeviceSO::PrintPowerLossReport()
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintPowerLossReport();
}

long CNCRTBDeviceSO::PrintRecItem(LPCTSTR Description, CURRENCY Price, long Quantity, long VatInfo, CURRENCY UnitPrice, LPCTSTR UnitName)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintRecItem(Description, Price, Quantity, VatInfo, UnitPrice, UnitName);
}

long CNCRTBDeviceSO::PrintPeriodicTotalsReport(LPCTSTR Date1, LPCTSTR Date2)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintPeriodicTotalsReport(Date1, Date2);
}

long CNCRTBDeviceSO::PrintRecItemAdjustment(long AdjustmentType, LPCTSTR Description, CURRENCY Amount, long VatInfo)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintRecItemAdjustment(AdjustmentType, Description, Amount, VatInfo);
}

long CNCRTBDeviceSO::PrintRecMessage(LPCTSTR Message)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintRecMessage(Message);
}

long CNCRTBDeviceSO::PrintRecNotPaid(LPCTSTR Description, CURRENCY Amount)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintRecNotPaid(Description, Amount);
}

long CNCRTBDeviceSO::PrintRecSubtotal(CURRENCY Amount)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintRecSubtotal(Amount);
}

long CNCRTBDeviceSO::PrintRecRefund(LPCTSTR Description, CURRENCY Amount, long VatInfo)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintRecRefund(Description, Amount, VatInfo);
}

long CNCRTBDeviceSO::PrintRecTotal(CURRENCY Total, CURRENCY Payment, LPCTSTR Description)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintRecTotal(Total, Payment, Description);
}

long CNCRTBDeviceSO::PrintRecSubtotalAdjustment(long AdjustmentType, LPCTSTR Description, CURRENCY Amount)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintRecSubtotalAdjustment(AdjustmentType, Description, Amount);
}

long CNCRTBDeviceSO::PrintRecVoid(LPCTSTR Description)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintRecVoid(Description);
}

long CNCRTBDeviceSO::PrintRecVoidItem(LPCTSTR Description, CURRENCY Amount, long Quantity, long AdjustmentType, CURRENCY Adjustment, long VatInfo)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintRecVoidItem(Description, Amount, Quantity, AdjustmentType, Adjustment, VatInfo);
}

long CNCRTBDeviceSO::PrintReport(long ReportType, LPCTSTR StartNum, LPCTSTR EndNum)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintReport(ReportType, StartNum, EndNum);
}

long CNCRTBDeviceSO::PrintXReport()
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintXReport();
}

long CNCRTBDeviceSO::ResetPrinter()
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->ResetPrinter();
}

long CNCRTBDeviceSO::SetDate(LPCTSTR Date)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->SetDate(Date);
}

long CNCRTBDeviceSO::PrintZReport()
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->PrintZReport();
}

long CNCRTBDeviceSO::SetStoreFiscalID(LPCTSTR ID)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->SetStoreFiscalID(ID);
}

long CNCRTBDeviceSO::SetTrailerLine(long LineNumber, LPCTSTR Text, BOOL DoubleWidth)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->SetTrailerLine(LineNumber, Text, DoubleWidth);
}

long CNCRTBDeviceSO::SetVatTable()
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->SetVatTable();
}

long CNCRTBDeviceSO::SetHeaderLine(long LineNumber, LPCTSTR Text, BOOL DoubleWidth)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->SetHeaderLine(LineNumber, Text, DoubleWidth);
}

long CNCRTBDeviceSO::SetPOSID(LPCTSTR POSID, LPCTSTR CashierID)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->SetPOSID(POSID, CashierID);
}

long CNCRTBDeviceSO::SetVatValue(long VatID, LPCTSTR VatValue)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->SetVatValue(VatID, VatValue);
}

long CNCRTBDeviceSO::VerifyItem(LPCTSTR ItemName, long VatID)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->VerifyItem(ItemName, VatID);
}

BOOL CNCRTBDeviceSO::CheckEntry(short nOptions, BSTR FAR* pnStn)
{
	CFiscalPrinter* pFPrinter = (CFiscalPrinter*)m_oService.GetDevice();
	return pFPrinter->CheckEntry(nOptions, pnStn);
}

void CNCRTBDeviceSO::ClaimDevice(long TimeOut)
{
    this->Claim( TimeOut );
}

void CNCRTBDeviceSO::ReleaseDevice()
{
    this->Release();
}

// KeyLock
long CNCRTBDeviceSO::WaitForKeylockChange( long KeyPosition, long Timeout)
{
    CKeyLock* pKeyLock = (CKeyLock*)m_oService.GetDevice();
    return pKeyLock->WaitForKeylockChange( KeyPosition, Timeout );
}


//ToneIndicator
long CNCRTBDeviceSO::Sound(long NumberOfCycles, long InterSoundWait)
{
    CToneIndicator* pTone = (CToneIndicator*)m_oService.GetDevice();
    return pTone->Sound( NumberOfCycles, InterSoundWait );
}

long CNCRTBDeviceSO::SoundImmediate()
{
    CToneIndicator* pTone = (CToneIndicator*)m_oService.GetDevice();
    return pTone->SoundImmediate( );
}


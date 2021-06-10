// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "..\stdafx.h"
#include "CardDispenser.h"
#include "SankyoCPConst.h"
#define COMP_ID ID_DM		// used by the trace() calls to identify this component
#define T_ID    _T("DCD")

/////////////////////////////////////////////////////////////////////////////
// CCardDispenser

//IMPLEMENT_DYNCREATE(CCardDispenser, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CCardDispenser properties

/////////////////////////////////////////////////////////////////////////////
// CCardDispenser operations
BOOL CCardDispenser::Create( 
   CWnd* pParentWnd, 
   UINT nID )
{
	static LPCTSTR sProgId = _T("OPOS.PointCardRW");
	BOOL bResult;
	bResult = CreateControl( GetClsid(), NULL, 0, CRect(0,0,0,0), pParentWnd, nID );
	return bResult;
}

long CCardDispenser::GetOpenResult()
{
	long result;
	InvokeHelper(0x31, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetBinaryConversion()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetBinaryConversion(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CCardDispenser::GetCapPowerReporting()
{
	long result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString CCardDispenser::GetCheckHealthText()
{
	CString result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetClaimed()
{
	BOOL result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetDataCount()
{
	long result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetDataEventEnabled()
{
	BOOL result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetDataEventEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCardDispenser::GetDeviceEnabled()
{
	BOOL result;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetDeviceEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
    trace(L6, _T("CCardDispenser:OpenDevice called"));
	InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCardDispenser::GetFreezeEvents()
{
	BOOL result;
	InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetFreezeEvents(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CCardDispenser::GetOutputID()
{
	long result;
	InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetPowerNotify()
{
	long result;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetPowerNotify(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CCardDispenser::GetPowerState()
{
	long result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetResultCode()
{
	long result;
	InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    trace(L6, _T("CCardDispenser:GetResultCode called %d"), result);
	return result;
}

long CCardDispenser::GetResultCodeExtended()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetState()
{
	long result;
	InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString CCardDispenser::GetControlObjectDescription()
{
	CString result;
	InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetControlObjectVersion()
{
	long result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString CCardDispenser::GetServiceObjectDescription()
{
	CString result;
	InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetServiceObjectVersion()
{
	long result;
	InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString CCardDispenser::GetDeviceDescription()
{
	CString result;
	InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CCardDispenser::GetDeviceName()
{
	CString result;
	InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CCardDispenser::CheckHealth(long Level)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Level);
	return result;
}

long CCardDispenser::ClaimDevice(long Timeout)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		1000);
	return result;
}

long CCardDispenser::Claim(long Timeout)
{
	return ClaimDevice(Timeout);
}

long CCardDispenser::ClearInput()
{
	long result;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::ClearOutput()
{
	long result;
	InvokeHelper(0x22, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::Close()
{
	long result;
	InvokeHelper(0x23, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::DirectIO(long Command, long* pData, BSTR* pString)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_PI4 VTS_PBSTR;
	InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Command, pData, pString);
	return result;
}

long CCardDispenser::Open(LPCTSTR DeviceName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		DeviceName);
	return result;
}

long CCardDispenser::ReleaseDevice()
{
	long result;
	InvokeHelper(0x26, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetCapBold()
{
	BOOL result;
	InvokeHelper(0x32, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetCapCardEntranceSensor()
{
	BOOL result;
	InvokeHelper(0x33, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetCapCharacterSet()
{
	long result;
	InvokeHelper(0x34, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetCapCleanCard()
{
	BOOL result;
	InvokeHelper(0x35, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetCapClearPrint()
{
	BOOL result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetCapDhigh()
{
	BOOL result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetCapDwide()
{
	BOOL result;
	InvokeHelper(0x38, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetCapDwideDhigh()
{
	BOOL result;
	InvokeHelper(0x39, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetCapItalic()
{
	BOOL result;
	InvokeHelper(0x3a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetCapLeft90()
{
	BOOL result;
	InvokeHelper(0x3b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetCapPrint()
{
	BOOL result;
	InvokeHelper(0x3c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetCapPrintMode()
{
	BOOL result;
	InvokeHelper(0x3d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetCapRight90()
{
	BOOL result;
	InvokeHelper(0x3e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetCapRotate180()
{
	BOOL result;
	InvokeHelper(0x3f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetCapTracksToRead()
{
	long result;
	InvokeHelper(0x40, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetCapTracksToWrite()
{
	long result;
	InvokeHelper(0x41, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetCardState()
{
	long result;
	InvokeHelper(0x42, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetCharacterSet()
{
	long result;
	InvokeHelper(0x43, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetCharacterSet(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x43, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString CCardDispenser::GetCharacterSetList()
{
	CString result;
	InvokeHelper(0x44, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CCardDispenser::GetFontTypeFaceList()
{
	CString result;
	InvokeHelper(0x45, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetLineChars()
{
	long result;
	InvokeHelper(0x46, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetLineChars(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x46, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString CCardDispenser::GetLineCharsList()
{
	CString result;
	InvokeHelper(0x47, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetLineHeight()
{
	long result;
	InvokeHelper(0x48, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetLineHeight(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x48, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CCardDispenser::GetLineSpacing()
{
	long result;
	InvokeHelper(0x49, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetLineSpacing(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x49, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CCardDispenser::GetLineWidth()
{
	long result;
	InvokeHelper(0x4a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetMapMode()
{
	long result;
	InvokeHelper(0x4b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetMapMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CCardDispenser::GetMaxLine()
{
	long result;
	InvokeHelper(0x4c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetPrintHeight()
{
	long result;
	InvokeHelper(0x4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetReadState1()
{
	long result;
	InvokeHelper(0x4e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetReadState2()
{
	long result;
	InvokeHelper(0x4f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetRecvLength1()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetRecvLength2()
{
	long result;
	InvokeHelper(0x51, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetSidewaysMaxChars()
{
	long result;
	InvokeHelper(0x52, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetSidewaysMaxLines()
{
	long result;
	InvokeHelper(0x53, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString CCardDispenser::GetTrack1Data()
{
	CString result;
	InvokeHelper(0x56, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CCardDispenser::GetTrack2Data()
{
	CString result;
	InvokeHelper(0x57, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CCardDispenser::GetTrack3Data()
{
	CString result;
	InvokeHelper(0x58, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CCardDispenser::GetTrack4Data()
{
	CString result;
	InvokeHelper(0x59, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CCardDispenser::GetTrack5Data()
{
	CString result;
	InvokeHelper(0x5a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CCardDispenser::GetTrack6Data()
{
	CString result;
	InvokeHelper(0x5b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetTracksToRead()
{
	long result;
	InvokeHelper(0x54, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetTracksToRead(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x54, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CCardDispenser::GetTracksToWrite()
{
	long result;
	InvokeHelper(0x55, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetTracksToWrite(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x55, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString CCardDispenser::GetWrite1Data()
{
	CString result;
	InvokeHelper(0x5e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetWrite1Data(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString CCardDispenser::GetWrite2Data()
{
	CString result;
	InvokeHelper(0x5f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetWrite2Data(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString CCardDispenser::GetWrite3Data()
{
	CString result;
	InvokeHelper(0x60, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetWrite3Data(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString CCardDispenser::GetWrite4Data()
{
	CString result;
	InvokeHelper(0x61, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetWrite4Data(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x61, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString CCardDispenser::GetWrite5Data()
{
	CString result;
	InvokeHelper(0x62, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetWrite5Data(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x62, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString CCardDispenser::GetWrite6Data()
{
	CString result;
	InvokeHelper(0x63, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetWrite6Data(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x63, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long CCardDispenser::GetWriteState1()
{
	long result;
	InvokeHelper(0x5c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::GetWriteState2()
{
	long result;
	InvokeHelper(0x5d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::BeginInsertion(long Timeout)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x78, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Timeout);
	return result;
}

long CCardDispenser::BeginRemoval(long Timeout)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x79, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Timeout);
	return result;
}

long CCardDispenser::CleanCard()
{
	long result;
	InvokeHelper(0x7a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::ClearPrintWrite(long Kind, long Hposition, long Vposition, long Width, long Height)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x7b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Kind, Hposition, Vposition, Width, Height);
	return result;
}

long CCardDispenser::EndInsertion()
{
	long result;
	InvokeHelper(0x7c, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::EndRemoval()
{
	long result;
	InvokeHelper(0x7d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CCardDispenser::PrintWrite(long Kind, long Hposition, long Vposition, LPCTSTR Data)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_BSTR;
	InvokeHelper(0x7e, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Kind, Hposition, Vposition, Data);
	return result;
}

long CCardDispenser::RotatePrint(long Rotation)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7f, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Rotation);
	return result;
}

long CCardDispenser::ValidateData(LPCTSTR Data)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x80, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Data);
	return result;
}

BOOL CCardDispenser::GetCapMapCharacterSet()
{
	BOOL result;
	InvokeHelper(0x64, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetMapCharacterSet()
{
	BOOL result;
	InvokeHelper(0x65, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCardDispenser::SetMapCharacterSet(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x65, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCardDispenser::GetCapStatisticsReporting()
{
	BOOL result;
	InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CCardDispenser::GetCapUpdateStatistics()
{
	BOOL result;
	InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long CCardDispenser::ResetStatistics(LPCTSTR StatisticsBuffer)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x29, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		StatisticsBuffer);
	return result;
}

long CCardDispenser::RetrieveStatistics(BSTR* pStatisticsBuffer)
{
	long result;
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pStatisticsBuffer);
	return result;
}

long CCardDispenser::UpdateStatistics(LPCTSTR StatisticsBuffer)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		StatisticsBuffer);
	return result;
}

void CCardDispenser::OnOposStatusEvent( long  nStatus )
{
	long lStatus = 0;
    trace(L6, _T("CCardDispenser:OnOposStatusEvent called %d"), nStatus);
	switch( nStatus )
	{
	case SANKYO_USER_MT_CRD_STAT_1:
		//lStatus = DM_CD_CARD_REMOVED;
		lStatus = SANKYO_USER_MT_CRD_STAT_1;  //SR672
		break;
	case SANKYO_USER_MT_CRD_STAT_2:
		//lStatus = DM_CD_CARD_AT_GATE;
		lStatus = SANKYO_USER_MT_CRD_STAT_2;  //SR672
		break;
	case SANKYO_USER_MT_CRD_STAT_3:
		//lStatus =  DM_CD_CARD_IN_TRANSPORT;
		lStatus =  SANKYO_USER_MT_CRD_STAT_3;  //SR672
		break;

	default:
		lStatus = 0;
		break;
	}
	if (lStatus > 0)
	{
		PostDmEventToApplication( DM_STATUS, DMCLASS_CARDDISPENSER, m_nDeviceId, lStatus );
	}

   return;
}

/* virtual */
void CCardDispenser::OnOposDirectIOEvent( 
					 long lEventCode, 
					 long FAR* lpData, 
					 BSTR FAR *pString)

{
	long nStatus = 0;
    trace(L6, _T("CCardDispenser:OnOposDirectIOEvent called %d"), lEventCode);
	switch( lEventCode )
	{
	case SANKYO_DIOE_HOPPER_EMPTY:
		nStatus = DM_CD_HOPPER_EMPTY;
		break;
	case SANKYO_DIOE_HOPPER_NEAR_EMPTY:
		nStatus = DM_CD_HOPPER_NEAR_EMPTY;
		break;
	case SANKYO_DIOE_HOPPER_GUIDE_DOOR_OPEN:
		nStatus =  DM_CD_HOPPER_DOOR_OPEN;
		break;
	default:
		nStatus = 0;
		break;
	}
	if (nStatus > 0)
	{
		PostDmEventToApplication( DM_STATUS, DMCLASS_CARDDISPENSER, m_nDeviceId, nStatus );
	}

	return ;
}

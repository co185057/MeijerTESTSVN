// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "sigcap.h"

/////////////////////////////////////////////////////////////////////////////
// CSigCap

IMPLEMENT_DYNCREATE(CSigCap, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CSigCap properties

/////////////////////////////////////////////////////////////////////////////
// CSigCap operations

long CSigCap::GetOpenResult()
{
	long result;
	InvokeHelper(0x31, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString CSigCap::GetCheckHealthText()
{
	CString result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL CSigCap::GetClaimed()
{
	BOOL result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CSigCap::GetDataEventEnabled()
{
	BOOL result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CSigCap::SetDataEventEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CSigCap::GetDeviceEnabled()
{
	BOOL result;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CSigCap::SetDeviceEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CSigCap::GetFreezeEvents()
{
	BOOL result;
	InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CSigCap::SetFreezeEvents(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CSigCap::GetResultCode()
{
	long result;
	InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CSigCap::GetResultCodeExtended()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CSigCap::GetState()
{
	long result;
	InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString CSigCap::GetControlObjectDescription()
{
	CString result;
	InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CSigCap::GetControlObjectVersion()
{
	long result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString CSigCap::GetServiceObjectDescription()
{
	CString result;
	InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CSigCap::GetServiceObjectVersion()
{
	long result;
	InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString CSigCap::GetDeviceDescription()
{
	CString result;
	InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CSigCap::GetDeviceName()
{
	CString result;
	InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CSigCap::CheckHealth(long Level)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Level);
	return result;
}

long CSigCap::ClaimDevice(long Timeout)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Timeout);
	return result;
}

long CSigCap::ClearInput()
{
	long result;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CSigCap::Close()
{
	long result;
	InvokeHelper(0x23, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CSigCap::DirectIO(long Command, long* pData, BSTR* pString)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_PI4 VTS_PBSTR;
	InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Command, pData, pString);
	return result;
}

long CSigCap::Open(LPCTSTR DeviceName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		DeviceName);
	return result;
}

long CSigCap::ReleaseDevice()
{
	long result;
	InvokeHelper(0x26, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL CSigCap::GetCapDisplay()
{
	BOOL result;
	InvokeHelper(0x32, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CSigCap::GetCapUserTerminated()
{
	BOOL result;
	InvokeHelper(0x34, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long CSigCap::GetMaximumX()
{
	long result;
	InvokeHelper(0x35, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CSigCap::GetMaximumY()
{
	long result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

// CSigCap::GetPointArray
//
// Purpose: Correctly handle signature point data in UNICODE application.
// Problem: OPOS defines that the data is byte data even though it is returned in a BSTR
//          When the application is UNICODE, this is not what happens
//          - Both Topaz and 5992 SOs return the data as follows:
//          Assume that a point is: (x,y) = 0x3B2,0x0090
//          OPOS defines that we should see: B2 03 90 00 (bytes) in the return value
//          If we inspect what comes from the SO, we will see 
//          B2 00 03 00 90 00 00 00
//          This is incorrect. However, when this application is ANSI, MFC corrects
//          this automatically and returns the data as B2 03 90 00 (bytes)
//          When the application is UNICODE, MFC does not conversion,
//          so the data has the unwanted zeros.
//          This code tells the SO to convert the data to ascii, as
//          3B 32 30 33 39 30 30 30
//          This gets returned as exactly that when the application is ANSI,
//          but as 3B 00 32 00 30 00 33 00 39 00 30 00 30 00 30 00
//          when the application is UNICODE. 
//          This code handles both cases correctly by converting each TCHAR
//          in the CString to a single HEX byte
//          ANSI:    3B 32 becomes B2
//          UNICODE: 3B 00 32 00 also becomes B2

CString CSigCap::GetPointArray()
{
   // Get this in ascii format. 
	CString result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   // Restore BC option
   // Now convert the ascii data to hex 
   // Note: If the byte length of the sig is N, then the length of the string in nibble
   // format is 2N. This is true regardless of whether the nibble string is UNICODE or not
   // If the length of the CString is 96, then this will be words in UNICODE, but bytes in
   // non-UNICODE. In both cases, the byte length will be 48 bytes. When the string is 
   // UNICODE, we throw away 3/4 of the data. In ANSI, we throw away 1/2 the data.
   int nLength = result.GetLength() / 2;
   // Do not change this to a TCHAR
   char* charSigData = (char*)_alloca( nLength );
   for ( int nIndex = 0; nIndex < nLength; nIndex++ )
      {
      TCHAR highNibble = result[nIndex * 2] & 0x0F;
      TCHAR lowNibble = result[nIndex * 2 + 1] & 0x0F;
      *(charSigData + nIndex) = (highNibble  << 4) + lowNibble;
      }
   // Now move the hex data to a CString (no conversion)
   LPTSTR sBuffer = result.GetBufferSetLength( nLength );
   memcpy( sBuffer, charSigData, nLength );
   // Now set length to correct size for ANSI/UNICODE build
   // Caller must realize that CString::GetLength() is words in UNICODE build
   result.ReleaseBuffer( nLength / sizeof( TCHAR ) );
   // Now we are returning what OPOS indicated should be returned regardless
   // of whether the application is UNICODE or not. 
   return result;
}

CString CSigCap::GetRawData()
{
	CString result;
	InvokeHelper(0x38, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CSigCap::GetTotalPoints()
{
	long result;
	InvokeHelper(0x3a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CSigCap::BeginCapture(LPCTSTR FormName)
{
   SetBinaryConversion(OPOS_BC_NIBBLE);
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x46, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		FormName);
   	return result;
}

long CSigCap::EndCapture()
{
	long result;
	InvokeHelper(0x47, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL CSigCap::GetAutoDisable()
{
	BOOL result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CSigCap::SetAutoDisable(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CSigCap::GetBinaryConversion()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CSigCap::SetBinaryConversion(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CSigCap::GetDataCount()
{
	long result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL CSigCap::GetCapRealTimeData()
{
	BOOL result;
	InvokeHelper(0x33, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CSigCap::GetRealTimeDataEnabled()
{
	BOOL result;
	InvokeHelper(0x39, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CSigCap::SetRealTimeDataEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x39, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CSigCap::GetCapPowerReporting()
{
	long result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CSigCap::GetPowerNotify()
{
	long result;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CSigCap::SetPowerNotify(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CSigCap::GetPowerState()
{
	long result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

#if !defined(AFX_MOTIONSENSOR_H__9B1B14BF_BF5D_427B_9686_A9336E1821CD__INCLUDED_)
#define AFX_MOTIONSENSOR_H__9B1B14BF_BF5D_427B_9686_A9336E1821CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CMotionSensor wrapper class
#include "OposControl.h"

class CMotionSensor : public COposControl
{
protected:
	DECLARE_DYNCREATE(CMotionSensor)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xbe304b95, 0xc3ea, 0x11d1, { 0xa0, 0xbf, 0x0, 0xaa, 0x0, 0x5b, 0x4, 0xcb } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:
	long GetBinaryConversion();
	void SetBinaryConversion(long);
	CString GetCheckHealthText();
	void SetCheckHealthText(LPCTSTR);
	BOOL GetClaimed();
	void SetClaimed(BOOL);
	BOOL GetDeviceEnabled();
	void SetDeviceEnabled(BOOL);
	BOOL GetFreezeEvents();
	void SetFreezeEvents(BOOL);
	long GetResultCode();
	void SetResultCode(long);
	long GetResultCodeExtended();
	void SetResultCodeExtended(long);
	long GetState();
	void SetState(long);
	CString GetControlObjectDescription();
	void SetControlObjectDescription(LPCTSTR);
	long GetControlObjectVersion();
	void SetControlObjectVersion(long);
	CString GetServiceObjectDescription();
	void SetServiceObjectDescription(LPCTSTR);
	long GetServiceObjectVersion();
	void SetServiceObjectVersion(long);
	CString GetDeviceDescription();
	void SetDeviceDescription(LPCTSTR);
	CString GetDeviceName();
	void SetDeviceName(LPCTSTR);
	long GetCapPowerReporting();
	void SetCapPowerReporting(long);
	long GetPowerNotify();
	void SetPowerNotify(long);
	long GetPowerState();
	void SetPowerState(long);

// Operations
public:
	long Open(LPCTSTR DeviceName);
	long Close();
	long Claim(long Timeout);
	long Release();
	long DirectIO(long Command, long* pData, BSTR* pString);
	long CheckHealth(long Level);
	long WaitForMotion(long Timeout);
	void SODirectIO(long Command, long* pData, BSTR* pString);
	void SOStatusUpdate(long Status);
	long SOProcessID();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONSENSOR_H__9B1B14BF_BF5D_427B_9686_A9336E1821CD__INCLUDED_)

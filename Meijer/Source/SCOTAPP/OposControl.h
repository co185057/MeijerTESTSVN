// OposControl.h: interface for the COposControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPOSCONTROL_H__54025944_C9D1_11D3_BBA8_00A0C9EDD523__INCLUDED_)
#define AFX_OPOSCONTROL_H__54025944_C9D1_11D3_BBA8_00A0C9EDD523__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "opos.h"
#include "oposdisp.h"
#include "oposmsr.h"
#include "oposppad.h"

// Device Availability codes to emulate DM
#define SCOT_DA_NOT_CONFIGURED            _T('0')
#define SCOT_DA_LOADED                    _T('1')
#define SCOT_DA_CONFIGURED_BUT_NOT_LOADED _T('2')

class COposControl : public CWnd  
{
public:
	void UnInitialize();
	BOOL Initialize(LPCTSTR profile, CWnd* pParent, UINT nID, BOOL bLeaveEnabled = FALSE);
	virtual BOOL OpenControl(LPCTSTR profile, UINT nID, BOOL bLeaveEnabled = FALSE);
	virtual void CloseControl();
	virtual ~COposControl();
	CString GetErrorFunc() { return m_csFuncInErr; }
	
protected:
	BOOL m_bOpened;
	BOOL m_bClaimed;
	BOOL m_bEnabled;
	BOOL m_bCreated;
	CHAR m_Availability;
	CString m_csFuncInErr;
	
	COposControl();
	
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, 
		DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, 
		UINT nID,
		CCreateContext* pContext = NULL) = 0;
	
	virtual BOOL Create(LPCTSTR lpszWindowName, 
		DWORD dwStyle,
		const RECT& rect, 
		CWnd* pParentWnd, 
		UINT nID,
		CFile* pPersist = NULL, 
		BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL) = 0;
	
	virtual long Open(LPCTSTR profile) = 0;
	virtual long Close() = 0;
	virtual long ClaimDevice(long Timeout) = 0;    //TAR253061
	virtual long ReleaseDevice() = 0;              //TAR253061
	virtual long CheckHealth(long Level) = 0;
	
	virtual void SetDataEventEnabled(BOOL) {}
	virtual BOOL GetDeviceEnabled() = 0;
	virtual void SetDeviceEnabled(BOOL) = 0;
	
	virtual long GetResultCode() = 0;
	virtual long GetResultCodeExtended() = 0;
	
};

#endif // !defined(AFX_OPOSCONTROL_H__54025944_C9D1_11D3_BBA8_00A0C9EDD523__INCLUDED_)

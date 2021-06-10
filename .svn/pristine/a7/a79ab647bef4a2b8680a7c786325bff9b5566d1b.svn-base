// ScotAppBusyStatus.h: interface for the ApplStatus class.
//
//////////////////////////////////////////////////////////////////////
//Implemented for RFC 263404

#if !defined(AFX_ScotAppBusyStatus_H__23CA67AC_8EA5_45D3_B135_E97B08F9DD27__INCLUDED_)
#define AFX_ScotAppBusyStatus_H__23CA67AC_8EA5_45D3_B135_E97B08F9DD27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef const TCHAR   *PCSTRING;

#include "DLLDefine.h"
#include "COMDEF.H"

//class DLLEXPORT
//ErrorObject : public CObject
class DLLEXPORT
ScotAppBusyStatus  : public CObject
{
public:
	ScotAppBusyStatus(LPCTSTR sAppName);
	virtual ~ScotAppBusyStatus();
    void Initialize();
    long GetSystemBusyStatus();
    void SetBusyStatus(long nExpirationMinutes);
    void RenewBusyStatus();
    void ClearBusyStatus();
    bool IsBusy() {return m_bBusy;}

private:
//    CSCOTBusyStatus* m_ApplBusy;
    CString m_sAppName;
    CTime m_LastRenewalTime;
    long m_lLeaseMinutes;
    bool m_bBusy;
};

#endif // !defined(AFX_ScotAppBusyStatus_H__23CA67AC_8EA5_45D3_B135_E97B08F9DD27__INCLUDED_)

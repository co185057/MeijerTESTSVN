// FLTransporter.h: interface for the CFLTransporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLTRANSPORTER_H__BB5291B4_0848_42AC_AF93_1D04258A33E3__INCLUDED_)
#define AFX_FLTRANSPORTER_H__BB5291B4_0848_42AC_AF93_1D04258A33E3__INCLUDED_


#ifndef _CFLTRANSPORTER
#define _CFLTRANSPORTER 
#endif
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFLTransporter  
{
public:
    long TransportFiles();
    CFLTransporter();
    virtual ~CFLTransporter();

	inline CString GetServerSharePoint() const { return m_csServerSharePoint; }
	inline CString GetServerName() const { return m_csServerName; }

protected:
    CString m_csServerSharePoint;
    CString m_csServerName;
    void InitializeTransporter();
    bool m_bInitialized;
	bool m_bTransportThreadActive;
	TClassPtr<CWinThread> m_pTransporterThread;
	static UINT TransporterThreadEntry(LPVOID *lParam);
	void TransporterThread();
};

#endif // !defined(AFX_FLTRANSPORTER_H__BB5291B4_0848_42AC_AF93_1D04258A33E3__INCLUDED_)

// SCOTDevice.h: interface for the CSCOTDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOTDEVICE_H__8039CE84_D291_11D3_BBAD_00A0C9EDD523__INCLUDED_)
#define AFX_SCOTDEVICE_H__8039CE84_D291_11D3_BBAD_00A0C9EDD523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSCOTDevice  
{
public:
  CSCOTDevice() {};
  virtual ~CSCOTDevice() {} ;
	virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID) = 0;
	virtual long UnInitialize() = 0;
  virtual long TranslateOPOSRetToSCOTRet(long rc, long rce = 0) { if (rc == OPOS_SUCCESS) return 0; else return -1; }
  virtual void DataEvent(long Status) {};
  virtual void ErrorEvent(LONG ResultCode, 
                          LONG ResultCodeExtended, 
                          LONG ErrorLocus, 
                          LONG* pErrorResponse) {};
  const CString& GetProfileName() { return m_csProfile; };

protected:
  CWnd*   m_pMainWnd; 
  CString m_csProfile;
};

#endif // !defined(AFX_SCOTDEVICE_H__8039CE84_D291_11D3_BBAD_00A0C9EDD523__INCLUDED_)

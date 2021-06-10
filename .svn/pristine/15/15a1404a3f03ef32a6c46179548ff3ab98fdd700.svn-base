// DynaTrans.h: interface for the CDynaTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DYNATRANS_H__8E5F76AA_A8AC_11D1_90AC_00609761BB2E__INCLUDED_)
#define AFX_DYNATRANS_H__8E5F76AA_A8AC_11D1_90AC_00609761BB2E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "time.h"
#include "tbStateListType.h"

class CDynaTrans  
{
public:
	void UpdateDynaStats(time_t elapsedTime);
	static CString GetHMSString(int numSeconds);
	void DisplayDynaframeStats();
	time_t GetTime();
	BOOL IsEndOfTransaction();
	BOOL IsLaneClosed();
  BOOL IsSeparator();
	BOOL IsLoadState();
  BOOL IsShutdownState();

	int GetIndex();
	int  GetTBState();
	LPCTSTR  GetTBStateName();
  LPCTSTR  GetDateTime();
	LPCTSTR  GetDynaframeName();

	BOOL Initialize(int index, CString& data, CDynaTrans* pPreviousTrans);
	BOOL InitSeparator(CString& desc);

	CDynaTrans();
  ~CDynaTrans();

private:
	TBSLISTTYPE *m_pTBSInfo;

	BOOL m_bTransactionStart;
	BOOL m_bTransactionEnd;
  BOOL m_bLaneClosed;
	BOOL m_bLoadState;
	BOOL m_bIsSeparator;
  BOOL m_bShutdownState;

	time_t m_dfTime;
  CString m_StateName;
  CString* m_pSeparatorName;
  int m_LastTBState;

  int m_Index;
};

#endif // !defined(AFX_DYNATRANS_H__8E5F76AA_A8AC_11D1_90AC_00609761BB2E__INCLUDED_)

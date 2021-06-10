// ScotState.h: interface for the CScotState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOTSTATE_H__5A6ED332_7297_11D2_90DE_00609761BB2E__INCLUDED_)
#define AFX_SCOTSTATE_H__5A6ED332_7297_11D2_90DE_00609761BB2E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CScotState  
{
public:
	virtual BOOL IsTimeInUTC();
	virtual int GetStoreNum();
	virtual int GetTransNum();
	virtual LPCTSTR GetTerminal();
	virtual LPCTSTR GetTypeDesc();
	virtual int GetType();
	virtual LPCTSTR GetStateDesc();
	virtual DWORD GetStateID();
	virtual LPCTSTR GetTimeString();
	virtual COleDateTime& GetTime();
	CScotState();
	virtual ~CScotState();

private:
	COleDateTime m_Time;
};

#endif // !defined(AFX_SCOTSTATE_H__5A6ED332_7297_11D2_90DE_00609761BB2E__INCLUDED_)

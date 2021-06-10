// DataQueue.h: interface for the CDataQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DataQueue_H__D4DD29C0_2482_472B_97E4_4E09B1DC5AA3__INCLUDED_)
#define AFX_DataQueue_H__D4DD29C0_2482_472B_97E4_4E09B1DC5AA3__INCLUDED_

#include "DataMessage.h"
#include <deque>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef std::deque<CDataMessage*> QUEUEPIPE;

class CDataQueue  
{
public:
	HANDLE GetDataEvent();
	size_t Size();
	CDataMessage* Pop();
	BOOL Push( CDataMessage* pMessage);
	BOOL PushFront( CDataMessage* pMessage);
	CDataQueue();
	virtual ~CDataQueue();
  CRITICAL_SECTION m_CriticalSection;
private:
	HANDLE m_hDataAvailable;
	QUEUEPIPE m_Queue;
};

#endif // !defined(AFX_DataQueue_H__D4DD29C0_2482_472B_97E4_4E09B1DC5AA3__INCLUDED_)

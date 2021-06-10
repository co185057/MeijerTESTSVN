// PipeQueue.h: interface for the CPipeQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIPEQUEUE_H__D4DD29C0_2482_472B_97E4_4E09B1DC5AA3__INCLUDED_)
#define AFX_PIPEQUEUE_H__D4DD29C0_2482_472B_97E4_4E09B1DC5AA3__INCLUDED_

#include "PipeMessage.h"
#include <queue>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef std::queue<CPipeMessage*> QUEUEPIPE;

class CPipeQueue  
{
public:
	HANDLE GetDataEvent();
	size_t Size();
	CPipeMessage* Pop();
	BOOL Push( CPipeMessage* pMessage);
	CPipeQueue();
	virtual ~CPipeQueue();
  CRITICAL_SECTION m_CriticalSection;
private:
	HANDLE m_hDataAvailable;
	QUEUEPIPE m_Queue;
};

#endif // !defined(AFX_PIPEQUEUE_H__D4DD29C0_2482_472B_97E4_4E09B1DC5AA3__INCLUDED_)

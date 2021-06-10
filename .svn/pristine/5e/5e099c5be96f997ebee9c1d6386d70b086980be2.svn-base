// DataQueue.cpp: implementation of the CDataQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataQueue.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataQueue::CDataQueue()
{
	m_hDataAvailable = CreateEvent( NULL, TRUE, FALSE, NULL );
  InitializeCriticalSection( &m_CriticalSection);
}

CDataQueue::~CDataQueue()
{
	CDataMessage* pMsg;

	// Clean up any messages on the queue
	while( ! m_Queue.empty() ){
		pMsg = m_Queue.front();
        m_Queue.pop_front();
		delete pMsg;
	}

	// release the handle
	CloseHandle( m_hDataAvailable );
  DeleteCriticalSection( &m_CriticalSection);
}


// Function:  Pop
// Purpose:  Pop a CDataMessage off of the queue
// Inputs:  None
// Outptus:  Pointer to a CDataMessage or NULL if the queue is empty.
// Assumptions:  The routine that pops the message off of the queue is 
//    responsible for the deletion of the message.
// Comments:  None
//
CDataMessage* CDataQueue::Pop()
{
  EnterCriticalSection( &m_CriticalSection);
	CDataMessage* pMsg = NULL;

	if ( ! m_Queue.empty() ) {
		pMsg = m_Queue.front();
        m_Queue.pop_front();
		if ( m_Queue.empty() ) {
			ResetEvent( m_hDataAvailable );
		}
	}
  LeaveCriticalSection( &m_CriticalSection);
	return pMsg;
}

// Function:  Push
// Purpose:  Push a CDataMessage onto the queue
// Inputs:  pMessage - pointer to a message to be added to the queue.  
// Outputs:  Always true
// Assumptions:  Once a message is pushed it is owned by the queue.  The code
//    that pushed the message must not delete it.
// Comments:  None
//
BOOL CDataQueue::Push(CDataMessage *pMessage)
{
  EnterCriticalSection( &m_CriticalSection);
  m_Queue.push_back( pMessage );
  SetEvent( m_hDataAvailable );
  LeaveCriticalSection( &m_CriticalSection);
	return true;
}

// Function:  PushFront
// Purpose:  PushFront a CDataMessage onto the queue
// Inputs:  pMessage - pointer to a message to be added to the queue.  
// Outputs:  Always true
// Assumptions:  Once a message is pushed it is owned by the queue.  The code
//    that pushed the message must not delete it.
// Comments:  None
//
BOOL CDataQueue::PushFront(CDataMessage *pMessage)
{
    EnterCriticalSection( &m_CriticalSection);
    m_Queue.push_front( pMessage );
    // ***** PUSH FRONT DOES NOT SET THE DataAvailable EVENT *****
    //SetEvent( m_hDataAvailable );
    LeaveCriticalSection( &m_CriticalSection);
	return true;
}

// Function: Size
// Purpose:  Returns the number of messages on the queue
// Inputs:  None
// Outputs:  The number of messages on the queue
// Assumptions:  None
// Comments:  None
//
size_t CDataQueue::Size()
{
	return m_Queue.size();
}

// Function:  GetDataEvent
// Purpose:  Returns an event handle that can be used to wait for data to be
//    placed on the queue.
// Inputs:  None
// Outputs:  HANDLE for the data available event.
// Assumptions:  None
// Comments:  None
//
HANDLE CDataQueue::GetDataEvent()
{
	return m_hDataAvailable;
}

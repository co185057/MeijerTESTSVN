// DataMessage.cpp: implementation of the CDataMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PipeServer.h"
#include "DataMessage.h"
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataMessage::CDataMessage()
{
	m_pData = NULL;
	m_dwByteLength = 0;
   m_pConnSource =NULL;
   m_lifeTime.Start();
}

CDataMessage::CDataMessage(CConnectBase *pConnSource, void *pData, DWORD dwByteLength)
:	m_pConnSource(pConnSource),
   m_pData(NULL),
	m_dwByteLength(0)
{
   m_lifeTime.Start();
	SetMessage( pConnSource, pData, dwByteLength );
}


CDataMessage::~CDataMessage()
{
	if ( m_pData != NULL )
	{
		delete[] m_pData;
	}
}

// Function:  GetMessage
// Purpose:  Copies the message data into the buffer provided
// Inputs:  pBuffer - buffer to hold the message
//          dwBufferLength - length of the buffer in bytes
//          dwBytesReturned - number of bytes copied to the buffer.
// Outputs:  TRUE - Success, FALSE - Insufficient buffer.
// Assumptions:  None
// Comments: None
//
BOOL CDataMessage::GetMessage(void *pBuffer, DWORD dwBufferLength, DWORD* dwBytesReturned)
{
	if ( dwBufferLength < m_dwByteLength )
	{
		return false;
	}

	memmove( pBuffer, m_pData, m_dwByteLength );
	*dwBytesReturned = m_dwByteLength;

	return true;
}

// Function:  SetMessage
// Purpose:  Copies the provided message
// Inputs:  pData - Data to be copied
//          dwByteLength - length of the data in bytes
// Outputs: TRUE - Success, FALSE - failed to allocate buffer
// Assumptions:  If the class already contains a message, the existing message
//    will be destroyed and replaced with the new message.
// Comments: None
//
BOOL CDataMessage::SetMessage(CConnectBase *pConnSource, void *pData, DWORD dwByteLength)
{
   m_pConnSource= pConnSource;
	if ( m_pData != NULL )
		delete[] m_pData;
	m_pData = (void*) new BYTE[ dwByteLength ];
	memmove( m_pData, pData, dwByteLength );
	m_dwByteLength = dwByteLength;
	return (m_pData != NULL );
}

// Function:  Size
// Purpose:  Returns the size of the contained message
// Inputs:  None
// Outputs:  Size of the data.
// Assumptions:  None
// Comments:  None
//
DWORD CDataMessage::Size()
{
	return m_dwByteLength;
}

// Function: Detach
// Purpose:  Extracts the message from the class.
// Inputs:  None
// Outputs:  pdwByteLength - loaded with length of the data returned.
//           Returns a pointer to the data.
// Assumptions:  The code calling Detach is responsible for deleting the data
// Comments:  None
//
void* CDataMessage::Detach( DWORD* pdwByteLength )
{
	void* pRet;

	pRet = m_pData;
	m_pData = NULL;
	*pdwByteLength = m_dwByteLength;
	m_dwByteLength = 0;

	return pRet;
}

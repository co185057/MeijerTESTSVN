// PipeMessage.cpp: implementation of the CPipeMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PipeMessage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPipeMessage::CPipeMessage()
{
	m_pData = NULL;
	m_dwByteLength = 0;
}

CPipeMessage::CPipeMessage( void *pData, DWORD dwByteLength)
{
	m_pData = NULL;
	m_dwByteLength = 0;
	SetMessage( pData, dwByteLength );
}


CPipeMessage::~CPipeMessage()
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
BOOL CPipeMessage::GetMessage(void *pBuffer, DWORD dwBufferLength, DWORD* dwBytesReturned)
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
BOOL CPipeMessage::SetMessage(void *pData, DWORD dwByteLength)
{
	if ( m_pData != NULL )
	{
		delete[] m_pData;
		m_pData = NULL;
	}

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
DWORD CPipeMessage::Size()
{
	return m_dwByteLength;
}


// Function:  Attach
// Purpose:  Attaches a message to the class
// Inputs:  pData - data to be attached.
//          dwByteLength - length of the data
// Outputs:  None
// Assumptions:  
//    *  If the class already contains a message, the existing message
//       will be destroyed and replaced with the new message.  
//    *  After an Attach the CPipeMessage class owns the data so it should
//       not be deleted by the code that called Attach.
// Comments:  None
void CPipeMessage::Attach( void* pData, DWORD dwByteLength )
{
	if ( m_pData != NULL )
	{
		delete[] m_pData;
	}

	m_pData = pData;
	m_dwByteLength = dwByteLength;
}

// Function: Detach
// Purpose:  Extracts the message from the class.
// Inputs:  None
// Outputs:  pdwByteLength - loaded with length of the data returned.
//           Returns a pointer to the data.
// Assumptions:  The code calling Detach is responsible for deleting the data
// Comments:  None
//
void* CPipeMessage::Detach( DWORD* pdwByteLength )
{
	void* pRet;

	pRet = m_pData;
	m_pData = NULL;
	*pdwByteLength = m_dwByteLength;
	m_dwByteLength = 0;

	return pRet;
}

// KeyLock.cpp: implementation of the CKeyLock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KeyLock.h"
#include "KeyLockMsg.h"
#include "SocketForTBDevice.h"
#include "TBService.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyLock::CKeyLock(int nPort, int nId,  CmDataCapture& cmDc )
                :CTBDevice(OPOS_CLASSKEY_LOCK,nId,nPort, cmDc)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CKeyLock::CKeyLock") );
    m_hKeyWait = NULL;
    m_lWaitForPosition = -1;
    m_lMaxPositions = 3;
}

CKeyLock::~CKeyLock()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CKeyLock::~CKeyLock") );
}

//
// Function:  Open
// Purpose:  Opens the KeyLock and initializes operations based on the 
//           contents of the registry key for the device profile.
// Inputs:  pService - pointer to CTBService object that handles the service
//                     operations for the device.
//          oRegistry - Registry class for accessing profile properties.
// Outputs:  Return code from Base Class
// Assumptions:  None
// Comments:  None
// 
long CKeyLock::Open( CTBService* pService, COposRegistry& oRegistry )
{
    long lKeyPosition = LOCK_KP_LOCK;
    long lResult;

    lKeyPosition = oRegistry.ReadInt( _T("KeyPosition"), 0, LOCK_KP_LOCK );
    m_lMaxPositions = oRegistry.ReadInt( _T("PositionCount"), 0, 3 );
    pService->InitializeKeyLock( lKeyPosition, m_lMaxPositions );

    // Create the message event
    m_hKeyWait = CreateEvent(NULL, FALSE, FALSE, _T("KeyWait") );
    if ( NULL == m_hKeyWait )
    {
        lResult = OPOS_E_FAILURE;
    }
    else
    {
        lResult = CTBDevice::Open( pService, oRegistry );
    }


	return lResult;
}

// 
// Function: Close
// Purpose:  Frees resources related to an instance of a KeyLock device
// Inputs:  pService - pointer to CTBService object that handles the service
//                     operations for the device.
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CKeyLock::Close( CTBService* pService )
{
    if ( NULL != m_hKeyWait)
    {
        CloseHandle( m_hKeyWait );
    }
	CTBDevice::Close( pService);
}



long CKeyLock::WaitForKeylockChange(long KeyPosition, long Timeout)
{
    m_csBlock.Lock();
    long lCurrentPos = this->m_pTBService->GetPropertyNumber( PIDXLock_KeyPosition);
    long lResult = OPOS_SUCCESS;

    if ( KeyPosition != lCurrentPos )
    {
        m_lWaitForPosition = KeyPosition;
        ResetEvent( this->m_hKeyWait );
        m_csBlock.Unlock();
        DWORD dwWait = WaitForSingleObject( m_hKeyWait, Timeout );
        switch( dwWait )
        {
        case WAIT_OBJECT_0:
            lResult = OPOS_SUCCESS;
            break;
        case WAIT_TIMEOUT:
            lResult = OPOS_E_TIMEOUT;
            break;
        default:
            lResult = OPOS_E_FAILURE;
            break;
        }
    }
    else
    {
        m_csBlock.Unlock();
    }

    return lResult;
}


//
// Function:  ProcessRawData
// Purpose:  Process messages sent from the TB
// Inputs:  pData - buffer containing the message data
//          dwByteLen - the length of the message data
// Outputs:  None
// Assumptions:  the only expected message is the POLL message
// Comments: None
//
void CKeyLock::ProcessRawData( void* pData, DWORD dwByteLen )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CKeyLock::ProcessRawData") );
    if (CSocketForTBDevice::GetSpecVersion() > 1) 
    {
        CKeylockMsg msgKeyLock;
        msgKeyLock.Parse((BYTE*)pData);
        long lNewPosition = msgKeyLock.Position();

        m_csBlock.Lock();
        if ( !this->m_pTBService->SetKeyLockPosition( lNewPosition) )
        {
            // Throw away invalid data from the TB
            m_csBlock.Unlock();
            m_cmDc.DCPrintf(TRACE_ALWAYS, _T("Invalid KeyLock Position received from TB %d"), 
                lNewPosition);
            return;
        }

        this->OnStatus( lNewPosition );
        if ( (m_lWaitForPosition == lNewPosition) ||
             (m_lWaitForPosition == LOCK_KP_ANY) )
        {
            SetEvent( this->m_hKeyWait );
        }
        m_csBlock.Unlock();
    }
    else
    {
        m_cmDc.DCPrintf(TRACE_ALWAYS, _T("Invalid SPEC Version"));
    }
}


// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMEvent.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCMEvent.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
 *
 * Copyright 2004 by NCR Corporation,
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMEvent.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 3     2/28/05 4:19p Dm185016
 * lint
 * 
 * 2     2/18/05 9:20a Dm185016
 * TAR 293708.
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
// 
// 3     11/16/04 11:30a Dm185016
// Added OnAssistMode event
// 
// 2     11/02/04 3:07p Dm185016
// Filled in valid event types
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/

/*lint -save -e1506         */

#include "stdafx.h"
#include "singleton.h"
#include "RCMEvent.h"
#include "RCManager.h"
#include "RemoteConsoleMgrCtrl.h"

using namespace fastlane;

//////////////////////////////////////////////////////////////////

const LONG WMU_FIRE_EVENT = WM_USER + 100;

// CRCMEventQueue::CRCMEventQueue
//
// Purpose: Constructor


/*lint -save -e119      Lint doesn't have up-to-date definition */
CRCMEventQueue::CRCMEventQueue( void )
   : m_sClassName( AfxRegisterWndClass( 0 ) ),
   m_pReceiver( NULL )
{
}
/*lint -restore */

// CRCMEventQueue::~CRCMEventQueue
//
// Purpose: Destructor

/*lint -save -e1551 */
CRCMEventQueue::~CRCMEventQueue( void )
{
    ASSERT( m_qList.size() == 0 );   // Call AbortAll() 

    m_pReceiver = NULL;
}
/*lint -restore */


/*lint -save -e64 -e1025 */
void CRCMEventQueue::BeginService( fastlane::CRCManager*  pReceiver )
{
    ASSERT( pReceiver != NULL );
    CSingleLock csAccess( &m_csAccess, TRUE );
    ASSERT( m_pReceiver == NULL );
    BOOL bResult = CreateEx(
        0,                                  // Extended style.
        m_sClassName,                       // Class name.
        _T("FireEventWindow"),              // Window name.
        0,                                  // Window style,
        0, 0,                               // Initial x,y.
        0, 0,                               // Width, Height.
        NULL,                               // Parent.
        0,                                  // Menu ID.
        0);                                 // Params.
    if ( bResult == FALSE )       
    {
    //     AfxMessageBox( sExText);
    //     AfxThrowOleDispatchException( 0x1234, sExText);
    }
    m_pReceiver = pReceiver;      // Must set up for End Service
}
/*lint -restore */


// CRCMEventQueue::EndService
//              
// Purpose: Make sure the service thread is not running.

void CRCMEventQueue::EndService( void )
{
    ASSERT( m_pReceiver != NULL );
    CSingleLock csAccess( &m_csAccess, TRUE );
    AbortAll();
    DestroyWindow();        //lint !e534    Return Value
    m_pReceiver = NULL;
}

// CRCMEventQueue::AddEvent
//
// Purpose: Add an event at the end of the queue.  
//          Signal the thread to send a message, if necessary.
// Note:    The windows message may be sent and received before we place the 
//          event into the queue, but the critical section keeps the 
//          OnFireEvent from sending it until we put it in the queue. 

void CRCMEventQueue::AddEvent( CRCMEvent* pEvent )
{
    CSingleLock csAccess( &m_csAccess, TRUE );
    ASSERT( pEvent != NULL );
    m_qList.push( pEvent );
    PostMessage( WMU_FIRE_EVENT );      //lint !e534
}


// CRCMEventQueue::AbortAll
//
// Purpose: Discard all queued events. This occurs when the SO claim is release,
//          and when the queue is destroyed. 

void CRCMEventQueue::AbortAll(void )
{
    CSingleLock csAccess( &m_csAccess, TRUE );

    while (!m_qList.empty())
    {
        CRCMEvent* pEvent = m_qList.front( );
        ASSERT( pEvent != NULL );
        delete pEvent;
        m_qList.pop();
    }
  return;
}


/////////////////////////////////////////////////////////////////////////////
// CRCMEventQueue Window Information

BEGIN_MESSAGE_MAP(CRCMEventQueue, CWnd)
    //{{AFX_MSG_MAP(CRCMEventQueue)
        // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
    ON_MESSAGE(WMU_FIRE_EVENT, OnFireEvent)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRCMEventQueue message handlers

// CRCMEventQueue::OnFireEvent
// Validate that events may be fired, and if the specific event may be fired.
// Data and Input error events require m_bEnableData to be TRUE 
// Note:    1) Can't DataCap Entry/Exit Here using standard mechanism
//          Because the DataCap will be deleted in the final event
//          that is handled. Therefore, we trace entry, but not exit!
//          2) The semAuto semaphore applies to all SECURITY
//          service objects in the application ( using this framework )
//          It keeps service objects from re-entering the application
//          when the application handles an event by putting up a 
//          dialog or message box.  Windows continues to call this 
//          function from the dialog box procedure and will force 
//          re-entry into the application code. We stop this by checking
//          this flag.  We do not care which service object has entered
//          the code, just that one has.  All service objects retry
//          periodically to dispatch the event. See the thread code above!

afx_msg LONG CRCMEventQueue::OnFireEvent( UINT , LONG )
{
    if ( m_pReceiver == NULL)
        return 0;

    CSingleLock csAccess( &m_csAccess, TRUE );
    // If no events or events not legal then return without firing.
    if ( m_qList.empty() == FALSE )
    {
        CRCMEvent* pEvent = m_qList.front( );
        m_qList.pop();
        ASSERT( pEvent != NULL );        // Check the first event
        csAccess.Unlock();               //lint !e534
        pEvent->Dispatch( *m_pReceiver );              
        delete pEvent;                
    }
   return 0;
}


CAssistModeEvent::CAssistModeEvent( const StringType &szParms )
{
    SetParms(szParms);
}


void CAssistModeEvent::Dispatch( fastlane::CRCManager& target )
{
    (*target.GetControl()).OnAssistMode( GetParms() );  
}


CRemoteConsoleEvent::CRemoteConsoleEvent( const StringType &szParms )
{
    SetParms(szParms);
}


void CRemoteConsoleEvent::Dispatch( fastlane::CRCManager& target )
{
    (*target.GetControl()).OnRemoteConsoleEvent( GetParms() );  
}


CUnsolicitedDataEvent::CUnsolicitedDataEvent( const StringType &szParms )
{
    SetParms(szParms);
}


void CUnsolicitedDataEvent::Dispatch( fastlane::CRCManager& target )
{
    (*target.GetControl()).OnUnsolicitedData( GetParms() );  
}


CRequestApprovalEvent::CRequestApprovalEvent( const StringType &szParms )
{
    SetParms(szParms);
}


void CRequestApprovalEvent::Dispatch( fastlane::CRCManager& target )
{
    (*target.GetControl()).OnRequestApproval( GetParms() );  
}


CRequestSuspendEvent::CRequestSuspendEvent( const StringType &szParms )
{
    SetParms(szParms);
}


void CRequestSuspendEvent::Dispatch( fastlane::CRCManager& target )
{
    (*target.GetControl()).OnRequestSuspend( GetParms() );  
}


CCancelSuspendEvent::CCancelSuspendEvent( const StringType &szParms )
{
    SetParms(szParms);
}


void CCancelSuspendEvent::Dispatch( fastlane::CRCManager& target )
{
    (*target.GetControl()).OnCancelSuspend( GetParms() );  
}

CValidateOperatorEvent::CValidateOperatorEvent( const StringType &szParms )
{
    SetParms(szParms);
}


void CValidateOperatorEvent::Dispatch( fastlane::CRCManager& target )
{
    (*target.GetControl()).OnValidateOperator( GetParms() );  
}
/*lint -restore */
#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMEvent.h 3     10/21/08 4:09p Sm133076 $
/*
* @(#)RCMEvent.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
*
* Copyright (c) 2004 NCR.  All rights reserved.
*
*/
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMEvent.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 4     5/17/05 1:46p Dm185016
 * Changes for performance measurements.
 * 
 * 2     2/18/05 9:20a Dm185016
 * TAR 293708.
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
* 
* 3     11/16/04 11:30a Dm185016
* Added OnAssistMode event
* 
* 2     11/02/04 3:07p Dm185016
* Filled in valid event types
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/

#include <queue>
#include "RCMApi.h"

namespace fastlane
{

class CRCManager;

// Classes declared in this file.


// Abstract Base Class for Events
class RCMANAGER_API CRCMEvent
    : virtual public CBaseObject
{
public:
    CRCMEvent(  ) { }
    virtual ~CRCMEvent() {}
    virtual void Dispatch( fastlane::CRCManager& pTarget)   = 0;  
    virtual const _TCHAR *GetParms() const { return m_szParms.c_str(); }

protected:

    virtual void SetParms(const StringType &szParms) { m_szParms = szParms; }

private:

    CRCMEvent( const CRCMEvent& );             // Disable Copies
    CRCMEvent& operator=( const CRCMEvent& );  // Disable Copies

    StringType m_szParms;
};


// Event class corresponding to OnItemOK event
class RCMANAGER_API CValidateOperatorEvent : public CRCMEvent
{
public:
    CValidateOperatorEvent( const StringType &szParms );
    virtual void Dispatch( fastlane::CRCManager& pTarget );
private:
    CValidateOperatorEvent( );
    CValidateOperatorEvent( const CValidateOperatorEvent& );             // Disable Copies
    CValidateOperatorEvent& operator=( const CValidateOperatorEvent& );  // Disable Copies
};


// Event class corresponding to OnItemOK event
class RCMANAGER_API CCancelSuspendEvent : public CRCMEvent
{
public:
    CCancelSuspendEvent( const StringType &szParms );
    virtual void Dispatch( fastlane::CRCManager& pTarget );
private:
    CCancelSuspendEvent( );
    CCancelSuspendEvent( const CCancelSuspendEvent& );             // Disable Copies
    CCancelSuspendEvent& operator=( const CCancelSuspendEvent& );  // Disable Copies
};


// Event class corresponding to OnItemOK event
class RCMANAGER_API CRequestSuspendEvent : public CRCMEvent
{
public:
    CRequestSuspendEvent( const StringType &szParms );
    virtual void Dispatch( fastlane::CRCManager& pTarget );
private:
    CRequestSuspendEvent( );
    CRequestSuspendEvent( const CRequestSuspendEvent& );             // Disable Copies
    CRequestSuspendEvent& operator=( const CRequestSuspendEvent& );  // Disable Copies
};


// Event class corresponding to OnItemOK event
class RCMANAGER_API CRequestApprovalEvent : public CRCMEvent
{
public:
    CRequestApprovalEvent( const StringType &szParms );
    virtual void Dispatch( fastlane::CRCManager& pTarget );
private:
    CRequestApprovalEvent( );
    CRequestApprovalEvent( const CRequestApprovalEvent& );             // Disable Copies
    CRequestApprovalEvent& operator=( const CRequestApprovalEvent& );  // Disable Copies
};


// Event class corresponding to OnItemOK event
class RCMANAGER_API CUnsolicitedDataEvent : public CRCMEvent
{
public:
    CUnsolicitedDataEvent( const StringType &szParms );
    virtual void Dispatch( fastlane::CRCManager& pTarget );
private:
    CUnsolicitedDataEvent( );
    CUnsolicitedDataEvent( const CUnsolicitedDataEvent& );             // Disable Copies
    CUnsolicitedDataEvent& operator=( const CUnsolicitedDataEvent& );  // Disable Copies
};


// Event class corresponding to OnAssistMode event
class RCMANAGER_API CAssistModeEvent : public CRCMEvent
{
public:
    CAssistModeEvent( const StringType &szParms );
    virtual void Dispatch( fastlane::CRCManager& pTarget );
private:
    CAssistModeEvent( );
    CAssistModeEvent( const CAssistModeEvent& );             // Disable Copies
    CAssistModeEvent& operator=( const CAssistModeEvent& );  // Disable Copies
};


// Event class corresponding to OnRemoteConsole event
class RCMANAGER_API CRemoteConsoleEvent : public CRCMEvent
{
public:
    CRemoteConsoleEvent( const StringType &szParms );
    virtual void Dispatch( fastlane::CRCManager& pTarget );
private:
    CRemoteConsoleEvent( );
    CRemoteConsoleEvent( const CRemoteConsoleEvent& );             // Disable Copies
    CRemoteConsoleEvent& operator=( const CRemoteConsoleEvent& );  // Disable Copies
};


// Event Queue Mechanism 
class CRCMEventQueue : private CWnd
{
public:

    CRCMEventQueue( void );
    ~CRCMEventQueue( void );
    void AddEvent( CRCMEvent* pEvent );
    void AbortAll( void );
    void BeginService( CRCManager*  pReceiver );
    void EndService(   void );

private:

    std::queue < CRCMEvent* > m_qList;
    CCriticalSection  m_csAccess;       // Guard Access to m_qList
    CString m_sClassName;
    CRCManager* m_pReceiver;
    // Generated message map functions
    //{{AFX_MSG(CRCMEventQueue)
        // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    long OnFireEvent( UINT wParam, LONG lParam );
private:
    CRCMEventQueue( const CRCMEventQueue& );    // Disable Copies
    CRCMEventQueue& operator=( const CRCMEventQueue& );

};

}

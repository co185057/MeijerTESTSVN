#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCManager.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCManager.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCManager.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 8     3/30/05 2:37p Dm185016
 * TAR 297008
 * 
 * 7     3/14/05 8:14p Dm185016
 * Added API statistics.
 * 
 * 6     3/11/05 1:08p Dm185016
 * Factored out RAP specific API features
 * 
 * 5     3/07/05 6:55p Dm185016
 * TAR 294609.
 * TAR 294610.
 * 
 * 4     2/28/05 4:22p Dm185016
 * lint
 * 
 * 3     2/15/05 11:29a Dm185016
 * TAR 293322.
 * 
 * 2     2/07/05 7:17p Dm185016
 * TAR 292647
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
* 
* 9     1/12/05 5:55p Dm185016
* Multiple RAP support.
* 
* 8     12/22/04 11:07a Dm185016
* HH Support.  Factored out remote data into new class CRemoteConnection.
* 
* 7     12/14/04 11:23a Dm185016
* Lane number no longer needed
* 
* 6     12/02/04 3:42p Dm185016
* 
* 5     11/16/04 11:34a Dm185016
* Input event made synchronous
* 
* 4     11/02/04 3:06p Dm185016
* Misspelled name of control
* 
* 3     10/29/04 9:01a Dm185016
* Added error checks for APIs.
* Split out RAP specific function.
* 
* 2     10/14/04 1:38p Dm185016
* New OnInputMethod Api.
* PSX now passed as handle.
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/

/*lint -save -e1725 -e763 */

#include "singleton.h"
#include "RCMApi.h"
#include "RCMEvent.h"
#include "SMConstants.h"
#include "RemoteConnection.h"
#include "StateMApi.h"
#include <set>
#include <list>

#define INVALID_LANE        -1


namespace STATE_MACHINE_NAMESPACE
{
    class IMessage;
}


using namespace STATE_MACHINE_NAMESPACE;

class CRemoteConsoleMgrCtrl;
class CXMLErrorInfo;
class CSMError;
class CConfigManager;
class CPSX;

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace fastlane
{

class CRCMStateMachine;
class CRCMRecorder;


class RCMANAGER_API CRCManager
    : public CSingleton<CRCManager>
    , virtual public CBaseObject
{

public:

    friend class CSingleton<CRCManager>;

    virtual ~CRCManager();

    CPSX &psx() { return *m_pPSX; }

    virtual CRCMStateMachine *GetStateMachine();
    
    virtual CConfigManager *GetConfigMgr();

    CRemoteConsoleMgrCtrl *GetControl() const;

    void SetControl(CRemoteConsoleMgrCtrl *);

    virtual CXMLErrorInfo *GetErrorInfo(void);

    void FireEvent(CRCMEvent *);

    virtual HRESULT LoadOptions();

//	virtual void SetHandHeld(bool bHandHeld);
	virtual bool IsHandHeld() const throw(...);
	virtual bool IsHandHeld(const _TCHAR *) const throw(...);

	virtual void SetCurrentContext(const _TCHAR *);
	virtual void SetCurrentContext(const _TCHAR *, const _TCHAR *);
	virtual const _TCHAR *GetCurrentContext() const throw(...);
	virtual const _TCHAR *GetCurrentContext(const _TCHAR *) const throw(...);

	virtual const _TCHAR *GetRemoteMachineName() const throw(...);
	virtual const _TCHAR *GetRemoteMachineName(const _TCHAR *) const throw(...);
    virtual const _TCHAR *GetMachineName() const;

	virtual const _TCHAR *GetRemoteConnectionName() const throw(...);
	virtual const _TCHAR *GetRemoteConnectionName(const _TCHAR *) const throw(...);

    virtual void AddRemoteConnection(PREMOTECONN pRConn);
    virtual void RemoveRemoteConnection(const _TCHAR *pszName);
    virtual PREMOTECONN GetRemoteConnection(const _TCHAR *) const;
    virtual PREMOTECONN GetCurrentRemoteConnection() const;
    virtual void SetCurrentRemoteConnection(PREMOTECONN);
    virtual const CRemoteConnectionsMap &GetRemoteConnections();
    virtual bool IsRemoteConnectionAvailable() const;

	void SetExclusiveAccessGranted(bool);
	bool IsExclusiveAccessGranted() const;
	void SetExclusiveAccessReleaseRequired(bool);
	bool IsExclusiveAccessReleaseRequired() const;

public:
    
    virtual LONG Initialize(LONG lPSXHandle, HTRACE hTrace);
    virtual LONG Terminate(void);
    virtual LONG OnControl(LPCTSTR bstrInParms, BSTR* pbstrOutParms);
    virtual LONG OnEvent(LPCTSTR bstrInParms);
    virtual LONG Get(LPCTSTR bstrInParms, BSTR* pbstrOutParms);
    virtual LONG Set(LPCTSTR bstrInParms);
    virtual LONG OnInputEvent(LPCTSTR bstrInParms);

    // TODO:  Need to add this as a property to the control
    virtual BSTR GetErrorText(void);

protected:

    CRCManager();                                     // Hidden constructor

    virtual void BuildStateMachine();
    
    virtual CRCMStateMachine *GetStateMachine(const _TCHAR *);

    CRCMRecorder      &recorder() { return *m_pRecorder; }
    CRemoteConnection &remoteConnection(const _TCHAR *) const throw(...);
    CRemoteConnection &currentRemoteConnection() const throw(...);

    HRESULT SendMsg(IMessage **, const _TCHAR *, const void *pData=NULL, size_t nDataLength=0);
    HRESULT PostMsg(const _TCHAR *, const void *pData=NULL, size_t nDataLength=0);

    bool                    m_bStateMachineReady;

private:

    CRCManager(const CRCManager &);             // Hidden Copy construcor
    CRCManager &operator =(const CRCManager &); // Hidden assignment operator

private:

    CRCMStateMachine   *m_pStateMachine;
    CSMError           *m_pXMLError;
    CConfigManager         *m_pConfigMgr;

    CPSX               *m_pPSX;

    CRCMEventQueue      m_qEvents;

    mutable CRemoteConsoleMgrCtrl   *m_pControl;

    CRCMRecorder      *m_pRecorder;

    bool                    m_bServiceAvailable;
    bool                    m_bExclusiveAccessGranted;
    bool                    m_bExclusiveAccessReleaseRequired;

    unsigned                m_ulExceptionCount;

    StringType              m_sComputerName;
    PREMOTECONN             m_pCurrentRemoteConnection;

    HRESULT                 m_hrSMFailureReason;

    CRemoteConnectionsMap   m_RemoteConnections;

    DEFINE_API_STATS(Initialize)
    DEFINE_API_STATS(Terminate)
    DEFINE_API_STATS(OnEvent)
    DEFINE_API_STATS(OnControl)
    DEFINE_API_STATS(Get)
    DEFINE_API_STATS(Set)
    DEFINE_API_STATS(OnInputEvent)
};

}

#pragma warning( pop )

/*lint -restore */

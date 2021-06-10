#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RemoteConnection.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RemoteConnection.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RemoteConnection.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 6     3/30/05 5:05p Dm185016
 * TAR 297001
 * 
 * 5     3/15/05 3:35p Dm185016
 * TAR 295676 - AcceptConnection is now handled by a separate thread to
 * prevent hanging the state machine thread.
 * 
 * 4     3/07/05 6:56p Dm185016
 * Added ToString() method.
 * 
 * 3     2/28/05 4:23p Dm185016
 * lint
 * 
 * 2     2/07/05 7:17p Dm185016
 * TAR 292647
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
* 
* 2     1/12/05 5:56p Dm185016
* Multiple RAP support.  Removed starting context as no longer needed.
* 
* 1     12/22/04 11:08a Dm185016
* HH support.  Contains data describing the remote connection.
*/

#include "SMConstants.h"
#include "BaseObject.h"
#include "KeyValueHandler.h"
//#include <hash_map>
#include <map>


namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;


namespace fastlane
{

typedef long CRemoteConnectionKeyType;

class CRemoteConnection
    : public CBaseObject
{

public:

    static CRemoteConnection *CreateRemoteConnection(const IMessage *);

    static StringType GetRemoteConnectionName(const IMessage *);

    virtual CRemoteConnectionKeyType GetId();

	virtual void SetCurrentContext(const _TCHAR *);
	virtual const _TCHAR *GetCurrentContext() const;

	virtual void SetEnabled(bool);
	virtual bool IsEnabled() const;

    const _TCHAR *GetRemoteConnectionName() const;
    const _TCHAR *GetMachineName() const;
    //const _TCHAR *GetStartContext() const;
    bool IsHandheld() const;

    bool operator!=(const CRemoteConnection& rhs) const;

    virtual StringType ToString() throw();

protected:

    CRemoteConnection();

private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(RCParms)
        DECLARE_KV_CSTRING(RemoteConnectionName)
        //DECLARE_KV_CSTRING(StartContext);
        DECLARE_KV_BOOL(Handheld)
    DECLARE_KV_HANDLER_CLASS_END

    static void ParseMsg(RCParms &, const IMessage *);

    StringType  m_sRemoteConnectionName;
    StringType  m_sMachineName;
    //StringType  m_sStartContext;
    StringType  m_sCurrentContext;

    _TCHAR *                m_pszMachineName;

    bool m_bHandheld;

    bool m_bEnabled;

    CRemoteConnectionKeyType m_Id;

    static CRemoteConnectionKeyType m_lCount;
};


typedef CRemoteConnection *PREMOTECONN;

typedef map<StringType, PREMOTECONN, StringComparator> CRemoteConnectionsMap;

}
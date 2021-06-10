#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/GetCommand.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)GetCommand.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/GetCommand.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     4/05/05 11:59a Dm185016
 * TAR 297356
 * 
 * 3     3/30/05 2:37p Dm185016
 * TAR 297008
 * 
 * 2     3/03/05 3:16p Dm185016
 * TAR 291700 - Cleaned this up.  Added new Gets for
 * remote-connection-handheld and remote-connection-name.
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
* 
* 3     10/29/04 10:05a Dm185016
* Fixed compile error.
* 
* 2     10/29/04 9:11a Dm185016
* Filled in fxn.
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/

#include "SMConstants.h"
#include "Createable.h"
#include "KeyValueHandler.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "Pool.h"


#define GET_ID_REQUEST_ENABLE_AM            1
#define GET_ID_CURRENT_APPROVAL_CODE        2
#define GET_ID_RAP_DATA_AVAILABLE           3
#define GET_ID_RAP_DATA                     4
#define GET_ID_PREVIOUS_VIEW                5
#define GET_ID_RAP_DATA_PENDING_COUNT       6
#define GET_ID_CONNECTION_MODE              7
#define GET_ID_REMOTE_CONNECTION_HANDHELD   8
#define GET_ID_REMOTE_CONNECTION_NAME       9
#define GET_ID_REQUEST_ENABLE_HH_AM         10


namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CGetCommand
    : virtual public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CGetCommand, IAction>
//    , public CPool<CGetCommand>
{

public:

    CGetCommand();
	virtual ~CGetCommand();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    virtual HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(GCParms)
        DECLARE_KV_CSTRING(Id)
        DECLARE_KV_CSTRING(LookupTableName)
        DECLARE_KV_CSTRING(RemoteConnectionName)
    DECLARE_KV_HANDLER_CLASS_END

    GCParms m_parms;

    virtual HRESULT get_id_request_enable_am(CKeyValueParmList &);
    virtual HRESULT get_id_current_approval_code(CKeyValueParmList &);
    virtual HRESULT get_id_rap_data_available(CKeyValueParmList &);
    virtual HRESULT get_id_rap_data(CKeyValueParmList &);
    virtual HRESULT get_id_previous_view(CKeyValueParmList &);
    virtual HRESULT get_id_rap_data_pending_count(CKeyValueParmList &);
    virtual HRESULT get_id_connection_mode(CKeyValueParmList &);
    virtual HRESULT get_id_remote_connection_handheld(CKeyValueParmList &);
    virtual HRESULT get_id_remote_connection_name(CKeyValueParmList &);
    virtual HRESULT get_id_request_enable_hh_am(CKeyValueParmList &);

private:

    // hide copy and assignment
	CGetCommand(const CGetCommand&);
	CGetCommand& operator = (const CGetCommand &);
};

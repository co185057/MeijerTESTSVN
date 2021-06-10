#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchContextCommand.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SwitchContextCommand.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchContextCommand.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/17/05 5:52p Dm185016
 * Now ignoring context switch if the remote connection name is not the
 * same as the current remote connection.
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
* 
* 3     1/12/05 5:37p Dm185016
* Cleaned up trace logs.
* 
* 2     12/07/04 4:18p Dm185016
* Assist mode contexts no longer loaded in LaneRAP.xml
* 
* 1     11/16/04 11:23a Dm185016
*/

#include "rcmapi.h"
#include "SMConstants.h"
#include "Createable.h"
#include "KVParmList.h"
#include "KeyValueHandler.h"
#include "RCMKVActionBase.h"
#include "ActionBaseRAP.h"
#include "Pool.h"
#include "Message.h"

using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

namespace fastlane
{

class CContextSwitch;
class CUnknownContextSwitch;
class CLocalContextSwitch;

class RCMANAGER_API CSwitchContextCommand
    : virtual public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CSwitchContextCommand, IAction>
//    , public CPool<CSwitchContextCommand>
{

public:
	CSwitchContextCommand();
	virtual ~CSwitchContextCommand();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    virtual void ExitCurrentState();
    virtual void EnterNewState();

    CContextSwitch &GetContextSwitch() const throw();

    CContextSwitch *m_pContextSwitch;

private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SCCParms)
        DECLARE_KV_CSTRING(ContextName);
        DECLARE_KV_CSTRING(RemoteConnectionName);
    DECLARE_KV_HANDLER_CLASS_END

    SCCParms m_parms;

private:

    // hide copy and assignment
	CSwitchContextCommand(const CSwitchContextCommand&);
	CSwitchContextCommand& operator = (const CSwitchContextCommand &);

    CUnknownContextSwitch *m_pUnknownCS;
    CLocalContextSwitch *m_pLocalCS;
};

}

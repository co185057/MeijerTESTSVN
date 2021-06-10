#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/MediaCommand.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)MediaCommand.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/MediaCommand.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     3/22/05 4:07p Dm185016
 * UNICODE
 * 
 * 1     3/11/05 2:44p Dm185016
 * 
 * 2     2/28/05 4:38p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
* 
* 3     12/07/04 4:22p Dm185016
* Fixed for lane status window
* 
* 2     11/30/04 4:30p Dm185016
* Fixed receipt source for media messages.
* 
* 1     10/29/04 9:04a Dm185016
*/

/*lint -save -e1516 */

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "PSXAction.h"
#include "KeyValueHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;


class CMediaCommand
    : public CPSXAction
    , public CCreateable<CMediaCommand, IAction>
//    , public CPool<CMediaCommand>
{


public:

    CMediaCommand();
	virtual ~CMediaCommand();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    ///<summary>Obtains all the controls names from the xml file.</summary>
    HRESULT ParseOperands();
    HRESULT ProcessMediaOutput(const _TCHAR *pMsgData);

// This section contains the handlers for the various K,V pairs
private:


    DECLARE_KV_HANDLER_CLASS_BEGIN(MCParms)
        DECLARE_KV_CSTRING(DataSource)
    DECLARE_KV_HANDLER_CLASS_END

private:

    // hide copy and assignment
	CMediaCommand(const CMediaCommand&);
	CMediaCommand& operator = (const CMediaCommand &);

    MCParms m_parms;
};

/*lint -restore */
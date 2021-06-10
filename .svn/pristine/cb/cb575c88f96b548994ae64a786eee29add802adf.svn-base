#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/RCMActionBase.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCMActionBase.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/RCMActionBase.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     4/05/05 12:00p Dm185016
 * TAR 297356
 * 
 * 2     2/28/05 4:40p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
* 
* 4     12/07/04 4:23p Dm185016
* Made LoadableActionBase a friend of this class
* 
* 3     11/16/04 11:41a Dm185016
* made base class virtual public
* 
* 2     10/29/04 9:13a Dm185016
* Split RCManager.
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/

//lint -e763        Redundant declaration
#include "rcmapi.h"
#include "actions/ActionBase.h"
#include "Configuration.h"
#include "KVParmList.h"
//#include "RCMKeyValue.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CKeyValueParmList;
class CPSX;

namespace fastlane
{

class CRCMStateMachine;
class CRCManager;

class RCMANAGER_API CRCMActionBase
    : virtual public CActionBase
{

public:

    friend class CLoadableActionBase;

    CRCMActionBase(){};
    virtual ~CRCMActionBase(){};

public:

    static CRCManager &rcm();
    static CPSX &psx();
    CKeyValueParmList GetKeyValueParmList(const IMessage &) const;

protected:

    CRCMStateMachine &rcmStateMachine();

private:

    // hide copy and assignment
	CRCMActionBase(const CRCMActionBase&);
	CRCMActionBase& operator = (const CRCMActionBase &);
};

}

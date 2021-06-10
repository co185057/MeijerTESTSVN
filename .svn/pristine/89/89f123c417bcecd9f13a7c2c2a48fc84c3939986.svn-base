// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/RCMActionBase.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCMActionBase.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/RCMActionBase.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:40p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 3     10/29/04 10:06a Dm185016
// Fixed compiler error.
// 
// 2     10/29/04 9:13a Dm185016
// Split RCManager.
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
/*lint -save -e1774     Don't want to use dynamic_cast */
#include "StdAfx.h"
#include "RCMActionBase.h"
#include "RCMStateMachine.h"
#include "RCManager.h"
#include "KVParmList.h"
#include "Message.h"

using namespace fastlane;


CRCManager &CRCMActionBase::rcm() { return *CSingleton<CRCManager>::instance(); }


CPSX &CRCMActionBase::psx() { return rcm().psx(); }


CRCMStateMachine &CRCMActionBase::rcmStateMachine() { return dynamic_cast<CRCMStateMachine &>(stateMachine()); }


CKeyValueParmList CRCMActionBase::GetKeyValueParmList(const IMessage &message) const 
    { return CKeyValueParmList((const _TCHAR *)message.GetData()); }

/*lint -restore */
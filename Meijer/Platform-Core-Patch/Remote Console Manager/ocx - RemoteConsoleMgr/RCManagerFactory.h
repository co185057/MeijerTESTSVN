#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/ocx - RemoteConsoleMgr/RCManagerFactory.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)RCMManagerFactory.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/ocx - RemoteConsoleMgr/RCManagerFactory.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 1     1/26/05 2:15p Dm185016
 * Moved to new Repository.
* 
* 1     10/04/04 5:35p Dm185016
*/
namespace fastlane
{

class CRCManager;

class CRCManagerFactory
{
public:
    
    static CRCManager *GetRCManagerInstance();

private:
    
    static CRCManager *m_pRCManager;
};

}
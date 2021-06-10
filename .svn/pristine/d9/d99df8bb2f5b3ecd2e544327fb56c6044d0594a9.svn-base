#pragma once
/*
 * @(#)Library.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Library.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 5     1/18/05 3:55p Dm185016
* UNICODE support
* 
* 3     5/19/04 1:33p Dm185016
* lint
* 
* 2     3/19/04 3:02p Dm185016
* Added release function
* 
* 1     1/27/04 1:54p Dm185016
* New Library support for DLLs containing Actions
*/

#include <map>
#include "StateMachineObject.h"
#include "statem.h"

using namespace std;

namespace STATE_MACHINE_NAMESPACE
{

class STATEM_API ILibrary
    : public IStateMachineObject
{
public:

    virtual ~ILibrary(void) {};

    virtual const _TCHAR *GetName()=0;

    virtual FARPROC FindFunction(LPCTSTR pcsFxnName)=0;

    virtual FARPROC GetCreateFunction(LPCTSTR pcsFxnName)=0;

    virtual FARPROC GetReleaseFunction(LPCTSTR pcsFxnName)=0;
};

typedef ILibrary *PLIBRARY;

typedef map<StringType, PLIBRARY, StringComparator> CLibrarysMap;

}
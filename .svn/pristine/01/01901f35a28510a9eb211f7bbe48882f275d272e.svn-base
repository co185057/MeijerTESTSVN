#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ReleaseFullScreen.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)ReleaseFullScreen.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ReleaseFullScreen.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
* 
* 1     1/12/05 5:21p Dm185016
* Multiple RAP support.
*/

#include "SMConstants.h"
#include "Createable.h"
#include "GenerateEvent.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CReleaseFullScreen
    : public CGenerateEvent
    , public CCreateable<CReleaseFullScreen, IAction>
//    , public CPool<CReleaseFullScreen>
{

public:

    CReleaseFullScreen();
	virtual ~CReleaseFullScreen();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    virtual const _TCHAR *GetEventText();

private:

    // hide copy and assignment
	CReleaseFullScreen(const CReleaseFullScreen&);
	CReleaseFullScreen& operator = (const CReleaseFullScreen &);

    StringType m_sEventText;
};

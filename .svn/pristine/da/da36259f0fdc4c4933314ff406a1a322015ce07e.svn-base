#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/GenerateEvent.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)GenerateEvent.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/GenerateEvent.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
* 
* 1     1/12/05 6:01p Dm185016
* Multiple RAP support.
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CGenerateEvent
    : public CRCMActionBase
    , public CCreateable<CGenerateEvent, IAction>
//    , public CPool<CGenerateEvent>
{

public:

    CGenerateEvent();
	virtual ~CGenerateEvent();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    ///<summary>Obtains all the controls names from the xml file.</summary>
    virtual HRESULT ParseOperands();

    virtual const _TCHAR *GetEventText();
    
    virtual HRESULT GenerateEvent();

private:

    // hide copy and assignment
	CGenerateEvent(const CGenerateEvent&);
	CGenerateEvent& operator = (const CGenerateEvent &);

    StringType m_sParameters;
};

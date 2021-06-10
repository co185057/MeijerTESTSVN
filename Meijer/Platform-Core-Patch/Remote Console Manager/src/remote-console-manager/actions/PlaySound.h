#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/PlaySound.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)PlaySound.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/PlaySound.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     5/09/05 4:40p Dm185016
 * Added sound support for RAP interventions
*/
#include "Createable.h"
#include "PSXAction.h"
#include "KeyValueHandler.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CPlaySound
    : public CPSXAction
    , public CCreateable<CPlaySound, IAction>
//    , public CPool<CPlaySound>
{

public:

	CPlaySound();
	virtual ~CPlaySound();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

    virtual HRESULT GetSoundName(StringType &);
    virtual StringType TranslateSoundName(const _TCHAR *);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(PSParms)
        DECLARE_KV_CSTRING(LookupTableName)
        DECLARE_KV_CSTRING(SoundName);
    DECLARE_KV_HANDLER_CLASS_END

    PSParms m_parms;

private:

    // hide copy and assignment
	CPlaySound(const CPlaySound&);
	CPlaySound& operator = (const CPlaySound &);
};

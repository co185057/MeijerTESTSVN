#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/PlayInterventionSound.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)PlayInterventionSound.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/PlayInterventionSound.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     5/09/05 4:39p Dm185016
 * Added sound support for RAP interventions
*/
#include "Createable.h"
#include "PlaySound.h"
#include "KeyValueHandler.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CPlayInterventionSound
    : public CPlaySound
    , public CCreateable<CPlayInterventionSound, IAction>
//    , public CPool<CPlayInterventionSound>
{

public:

	CPlayInterventionSound();
	virtual ~CPlayInterventionSound();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    virtual HRESULT GetSoundName(StringType &);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(PISParms)
        DECLARE_KV_LONG(Green);
        DECLARE_KV_LONG(Yellow);
        DECLARE_KV_LONG(Red);
    DECLARE_KV_HANDLER_CLASS_END

    PISParms m_parms;

private:

    // hide copy and assignment
	CPlayInterventionSound(const CPlayInterventionSound&);
	CPlayInterventionSound& operator = (const CPlayInterventionSound &);
};

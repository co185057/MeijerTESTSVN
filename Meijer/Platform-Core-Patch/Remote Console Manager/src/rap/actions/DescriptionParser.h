#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DescriptionParser.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DescriptionParser.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DescriptionParser.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     1/13/06 2:28p Dm185016
 * TAR 301329
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
* 
* 4     1/12/05 5:50p Dm185016
* TAR 289897
* 
* 3     12/22/04 10:32a Dm185016
* TAR 288934 - Added override to force updatig the buttons.
* 
* 2     12/14/04 11:18a Dm185016
* TAR 288478.  Changes to correctly display text on buttons
* 
* 1     11/30/04 4:18p Dm185016
*/

#include "PSXAction.h"
#include "KeyValueHandler.h"

class CPSX;

namespace rap
{

class CRemoteAPMgr;

class CDescriptionParser
    : virtual public CPSXAction
{

public:

    CDescriptionParser() {};
    virtual ~CDescriptionParser() {};

protected:

    ///<summary>Obtains all the controls names from the xml file.</summary>
    virtual HRESULT ParseDescription();
    virtual HRESULT UpdateDescription(bool bImmediate = false, bool bShowDescription = true);

    virtual const _TCHAR *GetDescription() const=0;
    virtual const _TCHAR *GetInstructionTextVariableName() const=0;
    virtual const _TCHAR *GetLaneButtonTextControlName() const=0;
    virtual const _TCHAR *GetLaneStatus1ControlName() const=0;
    virtual const _TCHAR *GetLaneStatus2ControlName() const=0;
    virtual const _TCHAR *GetLaneStatus3ControlName() const=0;

protected:

    static CPSX &psx();
    static CRemoteAPMgr &rapmgr();

    HRESULT SetText(const _TCHAR *pszControlName, const _TCHAR *pszText);
    
    HRESULT SetTransactionVariable(const _TCHAR *pszVarName, const _TCHAR *pszText);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(DescriptionParms)
        DECLARE_KV_CSTRING(Description);
        DECLARE_KV_CSTRING(LaneButtonText);
        DECLARE_KV_CSTRING(LaneStatusLine1);
        DECLARE_KV_CSTRING(LaneStatusLine2);
        DECLARE_KV_CSTRING(LaneStatusLine3);
    DECLARE_KV_HANDLER_CLASS_END

    DescriptionParms m_parms;

private:

    // hide copy and assignment
	CDescriptionParser(const CDescriptionParser&);
	CDescriptionParser& operator = (const CDescriptionParser &);
};

}

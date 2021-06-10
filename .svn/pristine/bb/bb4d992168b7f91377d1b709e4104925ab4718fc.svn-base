#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ActionImpl.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)ActionImpl.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ActionImpl.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/10/05 5:50p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
* 
* 8     1/12/05 6:31p Dm185016
* Select support.
* 
* 7     10/04/04 6:47p Dm185016
* Changed base class
* 
* 5     4/30/04 1:01p Dm185016
* Removed unused tags
* 
* 4     3/19/04 3:28p Dm185016
* Memory leak fix
* 
* 3     2/10/04 11:49a Dm185016
* Added header
*/
#include "XMLContent.h"
#include "action.h"

namespace STATE_MACHINE_NAMESPACE
{

class CActionImpl
    : public CXMLContent
	, public IAction
    , public CPoolObject<CActionImpl>
{
public:

    CActionImpl(int);
    virtual ~CActionImpl(void);

	virtual int GetId() const;

	virtual const _TCHAR *GetComment();

    virtual HRESULT Execute(bool *pResult, IMessage &pMessage);

    virtual void AddParameter(const StringType &, const StringType &);

    virtual CParameterList &GetParameters() { return m_vParms; }

    virtual void ClearParameters();

	virtual const _TCHAR *GetName();

    DECLARE_STRING_PROPERTY(Name);
    DECLARE_STRING_PROPERTY(Comment);

    virtual bool IsInternal();

    virtual StringType ToString();

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);

private:

    CActionImpl(CActionImpl &);                     // Hide the Copy construcor
    CActionImpl &operator =(const CActionImpl &);   // Hide the assignment operator

	int nId;

    CParameterList m_vParms;
};

};
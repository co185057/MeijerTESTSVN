#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/MsgImpl.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)RuleImpl.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/MsgImpl.h $
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
* 11    11/15/04 11:42a Dm185016
* Moved return code from action to message.
* 
* 10    10/04/04 6:47p Dm185016
* Changed base class
* 
* 8     4/06/04 1:53p Dm185016
* Suppressed warning 4251
* 
* 7     3/31/04 10:22a Dm185016
* Exported class for new derivatives
*/
#include "XMLContent.h"
#include "message.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace STATE_MACHINE_NAMESPACE 
{

class STATEM_API CMsgImpl
    : public CXMLContent
	, public IMessage
    , public CPoolObject<CMsgImpl>
{
public:

    friend class CRulesHandler;
    
    CMsgImpl();
    CMsgImpl(int);
    virtual ~CMsgImpl(void);

	virtual int GetId() const;

	virtual const _TCHAR *GetComment();

	virtual const _TCHAR *GetName();

	virtual long GetTimeout();

    virtual bool IsSynchronous() const;

    virtual void SetSynchronous(bool);

    virtual BSTR GetText() const;

    virtual void SetText(BSTR);

    virtual HRESULT GetRC() const;

    virtual HRESULT Wait() const;

    virtual void Post() const;

    virtual void* GetData() const;

    virtual size_t GetDataLength() const;


    DECLARE_STRING_PROPERTY(Name);
    DECLARE_STRING_PROPERTY(Comment);
    DECLARE_LONG_PROPERTY(Timeout);
    DECLARE_LONG_PROPERTY(Value);

public: 

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);

private:

    CMsgImpl(CMsgImpl &);                     // Hide the Copy construcor
    CMsgImpl &operator =(const CMsgImpl &);   // Hide the assignment operator

	int nId;
};

};

#pragma warning( pop )

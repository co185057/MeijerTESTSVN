#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/content/ContextSwitch.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ContextSwitch.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/content/ContextSwitch.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
* 
* 3     1/12/05 6:12p Dm185016
* Select support
* 
* 2     12/07/04 4:25p Dm185016
* Added local and unknown context definitions.
* 
* 1     11/16/04 11:45a Dm185016
*/

using namespace std;

#include "RCMApi.h"
#include "XMLContent.h"
#include "RCMConstants.h"
#include "StateObjectProxy.h"
#include "xerror.h"
#include <map>
#include <vector>


namespace STATE_MACHINE_NAMESPACE
{
    class CRuleImpl;
}
using namespace STATE_MACHINE_NAMESPACE;

namespace fastlane
{


class RCMANAGER_API CContextSwitch
    : public CStateObjectProxy
//  : public CXMLContent
//	, public IState
//  , public CPoolObject<CContextSwitch>
{
public:

    CContextSwitch();
    virtual ~CContextSwitch(void);

	virtual int GetId() const;

	virtual const _TCHAR *GetComment() const;

	virtual const _TCHAR *GetName() const;

    virtual bool IsPushInputControlSettings() const;
    virtual bool IsDisableInputControl() const;
    virtual bool IsEnableInputControl() const;

    void SetInputEventsEnabled (bool lValue);
    bool IsInputEventsEnabled() const;

    virtual int GetExitMsgID() const;
    virtual void SetExitMsgID(int);
    virtual int GetEntryMsgID() const;
    virtual void SetEntryMsgID(int);

    DECLARE_BOOLEAN_PROPERTY(PushInputControlSettings);
    DECLARE_BOOLEAN_PROPERTY(DisableInputControl);
    DECLARE_BOOLEAN_PROPERTY(EnableInputControl);
    DECLARE_STRING_PROPERTY(Name);
    DECLARE_STRING_PROPERTY(Comment);

    void AddExitRule(CRuleImplBase *);
    void AddEntryRule(CRuleImplBase *);
    void ClearRules();

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);     //lint !e1735

//  friend class CXMLStateMachine;

private:

    CContextSwitch(const CContextSwitch &);                     // Hide the Copy construcor
    CContextSwitch &operator =(const CContextSwitch &);   // Hide the assignment operator

	static int m_nContextCounter;

    bool m_bInputEventsEnabled;

    int m_nExitMsgID;
    int m_nEntryMsgID;
};

typedef CContextSwitch *PCONTEXTSWITCH;
typedef vector<PCONTEXTSWITCH> CContextSwitchVector;

//typedef hash_map<const _TCHAR *, IState *> CStatesMap;
typedef map<StringType, PCONTEXTSWITCH, StringComparator> CContextSwitchMap;


class RCMANAGER_API CLocalContextSwitch
    : public CContextSwitch
{
public:

    CLocalContextSwitch() {}

    bool IsPushInputControlSettings() const { return true; }
    bool IsDisableInputControl() const { return false; }
    bool IsEnableInputControl() const { return true; }

private:

    CLocalContextSwitch(const CLocalContextSwitch &);                     // Hide the Copy construcor
    CLocalContextSwitch &operator =(const CLocalContextSwitch &);   // Hide the assignment operator
};


class RCMANAGER_API CUnknownContextSwitch
    : public CContextSwitch
{
public:

    CUnknownContextSwitch() {}

    bool IsPushInputControlSettings() const { return true; }
    bool IsDisableInputControl() const { return true; }
    bool IsEnableInputControl() const { return false; }

private:

    CUnknownContextSwitch(const CUnknownContextSwitch &);                     // Hide the Copy construcor
    CUnknownContextSwitch &operator =(const CUnknownContextSwitch &);   // Hide the assignment operator
};

}

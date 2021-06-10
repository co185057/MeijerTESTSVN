#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/content/AppState.h 3     10/21/08 4:06p Sm133076 $
/*
 * @(#)ApplicationState.h    $Revision: 3 $ $Date: 10/21/08 4:06p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/content/AppState.h $
 * 
 * 3     10/21/08 4:06p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/01/05 2:05p Dm185016
 * TAR 294587.  Added application state functionality.
*/

using namespace std;

#include "XMLContent.h"
#include "Rule.h"
#include "RuleImpl.h"
#include "Action.h"
#include "StateObjectProxy.h"
#include "xerror.h"
#include <map>
#include <vector>

using namespace STATE_MACHINE_NAMESPACE;

namespace rap 
{


class CApplicationState
    : public CStateObjectProxy
{
public:

    CApplicationState();
    virtual ~CApplicationState(void);

	virtual int GetId() const;

	virtual const _TCHAR *GetComment() const;

	virtual const _TCHAR *GetName() const;

    virtual bool IsAssistModeEnabled() const;
    virtual bool IsAssistModeDisabled() const;
    virtual bool IsPushAssistModeEnabled() const;

    virtual int GetExitMsgID() const;
    virtual void SetExitMsgID(int);
    virtual int GetEntryMsgID() const;
    virtual void SetEntryMsgID(int);

    virtual void SetSavedAssistModeEnabled(bool);
    virtual bool IsSavedAssistModeEnabled() const;

    DECLARE_BOOLEAN_PROPERTY(AssistModeEnabled);
    DECLARE_BOOLEAN_PROPERTY(AssistModeDisabled);
    DECLARE_BOOLEAN_PROPERTY(PushAssistModeEnabled);
    DECLARE_STRING_PROPERTY(Name);
    DECLARE_STRING_PROPERTY(Comment);

    void AddExitRule(CRuleImpl *);
    void AddEntryRule(CRuleImpl *);
    void ClearRules();

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);     //lint !e1735

//  friend class CXMLStateMachine;

private:

    CApplicationState(const CApplicationState &);                     // Hide the Copy construcor
    CApplicationState &operator =(const CApplicationState &);   // Hide the assignment operator

    bool m_bSavedAssistModeEnabled;

    int m_nExitMsgID;
    int m_nEntryMsgID;
    
    static int m_nAppCounter;
};

typedef CApplicationState *PAPPSTATE;
typedef vector<PAPPSTATE> CApplicationStatesVector;

//typedef hash_map<const _TCHAR *, IState *> CStatesMap;
typedef map<StringType, PAPPSTATE, StringComparator> CApplicationStatesMap;

}

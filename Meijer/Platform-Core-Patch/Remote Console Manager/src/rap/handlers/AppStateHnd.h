#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/handlers/AppStateHnd.h 4     1/13/10 12:34a Cu185008 $
/*
 * @(#)ApplicationStateHandler.h    $Revision: 4 $ $Date: 1/13/10 12:34a $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/handlers/AppStateHnd.h $
 * 
 * 4     1/13/10 12:34a Cu185008
 * G2 Migration.
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/01/05 2:05p Dm185016
 * TAR 294587.  Added application state functionality.
*/
#include "xmlbuilder.h"
#include "XPathHandler.h"
#include "singleton.h"
#include "SMConstants.h"
#include "RuleHandler.h"
#include "KeyValueHandler.h"

using namespace STATE_MACHINE_NAMESPACE;

namespace rap
{

class CApplicationState;

class CApplicationStateHandler 
    : public CXPathHandler
    , public CXMLBuilder
    , public CSingleton<CApplicationStateHandler>
{
public:

    enum ActionDest
    {
        NO_DEST = 0,
        EXIT_LIST = 1,
        ENTRY_LIST
    };

    friend class CSingleton<CApplicationStateHandler>;

    virtual ~CApplicationStateHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT()
    DECLARE_END_ELEMENT()

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

    CApplicationState &appstate() { return *reinterpret_cast<CApplicationState *>(m_pContent); }

protected:

    virtual CApplicationState *NewContent();
    
    void Reference(std::wstring szValue);

    DECLARE_HANDLER(Name)
    DECLARE_HANDLER(Comment)

    DECLARE_HANDLER_BEGIN(AssistModeEnabled)
        DECLARE_HANDLER_START_ELEMENT()
    DECLARE_HANDLER_END(AssistModeEnabled)

    DECLARE_HANDLER_BEGIN(AssistModeDisabled)
        DECLARE_HANDLER_START_ELEMENT()
    DECLARE_HANDLER_END(AssistModeDisabled)

    DECLARE_HANDLER_BEGIN(PushAssistModeEnabled)
        DECLARE_HANDLER_START_ELEMENT()
    DECLARE_HANDLER_END(PushAssistModeEnabled)

    DECLARE_HANDLER_BEGIN(ExitList)
        DECLARE_HANDLER_START_ELEMENT()
        DECLARE_HANDLER_END_ELEMENT()
    DECLARE_HANDLER_END(ExitList)

    DECLARE_HANDLER_BEGIN(EntryList)
        DECLARE_HANDLER_START_ELEMENT()
        DECLARE_HANDLER_END_ELEMENT()
    DECLARE_HANDLER_END(EntryList)

    static HRESULT BeginRule(CRuleImplBase **);
    static HRESULT EndRule();

    void SetDestination(ActionDest eActionDest) { m_eActionDest = eActionDest; }
    void NewRule();
    void SaveRule();
    void CheckNullList();
    void ResetRuleList();

protected:

    CApplicationStateHandler();

private:

    CApplicationStateHandler(const CApplicationStateHandler &);                     // Hidden Copy construcor
    CApplicationStateHandler &operator =(const CApplicationStateHandler &);   // Hidden assignment operator

    ActionDest m_eActionDest;
    CRuleImpl *m_pCurrentRule;
    int m_nRuleCnt;

    CRuleHandler &hndlrRuleHandler;     //lint !e1725 Want reference here.  No problem since
                                        // this class cannot be copied - Singleton
};

}

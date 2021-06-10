#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/handlers/ContextSwitchHnd.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ContextSwitchHandler.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/handlers/ContextSwitchHnd.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:25p Dm185016
 * Moved to new Repository.
* 
* 2     1/12/05 6:13p Dm185016
* Select support.
* 
* 1     11/16/04 11:45a Dm185016
*/
#include "RCMApi.h"
#include "xmlbuilder.h"
#include "XPathHandler.h"
//#include "AppState.h"
#include "singleton.h"
#include "SMConstants.h"
#include "ContextSwitchHnd.h"

namespace STATE_MACHINE_NAMESPACE
{
    class CRuleImplBase;
    class CRuleHandler;
}
using namespace STATE_MACHINE_NAMESPACE;

namespace fastlane
{

class CContextSwitch;
class CRCMLoader;

class RCMANAGER_API CContextSwitchHandler 
    : public CXPathHandler
    , public CXMLBuilder
    , public CSingleton<CContextSwitchHandler>
{
public:

    enum ActionDest
    {
        NO_DEST = 0,
        EXIT_LIST = 1,
        ENTRY_LIST
    };

    friend class CSingleton<CContextSwitchHandler>;

    virtual ~CContextSwitchHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT()
    DECLARE_END_ELEMENT()

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

    CContextSwitch &contextSwitch() { return *reinterpret_cast<CContextSwitch *>(m_pContent); }

    inline void SetLoader(CRCMLoader *pL) { pLoader = pL; }

protected:

    virtual CContextSwitch *NewContent();
    
    void Reference(std::wstring szValue);

    DECLARE_HANDLER(Name)
    DECLARE_HANDLER(Comment)

    DECLARE_HANDLER_BEGIN(PushInputControlSettings)
        DECLARE_HANDLER_START_ELEMENT()
    DECLARE_HANDLER_END(PushInputControlSettings)

    DECLARE_HANDLER_BEGIN(DisableInputControl)
        DECLARE_HANDLER_START_ELEMENT()
    DECLARE_HANDLER_END(DisableInputControl)

    DECLARE_HANDLER_BEGIN(EnableInputControl)
        DECLARE_HANDLER_START_ELEMENT()
    DECLARE_HANDLER_END(EnableInputControl)

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

    CContextSwitchHandler();

private:

    CContextSwitchHandler(const CContextSwitchHandler &);                     // Hidden Copy construcor
    CContextSwitchHandler &operator =(const CContextSwitchHandler &);   // Hidden assignment operator

    CRCMLoader *pLoader;

    ActionDest m_eActionDest;
    CRuleImplBase *m_pCurrentRule;
    int m_nRuleCnt;

    CRuleHandler &hndlrRuleHandler;     //lint !e1725 Want reference here.  No problem since
                                        // this class cannot be copied - Singleton
};

}

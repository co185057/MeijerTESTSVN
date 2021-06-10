#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMLoader.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)CSMLoader.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMLoader.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 6     2/28/05 3:54p Dm185016
 * lint
 * 
 * 5     2/14/05 7:09p Dm185016
 * Release Exclusive Access Event support
 * 
 * 4     2/10/05 5:40p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 3     2/04/05 10:41a Dm185016
 * 
 * 2     2/04/05 10:38a Dm185016
 * Factored out base functionality.  New base class - XMLDocumentLoader.
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 10    1/12/05 6:28p Dm185016
* Select support.
* 
* 9     10/29/04 9:24a Dm185016
* Added <select-action>, <when>
* Added regular expressions to xpath
* 
* 8     10/04/04 6:41p Dm185016
* Trace object is no longer shared.
* 
* 6     6/30/04 3:27p Dm185016
* Lint changes
* Made trace output dependent on data capture mask
* 
* 5     4/30/04 1:03p Dm185016
* Added <include>
* 
* 4     3/19/04 3:03p Dm185016
* Memory leak fix
* 
* 3     1/27/04 1:56p Dm185016
* DLL Support
* 
* 2     1/16/04 9:09a Dm185016
* Namespace support
* New error handling
*/
#include "SMConstants.h"
#include "ContentHandler.h"
#include "property.h"
#include "XMLDocumentLoader.h"
#include "statem.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace STATE_MACHINE_NAMESPACE
{

class CXMLStateMachine;
class ISMObjectRepository;

class STATEM_API CSMLoader
    : public CXMLDocumentLoader
{
public:
    CSMLoader(HTRACE hTrace);
    ~CSMLoader(void);

    virtual void registerHandlers(void);

    inline CXMLStateMachine *GetStateMachine() { return m_pStateMachine; }

    void SetStateMachine(CXMLStateMachine *);

    inline ISMObjectRepository *GetObjectRepository() { return m_pObjectRepository; }

    void SetObjectRepository(ISMObjectRepository *);

protected:

	CXMLStateMachine &stateMachine() { return *m_pStateMachine; }

	void ReadFile(const TCHAR *);

    void InitVariables();
    void InitConstants();

    virtual bool IsFailOnMissingHandler() { return m_bFailOnMissingHandler; }

    DECLARE_HANDLER_BEGIN(IncludeHandler)
	DECLARE_HANDLER_OWNER(IncludeHandler, CSMLoader)
    DECLARE_HANDLER_START_ELEMENT()
    DECLARE_HANDLER_END_ELEMENT()
	DECLARE_HANDLER_PROPERTY(CStringProperty, m_sIncname)
    DECLARE_HANDLER_END(IncludeHandler)

    DECLARE_HANDLER_BEGIN(VariableListHandler)
	DECLARE_HANDLER_OWNER(VariableListHandler, CSMLoader)
    DECLARE_HANDLER_END_ELEMENT()
    DECLARE_HANDLER_END(VariableListHandler)

    DECLARE_HANDLER_BEGIN(ConstantListHandler)
	DECLARE_HANDLER_OWNER(ConstantListHandler, CSMLoader)
    DECLARE_HANDLER_END_ELEMENT()
    DECLARE_HANDLER_END(ConstantListHandler)

private:

    CSMLoader();
    CSMLoader(const CSMLoader &);               // Hide the Copy construcor
    CSMLoader &operator =(const CSMLoader &);   // Hide the assignment operator

    CXMLStateMachine *m_pStateMachine;
    ISMObjectRepository *m_pObjectRepository;

    bool m_bFailOnMissingHandler;
};

}

#pragma warning( pop )

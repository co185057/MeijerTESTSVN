#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/handlers/ContextHnd.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ContextHnd.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/handlers/ContextHnd.h $
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
* 2     11/04/04 2:16p Dm185016
* 
* 1     10/29/04 9:18a Dm185016
*/
#include "RAPApi.h"
#include "xmlbuilder.h"
#include "XPathHandler.h"
//#include "AppState.h"
#include "singleton.h"

using namespace STATE_MACHINE_NAMESPACE;

namespace fastlane
{

class CRCMLoader;
class CContextImpl;

class RAP_API CContextHandler 
    : public CXPathHandler
    , public CXMLBuilder
    , public CSingleton<CContextHandler>
{
public:

    friend class CSingleton<CContextHandler>;

    virtual ~CContextHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT()
    DECLARE_END_ELEMENT()

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

    CContextImpl &context() { return *(CContextImpl *)(m_pContent); }

    inline void SetLoader(CRCMLoader *pL) { pLoader = pL; }

protected:

    CContextHandler();

    DECLARE_HANDLER(DefaultButton);
    DECLARE_HANDLER(View);

    void SetDefaultButton(const StringType &);

private:

    CContextHandler(const CContextHandler &);                     // Hidden Copy construcor
    CContextHandler &operator =(const CContextHandler &);   // Hidden assignment operator

    CRCMLoader *pLoader;

};

}

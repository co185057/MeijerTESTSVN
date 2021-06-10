#pragma once
#include "xmlbuilder.h"
#include "XPathHandler.h"
#include "ActionImpl.h"
#include "singleton.h"

namespace STATE_MACHINE_NAMESPACE
{

class CActionHandler 
    : public CXMLBuilder
    , public CXPathHandler
    , public CSingleton<CActionHandler>
{
public:

    friend class CSingleton<CActionHandler>;

    ~CActionHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT();
    DECLARE_END_ELEMENT();

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

    CActionImpl &action() { return *reinterpret_cast<CActionImpl *>(m_pContent); }

protected:
    
    void Reference(std::wstring szValue);

    DECLARE_HANDLER(Name);
    DECLARE_HANDLER(Comment);

private:
    CActionHandler(void);
    CActionHandler(CActionHandler &);                     // Hidden Copy construcor
    CActionHandler &operator =(const CActionHandler &);   // Hidden assignment operator
};

};
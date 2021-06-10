#pragma once
#include "xmlbuilder.h"
#include "XPathHandler.h"
#include "MsgImpl.h"
#include "singleton.h"

namespace STATE_MACHINE_NAMESPACE
{

class CMsgHandler 
    : public CXMLBuilder
    , public CXPathHandler
    , public CSingleton<CMsgHandler>
{
public:

    friend class CSingleton<CMsgHandler>;

    ~CMsgHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT();
    DECLARE_END_ELEMENT();

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

    CMsgImpl &msg() { return *reinterpret_cast<CMsgImpl *>(m_pContent); }

protected:
    
    void Reference(std::wstring szValue);

    DECLARE_HANDLER(Name);
    DECLARE_HANDLER(Comment);
    DECLARE_HANDLER(Value);

private:
    CMsgHandler(void);
    CMsgHandler(CMsgHandler &);                     // Hidden Copy construcor
    CMsgHandler &operator =(const CMsgHandler &);   // Hidden assignment operator
};

};
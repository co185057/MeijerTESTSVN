#pragma once
#include "xmlbuilder.h"
#include "XPathHandler.h"
#include "StateImpl.h"
#include "singleton.h"

namespace STATE_MACHINE_NAMESPACE
{

class CStateHandler 
    : public CXMLBuilder
    , public CXPathHandler
    , public CSingleton<CStateHandler>
{
public:

    friend class CSingleton<CStateHandler>;

    ~CStateHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT();
    DECLARE_END_ELEMENT();

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

    CStateImpl &state() { return *reinterpret_cast<CStateImpl *>(m_pContent); }

protected:
    
    void Reference(std::wstring szValue);

    DECLARE_HANDLER(Name);
    DECLARE_HANDLER(Comment);

    DECLARE_HANDLER_BEGIN(Initial); 
    DECLARE_HANDLER_START_ELEMENT();
    DECLARE_HANDLER_CHARACTERS();
    DECLARE_HANDLER_END(Initial);

private:
    CStateHandler(void);
    CStateHandler(CStateHandler &);                     // Hidden Copy construcor
    CStateHandler &operator =(const CStateHandler &);   // Hidden assignment operator
};

};

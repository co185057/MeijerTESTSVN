#pragma once
#include "SMConstants.h"
#include "xmlbuilder.h"
#include "XPathHandler.h"
#include "WhenImpl.h"
#include "singleton.h"

namespace STATE_MACHINE_NAMESPACE
{

class CSelectHandler;

class CWhenHandler 
    : public CXMLBuilder
    , public CXPathHandler
    , public CSingleton<CWhenHandler>
{
public:

    friend class CSingleton<CWhenHandler>;

    ~CWhenHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT();
    DECLARE_END_ELEMENT();

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

    CWhenImpl &when() { return *reinterpret_cast<CWhenImpl *>(m_pContent); }

    void SetOwner(CSelectHandler *pOwner);

protected:
    
    void Reference(std::wstring szValue);

private:
    CWhenHandler(void);
    CWhenHandler(CWhenHandler &);                     // Hidden Copy construcor
    CWhenHandler &operator =(const CWhenHandler &);   // Hidden assignment operator

    CSelectHandler *m_pOwner;
};

};
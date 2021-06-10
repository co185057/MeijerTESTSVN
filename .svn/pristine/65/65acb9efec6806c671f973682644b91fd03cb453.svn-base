#pragma once
#include "xmlbuilder.h"
#include "XPathHandler.h"
#include "TimerImpl.h"
#include "singleton.h"

namespace STATE_MACHINE_NAMESPACE 
{

class CTimerHandler 
    : public CXMLBuilder
    , public CXPathHandler
    , public CSingleton<CTimerHandler>
{
public:

    friend class CSingleton<CTimerHandler>;

    ~CTimerHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT();
    DECLARE_END_ELEMENT();

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

    CTimerImpl &timer() { return *reinterpret_cast<CTimerImpl *>(m_pContent); }

protected:
    
    void Reference(std::wstring szValue);

    DECLARE_HANDLER(Name);
    DECLARE_HANDLER(Comment);

private:

    CTimerHandler(void);                                // Hidden constructor
    CTimerHandler(CTimerHandler &);                     // Hidden Copy construcor
    CTimerHandler &operator =(const CTimerHandler &);   // Hidden assignment operator
    
};

};
// based on source from
// www.codeguru.com/cpp/com-tech/atl/atl/article.php/c3601/ 

#ifndef _CXMLHTTPEVENT
#define _CXMLHTTPEVENT

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>

#ifdef error
#define TEMPERRORDEF error
#undef error
#endif
#import "..\MSXML6\msxml6.dll" rename_namespace("MSXML2") no_auto_exclude
#ifdef TEMPERRORDEF
#define error eo.Error
#undef TEMPERRORDEF
#endif

//#include "SSFCommonMinimal.h"

// CXMLHTTPEventCallBack is used for function call back
class CXMLHTTPEventCallBack
{
    public:
        virtual void OnReadyStateChange(MSXML2::IXMLHTTPRequestPtr pRequest, DWORD dwID) { };
};

static _ATL_FUNC_INFO OnEventInfo = {CC_STDCALL, VT_EMPTY, 0};

class CXMLHTTPEvent : public IDispEventSimpleImpl<1, CXMLHTTPEvent, &__uuidof(MSXML2::XMLDOMDocumentEvents)> 
{
    public:
        CXMLHTTPEvent(MSXML2::IXMLHTTPRequest* pRequest, CXMLHTTPEventCallBack* pCallBack = NULL, DWORD dwID = 0);
        ~CXMLHTTPEvent();
        void __stdcall OnReadyStateChange ();

    BEGIN_SINK_MAP(CXMLHTTPEvent)
        SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(MSXML2::XMLDOMDocumentEvents), /*dispid =*/ 0, OnReadyStateChange, &OnEventInfo)
    END_SINK_MAP()

    // XMLDOMDocumentEvents
    private:
        MSXML2::IXMLHTTPRequestPtr m_spRequest;
        CXMLHTTPEventCallBack*    m_pCallBack;
        DWORD m_id;
};

#endif
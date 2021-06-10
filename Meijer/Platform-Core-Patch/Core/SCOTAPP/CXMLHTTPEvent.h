// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Call Bacl Messages and Signatures
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#ifndef _CXMLHTTPEVENT
#define _CXMLHTTPEVENT

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>




#define WM_XMLHTTP_READYSTATE_CHANGE (WM_USER+4)

static _ATL_FUNC_INFO OnEventInfo = {CC_STDCALL, VT_EMPTY, 0};

// CXMLHTTPEventCallBack is used for function call back
class CXMLHTTPEventCallBack
{
	public:
		virtual void OnReadyStateChange(long lReadyState) { };
};


class CXMLHTTPEvent : public IDispEventSimpleImpl</*nID =*/ 1, CXMLHTTPEvent, &__uuidof(MSXML2::XMLDOMDocumentEvents)> 
{
	public:
		CXMLHTTPEvent(MSXML2::IXMLHTTPRequest* pRequest, CXMLHTTPEventCallBack* pCallBack = NULL);
 		~CXMLHTTPEvent();
		void __stdcall OnReadyStateChange ();

	BEGIN_SINK_MAP(CXMLHTTPEvent)
 		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(MSXML2::XMLDOMDocumentEvents), /*dispid =*/ 0, OnReadyStateChange, &OnEventInfo)
	END_SINK_MAP()

	// XMLDOMDocumentEvents
  	private:
		MSXML2::IXMLHTTPRequestPtr m_spRequest;
		CXMLHTTPEventCallBack*    m_pCallBack;
		DWORD					  m_dwAdviseCookie;
};

#endif
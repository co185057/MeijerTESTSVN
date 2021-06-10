// based on source from
// www.codeguru.com/cpp/com-tech/atl/atl/article.php/c3601/ 

#include "stdafx.h"

#include "XMLHTTPEvent.h"

// Create an event sink callback 
CXMLHTTPEvent::CXMLHTTPEvent(MSXML2::IXMLHTTPRequest* pRequest, 
                             CXMLHTTPEventCallBack* pCallBack,
                             DWORD dwID) 
{
    m_spRequest      = pRequest;
    m_pCallBack      = pCallBack;
    m_id             = dwID;
    // add sink to xml http request
    m_spRequest->put_onreadystatechange((IDispatch*)this);

    //test the callback
}

CXMLHTTPEvent::~CXMLHTTPEvent() 
{
    // Remove sink from xml http request
    m_spRequest->put_onreadystatechange(NULL);
    m_pCallBack       = NULL;
}

// State change call back handler
void __stdcall CXMLHTTPEvent::OnReadyStateChange( )
{
    if (m_pCallBack){
        m_pCallBack->OnReadyStateChange(m_spRequest, m_id);
    }
}

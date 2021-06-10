///////////////////////////////////////////////////////////////////////////////////////////
//
//
// TITLE:   CXMLHTTPEvent.cpp
//          Event handler class for HTTP requests
//
// AUTHOR:  Barbara Dawkins
//
// 
/////////////////////////////////////////////////////////////////////////////////////////
#import <msxml4.dll>
using namespace MSXML2;

#include "CXMLHTTPEvent.h"

///////////////////////////////////////////////////////////////////////////////////
// Function:    CXMLHTTPEvent                                 
// Class:       CXMLHTTPEvent                                         
// Description: Constructor
// Parameters:  None       
//             
// Comments:    N/A
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Create an event sink callback 
CXMLHTTPEvent::CXMLHTTPEvent(MSXML2::IXMLHTTPRequest* pRequest, 
                             CXMLHTTPEventCallBack* pCallBack) 
{
   	m_dwAdviseCookie = NULL;
 	m_spRequest      = pRequest;
  	m_pCallBack      = pCallBack;

	// add sink to xml http request
  	m_spRequest->put_onreadystatechange((IDispatch*)this);
}

///////////////////////////////////////////////////////////////////////////////////
// Function:    ~CXMLHTTPEvent                                 
// Class:       CXMLHTTPEvent                                         
// Description: 
// Parameters:  None       
//             
// Comments:    N/A
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
CXMLHTTPEvent::~CXMLHTTPEvent() 
{
	// Remove sink from xml http request
	m_spRequest->put_onreadystatechange(NULL);
  	m_pCallBack       = NULL;
}

///////////////////////////////////////////////////////////////////////////////////
// Function:    OnReadyStateChange                                 
// Class:       CXMLHTTPEvent                                         
// Description: 
// Parameters:  None       
//             
// Comments:    N/A
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
// State change call back handler
void __stdcall CXMLHTTPEvent::OnReadyStateChange( )
{

	if (m_pCallBack)
		m_pCallBack->OnReadyStateChange(m_spRequest->GetreadyState());
	
}

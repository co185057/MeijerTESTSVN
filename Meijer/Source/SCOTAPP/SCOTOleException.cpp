//***   Module File   *****************************************************//
//
// Class Name:			SCOTOleException
//
// Filename:			SCOTOleException.cpp
//
// Description:			This exception class is derived from
//                      COleDispatchException and includes additional
//                      nfo such as a SCOT Message ID and a DMCLASS Type.
//
// SCOT Feature:	    SCOT Release 2.0
//
// Related Classes:		    
//
// Copyright:           NCR Corporation Copyright © 1999
//
//*************************************************************************//

#include "stdafx.h"
#include "SCOTOleException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SCOTOleException::SCOTOleException(	LPCTSTR lpszDescription,
                                    UINT nHelpID,
                                    WORD wCode,
                                    PSTEXT MsgID,
                                    long DMClass)
:COleDispatchException(lpszDescription, nHelpID, wCode),
m_MsgID(MsgID),
m_lDMClass(DMClass)
{
}

// Virtual Destructor
SCOTOleException::~SCOTOleException()
{
}



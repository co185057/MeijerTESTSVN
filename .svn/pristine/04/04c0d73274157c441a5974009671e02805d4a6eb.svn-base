//***   Header File   *****************************************************//
//
// Class Name:			SCOTOleException
//
// Filename:			SCOTOleException.h
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

#ifndef _SCOTOleException_H
#define _SCOTOleException_H

#include "SMSystemMessage5.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class SCOTOleException : public COleDispatchException
{
public:
	// Constructor
	SCOTOleException(LPCTSTR, UINT, WORD, PSTEXT, long);
	// Destructor
    virtual ~SCOTOleException();

    // Variables
    PSTEXT  m_MsgID;
    long    m_lDMClass;

protected:


private:
	SCOTOleException(const SCOTOleException&);				// Temporarily blocks Copy Constructor
	SCOTOleException& operator= (const SCOTOleException&);	// Temporarily blocks Assignment Constructor
};

// Macro for catching the new SCOTOleException
#define COLECatch(_Text_A)\
try{##_Text_A;}\
catch (SCOTOleException * pEx)\
{\
    int nRtnCode = pEx->m_scError;\
	TRACE1(_T("COleDispatchException"), nRtnCode);\
    dm.setTriColorLight(DM_TRICOLORLIGHT_RED,\
                        DM_TRICOLORLIGHT_BLINK_1HZ,\
                        DM_TRICOLORLIGHT_NORMALREQUEST);\
    CREATE_AND_DISPENSE(SystemMessage5)(NULL,\
                                    pEx->m_MsgID,\
                                    PS_NOTEXT,\
                                    0,\
                                    pEx->m_lDMClass,\
                                    true);\
	pEx->Delete();\
}

#endif
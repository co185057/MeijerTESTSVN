//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage3.cpp
//
// Description: See h file
//
// Author:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSystemMessage3.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SystemMessage3")

IMPLEMENT_DYNCREATE(SMSystemMessage3, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

/////////////////////////////////////////////////////
SMSystemMessage3::SMSystemMessage3(LPCTSTR szMessage,   // string text message or NULL
                                   PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                                   PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
        		                       long   promptValue, // prompt area sale value
                                   long   deviceClass, // device class, -1 for none
                                   bool   checkHealth) // DMCheckHealth after msg?
: SMSystemMessage3Base( 
                  szMessage,
                  psMessage,
                  promptText,
                  promptValue,
                  deviceClass,
                  checkHealth)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSystemMessage3::SMSystemMessage3()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSystemMessage3::Deliver(LPCTSTR szMessage,   // string text message or NULL
                                   PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                                   PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
        		                       long   promptValue, // prompt area sale value
                                   long   deviceClass, // device class, -1 for none
                                   bool   checkHealth)
{
  SMSystemMessage3Base::Deliver( 
              szMessage,
              psMessage,
              promptText,
              promptValue,
              deviceClass,
              checkHealth);

  return this;
}



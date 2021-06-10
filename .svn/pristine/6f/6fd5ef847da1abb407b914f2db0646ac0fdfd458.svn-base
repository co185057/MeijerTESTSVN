//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage1.cpp
//
// Description: See h file
//
//
// Author:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSystemMessage1.h"       // MGV this

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SystemMessage1")

IMPLEMENT_DYNCREATE(SMSystemMessage1, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////////////
SMSystemMessage1::SMSystemMessage1(LPCTSTR szMessage,   // string text message or NULL
                                   PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                                   PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
 	                                 long   promptValue, // prompt area sale value
                                   long   deviceClass, // device class, -1 for none
                                   bool   checkHealth) // DMCheckHealth after msg?

: SMSystemMessage1Base(
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
SMSystemMessage1::SMSystemMessage1()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSystemMessage1::Deliver(LPCTSTR szMessage,   // string text message or NULL
                                   PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                                   PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
 	                                 long   promptValue, // prompt area sale value
                                   long   deviceClass, // device class, -1 for none
                                   bool   checkHealth)
{
  SMSystemMessage1Base::Deliver(
              szMessage,
              psMessage,
              promptText,
              promptValue,
              deviceClass,
              checkHealth);

  return this;
}


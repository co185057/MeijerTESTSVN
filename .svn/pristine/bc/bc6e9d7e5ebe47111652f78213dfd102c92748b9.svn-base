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
//               // MS MFC template header

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
                                   bool   checkHealth,
								   bool displayHTML) // DMCheckHealth after msg?

: SMSystemMessage1Base(
                  szMessage,
                  psMessage,
                  promptText,
                  promptValue,
                  deviceClass,
                  checkHealth,
				  displayHTML)
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
                                   bool   checkHealth,
								   bool displayHTML)
{
  SMSystemMessage1Base::Deliver(
              szMessage,
              psMessage,
              promptText,
              promptValue,
              deviceClass,
              checkHealth,
			  displayHTML);

  return this;
}

bool SMSystemMessage1::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSystemMessage1Base::DeliverDescriptorList(dlParameterList);
}

CString SMSystemMessage1::getWaveFile()
{
    return SMSystemMessage1Base::getWaveFile();
}

bool SMSystemMessage1::storeMode()
{
    return SMSystemMessage1Base::storeMode();
}
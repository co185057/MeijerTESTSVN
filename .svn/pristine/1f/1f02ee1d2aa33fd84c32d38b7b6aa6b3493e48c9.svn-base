//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessageOpPass.cpp
//
// Description: see h file
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSystemMessageOpPass.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SystemMessageOpPass")

IMPLEMENT_DYNCREATE(SMSystemMessageOpPass, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////////
// Constructor
////////////////////////////////////////////
SMSystemMessageOpPass::SMSystemMessageOpPass(BMPS   bitmap,
                                             PSTEXT title,       // PSTEXT title number or PS_NOTEXT
                                             bool   continueButton,
                                             bool   cancelButton,
                                             LPCTSTR szMessage,   // string text message or NULL
                                             PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                                             PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
		                                         long   promptValue, // prompt area sale value
                                             long   deviceClass, // device class, -1 for none
                                             bool   checkHealth) // DMCheckHealth after msg?
// Create a SMSystemMessage from the parms given us.
: SMSystemMessageOpPassBase(bitmap,
                   title,
                   continueButton,
                   cancelButton,
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
SMSystemMessageOpPass::SMSystemMessageOpPass()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSystemMessageOpPass::Initialize()
{
  return SMSystemMessageOpPassBase::Initialize();
}

////////////////////////////////////////
SMStateBase  *SMSystemMessageOpPass::Deliver(BMPS   bitmap,
                                             PSTEXT title,       // PSTEXT title number or PS_NOTEXT
                                             bool   continueButton,
                                             bool   cancelButton,
                                             LPCTSTR szMessage,   // string text message or NULL
                                             PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                                             PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
		                                         long   promptValue, // prompt area sale value
                                             long   deviceClass, // device class, -1 for none
                                             bool   checkHealth)
{
  SMSystemMessageOpPassBase::Deliver(bitmap,
               title,
               continueButton,
               cancelButton,
               szMessage,
               psMessage,
               promptText,
               promptValue,
               deviceClass,
               checkHealth);

  return this;
}



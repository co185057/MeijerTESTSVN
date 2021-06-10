//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage.cpp
//
// Description: base class for system message classes, see h file
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"

#include "SMSystemMessage.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SystemMessage")

IMPLEMENT_DYNCREATE(SMSystemMessage, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////////////
SMSystemMessage::SMSystemMessage(BMPS   bitmap,
                                 PSTEXT title,
                                 bool   continueButton,
                                 bool   cancelButton,
                                 LPCTSTR szMessage,   // string text message or NULL
                                 PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                                 PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
      		                       long   promptValue, // prompt area sale value
                                 long   deviceClass, // device class, -1 for none
                                 bool   checkHealth, // DMCheckHealth after msg?
                                 bool showTotal,
                                 bool showRetractedTotal,
                                 bool okButton,
                                 bool flashOkButton)
: SMSystemMessageBase(bitmap, title, continueButton,
					  cancelButton, szMessage, psMessage,
					  promptText, promptValue, deviceClass, checkHealth,
                      showTotal, showRetractedTotal, okButton, flashOkButton)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSystemMessage::SMSystemMessage()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSystemMessage::Initialize()
{
   return SMSystemMessageBase::Initialize();
}

////////////////////////////////////////
SMStateBase  *SMSystemMessage::Deliver(BMPS   bitmap,
                                 PSTEXT title,
                                 bool   continueButton,
                                 bool   cancelButton,
                                 LPCTSTR szMessage,   // string text message or NULL
                                 PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                                 PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
      		                       long   promptValue, // prompt area sale value
                                 long   deviceClass, // device class, -1 for none
                                 bool   checkHealth, // DMCheckHealth after msg?
                                 bool showTotal,
                                 bool showRetractedTotal,
                                 bool okButton,
                                 bool flashOkButton)
{
  SMSystemMessageBase::Deliver(bitmap, title, continueButton,
				  cancelButton, szMessage, psMessage,
				  promptText, promptValue, deviceClass, checkHealth,
                  showTotal, showRetractedTotal, okButton, flashOkButton);

  return this;
}



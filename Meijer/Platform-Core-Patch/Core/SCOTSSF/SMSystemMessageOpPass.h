//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSystemMessageOpPass.h
//
// Description: A base state for system messages that require an operator and
//   password to see them.  Note that this state is essentially both a
//   SMSystemMessage state and a SMOperatorPassword state.  Multiple inheritance
//   was not used because both those states inherit from SMHelpModeState,
//   leading to ambiguities, etc.  Thus a SMSystemMessageOpPass state inherits
//   from SMOperatorPassword state and has a SMSystemMessage.
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSTEMMESSAGEOPPASS
#define _SYSTEMMESSAGEOPPASS

#include "SMSystemMessageOpPassBase.h"


#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSystemMessageOpPass : public SMSystemMessageOpPassBase
{
public:
  SMSystemMessageOpPass( BMPS   bitmap,
                         PSTEXT title,       // specify PS_NOTEXT for default
                         bool   continueButton,
                         bool   cancelButton,
                         LPCTSTR szMessage,   // NULL if psMessage specified
                         PSTEXT psMessage,   // PS_NOTEXT if szMessage specified
                         PSTEXT promptText,  // PS_NOTEXT for no prompt
		                     long   promptValue, // prompt area sale value
                         long   deviceClass, // device class, -1 for none
                         bool   checkHealth);// DMCheckHealth after msg?
	SMSystemMessageOpPass();
	SMStateBase *Initialize();
	SMStateBase *Deliver( BMPS   bitmap,
                         PSTEXT title,       // specify PS_NOTEXT for default
                         bool   continueButton,
                         bool   cancelButton,
                         LPCTSTR szMessage,   // NULL if psMessage specified
                         PSTEXT psMessage,   // PS_NOTEXT if szMessage specified
                         PSTEXT promptText,  // PS_NOTEXT for no prompt
		                     long   promptValue, // prompt area sale value
                         long   deviceClass, // device class, -1 for none
                         bool   checkHealth);

  DECLARE_DYNCREATE(SMSystemMessageOpPass)// MFC Runtime class/object information
};

#endif

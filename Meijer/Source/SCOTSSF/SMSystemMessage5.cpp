//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage5.cpp
//
// Description: See h file.
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSystemMessage5.h"       // MGV sys msg 3 state
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SystemMessage5")

IMPLEMENT_DYNCREATE(SMSystemMessage5, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

/////////////////////////////////////////////////////
// Constructor
/////////////////////////////////////////////////////
SMSystemMessage5::SMSystemMessage5
                (LPCTSTR szMessage,   // char message text or NULL
		             PSTEXT psMessage,   // message text
		             PSTEXT promptText,  // prompt text or NULL
		             long   promptValue, // prompt area sale value
                 long   deviceClass, // device class, -1 for none
                 bool   checkHealth,
				 bool displayHTML)
// Construct our parent, most things are handled there
: SMSystemMessage5Base(
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
SMSystemMessage5::SMSystemMessage5()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSystemMessage5::Deliver(LPCTSTR szMessage,   // char message text or NULL
		             PSTEXT psMessage,   // message text
		             PSTEXT promptText,  // prompt text or NULL
		             long   promptValue, // prompt area sale value
                 long   deviceClass, // device class, -1 for none
                 bool   checkHealth,
				 bool displayHTML)
{
  SMSystemMessage5Base::Deliver(
                    szMessage,
                    psMessage,
                    promptText,
                    promptValue,
                    deviceClass,
                    checkHealth,
					displayHTML);


  return this;
}

SMStateBase * SMSystemMessage5::Initialize(void)
{
    return SMSystemMessage5Base::Initialize();
}

void SMSystemMessage5::UnInitialize(void)
{
	//Start CRD 366323
	NotifyExitStoreMode();
	//End CRD 366323

    SMSystemMessage5Base::UnInitialize();
}

bool SMSystemMessage5::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSystemMessage5Base::DeliverDescriptorList(dlParameterList);
}

CString SMSystemMessage5::getWaveFile()
{
    return SMSystemMessage5Base::getWaveFile();
}

SMStateBase * SMSystemMessage5::stateAfterPasswordVerify()
{
    return SMSystemMessage5Base::stateAfterPasswordVerify();
}

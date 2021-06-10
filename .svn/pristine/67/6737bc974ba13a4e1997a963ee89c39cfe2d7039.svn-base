//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage5Base.cpp
//
// Description: A SMSystemMessage5Base is a SMSystemMessageOpPass.  It is
//   like SystemMessage2 (cancel but no continue button), but an
//   operator/password is required to see this one.
//	 Has An Echo Box!!
///
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSystemMessage5.h"   // Header file

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SystemMessage5Base")

IMPLEMENT_DYNCREATE(SMSystemMessage5Base, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SystemMessage5)

DEFINE_TIMESTAMP

/////////////////////////////////////////////////////
// Constructor
// sys msg 5 is level 2 with op/passwd
/////////////////////////////////////////////////////
SMSystemMessage5Base::SMSystemMessage5Base
                      (LPCTSTR szMessage,   // char message text or NULL
		               PSTEXT psMessage,   // message text
		               PSTEXT promptText,  // prompt text or NULL
		               long   promptValue, // prompt area sale value
                       long   deviceClass, // device class, -1 for none
                       bool   checkHealth, // DMCheckHealth after msg?
					   bool displayHTML) //SR672

// Construct our parent, most things are handled there
  : STATE(SystemMessageOpPass)(BMP_ERRMSGLEVEL1, 
                       LTT_SYSTEMMSG2,
                       false,              // no continue button
                       true,               // cancel button
                       szMessage,
                       psMessage,
                       promptText,
                       promptValue,
                       deviceClass,
                       checkHealth,
					   displayHTML) //SR672

{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSystemMessage5Base::SMSystemMessage5Base()
{
  IMPLEMENT_TIMESTAMP
}


////////////////////////////////////////
SMSystemMessage5Base::~SMSystemMessage5Base()
{
}

////////////////////////////////////////
SMStateBase  *SMSystemMessage5Base::Deliver(LPCTSTR szMessage,   // char message text or NULL
		               PSTEXT psMessage,   // message text
		               PSTEXT promptText,  // prompt text or NULL
		               long   promptValue, // prompt area sale value
                       long   deviceClass, // device class, -1 for none
                       bool   checkHealth,
					   bool displayHTML)
{
  //Init base class members
  //BMP_ERRMSGLEVEL1 bmp has an echo box to emulate the approval needed screen
  SMSystemMessageOpPassBase::InitSystemMessage(BMP_ERRMSGLEVEL1,
                    LTT_SYSTEMMSG1,
                    false,
                    true,   // TAR 195982
                    szMessage,
                    psMessage,
                    promptText,
                    promptValue,
                    deviceClass,
                    checkHealth,
					displayHTML);


  return this;
}
/////////////////////////////////////////
bool SMSystemMessage5Base::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    
    // There should be ten parameters, a variety of types
    if (dlParameterList.GetDescriptorCount() == 6)
    {
        trace(L6, _T("+SMSystemMessage5Base::DeliverDescriptorList() - Parameter Count of 6"));
        Deliver((LPCTSTR)DLParamCString(dlParameterList), // szMessage
                (PSTEXT)DLParamDWORD(dlParameterList),  // psMessage
                (PSTEXT)DLParamDWORD(dlParameterList),  // promptText
                DLParamDWORD(dlParameterList),          // promptValue
                DLParamDWORD(dlParameterList),          // DeviceClass
                (bool)DLParamWORD(dlParameterList));    // checkHealth
        return true;
    }
    else
    {
        trace(L2, _T("+SMSystemMessage5Base::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}


///////////////////////////////////////////
SMStateBase  *SMSystemMessage5Base::Initialize(void)
{
  //ps.ShowVideoLarge();
  return STATE(SystemMessageOpPass)::Initialize();
}


SMStateBase * SMSystemMessage5Base::stateAfterPasswordVerify()
{
	// TAR:164390
	// entering secure state since password of operator is verified
	// inform security that SCOT is entering to secure mode
	// so that Security will not post warning or severe security messages while
	// the operator is waiting to do something at the terminal

	// TAR: 202693,94,95
	// No longer need to utilize this feature for 3.x - breaks SmartScale LPM043002
	//SASendSCOTInput( STORE_MODE );
	return SMSystemMessageOpPassBase::stateAfterPasswordVerify();
}

void SMSystemMessage5Base::UnInitialize()
{


	// TAR:164390
	// leaving this state
	// inform security that SCOT is returning to non-secure mode
	SASendSCOTInput(CUSTOMER_MODE);
	SMSystemMessageOpPassBase::UnInitialize();
}

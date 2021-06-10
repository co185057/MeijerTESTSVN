//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage6Base.cpp
//
// Description: A SMSystemMessage6Base is a SMSystemMessageOpPass.  It is
//   like SystemMessage3 (continue and cancel buttons), but an
//   operator/password is required to see this one.
//	 Has An Echo Box!!
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSystemMessage6.h"   // Header file

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SystemMessage6Base")

IMPLEMENT_DYNCREATE(SMSystemMessage6Base, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SystemMessage6)

DEFINE_TIMESTAMP

/////////////////////////////////////////////////////
// Constructor
// sys msg 6 is level 3 with op/passwd
/////////////////////////////////////////////////////
SMSystemMessage6Base::SMSystemMessage6Base
                      (LPCTSTR szMessage,    // char message text or NULL
		               PSTEXT psMessage,    // message text
		               PSTEXT promptText,   // prompt text or NULL
		               long   promptValue,  // prompt area sale value
                       long   deviceClass,  // device class, -1 for none
                       bool   checkHealth, // DMCheckHealth after msg?
					   bool displayHTML)   //SR672
// Construct our parent, most things are handled there
   //BMP_ERRMSGLEVEL1 bmp has an echo box to emulate the aproval needed screen
   : STATE(SystemMessageOpPass)(BMP_ERRMSGLEVEL1, 
                        LTT_SYSTEMMSG3,
                        true,               // continue button
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
SMSystemMessage6Base::SMSystemMessage6Base()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSystemMessage6Base::~SMSystemMessage6Base()
{
}


////////////////////////////////////////
SMStateBase  *SMSystemMessage6Base::Deliver(LPCTSTR szMessage,    // char message text or NULL
		               PSTEXT psMessage,    // message text
		               PSTEXT promptText,   // prompt text or NULL
		               long   promptValue,  // prompt area sale value
                       long   deviceClass,  // device class, -1 for none
                       bool   checkHealth,
					   bool displayHTML) //SR672
{
  //Init base class members
  //BMP_ERRMSGLEVEL1 bmp has an echo box to emulate the approval needed screen
  SMSystemMessageOpPassBase::InitSystemMessage(BMP_ERRMSGLEVEL1,	
                    LTT_SYSTEMMSG3,
                    true,
                    true,  // TAR 195982
                    szMessage,
                    psMessage,
                    promptText,
                    promptValue,
                    deviceClass,
                    checkHealth,
					displayHTML); //SR672



  return this;
}

/////////////////////////////////////////
bool SMSystemMessage6Base::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    
    // There should be ten parameters, a variety of types
    if (dlParameterList.GetDescriptorCount() == 6)
    {
        trace(L6, _T("+SMSystemMessage6Base::DeliverDescriptorList() - Parameter Count of 6"));
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
        trace(L2, _T("+SMSystemMessage6Base::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}


///////////////////////////////////////////
SMStateBase  *SMSystemMessage6Base::Initialize(void)
{
  // ps.ShowVideoLarge();
  return STATE(SystemMessageOpPass)::Initialize();
}


SMStateBase * SMSystemMessage6Base::stateAfterPasswordVerify()
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

void SMSystemMessage6Base::UnInitialize()
{
	// TAR:164390
	// leaving this state
	// inform security that SCOT is returning to non-secure mode
	SASendSCOTInput(CUSTOMER_MODE);
	SMSystemMessageOpPassBase::UnInitialize();
}

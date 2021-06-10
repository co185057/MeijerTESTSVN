//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage4Base.cpp
//
// Description: A SMSystemMessage4Base is a SMSystemMessageOpPass.  It is
//   like SystemMessage1 (continue but no cancel button), but an
//   operator/password is required to see this one.
//	 Has An Echo Box!!
//
// Author:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSystemMessage4.h"       // MGV sys msg 3 state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SystemMessage4Base")

IMPLEMENT_DYNCREATE(SMSystemMessage4Base, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SystemMessage4)

DEFINE_TIMESTAMP

/////////////////////////////////////////////////////
// Constructor
// sys msg 4 is level 1 with op/passwd
/////////////////////////////////////////////////////
SMSystemMessage4Base::SMSystemMessage4Base
                (LPCTSTR szMessage,   // char message text or NULL
		             PSTEXT psMessage,   // message text
		             PSTEXT promptText,  // prompt text or NULL
		             long   promptValue, // prompt area sale value
                 long   deviceClass, // device class, -1 for none
                 bool   checkHealth) // DMCheckHealth after msg?
// Construct our parent, most things are handled there
: STATE(SystemMessageOpPass)(BMP_ERRMSGLEVEL1, 
                        LTT_SYSTEMMSG1,
                        true,  // continue button
                        false, // no cancel button
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
SMSystemMessage4Base::SMSystemMessage4Base()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSystemMessage4Base::~SMSystemMessage4Base()
{
}


////////////////////////////////////////
SMStateBase  *SMSystemMessage4Base::Deliver(LPCTSTR szMessage,   // char message text or NULL
                                            PSTEXT psMessage,   // message text
                                            PSTEXT promptText,  // prompt text or NULL
                                            long   promptValue, // prompt area sale value
                                            long   deviceClass, // device class, -1 for none
                                            bool   checkHealth)
{
  //Init base class members
  //BMP_ERRMSGLEVEL1 bmp has an echo box to emulate the approval needed screen
  SMSystemMessageOpPassBase::InitSystemMessage(BMP_ERRMSGLEVEL1,
                    LTT_SYSTEMMSG1,
                    true,
                    false,
                    szMessage,
                    psMessage,
                    promptText,
                    promptValue,
                    deviceClass,
                    checkHealth);


  return this;
}
/////////////////////////////////////////
bool SMSystemMessage4Base::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    
    // There should be six parameters, a variety of types
    if (dlParameterList.GetDescriptorCount() == 6)
    {
        trace(L6, _T("+SMSystemMessage4Base::DeliverDescriptorList() - Parameter Count of 6"));
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
        trace(L2, _T("+SMSystemMessage4Base::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}



SMStateBase * SMSystemMessage4Base::stateAfterPasswordVerify()
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

void SMSystemMessage4Base::UnInitialize()
{
    // Tar 214729  RJF 101002
	if(fDMErrorCashDue)		// if true, we need to dispense change
	{
       DMAcceptor();
	   fDMErrorCashDue = false;
	}

	// Begin TAR 206353
	// CAN enter Remote Assist Mode after leaving this state
	//ra.OnSetTransactionState(RA_STATE_ASSISTOK);
    ra.RequestEnableAssistMode(true);   //TAR213249

	// End TAR 206353

	// TAR:164390
	// leaving this state
	// inform security that SCOT is returning to non-secure mode
	SASendSCOTInput(CUSTOMER_MODE);
	SMSystemMessageOpPassBase::UnInitialize();
}

bool SMSystemMessage4Base::PSRemoteLegalForState() //TAR408719
{
    long lDeviceClass = theSystemMessage.GetDeviceClass();

	return IsRemoteErrorAllowed(lDeviceClass);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage3Base.cpp
//
// Description: A SMSystemMessage3Base is a SMSystemMessage, whose constructor
//   is called with flags to create a Continue button *and* a Cancel
//   button - a distinguishing feature of a SMSystemMessage3Base.
//	 No Echo Box!!
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#include "Common.h"                 // Common includes

#include "SMSystemMessage3.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SystemMessage3Base")

IMPLEMENT_DYNCREATE(SMSystemMessage3Base, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SystemMessage3)

DEFINE_TIMESTAMP

/////////////////////////////////////////////////////
SMSystemMessage3Base::SMSystemMessage3Base(
				   LPCTSTR szMessage,   // string text message or NULL
                   PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                   PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
        		   long   promptValue, // prompt area sale value
                   long   deviceClass, // device class, -1 for none
                   bool   checkHealth, // DMCheckHealth after msg?
				   bool displayHTML)  //SR672
: STATE(SystemMessage)(BMP_ERRMSGLEVEL2,
                  LTT_SYSTEMMSG3,      // use default title if none
                  true,                // continue button
                  true,                // cancel button
                  szMessage,
                  psMessage,
                  promptText,
                  promptValue,
                  deviceClass,
                  checkHealth, true, false, false, true,
				  displayHTML), //SR672
  fHealth(checkHealth),
  lDevice(deviceClass)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSystemMessage3Base::SMSystemMessage3Base()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSystemMessage3Base::Deliver(
				   LPCTSTR szMessage,   // string text message or NULL
                   PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                   PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
        		   long   promptValue, // prompt area sale value
                   long   deviceClass, // device class, -1 for none
                   bool   checkHealth,
				   bool displayHTML) //SR672
{
  //BMP_ERRMSGLEVEL2 bmp has NO echo box to be displayed after the approval needed screen
  STATE(SystemMessage)::Deliver(
                  BMP_ERRMSGLEVEL2,
                  LTT_SYSTEMMSG3,        
                  true,  // TAR 195982 DEM                 
                  true,                  
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
bool SMSystemMessage3Base::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    
    // There should be ten parameters, a variety of types
    if (dlParameterList.GetDescriptorCount() == 6)
    {
        trace(L6, _T("+SMSystemMessage3Base::DeliverDescriptorList() - Parameter Count of 6"));
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
        trace(L2, _T("+SMSystemMessage3Base::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}



//////////////////////////////////////////////
SMStateBase  *SMSystemMessage3Base::Initialize(void)
{
  //ps.ShowVideoLarge();
  return STATE(SystemMessage)::Initialize();
}

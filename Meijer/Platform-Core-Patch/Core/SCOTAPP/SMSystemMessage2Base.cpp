//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage2Base.cpp
//
// Description: A SMSystemMessage2Base is a SMSystemMessage, whose constructor
//   is called with flags to create a Cancel button but not a Continue
//   button - a distinguishing feature of a SMSystemMessage2Base.
//	 No Echo Box!!
// 
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMSystemMessage2.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SystemMessage2Base")

IMPLEMENT_DYNCREATE(SMSystemMessage2Base, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SystemMessage2)

DEFINE_TIMESTAMP

SMSystemMessage2Base::SMSystemMessage2Base(
  					   LPCTSTR szMessage,  // string text message or NULL
                       PSTEXT psMessage,  // PSTEXT message number or PS_NOTEXT
                       PSTEXT promptText, // PSTEXT prompt number or PS_NOTEXT
                       long   promptValue,// prompt area sale value
                       long   deviceClass,// device class, -1 for none
                       bool   checkHealth)// DMCheckHealth after msg?
       : STATE(SystemMessage)(BMP_ERRMSGLEVEL2,
                  LTT_SYSTEMMSG2,         // use default title if none
                  false,                  // no continue button
                  true,                   // cancel button
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
SMSystemMessage2Base::SMSystemMessage2Base()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSystemMessage2Base::Deliver(
  					   LPCTSTR szMessage,  // string text message or NULL
                       PSTEXT psMessage,  // PSTEXT message number or PS_NOTEXT
                       PSTEXT promptText, // PSTEXT prompt number or PS_NOTEXT
                       long   promptValue,// prompt area sale value
                       long   deviceClass,// device class, -1 for none
                       bool   checkHealth)
{
  //BMP_ERRMSGLEVEL2 bmp has NO echo box to be displayed after the approval needed screen
  STATE(SystemMessage)::Deliver(
                  BMP_ERRMSGLEVEL2,
                  LTT_SYSTEMMSG2,        
                  false,                 
                  true,                  
                  szMessage,
                  psMessage,
                  promptText,
                  promptValue,
                  deviceClass,
                  checkHealth);

  return this;
}
/////////////////////////////////////////
bool SMSystemMessage2Base::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    
    // There should be ten parameters, a variety of types
    if (dlParameterList.GetDescriptorCount() == 6)
    {
        trace(L6, _T("+SMSystemMessage2Base::DeliverDescriptorList() - Parameter Count of 6"));
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
        trace(L2, _T("+SMSystemMessage2Base::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}


///////////////////////////////////////////
SMStateBase  *SMSystemMessage2Base::Initialize(void)
{
  // ps.ShowVideoLarge();
  return STATE(SystemMessage)::Initialize();
}

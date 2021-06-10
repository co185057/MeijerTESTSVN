//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage1Base.cpp
//
// Description: A SMSystemMessage1Base is a SMSystemMessage, whose constructor
//   is called with flags to create a Continue button but not a Cancel
//   button - a distinguishing feature of a SMSystemMessage1Base.
//	 No Echo Box!!
//
// Author:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#ifndef _CPPUNIT
#include "Common.h"                 // MGV common includes

#include "SMSystemMessage1.h"       // MGV this
#else 
#include "SMSystemMessage1Base.h"
#endif

#ifndef _CPPUNIT
#define COMP_ID ID_SM               // base state component
#define T_ID _T("SystemMessage1Base")

IMPLEMENT_DYNCREATE(SMSystemMessage1Base, CObject)// MFC Runtime class/object information
//IMPLEMENT_STATE(SystemMessage1)

DEFINE_TIMESTAMP
////////////////////////////////////////////////
SMSystemMessage1Base::SMSystemMessage1Base(LPCTSTR szMessage,   // string text message or NULL
                                   PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                                   PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
 	                                 long   promptValue, // prompt area sale value
                                   long   deviceClass, // device class, -1 for none
                                   bool   checkHealth, // DMCheckHealth after msg?
								   bool displayHTML)  //SR672

: STATE(SystemMessage)(BMP_ERRMSGLEVEL2,
                  LTT_SYSTEMMSG1, // use default title if none
                  true,  // continue button
                  false, // no cancel button
                  szMessage,
                  psMessage,
                  promptText,
                  promptValue,
                  deviceClass,
                  checkHealth,
				  false,
				  false,
				  false,
				  true, 
				  displayHTML)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSystemMessage1Base::SMSystemMessage1Base()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSystemMessage1Base::Deliver(LPCTSTR szMessage,   // string text message or NULL
                                   PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                                   PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
 	                               long   promptValue, // prompt area sale value
                                   long   deviceClass, // device class, -1 for none
                                   bool   checkHealth,
								   bool displayHTML) //SR672
{
  //BMP_ERRMSGLEVEL2 bmp has NO echo box to be displayed after the approval needed screen
  STATE(SystemMessage)::Deliver(BMP_ERRMSGLEVEL2,
              LTT_SYSTEMMSG1, // use default title if none
              true,  // continue button
              false, // no cancel button
              szMessage,
              psMessage,
              promptText,
              promptValue,
              deviceClass,
              checkHealth,
			  false,        // Total
			  true,         // Retracted total - TAR
			  false,
			  true,
			  displayHTML); //SR672

  return this;
}

/////////////////////////////////////////
bool SMSystemMessage1Base::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    
    // There should be ten parameters, a variety of types
    if (dlParameterList.GetDescriptorCount() == 6)
    {
        trace(L6, _T("+SMSystemMessage1Base::DeliverDescriptorList() - Parameter Count of 6"));
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
        trace(L2, _T("+SMSystemMessage1Base::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}

#endif
bool SMSystemMessage1Base::storeMode()
{
    if(fSAInStoreMode)
    {
        return true;
    }
    else
    {
        return false;
    }
}
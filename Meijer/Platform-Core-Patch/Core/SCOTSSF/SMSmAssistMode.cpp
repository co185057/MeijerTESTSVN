//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAssitMode.CPP
//
// TITLE: Class implementation for Assist Mode keyboard
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSmAssistMode.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmAssistMode")

IMPLEMENT_DYNCREATE(SMSmAssistMode, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////////
SMSmAssistMode::SMSmAssistMode()
: SMSmAssistModeBase()
{
  IMPLEMENT_TIMESTAMP
} 


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Method:		SMStateBase  *SMSmAssistMode::PSAssistKey
//
// Purpose:		Send TB the assist mode key press
//
// Parameters:	const long nKey
//
// Returns:		SMStateBase
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SMStateBase  *SMSmAssistMode::PSAssistKey(const long nKey)// Assist Mode or Stop program
{
   USHORT uKey=TBGetKeyCode(nKey);

   trace(L0,_T("+Assist Mode %d"),nKey);
   
   if (uKey) TBKeyPress(uKey);
   TBShow2x20();
   return STATE_NULL;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Method:		SMStateBase  *SMSmAssistMode::PSKeystroke
//
// Purpose:		Precess keystroke for physical keyboard
//
// Parameters:	const long wParam, const long lParam
//
// Returns:		SMStateBase
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SMStateBase *SMSmAssistMode::PSKeystroke(const long wParam, const long lParam)
{
    trace (L6, _T("+SMSmAssistMode::PSKeystroke(0x%08.8x, 0x%08.8x)"), wParam, lParam);
 	
	if(!lParam)  //No shift, control,or alt key pressed
	{
		for(int i = 1; i < 200; i++)
		{
			if(wParam == m_nAssistKeyVirtualKeyValue[i])   
			{
				//Find the key, send keycode to TB
				return PSAssistKey(i);
				
			}
		}
	} 

    SMStateBase* retState = SMSmAssistModeBase::PSKeystroke(wParam, lParam);
    return retState;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Method:		SMStateBase  *SMSmAssistMode::PSNumericKey
//
// Purpose:		Find keycode for numberic keys. Send these keycode to TB 
//
// Parameters:	const long lKey
//
// Returns:		SMStateBase
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SMStateBase  *SMSmAssistMode::PSNumericKey(const long lKey)
{
  trace(L0,_T("+SMSmAssistMode::PSNumericKey %d"),lKey);


  //find keycodes for numberic keys
  for(int i=0; i<200; i++)
  {
	  CString csLabelTemp;
	  TBGetKeyLabel(i, csLabelTemp);
	  if(!csLabelTemp.IsEmpty() && ((lKey-48) == _ttoi(csLabelTemp) && lKey != 48)  // not '0' key
		  || (lKey == 48 && csLabelTemp == "0"))   // process '0' key
	  { 
		//find the matched numberic key
		TBKeyPress(TBGetKeyCode(i));   //Send the keycode to TB
		TBShow2x20();                  //Update 2x20
		return STATE_NULL;
	  }
  }

  trace(L0,_T("-SMSmAssistMode::PSNumericKey Did not find matched key for %d"),lKey);

  return STATE_NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Method:		SMStateBase  *SMSmAssistMode::PSAlphaNumericKey
//
// Purpose:		Find keycode for numberic keys. Send these keycode to TB 
//
// Parameters:	const long lKey
//
// Returns:		SMStateBase
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SMStateBase *SMSmAssistMode::PSAlphaNumericKey(const long lKey)
{
  trace(L0,_T("+SMSmAssistMode::PSAlphaNumericKey %d"),lKey);
  return STATE_NULL;                
}
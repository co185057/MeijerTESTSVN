//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMLookUpItemBase.CPP
//
// TITLE: Class implementation for Look Up Item Alphanumeric Base state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#ifndef _CPPUNIT
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMInProgress.h"           // In progress state
#include "SMEnterQuantity.h"
#include "SMSmAssistMenu.h"
#include "SMCustomMessage.h"
#include "SMPLAEnterWeight.h"          //	enter wgt state

#else  // _CPPUNIT
#endif // _CPPUNIT

#include "SMLookUpItemBase.h"      

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("LookUpItemBase")

IPSProcedures::HKStatus SMLookUpItemBase::m_HKStatus = IPSProcedures::HK_NORMAL;
bool SMLookUpItemBase::m_bShiftOn = false;
bool SMLookUpItemBase::m_bAltGrOn = false;

IMPLEMENT_DYNCREATE(SMLookUpItemBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////
SMLookUpItemBase::SMLookUpItemBase()
{
  IMPLEMENT_TIMESTAMP
  // ++ NXTUI-73 SystemFunctions buttons; Set limit to 250.
  //m_nCharLimit = 3;
  m_nCharLimit = 250;
  // -- NXTUI-73
}

#ifndef _CPPUNIT
///////////////////////////////////////
SMStateBase  *SMLookUpItemBase::Initialize(void)
{
   saMsgFuncPtr = ps.SecurityMessageNonmodal;
   //Update RAP window with SCOT state
   UpdateStateName(ps.GetPSText(TXT_TYPEITEMSDESC, SCOT_LANGUAGE_PRIMARY));

   ps.Message12(false, false);

   DMSayPhrase(TYPEITEMSDESCRIPTION);
   setAnchorState(BES_LOOKUPITEM);

   SAWLDBScannerDisable();   // Disable the scanner

   SetPSContext();
   m_csFrameName = ps.GetCurrentContext();

   trace(L6, _T("Loaded keyboard %s"), m_csFrameName);

   //SetAlphaShiftState(m_csFrameName, m_bShiftOn);

   ps.ShowCMTBText(csTBMessage2Scot);	//thp
   ps.AllowInput(m_nCharLimit,false,false);
   ps.Echo(_T("***"));
   ps.ShowCMFrame();

   //+SR828
   m_pAppPSX->SetConfigProperty(_T("InstructionScreenTitle"), m_csFrameName, UI::PROPERTYTEXT, ps.GetPSText(MSG_TYPEITEMDESC));
   m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"), m_csFrameName, UI::PROPERTYVISIBLE, VARIANT_FALSE);
   m_pAppPSX->SetConfigProperty(_T("Instruction_Line1"), m_csFrameName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
   m_pAppPSX->SetConfigProperty(_T("Instruction_Line1"), m_csFrameName, UI::PROPERTYTEXT, ps.GetPSText(MSG_KEYINACCENTLETTERSINSTRUCTION));

   //ps.PopulateHotKeys(m_csFrameName);
   //ps.PopulateHKSensitiveControls(m_csFrameName, m_HKStatus, m_bShiftOn, m_bAltGrOn);

   ps.ButtonState(_T("CMWeight"), false, false);
   ps.ButtonState(_T("ScaleClassLogo"), false, false);
   ps.ButtonState(_T("CMScaleImage"), false, false);
   //-SR828

   return STATE_NULL; 
}

/////////////////////////////////////
void SMLookUpItemBase::UnInitialize(void)
{

}


//////////////////////////////////////
/* @@@INFOPOINT*/
SMStateBase  *SMLookUpItemBase::PSButton1(void)// cancel
{
  return setAndCreateAnchorState(BES_SCANANDBAG);
}

SMStateBase  *SMLookUpItemBase::PSSpaceKey(void)// 
{
  return PSCharKey(_T(' '));
}

//////////////////////////////////////
SMStateBase  *SMLookUpItemBase::PSButtonGoBack(void)
{ 
   ps.SetFoundPickListItemsFromSearch(false);
   //+sscop-661
   if ( csLastNonHelpStateName == _T("SMScanAndBag") || g_csStateBeforeLookUp == _T("SMScanAndBag") ) { //SR841 - SSCOP-825
	   g_csStateBeforeLookUp = _T("");
		return setAndCreateAnchorState(BES_SCANANDBAG);
   }//-
   else{
		return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
   }

}


//************************************************************************ 
//
// Function:		PSCharKey
// Purpose:			This function handles the button clicks from the alphanumeric keys
// Parameters:    [in] TCHAR c - The character on the alphanumeric key that has been clicked
// Returns:	      SMStateBase  * 
// Calls:
// Side Effects:
// Notes:           
//
//************************************************************************
SMStateBase  *SMLookUpItemBase::PSCharKey(TCHAR c)
{
   ps.EchoInput(c, true, true);
   ps.GetInput(csItemSent);
   int nLen=csItemSent.GetLength();
   if( nLen == m_nCharLimit)
   {
      return PSEnterKey();
   }
   return STATE_NULL;//remain in same state
}

#endif //_CPPUNIT
///////////////////////////////////////
SMStateBase  *SMLookUpItemBase::PSEnterKey(void) // enter key pressed, sell this item
{
   int nCount=ps.DisplaySelectedItem(csItemSent, true);
   if(nCount<=0)
   {
      //reset if search string doesn't have a match
      lLengthOfPreviousEchoData = 0;
#ifndef _CPPUNIT
      CustomMessageObj.csScreenText = ps.GetPSText(MSG_NOMATCHTRYAGAIN);
      CustomMessageObj.csLeadThruText = ps.GetPSText(MSG_ITEMNOTFOUND);
#endif //_CPPUNIT
      RETURNSTATE(CustomMessage)
   }
   else
   {
      ps.SetFoundPickListItemsFromSearch(true);
      return setAndCreateAnchorState(BES_PRODUCEFAVORITES);
   }
}

#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		PSHotKey
// Purpose:			This function handles the button clicks from the hotkeys control
// Parameters:    [in] CString & csHotKey - The name of the hot key that has been clicked
// Returns:	      SMStateBase  * 
// Calls:         PopulateHKSensitiveControls
// Side Effects:
// Notes:           
//
//************************************************************************
SMStateBase  *SMLookUpItemBase::PSHotKey(CString & csHotKey) 
{
  PSProceduresBase::HKStatus newStatus=PSProceduresBase::HK_NORMAL;
  if(csHotKey==_T("HKAcute"))
  {
    newStatus=PSProceduresBase::HK_ACUTEON;
  }
  else if(csHotKey==_T("HKGrave"))
  {
    newStatus=PSProceduresBase::HK_GRAVEON;
  }
  else if(csHotKey==_T("HKCircumflex"))
  {
    newStatus=PSProceduresBase::HK_CIRCUMFLEXON;
  }
  else if(csHotKey==_T("HKUmlaut"))
  {
    newStatus=PSProceduresBase::HK_UMLAUTON;
  }
  else if(csHotKey==_T("HKTilde"))
  {
    newStatus=PSProceduresBase::HK_TILDEON;
  }

  COleVariant vIndex(0L), vReturn;
  
  if(m_HKStatus==newStatus)
  {
    m_HKStatus=PSProceduresBase::HK_NORMAL;
  }
  else
  {
    m_HKStatus=newStatus;
  }


   PSXRC rc=PSX_SUCCESS;
   rc=m_pAppPSX->GetControlProperty( _T("PFKBHotKeys"), UI::PROPERTYBUTTONCOUNT, vIndex );
   if(rc==PSX_SUCCESS)
   {
      long nButtonCount = vIndex.lVal;
      if(nButtonCount)
      {
         m_pAppPSX->SetConfigProperty(_T("PFKBHotKeys"), m_csFrameName, UI::PROPERTYREDRAW, VARIANT_FALSE);
         if(m_HKStatus == PSProceduresBase::HK_NORMAL)
         {
            for( long i = 0L; i < nButtonCount; i++ )
            {
               COleVariant pvParams[2];
               pvParams[0]=i;
               pvParams[1]=(long) UI::Normal;
               m_pAppPSX->SendCommand( _T("PFKBHotKeys"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, pvParams );
            }
         }
         m_pAppPSX->SetConfigProperty(_T("PFKBHotKeys"), m_csFrameName, UI::PROPERTYREDRAW, VARIANT_TRUE);
      }

      ps.PopulateHKSensitiveControls(m_csFrameName, m_HKStatus, m_bShiftOn, m_bAltGrOn);
   }


   return STATE_NULL;//remain in same state

}

void SMLookUpItemBase::SetPSContext()
{
    CString csProduceFavoritesContext;
    if ( co.GetfOperationsAllowMultiPick() )
        csProduceFavoritesContext= _T("MultiSelectLookUpItem") ;
    else
        csProduceFavoritesContext = _T("LookUpItem") ;

    csProduceFavoritesContext = ps.AppendContextWithLCID(csProduceFavoritesContext);
    ps.SetCMFrame(csProduceFavoritesContext);

    SetAlphaShiftState(csProduceFavoritesContext, m_bShiftOn);

    ps.PopulateHotKeys(csProduceFavoritesContext);
    ps.PopulateHKSensitiveControls(csProduceFavoritesContext, m_HKStatus, m_bShiftOn, m_bAltGrOn);

    return;
}


///////////////////////////////////////
SMStateBase  *SMLookUpItemBase::PSClearKey(void)//backspace key
{
  CString csItemSent;
  long lLen = ps.GetInput(csItemSent);
  if (lLen)
  {
    DMSayPhrase(KEYPADVALID);
    ps.EchoInput(0x08, false, true);	// clear or correction key should clear only one character
  }
  else
  {
    DMSayPhrase(KEYPADINVALID);
  }

  return STATE_NULL;
}

/////////////////////////////////////
SMStateBase  *SMLookUpItemBase::PSShiftKey(void)
{
  
  m_bShiftOn=!m_bShiftOn;
  SetAlphaShiftState(m_csFrameName, m_bShiftOn);

  ps.PopulateHKSensitiveControls(m_csFrameName, m_HKStatus, m_bShiftOn, m_bAltGrOn);

  return STATE_NULL;
}

/////////////////////////////////////
SMStateBase  *SMLookUpItemBase::PSAltGrKey(void)
{
  
  m_bAltGrOn=!m_bAltGrOn;

  ps.PopulateHKSensitiveControls(m_csFrameName, m_HKStatus, m_bShiftOn, m_bAltGrOn);

  return STATE_NULL;
}


#endif // _CPPUNIT
/////////////////////////////////////
SMStateBase  *SMLookUpItemBase::DMScanner(void)// scanned item
{
	csItemSent = csDMLastBarCode;
    if (co.IsPicklistAssistEnable() && ps.IsPickListItem(csDMLastBarLabel))
    {
        io.Reset();
      csItemSent = csDMLastBarLabel;
      io.csOrigCode = csItemSent;
      io.csItemCode = csItemSent;
        RETURNSTATE(PLAEnterWeight)
    }
    else
    {
        return TBItemSale(false,0,true);
    }
}





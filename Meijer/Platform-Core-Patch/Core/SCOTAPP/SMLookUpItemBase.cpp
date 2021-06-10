//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMLookUpItemBase.CPP
//
// TITLE: Class implementation for Look Up Item Alphanumeric Base state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMLookUpItemBase.h"      
#include "SMInProgress.h"           // In progress state
#include "SMEnterQuantity.h"
#include "SMSmAssistMenu.h"
#include "SMCustomMessage.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("LookUpItemBase")

IMPLEMENT_DYNCREATE(SMLookUpItemBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////
SMLookUpItemBase::SMLookUpItemBase()
:m_HKStatus(HK_NORMAL),
 m_bShiftOn(false),
 m_bAltGrOn(false)
{
  IMPLEMENT_TIMESTAMP
  m_nCharLimit = 3;	
  m_HKSensitiveControls.Add(CString(_T("Line1AlphaNumericKeys")));
  m_HKSensitiveControls.Add(CString(_T("Line2AlphaNumericKeys")));
  m_HKSensitiveControls.Add(CString(_T("Line3AlphaNumericKeys")));
  m_HKSensitiveControls.Add(CString(_T("Line4AlphaNumericKeys")));
}

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

   m_csFrameName=ps.GetKeyboardContextOfCurrentLang();

   ps.SetCMFrame(m_csFrameName);

   trace(L6, _T("Loaded keyboard %s"), m_csFrameName);

   m_HKStatus=HK_NORMAL;

   m_bShiftOn=false;
   m_bAltGrOn=false;

   SetAlphaShiftState(m_csFrameName, m_bShiftOn);
   m_pAppPSX->SetConfigProperty(_T("AltGrKey"), m_csFrameName, UI::PROPERTYSTATE, (long)UI::Normal);

   ps.ShowCMTBText(csTBMessage2Scot);	//thp
   ps.AllowInput(m_nCharLimit,false,false);
   ps.Echo(_T("***"));
   ps.ShowCMFrame();

   PopulateHotKeys();
   PopulateHKSensitiveControls();

   return STATE_NULL; 
}

/////////////////////////////////////
void SMLookUpItemBase::UnInitialize(void)
{

}

//************************************************************************ 
//
// Function:		PopulateHotKeys
// Purpose:			This function populates the "hotkeys" control (a buttonlist control) on the keyboard
// Parameters:      none
// Returns:	        
// Calls:
// Side Effects:
// Notes:           
//
//************************************************************************
void SMLookUpItemBase::PopulateHotKeys()
{
  COleVariant v, vReturn;

  long nTextCount=0;
  long nDataCount=0;

  CString csText;
  CString csData;

  PSXRC rcText=m_pAppPSX->GetCustomDataVar(_T("Text"), csText, _T("HotKeys"), m_csFrameName);
  PSXRC rcData=m_pAppPSX->GetCustomDataVar(_T("Data"), csData, _T("HotKeys"), m_csFrameName);

  if((PSX_SUCCESS==rcText) && (!csText.IsEmpty()))
  {
    nTextCount=0;
    nTextCount=GetTokenCount(csText, _T(' '));
  }

  if((PSX_SUCCESS==rcData) && (!csData.IsEmpty()))
  {
    nDataCount=0;
    nDataCount=GetTokenCount(csData, _T(' '));
  }

  if(nTextCount!=nDataCount)
  {
    trace(L7, _T("nTextCount %l does not equal nDataCount %l"), nTextCount, nDataCount);
    return;
  }

  COleSafeArray saTextArray;
  saTextArray.Create(VT_BSTR, 1, (DWORD*)&nTextCount);
  nTextCount=0;
  while(!csText.IsEmpty())
  {       
    int nIndex=0;
    nIndex=csText.Find(_T(" "));         
    CString csToken;
    if(nIndex>=0)
    {
      csToken=csText.Left(nIndex);
      csText=csText.Right(csText.GetLength()-nIndex-1);
    }
    else
    {
      csToken=csText;
      csText=_T("");
    }

    BSTR bstr=csToken.AllocSysString();
    saTextArray.PutElement(&nTextCount, bstr);
    ::SysFreeString(bstr);

    nTextCount++;
  }

  COleSafeArray saDataArray;
  saDataArray.Create(VT_VARIANT, 1, (DWORD*)&nDataCount);
  nDataCount=0;
  while(!csData.IsEmpty())
  {       
    int nIndex=0;
    nIndex=csData.Find(_T(" "));         
    CString csToken;
    if(nIndex>=0)
    {
      csToken=csData.Left(nIndex);
      csData=csData.Right(csData.GetLength()-nIndex-1);
    }
    else
    {
      csToken=csData;
      csData=_T("");
    }

    if(csToken.IsEmpty())
     csToken=_T("HKNormal");

    COleVariant strVar(csToken);
    VARIANT vData=strVar.Detach();
    saDataArray.PutElement(&nDataCount, &vData);

    nDataCount++;

    trace(L7, _T("csToken is %s, csData %s"), csToken, csData);         
  }

  v=nTextCount;
  m_pAppPSX->SetConfigProperty(_T("HotKeys"), m_csFrameName, UI::PROPERTYREDRAW, VARIANT_FALSE);
  m_pAppPSX->SetControlProperty( _T("HotKeys"),UI::PROPERTYBUTTONCOUNT, v );

  if( nTextCount )
  {
	  v = saTextArray;
	  m_pAppPSX->SendCommand( _T("HotKeys"), UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );

	  v = saDataArray;
	  m_pAppPSX->SendCommand( _T("HotKeys"), UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );

      for( long i = 0L; i < nTextCount; i++ )
      {
         COleVariant pvParams[2];
         pvParams[0]=i;
         pvParams[1]=(long) UI::Normal;
         m_pAppPSX->SendCommand( _T("HotKeys"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, pvParams );
      }
  }

  m_pAppPSX->SetConfigProperty(_T("HotKeys"), m_csFrameName, UI::PROPERTYREDRAW, VARIANT_TRUE);
}   

//************************************************************************ 
//
// Function:		PopulateHKSensitiveControls
// Purpose:			This function populates the controls in the array m_HKSensitiveControls on the keyboard
// Parameters:    none
// Returns:	        
// Calls:
// Side Effects:
// Notes:           
//
//************************************************************************

void SMLookUpItemBase::PopulateHKSensitiveControls()
{
  CString csCustDataName;
  switch(m_HKStatus)
  {
    case HK_NORMAL:
      csCustDataName=_T("Text");
      break;
    case HK_ACUTEON:
      csCustDataName=_T("TextAcuteOn");
      break;
    case HK_GRAVEON:
      csCustDataName=_T("TextGraveOn");
      break;
    case HK_CIRCUMFLEXON:
      csCustDataName=_T("TextCircumflexOn");
      break;
    case HK_UMLAUTON:
      csCustDataName=_T("TextUmlautOn");
      break;
    case HK_TILDEON:
      csCustDataName=_T("TextTildeOn");
      break;
  }

  if(m_bShiftOn)
     csCustDataName+=_T("ShiftOn");
  if(m_bAltGrOn)
     csCustDataName+=_T("AltGrOn");

  for(int i=0; i<m_HKSensitiveControls.GetSize(); i++)
  {
    CString csCustDataValue=_T("");
    CString csControl=m_HKSensitiveControls[i]; 
    
    PSXRC rc=m_pAppPSX->GetCustomDataVar(csCustDataName, csCustDataValue, csControl, m_csFrameName);

    bool bParseCustDataValue=false;
    if(PSX_SUCCESS==rc && (!csCustDataValue.IsEmpty()))
    {
      bParseCustDataValue=true;
    }
    else
    {
      CString csDefaultText=_T("Text");
      if(m_bShiftOn)
         csDefaultText+=_T("ShiftOn");
      if(m_bAltGrOn)
         csDefaultText+=_T("AltGrOn");

      if(csCustDataName!=csDefaultText )
      {
        PSXRC rc=m_pAppPSX->GetCustomDataVar(csDefaultText, csCustDataValue, csControl, m_csFrameName);
        if(PSX_SUCCESS==rc && (!csCustDataValue.IsEmpty()))
        {
          bParseCustDataValue=true;
        }
      }
    }

    if(bParseCustDataValue)
    {    
      long nCount=0;
      nCount=GetTokenCount(csCustDataValue, _T(' '));

      COleVariant v, vReturn;
      COleSafeArray saTextArray;
      COleSafeArray saDataArray;
      saTextArray.Create(VT_BSTR, 1, (DWORD*)&nCount);
      saDataArray.Create(VT_VARIANT, 1, (DWORD*)&nCount);

      nCount=0;
      while(!csCustDataValue.IsEmpty())
      {       
       int nIndex=0;
       nIndex=csCustDataValue.Find(_T(" "));         
       CString csText;
       if(nIndex>=0)
       {
          csText=csCustDataValue.Left(nIndex);
          csCustDataValue=csCustDataValue.Right(csCustDataValue.GetLength()-nIndex-1);
       }
       else
       {
          csText=csCustDataValue;
          csCustDataValue=_T("");
       }

       BSTR bstr=csText.AllocSysString();
       saTextArray.PutElement(&nCount, bstr);
       ::SysFreeString(bstr);

       CString csData=csText;
       COleVariant strVar(csText+_T(";"));//_T(";") is used as the padding to force psx to return strings for the number keys. 
                                          //We ignore the padding in SMStateBase::PSParse(...) when calling PSCharKey(...)
       VARIANT vData=strVar.Detach();
       saDataArray.PutElement(&nCount, &vData);

       nCount++;
      }

      v=nCount;
      m_pAppPSX->SetConfigProperty(m_HKSensitiveControls[i], m_csFrameName, UI::PROPERTYREDRAW, VARIANT_FALSE);
      m_pAppPSX->SetControlProperty( m_HKSensitiveControls[i], UI::PROPERTYBUTTONCOUNT, 0L );
      m_pAppPSX->SetControlProperty( m_HKSensitiveControls[i], UI::PROPERTYBUTTONCOUNT, v );
      if( nCount )
      {
	      v = saTextArray;
	      m_pAppPSX->SendCommand( m_HKSensitiveControls[i], UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );

	      v = saDataArray;
	      m_pAppPSX->SendCommand( m_HKSensitiveControls[i], UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );
      }

      m_pAppPSX->SetConfigProperty(m_HKSensitiveControls[i], m_csFrameName, UI::PROPERTYREDRAW, VARIANT_TRUE);
    }
  } 
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
   if (fInMultiSelectPickList)
   {
      return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
   }
   else
   {
      return setAndCreateAnchorState(BES_SCANANDBAG);
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


///////////////////////////////////////
SMStateBase  *SMLookUpItemBase::PSEnterKey(void) // enter key pressed, sell this item
{
   int nCount=ps.DisplaySelectedItem(csItemSent, true);
   if(nCount<=0)
   {
      CustomMessageObj.csScreenText = ps.GetPSText(MSG_NOMATCHTRYAGAIN);
      CustomMessageObj.csLeadThruText = ps.GetPSText(MSG_ITEMNOTFOUND);
      RETURNSTATE(CustomMessage)
   }
   else
   {
      ps.SetFoundPickListItemsFromSearch(true);
      return setAndCreateAnchorState(BES_PRODUCEFAVORITES);
   }
}

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
  HKStatus newStatus=HK_NORMAL;
  if(csHotKey==_T("HKAcute"))
  {
    newStatus=HK_ACUTEON;
  }
  else if(csHotKey==_T("HKGrave"))
  {
    newStatus=HK_GRAVEON;
  }
  else if(csHotKey==_T("HKCircumflex"))
  {
    newStatus=HK_CIRCUMFLEXON;
  }
  else if(csHotKey==_T("HKUmlaut"))
  {
    newStatus=HK_UMLAUTON;
  }
  else if(csHotKey==_T("HKTilde"))
  {
    newStatus=HK_TILDEON;
  }

  COleVariant vIndex(0L), vReturn;
  
  if(m_HKStatus==newStatus)
  {
    m_HKStatus=HK_NORMAL;
  }
  else
  {
    m_HKStatus=newStatus;
  }


   PSXRC rc=PSX_SUCCESS;
   rc=m_pAppPSX->GetControlProperty( _T("HotKeys"), UI::PROPERTYBUTTONCOUNT, vIndex );
   if(rc==PSX_SUCCESS)
   {
      long nButtonCount = vIndex.lVal;
      if(nButtonCount)
      {
         m_pAppPSX->SetConfigProperty(_T("HotKeys"), m_csFrameName, UI::PROPERTYREDRAW, VARIANT_FALSE);
         if(m_HKStatus == HK_NORMAL)
         {
            for( long i = 0L; i < nButtonCount; i++ )
            {
               COleVariant pvParams[2];
               pvParams[0]=i;
               pvParams[1]=(long) UI::Normal;
               m_pAppPSX->SendCommand( _T("HotKeys"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, pvParams );
            }
         }
         m_pAppPSX->SetConfigProperty(_T("HotKeys"), m_csFrameName, UI::PROPERTYREDRAW, VARIANT_TRUE);
      }

      PopulateHKSensitiveControls();
   }


   return STATE_NULL;//remain in same state

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

  PopulateHKSensitiveControls();

  return STATE_NULL;
}

/////////////////////////////////////
SMStateBase  *SMLookUpItemBase::PSAltGrKey(void)
{
  
  m_bAltGrOn=!m_bAltGrOn;

  PopulateHKSensitiveControls();

  return STATE_NULL;
}


/////////////////////////////////////
SMStateBase  *SMLookUpItemBase::DMScanner(void)// scanned item
{
	csItemSent = csDMLastBarCode;
  return TBItemSale(false,0,true);
}

long SMLookUpItemBase::GetTokenCount(const CString& csStr, const TCHAR cDelimiter)
{
  long nCount=0;
  for(int i=0; i<csStr.GetLength(); i++)
  {
     if(csStr.GetAt(i)==cDelimiter)
     {
        nCount++;
     }
   }
   if(csStr.GetAt(csStr.GetLength()-1)!=cDelimiter)
   {
     nCount++;
   }
   return nCount;
}







  





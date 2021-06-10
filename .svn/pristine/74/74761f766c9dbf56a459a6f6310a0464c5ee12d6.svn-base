//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMItemQuantityExceededBase.cpp
//
// TITLE: Class implementation for per item quantity exceeded
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "DMProcedures.h"


#include "ScotDataEntry.h"
#include "PSConfigData.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object
#else
#include "FakePSConfigData.h"
#define CPSConfigData CFakePSConfigData
#include "FakeSCOTDataEntry.h"
#define CSCOTDataEntry CFakeSCOTDataEntry
#endif //_CPPUNIT

#include "DataNeededUtil.h"
#include "QueuedActionGenericDelayedIntervention.h"

#include "SMItemQuantityExceeded.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("ItemQuantityExceededBase")

IMPLEMENT_DYNCREATE(SMItemQuantityExceededBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ItemQuantityExceeded)


DEFINE_TIMESTAMP

////////////////////////////////////
SMItemQuantityExceededBase::SMItemQuantityExceededBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////////
SMStateBase  *SMItemQuantityExceededBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ItemQuantityExceeded") );


  // +TAR 407200
  if (fInMultiSelectPickList)
  {
      g_bMultiPickSkipBaggingAllowed = true;
  }
  // -TAR 407200


  DMSayPhrase(QUANTITYLIMITREACHED);

  //lQuantityRestrictedItems++; //SSCOP-526
  if (!g_bInRecallMode)
  {
      CString csDesc, csInstruction;
      csDesc = ps.GetPSText(RA_QUANTITYRESTRICTED_ID, SCOT_LANGUAGE_PRIMARY);
      csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_QUANTITYRESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_ITEM_QUANTITY_EXCEEDED, SCOT_LANGUAGE_PRIMARY), 
          co.IsSecurityDisableOn()==true || co.GetSecurityDisableSubCheckOn() ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_QUANTITYRESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY));
      trace(L7, _T("+ra.OnQuantityRestrictedItem, Message=%s"), csInstruction);
      ra.OnQuantityRestrictedItem(csInstruction);
      trace(L7, _T("-ra.OnQuantityRestrictedItem"));
  }
  else
  {
      return CreateOrUpdateGenericDelayedIntervention();
  }
  if (co.IsImmedInterventionOn())
  {
      return PSButton1();
  }	
  setAnchorState(BES_ITEMQUANTITYEXCEEDED);
  
  // (*) SR898 ====================
  DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                     DM_TRICOLORLIGHT_BLINK_1HZ,
                     DM_TRICOLORLIGHT_NORMALREQUEST); 

  //@@@INFOPOINT
  ps.SetCMFrame(_T("ItemQuantityExceeded"));
  ps.CMButtonFlash(_T("CMButton1Med"),true);   
  
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  //@@@INFOPOINT
  
  ps.Prompt(io.d.csDescription);
	
  //ps.Message13(MSG_RECALLED);
  ps.Message12(MSG_ITEM_QUANTITY_EXCEEDED, true);
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in ItemNotForSaleBase state
}

SMStateBase  *SMItemQuantityExceededBase::PSButton1(void)// ok
{
  //Tar 123351 
  //SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
  if (fInMultiSelectPickList)
	  return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
  else
  {
  
	return setAndCreateAnchorState(BES_SCANANDBAG);
	
  }
}

SMStateBase  *SMItemQuantityExceededBase::PSEnterKey(void)
{
  DMSayPhrase(KEYPADVALID);
  //Tar 123351 
  //SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
  return PSButton1();
}

void SMItemQuantityExceededBase::UnInitialize()
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ItemQuantityExceeded") );
}

SMStateBase  *SMItemQuantityExceededBase::CreateOrUpdateGenericDelayedIntervention()
{
     // MSG_ITEM_QUANTITY_EXCEEDED,        //  495  -- this is for a per item limit exceeded
     // LTT_ITEM_QUANTITY_EXCEEDED,       //  496  -- this is for a per item limit exceeded

    CString csID = _T("QTYRestrict_");
    csID += io.d.csItemCode;

    CQueuedActionGenericDelayedIntervention *pDelayedIntervention = (CQueuedActionGenericDelayedIntervention*) m_GenericDelayedInterventions.Get(csID);
    CSCOTDataEntry *dataEntryQtyRestrict;
    CString csView = _T("");
    CString csINIFile = _T("");
    CString csSection = _T("");
    CString csInstrFmt;

    bool bNewIntervention = true;
    int nItems = 0;
    if (!pDelayedIntervention)
    {  
        csView = CString(_T("ID=")) + csID + _T(";NUMBEROFITEMS=0;");
        csINIFile = co.GetcsDataEntryINIFile();
        csSection = _T("DelayedQtyRestricted");
    }
    else
    {
        csView = pDelayedIntervention->GetActionString();
        ra.OnGenericDelayedApproval(csID, pDelayedIntervention->GetReportingID(), ps.RemoteMode());
        delete pDelayedIntervention;
        pDelayedIntervention = NULL;
        bNewIntervention= false;
    }

    CDataNeededUtil dataNeededUtil(ps.GetPSText(LTT_ITEM_QUANTITY_EXCEEDED), csView, csINIFile, csSection);
    //ScotApp_1908=Quantity limit exceeded for UPC %s (%s).\n Please collect %s unsold items.

    csInstrFmt = ps.GetPSText(MSG_GD_QUANTITY_RESTRICTED, SCOT_LANGUAGE_PRIMARY);
    csInstrFmt.Replace(_T("%%"), _T("%"));

    dataEntryQtyRestrict = dataNeededUtil.GetScotDataEntryObject();
    dataNeededUtil.GetInt(_T("NUMBEROFITEMS"), nItems);

    CString csItemQty;
    csItemQty.Format(_T("%d"), nItems+1);
    dataNeededUtil.GetScotDataEntryObject()->InsertString(_T("NUMBEROFITEMS"),csItemQty);

    CString csInstr;
    CString csDesc = io.d.csDescription;
    /*  //GetDescription() not yet available 
    if (csDesc.IsEmpty())
    {
        csDesc = recallList.GetDescription();
    }
    */

    csInstr.Format(csInstrFmt, io.d.csItemCode, csDesc, csItemQty );

    dataNeededUtil.GetScotDataEntryObject()->InsertString(_T("SUMMARYINSTRUCTION"), csInstr);
    CString csNewViewString = dataNeededUtil.GetScotDataEntryObject()->ToString();

    trace(L6, _T("csNewViewString=%s"), csNewViewString);
    CDataNeededUtil dataNeededUpdated(ps.GetPSText(LTT_ITEM_QUANTITY_EXCEEDED), csNewViewString, _T(""), _T(""));


    SMStateBase* retState = OnGenericDelayedIntervention(dataNeededUpdated);
    if( retState == STATE_NULL)
    {
        retState = PSButton1();
    }
    return retState;
}
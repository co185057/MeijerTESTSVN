// CustomerTBConnectFL.cpp: implementation of the CCustomerTBConnectFL class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "tbdefs.h"
#include "CustomerTBConnectFL.h"
#include "TraceSystem.h"
#include "TBUtilities.h"
#include "CustomerTrxBroker.h"
#include "CustomerMHItem.h"
#include "CustomerTBConnectPOS.h"
#include "CustomerTBProperties.h"

#define T_ID  _T("CCustomerTBConnectFL")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomerTBConnectFL::CCustomerTBConnectFL()
{

}

CCustomerTBConnectFL::~CCustomerTBConnectFL()
{
    
}

TBRC CCustomerTBConnectFL::GetMessageInfo(PSMessageInfo psMessageInfo)
{	
    CCustomerTBProperties & props = *CCustomerTBProperties::instance();
    
    CString csCardScanned = props.GetHookProperty(_T("TMDOrGuestCardScanned"));
    if(csCardScanned == _T("1"))
	{
		props.SetHookProperty(_T("TMDOrGuestCardScanned"), _T("0"));
        CString csEntryID = props.GetHookProperty(_T("TMDMemberEntryID"));
        if( csEntryID == _T("") )
        {
            csEntryID = props.GetHookProperty(_T("NSC4MemberEntryID"));
        }

        if( csEntryID == _T("") )
        {
            // Neither EntryID value is set, go ahead and use the item sold count
            // maintaining backwards compatibility
		    int nEntryID = CCustomerMHItem::instance()->GetNumberOfItemsSold();
		    csEntryID.Format(_T("%d"), nEntryID);
        }
		CCustomerTBConnectPOS::instance()->SetSelectedItem(csEntryID);
	}
	return CBaseTBConnectFL::GetMessageInfo(psMessageInfo);
}
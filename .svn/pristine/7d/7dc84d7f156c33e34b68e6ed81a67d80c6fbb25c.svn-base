// CustomerMHGenMsg.cpp: implementation of the CCustomerMHGenMsg class.
//
// CHANGE HISTORY:
//
// POS154826 Work Request:66559 Name:Saiprasad Srihasam  Date:January 22, 2015 
// POS12127 Work Request:13752 Name:Robert Susanto Date:April 5 2011  
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomerMHGenMsg.h"
#include "CustomerTrxBroker.h"

#define T_ID _T("CCustomerMHGenMsg")


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomerMHGenMsg::CCustomerMHGenMsg()
{

}

CCustomerMHGenMsg::~CCustomerMHGenMsg()
{

}

TBSTATE CCustomerMHGenMsg::ProcessMessage(CTBMsgObj *pMsgObj)
{
    tbtraceIO(_T("ProcessMessage"));

	TBSTATE rc = TB_IGNORE;

	CTBGenMsg *pGenMsg= dynamic_cast<CTBGenMsg*> (pMsgObj);

	if ( pGenMsg->GetTBMsgId() == TBMSG_POSSTATE )
    {
        _bstr_t bstrTemp = pGenMsg->GetValue(_T("Group"));
        CString csTemp = (LPCTSTR)bstrTemp;
        tbtrace( TM_DEBUG, _T("Found PosState Group %s"), csTemp );
        
        if ( csTemp == _T("IGNORE_POSSTATE") )
        {
            bstrTemp = pGenMsg->GetValue(_T("PosState"));
            csTemp = (LPCTSTR) bstrTemp;
            tbtrace( TM_INFO, _T("Ignoring PosState %s!!"), csTemp);
            return TB_IGNORE;
        }
    }
    
	if ( (pGenMsg->GetTBMsgId() == TBMSG_CUSTOM) &&
         (pGenMsg->GetName() == _T("ACSList")) )
    {
        _bstr_t bstrData = pGenMsg->GetValue2(_T("EntryValue"));
        CString csData = (TCHAR*)bstrData;
        CString csPrepaidList;
        CCustomerTrxBroker *pTrx = CCustomerTrxBroker::instance();

        csPrepaidList = pTrx->TBGetProperty(_T("PrepaidCardList"), true);
	    if (csPrepaidList == _T(""))
	    {
		    csPrepaidList = csData;
	    }
	    else
	    {
		    csPrepaidList = csPrepaidList + _T("\t") + csData;
	    }

	    pTrx->TBSetProperty(_T("PrepaidCardList"), csPrepaidList, true);

//(+) POS154826

        CString csOpt = pTrx->TBGetProperty(_T("WICItemList"), true);

        if(csOpt == _T("") || csOpt == _T("0"))
        {
        
        CString csWICList;

        csWICList = pTrx->TBGetProperty(_T("WICList"), true);

	    if (csWICList == _T(""))
	    {
		    csWICList = csData;
	    }
	    else
	    {
		    csWICList = csWICList + _T("\t") + csData;
	    }        

	    pTrx->TBSetProperty(_T("WICList"), csWICList, true);

        }
        else
        {
	       // pTrx->TBSetProperty(_T("WICList"), _T(""), true);
        }
//(-) POS154826

    }
    //pos12127 rrs
    // Get the tax list description and save it into the cstring buffer
    else if ( (pGenMsg->GetTBMsgId() == TBMSG_CUSTOM) &&
         (pGenMsg->GetName() == _T("ACSTaxList")) )
    {
        _bstr_t bstrData = pGenMsg->GetValue2(_T("TaxListDesc"));
        CString csData = (TCHAR*)bstrData;
        CString csTaxList;
        CCustomerTrxBroker *pTrx = CCustomerTrxBroker::instance();

        csTaxList = pTrx->TBGetProperty(_T("TaxListItem"), true);
	    if (csTaxList == _T(""))
	    {
		    csTaxList = csData;
	    }
	    else
	    {
		    csTaxList = csTaxList + _T("|") + csData;
	    }

	    pTrx->TBSetProperty(_T("TaxListItem"), csTaxList, true);
    }
    //e pos12127 rrs
    else
    {
        rc = CSolutionMHGenMsg::ProcessMessage(pMsgObj);
    }

	return rc;
}

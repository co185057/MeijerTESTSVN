// MobileAuditScreenBuilder.cpp: implementation of the CMobileAuditScreenBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"
#include "DllDefine.h"
#include "MobileAuditScreenBuilder.h"
#include "MobileCartAudit.h"
#include "MobileAuditPSDefs.h"

extern CMobileCartAudit mobileAudit;

const CString g_csSummaryInstruction = _T("SUMMARYINSTRUCTION=");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMobileAuditScreenBuilder::CMobileAuditScreenBuilder()
{

}

CMobileAuditScreenBuilder::~CMobileAuditScreenBuilder()
{

}

void CMobileAuditScreenBuilder::GetPartialAuditScreenData(_bstr_t a_bstrtStatus,
                               _bstr_t a_bstrtResult,
                               _bstr_t & a_bstrtSection,
                               _bstr_t & a_bstrtMessage)
{
    BOOL bResult = TRUE;
    CString csStatus = (LPCTSTR) a_bstrtStatus;
    CString csResult = (LPCTSTR) a_bstrtResult;
    bool bSuccess = csResult.Compare(_T("Success")) == 0;
    

    if (bSuccess && csStatus.Compare(_T("PartialRescan"))==0)
    {
        a_bstrtSection = _T("RescanItemSuccess");
        a_bstrtMessage = GetPartialRescanScanMoreItemsString();
    }
    else if (bSuccess && csStatus.Compare(_T("JustPassedMinimumRequirements")) == 0 )
    {
        a_bstrtSection = _T("RescanFinish");
        a_bstrtMessage= GetPartialRescanScanOrFinishString();
    }
    else if (bSuccess && csStatus.Compare(_T("FullRescanAfterError")) == 0)
    {
        a_bstrtSection = _T("FullRescan");
        a_bstrtMessage = GetFullRescanScanMoreItemsString();
    }
    else 
    {
        a_bstrtSection = _T("RescanItemFailed");
        a_bstrtMessage = GetPartialRescanErrorOrFullString();
    }
}

void CMobileAuditScreenBuilder::GetPartialAuditErrorScreenData(_bstr_t a_bstrtStatus,
                                   _bstr_t a_bstrtResult,
                                   _bstr_t & a_bstrtSection,
                                   _bstr_t & a_bstrtMessage)
{
    CString csStatus = (LPCTSTR) a_bstrtStatus;
    CString csResult = (LPCTSTR) a_bstrtResult;
    
    a_bstrtSection = _T("RescanItemFailed");
    a_bstrtMessage = GetPartialRescanErrorOrFullString();
}

void CMobileAuditScreenBuilder::GetFullAuditErrorScreenData(_bstr_t a_bstrtStatus,
                                        _bstr_t a_bstrtResult,
                                        _bstr_t & a_bstrtSection,
                                        _bstr_t & a_bstrtMessage)
{
    CString csStatus = (LPCTSTR) a_bstrtStatus;
    CString csResult = (LPCTSTR) a_bstrtResult;
    
    a_bstrtSection = _T("FullRescanItemFailed");
    a_bstrtMessage = GetFullRescanErrorString();
}

void CMobileAuditScreenBuilder::GetErrorScreenData(_bstr_t a_bstrtStatus,
                                   _bstr_t a_bstrtResult,
                                   _bstr_t & a_bstrtSection,
                                   _bstr_t & a_bstrtMessage)
{
    CString csStatus = (LPCTSTR) a_bstrtStatus;
    CString csResult = (LPCTSTR) a_bstrtResult;
    
    a_bstrtSection = _T("MobileAuditError");
    a_bstrtMessage = GetErrorString();
}

void CMobileAuditScreenBuilder::GetFullAuditScreenData(_bstr_t a_bstrtStatus,
                                   _bstr_t a_bstrtResult,
                                   _bstr_t & a_bstrtSection,
                                   _bstr_t & a_bstrtMessage)
{
    CString csStatus = (LPCTSTR) a_bstrtStatus;
    CString csResult = (LPCTSTR) a_bstrtResult;

    if (csStatus.Compare(_T("FullRescanAfterError"))==0)
    {
            a_bstrtSection = _T("FullRescan");
            a_bstrtMessage = GetFullRescanScanMoreItemsString();
    }
}

void CMobileAuditScreenBuilder::GetEndAuditScreenData(_bstr_t a_bstrtStatus,
                                                      _bstr_t a_bstrtResult,
                                                      _bstr_t & a_bstrtSection,
                                                      _bstr_t & a_bstrtMessage)
{
    CString csResult = (LPCTSTR) a_bstrtResult;
    if (csResult.CompareNoCase(_T("Success"))!=0)
    {
        GetErrorScreenData(a_bstrtStatus, a_bstrtResult, a_bstrtSection, a_bstrtMessage);
        return;
    }
    a_bstrtSection = _T("MobileAuditProcessing");
    a_bstrtMessage = GetEndAuditString();
}

void CMobileAuditScreenBuilder::GetExitAuditScreenData(_bstr_t a_bstrStatus,
                                                      _bstr_t a_bstrtResult,
                                                      _bstr_t & a_bstrtSection,
                                                      _bstr_t & a_bstrtMessage)
{
    a_bstrtSection = _T("MobileAuditProcessing");
    a_bstrtMessage = GetExitingAuditString();
}

void CMobileAuditScreenBuilder::GetAbortAuditSureScreenData(_bstr_t a_bstrStatus,
                                                            _bstr_t a_bstrtResult,
                                                            _bstr_t & a_bstrtSection,
                                                            _bstr_t & a_bstrtMessage)

{
    a_bstrtSection = _T("MobileCancelAlert");
    a_bstrtMessage = _T("");
}

_bstr_t CMobileAuditScreenBuilder::GetPartialRescanScanMoreItemsString()
{
    _bstr_t bstrResult;
    
    CString csNumItemsToScan = (LPCTSTR) mobileAudit.RemainingItems();
    CString csAmountToScan = (LPCTSTR) mobileAudit.RemainingAmount();
    CString csNumberOfItemsInTicket = (LPCTSTR) mobileAudit.NumberOfItemsInTicket();

    
    CString csMessageFormat = ps.GetPSText(MOBILE_AUDIT_SCAN_NUM_ITEMS_TXT, SCOT_LANGUAGE_PRIMARY);
    CString csMessage;
    csMessage.Format(csMessageFormat, csNumItemsToScan);
   // csMessage += _T(";"); SSCOIA-39183
    bstrResult = g_csSummaryInstruction+csMessage;
    
    return bstrResult;
}

_bstr_t CMobileAuditScreenBuilder::GetPartialRescanScanOrFinishString()
{
    _bstr_t bstrResult;
    bstrResult= g_csSummaryInstruction + ps.GetPSText(MOBILE_AUDIT_PARTIAL_AUDIT_COMP_TXT, SCOT_LANGUAGE_PRIMARY); 
    return bstrResult;
}

_bstr_t CMobileAuditScreenBuilder::GetPartialRescanErrorOrFullString()
{
    _bstr_t bstrResult;
    CString csErrorStringName = _T("auditmsg_");
    csErrorStringName += (LPCTSTR) mobileAudit.ResultMessage();
    CString csErrorString = ps.GetPSText(csErrorStringName, SCOT_LANGUAGE_PRIMARY);
    if (csErrorString.IsEmpty())
    {
        csErrorString = (LPCTSTR) mobileAudit.ResultMessage();
    }
    bstrResult = g_csSummaryInstruction + csErrorString + _T("\r\n") + ps.GetPSText(MOBILE_AUDIT_CONTINUE_OR_FULL_TXT, SCOT_LANGUAGE_PRIMARY);
    return bstrResult;
}

_bstr_t CMobileAuditScreenBuilder::GetFullRescanErrorString()
{
    _bstr_t bstrResult;
    CString csErrorStringName = _T("auditmsg_");
    csErrorStringName += (LPCTSTR) mobileAudit.ResultMessage();
    CString csErrorString = ps.GetPSText(csErrorStringName, SCOT_LANGUAGE_PRIMARY);
    if (csErrorString.IsEmpty())
    {
        csErrorString = (LPCTSTR) mobileAudit.ResultMessage();
    }
    bstrResult = g_csSummaryInstruction + csErrorString + _T("\r\n") + ps.GetPSText(MOBILE_AUDIT_ERROR_CONTINUE_FULL, SCOT_LANGUAGE_PRIMARY);
    return bstrResult;
}

_bstr_t CMobileAuditScreenBuilder::GetErrorString()
{
    _bstr_t bstrResult;
    CString csErrorStringName = _T("auditmsg_");
    csErrorStringName += (LPCTSTR) mobileAudit.ResultMessage();
    CString csErrorString = ps.GetPSText(csErrorStringName, SCOT_LANGUAGE_PRIMARY);
    if (csErrorString.IsEmpty())
    {
        csErrorString = (LPCTSTR) mobileAudit.ResultMessage();
    }
    bstrResult = g_csSummaryInstruction + csErrorString + _T("\r\n") + ps.GetPSText(MOBILE_AUDIT_ERROR_TXT, SCOT_LANGUAGE_PRIMARY);
    return bstrResult;
}

_bstr_t CMobileAuditScreenBuilder::GetFullRescanScanMoreItemsString()
{
    _bstr_t bstrResult;
    bstrResult= g_csSummaryInstruction + ps.GetPSText(MOBILE_AUDIT_FULL_AUDIT_TXT, SCOT_LANGUAGE_PRIMARY); 
    return bstrResult;
}

_bstr_t CMobileAuditScreenBuilder::GetEndAuditString()
{
    _bstr_t bstrResult;
    bstrResult= g_csSummaryInstruction + ps.GetPSText(MOBILE_AUDIT_COMPLETING_TXT, SCOT_LANGUAGE_PRIMARY); 
    return bstrResult;
}

_bstr_t CMobileAuditScreenBuilder::GetExitingAuditString()
{
    _bstr_t bstrResult;
    bstrResult= g_csSummaryInstruction + ps.GetPSText(MOBILE_AUDIT_EXITING_TXT, SCOT_LANGUAGE_PRIMARY); 
    return bstrResult;
}


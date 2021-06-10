// CustomerReceiptData.cpp: implementation of the CCustomerReceiptData class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CustomerMHReceipt.h"
#include "tracesystem.h"
#include "CustomerMHOptions.h"
#include <shlwapi.h>

#define T_ID _T("CCustomerMHReceipt")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomerMHReceipt::CCustomerMHReceipt()
{

}

CCustomerMHReceipt::~CCustomerMHReceipt()
{

}

TBSTATE CCustomerMHReceipt::Process_Bitmap( CPrinterMsg* pMsg )
{
    // Change the path of the bitmap to the new storage location
    CString strDest = CCustomerMHOptions::instance()->OptionValue(_T("SigBmpTarget"));
    TCHAR szPath[MAX_PATH];
    _tcscpy(szPath, (LPCTSTR)pMsg->Data());
    PathStripPath(szPath);
    CString strPath = szPath;
    PathCombine(szPath, (LPCTSTR)strDest, (LPCTSTR)strPath);
    strPath = szPath;
    pMsg->Data(strPath);
    
    
    tbtrace(TM_INFO,_T("path to be deleted %s"),strPath);
    TBSTATE tbRet = CSolutionMHReceipt::Process_Bitmap(pMsg);
    
    // clean out bitmap since we're now responsible for deleting it
    DeleteFile((LPCTSTR)strPath);
    
    return tbRet;
}
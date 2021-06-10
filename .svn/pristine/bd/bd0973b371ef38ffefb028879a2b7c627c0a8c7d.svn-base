// CCustomerMHOperator.cpp: implementation of the CCustomerMHOperator class.
//
// CHANGE HISTORY:
// POS105314 Work Request:61233 Name: Saiprasad Srihasam Date: August 07, 2014
// POS105314 Work Request:61233 Name: Saiprasad Srihasam Date: August 05, 2014
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TraceSystem.h"
#include "TBUtilities.h"
#include "CustomerMHOperator.h"
#include "CustomerTBConnectPOS.h"
#include "CustomerMHOptions.h"

#define T_ID _T("CCustomerMHOperator")

const CString SignOnBuilder::TERMINATOR = _T("$$$");
const CString SignOnBuilder::START = _T("{{");
const CString SignOnBuilder::END = _T("}}");
const CString SignOnBuilder::ASSIGN = _T("::");
const CString SignOnBuilder::PARM_USERID = _T("UserId");
const CString SignOnBuilder::PARM_USERPWD = _T("UserPwd");
const CString SignOnBuilder::PARM_USERIDSCANNED = _T("UserIdScanned");
const CString SignOnBuilder::PARM_BIOMETDATA = _T("BiometData");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCustomerMHOperator::CCustomerMHOperator()
{
}

CCustomerMHOperator::~CCustomerMHOperator()
{
}

/*************************************************************************
* ValidateOperator - If this is a valid Operator and PW then return TRUE.
*
* Parameters: 
*  LPCTSTR szOperatorID - Operator ID
*  LPCSTR szOperatorPw - Operator password
*
* Returns: BOOL - TRUE if a valid cashier and PW, FALSE otherwise
*************************************************************************/
bool CCustomerMHOperator::ValidateOperator(LPCTSTR szOperatorID, LPCTSTR szPassword)
{
    tbtraceIO(_T("ValidateOperator"));
    bool rc = false;
    CCustomerTBConnectPOS & con = *CCustomerTBConnectPOS::instance();
    
    if( SignOnBuilder::IsLegacyPassword(szPassword) )
    {
        if( IsEmployeeBadgeCode(szOperatorID) )
        {
            tbtrace(TM_INFO, _T("Received an employee badge that could only have been scanned, using extended API for authentication"));
            rc = con.ACSExValidateEmployeeOperatorString(szOperatorID, szPassword, ACS::ACSEX_OPER_SCANNED) ? true : false;
        }
        else
        {
            tbtrace(TM_INFO, _T("Received a keyed EMPLID and password."));
            rc = con.ACSExValidateEmployeeOperatorString(szOperatorID, szPassword, ACS::ACSEX_OPER_KEYED) ? true : false;
            //rc = CSolutionMHOperator::ValidateOperator(szOperatorID, szPassword);
        }
    }
    else
    {
        tbtrace(TM_INFO, _T("Received the new format password structure, parsing data and passing for validation."));
        SignOnBuilder sob = SignOnBuilder::FromString(szPassword);
        ACS::ExEntryFlag entryFlag = sob.UserIdScanned() ? ACS::ACSEX_OPER_SCANNED : ACS::ACSEX_OPER_KEYED;
        if( sob.BiometData().GetLength() )
        {
            rc = con.ACSExValidateEmployeeOperatorData(sob.UserId(), sob.BiometData(), entryFlag) ? true : false;
        }
        else
        {
            rc = con.ACSExValidateEmployeeOperatorString(sob.UserId(), sob.UserPwd(), entryFlag) ? true : false;
        }
    }
    
    return rc;
}

bool CCustomerMHOperator::ValidateSupervisor(LPCTSTR szOperatorID, LPCTSTR szPassword)
{
    tbtraceIO(_T("ValidateSupervisor"));
    bool rc = false;
    CCustomerTBConnectPOS & con = *CCustomerTBConnectPOS::instance();

    if( SignOnBuilder::IsLegacyPassword(szPassword) )
    {
        if( IsEmployeeBadgeCode(szOperatorID) )
        {
            tbtrace(TM_INFO, _T("Received an employee badge that could only have been scanned, using extended API for authentication"));
            rc = con.ACSExValidateEmployeeSupervisorString(szOperatorID, szPassword, ACS::ACSEX_OPER_SCANNED) ? true : false;
        }
        else
        {
            tbtrace(TM_INFO, _T("Received a keyed EMPLID and password."));
            rc = con.ACSExValidateEmployeeSupervisorString(szOperatorID, szPassword, ACS::ACSEX_OPER_KEYED) ? true : false;
            //rc = CSolutionMHOperator::ValidateSupervisor(szOperatorID, szPassword);
        }
    }
    else
    {
        tbtrace(TM_INFO, _T("Received the new format password structure, parsing data and passing for validation."));
        SignOnBuilder sob = SignOnBuilder::FromString(szPassword);
        ACS::ExEntryFlag entryFlag = sob.UserIdScanned() ? ACS::ACSEX_OPER_SCANNED : ACS::ACSEX_OPER_KEYED;
        if( sob.BiometData().GetLength() )
        {
            rc = con.ACSExValidateEmployeeSupervisorData(sob.UserId(), sob.BiometData(), entryFlag) ? true : false;
        }
        else
        {
            rc = con.ACSExValidateEmployeeSupervisorString(sob.UserId(), sob.UserPwd(), entryFlag) ? true : false;
        }
    }

    return rc;
}

bool CCustomerMHOperator::IsEmployeeBadgeCode(CString sCode)
{
    CString sPrefixList = CCustomerMHOptions::instance()->OptionValue(_T("EmployeeBadgePrefixList"));
    CStringArray sList;
    if( SplitString(sPrefixList, _T(";"), &sList) )
    {
        for(int i = 0; i < sList.GetSize(); i++)
        {
            // starts with one of the listed prefixes
            if( sCode.Find(sList[i]) == 0 )
            {
                return true;
            }
        }
    }
    return false;
}

bool CCustomerMHOperator::VerifyIsEmployeeBadgeAndGetBarcode(const CString & sBarcodeRaw, CString & sBadgeCode)
{
    tbtraceIO(_T("VerifyIsEmployeeBadgeAndGetBarcode"));
    bool bRet = false;
    // Extract the label and type 
    //B3TM51511101~TM51511101~110
    
    CString sCode, sType;
    int index = sBarcodeRaw.Find(_T('~'));
    if( index != -1 )
    {
        sCode = sBarcodeRaw.Mid(index+1);
        index = sCode.Find(_T('~'));
        if( index != -1 )
        {
            sType = sCode.Mid(index+1);
            sCode = sCode.Left(index);
            
			// remove any lingering control characters from the left of the barcode
            for(int i = 0; i < sCode.GetLength(); i++)
            {
                if( sCode[i] == _T('\n') || sCode[i] == _T('\r') )
                {
                    sCode.SetAt(i,_T(' '));
                }
                else
                {
                    break;
                }
            }
            sCode.TrimLeft();
            
            // remove any lingering control characters from the right of the barcode
            for(i = sCode.GetLength()-1; i >= 0; i--)
            {
                if( sCode[i] == _T('\n') || sCode[i] == _T('\r') )
                {
                    sCode.SetAt(i,_T(' '));
                }
                else
                {
                    break;
                }
            }
            sCode.TrimRight();

            CString sTypeList = CCustomerMHOptions::instance()->OptionValue(_T("EmployeeBadgeTypeList"));
            if( sTypeList.Find(sType) != -1 )
            {
                if( IsEmployeeBadgeCode(sCode) )
                {
                    sBadgeCode = sCode;
                    bRet = true;
                }
            }
        }
    }
    // it's possible we already have the actual barcode and not the raw scan data
    else if( IsEmployeeBadgeCode(sBarcodeRaw) )
    {
        sBadgeCode = sCode;
        bRet = true;
    }
    return bRet;
}

//POS105314
int CCustomerMHOperator::ACSCheckInOutOperator(int nFunc, LPCTSTR szOperatorID, LPCTSTR szOperatorPw, int nNewRAPid, int * currentRAPid, int * offlineStatus)
{
    
    tbtraceIO(_T("ACSCheckInOutOperator"));
    int rc = 0;
    CCustomerTBConnectPOS & con = *CCustomerTBConnectPOS::instance();
    
    if( SignOnBuilder::IsLegacyPassword(szOperatorPw) )
    {

            if( IsEmployeeBadgeCode(szOperatorID) )
             {
                CString csEmplID =  con.GetEmplidFromBadge(szOperatorID);
                rc = con.ACSCheckInOutOperator(nFunc, csEmplID, szOperatorPw, nNewRAPid, currentRAPid, offlineStatus);
             }
            else
            {
                rc = con.ACSCheckInOutOperator(nFunc, szOperatorID, szOperatorPw, nNewRAPid, currentRAPid, offlineStatus);
            }
    }
    else
    {
        tbtrace(TM_INFO, _T("Received the new format password structure, parsing data and passing for checkin."));
        SignOnBuilder sob = SignOnBuilder::FromString(szOperatorPw);

             if( IsEmployeeBadgeCode(szOperatorID) )
             {
                CString csEmplID =  con.GetEmplidFromBadge(szOperatorID);
                rc = con.ACSCheckInOutOperator(nFunc, csEmplID, sob.UserPwd(), nNewRAPid, currentRAPid, offlineStatus);
             }
             else
             {
                rc = con.ACSCheckInOutOperator(nFunc, sob.UserId(), sob.UserPwd(), nNewRAPid, currentRAPid, offlineStatus);
             }

    }
    
    return rc;

}
//POS105314


SignOnBuilder SignOnBuilder::FromString(CString serialString)
{
    CString userId = "";
    CString userPwd = "";
    bool userIdScanned = false;
    CString biometData = "";
    
    if( serialString.Left(TERMINATOR.GetLength()) == TERMINATOR )
    {
        if( serialString.Right(TERMINATOR.GetLength()) == TERMINATOR )
        {
            serialString = serialString.Left(serialString.GetLength() - TERMINATOR.GetLength());
        }

        LPTSTR fields = NULL;
       
        CopyString(fields, (LPCTSTR)serialString);
        ASSERT(fields && *fields);
        
        TCHAR * fieldToken = _tcstok(fields, START);
        while( fieldToken )
        {
            CString field = fieldToken;

            int namePos = field.Find(ASSIGN);
            if( namePos > -1 )
            {
                CString name = field.Left(namePos);
                CString val = field.Mid(namePos+ASSIGN.GetLength(), field.GetLength()-(namePos + END.GetLength() + ASSIGN.GetLength()));
                if( name == PARM_USERID )
                {
                    userId = val;
                }
                else if( name == PARM_USERPWD )
                {
                    userPwd = val;
                }
                else if( name == PARM_USERIDSCANNED )
                {
                    userIdScanned = (val == _T("1")); 
                }
                else if( name == PARM_BIOMETDATA )
                {
                    biometData = val;
                }
            }

            fieldToken = _tcstok(NULL, START);
        }
        delete [] fields;
    }
    else
    {
        // this is a standard(legacy) password field
        userPwd = serialString;
    }
    return SignOnBuilder(userId, userPwd, userIdScanned, biometData);
}

CString SignOnBuilder::ToString()
{
    CString output = TERMINATOR;
    
    if( userId != _T("") )
    {
        output += START + PARM_USERID + ASSIGN + userId + END;
    }
    if( userPwd !=_T("") )
    {
        output += START + PARM_USERPWD + ASSIGN + userPwd + END;
    }
    if( userIdScanned )
    {
        output += START + PARM_USERIDSCANNED + ASSIGN + (userIdScanned ? _T("1") : _T("0")) + END;
    }
    if( biometData != _T("") )
    {
        output += START + PARM_BIOMETDATA + ASSIGN + biometData + END;
    }
    output+= TERMINATOR;
    
    return output;
}


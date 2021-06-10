// CustomerMHOperator.h: interface for the CCustomerMHOperator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerMHOPERATOR_H__INCLUDED_)
#define AFX_CustomerMHOPERATOR_H__INCLUDED_

#include "SolutionMHOperator.h"
#include "SingleInstance.h"

class CCustomerMHOperator : public CSolutionMHOperator, 
                            public CSingleInstance<CCustomerMHOperator>
{
    friend class CSingleInstance<CCustomerMHOperator>;

public:
	virtual ~CCustomerMHOperator();

    virtual bool ValidateOperator(LPCTSTR szOperatorID, LPCTSTR szPassword);
    virtual bool ValidateSupervisor(LPCTSTR szOperatorID, LPCTSTR szPassword);

    virtual int ACSCheckInOutOperator(int nFunc, LPCTSTR szOperatorID, LPCTSTR szOperatorPw, int nNewRAPid, int * currentRAPid, int * offlineStatus);   //POS105314


    bool VerifyIsEmployeeBadgeAndGetBarcode(const CString & sBarcodeRaw, CString & sBadgeCode);
    bool IsEmployeeBadgeCode(CString sCode);

protected:
   	CCustomerMHOperator();

private:
    CCustomerMHOperator(CCustomerMHOperator &);                    // hide copy const
    CCustomerMHOperator & operator =(const CCustomerMHOperator &); // hide assignment

};


class SignOnBuilder
{
private:
    static const CString TERMINATOR;
    static const CString START;
    static const CString END;
    static const CString ASSIGN;
    static const CString PARM_USERID;
    static const CString PARM_USERPWD;
    static const CString PARM_USERIDSCANNED;
    static const CString PARM_BIOMETDATA;
    
    CString userId;
    CString userPwd;
    bool userIdScanned;
    CString biometData;

public:
    SignOnBuilder(LPCTSTR userId)
    {
        this->userId = userId;
        this->userPwd = _T("");
        this->userIdScanned = false;
        this->biometData = _T("");
    }
    
    SignOnBuilder(LPCTSTR userId, LPCTSTR userPwd)
    {
        this->userId = userId;
        this->userPwd = userPwd;
        this->userIdScanned = false;
        this->biometData = _T("");
    }
    
    SignOnBuilder(LPCTSTR userId, LPCTSTR userPwd, bool userIdScanned)
    {
        this->userId = userId;
        this->userPwd = userPwd;
        this->userIdScanned = userIdScanned;
        this->biometData = _T("");
    }
    
    SignOnBuilder(LPCTSTR userId, LPCTSTR userPwd, bool userIdScanned, LPCTSTR biometData)
    {
        this->userId = userId;
        this->userPwd = userPwd;
        this->userIdScanned = userIdScanned;
        this->biometData = biometData;
    }
    
    static bool IsLegacyPassword(CString passwordString) { return (passwordString.Left(TERMINATOR.GetLength()) != TERMINATOR); }
    static SignOnBuilder FromString(CString serialString);
    CString ToString();

    inline CString UserId() { return userId; } 
	inline CString UserPwd() { return userPwd; } 
	inline bool UserIdScanned() { return userIdScanned; }
	inline CString BiometData() { return biometData; }

};



#endif 

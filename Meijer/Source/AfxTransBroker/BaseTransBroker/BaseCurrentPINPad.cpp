// BaseCurrentPINPad.cpp: implementation of the CBaseCurrentPINPad class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseCurrentPINPad.h"
#include "TraceSystem.h"

#define T_ID  _T("CBaseCurrentPINPad")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseCurrentPINPad::CBaseCurrentPINPad()
{
    m_PSPINWorkingKey->szAccountNumber = NULL;
    m_PSPINWorkingKey->nAccountNumberLength = 0;
    Set_PWK_AccountNumber(NULL);
    m_PSPINWorkingKey->szWorkingKey = NULL;
    m_PSPINWorkingKey->nWorkingKeyLength = 0;
    Set_PWK_WorkingKey(NULL);
    Set_PWK_PinBlockFormat(TB_ANSI);
    Set_PWK_PinpadSystem(TB_PINSYSTEM_MS);
    Set_PWK_PinBlockEncoding(TB_PINENCODING_COMPATIBLE);
    Set_PWK_Unused3(0);
}

CBaseCurrentPINPad::~CBaseCurrentPINPad()
{    
}

/*************************************************************************
* ResetPINWorkingKey - resets all variables associated with PIN working key
*
* Parameters: none
*
* Returns: member variable value
*************************************************************************/
void CBaseCurrentPINPad::ResetPINWorkingKey()
{
    tbtraceIO(_T("ResetPINWorkingKey()"));

    Set_PWK_AccountNumber(NULL);
    Set_PWK_WorkingKey(NULL);
    Set_PWK_PinBlockFormat(TB_ANSI);
    Set_PWK_PinpadSystem(TB_PINSYSTEM_MS);
    Set_PWK_PinBlockEncoding(TB_PINENCODING_COMPATIBLE);
    Set_PWK_Unused3(0);
}

///////////////////////////////////////////////////////////
//
//  Set Data Functions below :
//                             Set_PWK_AccountNumber
//                             Set_PWK_AccountNumberLength
//                             Set_PWK_WorkingKey
//                             Set_PWK_WorkingKeyLength
//                             Set_PWK_PinBlockFormat
//                             Set_PWK_PinpadSystem
//                             Set_PWK_PinBlockEncoding
//                             Set_PWK_Unused3
//
/////////////////////////////////////////////////////////
void CBaseCurrentPINPad::Set_PWK_AccountNumber(LPTSTR szNewVal)
{
    long len = 0;
    
    tbtrace( TM_ENTRY, _T("+CBaseCurrentPINPad::Set_PWK_AccountNumber[%s]"), szNewVal);
    
    if (szNewVal) {
        len = _tcslen(szNewVal);
    }
    
    if (m_PSPINWorkingKey->szAccountNumber) 
    {
        delete [] m_PSPINWorkingKey->szAccountNumber;
    }
    
    m_PSPINWorkingKey->szAccountNumber = new TCHAR[ len + 1];
    memset(m_PSPINWorkingKey->szAccountNumber, 0, len + 1);
    
    if (len > 0) 
    {
        memcpy(m_PSPINWorkingKey->szAccountNumber, szNewVal, len);
    }
    
    Set_PWK_AccountNumberLength(len);
    
    tbtrace(TM_ENTRY, _T("-CBaseCurrentPINPad::Set_PWK_AccountNumber"));
}

void CBaseCurrentPINPad::Set_PWK_AccountNumberLength(long lNewVal)
{
    tbtrace( TM_ENTRY, _T("CBaseCurrentPINPad::Set_PWK_AccountNumberLength [%d]"), lNewVal);
    m_PSPINWorkingKey->nAccountNumberLength = lNewVal;
}

void CBaseCurrentPINPad::Set_PWK_WorkingKey(LPTSTR szNewVal)
{
    long len = 0;
    
    tbtrace( TM_ENTRY, _T("+CBaseCurrentPINPad::Set_PWK_WorkingKey[%s]"), szNewVal);
    
    if (szNewVal) {
        len = _tcslen(szNewVal);
    }
    
    if (m_PSPINWorkingKey->szWorkingKey) 
    {
        delete [] m_PSPINWorkingKey->szWorkingKey;
    }
    
    m_PSPINWorkingKey->szWorkingKey = new TCHAR[ len + 1];
    memset(m_PSPINWorkingKey->szWorkingKey, 0, len + 1);
    
    if (len > 0) 
    {
        memcpy(m_PSPINWorkingKey->szWorkingKey, szNewVal, len);
    }
    
    Set_PWK_WorkingKeyLength(len);
    
    tbtrace(TM_ENTRY, _T("-CBaseCurrentPINPad::Set_PWK_WorkingKey"));
}

void CBaseCurrentPINPad::Set_PWK_WorkingKeyLength(long lNewVal)
{
    tbtrace( TM_ENTRY, _T("CBaseCurrentPINPad::Set_PWK_WorkingKeyLength [%d]"), lNewVal);
    m_PSPINWorkingKey->nWorkingKeyLength = lNewVal;
}

void CBaseCurrentPINPad::Set_PWK_PinBlockFormat(TBPINBLOCKFORMAT NewVal)
{
    tbtrace( TM_ENTRY, _T("CBaseCurrentPINPad::Set_PWK_PinBlockFormat [%d]"), NewVal);
    m_PSPINWorkingKey->nPinBlockFormat = NewVal;
}

void CBaseCurrentPINPad::Set_PWK_PinpadSystem(TBPINPADSYSTEM NewVal)
{
    tbtrace( TM_ENTRY, _T("CBaseCurrentPINPad::Set_PWK_PinpadSystem [%d]"), NewVal);
    m_PSPINWorkingKey->nPinpadSystem = NewVal;
}

void CBaseCurrentPINPad::Set_PWK_PinBlockEncoding(TBPINENCODING NewVal)
{
    tbtrace( TM_ENTRY, _T("CBaseCurrentPINPad::Set_PWK_PinBlockEncoding [%d]"), NewVal);
    m_PSPINWorkingKey->nPinBlockEncoding = NewVal;
}

void CBaseCurrentPINPad::Set_PWK_Unused3(long lNewVal)
{
    tbtrace( TM_ENTRY, _T("CBaseCurrentPINPad::Set_PWK_Unused3 [%d]"), lNewVal);
    m_PSPINWorkingKey->lUnused3 = lNewVal;
}

/////////////////////////////////////////////////////
//
//  Get Data Functions below : 
//                             Get_PWK_AccountNumber
//                             Get_PWK_AccountNumberLength
//                             Get_PWK_WorkingKey
//                             Get_PWK_WorkingKeyLength
//                             Get_PWK_PinBlockFormat
//                             Get_PWK_PinpadSystem
//                             Get_PWK_PinBlockEncoding
//                             Get_PWK_Unused3
//
//////////////////////////////////////////////////////
LPTSTR CBaseCurrentPINPad::Get_PWK_AccountNumber(void)
{
    tbtrace( TM_ENTRY, _T("CBaseCurrentPINPad::Get_PWK_AccountNumber[%s]"), 
        m_PSPINWorkingKey->szAccountNumber);
    return m_PSPINWorkingKey->szAccountNumber;
}

long CBaseCurrentPINPad::Get_PWK_AccountNumberLength(void)
{
    tbtrace( TM_ENTRY, _T("CBaseCurrentPINPad::Get_PWK_AccountNumberLength[%d]"), 
        m_PSPINWorkingKey->nAccountNumberLength);
    return m_PSPINWorkingKey->nAccountNumberLength;
}

LPTSTR CBaseCurrentPINPad::Get_PWK_WorkingKey(void)
{
    tbtrace( TM_ENTRY, _T("CBaseCurrentPINPad::Get_PWK_WorkingKey[%s]"), 
        m_PSPINWorkingKey->szWorkingKey);
    return m_PSPINWorkingKey->szWorkingKey;
}

long CBaseCurrentPINPad::Get_PWK_WorkingKeyLength(void)
{
    tbtrace( TM_ENTRY, _T("CBaseCurrentPINPad::Get_PWK_WorkingKeyLength[%d]"), 
        m_PSPINWorkingKey->nWorkingKeyLength);
    return m_PSPINWorkingKey->nWorkingKeyLength;
}

TBPINBLOCKFORMAT CBaseCurrentPINPad::Get_PWK_PinBlockFormat(void)
{
    tbtrace( TM_ENTRY, _T("CBaseCurrentPINPad::Get_PWK_PinBlockFormat[%d]"), 
        m_PSPINWorkingKey->nPinBlockFormat);
    return m_PSPINWorkingKey->nPinBlockFormat;
}

TBPINPADSYSTEM CBaseCurrentPINPad::Get_PWK_PinpadSystem(void)
{
    tbtrace( TM_ENTRY, _T("CBaseCurrentPINPad::Get_PWK_PinpadSystem[%d]"), 
        m_PSPINWorkingKey->nPinpadSystem);
    return m_PSPINWorkingKey->nPinpadSystem;
}

TBPINENCODING CBaseCurrentPINPad::Get_PWK_PinBlockEncoding(void)
{
    tbtrace( TM_ENTRY, _T("CBaseCurrentPINPad::Get_PWK_PinBlockEncoding[%d]"), 
        m_PSPINWorkingKey->nPinBlockEncoding);
    return m_PSPINWorkingKey->nPinBlockEncoding;
}

long CBaseCurrentPINPad::Get_PWK_Unused3(void)
{
    tbtrace( TM_ENTRY, _T("CBaseCurrentPINPad::Get_PWK_Unused3[%d]"),
        m_PSPINWorkingKey->lUnused3);
    return m_PSPINWorkingKey->lUnused3;
}

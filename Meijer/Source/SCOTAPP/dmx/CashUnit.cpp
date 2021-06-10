#include "StdAfx.h"
#include "CashUnit.h"
#include "StringTool.h"
#include <sstream>
#include "StringConverter.h"

CCashUnit::CCashUnit()
{
    InitMembers();
}

CCashUnit::CCashUnit(long nDenomination, long nInitialCount)
{
    InitMembers();

    m_nDenomination = nDenomination;
    m_nCount = nInitialCount;
    m_nStartingCount = nInitialCount;
}

CCashUnit::CCashUnit(const tstring &sCashUnit)
{
    InitMembers();

    vector<tstring> vCashData;
    CStringTool::Split(sCashUnit, _T(':'), vCashData);

    if(vCashData.size() == 2)
    {
        m_nDenomination = _ttol(vCashData[0].c_str());
        m_nCount = m_nStartingCount = _ttol(vCashData[1].c_str());
    }
    else
    {
        CStringConverter stringConverter;
        throw(std::invalid_argument(stringConverter.AsString(sCashUnit)));
    }
}

CCashUnit::CCashUnit(long nDenomination, long nInitialCount, bool bIsCoin)
{
    InitMembers();

    m_nDenomination = nDenomination;
    m_nCount = nInitialCount;
    m_nStartingCount = nInitialCount;

    m_bIsCoin = bIsCoin;
}

CCashUnit::CCashUnit(const CCashUnit &rhs)
{
    InitMembers();

    *this = rhs;
}

CCashUnit::~CCashUnit()
{
}

CCashUnit & CCashUnit::operator=(const CCashUnit &rhs)
{
    if(this != &rhs)
    {
        m_nDenomination = rhs.m_nDenomination;
        m_nCount = rhs.m_nCount;
        m_nStartingCount = rhs.m_nStartingCount;
        m_bIsCoin = rhs.m_bIsCoin;
    }

    return *this;
}

CCashUnit & CCashUnit::operator=(long nNewCount)
{
    m_nCount = nNewCount;
    return *this;
}

CCashUnit & CCashUnit::operator+=(long nCount)
{
    m_nCount += nCount;
    return *this;
}

CCashUnit & CCashUnit::operator-=(long nCount)
{
    m_nCount -= nCount;
    return *this;
}

CCashUnit & CCashUnit::operator++(void)
{
    m_nCount++;
    return *this;
}

CCashUnit CCashUnit::operator++(int)
{
    CCashUnit result(*this);
    (void)operator++();

    return result;
}

CCashUnit & CCashUnit::operator--(void)
{
    m_nCount--;
    return *this;
}

CCashUnit CCashUnit::operator--(int)
{
    CCashUnit result(*this);
    (void)operator--();

    return result;
}

bool CCashUnit::operator==(const CCashUnit &rhs) const
{
    bool bResult(false);

    if(m_nDenomination == rhs.m_nDenomination &&
       m_bIsCoin == rhs.m_bIsCoin)
    {
        bResult = true;
    }
    return bResult;
}

bool CCashUnit::operator!=(const CCashUnit &rhs) const
{
    return(! (*this==rhs));
}

bool CCashUnit::operator<(const CCashUnit &rhs) const
{
    bool bResult(false);

    if(m_bIsCoin && ! rhs.m_bIsCoin)
    {
        bResult = true;
    }
    else if(! m_bIsCoin && rhs.m_bIsCoin)
    {
        bResult = false;
    }
    else
    {
        bResult = m_nDenomination < rhs.m_nDenomination;
    }

    return bResult;
}

bool CCashUnit::operator>(const CCashUnit &rhs) const
{
    bool bResult(true);

    if(*this < rhs || *this == rhs)
    {
        bResult = false;
    }
    return bResult;
}

void CCashUnit::SetIsCoin(bool bIsCoin)
{
    m_bIsCoin = bIsCoin;
}

bool CCashUnit::GetIsCoin(void) const
{
    return m_bIsCoin;
}

long CCashUnit::GetDenomination(void) const
{
    return m_nDenomination;
}

void CCashUnit::SetDenomination(long nDenomination)
{
    m_nDenomination = nDenomination;
}

long CCashUnit::GetCount(void) const
{
    return m_nCount;
}

void CCashUnit::SetCount(long nCount)
{
    m_nCount = nCount;
}

long CCashUnit::GetStartingCount(void) const
{
    return m_nStartingCount;
}

void CCashUnit::SetStartingCount(long nStartingCount)
{
    m_nStartingCount = nStartingCount;
}

long CCashUnit::GetChangeInCount(void) const
{
    return m_nCount - m_nStartingCount;
}

long CCashUnit::GetChangeInValue(void) const
{
    long nChangeInCount = GetChangeInCount();
    return m_nDenomination * nChangeInCount;
}

tstring CCashUnit::ToString(void) const
{
    CString csResult;
    csResult.Format(_T("%d:%d"), m_nDenomination, m_nCount);
    return (LPCTSTR)csResult;
#if 0
    tstringstream theStream;
    
    theStream << m_nDenomination << _T(':')
              << m_nCount;

    return theStream.str();
#endif
}

ostream & operator<<(ostream &out, const CCashUnit &cashUnit)
{
    const CStringConverter stringConverter;
    out << stringConverter.AsString(cashUnit.ToString())
        << "; m_nStartingCount= "
        << cashUnit.m_nStartingCount << "; m_bIsCoin="
        << (cashUnit.m_bIsCoin ? "true" : "false");

    return out;
}

void CCashUnit::InitMembers(void)
{
    m_bIsCoin = false;
    m_nDenomination = 0;
    m_nCount = 0;
    m_nStartingCount = 0;
}

#include "StdAfx.h"
#include "CashUnit.h"
#include "CashCounts.h"
#include "StringTool.h"
#include "CashCountsUpdater.h"
#include "CashCountsSummingUpdater.h"
#include <stdexcept>
#include <algorithm>

CCashCounts::CCashCounts()
{
    Init();
}

CCashCounts::CCashCounts(const tstring &sCashCounts)
{
    Init();
    *this = sCashCounts;
}

/*CCashCounts::CCashCounts(const string &sCashCounts)
{
    Init();
    *this = CStringTool::ToString(sCashCounts.c_str());
}*/

CCashCounts::CCashCounts(const CCashCounts &rhs)
{
    Init();
    *this = rhs;
}

CCashCounts::~CCashCounts()
{
    delete m_pUpdater;
}

void CCashCounts::Init(void)
{
    m_pUpdater = new CCashCountsUpdater();
}

CCashCounts & CCashCounts::operator=(const CCashCounts &rhs)
{
    if(this != &rhs)
    {
        m_cashUnits.clear();
        for(int i=0; i<rhs.GetSize(); i++)
        {
            m_cashUnits.push_back(rhs[i]);
        }

        delete m_pUpdater;
        m_pUpdater = rhs.m_pUpdater->MakeCopy();
    }

    return *this;
}

CCashCounts & CCashCounts::operator=(const tstring &sCashCounts)
{
    vector<tstring> vCashStrings;
    CStringTool::Split(sCashCounts, _T(';'), vCashStrings);

    if(vCashStrings.size() > 0)
    {
        if(! vCashStrings[0].empty())
        {
            std::vector<tstring> vCoinPortion;
            CStringTool::Split(vCashStrings[0], _T(','), vCoinPortion);

            if(vCoinPortion.empty())
            {
                vCoinPortion.push_back(vCashStrings[0]);
            }

            AddCashUnits(vCoinPortion, true);
        }

        if(vCashStrings.size() == 2)
        {
            std::vector<tstring> vBillPortion;
            CStringTool::Split(vCashStrings[1], _T(','), vBillPortion);

            if(vBillPortion.empty())
            {
                vBillPortion.push_back(vCashStrings[1]);
            }

            AddCashUnits(vBillPortion, false);
        }
    }

    std::sort(m_cashUnits.begin(), m_cashUnits.end());
    return *this;
}

CCashCounts & CCashCounts::operator=(const CCashUnit &cashUnit)
{
    Update(cashUnit, *m_pUpdater);
    std::sort(m_cashUnits.begin(), m_cashUnits.end());
    return *this;
}

const CCashUnit & CCashCounts::GetBill(long nBillDenomination) const
{
    for(size_t i=0; i<m_cashUnits.size(); i++)
    {
        if(m_cashUnits[i].GetDenomination() == nBillDenomination &&
           ! m_cashUnits[i].GetIsCoin())
        {
            return m_cashUnits[i];
        }
    }

    return m_NullCashUnit;
}

const CCashUnit & CCashCounts::GetCoin(long nCoinDenomination) const
{
    for(size_t i=0; i<m_cashUnits.size(); i++)
    {
        if(m_cashUnits[i].GetDenomination() == nCoinDenomination &&
           m_cashUnits[i].GetIsCoin())
        {
            return m_cashUnits[i];
        }
    }

    return m_NullCashUnit;
}

const CCashUnit & CCashCounts::operator[](size_t nIndex) const
{
    return m_cashUnits.at(nIndex);
}

CCashUnit & CCashCounts::operator[](size_t nIndex)
{
    return m_cashUnits.at(nIndex);
}

const bool CCashCounts::operator==(const CCashCounts &rhs) const
{
    bool bResult(false);
    if(GetSize() == rhs.GetSize())
    {
        bResult = true;
        for(size_t i=0; i<GetSize(); i++)
        {
            if((*this)[i] != rhs[i] ||
               (*this)[i].GetCount() != rhs[i].GetCount() ||
               (*this)[i].GetChangeInCount() != rhs[i].GetChangeInCount())
            {
                bResult = false;
                break;
            }
        }
    }
    return bResult;
}

void CCashCounts::AddCashUnits(const std::vector<tstring> &vData,
                               const bool bIsCoin)
{
    for(size_t i=0; i<vData.size(); i++)
    {
        try
        {
            CCashUnit cashUnit(vData[i]);
            cashUnit.SetIsCoin(bIsCoin);
            Update(cashUnit, *m_pUpdater);

        } catch(std::invalid_argument &)
        {
            // FIXME>  Log warning here!
        }
    }
}

void CCashCounts::Update(const CCashCounts &rhs)
{
    if (this != &rhs)
    {
        for(size_t i=0; i<rhs.GetSize(); i++)
        {
            Update(rhs[i], *m_pUpdater);
        }
    }

    std::sort(m_cashUnits.begin(), m_cashUnits.end());
}

void CCashCounts::Update(const CCashUnit &cashUnit,
                         ICashCountsUpdater &updater)
{
    bool bExists(false);

    for(size_t i=0; i<m_cashUnits.size(); i++)
    {
        if(updater.Update(cashUnit, m_cashUnits[i]))
        {
            bExists = true;
            break;
        }
    }

    if(! bExists)
    {
        m_cashUnits.push_back(cashUnit);
    }
}

size_t CCashCounts::GetSize(void) const
{
    return m_cashUnits.size();
}

size_t CCashCounts::GetCoinSize(void) const
{
    size_t nCoinCount(0);
    for(size_t i=0; i<m_cashUnits.size(); i++)
    {
        if(m_cashUnits[i].GetIsCoin())
        {
            ++nCoinCount;
        }
    }

    return nCoinCount;
}

size_t CCashCounts::GetBillSize(void) const
{
    size_t nBillCount(0);
    for(size_t i=0; i<m_cashUnits.size(); i++)
    {
        if(! m_cashUnits[i].GetIsCoin())
        {
            ++nBillCount;
        }
    }

    return nBillCount;
}

tstring CCashCounts::ToString(void) const
{
    tstring sResult(CoinsToString());
    sResult.erase(sResult.end()-1);
    sResult += BillsToString();

    return(sResult);
}

tstring CCashCounts::BillsToString(void) const
{
    tstring sResult(_T(";"));
    for(std::vector<CCashUnit>::size_type i=0; i<m_cashUnits.size(); i++)
    {
        if(! m_cashUnits[i].GetIsCoin())
        {
            sResult += m_cashUnits[i].ToString();
            if(i < m_cashUnits.size()-1)
            {
                sResult += _T(',');
            }
        }
    }

    return sResult;
}

tstring CCashCounts::CoinsToString(void) const
{
    tstring sResult;
    for(std::vector<CCashUnit>::size_type i=0; i<m_cashUnits.size(); i++)
    {
        if(m_cashUnits[i].GetIsCoin())
        {
            sResult += m_cashUnits[i].ToString();
            if(i+1 < m_cashUnits.size())
            {
                if(m_cashUnits[i+1].GetIsCoin())
                {
                    sResult += _T(',');
                }
            }
        }
    }

    sResult += _T(';');
    return sResult;
}

void CCashCounts::Clear(void)
{
    m_cashUnits.clear();
}

void CCashCounts::ZeroCoinCounts(void)
{
    for(size_t i=0; i<m_cashUnits.size(); i++)
    {
        if(m_cashUnits[i].GetIsCoin())
        {
            m_cashUnits[i].SetCount(0);
        }
    }
}

void CCashCounts::ZeroBillCounts(void)
{
    for(size_t i=0; i<m_cashUnits.size(); i++)
    {
        if(! m_cashUnits[i].GetIsCoin())
        {
            m_cashUnits[i].SetCount(0);
        }
    }
}

long CCashCounts::GetCoinCounts(void) const
{
    long nResult(0);
    for(size_t i=0; i<m_cashUnits.size(); i++)
    {
        if(m_cashUnits[i].GetIsCoin())
        {
            nResult += m_cashUnits[i].GetCount();
        }
    }

    return nResult;
}

long CCashCounts::GetBillCounts(void) const
{
    long nResult(0);
    for(size_t i=0; i<m_cashUnits.size(); i++)
    {
        if(! m_cashUnits[i].GetIsCoin())
        {
            nResult += m_cashUnits[i].GetCount();
        }
    }

    return nResult;
}

long CCashCounts::GetCoinValue(void) const
{
    long nResult(0);
    for(size_t i=0; i<m_cashUnits.size(); i++)
    {
        if(m_cashUnits[i].GetIsCoin())
        {
            nResult += (m_cashUnits[i].GetCount() * m_cashUnits[i].GetDenomination());
        }
    }

    return nResult;
}

long CCashCounts::GetBillValue(void) const
{
    long nResult(0);
    for(size_t i=0; i<m_cashUnits.size(); i++)
    {
        if(! m_cashUnits[i].GetIsCoin())
        {
            nResult += (m_cashUnits[i].GetCount() * m_cashUnits[i].GetDenomination());
        }
    }

    return nResult;
}

long CCashCounts::GetCoinChangeInCount(void) const
{
    long nResult(0);
    for(size_t i=0; i<m_cashUnits.size(); i++)
    {
        if(m_cashUnits[i].GetIsCoin())
        {
            nResult += m_cashUnits[i].GetChangeInCount();
        }
    }

    return nResult;
}

long CCashCounts::GetBillChangeInCount(void) const
{
    long nResult(0);
    for(size_t i=0; i<m_cashUnits.size(); i++)
    {
        if(! m_cashUnits[i].GetIsCoin())
        {
            nResult += m_cashUnits[i].GetChangeInCount();
        }
    }

    return nResult;
}

long CCashCounts::GetCoinChangeInValue(void) const
{
    long nResult(0);
    for(size_t i=0; i<m_cashUnits.size(); i++)
    {
        if(m_cashUnits[i].GetIsCoin())
        {
            nResult += m_cashUnits[i].GetDenomination() * m_cashUnits[i].GetChangeInCount();
        }
    }

    return nResult;
}

long CCashCounts::GetBillChangeInValue(void) const
{
    long nResult(0);
    for(size_t i=0; i<m_cashUnits.size(); i++)
    {
        if(! m_cashUnits[i].GetIsCoin())
        {
            nResult +=
                m_cashUnits[i].GetDenomination() * m_cashUnits[i].GetChangeInCount();
        }
    }

    return nResult;
}

const CCashCounts & CCashCounts::operator+=(const CCashCounts &rhs)
{
    CCashCountsSummingUpdater summingUpdater;
    for(size_t i=0; i<rhs.GetSize(); i++)
    {
        Update(rhs[i], summingUpdater);
    }

    std::sort(m_cashUnits.begin(), m_cashUnits.end());

    return *this;
}

const CCashCounts & CCashCounts::operator+=(const CCashUnit &cashUnit)
{
    CCashCountsSummingUpdater summingUpdater;
    Update(cashUnit, summingUpdater);
    std::sort(m_cashUnits.begin(), m_cashUnits.end());

    return *this;
}

const CCashCounts & CCashCounts::operator-=(const CCashCounts &rhs)
{
    CCashCountsSummingUpdater summingUpdater;
    for(size_t i=0; i<rhs.GetSize(); i++)
    {
        const CCashUnit negatedCashUnit(rhs[i].GetDenomination(),
                                        -rhs[i].GetCount(),
                                        rhs[i].GetIsCoin());
        Update(negatedCashUnit, summingUpdater);
    }
    std::sort(m_cashUnits.begin(), m_cashUnits.end());

    return *this;
}

bool CCashCounts::HaveCountsChanged(void) const
{
    bool bResult(false);
    for(size_t i=0; i<m_cashUnits.size(); i++)
    {
        if(m_cashUnits[i].GetChangeInCount() != 0)
        {
            bResult = true;
            break;
        }
    }
    return bResult;
}


CCashCounts::const_iterator CCashCounts::begin() const
{
    return m_cashUnits.begin();
}

CCashCounts::const_iterator CCashCounts::end() const
{
    return m_cashUnits.end();
}

CCashCounts::iterator CCashCounts::begin()
{
    return m_cashUnits.begin();
}

CCashCounts::iterator CCashCounts::end()
{
    return m_cashUnits.end();
}

void CCashCounts::SetUpdater(ICashCountsUpdater &updater)
{
    if(m_pUpdater != &updater)
    {
        delete m_pUpdater;
        m_pUpdater = &updater;
    }
}

const ICashCountsUpdater & CCashCounts::GetUpdater(void) const
{
    return *m_pUpdater;
}

/**
* Subtract second from first where denominations are the same.
*/
CCashCounts CCashCounts::SubtractLikeDenoms(const CCashCounts &first, const CCashCounts &second)
{
     CCashCounts cashCountResult(first.ToString());

     for(size_t i=0; i<cashCountResult.GetSize(); i++)
     {
         const long nDenom = cashCountResult[i].GetDenomination();
         if(cashCountResult[i].GetIsCoin())
         {
             cashCountResult[i] -= second.GetCoin(nDenom).GetCount();
         }
         else
         {
             cashCountResult[i] -= second.GetBill(nDenom).GetCount();
         }
     }

     return cashCountResult;
}

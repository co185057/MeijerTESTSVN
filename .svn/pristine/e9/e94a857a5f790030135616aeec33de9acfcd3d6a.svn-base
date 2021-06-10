#ifndef _CCASH_COUNTS_H
#define _CCASH_COUNTS_H

#include <vector>
#include <ostream>
#include <memory>

#include "CashUnit.h"

class ICashCountsUpdater;

using std::vector;

class CCashCounts
{
public:
    CCashCounts();
    CCashCounts(const tstring &sCashCounts);
    //CCashCounts(const string &sCashCounts);//commented out to fix ansi compile error
    CCashCounts(const CCashCounts &rhs);
    virtual ~CCashCounts();

    CCashCounts & operator=(const CCashCounts &rhs);
    CCashCounts & operator=(const tstring &sCashCounts);
    CCashCounts & operator=(const CCashUnit &cashUnit);

    const CCashUnit & GetBill(long nBillDenomination) const;
    const CCashUnit & GetCoin(long nCoinDenomination) const;
    const CCashUnit & operator[](size_t nIndex) const;
    CCashUnit & operator[](size_t nIndex);
    const bool operator==(const CCashCounts &rhs) const;
    size_t GetSize(void) const;
    size_t GetCoinSize(void) const;
    size_t GetBillSize(void) const;
    tstring ToString(void) const;
    tstring BillsToString(void) const;
    tstring CoinsToString(void) const;
    void Clear(void);
    void ZeroCoinCounts(void);
    void ZeroBillCounts(void);
    long GetCoinCounts(void) const;
    long GetBillCounts(void) const;
    long GetCoinValue(void) const;
    long GetBillValue(void) const;
    long GetCoinChangeInCount(void) const;
    long GetBillChangeInCount(void) const;
    long GetCoinChangeInValue(void) const;
    long GetBillChangeInValue(void) const;
    const CCashCounts & operator+=(const CCashCounts &rhs);
    const CCashCounts & operator+=(const CCashUnit &cashUnit);
    const CCashCounts & operator-=(const CCashCounts &rhs);

    /**
     * \return true if the counts for any denomination have changed.
     */
    bool HaveCountsChanged(void) const;
    void Update(const CCashCounts &rhs);
    CCashCounts SubtractLikeDenoms(const CCashCounts &first, const CCashCounts &second);

    typedef std::vector<CCashUnit>::const_iterator const_iterator;
    typedef std::vector<CCashUnit>::iterator iterator;

    const_iterator begin() const;
    const_iterator end() const;
    iterator begin();
    iterator end();

    void SetUpdater(ICashCountsUpdater &updater);
    const ICashCountsUpdater & GetUpdater(void) const;

private:
    vector<CCashUnit> m_cashUnits;
    ICashCountsUpdater *m_pUpdater;
    const CCashUnit m_NullCashUnit;

    void AddCashUnits(const std::vector<tstring> &vData, const bool bIsCoin);
    void Init(void);
    void Update(const CCashUnit &cashUnit, ICashCountsUpdater &updater);
};
#endif // _CCASH_COUNTS_H

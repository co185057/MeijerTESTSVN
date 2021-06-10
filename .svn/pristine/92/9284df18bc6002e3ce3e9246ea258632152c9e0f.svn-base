#ifndef _CCASHUNIT_H
#define _CCASHUNIT_H

/**
 * \class CCashUnit
 * \brief Simple class that allows access and storage of denomination/count
 *        pairs.
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */

#include <ostream>
#include "tstring.h"

using std::string;
using std::ostream;

class CCashUnit
{
public:
    CCashUnit();
    CCashUnit(long nDenomination, long nInitialCount);
    CCashUnit(long nDenomination, long nInitialCount, bool bIsCoin);
    explicit CCashUnit(const tstring &sCashUnit);
    CCashUnit(const CCashUnit &rhs);
    virtual ~CCashUnit();

    CCashUnit & operator=(const CCashUnit &rhs);
    CCashUnit & operator=(long nNewCount);

    CCashUnit & operator+=(long nCount);
    CCashUnit & operator-=(long nCount);
    CCashUnit & operator++(void);   // Prefix
    CCashUnit operator++(int);      // Postfix
    CCashUnit & operator--(void);   // Prefix
    CCashUnit operator--(int);      // Postfix

    bool operator==(const CCashUnit &rhs) const;
    bool operator!=(const CCashUnit &rhs) const;
    bool operator<(const CCashUnit &rhs) const;
    bool operator>(const CCashUnit &rhs) const;

    void SetIsCoin(bool bIsCoin);
    bool GetIsCoin(void) const;

    long GetDenomination(void) const;
    void SetDenomination(long nDenomination);

    long GetCount(void) const;
    void SetCount(long nCount);

    long GetStartingCount(void) const;
    void SetStartingCount(long nStartingCount);

    long GetChangeInCount(void) const;
    long GetChangeInValue(void) const;

    tstring ToString(void) const;
    friend ostream & operator<<(ostream &out, const CCashUnit &cashUnit);

private:
    long m_nDenomination;
    long m_nCount;
    long m_nStartingCount;
    bool m_bIsCoin;

    void InitMembers(void);
};

#endif // _CCASHUNIT_H

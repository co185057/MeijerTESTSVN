#ifndef _CCASH_COUNTS_SUMMING_UPDATER_H
#define _CCASH_COUNTS_SUMMING_UPDATER_H

#include "ICashCountsUpdater.h"
#include "CashUnit.h"

class CCashCountsSummingUpdater : public ICashCountsUpdater
{
public:
    CCashCountsSummingUpdater();
    virtual ~CCashCountsSummingUpdater();
    virtual bool Update(const CCashUnit &cashUnit, CCashUnit &toBeUpdated);
    virtual ICashCountsUpdater * MakeCopy(void) const;
};

#endif // _CCASH_COUNTS_SUMMING_UPDATER_H

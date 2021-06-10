#ifndef _CCASH_COUNTS_UPDATER_H
#define _CCASH_COUNTS_UPDATER_H

#include "ICashCountsUpdater.h"
#include "CashUnit.h"

class CCashCountsUpdater : public ICashCountsUpdater
{
public:
    CCashCountsUpdater();
    virtual ~CCashCountsUpdater();
    virtual bool Update(const CCashUnit &cashUnit, CCashUnit &toBeUpdated);
    virtual ICashCountsUpdater * MakeCopy(void) const;
};

#endif // _CCASH_COUNTS_UPDATER_H

#include "StdAfx.h"
#include "CashCountsUpdater.h"

CCashCountsUpdater::CCashCountsUpdater()
{
}

CCashCountsUpdater::~CCashCountsUpdater()
{
}

bool CCashCountsUpdater::Update(const CCashUnit &cashUnit,
                                CCashUnit &toBeUpdated)
{
    bool bResult(false);

    if(cashUnit == toBeUpdated)
    {
        toBeUpdated = cashUnit.GetCount();
        bResult = true;
    }
    return bResult;
}

ICashCountsUpdater * CCashCountsUpdater::MakeCopy(void) const
{
    return new CCashCountsUpdater();
}

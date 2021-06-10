#include "StdAfx.h"
#include "CashCountsSummingUpdater.h"

CCashCountsSummingUpdater::CCashCountsSummingUpdater()
{
}

CCashCountsSummingUpdater::~CCashCountsSummingUpdater()
{
}

bool CCashCountsSummingUpdater::Update(const CCashUnit &cashUnit,
                                       CCashUnit &toBeUpdated)
{
    bool bResult(false);

    if(cashUnit == toBeUpdated)
    {
        toBeUpdated += cashUnit.GetCount();
        bResult = true;
    }
    return bResult;
}

ICashCountsUpdater * CCashCountsSummingUpdater::MakeCopy(void) const
{
    return new CCashCountsSummingUpdater();
}

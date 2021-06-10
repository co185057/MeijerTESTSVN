#ifndef _ICASH_COUNTS_UPDATER_H
#define _ICASH_COUNTS_UPDATER_H

class CCashUnit;

/**
 * \brief Handles updates to internal data of CCashCounts.
 */
class ICashCountsUpdater
{
public:
    ICashCountsUpdater() {}
    virtual ~ICashCountsUpdater() {}

    /**
     * Update the internal data of the CCashCounts object.
     * \param[in] cashUnit Data that is used for the updated.
     * \param[in,out] toBeUpdated Reference to cashUnit that will be updated.
     * \return true if update was performed.  false if no update performed.
     */
    virtual bool Update(const CCashUnit &cashUnit, CCashUnit &toBeUpdated) = 0;

    /**
     * \return A newly-allocated ICashCountsUpdater that is of the same
     *         type/implementation of this updater.
     */
    virtual ICashCountsUpdater * MakeCopy(void) const = 0;
};

#endif // _ICASH_COUNTS_UPDATER_H

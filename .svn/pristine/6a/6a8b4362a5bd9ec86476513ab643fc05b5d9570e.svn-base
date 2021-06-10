#ifndef _CFPMAP_H
#define _CFPMAP_H

#include <map>

/**
 * \class CFPMap
 * \brief Template class that enables convenient storage of a value using
 *        a string as an index.  This is currently used to index function
 *        pointers.
 */
template <class TKEY, class TVAL>
class CFPMap 
{
public:
    /**
     * Constructor
     * \param[in] notfount - The value to return if a key is not found.
     */
    CFPMap(TVAL notfound)  : m_nfid(notfound) {}    
    ~CFPMap() { m_hash.clear(); }

    /**
     * Array-index operator.
     * \param[in] skey - String key to lookup value.
     * \return A reference to the stored value if skey is found or notfound
     *         if the key is not found.
     */
    TVAL & operator[](TKEY skey)
    {
        if(m_hash.find(skey) == m_hash.end())
            m_hash[skey] = m_nfid;

        return m_hash[skey];
    }

    /**
     * Const array-index operator.
     * \param[in] skey - String key to lookup value.
     * \return A copy of the stored value if skey is found or notfound
     *         if the key is not found.
     */
    TVAL operator[](TKEY skey) const
    {
        if(m_hash.find(skey) == m_hash.end())
            return m_nfid;

        return m_hash[skey];
    }

    /**
     * Dispose of all entries.
     */
    void clear(void)
    {
        m_hash.clear();
    }

private:
    mutable std::map<TKEY, TVAL> m_hash;
    TVAL m_nfid;                    ///< Value to return if key is not found.
};

#endif // _CFPMAP_H

//  CustomerCurrentJournal.h: interface for the CCustomerCurrentJournal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentJournal_H__INCLUDED_)
#define AFX_CustomerCurrentJournal_H__INCLUDED_

#include "BaseCurrentJournal.h"
#include "SingleInstance.h"

class CCustomerCurrentJournal : public CBaseCurrentJournal,
                                public CSingleInstance<CCustomerCurrentJournal>
{
    friend class CSingleInstance<CCustomerCurrentJournal>;

public:
    virtual ~CCustomerCurrentJournal();

protected:
    CCustomerCurrentJournal();

private:
    CCustomerCurrentJournal(CCustomerCurrentJournal &);                    // hide copy const
    CCustomerCurrentJournal & operator =(const CCustomerCurrentJournal &); // hide assignment
};

#endif 

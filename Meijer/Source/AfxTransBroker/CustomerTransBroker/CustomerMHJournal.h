// CustomerMHJournal.h: interface for the CCustomerMHJournal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerMHJournal_H__INCLUDED_)
#define AFX_CustomerMHJournal_H__INCLUDED_

#include "BaseMHJournal.h"
#include "SingleInstance.h"

class CCustomerMHJournal : public CBaseMHJournal, public CSingleInstance<CCustomerMHJournal>
{
    friend class CSingleInstance<CCustomerMHJournal>;

public:
    virtual ~CCustomerMHJournal();
    
protected:
    CCustomerMHJournal();
    
private:
    CCustomerMHJournal(CCustomerMHJournal &);                    // hide copy const
    CCustomerMHJournal & operator =(const CCustomerMHJournal &); // hide assignment
    
};

#endif 

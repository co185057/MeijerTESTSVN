// CustomerMHDisplay.h: interface for the CCustomerMHDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerMHDISPLAY_H__INCLUDED_)
#define AFX_CustomerMHDISPLAY_H__INCLUDED_

#include "SolutionMHDisplay.h"
#include "SingleInstance.h"

class CCustomerMHDisplay : public CSolutionMHDisplay, public CSingleInstance<CCustomerMHDisplay>
{
   friend class CSingleInstance<CCustomerMHDisplay>;

public:
   virtual ~CCustomerMHDisplay();

protected:
   CCustomerMHDisplay();  // should not be called outside of this class (instance function)
    
private:
   CCustomerMHDisplay(const CCustomerMHDisplay &);                    // hide copy const
   CCustomerMHDisplay & operator =(const CCustomerMHDisplay &); // hide assignment

};

#endif


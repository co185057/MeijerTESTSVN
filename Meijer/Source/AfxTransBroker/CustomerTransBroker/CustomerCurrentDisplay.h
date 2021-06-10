// CustomerCurrentDisplay.h: interface for the CCustomerCurrentDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentDISPLAY_H__INCLUDED_)
#define AFX_CustomerCurrentDISPLAY_H__INCLUDED_

#include "SolutionCurrentDisplay.h"
#include "SingleInstance.h"

class CCustomerCurrentDisplay : public CSolutionCurrentDisplay, 
                                public CSingleInstance<CCustomerCurrentDisplay>
{
   friend class CSingleInstance<CCustomerCurrentDisplay>;

public:
    virtual ~CCustomerCurrentDisplay();
    
    
protected:
    CCustomerCurrentDisplay();  // should not be called outside of this class (instance function)
    
private:
    CCustomerCurrentDisplay(CCustomerCurrentDisplay &);                    // hide copy const
    CCustomerCurrentDisplay & operator =(const CCustomerCurrentDisplay &); // hide assignment
    
};

#endif // !defined(AFX_CustomerCurrentDISPLAY_H__INCLUDED_)


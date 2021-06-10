// CustomerCurrentItem.h: interface for the CustomerCurrentItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCustomerCurrentItem_H__INCLUDED_)
#define AFX_CCustomerCurrentItem_H__INCLUDED_

#include "BaseCurrentItem.h"
#include "SingleInstance.h"
#include "TBGenMsg.h"
#include <vector>

using namespace std;


//typedef CArray<CTBGenMsg, CTBGenMsg&> SItemDetailsArray;
//typedef vector<CTBGenMsg*> SItemDetailsArray;

class CCustomerCurrentItem : public CBaseCurrentItem, 
                             public CSingleInstance<CCustomerCurrentItem>
{
    friend class CSingleInstance<CCustomerCurrentItem>;

public:
    virtual ~CCustomerCurrentItem();

    //SItemDetailsArray 
	vector<CTBGenMsg*>	m_SItemArray;

protected:
   	CCustomerCurrentItem();
    
private:
    CCustomerCurrentItem(CCustomerCurrentItem &);                    // hide copy const
    CCustomerCurrentItem & operator =(const CCustomerCurrentItem &); // hide assignment
};

#endif // !defined(AFX_CCustomerCurrentItem_H__E637CFF0_214F_42FB_B383_FEB3C527D1A8__INCLUDED_)

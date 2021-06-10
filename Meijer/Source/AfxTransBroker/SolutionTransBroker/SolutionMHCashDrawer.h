// SolutionMHCashDrawer.h: interface for the CSolutionMHCashDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SolutionMHCashDrawer_H__INCLUDED_)
#define AFX_SolutionMHCashDrawer_H__INCLUDED_

#include "BaseMHCashDrawer.h"
#include "SingleInstance.h"

class CSolutionMHCashDrawer : public CBaseMHCashDrawer 
{
public:
    virtual ~CSolutionMHCashDrawer();
    virtual void ClosePOSCashDrawer(CCashDrawerMsg::CashDrawerStations cdStation );
    virtual TBSTATE ProcessMessage(CTBMsgObj *pMsg);

protected:
    CSolutionMHCashDrawer();

private:
    CSolutionMHCashDrawer(CSolutionMHCashDrawer &);                    // hide copy const
    CSolutionMHCashDrawer & operator =(const CSolutionMHCashDrawer &); // hide assignment

};

#endif 

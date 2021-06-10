// CustomerMHGenMsg.h: interface for the CCustomerMHGenMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMERMHGENMSG_H__2E3E4FEF_B039_4EB0_BEDC_5E294114F1D4__INCLUDED_)
#define AFX_CUSTOMERMHGENMSG_H__2E3E4FEF_B039_4EB0_BEDC_5E294114F1D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SolutionMHGenMsg.h"
#include "singleinstance.h"

class CCustomerMHGenMsg : public CSolutionMHGenMsg, public CSingleInstance<CCustomerMHGenMsg>  
{
   friend class CSingleInstance<CCustomerMHGenMsg>;

public:
	virtual ~CCustomerMHGenMsg();
	TBSTATE ProcessMessage(CTBMsgObj *pMsgObj);

protected:
	CCustomerMHGenMsg();
private:
   CCustomerMHGenMsg(const CCustomerMHGenMsg &);                    // hide copy const
   CCustomerMHGenMsg & operator =(const CCustomerMHGenMsg &); // hide assignment

};

#endif // !defined(AFX_CUSTOMERMHGENMSG_H__2E3E4FEF_B039_4EB0_BEDC_5E294114F1D4__INCLUDED_)

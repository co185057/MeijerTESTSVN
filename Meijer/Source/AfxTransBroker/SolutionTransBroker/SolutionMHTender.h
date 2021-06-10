// SolutionMHTender.h: interface for the CSolutionMHTender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SolutionMHTender_H__INCLUDED_)
#define AFX_SolutionMHTender_H__INCLUDED_

#include "BaseMHTender.h"
#include "SingleInstance.h"

class CSolutionMHTender : public CBaseMHTender
{

public:
    virtual ~CSolutionMHTender();

    virtual TBSTATE HandleTenderEntry(LPARAM lparam);
    virtual TBSTATE ProcessMessage(CTBMsgObj *pMsgObj);

    TBSTATE HandleMessage300(bool bInItemization = false);
	virtual void HandleTenderAccepted(void);
	virtual TBRC GetTenderDetails(PSTenderDetails psTenderDetails);
protected:
    CSolutionMHTender();  // should not be called outside of this class (instance function)

    virtual int FindResponseCode(const CString &csResponseCode, TBTENDERTYPE tendertype);
        
private:
    CSolutionMHTender(const CSolutionMHTender &);              // hide copy const
    CSolutionMHTender & operator =(const CSolutionMHTender &); // hide assignment
    
};

#endif 


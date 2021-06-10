// SolutionMHDisplay.h: interface for the CSolutionMHDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SolutionMHDISPLAY_H__INCLUDED_)
#define AFX_SolutionMHDISPLAY_H__INCLUDED_

#include "BaseMHDisplay.h"
#include "SingleInstance.h"

class CSolutionMHDisplay : public CBaseMHDisplay
{
	
public:
	virtual ~CSolutionMHDisplay();
	virtual TBSTATE ProcessMessage(CTBMsgObj *pMsgObj);
	
protected:
	CSolutionMHDisplay();  // should not be called outside of this class (instance function)
    
private:
	CSolutionMHDisplay(const CSolutionMHDisplay &);                    // hide copy const
	CSolutionMHDisplay & operator =(const CSolutionMHDisplay &); // hide assignment
	
};

#endif


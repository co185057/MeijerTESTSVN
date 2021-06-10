// BaseMHPrinter.h: interface for the CBaseMHPrinter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEMHPRINTER_H__INCLUDED_)
#define AFX_BASEMHPRINTER_H__INCLUDED_

#include "MsgHandler.h"

class CBaseMHPrinter : public CMsgHandler
{
public:
    virtual ~CBaseMHPrinter();
    
	virtual TBRC GetPrinterError(LPTSTR* psErrorMessage);
    virtual TBRC Print(LPCTSTR szPrintFile);
    // This is required functionality for all MH classes
    virtual TBSTATE ProcessMessage(CTBMsgObj *pMsgObj);
    
protected:
    CBaseMHPrinter();
    
private:
    CBaseMHPrinter(const CBaseMHPrinter &);                    // hide copy const
    CBaseMHPrinter & operator =(const CBaseMHPrinter &); // hide assignment
};

#endif

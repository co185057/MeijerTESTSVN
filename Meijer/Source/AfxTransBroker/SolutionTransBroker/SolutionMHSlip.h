// SolutionMHSlip.h: interface for the CSolutionMHSlip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SolutionSLIPDATA_H__INCLUDED_)
#define		 AFX_SolutionSLIPDATA_H__INCLUDED_

#include "BaseMHSlip.h"
#include "SingleInstance.h"

#define SLIPSTATUS_SLIPDESC 4

class CSolutionMHSlip : public CBaseMHSlip
{
public:
	virtual ~CSolutionMHSlip();


	virtual TBRC EndorsementCompleted(bool bSuppressSubsequent);
	virtual TBRC GetDocEndorsementInfo(long   *plNumRemaining,
                                       LPTSTR *ppszDocDescription, 
                                       PTSTR  *ppszDocFront, 
                                       LPTSTR *ppszDocBack);

    // Methods for handling Printer message function codes
    //virtual TBSTATE Process_CutPaper( CPrinterMsg* pMsg );
    virtual TBSTATE Process_Print( CPrinterMsg* pMsg );
    //virtual TBSTATE Process_Barcode( CPrinterMsg* pMsg );
    //virtual TBSTATE Process_Bitmap( CPrinterMsg* pMsg );
    //virtual TBSTATE Process_SetBitmap( CPrinterMsg* pMsg );
    //virtual TBSTATE Process_SetLogo( CPrinterMsg* pMsg );
    virtual TBSTATE Process_SlipStatus( CPrinterMsg* pMsg );

	virtual HFILE GetSlipData(void);
	virtual TBRC ReleaseSlipData(HFILE hSlipData);

protected:
	CSolutionMHSlip();

    bool    m_bFront;

private:
    CSolutionMHSlip(CSolutionMHSlip &);                    // hide copy const
    CSolutionMHSlip & operator =(const CSolutionMHSlip &); // hide assignment
};

#endif 

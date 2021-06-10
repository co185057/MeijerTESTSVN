// CustomerCurrentSlip.h: interface for the CCustomerCurrentSlip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentSlip_H__INCLUDED_)
#define AFX_CustomerCurrentSlip_H__INCLUDED_

#include "SolutionCurrentSlip.h"
#include "SingleInstance.h"

class CCustomerCurrentSlip : public CSolutionCurrentSlip,
                                public CSingleInstance<CCustomerCurrentSlip>
{
    friend class CSingleInstance<CCustomerCurrentSlip>;

public:
    virtual ~CCustomerCurrentSlip();
	TBRC EndorsementCompleted(bool bSuppressSubsequent);
	TBRC GetDocEndorsementInfo(long   *plNumRemaining,
                                       LPTSTR *ppszDocDescription, 
                                       PTSTR  *ppszDocFront, 
                                       LPTSTR *ppszDocBack);
    void SetSlipDescription( CString csDesc );
    void FinishSlip(void);
    
    // DG30 - New for Check21, CheckHandling & CheckFlip 
    bool ShouldEjectSlipAfterMicr(void);
    bool ShouldEjectSlipBeforePrint(bool bJustFlippedToBack);
    bool ShouldFlipSlipBeforePrint(bool bCurrentSlipSideFront);
    bool ShouldFlipSlipAfterMicr(void);

public: 
    enum SlipType
    {
        SlipTypeCheckBack      = 0,
        SlipTypeCheckFront     = 1,
        SlipTypeCheckDenial    = 2,
        SlipTypeEBT            = 3,
        SlipTypeNegDept        = 4,
        SlipTypeTaxExempt      = 5
    };
    int GetLastSlipType(void) { return m_iSlipType; }

protected:
    CCustomerCurrentSlip();

    int IdentifySlipType(const CString & csData, CString & csRetDesc);
    CString RotateSlip( CString csData );

private:
    CCustomerCurrentSlip(CCustomerCurrentSlip &);                    // hide copy const
    CCustomerCurrentSlip & operator =(const CCustomerCurrentSlip &); // hide assignment

protected:
    int m_iSlipType;

};

#endif 

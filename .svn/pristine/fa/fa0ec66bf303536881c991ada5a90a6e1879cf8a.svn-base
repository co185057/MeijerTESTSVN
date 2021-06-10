// SolutionCurrentSlip.h: interface for the CSolutionCurrentSlip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SolutionCurrentSlip_H__INCLUDED_)
#define AFX_SolutionCurrentSlip_H__INCLUDED_

#include "BaseCurrentSlip.h"
#include "TransBroker.h"

#define SLIP_MAX_INDEX  3
#define SLIP_INDEX_DESC 0
#define SLIP_INDEX_FRONT 1
#define SLIP_INDEX_BACK 2


class CSolutionCurrentSlip : public CBaseCurrentSlip
{
public:
    virtual ~CSolutionCurrentSlip();

	// FastLane Endorsement methods
	virtual TBRC EndorsementCompleted(bool bSuppressSubsequent);
	virtual TBRC GetDocEndorsementInfo(long   *plNumRemaining,
                                       LPTSTR *ppszDocDescription, 
                                       PTSTR  *ppszDocFront, 
                                       LPTSTR *ppszDocBack);

	// TB Internal methods
    virtual long GetNumRemaining(void);
	virtual bool PurgeAllSlips(void);
	virtual CStringArray* GetNewSlip(void);
    virtual void SetEndorseWhenFinished( bool bVal );
    virtual bool GetEndorseWhenFinished(void);
    virtual bool IsSlipPrinting(void);
    virtual bool IsInProgress(void);

    virtual void SetSlipDescription( CString csDesc );
    virtual void WriteToSlipFront( CString csData );
    virtual void WriteToSlipBack( CString csData );
    virtual void FinishSlip(void);

protected:
    CSolutionCurrentSlip();
    virtual CString RotateSlip( CString csData );

    CObList			m_lstSlips;
    bool            m_bIsPrinting;
	CStringArray*	m_pSlipData;

private:
    CSolutionCurrentSlip(CSolutionCurrentSlip &);                    // hide copy const
    CSolutionCurrentSlip & operator =(const CSolutionCurrentSlip &); // hide assignment

    bool            m_bEndorseWhenFinished;
    CString         m_csSavedSlipDesc;
};

#endif 

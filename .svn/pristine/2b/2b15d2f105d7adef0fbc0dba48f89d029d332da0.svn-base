// SolutionCurrentSlip.cpp: implementation of the CSolutionCurrentSlip class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SolutionCurrentSlip.h"
#include "TraceSystem.h"
#include "ReturnStateMsg.h"
#include "CustomerTBScotMsg.h"
#include "CustomerTrxBroker.h"
#include "CustomerMsgMap.h"
#include "CustomerCurrentReceipt.h"
#include "BaseMessages.h"
#include "CustomerMHOptions.h"

#define T_ID  _T("CSolutionCurrentSlip")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSolutionCurrentSlip::CSolutionCurrentSlip() :
    m_pSlipData(NULL),
    m_bEndorseWhenFinished(false),
    m_bIsPrinting(false)
{
    PurgeAllSlips();
}

CSolutionCurrentSlip::~CSolutionCurrentSlip()
{
    PurgeAllSlips();
}

TBRC CSolutionCurrentSlip::EndorsementCompleted(bool bSuppressSubsequent)
{
    tbtraceIO(_T("EndorsementCompleted"));
    TBRC rc = TB_SUCCESS;

    CStringArray* pArr = (CStringArray*)m_lstSlips.RemoveHead();
    delete pArr;
    m_bIsPrinting = false;

    if ( GetNumRemaining() > 0 )
    {
        if (!bSuppressSubsequent)
        {
            CReturnStateMsg *pStateMsg = new CReturnStateMsg(TB_ENDORSEDOCUMENT); 
            // delete done in ReleaseMessageLParam()
            CCustomerTBScotMsg::instance()->SendScotMsg( pStateMsg);
        }
    }
    else if ( IsInProgress() )
    {
        SetEndorseWhenFinished( true );
    }
    else
    {
        // All endorsements are done.  Check for any receipts
	    CString  csCurTBState = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();
        tbtrace( TM_INFO, _T("All slips done in state '%s'"), csCurTBState );
        if ( -1 != csCurTBState.Find(_T("ASSISTMODE_TRXEND")) )
        {
            TBSTATE tbRet = TB_TRXEND;
            CCustomerMsgMap::instance()->msgAssistModeTrxEnd( tbRet, NULL );
            if ( tbRet != TB_IGNORE )
            {
                CCustomerTBScotMsg::instance()->SendReturnStateMsg( tbRet, NULL, NULL );
            }
        }
    }
    return rc;
}
     

#define SCUDMSG_TXT_ENDORSE  9045
     
TBRC CSolutionCurrentSlip::GetDocEndorsementInfo(long   *plNumRemaining,
                                       LPTSTR *ppszDocDescription, 
                                       PTSTR  *ppszDocFront, 
                                       LPTSTR *ppszDocBack)
{
	long lChit = *plNumRemaining;

    // The following looks really weird for handling NumRemaining; but it prevents
	// Fastlane from incorrectly kicking out a MICR'd check that needs endorsement.
	if ( GetNumRemaining() > 0 )
	{
	    *plNumRemaining = 1;

		CStringArray* pArr = (CStringArray*)m_lstSlips.GetHead();
		if ( NULL != pArr )
		{
			CString csTemp = CBaseMessages::instance()->GetStringValueFromID( SCUDMSG_TXT_ENDORSE );

            //
            // Slip descriptions for multiple endorsements may come back to back before
            // the first endorsement has completed, as such we can lose the original
            // slip description. To prevent this from happening, a second slip description
            // that comes in will be saved off if a first one has been applied. Thus if
            // we're in this method and there is no current description then look if there
            // is one saved off and use that one instead.
            //
            if( (pArr->GetAt(SLIP_INDEX_DESC).GetLength() == 0) 
                && (m_csSavedSlipDesc.GetLength() > 0) )
            {
                tbtrace(TM_INFO, _T("Slip has no description although a description was previously received and saved off.  Using it...")); 
                pArr->SetAt(SLIP_INDEX_DESC, m_csSavedSlipDesc);
                m_csSavedSlipDesc = _T("");
            }

			CString csPrompt;
			csPrompt.Format( csTemp, pArr->GetAt(SLIP_INDEX_DESC) );
			CopyString(*ppszDocDescription, csPrompt);
			CopyString(*ppszDocFront, pArr->GetAt(SLIP_INDEX_FRONT));

			CString csBack;

            CCustomerTrxBroker & trxBroker = *CCustomerTrxBroker::instance();

			// Handle rotated slips for Check printing
			if ( ( -1 != pArr->GetAt(SLIP_INDEX_DESC).Find(_T("CHECK Front")) ) &&
                 ( lChit != -42 ) &&
                 ( trxBroker.InRemoteAssistMode() || trxBroker.IsCheckPaymentAtLaneAvailable() )  )
			{
				csBack = RotateSlip( pArr->GetAt(SLIP_INDEX_BACK) );
			}
			else
			{
				csBack = pArr->GetAt(SLIP_INDEX_BACK);
			}

			CopyString(*ppszDocBack, csBack);
			m_bIsPrinting = true;
		}
	}
	else
	{
		*plNumRemaining = 0;
		if ( IsInProgress() )
		{
			SetEndorseWhenFinished( true );
		}
	}

	return TB_SUCCESS;
}


long CSolutionCurrentSlip::GetNumRemaining(void)
{
    tbtraceIO(_T("GetNumRemaining") );
    long lRet = 0;

    lRet = m_lstSlips.GetCount();

    return lRet;
}

bool CSolutionCurrentSlip::PurgeAllSlips(  )
{
    tbtraceIO(_T("PurgeAllSlips") );

    while ( !m_lstSlips.IsEmpty() )
    {
        CStringArray* pArr = (CStringArray*)m_lstSlips.RemoveHead();
        delete pArr;
    }

    m_csSavedSlipDesc = _T("");

    return true;
}

CStringArray* CSolutionCurrentSlip::GetNewSlip(void)
{
    tbtraceIO(_T("GetNewSlip"));
    CStringArray* pRet = new CStringArray();
    pRet->SetSize(SLIP_MAX_INDEX);
	pRet->GetAt(SLIP_INDEX_FRONT).Empty();
	pRet->GetAt(SLIP_INDEX_BACK).Empty();
	pRet->GetAt(SLIP_INDEX_DESC).Empty();
    return pRet;
}

bool CSolutionCurrentSlip::IsSlipPrinting(void)
{
    tbtraceIO(_T("IsSlipPrinting"));
    return m_bIsPrinting;
}

bool CSolutionCurrentSlip::IsInProgress(void)
{
    tbtraceIO(_T("IsInProgress"));
	bool bRet = false;

	if ( m_pSlipData )
	{
		bRet = !m_pSlipData->GetAt(SLIP_INDEX_FRONT).IsEmpty() ||
			   !m_pSlipData->GetAt(SLIP_INDEX_BACK).IsEmpty();
	}
	return bRet;

}

void CSolutionCurrentSlip::SetEndorseWhenFinished( bool bVal )
{
    tbtraceIO(_T("SetEndorseWhenFinished"));
    m_bEndorseWhenFinished = bVal;
}

bool CSolutionCurrentSlip::GetEndorseWhenFinished( void )
{
    tbtraceIO(_T("GetEndorseWhenFinished"));
    return m_bEndorseWhenFinished;
}

void CSolutionCurrentSlip::SetSlipDescription( CString csDesc )
{
    tbtraceIO(_T("SetSlipDescription"));
    if ( !m_pSlipData )
    {
        m_pSlipData = GetNewSlip();
    }

    if ( m_pSlipData )
    {
        // Race condition may cause a Slip Description for the NEXT slip to 
        // arrive before the current one has completed processing. If a slip
        // description has already been applied, save off the one that just
        // came in.
        if( m_pSlipData->GetAt(SLIP_INDEX_DESC).GetLength() > 0 )
        {
            tbtrace(TM_WARNING, _T("Received second slip description before slip has been printed. Saving it off.")); 
            m_csSavedSlipDesc = csDesc;
        }
        else
        {
		    m_pSlipData->SetAt( SLIP_INDEX_DESC, csDesc );
            m_csSavedSlipDesc = _T("");
        }
    }
    else
    {
        tbtrace( TM_ERROR, _T("Failed to allocate new slip") );
    }
}

void CSolutionCurrentSlip::WriteToSlipFront( CString csData )
{
    tbtraceIO(_T("WriteToSlipFront"));
    if ( !m_pSlipData )
    {
        m_pSlipData = GetNewSlip();
    }

    if ( m_pSlipData )
    {
        CString csSlipData = m_pSlipData->GetAt(SLIP_INDEX_FRONT);
        csSlipData += csData;
		m_pSlipData->SetAt(SLIP_INDEX_FRONT, csSlipData );
    }
    else
    {
        tbtrace( TM_ERROR, _T("Failed to allocate new slip") );
    }
}

void CSolutionCurrentSlip::WriteToSlipBack( CString csData )
{
    tbtraceIO(_T("WriteToSlipBack"));
    if ( !m_pSlipData )
    {
        m_pSlipData = GetNewSlip();
    }

    if ( m_pSlipData )
    {
        CString csSlipData = m_pSlipData->GetAt(SLIP_INDEX_BACK);
        csSlipData += csData;
		m_pSlipData->SetAt( SLIP_INDEX_BACK, csSlipData );
    }
    else
    {
        tbtrace( TM_ERROR, _T("Failed to allocate new slip") );
    }
}

void CSolutionCurrentSlip::FinishSlip(void)
{
    tbtraceIO(_T("FinishSlip"));

    if( IsInProgress() )
    {
        m_lstSlips.AddTail( m_pSlipData );
        m_pSlipData = NULL;

		if ( GetEndorseWhenFinished())
		{
			if (!IsSlipPrinting() && !CCustomerCurrentReceipt::instance()->IsPrinting())
			{
				CReturnStateMsg *pStateMsg = new CReturnStateMsg(TB_ENDORSEDOCUMENT); // delete done in ReleaseMessageLParam()
				CCustomerTBScotMsg::instance()->SendScotMsg( pStateMsg);
			}
		}
    }
}

CString CSolutionCurrentSlip::RotateSlip( CString csData )
{
	tbtraceIO(_T("RotateSlip"));
	long lPOS0 = 0;
	long lPOS1 = 0;
	long lMaxLen = 0;
	bool bProcessing = true;
	CStringArray arLines;
	arLines.RemoveAll();
	CString token;
	int nIgnore = 0;
	bool bFirst = true;

    // Greater adjustment value will move the check print higher up on the check.
    // 3 - 7158HW emulation
    // 13 - 7167 mode
    int nAdjustment = CCustomerMHOptions::instance()->IntOptionValue(_T("CheckFrontTopAdjust"));
    if( (nAdjustment < 0) || (nAdjustment > 20) )
    {
        nAdjustment = 3;
    }

	CString csNewData = _T("\x1B\x12");
	while( bProcessing )
	{
		token.Empty();
	    lPOS1 = csData.Find(_T("\x0a"), lPOS0);
		if (lPOS1 != -1)
		{
			token = csData.Mid(lPOS0, lPOS1-lPOS0);
	        lPOS0 = lPOS1+1;
		}
	    else
		{
			token = csData.Mid(lPOS0);
			bProcessing = false;
		}
		if( token.GetLength() > lMaxLen )
		{
			lMaxLen = token.GetLength();
		}
		if ( !token.IsEmpty() )
		{
			// Ignore ESC sequences
			if ( token.GetAt(0) != 0x1B )
			{
				if ( 0 == nIgnore )
				{
					if ((token.GetLength() > 5) && bFirst)
					{
						bFirst= false;
						token += _T("    ");
					}

					token.MakeReverse();
					arLines.Add( token );
				}
				else
				{
					nIgnore--;
				}
			}
			else
			{
				nIgnore = nAdjustment;
			}
		}
    }

	for( int i=0; i<lMaxLen; i++)
	{
		CString csNewLine = "";
		for( int j=arLines.GetSize(); j>0; j-- )
		{
			if ( arLines.GetAt(j-1).GetLength() > i )
			{
				csNewLine += arLines.GetAt(j-1).GetAt(i);
			}
			else
			{
				csNewLine += _T(" ");
			}
		}
		csNewLine.MakeReverse();
		csNewData += csNewLine + _T("\x0A");
	}
	csNewData += _T("\x1BV0\x0A");

	return csNewData;
}
// CustomerCurrentSlip.cpp: implementation of the CCustomerCurrentSlip class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CustomerCurrentSlip.h"
#include "TraceSystem.h"
#include "CustomerTBScotMsg.h"
#include "CustomerTrxBroker.h"
#include "CustomerMsgMap.h"
#include "CustomerCurrentCashDrawer.h"
#include "CustomerMHOptions.h"
#include "BaseMessages.h"
#include "CustomerCurrentReceipt.h"
#include "CustomerTBProperties.h"
#include "CurrentPosState.h"


#define T_ID  _T("CCustomerCurrentSlip")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomerCurrentSlip::CCustomerCurrentSlip()
: m_iSlipType(-1)
{
}

CCustomerCurrentSlip::~CCustomerCurrentSlip()
{
    
}

TBRC CCustomerCurrentSlip::EndorsementCompleted(bool bSuppressSubsequent)
{
	tbtraceIO(_T("EndorsementCompleted"));
    TBRC rc = TB_SUCCESS;

	// TAR 358196 set this variable to false now. It is checked before cash drawer will open   
    m_bIsPrinting = false;

	CString  csCurTBState = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();

	//this is just to prevent the solution level to execute the following line
	//CCustomerMsgMap::instance()->msgAssistModeTrxEnd( tbRet, NULL ); //TAR 320512
	//calling the msgAssistModeTrxEnd already handled through TBMsgMap entry
    if ( -1 != csCurTBState.Find(_T("ASSISTMODE_TRXEND")) )
    {
		CCustomerTrxBroker::instance()->SetCurrentInProgressState(_T("ASSISTMODE_TEMP_TRXEND"));
    }

    //TAR334738 -- Can't call CSolutionCurrentSlip::EndorsementCompleted
    //             unless there are slips left to print
    if ( GetNumRemaining() > 0 )
    {
        CSolutionCurrentSlip::EndorsementCompleted(bSuppressSubsequent);
    }
    else
    {
        if ( IsInProgress() )
        {
            SetEndorseWhenFinished( true );
        }
    }

	csCurTBState = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();
	
	//this is to let FastLane complete its transaction after the endorsement prompt from the lane and
	//completed in assistmode.
	CString csIsFromLaneEndorsement = CCustomerTrxBroker::instance()->TBGetProperty(_T("IsFromEndorsement"), false);

	CString csIsTrainingMode = CCustomerTrxBroker::instance()->TBGetProperty(_T("TrainingMode"));

    if ( -1 != csCurTBState.Find(_T("ASSISTMODE_TEMP_TRXEND")) ||
		csIsFromLaneEndorsement == _T("1") && -1 != csCurTBState.Find(_T("ASSISTMODE_ENTER")))
     {
		CCustomerTrxBroker::instance()->SetCurrentInProgressState(_T("ASSISTMODE_TRXEND"));
		
		if ( !IsInProgress() && GetNumRemaining() == 0 )
		{
			if ( CCustomerCurrentCashDrawer::instance()->Status() ==
					CCashDrawerMsg::CashDrawerStates::CD_OPEN )
			{
				long lExpect;
				CCustomerCurrentCashDrawer::instance()->GetExpectedCashDrawerState( &lExpect );
				if ( lExpect == CASHDRAWER_OPEN )
				{
					// FastLane needs to open the cash drawer at end of transaction.
					tbtrace( TM_INFO, _T("Endorsement complete, now its OK to open the cash drawer.") );
					CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_NEWCASHDRAWERSTATE, NULL, NULL );
				}
			}
			else if( csIsTrainingMode == "Y" )
			{	
				tbtrace( TM_INFO, _T("Inside EndorsementCompleted. FL in Training Mode") );
				TBSTATE tbRet = TB_COMPLETEPRINTANDCUT;
				CCustomerTBScotMsg::instance()->SendReturnStateMsg( tbRet, NULL, NULL );
			}	
			else{
				TBSTATE tbRet = TB_TRXEND;
				CCustomerTBScotMsg::instance()->SendReturnStateMsg( tbRet, NULL, NULL );
			}
        }
    }

    return rc;
}

    
TBRC CCustomerCurrentSlip::GetDocEndorsementInfo(long   *plNumRemaining,
                                       LPTSTR *ppszDocDescription, 
                                       PTSTR  *ppszDocFront, 
                                       LPTSTR *ppszDocBack)
{
	long lChit = *plNumRemaining;
    m_iSlipType = -1;

    // The following looks really weird for handling NumRemaining; but it prevents
	// Fastlane from incorrectly kicking out a MICR'd check that needs endorsement.
	if ( GetNumRemaining() > 0 )
	{
	    //*plNumRemaining = 1;
        *plNumRemaining = GetNumRemaining();

		CStringArray* pArr = (CStringArray*)m_lstSlips.GetHead();
		if ( NULL != pArr )
		{
            CString csSlipDesc;
            m_iSlipType = IdentifySlipType(pArr->GetAt(SLIP_INDEX_BACK), csSlipDesc);

			CopyString(*ppszDocDescription, csSlipDesc);
			CopyString(*ppszDocFront, pArr->GetAt(SLIP_INDEX_FRONT));

			CString csBack;

            CCustomerTrxBroker & trxBroker = *CCustomerTrxBroker::instance();

			// Handle rotated slips for Check printing
			if ( ( m_iSlipType == 1 ) && // check front slip type
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


int CCustomerCurrentSlip::IdentifySlipType(const CString & csData, CString & csRetDesc)
{
    tbtraceIO(_T("IdentifySlipType"));
    BOOL bFound[] = { 
        false, 
        false, 
        false,
        false, 
        false,
        false
        };

    LPCTSTR szDetect[] = { 
        _T("SlipDetectCheckBack"),
        _T("SlipDetectCheckFront"),
        _T("SlipDetectCheckDenial"),
        _T("SlipDetectEBT"),
        _T("SlipDetectNegDept"),
        _T("SlipDetectTaxExempt")
        };

    LPCTSTR szDescKey[] = { 
        _T("SlipDescCheckBack"),
        _T("SlipDescCheckFront"),
        _T("SlipDescCheckDenial"),
        _T("SlipDescEBT"),
        _T("SlipDescNegDept"),
        _T("SlipDescTaxExempt")
        };

    const int TYPECOUNT = sizeof(bFound)/sizeof(bFound[0]);
    CCustomerMHOptions & opts = *CCustomerMHOptions::instance();
    int iFindCount = 0;
    csRetDesc = _T("");
    int iSlipType = -1;

    for(int i = 0; i < TYPECOUNT; i++)
    {
        LPTSTR pszDetect = NULL;
        CopyString(pszDetect, opts.OptionValue(szDetect[i]));
        
        int iPos = 0;
        CString csToken;
        LPCTSTR psz;
        
        psz = _tcstok(pszDetect, _T(";"));

        while( psz )
        {
            csToken = psz;
            csToken.TrimLeft();
            csToken.TrimRight();
            if( -1 != csData.Find(csToken) )
            {
                tbtrace(TM_INFO, _T("Identified slip as %s. Found match on token '%s'."), szDetect[i], (LPCTSTR)csToken);
                bFound[i] = true;
                if( ++iFindCount == 1 )
                {
                    csRetDesc = opts.OptionValue(szDescKey[i]);
                    iSlipType = i;
                }
                break;
            }
            psz = _tcstok(NULL, _T(";"));
        }
        delete [] pszDetect;
    }

    // detection algorithm
    if( iFindCount == 0 )
    {
        // If none are found, assume it's a check front -- it's the least discernable of all
        tbtrace(TM_INFO, 
            _T("No matches found, assuming it is a check front."));
        csRetDesc = opts.OptionValue(szDescKey[1]);
        iSlipType = 1;
    }
    else if( iFindCount > 1 )
    {
        tbtrace(TM_INFO, 
            _T("Identified multiple slip type matches. Using first match found. Consider revising criteria."));
    }
    return iSlipType;
}


CString CCustomerCurrentSlip::RotateSlip( CString csData )
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

    // To reuse the RotateSlip function of the solution layer for it to work
    // with front of check printing for electronic checks, we need to ensure
    // that there's an escape sequence that precedes the data. This way the
    // solution level parser will work properly. 
    if( csData.GetAt(0) != 0x1B )
    {
        csData = _T("\x1B|N") + csData;
    }

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

    //
    // Strip off the last few line feeds from the rotated check
    //
    int iEnd = csNewData.ReverseFind(_T('\x0A'));
    while( -1 != iEnd )
    {
        int iStart = csNewData.Left(iEnd).ReverseFind(_T('\x0A'));
        if( -1 != iStart )
        {
            CString cs = csNewData.Mid(iStart+1,iEnd - iStart);
            cs.Remove(_T(' '));
            if( cs == _T("\x0A") )
            {
                // blank string, remove it
                csNewData.Delete(iStart+1, iEnd - iStart);
                iEnd = iStart;
            }
            else
            {
                // string is not blank, only remove the last line 
                // feed and stop processing
                csNewData.Delete(iEnd,1);
                iEnd = -1;
            }
        }
        else
        {
            iEnd = -1;
        }
    }

	csNewData += _T("\x1BV0\x0A");

	return csNewData;    
}

void CCustomerCurrentSlip::SetSlipDescription( CString csDesc )
{
    tbtrace(TM_INFO, _T("SetSlipDescription: '%s' -- Not used for Meijer"), csDesc);
}

void CCustomerCurrentSlip::FinishSlip(void)
{
    tbtraceIO(_T("FinishSlip"));

    if( IsInProgress() )
    {
        // Bug in ACS Meijer -> causes ghost slips with no data to come in, 
        // after an open_jaws command. Filter out any of those blank slips.
        CString csFront = m_pSlipData->GetAt(SLIP_INDEX_FRONT);
        CString csBack = m_pSlipData->GetAt(SLIP_INDEX_BACK);
        csFront.TrimLeft();
        csFront.TrimRight();
        csBack.TrimLeft();
        csBack.TrimRight();
        bool bGhostSlip = csFront.IsEmpty() && csBack.IsEmpty();
		CString csConcatenated = csFront + csBack;

		//there's an ACS bug where we receive slip data with only the \xBv characters
		//this causes FL to do a fake endorsement
        if(( bGhostSlip ) || (csConcatenated == _T("\x1Bv")))
        {
            m_pSlipData = NULL;
            tbtrace(TM_WARNING, _T("Received a ghost slip from POS containing no data. Ignoring it."));
        }
        else
        {
            m_lstSlips.AddTail( m_pSlipData );
            m_pSlipData = NULL;
            tbtrace(TM_INFO, _T("FinishSlip() returning TB_ENDORSEDOCUMENT"));


			CReturnStateMsg *pStateMsg = new CReturnStateMsg(TB_ENDORSEDOCUMENT); // delete done in ReleaseMessageLParam()
			CCustomerTBScotMsg::instance()->SendScotMsg( pStateMsg);
			    
		    
        }
    }
}


bool CCustomerCurrentSlip::ShouldEjectSlipAfterMicr(void)
{
    // DG30: CHECK HANDLING START
    
    // Check will be ejected in certain cases only, here's the truth table:
    // SlipCheckFlip     SlipCheck21    SlipCheckHandling   Eject
    // -------------    ------------    -----------------   -----      
    //  0               1               1                   false    (1 case: no eject for check decline after MICR read)
    //  0               1               0                   true
    //  0               0               1                   false (2 cases: no eject after MICR for written by guest written by printer)
    //  0               0               0                   true
    //  1               1               1                   false    (4 cases: echeck, written by guest, written by printer, declined)                 
    //  1               1               0                   true                  
    //  1               0               1                   false (2 cases: no eject for written by guest, written by printer)
    //  1               0               0                   true
    
    // Eject only when CheckHandling is off
    tbtraceIO(_T("ShouldEjectSlipAfterMicr"));
    CString csValue = CCustomerTBProperties::instance()->GetHookProperty(_T("SlipCheckHandling"));
    return csValue != _T("1");
}

bool CCustomerCurrentSlip::ShouldEjectSlipBeforePrint(bool bJustFlippedToBack)
{
    //
    // Decide on whether we should be ejecting or not before the next endorsement 
    // This is fine tuning what's already been done after MICR read in MICRDisable().
    // Although since we are handling multiple queued endorsements, it's possible that
    // at this point SlipCheckHandling is 0 and we are still holding on to the slip,
    // As such we need to test for that again as well.
    //
    // Check will be ejected in certain cases only, here's the truth table:
    // SlipCheckFlip     SlipCheck21    SlipCheckHandling   Eject
    // -------------    ------------    -----------------   -----      
    //  0               1               1                   All except for Decline slip print (prints to back of check).
    //  0               0               1                   Eject for 2 cases: Decline slip print (prints to back of check) and written by printer after endorsement, before front of check print 
    //  1               1               1                   Eject for written by Guest only.
    //  1               0               1                   Eject for no cases.
    //  1               0               0                   All cases.
    //  0               1               0                   All cases.
    //  0               0               0                   All cases.
    //  1               1               0                   All cases.
    tbtraceIO(_T("ShouldEjectSlipBeforePrint"));
    CCustomerTBProperties & props = *CCustomerTBProperties::instance();
    bool bRet = false;
    bool bIsCheckBack = (m_iSlipType == 0);
    bool bIsCheckFront = (m_iSlipType == 1);
    bool bIsCheckDenial = (m_iSlipType == 2);
    bool bCheckHandling = (props.GetHookProperty(_T("SlipCheckHandling")) == _T("1"));
    bool bCheck21 = (props.GetHookProperty(_T("SlipCheck21")) == _T("1"));
    bool bCheckFlip = false;
    
    if( props.GetHookProperty(_T("SlipCheckFlip")) == _T("1") )
    {   
        bCheckFlip = !CCustomerTrxBroker::instance()->InRemoteAssistMode();
        if( !bCheckFlip )
        {
            tbtrace(TM_INFO, _T("Check flip will be disabled; slip print is being processed on RAP."));
        }
    }

    if( !bCheckHandling )  // Cases 5-8
    {
        bRet = true;
    }
    else if( !bCheckFlip && bCheck21 && !bIsCheckDenial )  // Case 1
    {
        bRet = true;
    }
    else if( !bCheckFlip && !bCheck21 && (bIsCheckDenial || bIsCheckFront) ) // Case 2
    {
        bRet = true;
    }
    else if( bCheckFlip && bCheck21 && bIsCheckBack && !bJustFlippedToBack) // Case 3
    {
        bRet = true;
    }
    else if( bCheckFlip && !bCheck21 )  // case 4 
    {
        bRet = false;
    }

    return bRet;
}

bool CCustomerCurrentSlip::ShouldFlipSlipBeforePrint(bool bIsCurrentSlipSideFront)
{
    // Detect whether or not we are supposed to be flipping the slip before printing.
    // These conditions will depend on the current state we're in. It will also depend
    // on the the current check handling configuration.
    tbtraceIO(_T("ShouldFlipSlipBeforePrint"));

    CCustomerTBProperties & props = *CCustomerTBProperties::instance();
    bool bRet = false;
    bool bIsCheckFront = (m_iSlipType == 1);
    bool bCheckHandling = (props.GetHookProperty(_T("SlipCheckHandling")) == _T("1"));
    bool bCheck21 = (props.GetHookProperty(_T("SlipCheck21")) == _T("1"));
    bool bCheckFlip = false;
    
    
    if( props.GetHookProperty(_T("SlipCheckFlip")) == _T("1") )
    {   
        bCheckFlip = !CCustomerTrxBroker::instance()->InRemoteAssistMode();
        if( !bCheckFlip )
        {
            tbtrace(TM_INFO, _T("Check flip will be disabled; slip flip is being processed on RAP."));
        }
    }
    
    tbtrace(TM_INFO, 
        _T("Slip printing info: SlipType:%d  CheckHandling:%d  Check21:%d  CheckFlip:%d  m_bSlipPrintSideFront:%d"),
        m_iSlipType, bCheckHandling, bCheck21, bCheckFlip, bIsCurrentSlipSideFront);
    
        // Assuming CheckFlip is enabled, check will be flipped in certain cases only.
        //  SlipCheck21    SlipCheckHandling   Flip Check
        // ------------    -----------------   ----------- 
        //  1               1                   yes     (flip once for echeck, twioe for written by printer, 0 for written by guest)
        //  1               0                   no      
        //  0               1                   yes     (flip once for printer write (back will be endorsed first)
        //  0               0                   no
        
    
    if( bCheckFlip && bCheckHandling )
    {
        return bIsCheckFront != bIsCurrentSlipSideFront;
    }
    
    return bRet;
}

bool CCustomerCurrentSlip::ShouldFlipSlipAfterMicr(void)
{
    tbtraceIO(_T("ShouldFlipSlipAfterMicr"));
   
    if( CCustomerTBProperties::instance()->GetHookProperty(_T("SlipCheckFlip")) == _T("1") )
    {   
        bool bCheckFlip = !CCustomerTrxBroker::instance()->InRemoteAssistMode();
        if( !bCheckFlip )
        {
            tbtrace(TM_INFO, 
                _T("Check flip will be disabled; check printing is being processed on RAP."));
        }
        else
        {
            // Verify that the POS and pinpad are displaying the ECheck Approved auth. screen
            _bstr_t bstrPosState = CCurrentPosState::instance()->GetValue(_T("PosState"));
            CString csPosState = (LPCTSTR)bstrPosState;
            csPosState.TrimLeft();
            csPosState.TrimRight();

            if( !csPosState.Compare(_T("ECHECK_VERIFY")) )
            {
                return true;
            }
            else
            {
                tbtrace(TM_INFO, 
                    _T("Check will not be flipped; POS not displaying authorization screen, it's displaying %s."), 
                    (LPCTSTR)csPosState);
            }
        }
    }

    return false;
}


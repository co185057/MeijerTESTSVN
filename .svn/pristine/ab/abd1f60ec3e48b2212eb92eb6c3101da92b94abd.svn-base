#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecBagAreaBackup.h"
#include "SMSecUnExpectedIncrease.h"
#include "SMSecUnexpectedDecrease.h"


#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecBagAreaBackup")


IMPLEMENT_DYNCREATE(SMSecBagAreaBackupBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP



//////////////////////////////////////////
SMSecBagAreaBackupBase::SMSecBagAreaBackupBase()
    : nCountForPeriodicClearBagAreaAudio(0)
{
  IMPLEMENT_TIMESTAMP
}
///////////////////////////////////
///////////////////////////////////////
SMStateBase  *SMSecBagAreaBackupBase::Initialize(void)
{
	if (co.IsSecurityBypassOn())
	{
		trace(L6,_T("Security bypassed, ignoring BagAreaBackup, return to Scan and bag"));
        dm.fTakeawayBeltBackup = false;
		return setAndCreateAnchorState(BES_SCANANDBAG);
	}

	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SecBagAreaBackup") );

	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_BAG_AREA_BACKUP, SCOT_LANGUAGE_PRIMARY));

    DMTakeawayBeltNotify(tabEventHandleBagAreaBackup);
    // NewUI
	ps.SetCMFrame(_T("SecBagAreaBackup"));
    // End NewUI
	SAWLDBScannerDisable();			// Disable the scanner
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	ps.Message12(MSG_BAG_AREA_BACKUP, true);
	ps.ShowCMFrame(false);
	nTimeOut = co.getTimeOutInitialClearBagAreaAudio();

    if (nTimeOut == 0)
    {
        // Zero disables timeout, but we want immediate so call the TimedOut
        // function directly.
		trace(L6, _T("InitialClearBagAreaAudio is immediate"));
        return TimedOut();
    }
    else
    {
        // Just allow normal timeout to kick off the initial audio.
        return STATE_NULL;
    }
}

//////////////////////////////////////
void SMSecBagAreaBackupBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SecBagAreaBackup") );
	nCountForPeriodicClearBagAreaAudio = 0;
    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing
}

///////////////////////////////////////
SMStateBase *SMSecBagAreaBackupBase::TimedOut(void) // timeout
{
	nTimeOut = co.getTimeOutPeriodicClearBagAreaAudio();
	DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_BAGAREABACKUP));

	if(nCountForPeriodicClearBagAreaAudio == 1)	// only do on the 1st periodic timeout
	{
        //The text here is just the name of the section in scotdataentry.ini.  Don't need to be in SCOTMSGE.DAT
        SAEnterSecurityCondition();
	    cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("BagAreaBackup"));
	    m_View = DATANEEDED_BAGAREABACKUP;
	    CString csSecInfo, csDesc;
		//tar 199300
		//csDesc can't be empty, otherwise RAP will overwrite the Button text.
		//This text will show on intervention screen.
		csDesc = ps.GetPSText(TOP_CAP_RA_BAG_AREA_BACKUP, SCOT_LANGUAGE_PRIMARY); 
	    csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_BAG_AREA_BACKUP, SCOT_LANGUAGE_PRIMARY), co.IsSecurityDisableOn()==true ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_BAGAREABACKUP, SCOT_LANGUAGE_PRIMARY));
	    ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
	}
	nCountForPeriodicClearBagAreaAudio++;
    return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecBagAreaBackupBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
	trace(L6, _T("RAP cleared lane btn.  Belt still needs clearing."));
    return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecBagAreaBackupBase::OnWtIncreaseNotAllowed()	 
{
	DMSaySecurity( SEC_NONE, _T("blank.wav") );
	CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_SECBAGAREABACKUP, BES_SECBAGAREABACKUP);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecBagAreaBackupBase::OnWtDecrease()	 
{
//	return ThresholdViolWtDecrease(prevState, BES_SECBAGAREABACKUP);
    return STATE_NULL;
}

///////////////////////////////////////////////
SMStateBase* SMSecBagAreaBackupBase::DMTakeawayBagAreaBackup(bool isBackedUp)
{
    trace(L2, _T("+SMSecBagAreaBackupBase::DMTakeawayBagAreaBackup(isBackedUp=%s)"),
          isBackedUp ? _T("true") : _T("false"));

    SMStateBase* baseNextState = SMStateBase::DMTakeawayBagAreaBackup(isBackedUp);
    if (!isBackedUp)
    {
		//228082 - don't need to call SAClearSecurityCondition because don't want to 
		// also call TABTransportclearedItem from there.  handling light and RAP here. LPM021703
		ClearSecurityViolationLight();
		//Clear RAP Approval 
		trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
		ra.OnNoRAPDataNeeded();  

		//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
		trace(L2, _T("bEASReEnableScanner is set to true."));
		bEASReEnableScanner = true;
	    DMSaySecurity(SEC_NONE, _T("blank.wav"));		// to interrupt the wave file if it's playing
        trace(L6, _T("Takeaway Belt backed up items cleared"));
        // BagAreaBackup can only be entered from ScanAndBag, so always return
        // to ScanAndBag when the backup is cleared.
        trace(L2, _T("-SMSecBagAreaBackupBase::DMTakeawayBagAreaBackup"));
        return setAndCreateAnchorState(BES_SCANANDBAG);
    }
    else
    {
        trace(L2, _T("-SMSecBagAreaBackupBase::DMTakeawayBagAreaBackup"));
        return baseNextState;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage.cpp
//
// Description: base class for system message classes, see h file
//
// Author: Jeff Connelly
//
//
// CHANGE HISTORY:
//
// CRD 450481 Name: CJ Estrada Date: June 23, 2020
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"

#include "SMSystemMessage.h"

#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SystemMessage")

IMPLEMENT_DYNCREATE(SMSystemMessage, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////////////
SMSystemMessage::SMSystemMessage(BMPS   bitmap,
                                 PSTEXT title,
                                 bool   continueButton,
                                 bool   cancelButton,
                                 LPCTSTR szMessage,   // string text message or NULL
                                 PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                                 PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
      		                       long   promptValue, // prompt area sale value
                                 long   deviceClass, // device class, -1 for none
                                 bool   checkHealth, // DMCheckHealth after msg?
                                 bool showTotal,
                                 bool showRetractedTotal,
                                 bool okButton,
                                 bool flashOkButton,
								 bool fHTML )
: SMSystemMessageBase(bitmap, title, continueButton,
					  cancelButton, szMessage, psMessage,
					  promptText, promptValue, deviceClass, checkHealth,
                      showTotal, showRetractedTotal, okButton, flashOkButton, fHTML)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSystemMessage::SMSystemMessage()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSystemMessage::Deliver(BMPS   bitmap,
                                 PSTEXT title,
                                 bool   continueButton,
                                 bool   cancelButton,
                                 LPCTSTR szMessage,   // string text message or NULL
                                 PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                                 PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
      		                       long   promptValue, // prompt area sale value
                                 long   deviceClass, // device class, -1 for none
                                 bool   checkHealth, // DMCheckHealth after msg?
                                 bool showTotal,
                                 bool showRetractedTotal,
                                 bool okButton,
                                 bool flashOkButton,
								 bool displayHTML)
{
  SMSystemMessageBase::Deliver(bitmap, title, continueButton,
				  cancelButton, szMessage, psMessage,
				  promptText, promptValue, deviceClass, checkHealth,
                  showTotal, showRetractedTotal, okButton, flashOkButton, displayHTML);

  return this;
}

SMStateBase * SMSystemMessage::Initialize(void)
{
    return SMSystemMessageBase::Initialize();
}

void SMSystemMessage::UnInitialize(void)
{

	//Start CRD 366323
	NotifyExitStoreMode();
	//End CRD 366323

    SMSystemMessageBase::UnInitialize();
	
	// Tell RAP we're exiting a system message. Since we can't just create 
    // our own custom message, we have to use one that already exists in RCM 
    // in this case we'll reuse EnableNumItems which generates 'Enable NUMItems Event'
    ra.OnAddOtherEvent(_T("COMMAND=EnableNumItems;"));
}

bool SMSystemMessage::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSystemMessageBase::DeliverDescriptorList(dlParameterList);
}

// +POS143692
SMStateBase *SMSystemMessage::OnMatchedWt()
{
	//442275+
	//racing condition: FL goes from BagAndEAS -->SystemMessage and item is bagged but 
	//onItemOK is arrived after FL goes to SystemMessage
	//FL should call smstatebase::OnMatchedWt(), intead, it comes here
	if (getAnchorState() == BES_BAGANDEAS)
	{
		SMStateBase *pRetState = SMStateBase::OnMatchedWt();
		delete getRealStatePointer( pRetState);
	}
	//442275-
	return SMSystemMessageBase::OnMatchedWt();
}
// -POS143692

bool SMSystemMessage::PSRemoteLegalForState()
{
    return SMSystemMessageBase::PSRemoteLegalForState();
}

long SMSystemMessage::GetDeviceClass()
{
    return SMSystemMessageBase::GetDeviceClass();
}

SMStateBase * SMSystemMessage::CkStateStackAndReturn(void)
{
    return SMSystemMessageBase::CkStateStackAndReturn();
}

SMStateBase * SMSystemMessage::ShowDeviceErrorNotFixedScreen(void)
{
    return SMSystemMessageBase::ShowDeviceErrorNotFixedScreen();
}

bool SMSystemMessage::IsMediaStatusError(void)
{
    return SMSystemMessageBase::IsMediaStatusError();
}

void SMSystemMessage::showScreen(bool showInfo)
{
    //Start CRD 450481 - reset Degraded Mode Type so that Base will enable the Cards Only button and show the correct description
	if ( ( dm.GetDegradedModeType() == DEGRADEDMODE_CARD_ONLY ) && 
		( IsCashDeviceError( dm.m_nStartDeviceErrorDeviceClass ) || 
		IsCashDeviceError( deviceClass )) )
	{
		dm.SetDegradedModeType(DEGRADEDMODE_OFF);
	}
	//End CRD 450481

    SMSystemMessageBase::showScreen(showInfo);
}

void SMSystemMessage::showRemoteScreen(bool showInfo)
{
    SMSystemMessageBase::showRemoteScreen(showInfo);
}

CString SMSystemMessage::getWaveFile()
{
    return SMSystemMessageBase::getWaveFile();
}

int SMSystemMessage::getTimeout()
{
    return SMSystemMessageBase::getTimeout();
}

SMStateBase * SMSystemMessage::PSButton1(void)
{
    trace(L6,_T("+SMSystemMessage::PSButton1"));
	bool bSavedfStateTakeawayBelt = dm.fStateTakeawayBelt;
	
	trace(L6,_T("TAR 326302 - SMStateBase::g_bSafetyCoverClosed=%d,deviceClass=%d"),
		SMStateBase::g_bSafetyCoverClosed, deviceClass);
	
	if( SMStateBase::g_bSafetyCoverClosed && (DMCLASS_TAKEAWAYBELT != deviceClass))
	{
		trace(L6,_T("TAR 326302 workaround in effect"));
		dm.fStateTakeawayBelt = false;
	}
	
	SMStateBase* rc = SMSystemMessageBase::PSButton1();
	dm.fStateTakeawayBelt = bSavedfStateTakeawayBelt;
	
	bool bLeavingRemotedSystemMessage = false;
	CString csStateName = _T("");
	if(ps.RemoteMode())
	{
		
		if(( rc == STATE_RETURN) || (rc == STATE_RESUME))
		{
            bLeavingRemotedSystemMessage = true;
		}else if(getRealStatePointer(rc) != NULL)
		{
			csStateName = rc->GetRuntimeClass()->m_lpszClassName;
			if (csStateName != _T("SystemMessage"))
			{
				
				bLeavingRemotedSystemMessage = true;                
				
			}
		}
	}
	
	if(bLeavingRemotedSystemMessage)
	{
		if(dm.IsDeviceRemote(DMCLASS_RECEIPT, 0))
		{
			m_bAssistModeReturnAfterSystemMessage = true;
		}
		trace(L6,_T("Remoted and leaving System Message State"));
		if (dm.fStateTakeawayBelt)
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmRemoteAssistMode") );
		else
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode") );
		
		
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAuthorization") );
		
	}
	
	trace(L6,_T("-SMSystemMessage::PSButton1"));
  return rc;
}

SMStateBase * SMSystemMessage::PSButton2(void)
{
    return SMSystemMessageBase::PSButton2();
}

SMStateBase * SMSystemMessage::PSButton3(void)
{
    return SMSystemMessageBase::PSButton3();
}

SMStateBase * SMSystemMessage::PSButton8(void)
{
    return SMSystemMessageBase::PSButton8();
}

SMStateBase * SMSystemMessage::PSCancelKey(void)
{
    return SMSystemMessageBase::PSCancelKey();
}

SMStateBase * SMSystemMessage::OnBackToLGW(void)
{
    return SMSystemMessageBase::OnBackToLGW();
}

void SMSystemMessage::LogDeviceError(void)
{
    SMSystemMessageBase::LogDeviceError();
}

bool SMSystemMessage::getInSystemMessage(void)
{
    return SMSystemMessageBase::getInSystemMessage();
}

void SMSystemMessage::setInSystemMessage(bool b)
{
    SMSystemMessageBase::setInSystemMessage(b);
}


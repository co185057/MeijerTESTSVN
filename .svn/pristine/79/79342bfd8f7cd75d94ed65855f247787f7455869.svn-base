// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/remote-console-manager/RCMRegistration.cpp 5     1/25/10 8:19p Cu185008 $
/*
 * @(#)TabRegistration.cpp      $Revision: 5 $  $Date: 1/25/10 8:19p $
 *
 * Copyright 2004, NCR Corporation.
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/remote-console-manager/RCMRegistration.cpp $
 * 
 * 5     1/25/10 8:19p Cu185008
 * G2 Migration.
 * 
 * 4     11/06/08 4:16p Sa250050
 * RFQ 2183 - DG35 - Patch G Migration (updated to build 331)
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 37    2/14/08 11:16a Mm185096
 * added new actions, SetText, SetTextColor
 * 
 * 36    11/29/07 4:09p Cb160006
 * TAR 364905
 * 
 * 35    8/28/07 4:02p Jj100011
 * 354765
 * 
 * 34    11/21/06 4:42p Jj100011
 * Tar 338040
 * 
 * 33    9/29/06 8:16a Dm185016
 * TAR 333138
 * 
 * 32    1/24/06 10:38a Dm185016
 * TAR 315980
 * 
 * 31    5/31/05 11:34a Dm185016
 * TAR 301203
 * 
 * 31    5/20/05 3:31p Dm185016
 * TAR 301079
 * 
 * 30    5/19/05 5:25p Dm185016
 * TAR 300190
 * 
 * 29    5/09/05 4:42p Dm185016
 * Added sound support for RAP interventions
 * 
 * 28    5/02/05 2:59p Tp151000
 * TAR 299442
 * 
 * 27    5/01/05 8:59a Tp151000
 * Performmance
 * 
 * 26    4/18/05 7:16p Dm185016
 * Keyboard support
 * Toggle support
 * 
 * 25    4/15/05 9:18a Dm185016
 * Merge from Patch B
 * 
 * 23    4/14/05 9:59p Dm185016
 * TAR 298021
 * 
 * 22    4/07/05 1:51p Dm185016
 * TAR 297347
 * 
 * 21    4/05/05 11:58a Dm185016
 * TAR 297356
 * 
 * 20    3/30/05 5:05p Dm185016
 * TAR 297001
 * 
 * 19    3/28/05 2:20p Tp151000
 * TAR 296896
 * 
 * 18    3/17/05 2:30p Tp151000
 * Add Media Low event
 * 
 * 17    3/15/05 3:36p Dm185016
 * TAR 295676 - AcceptConnection is now handled by a separate thread to
 * prevent hanging the state machine thread.
 * 
 * 16    3/14/05 8:26p Dm185016
 * TAR 294587
 * 
 * 15    3/07/05 6:56p Dm185016
 * TAR 294609.
 * TAR 294610.
 * 
 * 14    3/03/05 3:14p Dm185016
 * TAR 291700
 * 
 * 13    2/28/05 4:21p Dm185016
 * lint
 * 
 * 12    2/25/05 5:53p Tk124825
 * Rolling back to the old version
 * 
 * 12    2/25/05 5:53p Tk124825
 * Rolling back to the old version
 * 
 * 10    2/18/05 11:28a Dm185016
 * TAR 293795.
 * 
 * 9     2/18/05 9:20a Dm185016
 * TAR 293708.
 * 
 * 8     2/17/05 5:54p Dm185016
 * 
 * 7     2/15/05 11:29a Dm185016
 * TAR 293322.
 * 
 * 6     2/14/05 7:08p Dm185016
 * Release Exclusive Access Event support
 * 
 * 5     2/08/05 5:31p Tp151000
 * TAR 292777
 * 
 * 4     2/08/05 11:03a Tp151000
 * 290952
 * 
 * 3     2/03/05 5:46p Dm185016
 * TAR 292378 - New actions checking immediate intervention stack
 * 
 * 2     2/03/05 5:42p Tp151000
 * TAR 292361
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
// 
// 19    1/20/05 10:37a Dm185016
// Added handler for lane context switches.
// 
// 18    1/13/05 1:39p Dm185016
// TAR 290304
// 
// 17    1/12/05 5:54p Dm185016
// Multiple RAP support.
// 
// 16    1/07/05 2:28p Dm185016
// TAR 288789
// 
// 15    12/22/04 11:04a Dm185016
// TAR 288934 - New actions to handle proper updates of buttons
// HH support.
// 
// 14    12/16/04 3:34p Dm185016
// 
// 13    12/15/04 5:56p Tp151000
// TAR 287344 and 288069
// 
// 12    12/14/04 11:23a Dm185016
// TAR 288478.  New actions for clearing button text values.
// 
// 11    12/09/04 5:25p Dm185016
// TAR 287903.  Added handler to clear rap-data-needed interventions.
// 
// 10    12/09/04 1:48p Dm185016
// TAR 287222, 287903, 287907
// 
// 9     12/07/04 4:19p Dm185016
// Added option reload feature
// 
// 8     12/02/04 3:41p Dm185016
//
// 7     11/30/04 4:28p Dm185016
//
// 6     11/18/04 1:40p Dm185016
// New actions for intervention checking.
//
// 5     11/16/04 11:33a Dm185016
// More actions
//
// 4     11/02/04 3:07p Dm185016
// More updates
//
// 3     10/29/04 8:59a Dm185016
//
// 2     10/14/04 1:37p Dm185016
//
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/

//lint -e534

#include "StdAfx.h"
#include "Createable.h"
#include "Action.h"
#include "Registration.h"

namespace fastlane {}

using namespace fastlane;

#include "actions/EventCommand.h"
#include "actions/AddOtherEventCommand.h"
#include "actions/CanApproveCommand.h"
#include "actions/ApprovalCommand.h"
#include "actions/GetApprovalCommand.h"
#include "actions/GetCommand.h"
#include "actions/SetCommand.h"
#include "actions/ReloadConfiguration.h"
#include "actions/LoadConfiguration.h"
#include "actions/ControlCommand.h"
#include "actions/SwitchContext.h"
#include "actions/SelectButton.h"
#include "actions/SetTransactionState.h"
#include "actions/EnterStoreMode.h"
#include "actions/ExitStoreMode.h"
#include "actions/TransactionBegin.h"
#include "actions/TransactionEnd.h"
#include "actions/SetLightState.h"
#include "actions/ValidateOperator.h"
#include "actions/AddNormalItem.h"
#include "actions/ApproveIntervention.h"
#include "actions/ClearImmediateIntervention.h"
#include "actions/SaveImmediateIntervention.h"
#include "actions/SaveDelayedIntervention.h"
#include "actions/EnableButton.h"
#include "actions/DisableButton.h"
#include "actions/MediaCommand.h"
#include "actions/ShowControl.h"
#include "actions/HideControl.h"
#include "actions/CanSuspendNowEvent.h"
#include "actions/ConnectCommand.h"
#include "actions/DisconnectCommand.h"
#include "actions/SignonCommand.h"
#include "actions/SignoffCommand.h"
#include "actions/CheckImmediateIntervention.h"
#include "actions/CheckDelayedIntervention.h"
#include "actions/DisplayImmediateIntervention.h"
#include "actions/DisplayDelayedIntervention.h"
#include "actions/CheckDefaultButton.h"
#include "actions/Append1To9.h"
#include "actions/Append0.h"
#include "actions/ClearEntryText.h"
#include "actions/ValidateBirthday.h"
#include "actions/NotifyAssistMode.h"
#include "actions/GenerateLocalButtonPress.h"
#include "actions/ShowMessage.h"
#include "actions/SwitchContextCommand.h"
#include "actions/IsInputEventEnabled.h"
#include "actions/CheckAssistModeControl.h"
#include "actions/SendUnsolicitedData.h"
#include "actions/IsNotStartTransactionView.h"
#include "actions/IsNotStartTransactionAMView.h"
#include "actions/CheckRAPDataNeededEvent.h"
#include "actions/SaveAsDelayedIntervention.h"
#include "actions/SetLaneName.h"
#include "actions/PushbackDelayedIntervention.h"
#include "actions/SendSuspendRequest.h"
#include "actions/SaveSuspendRequest.h"
#include "actions/RequestFullScreenAssistMode.h"
#include "actions/ReleaseFullScreenAssistMode.h"
#include "actions/IsOutsideTransaction.h"
#include "actions/ClearApprovedIntervention.h"
#include "actions/ClearRemoteIntervention.h"
#include "actions/SaveMinimumAge.h"
#include "actions/ScrollReceipt.h"
#include "actions/ClearRAPDataNeededIntervention.h"
#include "actions/IsInterventionOutstanding.h"
#include "actions/IsCurrentInterventionInProgress.h"
#include "actions/ClearLaneStatusWindows.h"
#include "actions/AppendDot.h"
#include "actions/ClearReceipt.h"
#include "actions/SaveRemoteConnection.h"
#include "actions/SwitchContextDelayedIntervention.h"
#include "actions/IsHandheld.h"
#include "actions/DisplayNextDelayedIntervention.h"
#include "actions/DisplayAgeRestrictionInstructionText.h"
#include "actions/GenerateEvent.h"
#include "actions/AcquireFullScreen.h"
#include "actions/ReleaseFullScreen.h"
#include "actions/SaveStartTransactionIntervention.h"
#include "actions/AcquireExclusiveAccess.h"
#include "actions/ReleaseExclusiveAccess.h"
#include "actions/SwitchLaneContextCommand.h"
#include "actions/EnableInputEvents.h"
#include "actions/IsNotImmediateInterventionOutstanding.h"
#include "actions/DisplayNextImmediateIntervention.h"
#include "actions/IsNotConfirmSuspend.h"
#include "actions/IsNotMaxOfDOBInput.h"
//#include "actions/DisableUserInput.h"
#include "actions/RemoteDataCommand.h"
#include "actions/RemoteDataCommandRAP.h"
#include "actions/IsRemoteConnectionAvailable.h"
#include "actions/SendInterventionEvent.h"
#include "actions/SendAllInterventionEvents.h"
#include "actions/SendClientMachineNameEvent.h"
#include "actions/IsNotHandheld.h"
#include "actions/ConnectCommandRAP.h"
#include "actions/SwitchContextImmediateIntervention.h"
#include "actions/IsExclusiveAccessGranted.h"
#include "actions/SetReleaseExclusiveAccessRequired.h"
#include "actions/IsCurrentRemoteConnection.h"
#include "actions/IsInterventionTypeOutstanding.h"
#include "actions/DisableAMOnHandheld.h"
#include "actions/ConnectCommandComplete.h"
#include "actions/MediaLowEvent.h"
#include "actions/CheckForDateNotNeededButtonOption.h"
#include "actions/IsRemoteConnectionInputEnabled.h"
#include "actions/IsHandheldAssistModeAllowed.h"
#include "actions/IsAssistModeEnabled.h"
#include "actions/SendAllLaneStatusSummary.h"
#include "actions/ClearCurrentIntervention.h"
#include "actions/SwitchContextRAPDataNeededIntervention.h"
#include "actions/ForwardAuxData.h" 
#include "actions/SelectKeyDown.h" 
#include "actions/IsCurrentTriliteColor.h" 
#include "actions/IsButtonEnabled.h" 
#include "actions/IsTriliteColorGray.h" 
#include "actions/PushbackImmediateIntervention.h"
#include "actions/PlaySound.h" 
#include "actions/PlayInterventionSound.h" 
#include "actions/ClearAllInterventions.h" 
#include "actions/MakeInterventionCurrent.h" 
#include "actions/MatchLaneContext.h" 
#include "actions/CompareToCurrentContext.h"
#include "actions/DisplayCurrentIntervention.h"
#include "actions/IsLastDelayIntervention.h"
#include "actions/IsForceApprovalBeforeSuspend.h"
#include "actions/SetText.h"
#include "actions/SetTextColor.h"
#include "actions/IsTriliteColorGreen.h" 

//#include "Pool.h"

using namespace fastlane;

BEGIN_REGISTRATION_MAP(names)
    REGISTRATION_ENTRY(EventCommand)
    REGISTRATION_ENTRY(AddOtherEventCommand)
    REGISTRATION_ENTRY(CanApproveCommand)
    REGISTRATION_ENTRY(ApprovalCommand)
    REGISTRATION_ENTRY(GetApprovalCommand)
    REGISTRATION_ENTRY(GetCommand)
    REGISTRATION_ENTRY(SetCommand)
    REGISTRATION_ENTRY(ReloadConfiguration)
    REGISTRATION_ENTRY(LoadConfiguration)
    REGISTRATION_ENTRY(ControlCommand)
    REGISTRATION_ENTRY(SwitchContext)
    REGISTRATION_ENTRY(SelectButton)
    REGISTRATION_ENTRY(SetTransactionState)
    REGISTRATION_ENTRY(EnterStoreMode)
    REGISTRATION_ENTRY(ExitStoreMode)
    REGISTRATION_ENTRY(TransactionBegin)
    REGISTRATION_ENTRY(TransactionEnd)
    REGISTRATION_ENTRY(SetLightState)
    REGISTRATION_ENTRY(ValidateOperator)
    REGISTRATION_ENTRY(AddNormalItem)
    REGISTRATION_ENTRY(ApproveIntervention)
    REGISTRATION_ENTRY(ClearImmediateIntervention)
    REGISTRATION_ENTRY(SaveImmediateIntervention)
    REGISTRATION_ENTRY(SaveDelayedIntervention)
    REGISTRATION_ENTRY(EnableButton)
    REGISTRATION_ENTRY(DisableButton)
    REGISTRATION_ENTRY(MediaCommand)
    REGISTRATION_ENTRY(ShowControl)
    REGISTRATION_ENTRY(HideControl)
    REGISTRATION_ENTRY(CanSuspendNowEvent)
    REGISTRATION_ENTRY(ConnectCommand)
    REGISTRATION_ENTRY(DisconnectCommand)
    REGISTRATION_ENTRY(SignonCommand)
    REGISTRATION_ENTRY(SignoffCommand)
    REGISTRATION_ENTRY(CheckImmediateIntervention)
    REGISTRATION_ENTRY(CheckDelayedIntervention)
    REGISTRATION_ENTRY(DisplayImmediateIntervention)
    REGISTRATION_ENTRY(DisplayDelayedIntervention)
    REGISTRATION_ENTRY(CheckDefaultButton)
    REGISTRATION_ENTRY(Append1To9)
    REGISTRATION_ENTRY(Append0)
    REGISTRATION_ENTRY(ClearEntryText)
    REGISTRATION_ENTRY(ValidateBirthday)
    REGISTRATION_ENTRY(NotifyAssistMode)
    REGISTRATION_ENTRY(GenerateLocalButtonPress)
    REGISTRATION_ENTRY(ShowMessage)
    REGISTRATION_ENTRY(SwitchContextCommand)
    REGISTRATION_ENTRY(IsInputEventEnabled)
    REGISTRATION_ENTRY(CheckAssistModeControl)
    REGISTRATION_ENTRY(SendUnsolicitedData)
    REGISTRATION_ENTRY(IsNotStartTransactionView)
    REGISTRATION_ENTRY(IsNotStartTransactionAMView)
    REGISTRATION_ENTRY(CheckRAPDataNeededEvent)
    REGISTRATION_ENTRY(SaveAsDelayedIntervention)
    REGISTRATION_ENTRY(SetLaneName)
    REGISTRATION_ENTRY(PushbackDelayedIntervention)
    REGISTRATION_ENTRY(SendSuspendRequest)
    REGISTRATION_ENTRY(SaveSuspendRequest)
    REGISTRATION_ENTRY(RequestFullScreenAssistMode)
    REGISTRATION_ENTRY(ReleaseFullScreenAssistMode)
    REGISTRATION_ENTRY(IsOutsideTransaction)
    REGISTRATION_ENTRY(ClearApprovedIntervention)
    REGISTRATION_ENTRY(ClearRemoteIntervention)
    REGISTRATION_ENTRY(SaveMinimumAge)
    REGISTRATION_ENTRY(ScrollReceipt)
    REGISTRATION_ENTRY(ClearRAPDataNeededIntervention)
    REGISTRATION_ENTRY(IsInterventionOutstanding)
	REGISTRATION_ENTRY(IsCurrentInterventionInProgress)
    REGISTRATION_ENTRY(ClearLaneStatusWindows)
    REGISTRATION_ENTRY(AppendDot)
    REGISTRATION_ENTRY(ClearReceipt)
    REGISTRATION_ENTRY(SaveRemoteConnection)
    REGISTRATION_ENTRY(SwitchContextDelayedIntervention)
    REGISTRATION_ENTRY(IsHandheld)
    REGISTRATION_ENTRY(DisplayNextDelayedIntervention)
    REGISTRATION_ENTRY(DisplayAgeRestrictionInstructionText)
    REGISTRATION_ENTRY(GenerateEvent)
    REGISTRATION_ENTRY(AcquireFullScreen)
    REGISTRATION_ENTRY(ReleaseFullScreen)
    REGISTRATION_ENTRY(SaveStartTransactionIntervention)
    REGISTRATION_ENTRY(AcquireExclusiveAccess)
    REGISTRATION_ENTRY(ReleaseExclusiveAccess)
    REGISTRATION_ENTRY(SwitchLaneContextCommand)
    REGISTRATION_ENTRY(EnableInputEvents)
    REGISTRATION_ENTRY(IsNotImmediateInterventionOutstanding)
    REGISTRATION_ENTRY(DisplayNextImmediateIntervention)
    REGISTRATION_ENTRY(IsNotConfirmSuspend)
    REGISTRATION_ENTRY(IsNotMaxOfDOBInput)
    //REGISTRATION_ENTRY(DisableUserInput)
    REGISTRATION_ENTRY(RemoteDataCommand)
    REGISTRATION_ENTRY(RemoteDataCommandRAP)
    REGISTRATION_ENTRY(IsRemoteConnectionAvailable)
    REGISTRATION_ENTRY(SendInterventionEvent)
    REGISTRATION_ENTRY(SendAllInterventionEvents)
    REGISTRATION_ENTRY(SendClientMachineNameEvent)
    REGISTRATION_ENTRY(IsNotHandheld)
    REGISTRATION_ENTRY(ConnectCommandRAP)
    REGISTRATION_ENTRY(SwitchContextImmediateIntervention)
    REGISTRATION_ENTRY(IsExclusiveAccessGranted)
    REGISTRATION_ENTRY(SetReleaseExclusiveAccessRequired)
    REGISTRATION_ENTRY(IsCurrentRemoteConnection)
    REGISTRATION_ENTRY(IsInterventionTypeOutstanding)
    REGISTRATION_ENTRY(DisableAMOnHandheld)
    REGISTRATION_ENTRY(ConnectCommandComplete)
    REGISTRATION_ENTRY(MediaLowEvent)
    REGISTRATION_ENTRY(CheckForDateNotNeededButtonOption)
    REGISTRATION_ENTRY(IsRemoteConnectionInputEnabled)
    REGISTRATION_ENTRY(IsHandheldAssistModeAllowed)
    REGISTRATION_ENTRY(IsAssistModeEnabled)
    REGISTRATION_ENTRY(SendAllLaneStatusSummary)
    REGISTRATION_ENTRY(ClearCurrentIntervention)
    REGISTRATION_ENTRY(SwitchContextRAPDataNeededIntervention)
    REGISTRATION_ENTRY(ForwardAuxData)
    REGISTRATION_ENTRY(SelectKeyDown)
    REGISTRATION_ENTRY(IsCurrentTriliteColor)
    REGISTRATION_ENTRY(IsButtonEnabled)
    REGISTRATION_ENTRY(IsTriliteColorGray)
    REGISTRATION_ENTRY(PushbackImmediateIntervention)
    REGISTRATION_ENTRY(PlaySound)
    REGISTRATION_ENTRY(PlayInterventionSound)
    REGISTRATION_ENTRY(ClearAllInterventions)
    REGISTRATION_ENTRY(MakeInterventionCurrent)
    REGISTRATION_ENTRY(MatchLaneContext)
	REGISTRATION_ENTRY(CompareToCurrentContext)
    REGISTRATION_ENTRY(DisplayCurrentIntervention)
	REGISTRATION_ENTRY(IsLastDelayIntervention)
	REGISTRATION_ENTRY(IsForceApprovalBeforeSuspend)
    REGISTRATION_ENTRY(SetText)
    REGISTRATION_ENTRY(SetTextColor)
	REGISTRATION_ENTRY(IsTriliteColorGreen)
END_REGISTRATION_MAP()


static CCreateableFactory<IAction> actionFactory;

extern "C"
{
    void __declspec(dllexport) RegisterAll(regmap_t*, HTRACE);
    void __declspec(dllexport) UnregisterAll(void);
    HRESULT __declspec(dllexport) CreateObject(const _TCHAR *, IAction **);
    HRESULT __declspec(dllexport) ReleaseObject(const _TCHAR *, IAction *);
};


void __declspec(dllexport) RegisterAll(regmap_t*ppNames, HTRACE hTrace)
{
    *ppNames = (regmap_t)&names;        //lint !e545

    actionFactory.SetTraceObject(hTrace);

    REGISTER_CREATEABLE(actionFactory, EventCommand);
    REGISTER_CREATEABLE(actionFactory, AddOtherEventCommand);
    REGISTER_CREATEABLE(actionFactory, CanApproveCommand);
    REGISTER_CREATEABLE(actionFactory, ApprovalCommand);
    REGISTER_CREATEABLE(actionFactory, GetApprovalCommand);
    REGISTER_CREATEABLE(actionFactory, GetCommand);
    REGISTER_CREATEABLE(actionFactory, SetCommand);
    REGISTER_CREATEABLE(actionFactory, ReloadConfiguration);
    REGISTER_CREATEABLE(actionFactory, LoadConfiguration);
    REGISTER_CREATEABLE(actionFactory, ControlCommand);
    REGISTER_CREATEABLE(actionFactory, SwitchContext);
    REGISTER_CREATEABLE(actionFactory, SelectButton);
    REGISTER_CREATEABLE(actionFactory, SetTransactionState);
    REGISTER_CREATEABLE(actionFactory, EnterStoreMode);
    REGISTER_CREATEABLE(actionFactory, ExitStoreMode);
    REGISTER_CREATEABLE(actionFactory, TransactionBegin);
    REGISTER_CREATEABLE(actionFactory, TransactionEnd);
    REGISTER_CREATEABLE(actionFactory, SetLightState);
    REGISTER_CREATEABLE(actionFactory, ValidateOperator);
    REGISTER_CREATEABLE(actionFactory, AddNormalItem);
    REGISTER_CREATEABLE(actionFactory, ApproveIntervention);
    REGISTER_CREATEABLE(actionFactory, ClearImmediateIntervention);
    REGISTER_CREATEABLE(actionFactory, SaveImmediateIntervention);
    REGISTER_CREATEABLE(actionFactory, SaveDelayedIntervention);
    REGISTER_CREATEABLE(actionFactory, EnableButton);
    REGISTER_CREATEABLE(actionFactory, DisableButton);
    REGISTER_CREATEABLE(actionFactory, MediaCommand);
    REGISTER_CREATEABLE(actionFactory, ShowControl);
    REGISTER_CREATEABLE(actionFactory, HideControl);
    REGISTER_CREATEABLE(actionFactory, CanSuspendNowEvent);
    REGISTER_CREATEABLE(actionFactory, ConnectCommand);
    REGISTER_CREATEABLE(actionFactory, DisconnectCommand);
    REGISTER_CREATEABLE(actionFactory, SignonCommand);
    REGISTER_CREATEABLE(actionFactory, SignoffCommand);
    REGISTER_CREATEABLE(actionFactory, CheckImmediateIntervention);
    REGISTER_CREATEABLE(actionFactory, CheckDelayedIntervention);
    REGISTER_CREATEABLE(actionFactory, DisplayImmediateIntervention);
    REGISTER_CREATEABLE(actionFactory, DisplayDelayedIntervention);
    REGISTER_CREATEABLE(actionFactory, CheckDefaultButton);
    REGISTER_CREATEABLE(actionFactory, Append1To9);
    REGISTER_CREATEABLE(actionFactory, Append0);
    REGISTER_CREATEABLE(actionFactory, ClearEntryText);
    REGISTER_CREATEABLE(actionFactory, ValidateBirthday);
    REGISTER_CREATEABLE(actionFactory, NotifyAssistMode);
    REGISTER_CREATEABLE(actionFactory, GenerateLocalButtonPress);
    REGISTER_CREATEABLE(actionFactory, ShowMessage);
    REGISTER_CREATEABLE(actionFactory, SwitchContextCommand);
    REGISTER_CREATEABLE(actionFactory, IsInputEventEnabled);
    REGISTER_CREATEABLE(actionFactory, CheckAssistModeControl);
    REGISTER_CREATEABLE(actionFactory, SendUnsolicitedData);
    REGISTER_CREATEABLE(actionFactory, IsNotStartTransactionView);
    REGISTER_CREATEABLE(actionFactory, IsNotStartTransactionAMView);
    REGISTER_CREATEABLE(actionFactory, CheckRAPDataNeededEvent);
    REGISTER_CREATEABLE(actionFactory, SaveAsDelayedIntervention);
    REGISTER_CREATEABLE(actionFactory, SetLaneName);
    REGISTER_CREATEABLE(actionFactory, PushbackDelayedIntervention);
    REGISTER_CREATEABLE(actionFactory, SendSuspendRequest);
    REGISTER_CREATEABLE(actionFactory, SaveSuspendRequest);
    REGISTER_CREATEABLE(actionFactory, RequestFullScreenAssistMode);
    REGISTER_CREATEABLE(actionFactory, ReleaseFullScreenAssistMode);
    REGISTER_CREATEABLE(actionFactory, IsOutsideTransaction);
    REGISTER_CREATEABLE(actionFactory, ClearApprovedIntervention);
    REGISTER_CREATEABLE(actionFactory, ClearRemoteIntervention);
    REGISTER_CREATEABLE(actionFactory, SaveMinimumAge);
    REGISTER_CREATEABLE(actionFactory, ScrollReceipt);
    REGISTER_CREATEABLE(actionFactory, ClearRAPDataNeededIntervention);
    REGISTER_CREATEABLE(actionFactory, IsInterventionOutstanding);
	REGISTER_CREATEABLE(actionFactory, IsCurrentInterventionInProgress);
    REGISTER_CREATEABLE(actionFactory, ClearLaneStatusWindows);
    REGISTER_CREATEABLE(actionFactory, AppendDot);
    REGISTER_CREATEABLE(actionFactory, ClearReceipt);
    REGISTER_CREATEABLE(actionFactory, SaveRemoteConnection);
    REGISTER_CREATEABLE(actionFactory, SwitchContextDelayedIntervention);
    REGISTER_CREATEABLE(actionFactory, IsHandheld);
    REGISTER_CREATEABLE(actionFactory, DisplayNextDelayedIntervention);
    REGISTER_CREATEABLE(actionFactory, DisplayAgeRestrictionInstructionText);
    REGISTER_CREATEABLE(actionFactory, GenerateEvent);
    REGISTER_CREATEABLE(actionFactory, EnableInputEvents);
    REGISTER_CREATEABLE(actionFactory, IsNotConfirmSuspend);
    REGISTER_CREATEABLE(actionFactory, IsNotMaxOfDOBInput);
    REGISTER_CREATEABLE(actionFactory, RemoteDataCommand);
    REGISTER_DERIVED_CREATEABLE(actionFactory, RemoteDataCommandRAP);
    REGISTER_CREATEABLE(actionFactory, IsRemoteConnectionAvailable);
    REGISTER_CREATEABLE(actionFactory, SendInterventionEvent);
    REGISTER_CREATEABLE(actionFactory, SendAllInterventionEvents);
    REGISTER_CREATEABLE(actionFactory, SendClientMachineNameEvent);
    REGISTER_CREATEABLE(actionFactory, IsNotHandheld);
    REGISTER_CREATEABLE(actionFactory, ConnectCommandRAP);
    REGISTER_CREATEABLE(actionFactory, SwitchContextImmediateIntervention);
    REGISTER_CREATEABLE(actionFactory, IsExclusiveAccessGranted);
    REGISTER_CREATEABLE(actionFactory, SetReleaseExclusiveAccessRequired);
    REGISTER_CREATEABLE(actionFactory, IsCurrentRemoteConnection);
    REGISTER_CREATEABLE(actionFactory, IsInterventionTypeOutstanding);
    REGISTER_CREATEABLE(actionFactory, DisableAMOnHandheld);
    REGISTER_CREATEABLE(actionFactory, ConnectCommandComplete);
    REGISTER_CREATEABLE(actionFactory, MediaLowEvent);
    REGISTER_CREATEABLE(actionFactory, CheckForDateNotNeededButtonOption);
    REGISTER_CREATEABLE(actionFactory, IsRemoteConnectionInputEnabled);
    REGISTER_CREATEABLE(actionFactory, IsHandheldAssistModeAllowed);
    REGISTER_CREATEABLE(actionFactory, IsAssistModeEnabled);
    REGISTER_DERIVED_CREATEABLE(actionFactory, SendAllLaneStatusSummary);
    REGISTER_CREATEABLE(actionFactory, ClearCurrentIntervention);
    REGISTER_CREATEABLE(actionFactory, SwitchContextRAPDataNeededIntervention);
    REGISTER_DERIVED_CREATEABLE(actionFactory, ForwardAuxData);
    REGISTER_DERIVED_CREATEABLE(actionFactory, SelectKeyDown);
    REGISTER_DERIVED_CREATEABLE(actionFactory, IsCurrentTriliteColor);
    REGISTER_DERIVED_CREATEABLE(actionFactory, IsButtonEnabled);
    REGISTER_DERIVED_CREATEABLE(actionFactory, IsTriliteColorGray);
    REGISTER_CREATEABLE(actionFactory, PushbackImmediateIntervention);
    REGISTER_DERIVED_CREATEABLE(actionFactory, PlaySound);
    REGISTER_DERIVED_CREATEABLE(actionFactory, PlayInterventionSound);
    REGISTER_DERIVED_CREATEABLE(actionFactory, ClearAllInterventions);
    REGISTER_DERIVED_CREATEABLE(actionFactory, MakeInterventionCurrent);
    REGISTER_DERIVED_CREATEABLE(actionFactory, MatchLaneContext);
	REGISTER_DERIVED_CREATEABLE(actionFactory, CompareToCurrentContext);
    //REGISTER_CREATEABLE(actionFactory, DisableUserInput);
    REGISTER_DERIVED_CREATEABLE(actionFactory, AcquireFullScreen);
    REGISTER_DERIVED_CREATEABLE(actionFactory, ReleaseFullScreen);
    REGISTER_DERIVED_CREATEABLE(actionFactory, SaveStartTransactionIntervention);
    REGISTER_DERIVED_CREATEABLE(actionFactory, AcquireExclusiveAccess);
    REGISTER_DERIVED_CREATEABLE(actionFactory, ReleaseExclusiveAccess);
    REGISTER_DERIVED_CREATEABLE(actionFactory, SwitchLaneContextCommand);
    REGISTER_DERIVED_CREATEABLE(actionFactory, IsNotImmediateInterventionOutstanding);
    REGISTER_DERIVED_CREATEABLE(actionFactory, DisplayNextImmediateIntervention);
    REGISTER_DERIVED_CREATEABLE(actionFactory, DisplayCurrentIntervention);
	REGISTER_CREATEABLE(actionFactory, IsLastDelayIntervention);
	REGISTER_CREATEABLE(actionFactory, IsForceApprovalBeforeSuspend);
    REGISTER_CREATEABLE(actionFactory, SetText);
    REGISTER_CREATEABLE(actionFactory, SetTextColor);
	REGISTER_DERIVED_CREATEABLE(actionFactory, IsTriliteColorGreen);
}


void __declspec(dllexport) UnregisterAll()
{
}


HRESULT __declspec(dllexport) CreateObject(const _TCHAR *pszObjectName, IAction **ppAction)
{
    HRESULT hr = E_FAIL;

    hr = actionFactory.NewInstance(ppAction, pszObjectName);

    return hr;
}


HRESULT __declspec(dllexport) ReleaseObject(const _TCHAR *pszObjectName, IAction *pAction)
{
    HRESULT hr = E_FAIL;

    hr = actionFactory.ReleaseInstance(pAction, pszObjectName);

    return hr;
}

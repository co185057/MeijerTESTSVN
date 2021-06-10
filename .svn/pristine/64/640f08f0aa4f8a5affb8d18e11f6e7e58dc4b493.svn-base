#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RemoteAPMgr.h 5     1/12/10 11:59p Cu185008 $
/*
 * @(#)RCManager.h    $Revision: 5 $ $Date: 1/12/10 11:59p $
 *
 * Copyright 2004 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RemoteAPMgr.h $
 * 
 * 5     1/12/10 11:59p Cu185008
 * G2 Migration.
 * 
 * 22    3/17/09 1:27p De200005
 * TAR 390603 added access protection to intervention list
 * 
 * 21    1/22/08 5:26a pl250020
 * TAR 352409 : Added code for Suspension Decline Event 
 *
 * 20    11/29/07 4:33p Cb160006
 * TAR 364905
 *
 * 19    12/22/06 10:35a Jj100011
 * RFC 337360
 *
 * 18    10/20/06 9:50a Tn185011
 * Roll back to 244
 *
 * 16    6/23/06 8:33p Lt185007
 * tar migration 300190
 *
 * 15    5/31/05 11:32a Dm185016
 * Data Model Support
 * TAR 300883
 *
 * 15    5/20/05 3:29p Dm185016
 * TAR 301079
 *
 * 14    5/19/05 5:21p Dm185016
 * TAR 300190
 *
 * 13    5/02/05 2:58p Tp151000
 * TAR 299442
 *
 * 12    5/01/05 8:57a Tp151000
 * Performmance
 *
 * 11    4/15/05 9:11a Dm185016
 *
 * 10    4/14/05 10:05p Dm185016
 * TAR 298021
 *
 * 9     4/05/05 11:54a Dm185016
 * TAR 297356
 *
 * 8     3/11/05 1:08p Dm185016
 * Factored out RAP specific API features
 *
 * 7     3/08/05 4:36p Tp151000
 * 295124
 *
 * TP & AQ
 *
 * 6     3/01/05 4:42p Dm185016
 *
 * 5     3/01/05 1:59p Dm185016
 * TAR 294587.  Added application state functionality.
 *
 * 3     2/17/05 5:51p Dm185016
 * Replaced std::stack with std::list.
 *
 * 2     2/03/05 5:41p Dm185016
 * TAR 292378 - Converted single immediate intervention to use of stack
 *
 * 1     1/26/05 2:17p Dm185016
 * Moved to new Repository.
*
* 13    1/20/05 10:35a Dm185016
* Added handler for lane context switches.
*
* 12    1/19/05 2:07p Dm185016
* TAR 290775 - Operator not available on any but the one that handled the
* signon.
*
* 11    1/12/05 5:22p Dm185016
* TAR 290169 - Multiple interventions of the same kind were not clearing
* with single approval.
*
* 10    12/22/04 10:29a Dm185016
* TAR 288934 - priority of interventions is now respected.  Urgent
* interventions are added to the head of the Q.  deferred are still added
* at the tail.
*
* 9     12/14/04 11:16a Dm185016
* Removed unneccessary code for HH support
*
* 8     12/09/04 1:44p Dm185016
* TARs 287903, 287907
*
* 7     12/02/04 3:31p Dm185016
* Added inside transaction indicator
*
* 6     11/30/04 4:17p Dm185016
* Added PushbackDelayedIntervention().
* Delayed intervention list now derived from std::list rather than
* std::queue.
*
* 5     11/22/04 3:42p Dm185016
* Added method to set the current intervention.
*
* 4     11/19/04 1:32p Dm185016
* Added current intervention field
*
* 3     11/16/04 11:23a Dm185016
* More updates
*
* 2     10/29/04 8:52a Dm185016
* Added delayed intervention functions.
* Added config for RAP.ini
*
* 1     10/21/04 2:36p Dm185016
*
* 2     10/14/04 1:38p Dm185016
* New OnInputMethod Api.
* PSX now passed as handle.
*
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/

/*lint -save -e1725 -e763 */

#include "singleton.h"
#include "RAPApi.h"
#include "RCManager.h"
#include "RCMEvent.h"
#include "SMConstants.h"
#include "Approval.h"
#include "StringComparator.h"
#include "KeyValueHandler.h"
#include "DataModel.h"
#include <set>
#include <list>
#include <map>
#include <stack>


class CResource; //TR390603
namespace STATE_MACHINE_NAMESPACE
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

namespace fastlane
{
    class CRCMStateMachine;
    class CRCMRecorder;
    class CContextSwitch;
}
using namespace fastlane;


class CRemoteConsoleManagerCtrl;
class CXMLErrorInfo;
class CSMError;
class CConfigManager;
class CPSX;
enum EConnectionModes;

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace rap
{

class CApplicationState;
class CRCMStateMachineRAP;
class COperator;

//typedef IMessage CIntervention;
typedef std::list<CIntervention *> CInterventionList;
typedef std::map<StringType, CIntervention *, StringComparator> CInterventionMap;

typedef bool (* PACCEPT_FXN)(CIntervention *);

typedef enum
{
    None = 0,
    Gray,
    Red,
    Yellow,
    Green
} ETriliteColors;


class RAP_API CRemoteAPMgr
    : public CSingleton<CRemoteAPMgr>
    , public CRCManager
    , public CDataModel<CRemoteAPMgr>
{

public:

    friend class CDataModel<CRemoteAPMgr>;

    //BEGIN_PROPERTIES_TABLE(RemoteAPMgr)
    //    DEFINE_BOOL_PROPERTY(AssistModeRequested)
    //    DEFINE_BOOL_PROPERTY(ImmediateInterventionPending)
    //    DEFINE_BOOL_PROPERTY(DelayedInterventionPending)
    //    DEFINE_INT_PROPERTY(ConnectionMode)
    //END_PROPERTIES_TABLE()

    friend class CSingleton<CRemoteAPMgr>;

    virtual ~CRemoteAPMgr();

    CRCMStateMachineRAP *GetStateMachineRAP();

    virtual HRESULT LoadOptions();

    virtual unsigned long GetNextItem() throw();

    void SetCurrentApplicationState(CApplicationState *);
    CApplicationState *GetCurrentApplicationState();

	void SetAssistModeEnabled(bool bAMEnabled);
	bool IsAssistModeEnabled() const;

	void SetHandheldAssistModeEnabled(bool bAMEnabled);
	bool IsHandheldAssistModeEnabled() const;

	void SetConnectionMode(EConnectionModes eConnectionMode);
	EConnectionModes GetConnectionMode() const;

	void SetAssistModeRequested(bool bAMRequested);
	bool IsAssistModeRequested() const;

	void SetImmediateIntervention(CIntervention * pIntervention);
	CIntervention * GetImmediateIntervention() const;
    bool IsImmediateInterventionPending() const;
    int GetImmediateInterventionPendingCount() const;
	CIntervention * GetNextImmediateIntervention();
	CIntervention * PeekNextImmediateIntervention();
    CInterventionList &GetImmediateInterventions();
    void RemoveImmediateIntervention(CIntervention * pIntervention);
	void PushbackImmediateIntervention(CIntervention * pIntervention);

    bool IsDelayedInterventionTypePending(const _TCHAR *) const;
    bool IsDelayedInterventionPending();  //TR390603
    int GetDelayedInterventionPendingCount(); //TR390603
	void SetNextDelayedIntervention(CIntervention * pIntervention, DWORD dwPriority);
	void PushbackDelayedIntervention(CIntervention * pIntervention);
	CIntervention * GetNextDelayedIntervention();
	CIntervention * GetInterventionByType(const _TCHAR *);
    CIntervention * PeekNextDelayedIntervention();
    void RemoveDelayedInterventions(PACCEPT_FXN pFxn);
    void RemoveDelayedIntervention(CIntervention * pIntervention, bool bRemoveFromList=true);
    CInterventionList &GetDelayedInterventions();

    void SetCurrentIntervention(CIntervention *);
    CIntervention * GetCurrentIntervention();

    bool HasOperator () const;
    void SetOperator (COperator *pOperator);
    const COperator &GetOperator () const;

    void SetLaneName (const _TCHAR *pszLaneName);
    const _TCHAR *GetLaneName () const;

    void SetAssistModeControl (long lValue);
    long GetAssistModeControl () const;

    void SetClearReceiptTime (long lValue);
    long GetClearReceiptTime() const;

    void SetReminderDisplayTime (long lValue);
    long GetReminderDisplayTime() const;

    void SetConfirmationForSuspendingTransaction (bool lValue);
    bool IsConfirmationForSuspendingTransaction () const;

    void SetAutoAssistOnEnable (bool lValue);
    bool IsAutoAssistOnEnable() const;

    void SetAllowParialAssistMode (bool lValue);
    bool IsAllowParialAssistMode() const;

    void SetInputEventsEnabled (bool lValue);
    bool IsInputEventsEnabled() const;

    void SetInsideTransaction (bool lValue=true);
    bool IsInsideTransaction() const;
    bool IsOutsideTransaction() const;

    const _TCHAR *GetCurrentLaneContext() const;
    void SetCurrentLaneContext(const _TCHAR *);

    ETriliteColors GetCurrentTriliteColor() const;
    void SetCurrentTriliteColor(ETriliteColors);
    bool IsCurrentTriliteColor(ETriliteColors) const;

    ETriliteColors GetPreviousInterventionColor() const;
    void SetPreviousInterventionColor(ETriliteColors);

    CContextSwitch *GetCurrentContextSwitch();
    void SetCurrentContextSwitch(CContextSwitch *);

    bool HasEntryText() const;
    const _TCHAR *GetEntryText() const;
    void SetEntryText(const _TCHAR *);
    void AppendText(const _TCHAR *sChar);
    void ResetEntryText();

    virtual bool CanApprove(LPCTSTR) const;

    void SetCurrentApproval(PApproval pApproval);
    CApproval &GetCurrentApproval();

    void SetMinimumAge(int);
    const COleDateTime &GetValidBirthday() const;

    HRESULT PostMsg(CIntervention * pIntervention, const void *pData=NULL, size_t nDataLength=0);

    virtual const _TCHAR *GetDataModelName() const;
	void SetForceApprovalBeforeSuspend(bool bApprovalBeforeSuspend);
	bool IsForceApprovalBeforeSuspend() const;

// API methods
public:

    virtual LONG CanApprove(LPCTSTR bstrInParms, LONG* pResult);
    virtual LONG GetApprovalInfo(LPCTSTR bstrInParms, BSTR* pbstrOutParms);
    virtual LONG OnApproval(LPCTSTR bstrInParms);

protected:

    CRemoteAPMgr();                                     // Hidden constructor

    CRCMStateMachine *GetStateMachine(const _TCHAR *pSMName);

    void SaveInterventionType(CIntervention * pIntervention);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(InterventionParms)
        DECLARE_KV_CSTRING(Type)
        DECLARE_KV_CSTRING(Priority)
    DECLARE_KV_HANDLER_CLASS_END

private:

    CRemoteAPMgr(const CRemoteAPMgr &);             // Hidden Copy construcor
    CRemoteAPMgr &operator =(const CRemoteAPMgr &); // Hidden assignment operator

private:

	void LockInterventionsList(unsigned long ulTimeout=500); //TR390603
    void UnlockInterventionsList(); //TR390603
    CRCMStateMachineRAP*m_pStateMachine;

    CApplicationState  *m_pApplicationState;
    CApplicationState  *m_pNullApplicationState;

    CIntervention      *m_CurrentIntervention;
    CInterventionList   m_ImmediateInterventions;
    CInterventionList   m_Interventions;
    CResource          *m_presInterventionsListLock; //TR390603
    CInterventionMap    m_InterventionTypes;

    PApproval           m_pCurrentApproval;
    COperator          *m_pOperator;
    CContextSwitch     *m_pCurrentContextSwitch;

    bool					m_bAssistModeEnabled;
    bool					m_bHHAssistModeEnabled;
    bool					m_bAssistModeRequested;
    bool					m_bInputEventsEnabled;
    bool					m_bInsideTransaction;

    bool m_bAllowSecurityApproval;
    bool m_bAllowCouponApproval;
    bool m_bAllowVoidApproval;
    bool m_bAllowAgeRestrictionApproval;
    bool m_bAllowVisualValidationApproval;
	bool m_bAllowHopperSubstitutionApproval;
    bool m_bAllowCallForHelpApproval;
    bool m_bAllowOtherPaymentApproval;
    bool m_bAllowSuspendApproval;
    bool m_bAllowPriceRequiredApproval;
    bool m_bAllowUnknownItemApproval;
    bool m_bAllowRecalledItemApproval;
    bool m_bAllowTimeRestrictedItemApproval;
    bool m_bAllowSigCapApproval;
    bool m_bAllowApplicationErrorApproval;
    bool m_bAllowDeviceErrorApproval;
    bool m_bAllowQuantityRestrictedItemApproval;
    bool m_bAllowUndeactivatedItemApproval;
    bool m_bAllowRestrictedNotAllowedApproval;
    bool m_bAllowSuspensionDeclinedApproval;	// TAR 352409


    long m_lReminderDisplayTime;
    long m_lAssistModeControl;
    long m_lClearReceiptTime;
    bool m_bConfirmationForSuspendingTransaction;
    bool m_bAutoAssistOnEnable;
    bool m_bAllowParialAssistMode;
	bool m_bApprovalBeforeSuspend;

    unsigned long m_lItemCount;

    StringType m_sEntryText;
    StringType m_sLaneName;
    StringType m_sCurrentLaneContext;

    EConnectionModes m_eConnectionMode;

    COleDateTime m_ValidBirthday;

    ETriliteColors m_eCurrentTriliteColor;
    ETriliteColors m_ePreviousInterventionColor;

    DECLARE_DATA_MODEL_BEGIN(CRemoteApMgr)
        DECLARE_DATA_MODEL_BOOL_PROPERTY(AssistModeEnabled)
        DECLARE_DATA_MODEL_BOOL_PROPERTY(HandheldAssistModeEnabled)
    DECLARE_DATA_MODEL_END()

};
//BEGIN_DATA_MODEL(CRemoteApMgr)
//    DATA_MODEL_BOOL_PROPERTY(AssistModeEnabled)
//    DATA_MODEL_BOOL_PROPERTY(HandheldAssistModeEnabled)
//    DATA_MODEL_ENUM_PROPERTY(ConnectionMode)
//END_DATA_MODEL()


}

#pragma warning( pop )

/*lint -restore */

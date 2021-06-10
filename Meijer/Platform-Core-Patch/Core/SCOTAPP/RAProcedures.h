#ifndef _RAPROCEDURES_INCLUDED
#define _RAPROCEDURES_INCLUDED

#include <afxtempl.h>
#include "scotdeferredoverride.h"
#include "RAInterface.h"
#include "RAViews.h"
#include "RAConnections.h"
#include "RAEvents.h"
#include "PSXInterface.h"
#include "KeyValueHandler.h"


#include <set>
#include <map>

typedef DWORD RAHANDLE;

typedef std::set<CString> CConnections;


typedef BOOL (CALLBACK *PRACALLBACK) (UINT Msg, 
                                      LPARAM lParam1, 
                                      LPARAM lParam2, 
                                      LPARAM lParam3, 
                                      LPARAM lParam4,
                                      LPARAM lParam5,
                                      LPARAM appParam, 
                                      DWORD dwCookie);

// Callback will the called with Msg = any of the following message types:

#define WMRA_REQUEST_APPROVAL    WM_USER + 1
// lParam1   = RAHANDLE handle          (handle of approval)
// lParam2   = int      approvalState   (requested approval code - 
//                                       use RA_CODE's in SCOTRAEventInfo.h)
// lParam3   = LPCTSTR  id              (id of user requesting approval)
// lParam4   = LPCTSTR  password        (password of user requesting approval)
// appParam  = parameter passed to RAInitialize - for application use.
// dwCookie  = should be passed AS IS back to RAUpdateApproval()


#define WMRA_VALIDATE_OPERATOR   WM_USER + 2
// lParam1   = LPCTSTR  id              (id of user requesting validation)
// lParam2   = LPCTSTR  password        (password of user requesting validation)
// lParam3   = NULL
// lParam4   = NULL
// appParam  = parameter passed to RAInitialize - for application use.
// dwCookie  = should be passed AS IS back to RAOperatorIsValid()

#define WMRA_REQUEST_SUSPEND     WM_USER + 3
// lParam1   = LPCTSTR  desc            (desc. of why suspend was requested)
// lParam2   = NULL
// lParam3   = NULL
// lParam4   = NULL
// appParam  = parameter passed to RAInitialize - for application use.
// dwCookie  = should be ignored

#define WMRA_CANCEL_SUSPEND     WM_USER + 4
// lParam1   = LPCTSTR  desc            (desc. of why suspend was requested)
// lParam2   = NULL
// lParam3   = NULL
// lParam4   = NULL
// appParam  = parameter passed to RAInitialize - for application use.
// dwCookie  = should be ignored

#define WMRA_SEND_CLIENT_MACHINE_NAME     WM_USER + 5
// lParam1   = LPCTSTR  clientname            (clientname. client machine name)
// lParam2   = NULL
// lParam3   = NULL
// lParam4   = NULL
// appParam  = parameter passed to RAInitialize - for application use.
// dwCookie  = should be ignored

#define WMRA_SEND_UNSOLICITED_DATA     WM_USER + 6
// lParam1   = LPCTSTR  message            (text message from RAP to SCOT)
// lParam2   = NULL
// lParam3   = NULL
// lParam4   = NULL
// appParam  = parameter passed to RAInitialize - for application use.
// dwCookie  = should be ignored


//tar371918
#define WMRA_SEND_INTERVENTION_APPROVED     WM_USER + 7
// lParam1   = NULL
// lParam2   = NULL
// lParam3   = NULL
// lParam4   = NULL
// appParam  = parameter passed to RAInitialize - for application use.
// dwCookie  = should be ignored

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
  RAProcedures : public CWnd
{
public:

    friend class RAInterface;
    typedef std::map<StringType, int, StringComparator> CApprovalTypeMap;
    typedef std::map<int, StringType> CCategoryMap;

    DECLARE_KV_HANDLER_CLASS_BEGIN(ConnectParms)
        DECLARE_KV_LONG(HandheldRAP)
        DECLARE_KV_CSTRING(StartContext)
    DECLARE_KV_HANDLER_CLASS_END

    DECLARE_KV_HANDLER_CLASS_BEGIN(SignonParms)
        DECLARE_KV_CSTRING(UserId)
        DECLARE_KV_CSTRING(UserPwd)
        DECLARE_KV_CSTRING(ScannerData)
    DECLARE_KV_HANDLER_CLASS_END

    DECLARE_KV_HANDLER_CLASS_BEGIN(ScannerParms)
        DECLARE_KV_CSTRING(ScannerData)
        DECLARE_KV_CSTRING(LabelData)
        DECLARE_KV_LONG(Type)
    DECLARE_KV_HANDLER_CLASS_END

    DECLARE_KV_HANDLER_CLASS_BEGIN(BiometParms)
        DECLARE_KV_CSTRING(BiometData)
        DECLARE_KV_CSTRING(EncodedData)
    DECLARE_KV_HANDLER_CLASS_END

    DECLARE_KV_HANDLER_CLASS_BEGIN(GetCurrentApprovalParms)
        DECLARE_KV_CSTRING(ApprovalType)
    DECLARE_KV_HANDLER_CLASS_END

    DECLARE_KV_HANDLER_CLASS_BEGIN(GetRAPDataPendingCountParms)
        DECLARE_KV_LONG(RAPDataPendingCount)
    DECLARE_KV_HANDLER_CLASS_END

    DECLARE_KV_HANDLER_CLASS_BEGIN(GetLongValueParms)
        DECLARE_KV_LONG(Value)
    DECLARE_KV_HANDLER_CLASS_END

    DECLARE_KV_HANDLER_CLASS_BEGIN(GetBoolValueParms)
        DECLARE_KV_BOOL(Value)
    DECLARE_KV_HANDLER_CLASS_END
    
    DECLARE_KV_HANDLER_CLASS_BEGIN(GetStringValueParms)
        DECLARE_KV_CSTRING(Value)
    DECLARE_KV_HANDLER_CLASS_END

    DECLARE_KV_HANDLER_CLASS_BEGIN(GetRequestEnableAssistModeParms)
        DECLARE_KV_BOOL(RequestEnableAssistMode)
    DECLARE_KV_HANDLER_CLASS_END
    
    DECLARE_KV_HANDLER_CLASS_BEGIN(GetRequestEnableHandheldAssistModeParms)
        DECLARE_KV_BOOL(RequestEnableHandheldAssistMode)
    DECLARE_KV_HANDLER_CLASS_END

public:

    RAProcedures();
    virtual ~RAProcedures();

    long Initialize(CWnd* pWnd = NULL); // called by MP at start up
    long UnInitialize(void);            // called by MP at shut down
    long ReloadOptions(void);
    bool m_bRCMgrICreated;
    bool m_bRCMgrPCreated;
    bool m_bDoRCMgrInit;

// Attributes
public:

// Operations
public:

    virtual BOOL CanApproveSecurity( LPCTSTR sOpId, LPCTSTR sPassword ) const;
    virtual BOOL CanApproveCoupons( LPCTSTR sOpId, LPCTSTR sPassword ) const;
    virtual BOOL CanApproveVoids( LPCTSTR sOpId, LPCTSTR sPassword ) const;
    virtual BOOL CanApproveAgeRestrictions( LPCTSTR sOpId, LPCTSTR sPassword ) const;
    virtual BOOL CanApproveVisualValidation( LPCTSTR sOpId, LPCTSTR sPassword ) const;
    virtual BOOL CanApproveCallForHelp( LPCTSTR sOpId, LPCTSTR sPassword ) const;
    virtual BOOL CanApproveOtherPayment( LPCTSTR sOpId, LPCTSTR sPassword ) const;
    virtual BOOL CanApproveSuspends( LPCTSTR sOpId, LPCTSTR sPassword ) const;
    virtual BOOL CanApprovePriceRequireds( LPCTSTR sOpId, LPCTSTR sPassword ) const;
    virtual BOOL CanApproveUnknownItems( LPCTSTR sOpId, LPCTSTR sPassword ) const;
    virtual BOOL CanApproveRecalledItems( LPCTSTR sOpId, LPCTSTR sPassword ) const;
    virtual BOOL CanApproveTimeRestrictedItems( LPCTSTR sOpId, LPCTSTR sPassword ) const;
    virtual BOOL CanApproveSigCap( LPCTSTR sOpId, LPCTSTR sPassword ) const;
    virtual BOOL CanApproveApplicationErrors( LPCTSTR sOpId, LPCTSTR sPassword, DWORD nEventId ) const;
    virtual BOOL CanApproveDeviceErrors( LPCTSTR sOpId, LPCTSTR sPassword ) const;
    virtual BOOL CanApproveQuantityRestrictedItems( LPCTSTR sOpId, LPCTSTR sPassword ) const;
    virtual BOOL CanApproveUnDeActivatedItem ( LPCTSTR sOpId, LPCTSTR sPassword ) const;

    //TAR 192098 start
    DWORD GetCurrentApprovalCode(); // {return CurrentApprovalCode;}
    //TAR 192098 end

public:
    /* 1. Age Restriction operations */
    void OnNoAgeRestrictions( void );
    void OnAgeRestrictedItem( LPCTSTR desc,int nMinAge, BOOL bVoided, BOOL bDOBNotNeededBtn = true );
    BOOL OnAgeApproval( void );
    void GetAgeRestrictionInfo( SCOTDO_INFO* Info )  const;

    /* 2. Coupon operations */
    void OnCouponItem( LPCTSTR sDesc, long nAmount, bool bNeedApproval);
    BOOL OnCouponApproval( void );
    void GetCouponInfo(SCOTDO_INFO *pInfo ) const;

    //Added by Nitin for the coupon & nsc4 handling
    int GetCouponTotalCount();  
    // Nitin end 
        
    /* 3. Voided Item Operations */
    void OnVoidedItem( LPCTSTR desc, long amount, long gross );
    BOOL OnVoidApproval( void );
    void GetVoidInfo( SCOTDO_INFO * pInfo ) const;

    /* 4. Security Operations */
    void OnSecurityEvent( LPCTSTR sDesc, DWORD nConclusion, int nLevel );
    BOOL OnSecurityApproval( void );
    void GetSecurityInfo( SCOTDO_INFO * pInfo ) const;

    void OnEnableSecurity( void ); // SR683
    void OnDisableSecurity( void ); // SR683

    /* 5. Visual Verify Operations */
    void OnVisualValidationItem(LPCTSTR desc, BOOL bVoided );
	void OnHopperSubstitution(LPCTSTR desc);
	BOOL OnHopperSubstitutionApproval( void );

    BOOL OnVisualValidationApproval( void );
    void GetVisualValidationInfo( SCOTDO_INFO * pInfo ) const;

    /* 6. Suspend Operations */
    void OnRequestSuspend( LPCTSTR sReason );
    void OnReadyToSuspend( void );
    BOOL OnSuspendApproval( void );

    /* 7. Unknown Price Operations */
    BOOL OnUnknownPriceApproval(void );
    void OnUnknownPrice( LPCTSTR desc );
    void GetUnknownPriceInfo( SCOTDO_INFO * pInfo ) const;

    /* 8. Recalled Item Operations */
    BOOL OnRecalledItemApproval( void );
    void OnRecalledItem( LPCTSTR desc );
    void GetRecalledItemInfo( SCOTDO_INFO * pInfo ) const;

    /* 9. Unknown Item  Operations */
    BOOL OnUnknownItemApproval( void );
    BOOL OnUnknownItem( LPCTSTR desc, BOOL bKeyed );
    void GetUnknownItemInfo( SCOTDO_INFO * pInfo ) const;

    /* 10. Device Error Operations */
    
    // Begin TAR 192405, 192461

    //These two methods are NOT used anymore 

    // In the place of call to OnDeviceError(), OnRAPDataNeeded() is called now
    // since RAP is notified of any Device errors as an DataNeeded Intervention.
    // void OnDeviceError( const TCHAR *Error,  int nDeviceClass, int nDeviceId );
    
    // In the place of call to ClearDeviceError(), OnNoRAPDataNeeded() is called now
    // since RAP is notified of any Device errors as an DataNeeded Intervention.
    //BOOL ClearDeviceError( void );


    // End TAR 192405, 192461

    void GetDeviceErrorInfo( SCOTDO_INFO * pInfo ) const;

    /* 11. Application Error Operations */
    void OnApplicationErrorEvent( LPCTSTR desc, int nRaCat, int nEventId );
    BOOL OnApproveApplicationError( void );
    void OnCancelApplicationError( void );
    void GetApplicationErrorInfo( SCOTDO_INFO * pInfo ) const;

    /* 12. Time Restricted Item Operations */
    BOOL OnTimeRestrictedItemApproval( void );
    void OnTimeRestrictedItem( LPCTSTR desc );
    void GetTimeRestrictedItemInfo( SCOTDO_INFO * pInfo ) const;
 
    /* 13. SigCap  Operations */
    BOOL OnSigCapApproval( void );
    void OnSigCapData( LPCTSTR sDesc, LPCTSTR cSigData, DWORD nLength );
    void GetSigCapInfo( SCOTDO_INFO * pInfo ) const;

    /* 14. Collection Operations */
    void OnReadyToCollect( LPCTSTR  sDesc );

    /* 15. DataNeeded Operations */
    void OnNoRAPDataNeeded( void );
    void OnRAPDataNeeded( LPCTSTR desc, LPCTSTR view, int iView, LPCTSTR sReportCategoryName= _T("") );
    void OnRAPDataNeeded( LPCTSTR desc, int iView );
    BOOL IsRAPDataAvailable( void ) const;
    CString GetRAPData( void ) const;
    int GetRAPPreviousView( void ) const;
    int GetRAPView( void ) const;
    int GetRAPDataPendingCount( void ) const;

    /* 15. Miscellaneous Functions */
    void OnTransactionStart( void );
    void OnTransactionEnd( void );
    void OnSetTransactionState(int state);
    void GetApprovalInfo( SCOTDO_APPROVAL_INFO* pInfo ) const;
    void OnRemoteApproval( DWORD dwHandle, LPCTSTR  sOpId, LPCTSTR  sPassword,
        int nApprovalCode, LPCTSTR sAuxdata, DWORD dwCookie );
    void OnNormalItem( LPCTSTR  sDesc );
    void OnEnterStoreMode( LPCTSTR sOpId, LPCTSTR sPassword );
    void OnExitStoreMode( void );
    void OnMediaLow( LPCTSTR sDesc, bool bMediaLow);
    void OnOutOfService( LPCTSTR sDesc );
    void OnReturnToService( void );
    void OnCallForHelp( LPCTSTR sDesc );
    BOOL OnCallForHelpApproval( void );
    void SetTransactionTotal( LPCTSTR sAmount );
    void ValidateOperator( LPCTSTR desc, LPCTSTR operatorID, LPCTSTR password,
        BOOL bValid, DWORD dwCookie );
    void SetLightState( int greenState, int yellowState, int redState);
    void OnCanSuspendNow(BOOL bCanSuspendNow ) ;

    /* 16. Other Payment Operations */
    void OnOtherPaymentEvent(LPCTSTR desc, BOOL bVoided );
    BOOL OnOtherPaymentApproval( void );
    void GetOtherPaymentInfo( SCOTDO_INFO * pInfo ) const;
    void OnAddOtherEvent( LPCTSTR sDesc );

    /* 17. Quantity Restricted Item Operations */
    BOOL OnQuantityRestrictedItemApproval( void );
    void OnQuantityRestrictedItem( LPCTSTR desc );
    void GetQuantityRestrictedItemInfo( SCOTDO_INFO * pInfo ) const;

    /* 17. EAS Item Active / UnDeaActivated items Operations */   
    void OnUnDeActivatedItem(LPCTSTR desc);
    BOOL OnUnDeActivatedItemApproval( void );
    void GetUnDeActivatedItemInfo( SCOTDO_INFO * pInfo ) const;

    /* 18. Restricted Not Allowed Operations */
    BOOL OnRestrictedNotAllowedApproval( void );
    void OnRestrictedNotAllowed( LPCTSTR desc );
    void GetRestrictedNotAllowedInfo( SCOTDO_INFO * pInfo ) const;

    void RequestEnableAssistMode(bool); //Tar213249
    bool GetRequestEnableAssistMode(void);  //Tar213249

    void RequestEnableHandheldAssistMode(bool);
    bool GetRequestEnableHandheldAssistMode(void);

    long GetConnectionMode();

    bool IsRemoteMode() const;

    void DisconnectFromServer();

    void AcceptConnection(const CString &, const CString &);

    bool IsRemoteConnectionHandheld(const CString &);
    CString GetRemoteConnectionName();

    void AcquireExclusiveAccess(const CString &);
    void ReleaseExclusiveAccess();

    BOOL SignOn( const CString &sRemoteConnectionName
               , const CString &sUserId
               , const CString &sUserPwd
               , BOOL bValid);
	void SetIsRemoteApprovalFlag(bool bValue);
	void ForceApprovalBeforeSuspend(bool);

public:

    static bool RAProcedures::EventHandler( CPSX *pPSX
                                          , PPSXEVENT pEvent );

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(RCMgrProcedures)
  //}}AFX_VIRTUAL
    
protected:

    mutable RAInterface m_rcmi;

    //{{AFX_MSG(RCMgrProcedures)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    LONG GetLongValue(const _TCHAR *pszId, long lDefaultValue);
    bool GetBoolValue(const _TCHAR *pszId, bool lDefaultValue);
    CString GetStringValue(const _TCHAR *pszId, const _TCHAR *szDefaultValue);

    virtual bool DisconnectFromRemote( PPSXEVENT pEvent );
    virtual bool ConnectToRemote( PPSXEVENT pEvent );
    virtual bool ForwardInput( PPSXEVENT pEvent );
    virtual BOOL SignOn( PPSXEVENT pEvent );
    virtual bool SignOff( PPSXEVENT pEvent );
    virtual bool SwitchContext( PPSXEVENT pEvent );
    virtual bool SwitchLaneContext( PPSXEVENT pEvent );
    virtual bool ScannerData( PPSXEVENT pEvent );
    virtual bool BiometricData( PPSXEVENT pEvent );
    virtual bool RemoteData( PPSXEVENT pEvent );

    bool ScannerData( const CString &sDataType
                    , const CString &sScannerData
                    , const CString &sScannerLabelData
                    , long lScannerType
                    , const CString &sAuxData = _T(""));
    
    bool BiometricData( const CString &sDataType
                    , const CString &sBiometData
                    , const CString &sEncodedData
                    , const CString &sAuxData = _T(""));

private:

    BOOL OnApproval( const _TCHAR *pszApprovalType );
    BSTR GetApprovalInfo( LPCTSTR pDesc ) const;
    BOOL CanApprove( LPCTSTR sType, LPCTSTR sOpId, LPCTSTR sPassword ) const;
    
    void AddApproval( const CKeyValueParmList &pList, LPCTSTR  sType, DWORD dwPriority = RA_PRIORITY_DEFER );

    void ReportInterventionStart( long lId, DWORD dwPriority, LPCTSTR pcszCategoryName );
    void ReportInterventionStart( long lId, DWORD dwPriority, int nView, LPCTSTR pcszReportCategoryName );
    void ReportInterventionEnd( long lId, int nView );
    void ReportInterventionEnd( long lId, LPCTSTR pcszCategoryName );

    const _TCHAR *GetPriority(DWORD dwPriority);

    DWORD GetCategory( LPCTSTR pcszCategory );

    CConnections m_connections;

    CApprovalTypeMap approvalMap;
    CCategoryMap    categoryMap;

    long    m_nApprovedCount;
    int     m_nRAPPreviousView;
    int     m_nRAPCurrentView;
	int		m_nInterventionView; //tar 393092: transaction xml file error one sided entry

    CString m_sSuspendReason;

    static long m_lInterventionCount;

    bool m_bExclusiveAccessGranted;

    // Mechanism to collect & match intervention Start/End pairs
    // so that they can be submitted for logging in TB.
private:
    typedef struct _SavedIntervention
    {
        CString csManager; // SOTF 6312
        long lOverride;
        long lLocation;
        long lStartTime;
        long lWaitTime;    
        long lTerminal;
        long lWaitTimeMs;  //SOTF 7242
    } SavedIntervention, * PSavedIntervention;
    typedef std::map<int, SavedIntervention> CSavedInterventionMap;

    bool m_bIsRemoteApproval;
	bool m_bInterventionStarted;
	long m_lCategoryNumber;
	int m_nViewNumber;
    void SaveInterventionForTLOG(long lId, long lOverride);
    void SendInterventionToTLOG(long lId, long lOverride);

    CSavedInterventionMap m_savedIntMap;

    
    CString GetRegString(LPCTSTR szKey, LPCTSTR szName);
    void SetRegString(LPCTSTR szKey, LPCTSTR szName, LPCTSTR szValue);
    
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
#endif

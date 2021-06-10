//
// SCOTRADll.h
//
// Header file exposing the SCOT RA "C" API's
//

#ifndef _SCOTRADLL_H_
#define _SCOTRADLL_H_


// Event and Approval Categories and Priorities are defined in the 
// following header file

#include "SCOTRAEventInfo.h"


#ifdef _SCOTRA_DLL
#define RAEXPORT __declspec( dllexport) 
#else
#define RAEXPORT __declspec( dllimport) 
#endif

#define RAEXPORTS   RAEXPORT WINAPI       // _stdcall convention
#define RAEXPORTC   RAEXPORT WINAPIV      // _cdecl   convention

typedef DWORD RAHANDLE;

// A function of the following type must be passed to RAInitialize()
// It will be called with various messages when things happen
// in SCOTRA.

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

// Structure returned when getting Event/Approval info

typedef struct
{
  RAHANDLE handle;
  LPTSTR    desc;
  int      descLen;
  LPTSTR    auxData;
  int      auxDataLen;
  DWORD    category;
  DATE     createTime;
  DWORD    priority;
  int      approvalState;

  // this field only valid when doing GetFirstUnique/GetNextUnique() and 
  // then calling GetInfo().  Otherwise, implicitly the count is always 1.
  int      count;

  union
  {
    struct
    {
      long amount;
      BOOL bVoided;
      BOOL bHighRisk;
    } negDeptInfo;
    struct
    {
      int conclusion;
    } securityInfo;
    struct
    {
      DATE birthDate;
      int minAgeInYears;
	  bool bDOBNotNeededBtn;
    } ageRestrictInfo;
    struct
    {
      long amount;
    } voidInfo;
    struct
    {
      int  numEventsApproved;
      BOOL bApprovedLocally;
      BOOL bIsRemotelyApprovable;
    } approvalInfo;
  };
} RAEVENTINFO;

typedef RAEVENTINFO* PRAEVENTINFO;


//  Use these constants for RAGetEventCounts(countType) and 
//  RAGetFirst/NextEvent(eventType)

#define RA_EVENT_ALL          0
#define RA_EVENT_APPROVED     1
#define RA_EVENT_UNAPPROVED   2

// Commnad constants to be used by each side to call into the other

#define RAP_CMD_ADD_APPROVAL                 (DWORD)0x00000001
#define RAP_CMD_CAN_SUSPEND_NOW              (DWORD)0x00000002
#define RAP_CMD_CLEAR_APPROVAL               (DWORD)0x00000003
#define RAP_CMD_OPERATOR_IS_VALID            (DWORD)0x00000004
#define RAP_CMD_REQUEST_DISCONNECT           (DWORD)0x00000005
#define RAP_CMD_SEND_AGE_RESTRICT_EVENT      (DWORD)0x00000006
#define RAP_CMD_SEND_DATA_NEEDED_EVENT       (DWORD)0x00000007
#define RAP_CMD_SEND_NEG_DEPT_EVENT          (DWORD)0x00000008
#define RAP_CMD_SEND_OTHER_EVENT             (DWORD)0x00000009
#define RAP_CMD_SEND_SECURITY_EVENT          (DWORD)0x0000000A
#define RAP_CMD_SEND_SIG_CAP_EVENT           (DWORD)0x0000000B
#define RAP_CMD_SEND_VOID_EVENT              (DWORD)0x0000000C
#define RAP_CMD_SET_CONFIGURED_CATEGORIES    (DWORD)0x0000000D
#define RAP_CMD_SET_LIGHT_STATE              (DWORD)0x0000000E
#define RAP_CMD_SET_TOTAL                    (DWORD)0x0000000F
#define RAP_CMD_SET_WLDB_STATE               (DWORD)0x00000010
#define RAP_CMD_STORE_MODE                   (DWORD)0x00000011
#define RAP_CMD_TRANSACTION_ACTIVE           (DWORD)0x00000012
#define RAP_CMD_TRANSACTION_STATE            (DWORD)0x00000013
#define RAP_CMD_UPDATE_APPROVAL              (DWORD)0x00000014

#define FASTLANE_CMD_CANCEL_SUSPEND          (DWORD)0x00000001
#define FASTLANE_CMD_INITIAL_REFRESH         (DWORD)0x00000002
#define FASTLANE_CMD_KEEP_ALIVE              (DWORD)0x00000003
#define FASTLANE_CMD_REQUEST_APPROVE         (DWORD)0x00000004
#define FASTLANE_CMD_REQUEST_SUSPEND         (DWORD)0x00000005
#define FASTLANE_CMD_SEND_CLIENT_MACHINE_NAME (DWORD)0x00000006
#define FASTLANE_CMD_SEND_UNSOLICITED_DATA   (DWORD)0x00000007
#define FASTLANE_CMD_VALIDATE_OPERATOR       (DWORD)0x00000008

#ifdef __cplusplus
extern "C"
{
#endif


extern BOOL     RAEXPORTS RAInitialize(PRACALLBACK pRACallback, 
                                       LPARAM      appParam);

extern BOOL     RAEXPORTS RATerminate();

extern RAHANDLE RAEXPORTS RAAddApproval(LPCTSTR desc, 
                                        DWORD   category, 
                                        DWORD   priority, 
                                        BOOL    bRemotelyApprovable, 
                                        int     numEventsApproved);

extern RAHANDLE RAEXPORTS RAAddVoidEvent(LPCTSTR desc, 
                                         long    amount);

extern RAHANDLE RAEXPORTS RAAddSecurityEvent(LPCTSTR desc, 
                                             int     conclusion);

extern RAHANDLE RAEXPORTS RAAddNegDeptEvent(LPCTSTR desc, 
                                            long    amount, 
                                            BOOL    bVoided, 
                                            BOOL    bHighRisk);

extern RAHANDLE RAEXPORTS RAAddAgeRestrictEvent(LPCTSTR desc, 
                                                int     minYears,
                                                int     year, 
                                                int     month, 
                                                int     day,
												bool	bDOBNotNeededBtn);

extern RAHANDLE RAEXPORTS RAAddSigCapEvent(LPCTSTR desc, 
                                           LPCTSTR  sigData, 
                                           DWORD   sigDataLen);

extern RAHANDLE RAEXPORTS RAAddOtherEvent(LPCTSTR desc, 
                                          DWORD   category);

extern RAHANDLE RAEXPORTS RAAddDataNeededEvent(LPCTSTR desc, 
                                               LPCTSTR view);

extern RAHANDLE RAEXPORTS RAAddReceiptLine(LPCTSTR line);

extern BOOL     RAEXPORTS RAUpdateApproval(RAHANDLE handle, 
                                           int      approvalState, 
                                           DWORD    dwCookie);

extern BOOL     RAEXPORTS RAClearApproval(RAHANDLE handle);

extern void     RAEXPORTS RAOperatorIsValid(LPCTSTR desc, 
                                            LPCTSTR id, 
                                            LPCTSTR password, 
                                            BOOL    bValid, 
                                            DWORD   dwCookie);

extern void     RAEXPORTS RASendClientMachineName(LPCTSTR clientname, 
                                            DWORD   dwCookie);

extern int      RAEXPORTS RAGetEventCounts(int   countType, 
                                           DWORD category);

extern RAHANDLE RAEXPORTS RAGetFirstEvent(int   eventType, 
                                          DWORD category);

extern RAHANDLE RAEXPORTS RAGetNextEvent(RAHANDLE hPrev, 
                                         int      eventType, 
                                         DWORD    category);


extern RAHANDLE RAEXPORTS RAGetFirstUniqueEvent(int   eventType, 
                                                DWORD category);

extern RAHANDLE RAEXPORTS RAGetNextUniqueEvent(RAHANDLE hPrev, 
                                               int      eventType, 
                                               DWORD    category);

extern BOOL     RAEXPORTS RAGetEventInfo(RAHANDLE     handle,    
                                         PRAEVENTINFO pInfo);

extern RAHANDLE RAEXPORTS RAGetFirstApproval(int   approvalType, 
                                             DWORD category);

extern RAHANDLE RAEXPORTS RAGetNextApproval(RAHANDLE hPrev, 
                                            int      approvalType, 
                                            DWORD    category);

extern BOOL     RAEXPORTS RAGetApprovalInfo(RAHANDLE     handle, 
                                            PRAEVENTINFO pInfo);

extern void     RAEXPORTS RASetConfiguredCategories(DWORD categories);

extern DWORD    RAEXPORTS RAGetConfiguredCategories();

extern void     RAEXPORTS RASetTotal(LPCTSTR total);

extern void     RAEXPORTS RASetLightState(int greenState, 
                                          int yellowState, 
                                          int redState);

extern void     RAEXPORTS RASetWLDBState(BOOL bConfigured, BOOL bOnline);

extern void     RAEXPORTS RABeginStoreMode();

extern void     RAEXPORTS RAEndStoreMode();

extern void     RAEXPORTS RACanSuspendNow(BOOL bWaitForApprove);

extern void     RAEXPORTS RABeginTransaction();

extern void     RAEXPORTS RAEndTransaction();

extern void     RAEXPORTS RASetTransactionState(int state);

#ifdef __cplusplus
}
#endif

#endif // _SCOTRADLL_H_
// CustomerTBConnectPOS.h: interface for the CCustomerTBConnectPOS class.
//
//
// CRD 541144 Name: CJ Estrada Date: Jan 7, 2021
// POS81632 Work Request:59313 Name:Matt Condra Date:December 10, 2013
// POS29719 WORK REQUEST:19114 Name:Matt Condra Date:April 20, 2012
// POS29349 Work Request:19114 Name:Matt Condra Date:March 8, 2012
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerTBConnectPOS_H__INCLUDED_)
#define AFX_CustomerTBConnectPOS_H__INCLUDED_

#include "SolutionTBConnectPOS.h"
#include "SingleInstance.h"

class CCustomerTBConnectPOS : public CSolutionTBConnectPOS,
                                public CSingleInstance<CCustomerTBConnectPOS>
{
    friend class CSingleInstance<CCustomerTBConnectPOS>;

public:
    virtual ~CCustomerTBConnectPOS();
    virtual TBRC LogErrorToServer(LPCTSTR szErrorText, const int nSeverity,
                        const int nComponentID, LPCTSTR szComponentID,
                        LPCTSTR szModuleID, const int nModuleLineNumber);
	
    BOOL SetSelectedItem(LPCTSTR szEntryID);
    
    // SendAuxMsgTLOGOverride parameters:
    // ---------------------------------
    // Manager:  ACS Manager ID
    // Override: ID of the override/intervention
    // Location: 1 = FastLane, 2 = HHRAP, 3 = RAP, 4 = ACS
    // WaitTime: Customer wait time from the time the assistance needed 
    //           was initiated until the override was completed, in seconds.
    // Terminal: The ID of the POS (default to the current terminal instance)
    // WaitTimeMs: SOTF 7242 The customer wait time from the time the assistance needed 
    //           was initiated until the override was completed, in milliseconds.
    BOOL SendAuxMsgTLOGOverride(CString csManager, long lOverride, long lLocation, long lWaitTime, long lTerminal = -1, long lWaitTimeMs = 0); 
	BOOL SendAuxMsgPreSelectedTender(long lTender);		// send preselected tender type to POS
	BOOL SendAuxMsgRemoteAssistMode(bool bRemoteAssistMode); //send entering/exiting remote assistmode to POS
	BOOL SendAuxMsgRobotCashFlow(CString csMediaReport); //SOTF 5572
    BOOL SendAuxMsgAgeVerifyData(int ageEntry, CString csCustomerDOB, CString csUPC, int ageRequired, int itemAllowed, int nCustomerAge); //SOTF 6226
	BOOL SendAuxMsgBroughtBag(bool bBroughtBag); // POS29349
	BOOL SendAuxMsgEOT(bool bEOT); // POS29719
	BOOL SendAuxMsgFLMS(CString csAuxMsg); // POS111525 POS111527 CRD 145555
	BOOL CCustomerTBConnectPOS::SendAuxMsgTransMode(int); // CRD126015
	BOOL SendAuxMsgNotifyExitStoreMode(CString nextState); 	//CRD366323
	BOOL SendAuxMsgNotifyCancelAlert();		//CRD 321191
	BOOL SendAuxMsgNotifyStoreModePrompt();	//CRD 338206
	
    BOOL SendAuxMsgTLOGRAPTrack(LPCTSTR szOperatorID, LPCTSTR szTimestamp, int nFunc, int nRAP, int nVRAP, int nOffline); // POS81632
	BOOL SendAuxMsgHHScannerActiveFlag(int hhScannerActive);
	BOOL SendAuxMsgCitiAccountDetails(CString csAuxMsg); // CRD 541144

    CString GetServerName(void);
    CString GetServerIP(void);
    CString GetErrorString(DWORD dwError);

    CString GetEmplidFromBadge(LPCTSTR szBadge); // SOTF 6312

	BOOL SendAuxMsgNotifySCOLoadConfirmation();

protected:
    bool    m_bServerInfoLoaded;
    CString m_csServerName;
    CString m_csServerIP;
    
    CCustomerTBConnectPOS();
    void LoadServerInfoFromLogs(void);

private:
    CCustomerTBConnectPOS(CCustomerTBConnectPOS &);                    // hide copy const
    CCustomerTBConnectPOS & operator =(const CCustomerTBConnectPOS &); // hide assignment
};

#endif 
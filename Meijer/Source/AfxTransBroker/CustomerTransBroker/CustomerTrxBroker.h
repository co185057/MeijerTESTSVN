//*************************************************************************************
//  CustomerTrxBroker.h :   Declaration file for the CCustomerTrxBroker class
//
//*************************************************************************************
// CHANGE HISTORY:
//
// POS11024 Work Request:16632 Name:Sandeep Shenoy Date:October 24, 2011
// POS6135 Work Request:15196 Name:Robert Susanto Date:April 28, 2011
// 
//////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerTRXBROKER_H__INCLUDED_)
#define AFX_CustomerTRXBROKER_H__INCLUDED_

#include "SolutionTrxBroker.h"
#include "SingleInstance.h"

class CCustomerTrxBroker : public CSolutionTrxBroker,
                            public CSingleInstance<CCustomerTrxBroker>
{
    friend class CSingleInstance<CCustomerTrxBroker>;

public:
    virtual ~CCustomerTrxBroker();
    
    virtual TBRC TBInitialize(HWND hWnd, LPCTSTR szFilePath);
    virtual TBRC TBUninitialize(void);
	virtual TBRC TBLaneClosed(const bool bEnter);
	virtual TBRC SendTBCommand(long lCommandID, va_list pArgs);
    LPCTSTR GetTBSTATEName(long key);
    long GetTBSTATEFromName(LPCTSTR szTBState);
    void TBSayPhrase(LPCTSTR szWaveFileName);
	virtual TBRC SetSelectedItem(LPCTSTR szEntryID);
	bool IsItemScrolled(){return m_bIsScrolled;}
	void IsItemScrolled(bool bScrolled) { m_bIsScrolled = bScrolled;}
    virtual TBRC GetNextState(LPCTSTR lpLastStateName, LPCTSTR lpNextStateName,
                              bool *pbOverride, LPTSTR *lpOverrideStateName, CDescriptorList &dlParameterList);
	long SetFLState( LPCTSTR szState );
	virtual TBRC TBGetCurrentBusinessDate(CString &csBusinessDate);
    virtual TBRC ReleaseMessageLParam(PSMessageElement psMessage);

    bool LaunchAppAndWait(LPCTSTR szBatchFile, DWORD dwWaitTime, bool bKillIfHung);
    bool InAssistMenu(void)
    {
        return m_bInAssistMenu;
    }

    /* Meijer POS6135 */
	virtual TBRC TBLoadOptions(void);
   static int nIndex;                  //POS11024

protected:
    CCustomerTrxBroker();
	// Perform All the operations that we need before we launch POS Application.
	//	For example, print all spooled receipts
	void PrePOSStartUp();

    friend DWORD WINAPI SigBitmapMonitorProc(LPVOID pvParam);
    DWORD SigBitmapMonitor(void);
    void TerminateSigBitmapMonitor(DWORD dwWaitTimeout);
    CString GetBuildFromHistory(void);
    
    /* Meijer POS6135 */
	bool LoadACSOptions();

private:
	bool m_bIsScrolled;
    CCustomerTrxBroker(const CCustomerTrxBroker &);              // hide copy const
    CCustomerTrxBroker & operator =(const CCustomerTrxBroker &); // hide assignment
	static bool m_bPrePOSStartUp;
	static bool m_bLaunchedPOS;
	HANDLE m_hSigBmpMonStopSignal;
	HANDLE m_hSigBmpMonThread;
    bool m_bInAssistMenu;


};

#endif 

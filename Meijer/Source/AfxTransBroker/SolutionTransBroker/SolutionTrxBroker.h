//*************************************************************************************
//  SolutionTrxBroker.h :   Declaration file for the CSolutionTrxBroker class
//
//*************************************************************************************

#if !defined(AFX_SolutionTRXBROKER_H__INCLUDED_)
#define AFX_SolutionTRXBROKER_H__INCLUDED_

#include "BaseTrxBroker.h"
#include "SingleInstance.h"

class CSolutionTrxBroker : public CBaseTrxBroker
{
public:

    virtual ~CSolutionTrxBroker();
    
    virtual TBRC TBInitialize(HWND hWnd, LPCTSTR szFilePath);
    virtual TBRC TBUninitialize(void);
	virtual TBRC TBLaneClosed(const bool bEnter);
	virtual TBRC SendKeyedData(va_list pArgs);
    virtual TBRC GetDisplayXML(va_list pArgs);
	// virtual TBRC SetSelectedItem(va_list pArgs); -- Deprecated. Use SetSelectedItem(LPCTSTR szEntryID instead
    virtual TBRC SetSelectedItem(LPCTSTR csEntryID);
	virtual TBRC TBFinish();
    virtual TBRC PreprocessSignatureData(LPCTSTR lpFileName,BSTR bstrSignatureData, LPCTSTR lpReceiptType, BOOL* bProcessData);
    virtual LPCTSTR GetFLNextStateName(void);

	virtual TBRC SendTBCommand(long lCommandID, va_list pArgs);

    virtual TBRC GetNextState(LPCTSTR lpLastStateName, LPCTSTR lpNextStateName,
                              bool *pbOverride, LPTSTR *lpOverrideStateName, CDescriptorList &dlParameterList);
    virtual bool  InRemoteAssistMode( void);
    virtual void SetInRemoteAssistMode( bool);
    virtual bool IsLimitedAssistMode();
    virtual void SetLimitedAssistMode( bool bVal );
    virtual void TBSayPhrase(LPCTSTR);
    virtual long GetTBSTATEFromName(LPCTSTR szTBState);
    virtual LPCTSTR GetTBSTATEName(long key);
    virtual bool IsTenderInProgress(void)
        {
            return m_bTenderInProgress;
        }
    virtual void SetTenderInProgress(bool f)
        {
            m_bTenderInProgress = f;
        }
    virtual bool AnyRewardItems(void)
        {
            return m_bRewardItem;
        }
    virtual void SetAnyRewardItems(bool f)
        {
            m_bRewardItem = f;
        }
    virtual TBRC SendScanData(va_list pArgs);

    // Support for check tendering with second printer at FL (FL 4.0 Patch F+)
    virtual bool IsCheckPaymentAtLaneAvailable();
    virtual void SetCheckPaymentAtLaneAvailable(bool bAvailable);
	virtual BOOL TBValidateHeadCashier( LPCTSTR szCashierID, LPCTSTR szCashierPw );

protected:
    CSolutionTrxBroker();
    CString m_csFLNextState;
    COleMessageFilter m_oleMsgFilter;
    
private:
    CSolutionTrxBroker(const CSolutionTrxBroker &);              // hide copy const
    CSolutionTrxBroker & operator =(const CSolutionTrxBroker &); // hide assignment

	static bool bLaunchedPOS;
    bool m_bRemoteAssist;
	bool m_bLimitedAssistMode;
    bool m_bTenderInProgress;
    bool m_bRewardItem;
    bool m_bCheckPaymentAtLaneAvailable;
};

#endif 


//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAudit.h
//
// TITLE: Class header for store mode Audit state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMAuditSTATE
#define _SMAuditSTATE

#include "SMSmDataEntry.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
    SMSmMobileAudit : public SMSmDataEntry
//  SMSmMobileAudit : public SMSmAssistMenu
{
public:
    SMSmMobileAudit();
    
    virtual SMStateBase  *Initialize();
    virtual void		 UnInitialize();
    virtual SMStateBase  *Parse(MessageElement *me);
protected:
    virtual SMStateBase *PSParse( const MessageElement* );	// same parse for all
    virtual void PSParseFinalProcessing(const MessageElement* me, SMStateBase* pRetState);
    virtual SMStateBase  *ParseMobile(MessageElement *me);
    virtual SMStateBase *OnMobileTransactionStart(CString csTransactionInfo);

    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton4(void);
    virtual SMStateBase  *PSButton5(void);
    virtual SMStateBase  *PSButton6(void);
    virtual SMStateBase  *PSButton7(void);
    virtual SMStateBase  *PSButton8(void);
    
    virtual SMStateBase  *PSCmdButton(CString csContext, CString csControl, CString csCommand); //RFQ 986
    virtual SMStateBase  *OnRAPUnsolicitedDataReceived(LPCTSTR sData);
    
    virtual SMStateBase  *SMSmMobileAudit::HandleDataNeeded();
    virtual SMStateBase  *DMScanner(void);
    virtual SMStateBase  *DMScannerEncrypted(void);

    virtual SMStateBase  *TimedOut(void);

    void RetrieveDataNeededDefinition(CString csDescription, CString csView, CString csINI, CString csSection);
	virtual void SetUserButton(int nIndex, int nPageOffset);
    void AddAuditItemToReceipt();
    bool IsValidMobileTransaction(CString csTransactionInfo);
    void PaintScanAndBagScreen();
    void UpdateTicketInfo();
    void UpdateRescanInfo();
    void LockButtons();
    void UnlockButtons();
	CString CSUnEncryptBarcode(CString CSBarcode);


    bool m_bWaitForResponse;
    int m_nResponseTimer;
    bool m_bRAPScannerAllowed;
    bool m_bRAPScannerAvailable;
    bool m_bRAPScannerConnected;
	
	bool m_bWaitingForStatusToConnect;  //MEIJER RAP Scanner

	CString ConvertToUPCA(CString csUPCEBarcode);
	CString calcCheckDigit(CString csMsg);
	int isEven(int i);
    CPSXUserInputLock* pUserInputLock;
    //bool m_bAbortingAudit;

	//Start CRD 186331
	CString m_bInvalidBarcodeTypePrefix;
	SMStateBase *RetrieveInvalidBarcode();
	void populateInvalidBarcodeTypePrefix();
	bool isScannerDataInvalid(CString csScannerData);
	CArray <CString, CString> csInvalidBarcodeTypePrefixList;
	//End CRD 186331
       

  DECLARE_DYNCREATE(SMSmMobileAudit)// MFC Runtime class/object information
};

#endif
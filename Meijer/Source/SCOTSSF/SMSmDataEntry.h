//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmDataEntry.h
//
// TITLE: Class header for store mode Restricted Items state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMDataEntrySTATE
#define _SMDataEntrySTATE

#include "SMSmDataEntryBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmDataEntry : public SMSmDataEntryBase
{
public:
    SMSmDataEntry();                      // constructor
    virtual SMStateBase * Initialize(void);
    virtual void UnInitialize(void);
    virtual bool PSRemoteLegalForState();
    virtual bool storeMode();

	virtual SMStateBase * TBItemDetails(void);
protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton4(void);
    virtual SMStateBase  *PSButton5(void);
    virtual SMStateBase  *PSButton6(void);
    virtual SMStateBase  *PSButton7(void);
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase  *PSNumericKey(const long); // a key 0-9 was pressed
    virtual SMStateBase  *PSEnterKey(void);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *PSDecimalKey(const long ch);
    virtual SMStateBase  *PSBackSpaceKey(void);
    virtual SMStateBase  *PSSpaceKey(void);
    virtual SMStateBase  *PSShiftKey(void);
    virtual SMStateBase  *PSAlphaKey(const long lKey);
    virtual SMStateBase  *PSAlphaNumericKey(const long lKey);
    virtual SMStateBase  *DMScanner();
    virtual SMStateBase  *DMScannerEncrypted();
    virtual SMStateBase  *DMCardReader(void);
    virtual SMStateBase  *OnTBNeedData(void);
    virtual SMStateBase  *HandleRAPData(void);
    virtual SMStateBase *PSReceiptUp(void);   
    virtual SMStateBase *PSReceiptDown(void);
    virtual void DisplayUserButtons(void);
    virtual void SetUserButton(int nIndex, int nPageOffset);
    virtual CString GetPagedButtonCommand(int index);
    virtual CString GetButtonCommand(int nButtonIndex);
    virtual SMStateBase *PSOnRemoteDisconnected(const long lParm); // SSCOI-31860
    virtual SMStateBase *PSCmdButton(CString, CString, CString);
    virtual SMStateBase *OnRAPUnsolicitedDataReceived(LPCTSTR sData);

	//POS11024 Mobile Shopper START
	virtual SMStateBase  *TBParse(MessageElement *);
	virtual void TBShowDisplay(bool bRefresh = false, bool bRepaintLastScreen = false);
	virtual CString  GetDisplayText(CString csFieldName);
	virtual _variant_t  GetFieldValue(CString csFieldName);
	MSXML2::IXMLDOMDocumentPtr m_POSDisplay;
    //POS11024 Mobile Shopper END
	//Start CRD 321191
	virtual SMStateBase  *stateAfterPasswordVerify();
	virtual SMStateBase  *PSButtonGoBack(void);
	//End CRD 321191
private:
	void PrintEndorsement(void);

	DECLARE_DYNCREATE(SMSmDataEntry)// MFC Runtime class/object information
};

#endif

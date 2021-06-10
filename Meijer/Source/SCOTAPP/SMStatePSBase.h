//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStatePS.h
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSTATEPSBASE
#define _SMSTATEPSBASE

#ifndef _CPPUNIT
// cm150000: Merge 2
public:
	virtual bool PSRemoteLegalForState();
	virtual bool PSAssistMenuLegalForState();
//cm150000: End Merge 2

protected:
    static PSEVT  nPSLastEvt;         // last PS message type
    static int    nPSLastParm;        // last PS message parm
	static int	  nPSLastWParam;

	
//private:
  virtual SMStateBase *PSParse( const MessageElement* );	// same parse for all

#endif //_CPPUNIT
protected:
  virtual long PSInitialize(void);
#ifndef _CPPUNIT
  virtual long PSUnInitialize(void);
  // all PS funcs are only 'protected; because many will be overridden
  // in the derived state
  virtual SMStateBase *PSButtonHelp(void);
  virtual SMStateBase *PSButtonGoBack(void);
#endif //_CPPUNIT
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSButton2(void);
  virtual SMStateBase *PSButton3(void);
  virtual SMStateBase *PSButton4(void);
  virtual SMStateBase *PSButton5(void);
  virtual SMStateBase *PSButton6(void);
  virtual SMStateBase *PSButton7(void);
  virtual SMStateBase *PSButton8(void);
  virtual SMStateBase *PSMenuKey1(void);
  virtual SMStateBase *PSMenuKey2(void);
  virtual SMStateBase *PSMenuKey3(void);
  virtual SMStateBase *PSMenuKey4(void);
  virtual SMStateBase *PSMenuKey5(void);
  virtual SMStateBase *PSMenuKey6(void);
  virtual SMStateBase *PSMenuKey7(void);
  virtual SMStateBase *PSMenuKey8(void);
  virtual SMStateBase *PSMenuKey9(void);
  virtual SMStateBase *PSMenuKey10(void);
  virtual SMStateBase *PSMenuKey11(void);
  virtual SMStateBase *PSMenuKey12(void);
  virtual SMStateBase *PSMenuKey13(void);
  virtual SMStateBase *PSMenuKey14(void);
  virtual SMStateBase *PSMenuKey15(void);
#ifndef _CPPUNIT
  virtual SMStateBase *PSAssistKey(const long);
#endif //_CPPUNIT
  virtual SMStateBase *PSCmdButton(CString, CString, CString);  //RFQ 986
  virtual SMStateBase *PSCmdList(CString, CString, const long);
#ifndef _CPPUNIT
  virtual SMStateBase *PSMainMenuKey(void);   // AM mode main menu button - TAR# 123183
  virtual SMStateBase *PSHelpTopics(void);
// NewUI
  virtual SMStateBase *PSSwitchContext(BOOL, CString); //RFQ 986
  virtual SMStateBase *PSListLookup(CString);
  virtual SMStateBase *PSListLookupTab(LPCTSTR);
  virtual SMStateBase *PSListScroll(const long);
#endif //_CPPUNIT
  virtual SMStateBase *PSConnectRemote(CString&,CString&);
#ifndef _CPPUNIT
  virtual SMStateBase *PSAlphaNumericKey(const long);	// ASCII key

// NewUI
  virtual SMStateBase *PSReceiptUp(void);
  virtual SMStateBase *PSReceiptDown(void);
  virtual SMStateBase *PSVideoDone(void);
  virtual SMStateBase *PSNumericKey(const long);	// ASCII key
  virtual SMStateBase *PSBackSpaceKey(void);
  virtual SMStateBase *PSShiftKey(void);
  virtual SMStateBase *PSDecimalKey(const long);	// decimal or period key
  virtual SMStateBase *PSOtherSpecialKeys(const long);
  virtual SMStateBase *PSEnterKey(void);
  virtual SMStateBase *PSCancelKey(void);
  virtual SMStateBase *PSClearKey(void);
  virtual SMStateBase *PSSpaceKey(void);
  virtual SMStateBase *PSAssistAccepted(void);			
  virtual SMStateBase *PSAssistDenied(void);
  virtual SMStateBase *PSKeystroke(const long, const long);
  virtual SMStateBase *PSHotKey(CString &);
  virtual SMStateBase *PSCharKey(TCHAR c);
  virtual SMStateBase *PSAltGrKey(void);
  virtual SMStateBase *PSSymbolKey(void);

  virtual TCHAR VirtKeyToTChar(UINT vKey, long shiftState);
  virtual long PSEchoInput(const long);
  virtual long PSEchoDecimalInput(const long);
#endif //_CPPUNIT
  virtual long PSClearInput(void);
  virtual long PSBackSpaceInput(void);
#ifndef _CPPUNIT
  virtual SMStateBase *PSClearAllInput(BOOL bSayValid = TRUE);
  virtual SMStateBase *PSAlphaKey(const long);	// ASCII alpha key
  //Begin_Suspend
  virtual void VoidAllCoupons();    //Before suspend transaction, void all coupons
	//End_Suspend
  
  virtual long SetButton(LPCTSTR szControlName,       // show a button   TAR 218915
			  	         LPCTSTR     szContent,  // button text
				         const bool fEnable);    // T=Enabled, F=Disa
  //Tar 143309
  static CStringArray csPickQtyList; 
#endif //_CPPUNIT
  virtual void PSSortQtyList(CStringArray &);
#ifndef _CPPUNIT
  virtual bool VerifyQtyItem(CString);
  virtual bool SCOTIsMinimized();
  virtual void SetFocusToVideo();  // TAR 154963 - Needed for maximize/min. video from switch app.
  virtual SMStateBase *PSOnRemoteDisconnected(const long lParm);
  virtual SMStateBase *PSButtonToggle(void);
  virtual SMStateBase *PSButtonLaneStatus(long lLaneIndex);
  virtual SMStateBase *OnExternalUIRequest(MessageElement* me);
  virtual bool FindItemID(CString& csGetItemID, CString csReceipt, CString csReceiptSrc, long lExtPrice, long lUnitPrice, CString csItemCode);
  virtual void TrimSymbology(CString& csItemCode); //tar 241704
  virtual SMStateBase *HandleNewContext();  // Rel 3.2 Requirement 4.5.1 - Single Mode RAP
  virtual SMStateBase* PreNotifySCOTEvent2TB(const MessageElement *me, sExtStateEventNotifyIn& sNotifyIn, sExtStateEventNotifyOut& sNotifyOut);
  virtual SMStateBase* FillPSEvent2NotifyTB(const MessageElement* pEvent, sExtStateEventNotifyIn& sNotifyIn, sExtStateEventNotifyOut& sNotifyOut);
  virtual void SetAlphaShiftState(CString csContext, bool bShiftOn);
  virtual void BuildCmdButtonList(CSCOTDataEntry& cDataEntry, CString csCtxName, CString csCtlName);
  virtual void BuildCmdList(CSCOTDataEntry& cDataEntry, const CString& csReceiptSrc);
#endif //_CPPUNIT
  virtual void PSParseFinalProcessing(const MessageElement* me, SMStateBase* pRetState);
  virtual void PSSendConfigOptionsToNextGenUI(void);
  virtual bool ShouldDiscardEvent(const PSInfo &psInfo) const; //SSCOADK-5341
	
#endif


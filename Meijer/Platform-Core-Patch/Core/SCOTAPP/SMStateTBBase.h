//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateTBBase.h
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SMSTATETBBASE
#define _SMSTATETBBASE

#define KEY_PROC1	   101		 // start of procedures 1-19
#define KEY_PROC19	  119		 // end of procedures 1-19
#define KEY_LOOKUP1	 200		 // start of lookup keys 200-239
#define KEY_LOOKUP40	239		 // end of lookup keys 200-239

//dp185016 tar 287258
#define OUTOFSYNCTEMPORARYPARAMETER _T("%APP_DRIVE%\\scot\\bin\\getdiagfiles.exe -f ") SCOTCONFIG _T("\\outofsyncdiagfile.ini")

public:
  static TBSTATE  nTBLastRealState;	// last nTBLastState that was not TS_IGNORE, TS_UKSE, etc.
  // NewUI
  static USHORT uKeyCodeList[200];					  // key scan codes
  static CString csKeyLabeList[200];				  // key labels
  // NewUI
  virtual void DoEvents();		// process all events on the message queue
  virtual long TBGetNextState(LPCTSTR lpLastStateName, 
                               LPCTSTR lpNextStateName, 
                               bool *pbOverride, 
                               LPTSTR *lpOverrideStateName, 
                               CDescriptorList &dlParameterList);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual TBRC SendTBCommand(  long lCommandID, ...);

protected:
  static int nTenderButtonIndex;   // from 1 - MAXTENDERNUM  //New Finalization
  static int nSavedTenderButtonIndex;   //TAR259370  For SigCap Options after Endorsement
  static TBSTATE  nTBLastState;			// last TB state from the display
  //static bool fPIPShowCardProcessingScreen;    // PIP, show Card Processing screen or PINPAD Entry screen after tendering

  static CString csOverrideCode;   // The Override code
  virtual  void	ProcessTBReturn(long rc);	   // Process TB return errors
  static bool m_bWaitForCashDrawerClose;    //RAP Dispense Change

 // bool TenderRequireSignature(void);  //Check for signature require or not
//private:
  virtual SMStateBase *TBParse(MessageElement* );	// same parse for all
  virtual SMStateBase *TBItemDetails(void); //get item details.
  virtual SMStateBase *TBGetTotalDetails(void); //get total details.
  virtual SMStateBase *TBParseFoodStampTotal(void); //get food stamp total
 // virtual SMStateBase *TBGetTenderDetails(void); //get tender details
  virtual void TBGetItemDetails(ItemDetails&);
  void TBCreateReportEntry(void);
  virtual void UpdateTotal(void);  //Update the total before sending to screen
  void TBGetItemExtendedDetails(ItemExtendedDetails&);
  virtual SMStateBase *TBBalanceDetails(void);
  //virtual SMStateBase *TBChangeDetails(void);
  virtual SMStateBase *TBGetTenderDetails(void);
  virtual SMStateBase *TBGetAvailableFoodStampAmount(void);
  virtual SMStateBase *TBGetAvailableBenefitAmount(void);
  virtual SMStateBase *TBRequestFoodStampEligibleTotal(void);

protected:
  virtual void TBShow2x20(void);	// allow some states to override
  virtual void TBShow2x20(CString csOpDisplay);

protected:
  virtual SMStateBase *TBStart(LPCTSTR = NULL);
  virtual SMStateBase *TBItemSale(			  // common point to start an item sale
							const bool, // ...if to void T/F
							const long,	// ...tare code selected 0,1,2,...
							const bool, // ...scanned item
							const bool = false, // Is Pick List Item
                            const long = ITEMUPDATE_NONE); //Type of update
  virtual SMStateBase *TBEnterItem(const bool=false, int lUpdateFields = ITEMUPDATE_NONE);	// item is in ItemObject
  virtual SMStateBase *TBEnterTender(
							const TBTENDERTYPE,		// type
							const long=0,		// amount +ve or -ve
							const bool=false);	// override allowed
  virtual SMStateBase *TBCancelTransaction(const bool=false);
  virtual SMStateBase *TBSuspendTransaction(const bool = false);
  virtual SMStateBase *TBIdleTimer(void);
  virtual SMStateBase *TBEndOfTransaction(void); // EOT processing
  virtual SMStateBase *TBFinish(void);
  SMStateBase *TBOverride(void);
  virtual TBRC TBLogErrorToServer(LPCTSTR szErrorText, const int nSeverity,
						   const int nComponentID, LPCTSTR szComponentID,
						   LPCTSTR szModuleID, const int nModuleLineNumber);
  virtual void TBEnterTenderState(const bool bEnter); // Enter or Leave Enter Tender state
  virtual void TBSendSigData(LPCTSTR CustomerName, LPCTSTR cSigData, DWORD nLength);
  virtual SMStateBase* TBGetMessageInfo(const bool bDisableSA = false);  // TAR 127247
  virtual void TBPrcessPIPTenderType(TBTENDERTYPE&);
  virtual void TenderComplete(void); //RFC 262343
  virtual SMStateBase * HandleVoidedTender(void);  //RFC 275377 Support for Voiding Tender

protected:
  virtual CString TBName(void);

  virtual TBRC  TBGetPINWorkingKey(CString &csAct,
								                   CString &csKey,
								                   TBTENDERTYPE nTyp,
                                   TBPINBLOCKFORMAT& nPinBlockFormat,
                                   TBPINPADSYSTEM& nPinpadSystem,
                                   TBPINENCODING& nPinBlockEncoding);

  virtual bool TBPromptForDebitAccountType(void);
  virtual long TBRefreshState(void);
  virtual long TBInitialize(HWND hwnd, LPCTSTR szFilePath);
  virtual long TBLoadOptions(void);
  virtual long TBUnInitialize(void);
  virtual void TBLoadAssistModeKeyboard(void);
  virtual USHORT *TBEmulatorKeys(void);
  virtual long TBGetKeyLabel(const int, CString &);
  virtual USHORT TBGetKeyCode(const int nKey); // 1.2.3,,,
  virtual CString TBLookupCode(const USHORT uKey);
  CString SmktLkUp[40];					      // 40 lookup key itemcodes
  virtual long TBGetTerminalNumber(void);	  // returns this SCOT terminal number
  virtual long TBGetCurrentTransactionNumber(void);	// returns current trx number
  virtual long TBGetCurrentBusinessDate(CString &csBusinessDate);	// returns current Business Date
  virtual BOOL TBValidateOperator(LPCTSTR,LPCTSTR);	// validate operator and pwd

    

  virtual HFILE TBGetSlipData(void);
  virtual HFILE TBGetReceiptData(void);
  virtual HFILE TBGetJournalData(void);
  virtual CString TBGetDisplay(void);               // internal call

  virtual void TBReleaseSlipData(HFILE);
  virtual void TBReleaseReceiptData(HFILE);
  virtual void TBReleaseJournalData(HFILE);
  virtual void TBLaneClosed(const bool bEnter); // Enter or Leave lane Closed state
  virtual TBSTATE TBProcessMessage(MessageElement* me);
  virtual SMStateBase *TBSetTrainingModeState(const bool bTMState);   // entering or leaving training mode
  virtual long TBSetBusinessDate(LPCTSTR szSetBusinessDate);
  static bool bUpdateFoodStampTotal; 
  void SetItemDetails(SItemDetails* dItem, ItemDetails &iDetails);
  virtual void TBEPSEndOfDay(LPVOID lp=0);
  virtual void TBEPSStartOfDay(LPVOID lp=0);
  static bool bEndOfDay;
  virtual void TBNeedStartOfDay(TBSTARTOFDAYSTATE &eState);

  virtual void ResetApprovalValues(const bool=false); //awk.2.5.2 TAR191836

   virtual SMStateBase *OnEndorseDocument(void);
   virtual SMStateBase *OnNewCashDrawerState(void);
   virtual SMStateBase *OnNeedMICR(void);
   virtual SMStateBase *OnNewSecurityLevel(void);
   void TBEndorsementCompleted(bool bSuppressSubsequent = false);
   virtual SMStateBase *OnAssistModeStatusChanged(void);
   virtual SMStateBase *OnTBNeedData(void);
   virtual long TBLogReceipt(LPTSTR*,BSTR *, LPTSTR*);
   virtual long TBPreprocessSignatureData(LPCTSTR,BSTR, LPCTSTR,BOOL*);
   virtual long TBGetGiftCardBalance(long *plBalance);

   static bool m_bCouldBeScannedLoyaltyCard;

   //+dp185016 POS - TB out of sync
   virtual SMStateBase *HandleOutOfSyncStop();
   virtual SMStateBase *HandleOutOfSyncAssist();
   virtual void TBSyncTBorPOS();
   virtual SMStateBase *getRealStatePointer(SMStateBase *pState=NULL); //307389
    //+SR697 gc185038
   virtual bool AllowImmediateReceiptPrint(); 
   virtual void EndOfTransactionPrint();
   //-SR697 gc185038
public:
   static int m_nTBOutOfSyncError; // 0 if no error, 1 outofsync_stop
   //-dp185016
   virtual BOOL TBValidateHeadCashier(LPCTSTR szCashierID, LPCTSTR szCashierPW); //dp185016 support glory device
   virtual void ProcessPendingActions(); //SR697 gc185038
public:
  static bool g_bTBCaptureDeclinedCard;
  static bool g_bTBNeedMoreData; // TAR 238061 
  static bool m_fVoidedInAssist; // TAR246858
  static bool fTenderDeclined;			  // If tenderdeclined
  static bool m_bTBNeedAssistModeGetDiag;  //RFC #282631
  static bool m_bCancellingCashTender; //RFC 275377 if Void Tender
  static TraceObject Out_of_sync_Trace_Object; //dp185016 tar 286968
  //+SR697 gc185038
  static bool m_bEOTPrintPending;
  static bool m_bForceReceiptPrint;
  //-SR697 gc185038

#endif

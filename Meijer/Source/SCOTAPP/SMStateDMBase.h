//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateDMBase.h
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////



/******cm150000: Added In Merged Code******/
#ifndef _SMSTATEDMBASE
#define _SMSTATEDMBASE
/******cm150000: End Merged Code***********/

//Error Handling
#define DMCLASS_ANY          -1
#define MODEL_ANY            _T("")
#define EVENTID_ANY          -99 //SR672
#define EXT_RESULT_ANY       -1
#define EXTENDED_RESULT_ANY   -99 //SR672
#define CONTEXT_ANY          -1
#define EXTRACT_CLASSNAME(pSCOTTempState) (CString(pSCOTTempState->GetRuntimeClass()->m_lpszClassName))
#define _BILLRECYCLER _T("BILLRECYCLER")
#define _COINRECYCLER _T("COINRECYCLER")
#define CASHACCEPTOR	_T("CashAcceptor=")
#define CASHDISPENSER	_T("CashDispenser=")
#define COINACCEPTOR	_T("CoinAcceptor=")
#define COINDISPENSER	_T("CoinDispenser=") 

#define SCOTEVT_DATA_SEP	_T(',')
// append ",(b,c)" to a
// ID_FS is used as field separator
#define ADD_SCOTEVENT_DATA(a, b, c)  {\
									   CString csTemp; \
									   csTemp.Format(_T(",(%s%c%s)"), b, SCOTEVT_DATA_SEP, c);\
									   a += csTemp;\
									 }


// append "(b,c)" to a
// ID_FS is used as field separator
#define ADD_SCOTEVENT_DATA_FIRST(a, b, c)  {\
									         CString csTemp; \
									         csTemp.Format(_T("(%s%c%s)"), b, SCOTEVT_DATA_SEP, c);\
									         a += csTemp;\
									       }
									  
public:
#ifndef _CPPUNIT
  virtual bool ReceiptPrintSuccessful();
  virtual void ResetMLBCValues(void); //RFC 241710 (Needs to be called by State Engine in MPProcedures
  virtual CString GetPrinterCharSet();		//TAR378451
  virtual void SendNewEscSeq2Printer();		//TAR378451
  virtual void SendOrigEscSeq2Printer();	//TAR378451
#endif //CPPUNIT
  static bool fRecyclerNotReady; //dp185016 Recycler Platform Issue #12. wait for a specific time for recycler to be ok.
                                 //if recycler is still not ready after n, then set this to true.
                                 //set by CCashRecyclerWrapper class.
  static CString csBalance;
  static CString csLoan;
  static CString csPickUp;
#ifndef _CPPUNIT
  virtual long DMConfigureScaleDisplay(void);
  virtual void DMConfigureScaleParams(long &divsize, long &constant);
  virtual CString ConvertPrinterString(CString) const; //SSCOP-137 (migrate fix of TAR459459)
  virtual CString FormatDeviceInfoInput(long lErrorCode);
#endif //CPPUNIT
protected:
   static int    nDMLastMsg;         // last DM message type
   static int    nDMLastDev;         // last DM device
   static long   lDMLastCode;        // last DM message code
   static long   lastDMReceiptCode;  // save last receipt printer status
   static long   lastDMJournalCode;  // save last journal printer status
	 /******cm150000: Added In Merged Code******/
	 static long   sm_lastDMSlipCode;	 // save last slip printer status
	 /******cm150000: End Merged Code***********/

   static long   lastDMCashAcceptorCode;  // save last cash acceptor status
   static long   lastDMCoinAcceptorCode;  // save last coin acceptor status
   static int    nDMLastLength;      // last DM message data length
	/*
   static int    nDMMaxLength;       // max DM data length ever received
   */
   static int    nDMLastAcceptor;    // last DM cash amt inserted in cents

   static int    nDMLastCoinEvent;  
   static int    nDMLastBillEvent;  
   static int    nDMCashInDrawer;    // total cash accepted by smkt

   static bool   fDMErrorCashDue;    // cash to be returned after dmacceptor filled  Tar 214729 RJF 101002 
   static int    nDMCashInserted;    // cash inserted, not yet tendered to smkt
   static int    nDMCashTendered;	 // cash tendered, not yet accepted by smkt
   // not yet reported to supermarket
   static int    nDMLastTrackLen;    // last DM MSR track length
   static bool fCancelPayment;
	
   static LPTSTR cDMLastTrack;      // last DM MSR track data, only 1 track
   static LPTSTR cDMLastData;       // last DM data buffer
   static long   lDMScaleWeight;     // last DM scale weight or -ve if broken
   static bool   fDMScaleChanged;    // set upon unstable weight
	
   static CString csDMLastBarCode;   // last DM bar code scanned

   // BeginCodeLocaleUPC
   static CString csDMLastBarLabel;  // last DM bar code label scanned (header removed)
   static long    csDMLastBarType;	 // last DM bar code type
   static BOOL    m_bLastScanDataWasMultiLabel;  //last DM bar code scanned was part of a multi-label barcode
   static BOOL    m_bLastMntScanDataWasMultiLabel;  //last DM bar code scanned was part of a multi-label barcode
   static int     m_nNumberOfPartsReturnedSoFar; //The number of parts of a multi-label barcode that we have saved so far
   static int     m_nTotalNumberOfMLBParts;      //The total number of parts of a multi-label barcode as configured by scotopts
#ifndef _CPPUNIT
   static CList   <MultiLabelBarcodeFormatRecord, MultiLabelBarcodeFormatRecord&> m_MLBarcodeList;
#endif //_CPPUNIT
   static CArray   <CString, CString> m_MntMLBarcodeArray;
   static PSMultiLabelBarCodeDetails m_psMLBarCodeDetails;
   // EndCodeLocaleUPC
   static CString csCardAccountNum;  // Account Number from MSR
	 	 
   static CString csDMEncryptedPin;  // last DM Pin from encryptor
   static bool    fDMReceiptError;
   static bool    fDMJournalError;
   static bool    fDMScaleZeroed;	 // scale had been zeroed T/F
   static bool	  bgCoinDispenserIsEmpty;  // True when DM send CC_STATUS_EMPTY
	
   static CString csDateCoinDispenserRefilled;  // TAR 140515
   static CString csTimeCoinDispenserRefilled;  // TAR 140515
   static int nDeviceErrorContext;

   static int m_nCoinDispenserWithLowSensor;   //0- without enhanced media low sensor //tar 246539
#ifndef _CPPUNIT
											   //1- with enhanced media low sensor
   //SR672
   virtual CString DMGetDeviceErrorHTML(int DeviceClass, CString& sDeviceModel, CString& sDeviceSubModel,
					int StatusEventId, long ExtendedResultCode, int nDataLength, void* sData, bool* bCheckHealth = NULL, int nContext = -1, CString sLCID = _T(""));

   //2- Device is not ready to check if there is an enhanced media low sensor

   virtual bool getNextHopperStatusLine(CString& currencyMap,
                                       CString& hopperStatusList,
                                       CString& hopperStatusLine, bool bDenominationOnly=false);
   virtual void getHopperStatus(const CString denominationCode,
                               CString& denominationStatus);
   virtual CString getHopperStatusString(void);

   virtual CString getHopperDenominations(long& lErrorCode);
   virtual bool isOnlyHopperSubsApprovalRequired(void);

   virtual SMStateBase *DMParse( const MessageElement* );	// same parse for all
   virtual SMStateBase *DMSetScaleWeight(bool &);
   
   // Glory cash recycler.  Notify that currency amount entered is >= amount due.
   // Return false to disable default base behavior.  Return true otherwise.
   virtual bool DMHookNotifyTenderOver(void);

   // Glory cash recycler.  Return true if currency should be rejected.
   virtual bool DMCashRecyclerShouldReject(const CString &csStateName);
   // Glory cash recycler.  Return true if inserted currency > amount due.
   // For Japan large change requirement.
   virtual bool DMCashRecyclerIsTenderOver(const CString &csStateName);
   virtual bool DMUnlockNoteCover(void);
#endif //CPPUNIT

   static bool m_bIsCashDrawerClosed;   // RAP Dispense Change Yes - Close, No - Open, default is Y
   static long m_nDMSavedDev;             //277025 Save the device class in last device error message
   static long nDMMaximumWeight;	// Scanner/scale maximum weight
   static long nDMMaximumWeightTol;	// Tolerance (number of divisions) above maximum weight that the...
									// ...scale display will blank. Default value = 9 * d (9 divisions).
protected:
#ifndef _CPPUNIT
  virtual long DMUnInitialize(void);
  virtual long DMInitialize(void);
  virtual CString DMGetDevicesInError(void);
  // handlers for DM device data
  virtual SMStateBase *DMScanner(void);
  virtual SMStateBase *DMScannerEncrypted(void); //SR93.4
  virtual SMStateBase *DMReceiptPrinter(DWORD dwEvent);
  virtual SMStateBase *DMJournalPrinter(DWORD dwEvent);
  virtual SMStateBase *DMSlipPrinter(DWORD dwEvent);
  virtual SMStateBase *DMMICR(DWORD dwEvent);
  virtual SMStateBase *DMKeyLock(DWORD dwEvent);
  virtual SMStateBase *DMCashDrawer(DWORD dwEvent);
  virtual SMStateBase *DMAcceptor(void);
  virtual SMStateBase *DMCardReader(void);
  virtual SMStateBase *DMCardRemoved(void);
  virtual SMStateBase *DMCashRemoved(void);
  virtual SMStateBase *DMDispenserPositionEvent( long lStat); //4.2 - Card Dispenser
   virtual void DMSlipPrinterPrint(LPCTSTR szPrintData){dm.PrintLine(DMCLASS_SLIP, szPrintData);}
   virtual long DMSlipPrinterBarcode(CString csBarcode,long lBCType, 
      long lBCHeight, long lBCAlign, bool fPrintNumbers)
      {return dm.PrintBarcode(csBarcode,lBCType,lBCHeight, lBCAlign,fPrintNumbers, DMCLASS_SLIP);}
   virtual long DMSlipPrinterPrintBitmap(CString csBitmap, long lBmpAlign)
      {return dm.PrintBitmap(csBitmap, lBmpAlign, DMCLASS_SLIP);}
   virtual bool DMSlipPrinterIsSlipInserted(){return dm.PrinterSlipGetInserted();}
   virtual bool DMSlipPrinterIsSlipNearEnd(){return dm.PrinterSlipGetNearEnd();}
   virtual void DMSlipPrinterInsertSlip(bool bInsert){dm.PrinterSlipInsert(bInsert);}
   virtual long DMSlipPrinterHome(){return dm.PrinterSlipHome();}
   virtual long DMSlipPrinterFeedLines(int iLines){return dm.PrinterFeedLines(DMCLASS_SLIP, iLines);}

  virtual void PrintEndorsementChit(LPCTSTR szEndorsement) const;
  virtual bool InRemoteDeviceMode(void) const;
  virtual bool IsHopperSubstitutionUsed(bool & bEmpty, bool &bChuteJammed);


  virtual bool DMWasCashTaken(void);								// TAR 127340
  virtual void DMSetCashTaken(void);						// TAR 127340

  //virtual SMStateBase *DMDeviceNowOK(const long);	// this is overriden in the system message 3 state
#endif
  virtual SMStateBase *DMScale(void);
#ifndef _CPPUNIT
  // printer printing
  virtual void DMReceiptReprint(void);
  virtual void DMJournalReprint(void);		//Journal Printer	
#endif

  virtual CString GetGLOBALAddThe128Prefix(); //Tar 430804
#ifndef _CPPUNIT
  virtual void DMReceiptLine(const CString& csPrintLine);
  virtual void DMReceiptCut();
  virtual void DMReceiptBitMap(const CString csBmpFile);
  virtual void DMReceiptPrintBarcode(LPCTSTR szBarcode, long     lBCType);	 //to print supervisor/store mode access bar codes on the receipt printer
  virtual void DMJournalLine(const CString& csPrintLine);
  virtual void DMReceiptPrintBarcode(CString  csBarcode,
   long     lBCType, 
   long     lBCHeight, 
   long     lBCAlign = DM_ALIGN_CENTER, 
   bool     fPrintNumbers = false, 
   int      nDeviceClass = DMCLASS_RECEIPT ); //Tar 430804

  /**
   * RFC 445940:
   * \return a system message corresponding to a device error that occurs at
   *         startup.
   * \note   This code was once part of InitializeAll.  Moved to a dedicated
   *         method so that it could be called from multiple places.
   */
  virtual SMStateBase * DMHandleStartupError(void);  

  //Begin RFC 367092
  virtual long DMPrintLine(int nDeviceClass,  const CString& csPrintLine); // RFC 367092
  virtual long DMPrintBitmap(CString csBitmap, long lBmpAlign, int nDeviceClass = DMCLASS_RECEIPT); // RFC 367092
  virtual long DMPrintBarcode(CString csBarcode,long lBCType, long lBCHeight, long lBCAlign, bool fPrintNumbers, int nDeviceClass = DMCLASS_RECEIPT);  // RFC 367092
  virtual long DMProcessPrintLine(int nDeviceClass, const CString& csPrintLine);
  virtual long DMProcessPrintTokens(int nDeviceClass, CString& CurrentPrintLine);
  //End RFC 367092

  // change dispensing
  virtual long DMDispenseChange(const long, CString& csCashCountsStatus, long *lDispenseErrorCode=NULL);  // TAR 314536 - Return dispense error code to caller.
  virtual long DMDispenseChange(CString& csChangeInDenomination, long *lDispenseErrorCode=NULL); //dp185016 support glory device
  virtual long DMReportDispensedCashList();  // TAR 373026
  
  virtual long DMPrintCashManagementReceipt( CArray<int,int>& caValueList, CArray<int,int>& caChangeInCount );
  virtual long DMPrintCashManagementReceipt( 
    CArray<CString,CString>& m_caCashAcceptorDenomList, 
    CArray<CString,CString>& m_caCoinAcceptorDenomList,  
    CArray<CString,CString>& m_caCashDispenserDenomList, 
    CArray<CString,CString>& m_caCoinDispenserDenomList,
    CArray<int,int>& m_caCashAcceptorCountList, 
    CArray<int,int>& m_caCoinAcceptorCountList,  
    CArray<int,int>& m_caCashDispenserCountList, 
    CArray<int,int>& m_caCoinDispenserCountList,
    CArray<int,int>& m_caCashAcceptorChangeInCountList,
    CArray<int,int>& m_caCoinAcceptorChangeInCountList,
    CArray<int,int>& m_caCashDispenserChangeInCountList,
    CArray<int,int>& m_caCoinDispenserChangeInCountList );
  virtual CString DMPrintCashmanagementReceiptByDevice( CArray<CString,CString>& m_caDenomList,
    CArray<int,int>& m_caCountList,
    CArray<int,int>& m_caChangeInCountList);
  virtual long SendPrintDataToTB(CString *csAllData); //TAR341375
  virtual CString Justify( const CString csData );
  virtual CString PositionData( CString csFirst, CString csMiddle, CString csLast );

  virtual CString ConvertValueToString( const int nDenom, bool bTrimDecimal );
  // sound
#endif //_CPPUNIT
  virtual long DMSayAmount(const long);
  virtual long DMSayPhrase(LPCTSTR);         // say a wave file, pass name, no ext, no path
  virtual long DMSayPhrase(const int);      // say a wave file, pass file name id, no ext, no path
  virtual long DMSaySecurity(const long, LPCTSTR, HANDLE hWaitEvent=0); // security tone & wav file
  // cash acceptor
  virtual long DMCashAcceptorEnable(LPCTSTR);    // Coin Acceptor Modifications
												// pass cash type list ";100,500,1000,2000,5000,10000"
  virtual long DMAcceptorLight(const int lightState);
  virtual long DMCoinAcceptorLight(const int lightState);
#ifndef _CPPUNIT
  virtual long DMCoinDispenserLight(const int lightState);    // SSCOADK-4200  
#endif //_CPPUNIT

  virtual long DMCashAcceptorAccept(void);    // Coin Acceptor Modifications
  virtual long DMCashAcceptorReject(void);    // Coin Acceptor Modifications
  virtual long DMCashAcceptorDisable(void);    // Coin Acceptor Modifications
  // coin acceptor
  virtual long DMCoinAcceptorEnable(void);    // Coin Acceptor Modifications
#ifndef _CPPUNIT
  virtual long DMCoinAcceptorAccept(void);    // Coin Acceptor Modifications
  virtual long DMCoinAcceptorReject(void);    // Coin Acceptor Modifications
#endif //_CPPUNIT
  virtual long DMCoinAcceptorDisable(void);   // Coin Acceptor Modifications
  virtual bool DMKeepCurrencyAcceptorsDisabled();
#ifndef _CPPUNIT

  bool    bCashDeviceErrorToDispaly;  //415172
  virtual SMStateBase  *DisplayCashDeviceError(void);  // SR742 

  /**
   * \return Error for cash acceptor after failed reload when exiting 
   *         Degraded mode.
   */ 
  virtual SMStateBase *GetUnloadedCashAcceptorError(void); // SSCOP-4169

  /**
   * \return Error for coin acceptor after failed reload when exiting 
   *         Degraded mode.
   */ 
  virtual SMStateBase *GetUnloadedCoinAcceptorError(void); // SSCOP-4169

  /**
   * \return Error for cash changer after failed reload when exiting 
   *         Degraded mode.
   */ 
  virtual SMStateBase *GetUnloadedCashChangerError(void);  // SSCOP-4169

  // scale
#endif //_CPPUNIT
  virtual long DMScaleEnable(void);
#ifndef _CPPUNIT
  virtual long DMScaleDisable(void);
  // MSR
  virtual long DMMSREnable(void);
  virtual long DMMSRLight(const int lightState);
  virtual long DMMSREject(void);
  virtual long DMMSRDisable(void);				  //
  virtual long DMMSRCapture(void);				  // draw card into the capture bin
  // coupon despositer
  virtual long DMCouponLight(const int lightState);
#endif //_CPPUNIT

  // scanner
  virtual long DMScannerEnable(void);
  virtual long DMScannerDisable(void);

  // takeaway belt
  virtual void DMTakeawayBeltNotify(long Event);
#ifndef _CPPUNIT
  virtual void DMTakeawayBeltReset();
  // encryptor
  virtual long DMEncryptorEnable(void);
  virtual long DMEncryptorMaster(void);
  virtual long DMEncryptorSetup(LPCTSTR szWorkingKey, 
                                TBPINBLOCKFORMAT nPinBlockFormat, 
                                TBPINPADSYSTEM   nPinpadSystem,
                                TBPINENCODING    nPinBlockEncoding);

  virtual long DMEncryptorKeyTest();
  virtual CString DMEncryptorGetPIN(void);
  virtual long DMEncryptorDisable(void);
  virtual long DMEncryptorCancelSecureMode(void);
  virtual long DMPrintVersion(void);
  virtual void DMCashChangerPurgeBills(void);
  virtual long DMCheckHealth(long nDevClass);				  // call CheckHealth
  
  //dp185016 support glory device - support currency is high
  virtual CString DMCheckCashCounts(const bool, 
    CString& csHighCurrencies = (CString)EMPTY_STRING,
    CString& csCoinDispenserAlertStatus = (CString)EMPTY_STRING, 
    CString& csCashDispenserAlertStatus = (CString)EMPTY_STRING); // T/F T to adjust counts
	
  virtual void BillOrCoinErrorStatus(bool *pCoin, bool* pBill);
  virtual CString CashChangerErrorMessage(long lChangeDue, CString addMsg);
  
	/*
     * Determine error to show for jam condition while in CM
     * \param[in] void
     * \return  CString
     */
    virtual CString GetCashChangerError(void);

  virtual SMStateBase *DMEncryptorPinIsTooShort(void); // The returned pin was to short.
  virtual SMStateBase *DMEncryptorSecured(void);
  virtual SMStateBase *DMEncryptorCanceled(void);
  virtual SMStateBase *DMEncryptorCompleted(void);
#endif //_CPPUNIT
  virtual SMStateBase *DMSigcapStarted();
  virtual SMStateBase *DMSigcapEnded();
  virtual SMStateBase* DMTakeawayReadyForItem();
  virtual SMStateBase* DMTakeawayBagAreaBackup(bool isBackedUp);
  virtual SMStateBase* DMTakeawaySafetyCoverOpen(bool isOpen);
#ifndef _CPPUNIT
// moved to SmStateBase		  virtual SMStateBase* DMTakeawaySendScaleStatus();
  virtual void DMReceiptRequestCompleted(long lRequestId) {};
  virtual void DMJournalRequestCompleted(long lRequestId) {};
  virtual SMStateBase *DMSigcapFailure();
  virtual SMStateBase *DMCashChangerError(int nDMLastEvent);	// Tar 214729 RJF 101002
  virtual void getPrinterStatusLine(const long lCode, CString& printerStatusLine); //awk.11.12.2
  virtual void getCashAcceptorStatusLine(const long lCode, CString& printerStatusLine); //awk.11.12.2
  virtual void getCoinAcceptorStatusLine(const long lCode, CString& printerStatusLine); //awk.11.12.2 
  virtual void ProcessPrinterExtendedResultCode(long &);
  virtual void AddMLBCRecord(MultiLabelBarcodeFormatRecord *pMLBFRecord);
  virtual CString GetItemCodeFromMLBC(void);
#endif //_CPPUNIT
  virtual void TurnOnYellowTriColorLight(void);
  virtual int DMCheckForCoinDispenserLowSensor(void);
#ifndef _CPPUNIT
  virtual SMStateBase *DMCashChangerInOperable();	
  virtual SMStateBase* FillDMEvent2NotifyTB(const MessageElement* pEvent, sExtStateEventNotifyIn& sNotifyIn, sExtStateEventNotifyOut& sNotifyOut);
  virtual SMStateBase* HandleScannerData( const MessageElement* message);
#endif //_CPPUNIT
  virtual SMStateBase* DMRemoteScannerConnect(bool bConnect);
#ifndef _CPPUNIT
  virtual void DMHandleAcceptorHigh();
//+307389
  virtual SMStateBase *ClearAllStatesButLast(long lDeviceClass, SMStateBase *sReturnState);
  virtual void ClearAllStates(long lDeviceClass, SMStateBase *sReturnState);
  virtual void CheckAndCleanStateStack(void);
  virtual void PrintFifoStack(void);
//-307389
  virtual bool IsMINIHardware(); // tar 275064
  virtual bool IsSUPERMINI();	//tar 397356
  virtual bool Is5XHardware(); // tar 403042
  virtual bool Is6XHardware(void) const;
  virtual bool IsCashDeviceConfigured(); //TAR 452490
  virtual bool IsVerifoneConfigured();// SR875
  virtual void DisplayVerifoneForm(_bstr_t csForm);// SR875
#endif //_CPPUNIT
  static bool bIgnoreScannedData; //dp185016 tar 286970
  static int m_nCashRecyclerHardwareErrorCode;	
  static bool m_bCashRecyclerStopDeposit;
  static long m_nDMCashPaid;

  static int m_nCashRecyclerFailedRejectAmt;  // TAR 315616 - Track amount failed to reject.
  
  static bool bIsCashAcceptorEnabled;		//RFC330995
  static bool bIsCoinAcceptorEnabled;		//RFC330995
  static CString m_csPrinterCharSet;		//TAR378451
#ifndef _CPPUNIT
  virtual CString Get563xCassetteParms(long lcode, int nContext); //SR672
  CString sDeviceErrorTitle;  //Tar 438366
  CString sDeviceErrorTitle2; //Tar 449200
#endif //_CPPUNIT
  static CString sDeviceErrorMessageText;  //Tar 448083
  static int m_nDevInitStatus;   // RFC 445940: Holds dev startup result. 
#ifndef _CPPUNIT
    /**
     * Sets cash discrepancy status.
     * \param[in] bDiscrepancy - if true, cash unbalance condition 
	 * is detected for the Glory cash recycler.False, otherwise
     * \note Added for Glory CM Support
     */
    virtual void SetCashDiscrepancyStatus(bool bDiscrepancy);

    /**
     * Gets cash discrepancy status.
     * \param[in] void
     * \return true if cash unbalance condition is detected,
     * false otherwise
     * \note Added for Glory CM Support
     */
    virtual bool GetCashDiscrepancyStatus(void) { return m_bDiscrepancy; }
#endif //CPPUNIT
private:
   static bool m_bDiscrepancy;	// Holds cash unbalance/discrepancy flag
#endif


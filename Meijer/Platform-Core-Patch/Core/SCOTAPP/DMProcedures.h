//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  FILE:    DMProcedures.H
//
//  TITLE:   Device Manager (NCR API) Interface
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DMProcedures
#define _DMProcedures

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// InputEvent types
//
#define DM_STATUS       103



//TAR 195179 & 191490 start
#define TEMP_SIGNATURE_DIRECTORY		_T("%DATA_DRIVE%\\scot\\data\\tempprint\\")
//TAR 195179 & 191490 end
#define _K580 _T("K580")
#define _K590REL2 _T("K590REL2")
#define _K590REL3 _T("K590REL3")
#define _PRINTER_F306 _T("PRINTER_F306")
#define _CASHACCEPTOR_CASHFLOW _T("CASHACCEPTOR_CASHFLOW")
#define _CASHACCEPTOR_ARDAC _T("CASHACCEPTOR_ARDAC")
#define _CASHACCEPTOR_ZT1000 _T("CASHACCEPTOR_ZT1000")
#define _BAGSCALE_FLINTEC _T("BAGSCALE_FLINTEC")
#define _BAGSCALE_TEDEAHUNTLEIGH _T("BAGSCALE_TEDEAHUNTLEIGH")
#define _COINDISPENSER_TELEQUIP _T("COINDISPENSER_TELEQUIP")
#define _COINDISPENSER_TELEQUIP_HOPPER _T("COINDISPENSER_TELEQUIP_HOPPER")
//+dp185016 tar 277590
#define _CASHDISPENSER_5635 _T("CASHDISPENSER_5635")
#define _CASHDISPENSER_5633 _T("CASHDISPENSER_5633")
//-dp185016
#define _CASHDISPENSER_GLORY _T("GLORY")

// TAR 354761 - Make this constant match what is returned by GetDeviceName().
#define _CASHDISPENSER_SAFEPAY _T("SAFEPAY RECYCLER")

#define _CASHDISPENSER_BNR _T("BNR")
#define _COINRECYCLER_BCR _T("BCR")	//SR742

#define _COINACCEPTOR_2x _T("COINACCEPTOR_2x")
#define _COINACCEPTOR_3x _T("COINACCEPTOR_3x")

#define _PINPAD_2X _T("PINPAD_2X")      //tar 310198
#define _PINPAD_3X4X _T("PINPAD_3X4X")  //tar 310198


#define IDC_SCOTDEVICEMANAGER1          1004
#define IDC_SCALE1                      1069

#define LEVEL2_HC_ON_CASHCHANGER        0x01

// TAR 353244 - Define number of supported cassettes.
#define DMCASSETTE_COUNT 4				

/////////////////////////////////////////////////////////////////////////////
// DMProcedures window
#include "DllDefine.h"
#include "dmx\MSRWrapper.h"
#include "NCRCashAcceptorSOEx.h"
#include "NCR5633CurrDispSOEx.h"
#include "OposCAcc.h"
#include "ScotTelequip.h"
#include "ncrcch.h"
#include "NCRScaleSOEx.h"
#include <set> //SR742
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
   DMProcedures : public CWnd
{
private:
  CString csDMLastReceiptLine;      // for re-printed lost lines
  CString csDMLastReceiptBmpFile;   // for re-printed lost BMPs
	
  CString csDMLastJournalLine;

public:
  DMProcedures();
	
  ////////////////////////////////////////////////////////////////////////////
  // The obvious
  long Initialize(CWnd* pWnd = NULL); // called by MP at start up
  long UnInitialize(void);          // called by MP at shut down
  long BeginRemote( LPCTSTR sRemoteKey = _T("Remote") );
  long EndRemote( void );
  CString GetDevicesInError(void);
	
	
  ////////////////////////////////////////////////////////////////////////////
  // Sound related functions
  long SayAmount( long amount );    // say the cash amount
  long SayPhrase( LPCTSTR WaveFile ); // speak what is in the wav file
  long SaySecurity(long, LPCTSTR, HANDLE hWaitEvent=0);  // play a security tone and wav file
  long SaySignal(long);
  bool SoundInitPath(LPCTSTR ProductRegPath);	// local function
  bool SoundFindSoundFilePath(LPTSTR pszPathName, unsigned long nIntBufferLen);
  BOOL SoundConnectToServer(int iFocus, int iActive);
  BOOL SoundDisconnectFromServer(BOOL bSendDisconnectMsg);
	
  ////////////////////////////////////////////////////////////////////////////
  // Printer Related Methods
  long ReceiptLine(const CString& csPrintLine);				  // print a line on the receipt printer  PD write logo
  long ReceiptBitMap(const CString csBmpFile);			  // print a BMP file PD write logo
  long ReceiptCut(void);            // cut the paper
  long ReceiptPrintBarcode(LPCTSTR, long); // print a barcode for supervisor/store mode access bar codes (NN 080202 extra parameter added by FZ for RFC 187993)
  void ReceiptReprint(void);			  // re-print last print
  void JournalReprint(void);			  // re-print last print  //Journal Printer

  long JournalLine(const CString& csPrintLine);				  // print a line on the journal printer
  long PrinterCancelOutput(void);   // clear the printer buffer
  bool PrintError(void);						// an error occured T/F
  bool IsCheckPaymentAtLaneAvailable();		//RFC 356020
		
  ////////////////////////////////////////////////////////////////////////////
  // Lights

  // Specify which light, what state to set it to, and the request
  // type (security request or normal - required because they have
  // different priorities.)
  void setTriColorLight(DMTriColorLightColor color,
                    		DMTriColorLightState state,
		                    DMTriColorLightRequestType request);
  void turnOffTriColorLight();
  inline DMTriColorLightColor getTriColorLightColor()
    { return m_theTriColorLight.getColor(); }
  inline DMTriColorLightState getTriColorLightState()
    { return m_theTriColorLight.getState(); }
  // Used to save and restore the light status
  void saveTriColorLightStatus();
  void restoreTriColorLight();

  void saveLaneLightStatus();
  void restoreLaneLight();
	
  long RemoteRelay( int state ); // Lane light
  long CardReaderLight( int state );
  long DepositoryLight( int state );
  long PassbookLight( int state );
  long CoinAcceptorLight( int state );
  
  bool IsOpen();  //TAR#125161 
  bool IsDeviceConfigured( long nDeviceClass, long nDeviceId = 0 ); // TAR199516
  bool fStateMotionSensor;                  // does system have a motion detector  RFC192983 RF012502
  bool fStateTakeawayBelt;                  // does system have a takeaway belt?
  bool fTakeawayBeltBackup;                 // is the takeaway belt bagging area backed up?
  bool fStartUpDevFactoryLoading;			// TAR154447 System message clearing aux display LPM101200
  bool fPendingMiscIFError;                 // Tar 208394
  // Accessor to determine if a given device is currently remote.
  // By definition, any device that is remote is also available.
  bool IsDeviceRemote(long nDeviceClass, long nDeviceId);
  bool EASDeactivatorPresent( void );

  ////////////////////////////////////////////////////////////////////////////
  //
  // Component Enabling Methods
  //
  // Devices that cannot cause Input Events are not affected
  // by the Enable() function and can be considered to have a
  // fixed state of "enabled". Devices that can cause Input
  // Events are affected by the Enable() function and have an
  // initial state of "disabled".  If a device with buffered
  // data is disabled, the buffered data is not discarded.
  // It is therefore possible to receive data for a disabled
  // device.  Also, repeated disabling and enabling of devices
  // can degrade system performance and should be avoided.
  long CashAcceptorEnable(void);
  // When it is enabled, it can receive cash and send data events.
	
	
  long MSREnable(void);						  // Motorized: When a motorized magnetic card reader/writer
  // (MCRW) is enabled, it will be ready to accept a card.
  // The card will be automatically read when inserted and a
  // DM_DATA InputEvent() will be generated.When the card is
  // ejected or captured, the MCRW will automatically be ready
  // to accept a new card.  If an error occurs, the card will
  // be ejected automatically.  After an Eject or Capture the
  // device will remain enabled.  When an MCRW is disabled,
  // it will eject the currently held card and not accept another card.
	
  long PinPadEnable(void);				  // When enabled, a DM_DATA InputEvent() will be generated
  // after an entire PIN is entered.
	
  long ScaleEnable(void);					  // see device manager documentation for more info.
  long ScannerEnable(void);				  //
  bool IsScannerEnabled(void);			// RFC 219283 DE/RF 7401 Enables Motion Sensor on 7883 scanner

  long EASDeactivatorEnable(long nEnableMode);
	
  // disable functions
  long CashAcceptorDisable(void);	  // Complementary disabling operations
  long MSRDisable(void);
  long PinPadDisable(void);
  long ScaleDisable(void);
  long ScannerDisable(void);
  long EASDeactivatorDisable(void);
	
  ////////////////////////////////////////////////////////////////////////////
  // MSR Related Methods tack data place into messge object
  long	GetMSRTracksToRead(void);
  CString GetMSRAccountNumber();
  long	SetMSRTracksToRead(long nNewValue);
  long	SetMSRNonMagneticEnable(bool bAllowMagnetic);
  long	MSRCaptureCard(void);
  long	MSREjectCard(void);
  bool	MSRIsMotorized();
  CString MSRGetDeviceName(void);


  long	CashDrawerOpen(void); //ExitGate/Cashdrawer awk.1
  CString CashDrawerGetDeviceName(void);
	
	
	
  ////////////////////////////////////////////////////////////////////////////
  // Scanner Methods
  long GetScannerPacesetterEnabled(void);
  long SetScannerPacesetterEnabled(bool bNewValue);
   void SetScannerAutoDisable(bool bNewValue);
  // BeginLocaleCode
	long SetScannerDecodeData(bool bNewValue);
  CString GetScannerDataLabel(void);
	long GetScannerDataType(void); 
    long ScannerRemoteAccess( BOOL bAcquire );
  CString ScannerGetDeviceName(void);

	// EndLocaleCode

  ////////////////////////////////////////////////////////////////////////////
  // Scale Methods
  long GetScaleWeight(void);	// proper way to obtain weight
  bool IsScaleEnable();

  ////////////////////////////////////////////////////////////////////////////
  // Chech availability of cash and coin acceptor
  bool IsCashAcceptorAvailable();
  bool IsCoinAcceptorAvailable();
	
  ////////////////////////////////////////////////////////////////////////////
  // Check availability of cashdrawer/exitgate, if it is configured or not. awk.1
  bool IsCashDrawerAvailable();
  bool IsCouponSensorAvailable();
  bool IsDeviceAvailable( long nDeviceClass );


  ////////////////////////////////////////////////////////////////////////////
  // Key Lock Method
  long GetKeylockPosition(void);
	
	
  ////////////////////////////////////////////////////////////////////////////
  // Cash Changer Methods
  long CashChangerDispense( long Amount);
  long CashChangerDispense( LPCTSTR sDispenseList );
  long CashChangerReplenishCoins(long* plData, BSTR* pbsData); // TAR140515
  long GetCashChangerCashTaken(void);
  long CashChangerPurgeBills(void);
  long SetCashChangerCashTaken(bool bNewValue);		// TAR 127340
  long CashChangerGetCashCounts( BSTR* pCashCounts, BOOL* pDiscrepancy);
  void CashChangerGetStatusValues(long* plCoinStatus, long* plBillStatus);
  CString CashChangerGetDispensedCashList(void);
  CString CashChangerGetCurrencyMap(void);
  CString CashChangerGetLowIndicators(void);
	
  ////////////////////////////////////////////////////////////////////////////
  // Cash Acceptor Methods
  long SetCashAcceptorCurrencyAcceptList(LPCTSTR lpszNewValue);	// sample string depicting ";100,500,1000,2000"
  long CashAcceptorReject(long nAmount = 0);    // accepting $5 $1 $10 $20 acceptance
  long DMProcedures::CashAcceptorAccept(void); // this works
  long CashAcceptorReadBillCounts(BSTR* pCashCounts);
  long GetCashAcceptorDeviceStatus(void);
  long GetCashAcceptorCurrencyCashList(CString* pcsCoinCounts);
	
  // Begin Coin Acceptor Modifications
  // Coin Acceptor Methods
  long CoinAcceptorEnable( void );
  long CoinAcceptorDisable(void);
  long CoinAcceptorReadBillCounts(BSTR* pCoinCounts);
  long SetCoinAcceptorEnableCoins(bool bNewValue);
  long CoinAcceptorReject(long nAmount = 0);
  long CoinAcceptorAccept(void);
  long GetCoinAcceptorDeviceStatus(void);
  long GetCoinAcceptorCoinList(CString* pCoinCounts);
  // End Coin Acceptor Modifications

  //SR742+
  long ReloadCoinAcceptor(void);
  long ReloadCashAcceptor(void);
  long ReloadCashChanger(void);
  long ReloadMSR(void);  
  //SR742-

  // Begin Signature Capture Modifications
  // ignature CaptureMethods
  long SigCapBeginCapture(LPCTSTR pFormName);
  long SigCapEndCapture();
  long SigCapSetWindowHandle(long* pHWnd);
  long SigCapEnable() ;
  long SigCapDiable() ;
  long GetSigCapNotifyStyle() ;
  long SetSigCapNotifyStyle(long lNewValue) ;
  long SigCapDataToBitmap(LPCTSTR pFormName) ;
  long SigCapDataTo5991Format(BSTR *pbsData) ;
  // End Signature Capture Modifications

  //+4.2 card dispenser
  long CardDispenserEnable();
  long CardDispenserDisable();
  long CardDispenserGetCardPosition();
  long CardDispenserDispense();
  long CardDispenserRead();
  long CardDispenserMoveToReject();
  long CardDispenserMoveToGate();
  long CardDispenserCaptureCard();
  long CardDispenserSetTracksToRead(long nNewValue);
  bool GetCardDispenserVeryLow();
  bool GetCardDispenserEmpty();
  bool GetCardDispenserGateOpen();
  void GetAdjustCardDispenserBinCounts(long& lCountBin, long& lCountReject, const long lAdjValue);
  bool CardDispenserWrite( const long lTracksToWrite, const CString& csTrack1Data, \
		const CString& csTrack2Data, const CString& csTrack3Data );
  void CardDispenserEnableInsertion(bool bEnable);
  void CardDispenserDisableInsertion();
  //-4.2 card dispenser
  /////////////////////////////////////////////////////////////////////////////
   long CheckHealth(long lDevClass, long nDeviceId = 0 ); // call CheckHealth
   bool GetCoinsOrBillsVeryLow();
   void SetCoinsOrBillsVeryLow(bool);
   bool GetCoinsVeryLow();      // TAR 279497
   void SetCoinsVeryLow(bool);  // TAR 279497
   bool GetBillsVeryLow();      // TAR 279497
   void SetBillsVeryLow(bool);  // TAR 279497
   bool GetCoinDispenserWithLowSensor();  // TAR 283924
   bool GetPrinterPaperLow();
   long LoadPrintObjects(void);	// load preset bitmaps for receipt and journal printers
   long PrintLine(int nDeviceClass, const CString& csPrintLine);
   long ProcessPrintLine(int nDeviceClass, const CString& csPrintLine);	// process print lines with tokens for bitmaps or barcodes or ???
   long ProcessPrintTokens(int nDeviceClass, CString& CurrentPrintLine);
   long PrintBitmap(CString csBitmap, long lBmpAlign, int nDeviceClass = DMCLASS_RECEIPT);		// DM doesn't allow bitmap printing on other stations
   long PrintBarcode(CString csBarcode,long lBCType, long lBCHeight, long lBCAlign, bool fPrintNumbers, int nDeviceClass = DMCLASS_RECEIPT); // DM doesn't allow barcode printing on other stations
   bool GetDeviceStatusInfo(const int nDeviceClass, bool& bIsEnabled, CDeviceStatistics* DeviceInfo = NULL);
   bool PrinterSlipGetInserted(void );
   bool PrinterSlipGetNearEnd();
   void PrinterSlipInsert( bool bInsert);
   long PrinterSlipHome( void );
   long PrinterSlipFlip( void );
   long PrinterSlipSetRotateSpecial(long lRotateProperty);
   long PrinterSlipGetRotateSpecial(long & lRotateProperty);

   long PrinterFeedLines( long nDeviceClass, int iLines);
   long PrinterSetBitmap( long nDeviceClass, CString& csBitmapName, long nAlign, 
      int nBmpIndex );
   // SA250050: Core Patch to support dynamic SetBitmap calls
   long PrinterSetBitmap( long nDeviceClass, CString& csBitmapName, long nAlign, 
      int nBmpIndex, int nWidth );
   // MICR Methods
   long MICREnable( void  );
   long MICRDisable( bool bEject = true);
   long MICRGetCheckData( CString& sRawData, CString& sAcctNumber,
      CString& sAmount, CString& sBankNumber, CString& sEPC,
      CString& sSerialNumber, CString& sTransitNumber, long* pnCheckType, 
      long* pnCountryCode );
   CString MICRGetDeviceName(void);  //RFC 356020
   CString GetEventDescription(DWORD dwEvent);
   //TAR 271776
   long GetBitmapWidth(CString&, float);
   virtual CPrintStation* GetPrintStation(long nDeviceClass); 	// RFC 367092
   virtual long PrinterGetBinaryConversion(long nDeviceClass);  // RFC 367092
   virtual void PrinterSetBinaryConversion(long nDeviceClass, long lBinaryVal); // RFC 367092
   CScaleWrapper* GetScaleWrapper(long nDeviceId  =0 );
   void SetPrinterPaperLow(bool); //TAR 408696

protected:
//  DMInterface dmi;

	bool bScannerIsEnabled;					// RFC 219283 DE/RF 7401 Enables Motion Sensor on 7883 scanner

  CString ApplicationRegistryRootPath;
	
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(DMProcedures)
  //}}AFX_VIRTUAL
	
  // Implementation
public:
  virtual ~DMProcedures();
	
  // Generated message map functions
protected:
   //{{AFX_MSG(DMProcedures)
	afx_msg void OnTimer(UINT nIDEvent);
   DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
  afx_msg void OnStatusEventScotdevicemanager1(long StatusType, long nDeviceClass, long DeviceID);
  afx_msg void OnInputEventScotdevicemanager1(long InputType, long nDeviceClass, long DeviceID, const VARIANT FAR& DeviceData);
  DECLARE_MESSAGE_MAP()
   private:
     long OleDispatchException(COleDispatchException *,const long,long,LPCTSTR,const int);
     void OleException(COleException *,const long,long,LPCTSTR,const int);
     void Exception(CException *,const long,long,LPCTSTR,const int);
   CString GetEncoded128CBarcode(CString csBarcode);	//TAR 211913 BZ
   long DMIInitialize(void);
//   long SetEnableFlags(const long,const BOOL);
   long DMIUnInitialize(void);
   long DMICheckDevices( DWORD dwOptions = 0 );
   public:
	  //SR742+
	  std::set<long> m_lDeviceClassThatWasInError;
	  std::set<long>::iterator m_lDeviceClassThatWasInErrorIterator;
	  //SR742-
      int   nDMLastReceiptFunction;  // 0-none, 1-line, 2-BMP, 11-cut line, 12-cut BMP
      int   nDMLastJournalFunction;  // 0-none, 1-line, Journal Printer	
  	  int   m_nStartDeviceErrorResultCodeExtended;   //Error Handling
  	  int   m_nStartDeviceErrorDeviceClass;   //Error Handling
	  CString m_csReceiptPrinterModel;           //Error Handling
	  int m_nBillDispenserType[DMCASSETTE_COUNT];
	  CString m_csSoundDefaultLanguageCode;
	  CString m_csCashAcceptorModel;
	  CString m_csBagScaleModel;
	  CString m_csCoinDispenserModel;
	  CString m_csCoinDispenserStage;
	  //+dp185016 tar 277590
    CString m_csCashDispenserModel;
    //-dp185016
    CString m_csCashRecyclerModel; //dp185016 generic cash management
    CString m_csCoinAcceptorRelease; // TAR 297103
    CString m_csPinpadRelease; // TAR 310198

   private:
     // The TriColor light
      bool  m_bDMOpened;
      // This is incorrect.
//      DeviceStatistics  m_Dvs[DMCLASS_MAX_CLASSES + 3];
      DMTriColorLight   m_theTriColorLight;
      CDeviceManagerEx  m_devmgr;                   // only seen by DMInterface class
      bool m_fIsCoinsOrBillsVeryLow;	// Reporting bool for the Green Blinking requirement
                                   // when we reach a second low level for hopper or cassette
      bool m_fIsCoinsVeryLow;  // TAR 279497
      bool m_fIsBillsVeryLow;  // TAR 279497
      long m_nLaneLightStatus;
      long m_nLaneLightStatusSaved;
	   long m_nDepositoryLightState;
      long m_nCoinAcceptorLightState;
      CString m_csDevicesInError;
      bool m_bCoinDispenserWithLowSensor; /// TAR 283924
  	  bool m_bIgnoreDMEvents;			  // TAR 309097
	  long m_lDegradedModeType; //SR742
      bool m_bPrinterPaperLow;            // TAR 408696
	  
   public: 
	  long GetResultCodeExtended(long, long);
  	  long GetResultCode(long, long);
	  CString GetDeviceName(long DeviceClass, long DeviceID);
	  CString GetDescription(const int nDeviceClass, long nDeviceId = 0);
	  long Enable(int nDeviceClass, long nDeviceId, bool fEnable); //SR742 moved here. changed access to public

   protected:
	  CString ReadReceiptPrinterModel(void);     //Device Error
	  void ReadBillDispenserTypes();             //Tar 227532
	  void ReadDevicesModel(void);    
      
   public:
       void TriColorLightEndTransaction();  
	   long CashRecyclerGetErrorCode(long pData);  //0x01 = get Coin Recycler status 0x80 get Bill Recycler status
     CCashRecyclerWrapper* GetCashRecycler(); //dp185016 expose CCashRecyclerWrapper
		CString CashRecyclerGetCheckHealthText(void);
	   long CashRecyclerStopDeposit(CString&);	
	   void CashRecyclerResetCounts(void);				// TAR 311471
	   void CashRecyclerUpdateDispensed(long nDisp);	// TAR 311471
	   void CashRecyclerPauseCounts(bool bPause);		// TAR 311471
	   BOOL CashRecyclerGetEnabled(void);				// TAR 311332, 313343
	   bool CashRecyclerDispenseRetry(long rc);			// TAR 310660
	   bool CashRecyclerGetCurrencyCashList(CString &csList);
	   bool CashRecyclerGetDepositCounts(CString &csList);

	   // +SafePay cash recycler.  
	   long UnlockNoteCover(const long nDeviceClass, long nDeviceId=0);
	   bool IsUrgentMessagePending(const long nDeviceClass, long nDeviceId=0);
	   long RetrieveUrgentMessage(CString &msg, const long nDeviceClass, 
								  long nDeviceId=0);
	   bool ReadStatusBits(long &pData, const long nDeviceClass, 
								  long nDeviceId=0);
	   bool IsPauseCountsActive(const long nDeviceClass, long nDeviceId=0);
	   bool SetDeviceEnabledRaw(BOOL bEnable, const long nDeviceClass, 
                                long nDeviceId);        // TAR 356035
  	   // -SafePay cash recycler.  

		long CloseDevice( long nDeviceClass, long nDeviceId = 0 ); 
		long OpenDevice( long nDeviceClass, long nDeviceId = 0 ); 
      long ReadFirmwareVersion( long nDeviceClass, long nDeviceId, CString& sVersion, long* lVersion );
		long DirectIO( long nDeviceClass, long nDeviceId, 
         long nCommand, long* plData, CString& sData, long* pnRcEx ); 
	   long	MSRGetCardPosition(void);
	   void EnableCouponProcessing( bool bIsNeeded );	//SR631.1
	   //SR742+
	   long GetDegradedModeType() { return m_lDegradedModeType; }; 
	   void SetDegradedModeType(long lDegradedMode);
	   //SR742-

   private:
	   bool GetNonAdminURLName(CString &csFileName);	// TAR 307991/308999
   };

#define DM_ERROR_TIMEOUT            99
#define DM_ERROR_NO_HARDWARE        98
#define DM_ERROR_OFFLINE            97

#define DM_MICR_ERR_LINE_ERROR      801
#define DM_MICR_ERR_NOCHECK         802
#define DM_MICR_ERR_NO_DATA         803
#define DM_MICR_ERR_BAD_DATA        804
#define DM_MICR_ERR_CHECKJAM        805
#define DM_MICR_ERR_COVER_OPEN      806
#define DM_MICR_ERR_BADSIZE         807
#define DM_MICR_ERR_CHECKDIGIT      808

#define DM_FS _T('~')         //Unicode and Japan requires us not to use E1, which
                              //is used to combine Kanji characters.






/////////////////////////////////////////////////////////////////////////////

#endif _DMProcedures

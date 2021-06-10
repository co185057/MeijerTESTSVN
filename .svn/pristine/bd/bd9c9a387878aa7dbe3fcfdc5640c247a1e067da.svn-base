    /////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // FILE:  PSProcedures.H
    //
    // TITLE:   Display Presentation Services Interface Procedures (High Level)
    //
    // This module handles all DPS calls for the SCOT application so that no other
    // component need know the details of DPS calls and all other components are
    // insulated from changes to the DPS API.
    //
    // From here we call the PSInterface routines
    //
    //
    // AUTHOR:  Peter Denhoed, Updated DcL-
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
#ifndef _PSPROCEDURES
#define _PSPROCEDURES

#include "PSProceduresBase.h"
#include "psevt.h" //  CRD126194

#ifdef _SCOTSSF_
    class DLLEXPORT
#else
    class DLLIMPORT
#endif
PSProcedures : public PSProceduresBase// presentation services procedures (high level)
{
    
public:

    PSProcedures();

#ifdef _TESTSSF
    void ShowCardAmountList(void);
    void ShowGiftCardActivityList(void);
#endif
	// Start RFQ 9554
    virtual void SetSkipFrameDisplay(bool bSkip);
    virtual bool SkipFrameDisplay(void);
	static  bool bSkipFrameDisplay;
	// end RFQ 9554

    virtual void SetUSSFFrame(LPCTSTR lpszContext);
    virtual int DisplaySelectedItem(CString csFirstchar="", bool bIsItemDescription=false, bool bFavoritesButton=false);
    virtual SCOT_LANGUAGE_TYPE GetLanguage(void);
    virtual CString GetPSText(const int nMsg);
    virtual CString GetPSText(const int nMsg, SCOT_LANGUAGE_TYPE language);
	virtual CString GetPSText(const CString csMsg, SCOT_LANGUAGE_TYPE language); // CRD126194
    virtual long LoadPicklistAssistItemList(const CString& csPLAItemList);
    virtual void SetLanguage(SCOT_LANGUAGE_TYPE);
    virtual void SetCategoryTitles(const CString& csCategory);
    virtual CString GetCategory(const CString& csCategories, const CString& csCategory);
    virtual bool FindCategory(const CString& csCategory, CString& csRetCategory, CString& csCategoryId, int& nTabIndex );
    virtual void ClearPicklistAssistItemList(void);
    virtual bool IsPicklistAssistItemListEmpty(void);
    virtual bool IsPickListItem(const CString& csItemCode);
    virtual bool IsInXM();
    virtual void SetInXM(bool bInXM);
    virtual void EchoAmountPaidToReceipt(int nTenderType, long lAmountPaid);
    virtual void PushLevels(const CString& csPreviousLevel, const CString& csCurrentLevel);
    virtual void PopLevels(CString& csPreviousLevel, CString& csCurrentLevel);
    virtual void DeleteAllLevels(void);
    virtual bool IsLevelEmpty(void);
    virtual bool CanScroll(const long lScrol);
    virtual CString GetScreenTitle();
    virtual CString GetInstructions();
    virtual void DisplayNextCategoryItem(const long lScroll);
    virtual void DisplaySelectedSubItem(CString csSubCategory);
    virtual void DisplayQuickPickItem();
    virtual CString GetLastProduceLookupList();
    virtual void SetLastProduceLookupList(CString sCategory);
    virtual long GetPicklistItemsSold();
    virtual void SetPicklistItemsSold(long lTotalPicklistItemsSold);
    virtual long GetPicklistItemsIncludeVoidItems();
    virtual void SetPicklistItemsIncludeVoidItems(long lTotalPicklistItemsIncludeVoidItems);

    //CPSX *GetPSXObject();
   // CString GetLocaleInfoString(LCID lcid);
    

    virtual void GetPicklistItemInfo( CItemInfo *pItem, CStringArray& csItemDescList, 
                                    CStringArray& csSubCatDescList, 
                                    CStringArray& csItemCodeTextList, 
                                    CStringArray& csImageList, 
                                    CStringArray& csItemDataList,
                                    CStringArray& csCatDescList,
                                    CArray<ITEMTYPE, ITEMTYPE>& nItemTypeList,
                                    const CString& csControl,
                                    const CString& csCat = _T("") );
    virtual void SetPickListButtonProperties( CStringArray& csItemDescList, 
                                         CStringArray& csSubCatDescList, 
                                         CStringArray& csItemCodeTextList, 
                                         CStringArray& csImageList, 
                                         CStringArray& csItemDataList,
                                         CStringArray& csCatDescList,
                                         CArray<ITEMTYPE, ITEMTYPE>& nItemTypeList,
                                         const CString& csControl,
                                         int nButtonCount = 0 );
    
    virtual void GetParentCategories(const CString& csCategories, CStringArray& csParents);
    virtual BOOL DisconnectFromServer(BOOL bSendDisconnectMsg);
    virtual BOOL ConnectToServer(int iFocus, int iDisplay);
    virtual long LoadProdList(const CString& csXML_List = _T("PickList.xml"));

    virtual void SetDevErrURL(LPCTSTR lpszURL);

    virtual long ShowTrainingModeOnScreen(bool bShowTrainingMode);
    virtual long InitializeConfigData(LPCTSTR szPlstName, LPCTSTR szTareName);
    virtual long Initialize(                  // do all initialization

            LPCTSTR,                         // ...path_name of SCOTBMPS.DAT file
            LPCTSTR,                         // ...path_name of SCOTMSGS.DAT file
            LPCTSTR,                         // ...path_name of SCOTPLST.DAT file or NULL
            LPCTSTR,                         // ...path_name of SCOTTARE.DAT file or NULL
            const int,                      // ...tare button 1 tare 1-10
            const int,                      // ...tare button 2 tare 1-10
            const bool,                     // ...Metric scale T/F
            const bool);                    // ...Live video installed T/F
    virtual long UnInitialize(void);          // do all un-initialization
    virtual long UnInitializeConfigData(void);

    //--------------------------------------------------------------------
    // Methods to change Assist Mode keys.
    //--------------------------------------------------------------------
    // NewUI
    virtual long SetKeyLabel(LPCTSTR sControlName ,const int,LPCTSTR);  // set an Assist keyboard key description
    virtual long DisableAssistModeKey( LPCTSTR sControlName, const int nKey);

    //--------------------------------------------------------------------
    // Methods to update current screen.
    //--------------------------------------------------------------------

    // 081502: Tar 212025 CHu/FFR- Make function virtual so that they can overidden in SSF
    virtual long Button(LPCTSTR szCtlName,LPCTSTR szContent, const bool fEnable);
    virtual long Button(LPCTSTR szCtlName,const PSTEXT nText, const bool fEnable);
    virtual long ButtonFlash(LPCTSTR szCtlName, const bool fFlash); 
    virtual long ButtonState(LPCTSTR szCtlName, const bool fEnable, const bool fShow);
    virtual long ButtonEnable(LPCTSTR szCtlName, const bool fEnable);
    virtual void HideCMWindows(); //hide controls - improve screen display
    virtual void HideCMButtons(); //hide buttons - improve screen display 
    virtual void SetDisplayWMFlag(bool bNeedDisplayWM); //set display flag for W&M - improve screen display
    virtual void SetFrame(LPCTSTR lpszContext);
    virtual void SetCMFrame(LPCTSTR lpszContext);
    virtual void SetCurrentFrame(LPCTSTR lpszContext);

    virtual void BuildControlsList();

    virtual void SetBackground(const BMPS); // just the BMP number
    virtual long SetLeadthruText(const PSTEXT nText1, const PSTEXT nText2 = PS_NOTEXT); // leadthru text string
    virtual long SetLeadthruText(LPCTSTR szText1,LPCTSTR szText2 = NULL);    // show text in the leadthrutext areas
    virtual long SetLeadthruText(const PSTEXT nText1,LPCTSTR szText2);      // show text in the leadthrutext areas
    virtual long SetLTTinto2(LPCTSTR szLTText);           // format one line into 2
    virtual long SetLTTinto2(const PSTEXT nLTText);       // format one line into 2, converts the ps text to string
    virtual long SetTitle(LPCTSTR szText1,LPCTSTR szText2 = NULL);// display text in title area
    virtual long SetTitle(const PSTEXT nText1, const PSTEXT nText2 = PS_NOTEXT);// display text in title area
    virtual void SetButtonText(LPCTSTR szCtlName, const PSTEXT nButton);
    virtual void SetAttractScreenMessage(LPCTSTR szText);
    virtual void SetAlphaNumericMessage(LPCTSTR szText);

    virtual void SetShiftAndSymbolState(const CString csContextName, const bool bSymbolOn);
    virtual void PopulateHKSensitiveControls(CString csCtxName, HKStatus hkStatus, bool bShiftOn, bool bAltGrOns, bool bSymbolOn=false);
    virtual void PopulateHotKeys(CString csCtxName);
    //-SR828

    virtual void SetCMGobackButtonText(const PSTEXT);
    virtual void RefreshCMTotal();                        // refresh total if it is visible.
    virtual void ShowCMTotal(const bool,                  // show current total as a total T/F
                   const bool);                 // show current total as retracted total T/F
    virtual void ShowCMWeightNumber(CString, CString, long);  // SR682 - Show Weight/Item Number Info On Intervention Screens
    virtual void HideCMWeightNumber();                    //SR682 - Hide Weight/Item Number Info From Intervention Screens
    virtual void ShowCMSmallReceipt(void);            //show small receipt in customer mode
    virtual void ShowCMTBText(LPCTSTR csMessage);     // show POS Message Display on Customer Mode
    virtual void ShowSMTBText(LPCTSTR csMessage);     // show POS Message Display on Store Mode
    virtual void ShowAmountDueAmountPaid( const long,     // Amount due    
                                const long);    // Amount paid
    virtual void ShowChangeDueAmountPaid( const long,     // Change due
                                const long,               // Amount paid
                                bool bChangeDueText = true);
    virtual void ShowAmountRefundedAmountPaid( const long,  // Amount refunded
                                     const long); // Amount paid
    virtual void UpdateAmountDueAmountPaid( const long,     // Amount due    
                                  const long);    // Amount paid
    virtual void ShowTotalSavings(const long lTotalTrxSavings);
    virtual void ShowTotalRewardPoints(const long lTotalTrxPoints);
    virtual void ShowCustomerRewardMessage(LPCTSTR pCustomerMessage);
    virtual void ShowCMRewardInfoBox(const long lTotalTrxSavings = LONG_MIN, const long lTotalTrxPoints = LONG_MIN,
                                LPCTSTR pCustomerMessage = _T("DefaultIsDoNotResetMessage"));
    virtual void ShowSavingsORRewards(bool bShowSavings, long lValue);
    virtual void ShowSMTotal(const bool fTotal);
    virtual long ShowFrame( bool bHelpState = true); //add a paramer for Help button display - improve screen display
    virtual void SetCMMascotImage(const BMPS);
    virtual CString GetMascotBmpName(const int nMsg); // return a pointer to the PS text string
    virtual CString GetSoundName(const int nMsg); // return a pointer to the PS text string
    virtual void SetCMGobackButtonVisible(bool); 
    virtual CString Background(void); // returns the name of the last BMP used
    virtual void ScanReceiptForExceptionCategorys(CStringArray &csArray); 
    virtual bool ScanReceiptForSecurityTagItems(void);
        
    virtual long ItemsInFavoritesList(void);
    virtual void ReadPImgFiles(CStringArray *, CStringArray *, CStringArray *);
    //Begin Tar 210315, make it virtual for SSF to override
    virtual void GetNewDescriptions( const CString&, const CString&, CStringArray&, CStringArray& );
    //End Tar 210315
    virtual void GetDeviceSpecificBMPFileNames(void); 
    virtual void CheckIfBMPFileExists(CString *);
    virtual bool SetDynamicBackground(CString *);
     
    virtual CString FmtDollar(const long, SCREEN_POSITION sp = SP_OTHER, FORMAT_OPTION bReporting = DEFAULT);  // format cents into a $0.00 string
    virtual CString FmtWeight(const long);     // format weight to string 0.00 lb or 0.000 kg
    virtual CString FmtWeightInfo(void);       // format info line for echo and e-rcpt
    virtual CString FmtQuantityInfo(void);     // format info line for echo and e-rcpt

    virtual long PromptAmount(long lAmt);     // display amount in prompt area

    virtual long Prompt(const PSTEXT);        // show text in prompt area
    virtual long Prompt(LPCTSTR);              // show text in prompt area
    virtual long Echo(const PSTEXT);          // show text in echo area
    virtual long Echo(LPCTSTR szText, LPCTSTR szText2 = NULL);

    // Support for screen with multiple echo lines: line 1-8).
    virtual long Echo(short lineNumber, LPCTSTR szText);

    virtual long HideEcho(void);
    virtual long InfoMessage(LPCTSTR);         // display message text for SelectCardType screen.

    virtual long Message12(const PSTEXT, bool bIsModal = false); 
    virtual long Message12(LPCTSTR, bool bIsModal = false);
    virtual long Message12(LPCTSTR);           // display text in message 12 
    virtual long Message12Modal(LPCTSTR);      // display text in message 12 

    virtual long Message13(const PSTEXT);   // display text in message 13 area
    virtual long Message13(LPCTSTR);         // display text in message 13 area

    virtual long Message23(const PSTEXT, bool bIsModal = true);  // display text in message 23 area
    virtual long Message23(LPCTSTR, bool bIsModal = true);        // display text in message 23 area

    virtual long Message24(const PSTEXT, bool bIsModal = true);  // display text in message 24 area
    virtual long Message24(LPCTSTR, bool bIsModal = true);        // display text in message 24 area

    virtual long Message2(const PSTEXT);    // display text in message 2 area
    virtual long Message2(LPCTSTR);         // display text in message 2 area

    virtual long Message3(const PSTEXT);    // display text in message 3 area
    virtual long Message3(LPCTSTR);         // display text in message 3 area

    virtual long Weight(const long);        // weight in 1/100ths lb or 1/1000 kg
    virtual long Total(const long);         // update total in cents
    virtual long Tax(const long);           // update tax in cents

    virtual long EchoInput(const _TCHAR, bool bAllowAlpha = false, bool bAsteriskMask = false); // append user input digits to echo area, pass 0x00 to reset
    virtual long EchoDecimalInput(const _TCHAR, bool bAllowAlpha = false);     // append user input decimal to echo area, pass 0x00 to reset

    virtual void PSHideKeyboard();          // hides CMKeyboard to clear screen and echo box
    virtual void PSHideNumericKeypad();     // hides CMNumericKeypad
    virtual void PSHideSMMNumericKeypad();  // hides SMMNumericKeypad
    virtual void PSBackSpaceKbd();          // sets CMKeyboard to apply correction key on echo box

    virtual void EnterClearKeys(const bool disableEnter=false,
                              const bool disableClear=false);
                              // enable or disable Enter and clear keys if numeric keypad is visible.
    
    virtual long AllowInput(
    const long lMax,                          // allow echo input digits to the echo area
    const bool fHide,                         // ...hide input as *'s T/F
    const bool fDollar,                       // ...fmr inout as dollar amount $0.00 T/F
    const long lMin = 1);                     // ...Minimum input before enabling Enter key
    virtual long ClearInput(void);        // reset all of echo input area
    virtual long BackSpaceInput(void);    // back up one echo input area character

    virtual long ScrollUp(const CString& csReceipt = _T(""));    // scroll this receipt up
    virtual long ScrollDown(const CString& csReceipt = _T(""));  // scroll this receipt down
    virtual long HideSalesReceipt(void);

    virtual long HideButtons(void);
    virtual long DisableButtons(void);    // disable all visible buttons
    virtual long DisableSelectionList(void);      // disable touch input.

    virtual long HideReceiptButtons(void);
    virtual long DisableReceiptButtons(void);

    virtual long DelayMessage12(const PSTEXT);    // display text in message 12 area of next screen

    virtual bool ShowDelayedMessages(void);
    virtual long ShowWideList(void);      
    virtual long ShowKeyboard(void);
    //Begin Tar 210104
    virtual long ShowAlphaNumericKeyboard(void);
    virtual void PSHideAlphaNumericKeypad();
    //End Tar 210104
    virtual long ShowNumericKeypad(void);
    virtual long ShowSMMNumericKeypad(const bool hideDecimal=false);  

    //  long ShowItemList(const int);
    virtual long ShowTareList(void);
    virtual long ShowVideoLarge(void);      // activate quick cam in large rea
    virtual long ShowVideoSmall(void);      // activate quick cam in small area
    virtual long ShowVideoProduce(void);      // activate quick cam on produce screens
    virtual long HideVideoLarge(void);      // hide quick cam in large rea
    virtual long HideVideoSmall(void);      // hide quick cam in small area
    virtual long HideVideoProduce(void);      // hide quick cam on produce screens
    virtual long ShowCashBackList(const long lDue, int nBack[], int numBack, bool bTenderEbt);

    //Tar 127562 check for receipt is empty or not
    virtual bool SalesReceiptEmpty(void);

    // these 5 routines set your pointer to a string, usually with the item code
    // the return code, if not -ve, is the index in the list, 1 based, of this item
    virtual long GetItem(                   // get currently selected item info
            CString &,                            // ...description
            CString &,                            // ...item code
            long &,                               // ...quantity
            long &,                               // ...weight
            long &,                               // ...price
            long &,                               // ...unit price
            long &,                               // ...tare code
            long & lBarCodeType,                  // ...scan type
            const CString& csReceiptSrc,
            const CString& csReceipt,
            const CString& csItemEntryId = _T(""));
    //*** end of NewUI ***//

    virtual int  GetCashList(CString &);    // get tender list CString desc
    virtual long GetTareList(CString &);    // get tare list CString desc
    virtual long GetTareAt(const long);     // get tare code at this index
    virtual int  GetWideList(CString &);    // get wide list CString item code //Feng

    virtual long GetInput(CString &);       // get the current input echoed data, returns length

        
    virtual CString GetBmpName(const int nBmp);
    virtual long SetPSText(const WORKMSGS,LPCTSTR);   // set a PS_WORKn pointer to a text string
    virtual void SetRemoteLanguage(const CString &csISOLanguageCode);
    virtual void SetNextGenUICustomerLanguage(const CString &csISOLanguageCode);
    virtual void SetNextGenUICustomerLanguage(SCOT_LANGUAGE_TYPE lang);

    virtual long AddItem(void);               // add item to receipt
    virtual long AddMessageToReceipt(const CString& csReceiptSrc, CString text);  // add text to e-receipt 
    virtual long ScanReceiptToVoid(const CString& csReceiptSrc, CString& csItemEntryId, int nIndex = -1);   // scan receipt for item to void   
    virtual int  FormatMessageToReceipt(CString&, bool bIsSmallRcpt=false, int nLines=-1);
    virtual long ResetTransactionLists(void); // reset all receipt to empty them, at end of transaction

    virtual long AddSecurityLog(LPCTSTR);     // add a text string to the security log list
    virtual long AddMediaList(CString);       // add text string to Media List
    virtual long UpdateMediaList(const long index, CString);   // update to Media List
    virtual long AddDetailsList(CString);     // add text string to Details List

    virtual long ResetMediaList();            // empties Media List       
    virtual long ResetDetailsList();          // empties Details List
    virtual long ResetAMTareList();           // empties AMTareList
    virtual long AddSMMItemList(CString);     // add text string to Media List
    virtual long ResetSMMItemList();          // empties Media List
    virtual long ClearSMMItemList();          // empties Media List

    virtual long AddDelaySecurityList(CString); // add text string to Delay Security List
    virtual long ResetDelaySecurityList();      // empties Delay Security List
    virtual long ClearDelaySecurityList();      // empties Delay Security List

    virtual long GetCoupon(long &, CString&, CString&, long&, long&, long&, long&, long&, long& lBarCodeType);                // Get coupon from coupon list
    virtual long GetVisualItem(long &, CString&, CString&, long&, long&, long&, long&, long&, long& lBarCodeType);          
    virtual LPCTSTR GetCurrentMessageBoxText();
    virtual void SetTareListToCurrentLanguage(void);    //set the tare list to the current language's tares
    virtual void SetCMReceiptToCurrentLanguage(void);   //set CM receipt to the current language

    virtual long AddToReport(const CString& text, const CString& csReceiptSrc);           // add a text string to report
    virtual void ResetReport(const CString& csReceiptSrc);            // reset report
    virtual void HideReport(const CString& csReceipt);            // hide report
    virtual void ShowReport(const CString& csReceipt);            // show report

    virtual HRESULT StartRemoteServer();
    virtual CString GetRemoteServerName();
    virtual BOOL RemoteMode();
    //+SSCOP-3531
    virtual CString GetCurrentContext();

    //--------------------------------------------------------------------
    // Store Mode screen methods.
    //--------------------------------------------------------------------
    virtual long SMShowItemReceipt(const bool); 
    virtual long SMShowRestrictedItemReceipt(const bool);
    virtual long RestrictedItemNotAllowed(int,  long&);
    virtual long GetCurrentItemAutoApprovalFlag(bool&);
    virtual long SetCurrentItemAutoApprovalFlag(const bool);
    virtual bool IsThisItemVoided(const CString& strItemId, bool& bValue);
    virtual long SMShowVoidedItemReceipt(const bool);

    virtual long SMShowCouponReceipt(const bool);
    virtual long SMShowVisualItemReceipt(const bool);
    virtual long SMShowSecurityLog(void);
    virtual long SMShowMediaList(void);
    virtual long SMShowSMMItemList(void);
    virtual long SMInfo(LPCTSTR);
    virtual int  ageFromDateTime(COleDateTime& date); //convert date into an age TAR155582 LPM102600
    virtual CString DateFormat(void); // TAR155582
    virtual CString DateFormat(int month, int day, int year);
    virtual long DeleteRewardItem(long lRewardID);
    virtual long AddRewardItem(const CString& csReceiptSrc, CString& csDesc, long lRewardID, long lValue, long lPosType, long lEntryIdForReference);
    virtual long FindRctIndexFromRewardID(long lRewardID, CString& csItemEntryId);
    virtual long FindNextRctIndexFromRewardID(long lRewardID, CString& csItemEntryId);
    virtual long FindRctIndexForReward(long lRefEntryID, long lPosType, CString& csItemEntryId);
    virtual long InsertRewardItem(long nIndex, CString& csDesc, long RewardID, long lValue, long lEmphasis);
    virtual long LoadTenderList(void);
    virtual long AddRewardItem2(CString& csDesc, long lRewardID, long lValue, long lEmphasis); //add reward to small receipt
    virtual long AddMessageToReceipt2(CString, const bool hilite = false); //add message to small receipt   

    virtual PSXRC SetSelection(const CString& csReceipt, const CString& csItemEntryId);
    virtual CString GetSelection(const CString& csReceipt);
    virtual bool CheckForScrolling(const CString& csReceiptSrc, const CString& csReceipt, const CString& csScrollMode);
    virtual long GetControlHandle(HWND& hControlWnd);
    virtual void ShowMoreInfoOnReceipt(const CString& csReceipt, const CString& csControl);
    virtual void SetModeDisplay(bool bShowModeDisplay);
    virtual long LoadLanguageList(void);

    virtual void CreateVolCtlThread(bool bSetInitial=true);
    // -TAR 351654
    //+ Personalization picklist
    virtual long LoadPersonalizedPopularList(const CString& csUPCList);
    virtual void AddItemToList(const CString csItemSent, CStringArray& csPersonalizedPicklist);
    virtual void UpdatePicklistFavoritesMetrics(CStringArray& csPicklistItemSold, CStringArray& csQuickPickItemSold);
    virtual void UpdateFailedDisplayPersonalPicklistItems(const CString& csUPCList);
    virtual void UpdatePicklistSoldItemData(const CStringArray& csPicklistItemSoldInfo, CStringArray& csQuickPickItemSold);
    //- Personalization picklist
    //+ Personalization age prompt
    virtual void SetInputText(LPCTSTR lpszInput);
    //- Personalization age prompt
    //+ Dynamic Quickpick
    virtual long LoadPersonalizedQuickPickList(const CString& csUPCList);
    virtual void UpdateQuickPickSoldItem(const CStringArray& csQuickPickItemSoldList, CStringArray& csPersonalizedPicklistItems);
      //- Dynamic Quickpicklong AssignAMTareList();
    
    //TAR 196908 end
    virtual long ExternalUIRequestExec(PS_UI_CMD pUIRequest);
    virtual long AddReceiptItemFromUIRequest(const CString& csReceiptSrc, CString csRewardItemId, CString csEntryIdForReference);
    virtual long FindReceiptItemWithMultipleCriteria(const CString& csReceiptName, S_UI_FindItem&  sUIFindCriteria);
    virtual long ProcessPIMReadReceiptVars(PS_UI_CMD ptrUIRequest);
    virtual long ProcessPIMItemInsert(PS_UI_CMD ptrUIRequest);
    virtual long ProcessPIMItemUpdate(PS_UI_CMD ptrUIRequest);
    virtual long ProcessPIMRemoveVars(PS_UI_CMD ptrUIRequest);
    virtual long ProcessPIMTransactionVars(PS_UI_CMD ptrUIRequest);
    virtual long ProcessPIMSelectItem(PS_UI_CMD ptrUIRequest);
    virtual long ProcessPIMFindItem(PS_UI_CMD ptrUIRequest);
    virtual long ProcessPIMScroll(PS_UI_CMD ptrUIRequest);
    virtual long ProcessPIMContext(PS_UI_CMD ptrUIRequest);
    virtual long ProcessPIMSendEvent(PS_UI_CMD ptrUIRequest);
    virtual void TraceItemVariables(CString csReceiptSrc, CString csItemEntryId);

    virtual void RegisterPsxEventHandler( void * EventHandler );

    virtual void UnRegisterPsxEventHandler( void * EventHandler );

    virtual void AddPsxClickEventMapping( LPCTSTR KeyName, 
                                                PSEVT evtType,
                                                long  wParam,
                                                bool  bUseWParam,
                                                long  lParam, 
                                                bool  bUseLParam,
                                                void  *pMappingFxn );

    virtual bool RemovePsxClickEventMapping( LPCTSTR KeyName );

	virtual long PSSendTBCommand(  long lCommandID, ...);
	void SetInput( CString csText, bool bHide );

protected:

    virtual long AddItemToReceipt(const CString& csReceipt = _T(""));   // add an item to a receipt list
    virtual long VoidItemFromReceipt(const bool, const CString& csReceipt = _T("")); //void an item from receipt                       // ...add if not found
    virtual long LoadPSText(LPCTSTR,const bool); // load message texts
    virtual long LoadTareInfo(LPCTSTR = NULL);  // load tare info from 4690

    // speciality window specific procedures, update both the CM & SM frames & windows
    virtual long Background(                // change the background
       const BMPS,                        // ...bmp number
       const int);                        // ...1 - store frame, 2 - Assist frame, other cust frame

	CString GetHookProperty(CString csProp);
    CString GetTBProperty(CString csProp);
    void SetHookProperty(CString csProp, CString csValue = _T(""));
    void SetTBProperty(CString csProp, CString csValue = _T(""));
	CString GetMSGItemReceiptIdByDesc(CString receiptSrc, CString itemDesc);
	long RemovemPerksFromEReceipt(CString csReceiptSrc);

private:
#ifdef _TESTSSF
    void PutToSafeArray(int nIndex, COleSafeArray& saArray, CString csString);
#endif


};

#endif

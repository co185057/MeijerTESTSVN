/////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  PSProceduresBase.H
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
// 081502: Tar 212025 CHu/FFR- Make function virtual so that they can overidden in SSF
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PSPROCEDURESBASE
#define _PSPROCEDURESBASE

#include "IPSProcedures.h"

#include "PSProceduresConstants.h"

#include <map> // SR804

// events from PS callback events for MO.PostPS(), coded in PSInterface()
#include "Library.h"

#ifndef _CPPUNIT
#include "OverrideScotStringList.h"
#include "ConfigObject.h"    
#include "PSEvt.h"

#ifdef _SCOTSSF_
#include "SCUDefs.h"        // include SCOT Customer defined string constant ids if it is SCOTSSF
#endif  // _SCOTSSF_

#include "PSXMfc.h"
#else
#include "FakePSXMFC.h"
#include "FakeVolumeControl.h"
#include "TestMacros.h"
#endif  // _CPPUNIT
#include "PSItemList.h"

template <class TYPE, class ARG_TYPE = const TYPE&> class CSortedArray : public CArray<TYPE,ARG_TYPE>
{
public:
   BOOL InsertBinary( TYPE value, BOOL doubles )
    {
        if( ( GetSize() ) < 100 ) // With small arrays bubble sort is faster
        {
            SortedInsert( value, doubles );
            return 0;
        }
        
        int bottom = 0, middle = 0, top = GetSize() - 1, nCompare = 0;
        while( top  > bottom )
        {
            middle = ( bottom + top ) / 2;
            nCompare = Compare( GetAt( middle ), value );
            if( -1 == nCompare ) // Upper half
            {
                if( 1 == Compare( GetAt( middle + 1 ), value ) )
                {   
                    InsertAt( middle + 1, value );
                    return FALSE;
                }
                else if( top - 1 == bottom ) 
                {
                    Add( value );
                    return FALSE;
                }
                bottom = middle;
            }
            else if( 1 == nCompare ) // Lower half
            {
                if( middle == 0 )
                {
                    InsertAt( middle, value );
                    return FALSE;
                }
                top = middle;
            }
            else                                    
            {
                if( doubles )
                {
                    InsertAt( middle, value );
                }
                return TRUE;
            }
        }       
        return FALSE;
    }   

    BOOL FindBinary(TYPE elem, int nStart, int nEnd)
    {
        //trace(L6, _T("In Binary Search. nStart is %d, nEnd is %d"), nStart, nEnd);
        if(nStart<0)
           nStart=0;

        if(nEnd>GetSize()-1)
           nEnd=GetSize()-1;
        
        if(nStart>nEnd)
           return FALSE;

        int nMid=(nStart+nEnd)/2;

        int nResult=Compare(GetAt(nMid), elem);
        
        if(nResult==0)
           return TRUE;
        else if (nResult>0)
           return FindBinary(elem, nStart, nMid-1);
        else
           return FindBinary(elem, nMid+1, nEnd);

    }
    

private:
    virtual int Compare( TYPE elem1, TYPE elem2 )
    {
        return elem1.Compare(elem2);
    }

    BOOL SortedInsert( TYPE elem, BOOL doubles )
    {   
        if( !( GetSize() ) )
        {
            Add( elem );
            return FALSE;
        }
        int nCompare = 0;
        for( int i = 0; i < GetSize(); i++ )
        {
            nCompare = Compare( GetAt( i ), elem );
            if( 0 == nCompare )
            {
                if( doubles )
                {
                    InsertAt( i, elem );
                }
                return TRUE;
            }
            if( 1 == nCompare )
            {
                InsertAt( i, elem );
                return FALSE;
            }
        }
        Add( elem );
        return FALSE;
    }
};    

class IPSStringManager;
#ifndef _CPPUNIT
class CVolumeControl;                   // TAR 351654

//+SR815 - Personalization
typedef struct
{
    bool bPrimary;     // Is context a "primary language" context?
    CString csContext; // Context name
    CString csControl; // Control name
    PSTEXT stringID;   // String ID
} STRINGID_CONTROL;
typedef CArray<STRINGID_CONTROL*, STRINGID_CONTROL*> STRINGID_CONTROL_LIST;
//-SR815 - Personalization
#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
PSProceduresBase : public CObject, public IPSProcedures
{
#ifndef _CPPUNIT
//USSF Start
    public:
        virtual void SetUSSFFrame(LPCTSTR lpszContext){m_csCurrentContext = lpszContext;};
//USSF End      

    protected:
        CString mItemDescBuffer;    // used to store the description of Item Selection: see TAR 273325 BMediano
#endif // _CPPUNIT

  //--------------------------------------------------------------------
  // Public.
  //--------------------------------------------------------------------
public:
    // ++++ METHODS STARTS HERE ++++
    long ConvertSCOTLangID2LCID(const CString& csISOCode, CString& csLCIDCode);
    virtual int DisplaySelectedItem(CString csFirstchar="", bool bIsItemDescription=false, bool bFavoritesButton=false);
    void EnableProduceScrollButtons(); // TODO: refactor this methode, unused.
    CString GetKeyboardContextOfCurrentLang();
    CString AppendContextWithLCID(CString csContext);
    CString GetOperatorKeyboardContextOfCurrentLang();
    virtual SCOT_LANGUAGE_TYPE GetLanguage(void);
    CString GetLCID(SCOT_LANGUAGE_TYPE lang);
    virtual CString GetPSSavedText(const int nMsg);  
    virtual CString GetPSText(const CString csMsg, SCOT_LANGUAGE_TYPE language);
    virtual CString GetPSText(const int nMsg);   // return a pointer to the PS text string
    virtual CString GetPSText(const int nMsg, SCOT_LANGUAGE_TYPE language);   // return a pointer to the PS text string in the given language
	virtual long LoadPicklistAssistItemList(const CString& csPLAItemList);
    virtual void SetLanguage(SCOT_LANGUAGE_TYPE);
    void SetLocale(SCOT_LANGUAGE_TYPE lang);
    void GetFuzzyChars(const TCHAR c, CString & csFussyChars);
    virtual void SetCategoryTitles(const CString& csCategory);
    virtual CString GetCategory(const CString& csCategories, const CString& csCategory);
    virtual bool FindCategory(const CString& csCategory, CString& csRetCategory, CString& csCategoryId, int& nTabIndex);
    CPSX *GetPSXObject();
    CString GetLocaleInfoString(LCID lcid);
	virtual void ClearPicklistAssistItemList(void);
	virtual bool IsPicklistAssistItemListEmpty(void);
    virtual bool IsPickListItem(const CString& csItemCode);
    virtual bool IsInXM();               
    virtual void SetInXM(bool bInXM);    

    virtual CString GetcsTenderDescription();
    virtual void SetcsTenderDescription(CString);
	//VSTS-267145
	virtual CString GetLastItemReceiptEntryId();
	//VSTS-267145

    // ++++ MEMBERS STARTS HERE ++++
    SCOT_LANGUAGE_TYPE m_languageUsed;
    bool m_bInXM;                        
    CString m_csTenderDescription; //sscoadk-3121

	//virtual long LoadPicklistAssistItemList(const CString& csPLAItemList);
      virtual void EchoAmountPaidToReceipt(int nTenderType, long lAmountPaid); // SR804
      virtual void PushLevels(const CString& csPreviousLevel, const CString& csCurrentLevel);
      virtual void PopLevels(CString& csPreviousLevel, CString& csCurrentLevel);
      virtual void DeleteAllLevels(void);   
      virtual bool IsLevelEmpty(void);
      virtual CString GetCategoryName(CString csCatID);
#ifndef _CPPUNIT
      virtual bool CanScroll(const long lScrol);
      virtual CString GetScreenTitle();
      virtual CString GetInstructions();
      virtual void DisplayNextCategoryItem(const long lScroll);
      virtual void DisplaySelectedSubItem(CString csSubCategory);
      virtual void DisplayQuickPickItem();
      virtual CString GetLastProduceLookupList() { return m_strLastProduceLookupList;};
#endif // _CPPUNIT
      virtual void SetLastProduceLookupList(CString sCategory) { m_strLastProduceLookupList=sCategory;};
      virtual long GetPicklistItemsSold();
#ifndef _CPPUNIT
      virtual void SetPicklistItemsSold(long lTotalPicklistItemsSold);
      virtual long GetPicklistItemsIncludeVoidItems();
      virtual void SetPicklistItemsIncludeVoidItems(long lTotalPicklistItemsIncludeVoidItems);

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
#endif
      virtual void GetParentCategories(const CString& csCategories, CStringArray& csParents);
      virtual bool HasSubCategory(CString csMainCategory, CString &csFirstSubCategoryID);
      virtual long GetTabSelectedIndexofSubCategory(CString csMainCategoryID, CString csSubCategoryID);
      virtual bool FindSubCategory(const CString& csSubCategory);
      virtual void SetSubCategorySelectedButton(long lIndex);
      virtual CString GetFirstLevelCategoryID(void);
      virtual CString GetSecondLevelCategoryID(void);
      virtual CString GetProduceLookupCategoryLevels(void);
      virtual void SetProduceLookupCategoryNumber(CString csIndexOfCategory);
      virtual void SetDisplayTabIsMainCategories(bool b);
      virtual bool GetDisplayTabIsMainCategories();
#ifndef _CPPUNIT
      void SetServerName(LPCTSTR serverName);
      bool GetPickListZeroWeightValue( const CString& csItemCode ); //Japan RFC#1 (RFQ 893)
      virtual BOOL DisconnectFromServer(BOOL bSendDisconnectMsg);
      virtual BOOL ConnectToServer(int iFocus, int iDisplay);

      CStringArray* GetPickQtyList();
     
     CString GetISOLanguageCode(CString csLCID); // SR814.5 SR815.6 SR851 Personalization and Security
#endif //_CPPUNIT
     virtual long LoadProdList(const CString& csXML_List = _T("PickList.xml"));

     void SetFoundPickListItemsFromSearch(bool b);
     bool GetFoundPickListItemsFromSearch();

#ifndef _CPPUNIT

      long SetCategoryListText(const CString& csCtlName, const CString& csItemCode);

      //071904: TAR 273325 BMediano - BEGIN
      void SetItemDesc( CString csitemDesc );
      CString GetItemDesc();
      //071904: TAR 273325 BMediano - END

      //SR672 - Enhanced device error display
      virtual void SetDevErrURL(LPCTSTR lpszURL);

#endif //_CPPUNIT
      bool UseCustomerLanguageForLoadString() { return m_bUseCustomerLanguageForLoadString; }
      void UseCustomerLanguageForLoadString(bool b) { m_bUseCustomerLanguageForLoadString = b; }
      

      PSProceduresBase();
      ~PSProceduresBase();
#ifndef _CPPUNIT
      virtual long ShowTrainingModeOnScreen(bool bShowTrainingMode);
#endif //_CPPUNIT
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
#ifndef _CPPUNIT
      virtual long UnInitialize(void);          // do all un-initialization
#endif //_CPPUNIT
      virtual long UnInitializeConfigData(void);
#ifndef _CPPUNIT

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
#endif
      virtual void SetFrame(LPCTSTR lpszContext);
      virtual void SetCMFrame(LPCTSTR lpszContext);
#ifndef _CPPUNIT
      virtual void SetCurrentFrame(LPCTSTR lpszContext);
      //+SR 815 - Personalization
      STRINGID_CONTROL_LIST controlsForLangSwitch;
      CArray<CString, CString> listButtonLists;
      virtual void BuildControlsList();
      //-SR 815 - Personalization

        void SetToRemoteDisplay(const long lDisplay);
        CString ConvertToEscapeSequence(LPCTSTR lpszText, LPCTSTR lpszString, LPCTSTR lpszEscSeq);
      virtual void SetBackground(const BMPS); // just the BMP number
      virtual long SetLeadthruText(const PSTEXT nText1, const PSTEXT nText2 = PS_BLANK); // leadthru text string
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
#endif
      virtual void ShowChangeDueAmountPaid( const long,     // Change due
                                    const long,                  // Amount paid
                                    bool bChangeDueText = true);   
#ifndef _CPPUNIT
      virtual void ShowAmountRefundedAmountPaid( const long,  // Amount refunded
                                         const long); // Amount paid
      virtual void UpdateAmountDueAmountPaid( const long,     // Amount due    
                                      const long);    // Amount paid
      virtual void ShowTotalSavings(const long lTotalTrxSavings);
      virtual void ShowTotalRewardPoints(const long lTotalTrxPoints);
      virtual void ShowCustomerRewardMessage(LPCTSTR pCustomerMessage);
#endif //_CPPUNIT
      virtual void ShowCMRewardInfoBox(const long lTotalTrxSavings = LONG_MIN, const long lTotalTrxPoints = LONG_MIN,
                                    LPCTSTR pCustomerMessage = _T("DefaultIsDoNotResetMessage"));
      virtual void ShowSavingsORRewards(bool bShowSavings, long lValue);
#ifndef _CPPUNIT
      virtual void ShowSMTotal(const bool fTotal);
#endif //_CPPUNIT
      virtual long ShowFrame( bool bHelpState = true); //add a paramer for Help button display - improve screen display
#ifndef _CPPUNIT
      virtual void SetCMMascotImage(const BMPS);
      virtual CString GetMascotBmpName(const int nMsg); // return a pointer to the PS text string
      virtual CString GetSoundName(const int nMsg); // return a pointer to the PS text string
//#endif //_CPPUNIT
      virtual void SetCMGobackButtonVisible(bool); 
//#ifndef _CPPUNIT
      virtual CString Background(void); // returns the name of the last BMP used
      virtual void ScanReceiptForExceptionCategorys(CStringArray &csArray); 
      virtual bool ScanReceiptForSecurityTagItems(void);
        
        virtual long ItemsInFavoritesList(void);
        virtual void ReadPImgFiles(CStringArray *, CStringArray *, CStringArray *);
        //Begin Tar 210315, make it virtual for SSF to override
        virtual void GetNewDescriptions( const CString&, const CString&, CStringArray&, CStringArray& );
        //End Tar 210315
#endif //_CPPUNIT
        virtual void GetDeviceSpecificBMPFileNames(void); 
#ifndef _CPPUNIT
        virtual void CheckIfBMPFileExists(CString *);
        virtual bool SetDynamicBackground(CString *);
     
#endif //_CPPUNIT

		virtual CString FmtDollar(const long, SCREEN_POSITION sp = SP_OTHER, FORMAT_OPTION bReporting = DEFAULT);  // format cents into a $0.00 string
      // EndCodeLocale


      virtual CString FmtWeight(const long);     // format weight to string 0.00 lb or 0.000 kg
#ifndef _CPPUNIT
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
//#endif //_CPPUNIT
      virtual long SecurityMessageNonmodal(LPCTSTR); 
      virtual long SecurityMessageModal(LPCTSTR);   
//#ifndef _CPPUNIT
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
#endif //_CPPUNIT
      virtual long ScrollUp(const CString& csReceipt = _T(""));    // scroll this receipt up
      virtual long ScrollDown(const CString& csReceipt = _T(""));  // scroll this receipt down
#ifndef _CPPUNIT
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
//#endif //_CPPUNIT
      virtual long ShowNumericKeypad(void);
//#ifndef _CPPUNIT
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
#endif //_CPPUNIT
      virtual long GetInput(CString &);       // get the current input echoed data, returns length
#ifndef _CPPUNIT
        
      virtual CString GetBmpName(const int nBmp);
      virtual long SetPSText(const WORKMSGS,LPCTSTR);   // set a PS_WORKn pointer to a text string
#endif //_CPPUNIT
      virtual void SetRemoteLanguage(const CString &csISOLanguageCode);
#ifndef _CPPUNIT
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
      virtual CString GetRemoteServerName() {return m_csServerName;}  
#endif      
      virtual BOOL RemoteMode();

      //+SSCOP-3531
      virtual CString GetCurrentContext();
#ifndef _CPPUNIT
      inline CString GetContextWithKeyPressed() {return m_csCurrentContext;}
      inline void SetContextWithKeyPressed() {
          m_csCurrentContext = GetCurrentContext() + _T("_KeyPressed"); 
      }
      //-SSCOP-3531
      inline void SetWeightUnits(const CString &wu) { csScaleUnits = wu; }

      //--------------------------------------------------------------------
      // Store Mode screen methods.
      //--------------------------------------------------------------------
      virtual long SMShowItemReceipt(const bool); 
      virtual long SMShowRestrictedItemReceipt(const bool);
      virtual long RestrictedItemNotAllowed(int,  long&);
      virtual long GetCurrentItemAutoApprovalFlag(bool&);
#endif
      virtual long SetCurrentItemAutoApprovalFlag(const bool);
      virtual void SetCurrentItemAsMobileItem(const bool bMobileItem);
      virtual bool GetCurrentItemMobileItemFlag();
#ifndef _CPPUNIT
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
#endif //_CPPUNIT
      virtual long LoadTenderList(void);
#ifndef _CPPUNIT
      virtual long AddRewardItem2(CString& csDesc, long lRewardID, long lValue, long lEmphasis); //add reward to small receipt
      virtual long AddMessageToReceipt2(CString, const bool hilite = false); //add message to small receipt   

      virtual PSXRC SetSelection(const CString& csReceipt, const CString& csItemEntryId);
#endif //_CPPUNIT
      virtual CString GetSelection(const CString& csReceipt);
#ifndef _CPPUNIT
      virtual bool CheckForScrolling(const CString& csReceiptSrc, const CString& csReceipt, const CString& csScrollMode);
#endif //_CPPUNIT
      virtual long GetControlHandle(HWND& hControlWnd);
      virtual void ShowMoreInfoOnReceipt(const CString& csReceipt, const CString& csControl);
#ifndef _CPPUNIT
      virtual void SetModeDisplay(bool bShowModeDisplay);
#endif //_CPPUNIT
      virtual long LoadLanguageList(void);
#ifndef _CPPUNIT

    // +TAR 351654
    /**
     * \return Event that controls when the default volume level is set.
     */ 
    HANDLE GetVolEventHandle(void) { return m_hWaitVolSet; }  

    /**
     * Creates a thread that will set the volume level to the default value
     * when the m_hWaitVolSet event is signaled.
     * \param[in] bSetInitial - If true, the volume level will be set to the 
     *                    default level when the thread is created.
     *                    If false, then the level will not be set until the
     *                    m_hWaitVolSet event is signaled.  (Note that setting
     *                    the default volume level also sets the logo shown
     *                    on the volume control button.)
     *
     * \note - Added for TAR 351654
     */
     virtual void CreateVolCtlThread(bool bSetInitial=true);
     // -TAR 351654

      DWORD dwtime;                     //TAR198849
#endif
      BOOL m_bFirstTimeNextButtonEnabled;
#ifndef _CPPUNIT
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
      //- Dynamic Quickpick
#endif //_CPPUNIT
	  virtual void LoadLanguageUIStrings(const int nLangID);
	  
    //--------------------------------------------------------------------
    // Protected.
    //--------------------------------------------------------------------
    protected:     
      IPSStringManager *m_pStrMgr;
      CString m_csCurrentContext;
      CString s_szProduceContextName;
      CString m_strLastProduceLookupList;
      CString m_ProduceLookupCategoryNumber;
      CString m_ProduceLookupCategoryLevels;
      CString m_FirstLevelCategoryID;
      CString m_SecondLevelCategoryID;
      CSItemList ItemList[SCOT_LANGUAGE_MAX];           //Pick item list
	  CSItemList m_PicklistAssistItemList[SCOT_LANGUAGE_MAX];
      CSItemList m_Search[SCOT_LANGUAGE_MAX];
      //+ Personalization picklist
      CSItemList m_StorePopularItemList[SCOT_LANGUAGE_MAX];    //Store popular item list
      CSItemList m_PersonalizedPopularItemList[SCOT_LANGUAGE_MAX];//contains Personalized popular item from server and Store popular item list
      CSItemList m_tempFavoritesFromServer[SCOT_LANGUAGE_MAX];//contains Personalized popular item from server
      //- Personalization picklist
      CPSX *m_pPSX;
      CString s_szProduceButtonListName ;
      bool m_bPsxInitialized;  
      CSCategoryList SubCategoryList[SCOT_LANGUAGE_MAX];   //Pick item subcategory list in primary language
      CSCategoryList MainCategoryList[SCOT_LANGUAGE_MAX];   //Pick item main category list in primary language
      CSCategoryList CategoryList[SCOT_LANGUAGE_MAX];   //Pick item category list in primary language
        CString s_szQuickPickButtonListName ;
      std::map<int, CString> m_TenderBtnMapByBtnNum;
      //+Japan RFC#1 (RFQ 893)
      CMapStringToBool m_mZeroWeightItems;
       //-Japan RFC#1 (RFQ 893)
      CMapStringToPickListItemInfo m_mPickListItemsMap;

      bool m_bFoundPickListItemsFromSearch;

      bool m_bDisplayTabIsMainCategories;
      int nNumLinkedItems; //tar 23376
//#ifndef _CPPUNIT
        CString s_szProduceScrollUpName ;
        CString s_szProduceScrollDownName ;
        CString s_szQuickPickContextName  ;
    
      HANDLE m_hWaitVolSet;                                     // TAR 351654
      CVolumeControl *m_pVolCtl;                                // TAR 351654

//#endif //_CPPUNIT
      bool m_bPsxConfigLoaded;   

#ifdef _CPPUNIT
    int nRegisterClassCalled;  //Only used in CPPUNIT tests.
#endif
            
      CString m_csSavedItemEntryId;
#ifndef _CPPUNIT
      CSItemList m_GraphicList;
#endif //_CPPUNIT
      long m_lMessageEntryId; 
      long m_lMediaListEntryId;    
      long m_lDetailsListEntryId;
      long m_lAMTareListEntryId;
      long m_lSmmEntryId;   
      long m_lReportEntryId;
      long m_lDelaySecurityEntryId;
#ifndef _CPPUNIT
      long AssignAssistKeyText(LPCTSTR szCtlName, int nIndex);
      long AssignTareList();
      long SetTopMostWindow();
      bool CALLBACK CallBack_Handler(
           const long,
           const WPARAM,
           const LPARAM);
#endif //_CPPUNIT
      virtual long AddItemToReceipt(const CString& csReceipt = _T(""));   // add an item to a receipt list
#ifndef _CPPUNIT
      virtual long VoidItemFromReceipt(const bool, const CString& csReceipt = _T("")); //void an item from receipt                       // ...add if not found
#endif //_CPPUNIT
      virtual long LoadPSText(LPCTSTR,const bool); // load message texts
#ifndef _CPPUNIT
      virtual long LoadTareInfo(LPCTSTR = NULL);  // load tare info from 4690
        
      // speciality window specific procedures, update both the CM & SM frames & windows
      virtual long Background(                // change the background
           const BMPS,                        // ...bmp number
           const int);                        // ...1 - store frame, 2 - Assist frame, other cust frame
#endif //_CPPUNIT
      int  nInputDigits;                // max digits allowed for EchoInput() func
      int  nMinInputDigits;             // minimum digits before EchoInput enables enter key
      _TCHAR szInputDigits[MAX_DIGITS+1];   // buffer for echoing input
#ifndef _CPPUNIT
      _TCHAR szInputAsterx[MAX_DIGITS+1];   // buffer for echoing input (hidden)
      bool fInputHide;                  // hide actual input, show as *'s
      bool fInputDollar;                // format input as a $0.00 amount

#endif //_CPPUNIT
      CString csScaleUnits;             // string for 'lb' or 'kg'

      CString szPSText[WORKMSGS_LAST - PS_MAXTEXT];
#ifndef _CPPUNIT
      CString csPSTare[SCOT_LANGUAGE_MAX][MAXTARE];        // List of Tare names
      CString csPSTareImage[SCOT_LANGUAGE_MAX][MAXTARE];   // List of Tare images
      int  nTareCount[SCOT_LANGUAGE_MAX];

      long lPSTareCode[SCOT_LANGUAGE_MAX][MAXTARE];        // List of tare codes corresponding to tare names in csPSTare[]
      long lTotal;                      // running total in cents
      long lTaxes;                      // running tax in cents
      long lPSAmountDue;
      long lPSPaid;
#endif //_CPPUNIT
      long lPSPointsTotal;  // total reward points for this trx
      long lPSSavingsTotal; // total savings for this trx
      CString csPSCustomerRewardMessage;
#ifndef _CPPUNIT
        CString m_csServerName; // Server Name for remote assist mode

      long lScaleWeight;                // last scale weight in 1/100 lb or 1/1000 kg
      PSTEXT nDelayedMessage12;         // for S&B or VOIDITEM screens
#endif //_CPPUNIT
      BMPS nLastBmp;
      CString m_lastMessageBoxText;
#ifndef _CPPUNIT
      bool m_bSaveMessage;
      bool m_bNeedDisplayWM; 
#endif

      //SR672 - Enhanced device error display
      CString m_csCurrentDevErrURL;
      CStringArray m_csPickQtyList;  //Pick quantity item code list
      CSItemList QuickPickItemList[SCOT_LANGUAGE_MAX];  //Quick Pick item list
      //+ Dynamic QuickPick
      CSItemList m_PersonalizedQuickPickItemList[SCOT_LANGUAGE_MAX];  //contains Personalized popular items from server and Store default Quick Pick item list
      //- Dynamic QuickPick 
#ifndef _CPPUNIT
      CString csBMPforFINISHPAYSWIPE;   // dynamic MSR background bitmap substitute for BMP_FINISHPAYSWIPE
      CString csBMPforCARDSWIPE;        // dynamic MSR background bitmap substitute for BMP_CARDSWIPE
      CString csBMPforPINENTRY;         // dynamic MSR background bitmap substitute for BMP_PINENTER_NOECHO
      CString csBMPforREQUESTSIGNATURE; // dynamic MSR background bitmap substitute for BMP_PINENTER_NOECHO

#endif //_CPPUNIT
      int currentFrameID;      
      long m_lModeType; //SR742

      bool m_bUseCustomerLanguageForLoadString;
      HWND m_SigCapWindow;
      bool m_bUsingPSXSigCapWindow;

      #include "PSLoadStrings.h"
//TAR 196908 start
    public:
    // ++++ METHODS STARTS HERE ++++

    // ++++ MEMBERS STARTS HERE ++++
        CString m_csScreenTitle;
        CString m_csInstructions;

#ifndef _CPPUNIT
        long AssignAMTareList();
        long FindRequiredAge(const long nCustomerAge);
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
#endif //_CPPUNIT
        virtual void TraceItemVariables(CString csReceiptSrc, CString csItemEntryId);
#ifndef _CPPUNIT
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
#endif // _CPPUNIT

        virtual void ResetTenderMap();
        long m_lTotalPicklistItemsSold; 

        long m_lTotalPicklistItemsIncludeVoidItems; //SR679 only count for all sold and void items

        typedef struct 
        {
            CString PreviousLevel;
            CString CurrentLevel;
            
        } LevelsRecord;
    
        PointerStack<LevelsRecord *> m_pLevelStack; // stack levels

        CString GetCustomerMessageDefault(); //SSCOB-1860


    protected:
        CStringArray m_HKSensitiveControls;
#ifndef _CPPUNIT
        //+SR828
        long GetTokenCount(const CString& csStr, const TCHAR cDelimiter);

        //-SR828
        void ParseTareInfo(CString csTareLine, int i, int nLanguage);
#endif //_CPPUNIT
};

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
CPSXRedrawLock
{
public:
    CPSXRedrawLock( CString csControl, CString csTraceID = _T(""));
    ~CPSXRedrawLock();
private:
    CString m_csControl;
    CString m_csTraceID;
};
  
//+TAR 371980 
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
CPSXUserInputLock
{
public:
    CPSXUserInputLock( CString csTraceID = _T("") );
    ~CPSXUserInputLock();
private:    
    CString m_csTraceID;
};  
//-TAR 371980
#endif


// DataEntry.h: interface for the CDataEntry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAENTRY_H__2190C3F4_E8C8_11D5_8D8F_00E029301EFC__INCLUDED_)
#define AFX_DATAENTRY_H__2190C3F4_E8C8_11D5_8D8F_00E029301EFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NamedStringContainer.h"
#ifdef _CPPUNIT
#include "TestMacros.h"
#include "FakeCommon.h"
#endif

#ifdef _SCOTSSF_
class DLLIMPORT
#elif defined _SCOT
class DLLEXPORT
#else
class 
#endif 
CDataEntry : public CNamedStringContainer
   {
   public:
    CDataEntry(CString sInit = _T(""));

      // Const accessor methods.
      CString GetButtonText(int iButton) const;
      CString GetButtonCommand(int iButton) const;
      int GetDefaultButton() const;
	  int GetDataNeededType() const;
      bool IsScannerInputEnabled() const;
	  bool IsMSRInputEnabled() const;   // RFQ 2605 - MSR data on Dataneeded screen
      bool IsStoreLogonEnabled() const;
      bool IsTotalHidden() const;
      bool IsHelpHidden() const;
      bool IsInputHidden() const;
      CString GetViewType() const;  // "Selection", "Numeric" or "Alpha"
      CString GetTopCaption() const;
      CString GetBottomCaption() const;
      CString GetInputMask() const;
      CString GetSummaryInstruction() const;
	  CString GetContextToDisplay() const;

      CString GetSummaryDescription() const;
      CString GetSecondaryButtonText() const;
      CString GetLaneStateOneText() const;
      CString GetLaneStateTwoText() const;
      CString GetLaneStateThreeText() const;
      CString GetSayPhrase() const;
      CString GetDetailedInstruction() const;
	  CString GetButtonFont() const;
	  CString GetRDNLaneMessage() const;  //RFQ 2605 - Message text for RapDataNeeded context
	  CString GetRDNLaneLeadthru() const; //RFQ 2605 - Leadthru text for RapDataNeeded context
      CString GetReportCategoryName() const;
      CString GetHandlingContext() const;	//SR831
	  
      int GetScannerDataFormat() const; //SR93.4 
      int GetInputLengthMin() const;
	  int GetButtonCount() const;         //RFQ 2605 - Count of regular buttons (8 max)
	  int GetCmdButtonListCount() const;  //RFQ 2605 - Count of command buttons
	  int GetCmdListEntryCount() const;   //RFQ 2605 - Count of cmdlist entries
  	  CString GetCmdButtonDef(int iButton) const;    //RFQ 2605 - Retrieve Definition of i-th command button
  	  CString GetCmdListEntry(int iEntry) const;     //RFQ 2605 - Retrieve Definition of specified CmdList entry
      CString GetDefaultCommandString() const;    //Default command string sent instead of displaying intervention.

    CString GetVideoPath();
    CString GetImagePath();
    CString GetDataRequester();
    CString GetDataHandler();

    virtual const int GetButtonID( const CString &sButtonLabel); //RFC375568 & RFC375577
    CString GetAutoRunResponse() const; //RFC375568 & RFC375577

    void SetVideoPath(CString);
    void SetImagePath(CString);
    void SetDataRequester(CString);
    void SetDataHandler(CString);
      void SetSummaryDescription(CString);
      void SetSecondaryButtonText(CString);
      void SetLaneStateOneText(CString);
      void SetLaneStateTwoText(CString);
      void SetLaneStateThreeText(CString);
	  void SetContextToDisplay(CString);
	  void SetRDNLaneMessage(CString);    //RFQ 2605 - Set message text for RapDataNeeded screen
	  void SetRDNLaneLeadthru(CString);   //RFQ 2605 - Set Leadthru text for RapDataNeeded Screen
      void SetReportCategoryName(CString);
      void SetDefaultCommandString(CString);

      virtual void Clear();
	  virtual int GetNextStringMatch(int iPosition, CString csPattern, CString &csStringName,  CString &csStringValue); //RFQ 2605 - get next matching value
	  bool IsSecurityEnable() const; //SR801

      int GetRAPLITEHandling() const;
      void SetRAPLITEHandling(int);
   protected:
      bool FromContents();
      

      CArray <CString, CString &> m_aButtons;
	  CArray <CString, CString &> m_cmdBtnList;   //RFQ 2605
	  CArray <CString, CString &> m_cmdList;
      CString m_sInputMask;
      CString m_sSummaryInstruction;
      CString m_sTopCaption;
      CString m_sBottomCaption;
      CString m_sViewType;
	  CString m_sContextToDisplay;

      CString m_sSummaryDescription;
      CString m_sButtonSecondaryText;
      CString m_sLaneStateOne;
      CString m_sLaneStateTwo;
      CString m_sLaneStateThree;
      CString m_sSayPhrase;
      CString m_sDetailedInstruction;
	  CString m_sButtonFont;
	  CString m_sAutoRunResponse;//RFC375568 & RFC375577
	  CString m_sRDNLaneMessage;   //Message displayed at lane for SM intervention
	  CString m_sRDNLaneLeadthru;
      CString m_sReportCategoryName;
      CString m_sHandlingContext;	//SR831
      CString m_sVideoPath;
      CString m_sImagePath;
      CString m_sDataRequester;
      CString m_sDataHandler;
      CString m_sDefaultCmdString;

      bool m_bScannerInputEnabled;
	  bool m_bMSRInputEnabled; //RFQ 2605
      int m_iDefaultButton;
	  int m_iDataNeededType;
      bool m_bStoreLogonEnabled;
      bool m_bHideTotal;
      bool m_bHideHelp;
	  bool m_bHideInput;
      int m_iInputLengthMin;
	  int m_nButtonCount; //RFQ 2605
	  int m_nCmdButtonListCount; //RFQ 2605
	  int m_nCmdListEntryCount; //RFQ 2605
	  bool m_bEnableSecurity; //SR801
      int m_nScannerDataFormat; //SR93.4

      int m_iRAPLITEHandling;
   };

#endif // !defined(AFX_DATAENTRY_H__2190C3F4_E8C8_11D5_8D8F_00E029301EFC__INCLUDED_)

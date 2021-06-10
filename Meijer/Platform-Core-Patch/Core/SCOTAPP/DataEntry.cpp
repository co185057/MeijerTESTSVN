// DataEntry.cpp: implementation of the CDataEntry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataEntry.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataEntry::CDataEntry(CString sInit):
   m_bScannerInputEnabled(false),
   m_iDefaultButton(-1),
   m_iDataNeededType(-1) // default (a negative #) indicates no view specified
   {
   if (!sInit.IsEmpty())
      FromString(sInit);
   }

// Populate and validate all fields from the contents string
bool CDataEntry::FromContents()
   {
   bool bStatus = false;
   CString sTemp, sStringName;
   CString csPattern, csStringName, csStringValue;  // RFQ 2605
   int iButton;

   int iPosition; // RFQ 2605

   Clear();

   m_nButtonCount = 0;  // RFQ 2605
   for (int iIndex = 0; iIndex < 8; iIndex++)
      {
      sStringName.Format(_T("Button%d"), iIndex);
      sTemp = ExtractString(sStringName);
      if (sTemp.Find(_T(",")) > 0)
         {
         m_nButtonCount++;
         }
      m_aButtons.SetAtGrow(iIndex, sTemp);
      }
// Begin RFQ 2605
   iPosition = 0;

   m_nCmdButtonListCount = 0;

   while ((iPosition = GetNextStringMatch(iPosition, _T("CmdButtonList"), csStringName,  csStringValue)) != -1)
   {
	   if (csStringValue.Find(_T(",")) >= 0)
	   {
           CString csIndex = csStringName.Mid(13);
           _stscanf(csIndex, _T("%d"), &iButton);
           if (iButton>0 && iButton > m_nCmdButtonListCount)
               m_nCmdButtonListCount = iButton;
		   m_cmdBtnList.SetAtGrow(iButton-1, csStringValue);
	   }
   }

   m_nCmdListEntryCount = 0;
   iPosition = 0;
   while ((iPosition = GetNextStringMatch(iPosition, _T("CmdListEntry"), csStringName,  csStringValue)) != -1)
   {
	   if (csStringValue.Find(_T(",")) > 0)
	   {
		   m_nCmdListEntryCount++;
		   m_cmdList.SetAtGrow(m_nCmdListEntryCount-1, csStringValue);
	   }
   } // End RFQ 2605

   m_sInputMask = ExtractString(_T("InputMask"));
   m_sTopCaption = ExtractString(_T("TopCaption"));
   m_sBottomCaption = ExtractString(_T("BottomCaption"));
   m_sSummaryInstruction = ExtractString(_T("SummaryInstruction"));
   m_sViewType = ExtractString(_T("ViewType"));
   m_sContextToDisplay = ExtractString(_T("ContextToDisplay"));

   m_sSummaryDescription = ExtractString(_T("Description"));
   m_sButtonSecondaryText = ExtractString(_T("LaneButtonText"));
   m_sLaneStateOne = ExtractString(_T("LSWND1"));
   m_sLaneStateTwo = ExtractString(_T("LSWND2"));
   m_sLaneStateThree = ExtractString(_T("LSWND3"));
   m_sSayPhrase = ExtractString(_T("SayPhrase"));
   m_sDetailedInstruction = ExtractString(_T("DetailedInstruction"));
   m_sButtonFont = ExtractString(_T("ButtonFont"));
   m_sRDNLaneMessage = ExtractString(_T("RDNLaneMessage"));  // RFQ 2605 - Message text for RapDataNeeded screen
   m_sRDNLaneLeadthru = ExtractString(_T("RDNLaneLeadthru")); // RFQ 2605  - Leadthru text for RapDataNeeded screen
   m_sReportCategoryName = ExtractString(_T("ReportCategoryName"));

   sTemp = ExtractString(_T("ScannerInputEnabled"));
   if (!sTemp.IsEmpty())
      {
      // Empty, "No, no, 0" will all set this to false -- other values are true.
      if (sTemp.FindOneOf(_T("0nN")) == -1)
         m_bScannerInputEnabled = true;
      }
   sTemp = ExtractString(_T("MSRInputEnabled"));  // Begin RFQ 2605 - MSR data on Dataneeded screen
   if (!sTemp.IsEmpty())
      {
      // Empty, "No, no, 0" will all set this to false -- other values are true.
      if (sTemp.FindOneOf(_T("0nN")) == -1)
         m_bMSRInputEnabled = true;
      } // End RFQ 2605
   sTemp = ExtractString(_T("DefaultButton"));
   if (!sTemp.IsEmpty())
      {
      m_iDefaultButton = _ttoi((LPCTSTR)sTemp);
      }

   sTemp = ExtractString(_T("DATANEEDEDTYPE"));
   if (!sTemp.IsEmpty())
   {
       m_iDataNeededType = _ttoi((LPCTSTR)sTemp);
   }

   sTemp = ExtractString(_T("INPUTLENGTHMIN"));
   if (!sTemp.IsEmpty())
   {
       m_iInputLengthMin = _ttoi((LPCTSTR)sTemp);
   }

   sTemp = ExtractString(_T("StoreLogon"));     // RFC 197844
   if (!sTemp.IsEmpty())
      {
      // "No, no, 0" will all set this to false -- other values are true.
      if (sTemp.FindOneOf(_T("1yY")) == -1)
         m_bStoreLogonEnabled = false;
      }

   sTemp = ExtractString(_T("HideTotal"));     // RFC 197844
   if (!sTemp.IsEmpty())
      {
      // Empty, "Yes, yes, 1" will all set this to true -- other values are false.
      if (sTemp.FindOneOf(_T("0nN")) == -1)
         m_bHideTotal = true;
      }

   sTemp = ExtractString(_T("HideHelp"));     // RFC 197844
   if (!sTemp.IsEmpty())
      {
      // Empty, "Yes, yes, 1" will all set this to true -- other values are false.
      if (sTemp.FindOneOf(_T("0nN")) == -1)
         m_bHideHelp = true;
      }

   sTemp = ExtractString(_T("HideInput"));     // Tar 230459
   if (!sTemp.IsEmpty())
      {
      // Empty, "Yes, yes, 1" will all set this to true -- other values are false.
      if (sTemp.FindOneOf(_T("0nN")) == -1)
         m_bHideInput = true;
      }

   //if (m_nButtonCount) /* Dataneededs not required to have a button */
   if (!m_sContents.IsEmpty())
      {
      bStatus = true;
      }
   return bStatus;
   }

// Begin RFQ 2605
int CDataEntry::GetNextStringMatch(int iPosition, CString csPattern, CString &csStringName,  CString &csStringValue)
{

   CString csName = _T("");
   CString csValue = _T("");
   bool bStatus = false;
   int nNewPosition = -1;

   csStringName=_T("");
   csStringValue=_T("");

   if (iPosition >= m_sContents.GetLength() || iPosition < 0)
   {
	   return -1;
   }

   csName=m_sContents.Mid(iPosition);
   csPattern.MakeUpper();
   int iFind = csName.Find(csPattern);

   if (iFind >= 0)
   {
	   nNewPosition += iPosition + iFind;
	   csName = csName.Mid(iFind);
	   iFind = csName.Find(_T("="));
	   ASSERT(iFind > 0);
	   if (iFind > 0)
	   {
		   nNewPosition += iFind;
		   csValue = csName.Mid(iFind + 1);
		   csName = csName.Mid(0,iFind);
		   iFind = csValue.Find(_T(";"));
		   //         ASSERT(iFind > 0);
		   if (iFind > 0)
		   {
			   nNewPosition += iFind;
			   csValue = csValue.Left(iFind);
		   }
		   
		   csStringName = csName;
		   csStringValue = csValue;
	   }

   
   }
   return nNewPosition;
} // End RFQ 2605


CString CDataEntry::GetButtonText(int iButton) const
   {
   CString sButtonText;
   if (iButton + 1 <= m_aButtons.GetSize())
      {
      sButtonText = m_aButtons[iButton];
      int iFind = sButtonText.Find(_T(","));
      if (iFind != -1)
         sButtonText = sButtonText.Mid(iFind + 1);
      }
   return sButtonText;
   }

CString CDataEntry::GetButtonCommand(int iButton) const
   {
   CString sButtonCommand;
   if (iButton + 1 <= m_aButtons.GetSize())
      {
      sButtonCommand = m_aButtons[iButton];
      int iFind = sButtonCommand.Find(_T(","));
      if (iFind != -1)
         sButtonCommand = sButtonCommand.Left(iFind);
      }
   return sButtonCommand;
   }

int CDataEntry::GetDefaultButton() const
   {
   return m_iDefaultButton;
   }

int CDataEntry::GetDataNeededType() const
{
   return m_iDataNeededType;
}

bool CDataEntry::IsScannerInputEnabled() const
   {
   return m_bScannerInputEnabled;
   }

bool CDataEntry::IsMSRInputEnabled() const                 // RFQ 2605
   {
   return m_bMSRInputEnabled;
   }

bool CDataEntry::IsStoreLogonEnabled() const
   {
   return m_bStoreLogonEnabled;
   }

bool CDataEntry::IsTotalHidden() const
   {
   return m_bHideTotal;
   }

bool CDataEntry::IsHelpHidden() const
   {
   return m_bHideHelp;
   }

bool CDataEntry::IsInputHidden() const
   {     // Tar 230459
   return m_bHideInput;
   }

CString CDataEntry::GetViewType() const
   {
   return m_sViewType;
   }  // "Selection", "Numeric" or "Alpha"

CString CDataEntry::GetTopCaption() const
   {
   return m_sTopCaption;
   }

CString CDataEntry::GetBottomCaption() const
   {
   return m_sBottomCaption;
   }

CString CDataEntry::GetInputMask() const
   {
   return m_sInputMask;
   }

CString CDataEntry::GetSummaryInstruction() const
   {
   return m_sSummaryInstruction;
   }
CString CDataEntry::GetContextToDisplay() const
   {
   return m_sContextToDisplay;
   }

CString CDataEntry::GetLaneStateOneText() const
{
   return m_sLaneStateOne;
}

CString CDataEntry::GetLaneStateTwoText() const
{
   return m_sLaneStateTwo;
}

CString CDataEntry::GetLaneStateThreeText() const
{
   return m_sLaneStateThree;
}

CString CDataEntry::GetSecondaryButtonText() const
{
	return m_sButtonSecondaryText;
}

CString CDataEntry::GetSummaryDescription() const
{
	return m_sSummaryDescription;
}

CString CDataEntry::GetSayPhrase() const
{
	return m_sSayPhrase;
}

CString CDataEntry::GetDetailedInstruction() const
   {
   return m_sDetailedInstruction;
   }

CString CDataEntry::GetButtonFont() const
   {
   return m_sButtonFont;
   }

int CDataEntry::GetInputLengthMin() const
{
   return m_iInputLengthMin;
}

int CDataEntry::GetButtonCount() const  // Begin RFQ 2605
{
	  return m_nButtonCount;
}

int CDataEntry::GetCmdButtonListCount() const
{
	  return m_nCmdButtonListCount;
}

int CDataEntry::GetCmdListEntryCount() const
{
	  return m_nCmdListEntryCount;
}

CString CDataEntry::GetRDNLaneMessage() const
{
	  return m_sRDNLaneMessage;
}

CString CDataEntry::GetRDNLaneLeadthru() const
{
	  return m_sRDNLaneLeadthru;
}
 
CString CDataEntry::GetReportCategoryName() const
{
      return m_sReportCategoryName;
}

//Returns the requested cmdButton definition.  The definition is 
// returned without parsing into text,data, and image information 
// to maintain flexibility.  The parsing will be done in the
// overrideable code.
CString CDataEntry::GetCmdButtonDef(int iButton) const
{
	CString sButtonDef = _T("");
	if (iButton + 1 <= m_cmdBtnList.GetSize())
	{
		sButtonDef = m_cmdBtnList[iButton];
	}
	
	return sButtonDef;
}

//Returns the requested cmdListEntry definition.  The definition is 
// returned without parsing to maintain flexibility.  
// The parsing will be done within the overrideable code.
CString CDataEntry::GetCmdListEntry(int iEntry) const
{
	CString sListEntry = _T("");
	if (iEntry + 1 <= m_cmdList.GetSize())
	{
		sListEntry = m_cmdList[iEntry];
	}
	return sListEntry;
} // End RFQ 2605

void CDataEntry::SetSummaryDescription(CString s)
   {
   InsertString(_T("Description"), s);
   }
void CDataEntry::SetContextToDisplay(CString s)
   {
   InsertString(_T("ContextToDisplay"), s);
   }

void CDataEntry::SetSecondaryButtonText(CString s)
   {
   InsertString(_T("LaneButtonText"), s);
   }

void CDataEntry::SetLaneStateOneText(CString s)
   {
   InsertString(_T("LSWND1"), s);
   }

void CDataEntry::SetLaneStateTwoText(CString s)
   {
   InsertString(_T("LSWND2"), s);
   }

void CDataEntry::SetLaneStateThreeText(CString s)
   {
   InsertString(_T("LSWND3"), s);
   }
void CDataEntry::SetRDNLaneMessage(CString s) // Begin RFQ 2605
   {
   InsertString(_T("RDNLaneMessage"), s);
   }
void CDataEntry::SetRDNLaneLeadthru(CString s)
   {
   InsertString(_T("RDNLaneLeadthru"), s);
   } 
void CDataEntry::SetReportCategoryName(CString s)
   {
   InsertString(_T("ReportCategoryName"), s);
   } // End RFQ 2605

void CDataEntry::Clear()
{
      m_aButtons.RemoveAll();
      m_aButtons.SetSize(8);    //Reduce memory reallocation for CArray
	  m_cmdBtnList.RemoveAll();
      m_cmdBtnList.SetSize(8,8);  //Reduce memory reallocation for CArray
	  m_cmdList.RemoveAll();
      m_cmdBtnList.SetSize(8,8);  //Reduce memory reallocation for CArray
      m_sInputMask;
      m_sSummaryInstruction;
      m_sTopCaption;
      m_sBottomCaption;
      m_sViewType;
	  m_sContextToDisplay;

      m_sSummaryDescription;
      m_sButtonSecondaryText;
      m_sLaneStateOne;
      m_sLaneStateTwo;
      m_sLaneStateThree;
      m_sSayPhrase;
      m_sDetailedInstruction;
	  m_sButtonFont;
	  m_sRDNLaneMessage;   //Message displayed at lane for SM intervention
	  m_sRDNLaneLeadthru;
      m_sReportCategoryName;

      m_bScannerInputEnabled = false;
	  m_bMSRInputEnabled = false;
      m_iDefaultButton = -1;
      m_iDataNeededType = -1;
      m_bStoreLogonEnabled = true;
      m_bHideTotal = false;
      m_bHideHelp = false;
	  m_bHideInput = false;
      m_iInputLengthMin = 1;
	  m_nButtonCount = 0;
	  m_nCmdButtonListCount = 0;
	  m_nCmdListEntryCount = 0;
}
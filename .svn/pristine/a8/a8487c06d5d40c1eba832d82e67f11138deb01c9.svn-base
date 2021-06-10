//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  PSLoadStrings.CPP
//
// TITLE: Load String Functions
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// @@@INFOPOINT
// AUTHOR:  Jason Neylon
// Added code to load in the mascot overlay image filenames file.
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#ifndef _CPPUNIT
#include "CommonServices.h"         // Common includes
#include "PSProcedures.h"           // Header
#include "common.h"             
#else
#include "PSProceduresBase.h"           // Header

#endif //_CPPUNIT
#define COMP_ID   ID_PS
#define T_ID      _T("PS")

#ifndef _CPPUNIT
CString PSProceduresBase::GetSCOTStrListFileExt(SCOTSTRLIST_TYPE iFileType)
{
	CString csFileExt;
    if (   (iFileType >= MSG_SCOTCORE_LANG_ONE)
        && (iFileType <= MSG_SCOTCORE_LANG_MAX)
        && (co.GetfStateDualLanguage()))
    {
        // Get the file extension for the appropriate language
        csFileExt = co.Getlanguage((SCOT_LANGUAGE_PRIMARY + (iFileType - MSG_SCOTCORE_LANG_ONE))).csMessageFile;
    }
    else if (   (iFileType >= MSG_SCOTUSERDEF_LANG_ONE)
             && (iFileType <= MSG_SCOTUSERDEF_LANG_MAX)
             && (co.GetfStateDualLanguage()))
    {
		csFileExt.Format(SCUDMSGFILE((SCOT_LANGUAGE_PRIMARY + (iFileType - MSG_SCOTUSERDEF_LANG_ONE))));
    }
	else if (   (iFileType >= MSG_SCOTTB_LANG_ONE) 
             && (iFileType <= MSG_SCOTTB_LANG_MAX)
             && (co.GetfStateDualLanguage()))
    {
        CString csLang = GetMSLocaleID( co.language[(SCOT_LANGUAGE_PRIMARY + (iFileType - MSG_SCOTTB_LANG_ONE))].szCode);
		csFileExt.Format(_T("TBMsg%s%s"), GetLanguageCode(csLang), _T(".dat"));
    }
    else
    {
	    switch(iFileType)
	    {
		    case BMP_SCOTCORE:
			    csFileExt = co.getBitmapFile() + _T(".dat");
		    break;
		    case BMP_SCOTUSERDEF:
			    csFileExt = SCUDBMPS _T(".dat");
		    break;
		    //@@@INFOPOINT
		    case BMP_SCOTMASCOT:
			    csFileExt = _T("SCOTMascot.dat");
		    break;
		    //@@@INFOPOINT
		    case WAVE_SCOTWAVE:
			    csFileExt = co.csDataWaveFile + _T(".dat");
		    break;
		    case MSG_SCOTCORE_LANG_ONE:
                // Dual language is caught in the if above
                ASSERT (!co.GetfStateDualLanguage());
       			csFileExt = co.csDataMessageFile + _T(".dat");
            break;
		    case MSG_SCOTUSERDEF_LANG_ONE:
                // Dual language is caught in the if above
                ASSERT (!co.GetfStateDualLanguage());
		        csFileExt = SCUDMSGS _T(".dat");
		    break;
			case MSG_SCOTTB_LANG_ONE:
				{
                // Dual language is caught in the if above
                ASSERT (!co.GetfStateDualLanguage());
                CString csLang = GetMSLocaleID(co.csPrimaryLanguageCode);
				csFileExt.Format(_T("TBMsg%s%s"), GetLanguageCode(csLang), _T(".dat"));
				}
   
            break;
		    case SCOTSTRLIST_LAST:
			    ASSERT(0);
		    break;
		    default:
		    break;
	    }
    }

	return csFileExt;
}
#endif //_CPPUNIT

BOOL PSProceduresBase::UnLoadSCOTStringConstants()
{
	int iListID = BMP_SCOTCORE;
	BOOL bRetStatus = true;
#ifndef _CPPUNIT

	for(; (bRetStatus && (iListID < SCOTSTRLIST_LAST)); ++iListID)
	{
		if (pSCOTStrTableList[iListID])
		{
			pSCOTStrTableList[iListID]->UnInitialize();
			delete pSCOTStrTableList[iListID];
		}
		pSCOTStrTableList[iListID] = NULL;
	}
#endif _CPPUNIT
	return bRetStatus;
}



BOOL PSProceduresBase::LoadSCOTStringConstants(CString csConfigPath)
{
	BOOL bRetStatus = true;
#ifndef _CPPUNIT
	int iListID = BMP_SCOTCORE;
	CFileFind FileSearchObj;
	

	for(; (bRetStatus && (iListID < SCOTSTRLIST_LAST)); ++iListID)
	{
		if (pSCOTStrTableList[iListID])
		{
			TRACE(_T("List: %d is already initialized\n"), iListID);
			//ASSERT(0);
			break;
		}
        
        if (   (iListID >= MSG_SCOTCORE_LANG_TWO)
            && (iListID <= MSG_SCOTCORE_LANG_MAX))
        {
            // If dual language isn't set and this isn't the primary language file, skip it
            if (   (!co.GetfStateDualLanguage())
                || ((iListID - MSG_SCOTCORE_LANG_ONE) >= co.GetNumberOfLanguagesSupported()))
            {
                continue;
            }
        }
        if (   (iListID >= MSG_SCOTUSERDEF_LANG_TWO)
            && (iListID <= MSG_SCOTUSERDEF_LANG_MAX))
        {
            // If dual language isn't set and this isn't the primary language file, skip it
            if (   (!co.GetfStateDualLanguage())
                || ((iListID - MSG_SCOTUSERDEF_LANG_ONE) >= co.GetNumberOfLanguagesSupported()))
            {
                continue;
            }
        }
		if (   (iListID >= MSG_SCOTTB_LANG_TWO) 
            && (iListID <= MSG_SCOTTB_LANG_MAX))
        {
            // If dual language isn't set and this isn't the primary language file, skip it
            if (   (!co.GetfStateDualLanguage())
                || ((iListID - MSG_SCOTTB_LANG_ONE) >= co.GetNumberOfLanguagesSupported()))
            {
                continue;
            }
        }

		pSCOTStrTableList[iListID] = NULL;
		SCOTSTRLIST_TYPE nListID = (SCOTSTRLIST_TYPE) iListID;

		// get file name to build a list of strings from the file
		CString csFileName = csConfigPath + _T('\\');
		csFileName += GetSCOTStrListFileExt((SCOTSTRLIST_TYPE) iListID);
		trace (L5, _T("Loading FileName for string list <%s>"), csFileName);

		
		if (FileSearchObj.FindFile(csFileName))	// make sure file exists
		{
			// build list from the file
			delete pSCOTStrTableList[iListID];
			pSCOTStrTableList[iListID] = (COverrideSCOTStringList *) new CSCOTStringList(csFileName, iListID);
			ASSERT(pSCOTStrTableList[iListID]);
		    bRetStatus = pSCOTStrTableList[iListID]->Initialize();
			
			if (bRetStatus)
			{
				int nSize = pSCOTStrTableList[iListID]->GetSize() - 1;	// 1 for oth element which is not used, SCOT indexing starts from 1
				TRACE(_T("SCOT built a list of %d strings from file :%s\n"), nSize, csFileName);
			}
		}
		else
		{
			// file doesn't exist to build a list
			CString csMsg;
			csMsg.Format(_T("SCOT could not open file: \"%s\"\n"), csFileName);
			TRACE(_T("%s"), csMsg);
			HRESULT rc = ScotError(INFO,SM_CAT_CONFIGURATION, SCOTAPP_PS_FILE, _T("%s"),csFileName);
			ASSERT(!rc);
		}
	}
	FileSearchObj.Close();
	 //Loading SCOTxxxx.000 and SCOTxxxx.xxx
    if (bRetStatus)
    {
		LoadSCOTStringConstantsOverrides(csConfigPath);
    }
#endif //_CPPUNIT
	return bRetStatus;
}
#ifndef _CPPUNIT
//Loading SCOTxxxx.000 and SCOTxxxx.xxx
//////////////////////////////////////////////////////////////////////////////
BOOL PSProceduresBase::LoadSCOTStringConstantsOverrides(CString &csConfigPath)
{
  BOOL bRetStatus = true;
  int iListID = BMP_SCOTCORE;
  CString csFileName, csExtension;

  trace (L2, _T("+LoadSCOTStringConstantsOverrides"));

	// cm150000 - Start FastLane 3.1 Modification for SwitchApp Display Terminal ID
	PutTerminalIDInReg(0);												// initialize terminalID in registry to 0
	int iTerminalNumber = TBGetTerminalNumber();
	if (iTerminalNumber != TB_E_NOTINITIALIZED)		// put Terminal ID value in registry only 
	{																							// if TB is initialized
		if(PutTerminalIDInReg(iTerminalNumber))
			trace (L5, _T("Terminal number available and placed in registry"));	// tell everybody it is ok
		else
			trace (L5, _T("Terminal number available but not placed in registry")); // report error
	}		
	else
		trace (L5, _T("Terminal number not available to put in registry"));	// report TB is not ready
	// cm150000 - End FastLane 3.1 Modification

  trace (L5, _T("Terminal number is <%d>"), iTerminalNumber);

  
  for(; iListID < SCOTSTRLIST_LAST; ++iListID)
    {  
        if (   (iListID >= MSG_SCOTCORE_LANG_TWO)
            && (iListID <= MSG_SCOTCORE_LANG_MAX))
        {
            // If dual language isn't set and this isn't the primary language file, skip it
            if (   (!co.GetfStateDualLanguage())
                || ((iListID - MSG_SCOTCORE_LANG_ONE) >= co.GetNumberOfLanguagesSupported()))
            {
                continue;
            }
        }
        if (   (iListID >= MSG_SCOTUSERDEF_LANG_TWO)
            && (iListID <= MSG_SCOTUSERDEF_LANG_MAX))
        {
            // If dual language isn't set and this isn't the primary language file, skip it
            if (   (!co.GetfStateDualLanguage())
                || ((iListID - MSG_SCOTUSERDEF_LANG_ONE) >= co.GetNumberOfLanguagesSupported()))
            {
                continue;
            }
        }
		if (   (iListID >= MSG_SCOTTB_LANG_TWO) 
            && (iListID <= MSG_SCOTTB_LANG_MAX))
        {
            // If dual language isn't set and this isn't the primary language file, skip it
            if (   (!co.GetfStateDualLanguage())
                || ((iListID - MSG_SCOTTB_LANG_ONE) >= co.GetNumberOfLanguagesSupported()))
            {
                continue;
            }
        }

        // get file name to build a list of strings from the file
        csFileName = GetSCOTStrListFileExt((SCOTSTRLIST_TYPE) iListID);
        trace (L5, _T("Loading FileName for string list <%s>"), csFileName);

        csExtension = csFileName.Right(4);
        csFileName = csFileName.Left(csFileName.GetLength() - 4);
        csExtension.MakeUpper();

        // Replace the ".DAT" with ".000" and ".xxx"
        // Make sure we do have a ".DAT"
        if (csExtension == _T(".DAT"))
        {
            bRetStatus = RefreshStringList(iListID, csConfigPath, csFileName, 0);
         //TAR 280684+
		if (iTerminalNumber > 0) 
	{
		// Replace the ".000" with ".xxx"
		bRetStatus = RefreshStringList(iListID, csConfigPath, csFileName, iTerminalNumber);
	}
	//TAR 280684-
        }
    }
  

  trace (L2, _T("-LoadSCOTStringConstantsOverrides <%d>"), bRetStatus);
  return bRetStatus;
}


//RefreshStringList
//Parameter: pass in an ID of table string list, config path, file name and 
// terminal number.
//Return true if overriding success otherwise.
//////////////////////////////////////////////////////////////////////////////
BOOL PSProceduresBase::RefreshStringList(int iListID,
                                         CString &csConfigPath,
                                         CString &csFileName,
                                         int iTerminal)
{

  trace (L2, _T("+RefreshStringList"));
  trace (L5, _T("    iListID        <%d>"), iListID);
  trace (L5, _T("    csConfigPath   <%s>"), csConfigPath);
  trace (L5, _T("    csFileName     <%s>"), csFileName);
  trace (L5, _T("    iTerminal      <%d>"), iTerminal);

  BOOL bRetStatus = true;
  CString csFileExt;
  TBRC nTBCallStatus;

  csFileExt.Format(_T("%s.%03d"),csFileName,iTerminal);
  nTBCallStatus = TBCopyFileFromServer(csFileExt,csConfigPath+_T("\\")+csFileExt,false);	

  // 11/2/2004 dtb Fix TAR280711
  // Load the files that may be present on the FastLane system.
  if (nTBCallStatus != TB_SUCCESS)
  {
	trace(L5, _T("Error returned from TBCopyFile <%d>"), nTBCallStatus);
	trace(L5, _T("     <%s>,    <%s>"), csFileExt, csConfigPath + _T("\\") + csFileExt);
  }

  if (pSCOTStrTableList[iListID])
  {
	//Get the file name from the table ID
	pSCOTStrTableList[iListID]->SetFileName(csFileExt);
	//Set the path of the file name
	CString tempName = csConfigPath+_T("\\")+csFileExt;
	//Get the table size
	int tablesize = pSCOTStrTableList[iListID]->GetSize();
	//Call Refreshlist to override the data from the file copy from the server
	bRetStatus = pSCOTStrTableList[iListID]->RefreshList(tempName, iListID,	tablesize);
  }
  else
  {
	// File wasn't loaded.  Come here to prevent crash
	trace(L5, _T("Error.  File not loaded "));
	trace(L5, _T("     <%s>,    <%s>"), csFileExt, csConfigPath + _T("\\") + csFileExt);
  }
  trace (L2, _T("-RefreshStringList <%d>"), bRetStatus);
  return bRetStatus;
}
//E 000 messages


PSProceduresBase::SCOTSTRLIST_TYPE PSProceduresBase::GetCurrentListPtr(int& nCurrentStringID, tStringType nMessageType)
{
	SCOTSTRLIST_TYPE iListID = SCOTSTRLIST_LAST;
	if (nMessageType == MSG_ID)
	{
		if (nCurrentStringID > TB_BASE) 
		{
			nCurrentStringID = (nCurrentStringID - TB_BASE);
            iListID = (SCOTSTRLIST_TYPE)(MSG_SCOTTB_LANG_ONE + m_languageUsed);
		}
		else if (nCurrentStringID > SCUD_BASE)
		{
			nCurrentStringID = (nCurrentStringID - SCUD_BASE);
            iListID = (SCOTSTRLIST_TYPE)(MSG_SCOTUSERDEF_LANG_ONE + m_languageUsed);
		}
        else
        {
            iListID = (SCOTSTRLIST_TYPE)(MSG_SCOTCORE_LANG_ONE + m_languageUsed);
        }
	}
	else if (nMessageType == BMP_ID)
	{
		if (nCurrentStringID > SCUD_BASE)
		{
			iListID = BMP_SCOTUSERDEF;
			nCurrentStringID = (nCurrentStringID - SCUD_BASE);
		}
		else
		{
			iListID = BMP_SCOTCORE;
		}
	}
	//@@@INFOPOINT
	//else if (nMessageType == MASCOT_BMP_ID)
	//{
	//	iListID = BMP_SCOTMASCOT;
	//}
	//@@@INFOPOINT
	else if (nMessageType == WAVE_ID)
	{
		iListID = WAVE_SCOTWAVE;
	}

	return iListID;
}


// returns an empty string if the required string is not found
CString PSProceduresBase::GetStringValueFromID(int nStringID, tStringType nMessageType)
{
	CString csValue;
	int nOriginalID = nStringID;

	ASSERT(nStringID);
	SCOTSTRLIST_TYPE iListID = (SCOTSTRLIST_TYPE) GetCurrentListPtr(nStringID,nMessageType);
	ASSERT(iListID != SCOTSTRLIST_LAST);
	COverrideSCOTStringList* pList   = pSCOTStrTableList[iListID];

	if(pList)
	{
		try
		{
			csValue = (*pList)[nStringID];
		}
		catch (...)
		{
			csValue = _T("");
			TRACE(_T("Referring an array with invalid index: %d\n"),nStringID);
			TRACE(_T("Index is expected to be defined from file:%s\n"), pList->GetFileName());
		}
	}
	else
	{
		//ASSERT(0);
		TRACE(_T("SCOT String does not exist for this: %d message ID\n"), nOriginalID);
	}
	if (!csValue.GetLength())
	{
		TRACE(_T("Missing string id %d"), nOriginalID);
	}
	return csValue;
}


int	PSProceduresBase::GetListSize(SCOTSTRLIST_TYPE iListID)
{
	COverrideSCOTStringList* pList = pSCOTStrTableList[iListID];
	int nSize = 0;
	if (pList)
	{
		nSize = pList->GetSize();
	}
	return nSize;
}


CString PSProceduresBase::GetFileNameFromID(int nStringID, tStringType nMessageType)
{
	SCOTSTRLIST_TYPE iListID = (SCOTSTRLIST_TYPE) GetCurrentListPtr(nStringID,nMessageType);
	ASSERT(iListID != SCOTSTRLIST_LAST);
    return GetSCOTStrListFileExt(iListID);
}

// cm150000 - Start FastLane 3.1 Modification for SwitchApp Display Terminal ID
/**************************************************************
Function:		PutTerminalIDInReg()
Author:			cm150000
Purpose:		Called after a successful get of the terminal ID
						number.  This function places the terminal ID 
						value in the registry for the Switch Ap to read 
						and display.  See lcSwitch.cpp in the Switch2
						Platform Project

						If the registry operations fail, false is 
						returned and an error is noted in traces.log.
						Otherwise, if success then true is returned.

						The only input is the termal ID from TB
**************************************************************/
bool PSProceduresBase::PutTerminalIDInReg(int nTerminalID)
{
	trace(L5, _T("+PutTerminalIDInReg"));
	HKEY hKey = NULL;
	long lResult = 0L;
	DWORD dwType = REG_DWORD;
	DWORD dwSize = sizeof(long);
	DWORD value = 0;

	value = nTerminalID;

	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\"), 0, KEY_ALL_ACCESS, &hKey);

	if (lResult != ERROR_SUCCESS)
	{
		trace(L5, _T("Error opening registry key to store terminal ID"));
		trace(L5, _T("-PutTerminalIDInReg (Error)"));
		return false;
	}
	
	lResult = RegSetValueEx(hKey, _T("TerminalIDNumber"), 0, REG_DWORD, (LPBYTE) &value, sizeof(DWORD));

	if (lResult != ERROR_SUCCESS)
	{
		trace(L5, _T("Error writing terminal ID to registry key"));
		trace(L5, _T("-PutTerminalIDInReg (Error)"));
		return false;
	}
	
	RegCloseKey(hKey);


	trace(L5, _T("-PutTerminalIDInReg"));
	return true;
}
// cm150000 - End FastLane 3.1 Modiciation

#endif //_CPPUNIT
CString PSProceduresBase::GetMSLocaleID(const CString& csLangCode)
{
    CString csLCID = csLangCode;
    if (csLangCode.CompareNoCase(_T("EN")) == 0)
        csLCID = _T("0409");
    else if (csLangCode.CompareNoCase(_T("UK")) == 0)
        csLCID = _T("0809");
    else if (csLangCode.CompareNoCase(_T("FR")) == 0)
        csLCID = _T("0C0C");
    else if (csLangCode.CompareNoCase(_T("ES")) == 0)
        csLCID = _T("080A");

    return csLCID;
}

CString PSProceduresBase::GetLanguageCode(const CString& csLangCode)
{
	CString csLCID = csLangCode;
	if (csLangCode.CompareNoCase(_T("EN")) == 0)
		csLCID = _T("0409");
	else if (csLangCode.CompareNoCase(_T("UK")) == 0)
		csLCID = _T("0809");
	else if (csLangCode.CompareNoCase(_T("FR")) == 0)
		csLCID = _T("0C0C");
	else if (csLangCode.CompareNoCase(_T("ES")) == 0)
		csLCID = _T("080A");

	return csLCID;
}

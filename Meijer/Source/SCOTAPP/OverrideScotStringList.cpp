// OverrideSCOTStringList.cpp: derive from the CSCOTStringList class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SCOTStringList.h"
#include "CommonServices.h"         // Common includes
#include "OverrideScotStringList.h"           // Header
#include "common.h"             

#define COMP_ID   ID_PS
#define T_ID      _T("OverrideScotStringList")


//This function does not assume the numbers in the file are sequential and instead
//of appending the data to the end of the list, it replaces existing data in the list.
//Parameter: pass in the whole path and file name, table ID, and table size.
//Return true if replacing success otherwise.
///////////////////////////////////////////////////////////////////////////////
  BOOL COverrideSCOTStringList::RefreshList(CString m_csFileName, int iFileID, int tablesize) 
{
    CString FileEntry;
    CStdioFile FileWrap;

	BOOL BuildStatus;
	BOOL bFileUnicode = IsFileUnicode(m_csFileName);
	trace(L6,_T("The file %s is Unicode? <%d>"), m_csFileName, bFileUnicode );

	if (!bFileUnicode)
		BuildStatus = FileWrap.Open(m_csFileName, CFile::modeRead | CFile::shareDenyWrite | CFile::typeText);
    else
		BuildStatus = FileWrap.Open(m_csFileName, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary);
	
	if (!BuildStatus) // open failed
    {
		return FALSE;
    }
    try
    {
      // read strings and add to the list
      while (FileWrap.ReadString(FileEntry))
      {
        int nEntryLen;
		if (!bFileUnicode)
			nEntryLen	= FileEntry.GetLength();
		else
			nEntryLen	= FileEntry.GetLength()-1;
		
		if (FileEntry.Left(1) == 0xFEFF)  //byte order mark for unicode file
			FileEntry = FileEntry.Mid(1);

        if (nEntryLen > 0 && (FileEntry[0] == _T('[')))
		{
				if (!bFileUnicode)
					SetListName(FileEntry);
				else
					SetListName(FileEntry.Mid(0, nEntryLen-1));
				continue;
		}
		if ((!nEntryLen) ||(FileEntry[0] == _T(';')) || ((FileEntry[0] == _T('/')) && (FileEntry[1] == _T('/'))))
        {
          // read next (uncommented) line
				  continue;
        }
		else
		{
				  CString  csStringValue = FileEntry;
				  int nStringIndexPos = FileEntry.Find(_T('='));
				  if (nStringIndexPos >= 0)
				  {
					  int nIndex = 0;
					  nStringIndexPos++;	// get string after '='
					  _stscanf((LPCTSTR) FileEntry,_T("%d="), &nIndex);
					  // extract right most (nEntryLen - nStringIndexPos)
					  if((nIndex > 0) && (nIndex < tablesize))  // check for index is out of bound or not
					  {
						//csStringValue  = FileEntry.Right(nEntryLen - nStringIndexPos);
						csStringValue  = FileEntry.Mid(nStringIndexPos,nEntryLen - nStringIndexPos);
						RemoveDoubleQuotes(csStringValue);				// remove double quotes at ends if exists
						//Create pcsStringValue and set it to the value from the new data file
						CString* pcsStringValue = new CString(csStringValue);

						// Get the original string from the table string list
						CString *pcsOldValue = GetAt(nIndex);	
						// delete the original string before replacing it	
						delete pcsOldValue;

						SetAt(nIndex, pcsStringValue);			// insert this valid string
						pcsStringValue = NULL;
					  }
					  else
					  {
						//Add an error message to trace.log and event log if the index is out of bound
						trace(L6, _T("RefreshList - Invalid index.  Index is %d.  The table size is %d."), nIndex, tablesize);
						ScotError(FATAL,SM_CAT_STARTUP_INIT, SCOTAPP_PS_INIT, _T("Invalid index.  Please check the bound in this file %s."), m_csFileName);
					  }
				  }
		}
      }
      FileWrap.Close();
    }
    catch(CFileException *e)
    {
      e->Delete();
      return FALSE;
    }
	return TRUE;	// successfully built the list
}




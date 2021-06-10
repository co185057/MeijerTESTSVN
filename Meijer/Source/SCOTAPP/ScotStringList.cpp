
// SCOTStringList.cpp: implementation of the CSCOTStringList class.   
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SCOTStringList.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int MAX_VALUELEN = 1024;	// size to store a string read from SCOT data files, used to declare a local array
const TCHAR szInvalidStr[20] = _T("INVALID_STRING");
#define		EMPTY_STRING  _T("")

CSCOTStringList::CSCOTStringList(LPCTSTR szFileName, int nListID)
{
	SetSize(500, 50);				// set string array to an initial size of 500, and from there grow by 50
	m_nListSize = 0;				// current list size is zero
	m_csFileName = szFileName;		// file name from which this string list has to be built.
	m_nListID	 = nListID;
}

CSCOTStringList::CSCOTStringList(int nListID)
{
	m_nListID = nListID;
	SetSize(500, 50);				// set string array to an initial size of 500, and from there grow by 50
	m_nListSize = 0;				// current list size is zero
}

CSCOTStringList::~CSCOTStringList()
{
	RemoveAll();
}



void CSCOTStringList::SetFileName(LPCTSTR szFileName)
{
	m_csFileName = szFileName;
}

CString CSCOTStringList::GetFileName(void)
{
	return m_csFileName;
}

BOOL CSCOTStringList::Initialize(LPCTSTR szFileName)
{
	// get file name
	CString csFileName;
	if (szFileName)
	{
		SetFileName(szFileName);
	}
	csFileName = GetFileName();
	// verify whether the file exists
	HFILE hFile = NULL;
	int nOpenMode;
	// open receipt file with given mode
	if (!IsFileUnicode(csFileName))
		nOpenMode = CFile::modeRead | CFile::shareDenyWrite | CFile::typeText;
	else
		nOpenMode = CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary;

	CStdioFile FileWrap;

	//+TAR247967 - changing all upper case MFC Macro TRY/CATCH to C++ try/catch
	try
	{
		hFile =  FileWrap.Open(csFileName, nOpenMode);
	}
	catch( CFileException* e )
	{ 
#ifdef _DEBUG_	
		afxDump << _T("File ") << csFileName << _T("could not be opened. ") << e.m_cause << _T("\n");    
#endif 
		e->Delete();
	}
	//TAR247967

	if (!hFile)
	{
		TRACE(_T("Could not build string list with file: %s\n"), csFileName);
		return FALSE;
	}
	else
	{
		FileWrap.Close();
	}

	// file exists, read file and add to the array
	BOOL bRetValue = BuildList();
	// free unused memory
	FreeExtra();
	SetSize(GetSize());

	return bRetValue;
}

void CSCOTStringList::UnInitialize()
{
	RemoveAll();
	m_nListSize = 0;
	m_csFileName = _T("");
}

int CSCOTStringList::GetSize()
{
	return m_nListSize;
}

BOOL CSCOTStringList::BuildList()
{
    CString FileEntry;
    CStdioFile FileWrap;

	BOOL BuildStatus;
	BOOL bFileUnicode = IsFileUnicode(m_csFileName);

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
		// insert first one in the table as a invalid string
		// this is done because the SCOTString lists are based on index 1 and actual array index is based on zero
		// by inserting an invalid string at zero index, the SCOTString list index matches with that of an actual array used to implement.
		ASSERT(!m_nListSize);
		CString* pcsFirstString = new CString(szInvalidStr);
		if (pcsFirstString)
		{
			InsertAt(m_nListSize++, pcsFirstString);
			pcsFirstString = NULL;
		}
		// read strings and add to the list
        while (FileWrap.ReadString(FileEntry))
        {
            int nEntryLen;
			if (!bFileUnicode)
				nEntryLen = FileEntry.GetLength();
			else
				nEntryLen = FileEntry.GetLength()-1;

			if (FileEntry.Left(1) == 0xFEFF)  //byte order mark for unicode file
				FileEntry = FileEntry.Mid(1);

			if (nEntryLen && (FileEntry[0] == _T('[')))
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
					ASSERT(nIndex == m_nListSize);
					// extract right most (nEntryLen - nStringIndexPos)
					//csStringValue  = FileEntry.Right(nEntryLen - nStringIndexPos);
					csStringValue  = FileEntry.Mid(nStringIndexPos, nEntryLen - nStringIndexPos);
				}
				RemoveDoubleQuotes(csStringValue);				// remove double quotes at ends if exists
				CString* pcsStringValue = new CString(csStringValue);
				InsertAt(m_nListSize, pcsStringValue);			// insert this valid string
				pcsStringValue = NULL;
				++m_nListSize;	// increment list size
			}
		}
#if 0
		for (int i = 0, nSize = GetSize(); i < nSize; ++i)
		{
			CString* pStr = GetAt(i);
			TRACE(_T("%d=%s\n"),i, *pStr);
		}
#endif
		ASSERT(GetSize());
        FileWrap.Close();
    }
    catch(CFileException *e)
    {
        e->Delete();
        return FALSE;
    }
	return TRUE;	// successfully built the list
}


// does a linear search, this class is not intended for search intensive lists
int CSCOTStringList::Find(CString csFindString)
{

	int TableSize = GetSize();
	int  iIndex = -1;

	//an empty table
	if (!TableSize)
	{
		return iIndex;
	}

	csFindString.TrimLeft();
	csFindString.TrimRight();

   // First look for exact matches.  If none are found, look for 
   // substring matches (table item is a substring of seach item)
   // We have to do this in case a message (such as "TOTAL")
   // is a substring of another message (such as "(RE)ENTER TOTAL FEET")

   for (int iSubStringMatch = 0; iSubStringMatch <= 1; iSubStringMatch++)
      {
	   // search a nonempty table
	   for(int i = 0; i < TableSize ; ++i)
   	   {
		   CString& csListElement = (*GetAt(i));
		   int nListElementLen = csListElement.GetLength();
		   csListElement.TrimLeft();
		   csListElement.TrimRight();

		   ASSERT(nListElementLen);
		   if (!nListElementLen)
		      {
			   continue;
   		   }

         // Outer loop controls whether to compare or find.
         // The first time through we compare, if none are found,
         // we look for substraing matches.


		   bool bFound = 
            iSubStringMatch ? 
               ((csFindString.Find(csListElement) == -1) ? false : true) :
               ((csFindString == csListElement)? true : false);

		   // retstatus is zero if the string is found
		   if (bFound)
		      {
			   iIndex = i;
			   break;
		      }
   	   }
      // If we found an exact match, no need to continue outer loop.
      if (iIndex >= 0 )
         break;
      }
	return iIndex;	
}


CString& CSCOTStringList::operator [](int index)
{
	if ((index >=0) && (index < GetSize()))
	{
		CString* pStr = GetAt(index);
		return  *pStr;
	}
	else
	{
		TRACE(_T("ERROR: Array index is out of bounds, built list from file: %s\n"), m_csFileName);
		AfxThrowMemoryException();
	}
	return (CString &) (EMPTY_STRING);
}


void CSCOTStringList::RemoveDoubleQuotes(CString& csQuoteString)
{
	ExtractStringContent(csQuoteString, _T('\"'));
}

int CSCOTStringList::GetListID()
{
	return m_nListID;
}

void CSCOTStringList::SetListID(int nListID)
{
	m_nListID = nListID;
}


CString CSCOTStringList::GetListName()
{
	return m_csListName;
}


bool CSCOTStringList::SetListName(LPCTSTR szListName)
{
	if (szListName)
	{
		m_csListName = szListName;
	}
	return ExtractStringContent(m_csListName, _T('['),_T(']'));
}


bool CSCOTStringList::ExtractStringContent(CString& csOrigString, TCHAR chDilimiter1, TCHAR chDilimiter2)
{
	bool retValue = false;
	int nOrigStringLen = csOrigString.GetLength();
	if (!nOrigStringLen) return retValue;
	if (!chDilimiter2)
	{
		chDilimiter2 = chDilimiter1;
	}
	TCHAR cFirstChar = csOrigString[0];
	TCHAR cLastChar  = csOrigString[nOrigStringLen - 1];
	if ((cFirstChar == chDilimiter1) && (cLastChar == chDilimiter2))
	{
		retValue = true;
		TCHAR szTmpStringValue[MAX_VALUELEN] = { _T('\0') } ;
		for (int i = 1; (i < (nOrigStringLen-1)); ++i)
		{
			szTmpStringValue[i-1] = csOrigString[i];
		}
		csOrigString = szTmpStringValue;
	}
	return retValue;
}

bool CSCOTStringList::IsFileUnicode(LPCTSTR lpInFile)
{
	DWORD	  dwSize;

	HANDLE	  _hFile=NULL;
	HANDLE	  _hMapFile = NULL;
	PBYTE	  _Buff;

    dwSize = 0L; /* use full size for input file	*/
					/* MapFile returns actual size	*/
			       

    /* calculate an upper bound for the size of the output file and
     * memorymap it.
     */

	_Buff = NULL;

    /* Open a file for read, or create a file for read/write */

    _hFile = (HANDLE) CreateFile(	lpInFile,
	 								GENERIC_READ,
									0,              // exclusive access
									0L,             // no security 
									OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL,
									0L);            // no template file 
    if( (DWORD)_hFile == 0xFFFFFFFF) 
	{
		TRACE(_T("Fail to open the file %s \n"), lpInFile);
		return false;
	}

    /* Create a File mapping that is either read or read/write */

    _hMapFile = CreateFileMapping(	_hFile,
									NULL,		// no security attributes
									PAGE_READONLY,
									0L,		// Hi size DWORD
									dwSize, 	// Lo size DWORD
									NULL);        // no name 
    if (!_hMapFile) 
	{
		TRACE(_T("Fail to CreateFileMapping for the file %s \n"), lpInFile);
		CloseHandle(_hFile);			// close the file
		return false;
	}


    /* Create a Mapped View that is either read or read/write */

    _Buff = (PBYTE) MapViewOfFile(	_hMapFile,
									FILE_MAP_READ,
									0L,		    // zero offset 
									0L,
									dwSize);
    if (!_Buff) 
	{
		TRACE(_T("Fail to MapViewOfFile for the file %s \n"), lpInFile);
		if (_hMapFile) 
			CloseHandle(_hMapFile);       // close the handle to the file mapping 
		CloseHandle(_hFile);			// close the file
		return false;
	}

    /* retain the size of the file opened */

    dwSize = GetFileSize( _hFile, NULL);
	bool ret = false;

    /* format and print error messages from GetLastError */
    if( _Buff && dwSize )
	{
		 if (IsTextUnicode(_Buff, dwSize, NULL ))
			 ret = true;
		 else
			 ret = false;
		 
	}

	if (_Buff) 
		UnmapViewOfFile(_Buff);     //unmap the output file
	if (_hMapFile) 
		CloseHandle(_hMapFile);       // close the handle to the file mapping 
	if (_hFile)
		CloseHandle(_hFile);			// close the file

	return ret;

}






	

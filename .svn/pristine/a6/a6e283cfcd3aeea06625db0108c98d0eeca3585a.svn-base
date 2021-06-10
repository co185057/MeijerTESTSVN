// SCOTMultiStringList.cpp: implementation of the CSCOTMultiStringList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCOTMultiStringList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSCOTMultiStringList::CSCOTMultiStringList(LPCTSTR szFileName, int nListID)
	: CSCOTStringList(szFileName, nListID)
{
}

CSCOTMultiStringList::CSCOTMultiStringList(int nListID) : CSCOTStringList(nListID)
{
}

CString CSCOTMultiStringList::GetFieldValue(int nIndex, int nField)
{
	if ((nIndex >=0) && (nIndex < GetSize()))
	{
		CString str = *GetAt(nIndex);
		return  GetField(str, nField, _T(';'));
	}
	else
	{
		TRACE(_T("ERROR: Array index is out of bounds, built list from file: %s\n"), GetFileName());
		AfxThrowMemoryException();
	}
	return CString(_T(""));
}

USHORT CSCOTMultiStringList::GetCode(int nIndex)
{
	CString str = GetFieldValue(nIndex, 0);
	if (str == _T(""))
		return NULL;
	/******cm150000: Modified in Merge 2******/
	//return atoi(str);
	return (USHORT)_tcstoul((LPCTSTR)str, NULL, 0);
	/******cm150000: End Modified Merge 2******/
}

CString CSCOTMultiStringList::GetLabel(int nIndex)
{
	return GetFieldValue(nIndex, 1);
}

bool CSCOTMultiStringList::GetStatus(int nIndex)
{
	CString str = GetFieldValue(nIndex, 2);
	str.TrimLeft();
	str.TrimRight();
	return ((str != _T("0")) && (GetLabel(nIndex) != _T("")) && (GetCode(nIndex) != NULL));
}

CString CSCOTMultiStringList::GetField(CString str, int nField, _TCHAR separator)
{
	int n = 0, i = 0;
//  BP 2/7/00 Changing algorithm to find field because it was causing memory leaks
/*	for(; i<str.GetLength(); i++)
		if (str[i] == separator) n++;
	if (nField > n) return "";

	CString* fields = new CString[n+1];
	i = 0;
	while(i < n + 1)
	{
		int j = str.Find(separator);
		if (j == -1) // separator not found
		{
			fields[i++] = str;
			str = "";
		}
		else // normal field (may be empty)
		{
			fields[i++] = str.Left(j);
			str = str.Mid(j + 1, str.GetLength() - j - 1);
		}
	}
	return fields[nField];
*/
    CString csRetVal = _T("");
    int nCurrentField = 0;
    while (nCurrentField <= nField)
    {
        i = str.Find(separator); 
        if (i == -1)   //separator not found
        { 
            if (nField != nCurrentField)  
                break;
            csRetVal = str;
        }
        else if (nField == nCurrentField)
        {
            csRetVal = str.Left(i);
        }
		else 
		{
			str = str.Mid(i + 1, str.GetLength() - 1);
		}
        nCurrentField++;
    }
	if ((nField == 2) && csRetVal.IsEmpty())
	{
		// by default, no status means enable the button if label is specified
		// with new bit field structure for status field, the default is to set the
		// button enabled in local mode, remote mode and inside transaction
		// that means Binary (0111) = Decinmal(7)
		csRetVal = _T("7");
	}
    return csRetVal;
}

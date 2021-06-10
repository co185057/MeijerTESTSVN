#include "stdafx.h"
#include "TBUtilities.h"

long CopyString(LPTSTR& Destination, LPCTSTR Source, int nSpecifiedLen)
{
   if (Destination) {
		delete [] Destination;
		Destination = NULL;
   }
	long Len = 0;
	if (Source)
    {
        if (nSpecifiedLen)
            Len = nSpecifiedLen;
        else
		    Len = _tcslen(Source) + 1; // will include NULL-terminating character
    }
	if (Len)
	{
		Destination = (TCHAR *) new TCHAR[Len];
		memset(Destination, 0, Len + sizeof(TCHAR));
		_tcsncpy(Destination, Source, Len);
	}
	else
	{
		Destination = NULL;
	}
	return Len;
}


// Function: LaunchApp
// Purpose:  Launch an application
// Inputs:  App name
// Outputs: boolean for success
// Assumptions: None
// Comments: None
//
bool LaunchApp(const CString &csShellName)
{
    SHELLEXECUTEINFO sExec;
    memset(&sExec, 0, sizeof(SHELLEXECUTEINFO));
    sExec.cbSize = sizeof(SHELLEXECUTEINFO);
    sExec.lpFile = csShellName;
    return ShellExecuteEx(&sExec) ? 1: 0 ;
}

bool SplitString(CString csList, CString csDelim, CStringArray* pcsaOutputArray)
{
	int iPosition, iLen;
	while(!csList.IsEmpty())
	{
		iPosition = csList.Find(csDelim);
		if(iPosition == -1)
		{
			iPosition = csList.GetLength();
			iLen = iPosition;
		}
		else
			iLen = iPosition + 1;
		pcsaOutputArray->Add(csList.Left(iPosition));
		csList.Delete(0, iLen);
	}
	 if(pcsaOutputArray->GetSize() > 0)
		 return true;
	 else
		 return false;
}
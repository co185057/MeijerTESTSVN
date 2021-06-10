//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SCOTOpts.cpp
//
// TITLE:	Class implementation for SCOTOpts
//
// AUTHOR:	John Z. Bush
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 11/21/2002,	jzb.10.x,	Lowering of Security for Item Removals
//							jzb.10.3, SCOTOpts allows access to FastLane option settings
//
//////////////////////////////////////////////////////////////////////////////////////////////////

// ***** jzb.10.3
#include "stdafx.h"
#include "SCOTOpts.h"
#include "Common.h"            // MGV Trace Object

#define COMP_ID ID_SM               // base state component
#define T_ID    "SCOTOpts"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SCOTOpts::SCOTOpts():
m_sFastLaneConfigPath("c:\\scot\\config"),
m_sDefaultProfile("SCOTopts.dat"),
m_sStoreLevelProfile("SCOTopts.000")
{
}

SCOTOpts::~SCOTOpts()
{
}

bool SCOTOpts::GetOptionFlag(CString &sOption, bool bDefault)
{
	bool bResult = bDefault;
	CString sOptVal;

	GetOption( sOption, sOptVal, true);
	if(sOptVal.Compare("?") == 0)
	{
		sOptVal.Empty();
		GetOption( sOption, sOptVal, false);
	}

	if((sOptVal.CompareNoCase("Y") == 0)||(sOptVal.CompareNoCase("yes") == 0))
	{
		bResult = true;
	}
	else if((sOptVal.CompareNoCase("N") == 0)||(sOptVal.CompareNoCase("no") == 0))
	{
		bResult = false;
	}

	CString strFmt;
	strFmt = _T("SCOTOpts - Flag Option: %s, Value: %d");
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_ERROR, strFmt, sOption, bResult);

	return bResult;
}

void SCOTOpts::GetOption(CString &sOption, CString &sValue, bool bStoreProfile)
{
	int iLength = 10;
	CString sFilePath, sResult;

	if(bStoreProfile)
		sFilePath.Format("%s\\%s", m_sFastLaneConfigPath, m_sStoreLevelProfile);
	else
		sFilePath.Format("%s\\%s", m_sFastLaneConfigPath, m_sDefaultProfile);

	LPTSTR pResult = sResult.GetBufferSetLength( iLength);
	DWORD dwResult = GetPrivateProfileString(	"SCOTSSF", sOption, "?",
												pResult, iLength, sFilePath);
	sResult.ReleaseBuffer( dwResult);

	sValue.Format("%s", sResult);
}

bool SCOTOpts::GetOptionStr(CString &sOption, CString &sValue, CString &sDefault)
{
	bool bResult = true;
	CString sOptVal;

	GetOption( sOption, sOptVal, true);
	if(sOptVal.Compare("?") == 0)
	{
		sOptVal.Empty();
		GetOption( sOption, sOptVal, false);
		if(sOptVal.Compare("?") == 0)
		{
			bResult = false;
		}
	}

	if(bResult)
	{
		sValue.Format("%s", sOptVal);
	}
	else
	{
		sValue.Format("%s", sDefault);
	}

	CString strFmt;
	strFmt = _T("SCOTOpts - String Option: %s, Value: %s");
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_ERROR, strFmt, sOption, sValue);

	return bResult;
}
// *****

//
// FILE:  USSFUtilityBase.cpp
//
// TITLE: USSF Utilties
//
//
//
// AUTHOR:   Christopher Swift
// DATE:     9/2007
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"                 // MS MFC always needed first
//                     // MS MFC template header
#include <afxpriv.h>                // for USES_CONVERSION
#include <afxdisp.h>
// TAR #429050 START
#include "Common.h"
#include "USSFCommon.h"
#include "PSIntList.h"                    // MGV common includes
#include "PSProcedures.h"
// TAR #429050 STOP
/////////////


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define Clear() Clear()
#define COMP_ID ID_SM               // base state component
#define T_ID    _T("USSFUtilityBase")

extern CPSX*  m_pGAPSX;          // NewUI - pointer to PSX object
DLLEXPORT extern HWND hw;

long USSFUtilityBase::lSymbolRecursionDepth = 0;
long USSFUtilityBase::lSymbolRecursionLimit = 10;

//Begin SWR740 - HHRAP performance improvement
long USSFUtilityBase::lUSSFVariableElementID = 0;
long USSFUtilityBase::lUSSFVariableSerial = 0;


USSFMetaData::USSFMetaData()
{
	trace(L8, _T("Constructing USSFMetaData"));
	lWriteCount = lReadCount = lSerial = lElementID = 0;
	csReadTime = csWriteTime = _T("");
	vPreviousValue = _T("");
}
USSFMetaData::USSFMetaData(const USSFMetaData &source)
{
	if (&source != NULL)
	{
		trace(L8, _T("Copying USSFMetaData"));
		lWriteCount = source.lWriteCount;
		lReadCount = source.lReadCount;
		lSerial = source.lSerial;
		lElementID = source.lElementID;
		vPreviousValue = source.vPreviousValue;
		csReadTime = source.csReadTime;
		csWriteTime = source.csWriteTime;
	}
}
USSFMetaData& USSFMetaData::operator=(const USSFMetaData &source)
{	
	if (this == &source)
	{
		return *this;
	}

	if (&source != NULL)
	{
		trace(L8, _T("Assigning USSFMetaData"));
		lWriteCount = source.lWriteCount;
		lReadCount = source.lReadCount;
		lSerial = source.lSerial;
		lElementID = source.lElementID;
		vPreviousValue = source.vPreviousValue;
		csReadTime = source.csReadTime;
		csWriteTime = source.csWriteTime;
	}
	return *this;
}
//End SWR740 - HHRAP performance improvement


USSFUtilityBase::USSFUtilityBase()
{
}
USSFUtilityBase::~USSFUtilityBase()
{
	m_USSF_MetaData.clear();
	m_USSF_Symbols.clear();
}

bool USSFUtilityBase::TestPSX(void)
{
  if (m_pGAPSX == NULL)
  {
    m_pGAPSX = ps.GetPSXObject();
    if (m_pGAPSX == NULL)
    {
      trace(L5, _T("PSX Object Unavailable."));
      return false;
    }
  }
  return true;
}

long USSFUtilityBase::SetTransactionVariable(CString name, COleVariant vValue)
{
	USES_CONVERSION;

    long rc = 0, lReadCount = -1, lWriteCount = -1, lSerial = -1, lElementID = -1;
  CString csReadTime = _T("?"), csWriteTime = _T("?");
  COleVariant vPreviousValue = _T("?");
  USSFMetaData thisElement;
  CString timeStamp;
  SYSTEMTIME st;
  GetLocalTime(&st);
  timeStamp.Format(_T("%02d:%02d:%02d:%03d"), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
  // New Mechanism!
  if (name.GetLength() > 0)
  {
	vValue.ChangeType(VT_BSTR);
	CString csTempstr = OLE2T(vValue.bstrVal);
	trace(L8, _T("START USSF Variable SET <%s> = <%s>"), name, csTempstr);
	
	// Try to update the meta-data!
	try
	{
		// Either exists already or create it.
		map<CString, USSFMetaData>::iterator it;
		it = m_USSF_MetaData.find(name);
		if (it != m_USSF_MetaData.end())
		{
			thisElement = m_USSF_MetaData[name];
			// Exists!
			thisElement.lWriteCount++;
			thisElement.lSerial = lUSSFVariableSerial++;
			thisElement.csWriteTime = timeStamp;
			try
			{
				thisElement.vPreviousValue = m_USSF_Symbols[name];
			}
			catch(...)
			{
				trace(L5, _T("Unable to Retrieve Existing Value <USSF Variable={%s}>."), name);
			}
			m_USSF_MetaData[name] = thisElement;
		}
		else
		{
			// New!
			thisElement.lElementID = lUSSFVariableElementID++;
			thisElement.lWriteCount = 1;
			thisElement.lReadCount = 0;
			thisElement.lSerial = lUSSFVariableSerial++;
			thisElement.csReadTime = _T("");
			thisElement.csWriteTime = timeStamp;
			thisElement.vPreviousValue = _T("");
			m_USSF_MetaData[name] = thisElement;
		}

		lElementID = m_USSF_MetaData[name].lElementID;
		lWriteCount = m_USSF_MetaData[name].lWriteCount;
		lReadCount = m_USSF_MetaData[name].lReadCount;
		lSerial = m_USSF_MetaData[name].lSerial;
		csWriteTime = m_USSF_MetaData[name].csWriteTime;
		csReadTime = m_USSF_MetaData[name].csReadTime;
		vPreviousValue = m_USSF_MetaData[name].vPreviousValue;
	}
	catch(...)
	{
		trace(L5, _T("Problem Setting USSF Variable Meta-Data <USSF Variable={%s}>."), name);
	}

	try
	{
		m_USSF_Symbols[name] = vValue;
	}
	catch(...)
	{
		trace(L5, _T("Problem Setting USSF Variable; <USSF Variable={%s}>."), name);
		rc = 1;
	}

	vValue.ChangeType(VT_BSTR);
	csTempstr = OLE2T(vValue.bstrVal);
	trace(L8, _T("END USSF Variable SET <%s> = <%s>; META <ID={%d};ser={%d};rdcnt={%d};wrcnt={%d};prev={%s};rt={%s};wt={%s}>"),
				name, csTempstr, lElementID, lSerial, lReadCount, lWriteCount, csTempstr, csReadTime, csWriteTime);
  }

  return rc;

}

long USSFUtilityBase::GetTransactionVariable(CString name, CString &value)
{
  USES_CONVERSION;

  COleVariant vValue;
  long rc = GetTransactionVariable(name, vValue);
  if (!rc)
  {
	  vValue.ChangeType(VT_BSTR);
	  value = OLE2T(vValue.bstrVal);
  }
  else
  {
	  trace(L8, _T("USSFUtilityBase::GetTransactionVariable %s failed"), name);
	  value = _T("");
  }

  return rc;

}

long USSFUtilityBase::GetTransactionVariable(CString name, long &value)
{
   USES_CONVERSION;

  COleVariant vValue;
  long rc = GetTransactionVariable(name, vValue);
  if (!rc)
  {
      CString csValue = vValue.bstrVal;
      trace(L8, _T("GetTransactionVariable() csValue {%s}"), csValue);
      if (csValue.Compare(_T("")) == 0)
      {
          value = 0;
      }
      else
      {
          vValue.ChangeType(VT_I4);
          value = vValue.lVal;
      } 
  }
  else
  {
	  trace(L8, _T("USSFUtilityBase::GetTransactionVariable %s failed"), name);
	  value = 0;
  }

  return rc;

}

long USSFUtilityBase::GetTransactionVariable(CString name, double &value)
{
  USES_CONVERSION;

  COleVariant vValue;
  long rc = GetTransactionVariable(name, vValue);
  if (!rc)
  {
      CString csValue = vValue.bstrVal;
      trace(L8, _T("GetTransactionVariable() csValue {%s}"), csValue);
      if (csValue.Compare(_T("")) == 0)
      {
          value = 0;
      }
      else
      {
          vValue.ChangeType(VT_R8);
          value = vValue.dblVal;
      }
  }
  else
  {
	  trace(L8, _T("USSFUtilityBase::GetTransactionVariable %s failed"), name);
	  value = 0;
  }

  return rc;

}

long USSFUtilityBase::GetTransactionVariable(CString name, COleVariant &value)
{
  USES_CONVERSION;

	long rc = 0, lReadCount = -1, lWriteCount = -1, lSerial = -1, lElementID = -1;
	CString csReadTime = _T("?"), csWriteTime = _T("?");
	COleVariant vValue, vPreviousValue = _T("?");
	USSFMetaData thisElement;
	CString timeStamp;
	SYSTEMTIME st;
	GetLocalTime(&st);
	timeStamp.Format(_T("%02d:%02d:%02d:%03d"), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	// New Mechanism!
	value = _T("");		// By Default, nothing.

	trace(L8, _T("START USSF Variable GET <%s>."), name);

	try
	{
		// Either exists already or create it.
		map<CString, USSFMetaData>::iterator it;
		it = m_USSF_MetaData.find(name);
		if (it != m_USSF_MetaData.end())
		{
			thisElement = m_USSF_MetaData[name];
			// Exists!
			thisElement.lReadCount++;
			thisElement.lSerial = lUSSFVariableSerial++;
			thisElement.csReadTime = timeStamp;
			m_USSF_MetaData[name] = thisElement;
		}
		else
		{
			// New!
			thisElement.lElementID = lUSSFVariableElementID++;
			thisElement.lWriteCount = 0;
			thisElement.lReadCount = 1;
			thisElement.lSerial = lUSSFVariableSerial++;
			thisElement.csReadTime = timeStamp;
			thisElement.csWriteTime = _T("");
			thisElement.vPreviousValue = _T("");
			m_USSF_MetaData[name] = thisElement;
		}

		lElementID = m_USSF_MetaData[name].lElementID;
		lWriteCount = m_USSF_MetaData[name].lWriteCount;
		lReadCount = m_USSF_MetaData[name].lReadCount;
		lSerial = m_USSF_MetaData[name].lSerial;
		csWriteTime = m_USSF_MetaData[name].csWriteTime;
		csReadTime = m_USSF_MetaData[name].csReadTime;
		vPreviousValue = m_USSF_MetaData[name].vPreviousValue;
	}
	catch(...)
	{
		trace(L5, _T("Problem Setting USSF Variable Meta-Data <USSF Variable={%s}>."), name);
	}

	try
	{
		map<CString, COleVariant>::iterator it;
		it = m_USSF_Symbols.find(name);
		if (it != m_USSF_Symbols.end())
		{
			value = it->second;
		}
		else
		{
			value = _T("");
			rc = 1;
		}
	}
	catch(...)
	{
		trace(L5, _T("Problem Getting USSF Variable <USSF Variable={%s}>."), name);
		rc = 1;
	}

	vPreviousValue.ChangeType(VT_BSTR);
	CString csTempstr = OLE2T(vPreviousValue.bstrVal);
	value.ChangeType(VT_BSTR);
	CString csTempstr2 = OLE2T(value.bstrVal);
	trace(L8, _T("END USSF Variable GET <%s> = <%s>; META <ID={%d};ser={%d};rdcnt={%d};wrcnt={%d};prev={%s};rt={%s};wt={%s}>"),
				name, csTempstr2, lElementID, lSerial, lReadCount, lWriteCount, csTempstr, csReadTime, csWriteTime);
	return rc;

}

long USSFUtilityBase::DumpSymbols(bool bMeta)
{
	USES_CONVERSION;

	long rc = 0, lReadCount, lWriteCount, lSerial, lElementID;
	CString name, value, csReadTime, csWriteTime;
	COleVariant vValue, vPreviousValue;

	map<CString, COleVariant>::iterator it_Symbols;
	map<CString, USSFMetaData>::iterator it_MetaData;

	// Logic to Dump USSF Symbols
	// Set Iterator to Next Element.
	it_Symbols = m_USSF_Symbols.begin();
	
	while(it_Symbols != m_USSF_Symbols.end())
	{
		name = it_Symbols->first;
		vValue = it_Symbols->second;
		vValue.ChangeType(VT_BSTR);
		value = OLE2T(vValue.bstrVal);
		lElementID = lSerial = lReadCount = lWriteCount = -1;
		csReadTime = csWriteTime = _T("?");
		vPreviousValue = _T("?");

		// Now Try to get the Meta Data!
		try
		{
			it_MetaData = m_USSF_MetaData.find(name);
			if (it_MetaData != m_USSF_MetaData.end())
			{
				// Exists!
				lElementID = m_USSF_MetaData[name].lElementID;
				lReadCount = m_USSF_MetaData[name].lReadCount;
				lWriteCount = m_USSF_MetaData[name].lWriteCount;
				lSerial = m_USSF_MetaData[name].lSerial;
				csReadTime = m_USSF_MetaData[name].csReadTime;
				csWriteTime = m_USSF_MetaData[name].csWriteTime;
				vPreviousValue = m_USSF_MetaData[name].vPreviousValue;
			}
		}
		catch(...)
		{
			trace(L5, _T("Problem Getting USSF Variable Meta-Data <USSF Variable={%s}>."), name);
		}

		vPreviousValue.ChangeType(VT_BSTR);
		CString csTempstr = OLE2T(vPreviousValue.bstrVal);
		//trace(L0, _T("USSF Variable DUMP <%s> = <%s>; META <ID={%d};ser={%d};rdcnt={%d};wrcnt={%d};prev={%s};rt={%s};wt={%s}>"),
		//		name, value, lElementID, lSerial, lReadCount, lWriteCount, csTempstr, csReadTime, csWriteTime);
		if (bMeta)
		{	// With Meta Data
			trace(L7, _T("USSF Variable DUMP Symbol=<%30s>;Serial=<%8d>;ID=<%4d>;Rdcnt=<%5d>;Wrcnt=<%5d>;RT=<%12s>;WT=<%12s>;Value=<%s>;Previous=<%s>."),
					name, lSerial, lElementID, lReadCount, lWriteCount, csReadTime, csWriteTime, value, csTempstr);
		}
		else
		{	// Without Meta Data
			trace(L7, _T("USSF Variable DUMP Symbol=<%30s>;Value=<%s>."), name, value);
		}
		it_Symbols++;
	}
	
	return rc;
}

long USSFUtilityBase::DumpSymbolsBySerial(bool bMeta)
{
	USES_CONVERSION;

	long rc = 0, lReadCount, lWriteCount, lSerial, lElementID;
	CString name, value, csReadTime, csWriteTime;
	COleVariant vValue, vPreviousValue;
	USSFMetaData thisElement;
	
	map<CString, COleVariant>::iterator it_Symbols;
	map<CString, USSFMetaData>::iterator it_MetaData;
	map<long, CString> m_USSF_Index;
	map<long, CString>::iterator it_Index;

	// First Build the Index
	it_MetaData = m_USSF_MetaData.begin();
	while(it_MetaData != m_USSF_MetaData.end())
	{
		name = it_MetaData->first;
		thisElement = it_MetaData->second;
		m_USSF_Index[thisElement.lSerial] = name;
		it_MetaData++;
	}

	// Logic to Dump USSF Symbols
	// Set Iterator to Next Element.
	it_Index = m_USSF_Index.begin();
	
	while(it_Index != m_USSF_Index.end())
	{
		name = it_Index->second;
		vValue = m_USSF_Symbols[name];
		vValue.ChangeType(VT_BSTR);
		value = OLE2T(vValue.bstrVal);
		lElementID = lSerial = lReadCount = lWriteCount = -1;
		csReadTime = csWriteTime = _T("?");
		vPreviousValue = _T("?");

		// Now Try to get the Meta Data!
		try
		{
			it_MetaData = m_USSF_MetaData.find(name);
			if (it_MetaData != m_USSF_MetaData.end())
			{
				// Exists!
				lElementID = m_USSF_MetaData[name].lElementID;
				lReadCount = m_USSF_MetaData[name].lReadCount;
				lWriteCount = m_USSF_MetaData[name].lWriteCount;
				lSerial = m_USSF_MetaData[name].lSerial;
				csReadTime = m_USSF_MetaData[name].csReadTime;
				csWriteTime = m_USSF_MetaData[name].csWriteTime;
				vPreviousValue = m_USSF_MetaData[name].vPreviousValue;
			}
		}
		catch(...)
		{
			trace(L5, _T("Problem Getting USSF Variable Meta-Data <USSF Variable={%s}>."), name);
		}

		vPreviousValue.ChangeType(VT_BSTR);
		CString csTempstr = OLE2T(vPreviousValue.bstrVal);
		if (bMeta)
		{	// With Meta Data
			trace(L7, _T("USSF Variable DUMP Serial=<%8d>;ID=<%4d>;Symbol=<%30s>;Rdcnt=<%5d>;Wrcnt=<%5d>;RT=<%12s>;WT=<%12s>;Value=<%s>;Previous=<%s>."),
					lSerial, lElementID, name, lReadCount, lWriteCount, csReadTime, csWriteTime, value, csTempstr);
		}
		else
		{	// Without Meta Data
			trace(L7, _T("USSF Variable DUMP Serial=<%8d>;Symbol=<%30s>;Value=<%s>."), lSerial, name, value);
		}
		it_Index++;
	}
	return rc;
}

long USSFUtilityBase::DumpSymbolsByElementID(bool bMeta)
{
	USES_CONVERSION;

	long rc = 0, lReadCount, lWriteCount, lSerial, lElementID;
	CString name, value, csReadTime, csWriteTime;
	COleVariant vValue, vPreviousValue;
	USSFMetaData thisElement;
	
	map<CString, COleVariant>::iterator it_Symbols;
	map<CString, USSFMetaData>::iterator it_MetaData;
	map<long, CString> m_USSF_Index;
	map<long, CString>::iterator it_Index;

	// First Build the Index
	it_MetaData = m_USSF_MetaData.begin();
	while(it_MetaData != m_USSF_MetaData.end())
	{
		name = it_MetaData->first;
		thisElement = it_MetaData->second;
		m_USSF_Index[thisElement.lElementID] = name;
		it_MetaData++;
	}

	// Logic to Dump USSF Symbols
	// Set Iterator to Next Element.
	it_Index = m_USSF_Index.begin();
	
	while(it_Index != m_USSF_Index.end())
	{
		name = it_Index->second;
		vValue = m_USSF_Symbols[name];
		vValue.ChangeType(VT_BSTR);
		value = OLE2T(vValue.bstrVal);
		lElementID = lSerial = lReadCount = lWriteCount = -1;
		csReadTime = csWriteTime = _T("?");
		vPreviousValue = _T("?");

		// Now Try to get the Meta Data!
		try
		{
			it_MetaData = m_USSF_MetaData.find(name);
			if (it_MetaData != m_USSF_MetaData.end())
			{
				// Exists!
				lElementID = m_USSF_MetaData[name].lElementID;
				lReadCount = m_USSF_MetaData[name].lReadCount;
				lWriteCount = m_USSF_MetaData[name].lWriteCount;
				lSerial = m_USSF_MetaData[name].lSerial;
				csReadTime = m_USSF_MetaData[name].csReadTime;
				csWriteTime = m_USSF_MetaData[name].csWriteTime;
				vPreviousValue = m_USSF_MetaData[name].vPreviousValue;
			}
		}
		catch(...)
		{
			trace(L5, _T("Problem Getting USSF Variable Meta-Data <USSF Variable={%s}>."), name);
		}

		vPreviousValue.ChangeType(VT_BSTR);
		CString csTempstr = OLE2T(vPreviousValue.bstrVal);
		if (bMeta)
		{	// With Meta Data
			trace(L7, _T("USSF Variable DUMP ID=<%4d>;Serial=<%8d>;Symbol=<%30s>;Rdcnt=<%5d>;Wrcnt=<%5d>;RT=<%12s>;WT=<%12s>;Value=<%s>;Previous=<%s>."),
					lElementID, lSerial, name, lReadCount, lWriteCount, csReadTime, csWriteTime, value, csTempstr);
		}
		else
		{	// Without Meta Data
			trace(L7, _T("USSF Variable DUMP ID=<%4d>;Symbol=<%30s>;Value=<%s>."), lElementID, name, value);
		}
		it_Index++;
	}
	return rc;
}


long USSFUtilityBase::ResolveSymbol(CString csToken, CString &csValue, CString csDefault)
{
	trace(L8, _T("ResolveSymbol(<%s>, <%s>, <%s>)"), csToken, csValue, csDefault);

	long rc = 0;

	rc = FindSymbolValue(csToken, csValue, csDefault);

	trace(L7, _T("ResolveSymbol() Result=<%s>."), csValue);
	trace(L7, _T("ResolveSymbol() Returns <%d>."), rc);

	return rc;
}



long USSFUtilityBase::ResolveSymbol(CString csToken, long &lValue, long lDefault)
{
	trace(L8, _T("ResolveSymbol(<%s>, <%d>, <%d>)"), csToken, lValue, lDefault);

	long rc = 0;

	CString csResult, csDefault;
	csDefault.Format(_T("%d"), lDefault);
	ResolveSymbol(csToken, csResult, csDefault);

	// Convert csResult into a long!
	lValue=_ttol(csResult);

	trace(L7, _T("ResolveSymbol() Result=<%d>."), lValue);
	trace(L7, _T("ResolveSymbol() Returns <%d>."), rc);

	return rc;
}

long USSFUtilityBase::ResolveSymbol(CString csToken, double &dValue, double dDefault)
{
	trace(L8, _T("ResolveSymbol(<%s>, <%2.6e>, <%2.6e>)"), csToken, dValue, dDefault);

	long rc = 0;

	CString csResult, csDefault;
	csDefault.Format(_T("%2.6e"), dDefault);
	ResolveSymbol(csToken, csResult, csDefault);

	// Convert csResult into a long!
	COleVariant vTempstr = csResult;
    vTempstr.ChangeType(VT_R8);
    dValue = vTempstr.dblVal;

	//dValue=_wtof(csResult);

	trace(L7, _T("ResolveSymbol() Result=<%2.6e>."), dValue);
	trace(L7, _T("ResolveSymbol() Returns <%d>."), rc);

	return rc;
}

CString USSFUtilityBase::ResolveSymbol2(CString csToken, CString csDefault)
{
  CString csTempstr;
  ResolveSymbol(csToken, csTempstr, csDefault);
  return csTempstr;
}

// If _T("$") resolution is to be used, you must override this!
// For example, for IBM perhaps some sort of text data base lookup will occur.
CString USSFUtilityBase::GetUserTokenValue(CString csToken, CString csDefault)
{
	if (csToken.GetLength() > 0)
	{
		return csToken;
	}
	else
	{
		return csDefault;
	}
}

long USSFUtilityBase::FindSymbolValue(CString csToken, CString &csValue, CString csDefault)
{
	trace(L8, _T("FindSymbolValue(<%s>, <%s>, <%s>)"), csToken, csValue, csDefault);

	long rc = 0;
    CString csInitialValue = csValue;

	// TAR #429050 START
	if (csToken.Mid(0,1) == _T("@"))
	{
		// PSX Transaction Variable.
		// Strip the "@".
		// Check to see if there is a value for the current language setting.
		CString csTemp = _T("USSF_%s_%s"), csVariable;
		csVariable.Format(csTemp, ps.GetLCID(ps.GetLanguage()), csToken.Mid(1));
		csVariable.MakeUpper();
		GetTransactionVariable(csVariable, csValue);
		if (csValue.GetLength() == 0)
		{
			// Do it again without the language (get the default).
			csTemp = _T("USSF_%s");
			csVariable.Format(csTemp, csToken.Mid(1));
			csVariable.MakeUpper();
			GetTransactionVariable(csVariable, csValue);
		}
	}
	// TAR #429050 STOP
	else if (csToken.Mid(0,1) == _T("#"))
	{
		// PSX (Fastlane) Text.
		// Strip the "#".
		CString csVariable = csToken.Mid(1);
		int iPSTextNumber = _ttoi(csVariable);
		ASSERT(iPSTextNumber > 0);
		csValue = ps.GetPSText(iPSTextNumber, ps.GetLanguage());
	}
	else if (csToken.Mid(0,1) == _T("$"))
	{
		// User Value - ::GetUserTokenValue() should be overidden if this type is used.
		// Strip the "$".
		CString csVariable = csToken.Mid(1);
		csValue = GetUserTokenValue(csVariable, csDefault);
	}
	else
	{	// Nothing to Resolve.
		rc = 1;
		csValue = csToken;
	}

	// If the result is _T(""), then use default, if any.
	if (csValue == _T(""))
	{
		rc = 2;
		csValue = csDefault;
	}

	if (csValue == csInitialValue)
	{//Done!
		trace(L8, _T("FindSymbolValue() : Base Case Reached! Depth=<%d>."), lSymbolRecursionDepth);
		return rc;
	}
	else
	{//Recurse! If Maximum Depth NOT Reached!
		trace(L8, _T("*** Calling FindSymbolValue() Recursively."));
		if (lSymbolRecursionDepth < lSymbolRecursionLimit)	// Arbitrary Recursion Depth=10 (hard-coded).
		{
			lSymbolRecursionDepth++;
			rc = FindSymbolValue(csValue, csValue, csDefault);	// Recurse
			lSymbolRecursionDepth--;
		}
		else if (lSymbolRecursionDepth >= lSymbolRecursionLimit)
		{
			trace(L5, _T("*** Recursion Prohibited - Limit <%d> Reached; Error Forced."), lSymbolRecursionLimit);
			//SetPendingEvent(_T("InternalError5"));
		}
	}

	return rc;
}

long USSFUtilityBase::ResetData(CString scope)
{
  trace(L5, _T("SMUSSFActionBase::ResetData(CString scope)"));

  trace(L5, _T("Scope of Reset: %s"), scope);

  long rc = 0;
  ///////////////////////////////////////////////////////////////////////
  // Need to get transaction variables as needed and set as changed.
  ///////////////////////////////////////////////////////////////////////
  // META, TRXN, ALL.

  if (!scope.CompareNoCase(_T("META")) || !scope.CompareNoCase(_T("ALL")))
  {
	SetTransactionVariable(_T("USSF_METAELEMENT"), _T(""));
	SetTransactionVariable(_T("USSF_METABUFFER"), _T(""));
  }
  if (!scope.CompareNoCase(_T("TRXN")) || !scope.CompareNoCase(_T("ALL")))
  {
	SetTransactionVariable(_T("USSF_QUANTITY"), (long)0);
	SetTransactionVariable(_T("USSF_PRICE"), (long)0);
	SetTransactionVariable(_T("USSF_WEIGHT"), (long)0);
	SetTransactionVariable(_T("USSF_TARE"), (long)0);
	SetTransactionVariable(_T("USSF_POSData"), _T(""));
	SetTransactionVariable(_T("USSF_METAELEMENT"), _T(""));
	SetTransactionVariable(_T("USSF_METABUFFER"), _T(""));
  }
  if (!scope.CompareNoCase(_T("ITEM")) || !scope.CompareNoCase(_T("ALL")))
  {
	SetTransactionVariable(_T("USSF_QUANTITY"), (long)0);
	SetTransactionVariable(_T("USSF_PRICE"), (long)0);
	SetTransactionVariable(_T("USSF_WEIGHT"), (long)0);
	SetTransactionVariable(_T("USSF_TARE"), (long)0);
	SetTransactionVariable(_T("USSF_COUPON"), _T("false"));
	SetTransactionVariable(_T("USSF_VOID"), _T("false"));
  }

  return rc;
}

long USSFUtilityBase::ConcatenateData(CString csToken, CString &csTarget, bool bForceStore)
{
	trace(L7, _T("ConcatenateData(<%s>, <%s>)"), csToken, csTarget);

	long rc = 0;
	CString csSourceData, csTargetData, csVariable;

	rc = ResolveSymbol(csToken, csSourceData);
	rc = ResolveSymbol(csTarget, csTargetData);
	csTargetData += csSourceData;
	rc = SaveData(csTargetData, csTarget, bForceStore);

	trace(L7, _T("ConcatenateData() Returns <%d>."), rc);
	return rc;
}

long USSFUtilityBase::StoreData(CString csToken, CString &csTarget, bool bForceStore)
{
	trace(L7, _T("StoreData(<%s>, <%s>)"), csToken, csTarget);

	long rc = 0;

	CString csSourceData, csTargetData, csVariable;

	rc = ResolveSymbol(csToken, csSourceData);
	rc = SaveData(csSourceData, csTarget, bForceStore);

	trace(L7, _T("StoreData() Returns <%d>."), rc);
	return rc;
}

long USSFUtilityBase::SaveData(CString csTargetData, CString &csTarget, bool bForceStore)
{
	trace(L7, _T("SaveData(<%s>, <%s>)"), csTargetData, csTarget);

	long rc = 0;

	CString csVariable;

	// If target is a symbol, store back to the transaction variable.
	if (csTarget.Mid(0,1) == _T("@"))
	{
		csVariable = csTarget.Mid(1);
		csVariable.MakeUpper();
		csVariable = _T("USSF_") + csVariable;
		trace(L7, _T("SaveData() Normal Store into <%s>=<%s>."), csVariable, csTargetData);
		SetTransactionVariable(csVariable, csTargetData);
	}
	else
	{// Shall we store into specified target variable anyway?
		rc = 1;
		if ((bForceStore) && (csTarget.GetLength() > 0))
		{
			csVariable = _T("USSF_") + csTarget;
			csVariable.MakeUpper();
			trace(L7, _T("SaveData() Force Store into <%s>=<%s>."), csVariable, csTargetData);
			SetTransactionVariable(csVariable, csTargetData);
			rc = 2;
		}

		// In either case, store into csTarget.
		csTarget = csTargetData;
	}

	trace(L7, _T("SaveData() Returns <%d>."), rc);
	return rc;
}

long USSFUtilityBase::StoreParsedData(CString csToken, CString &csTarget, bool bForceStore)
{
	return ParseData(csToken, csTarget, bForceStore);
}

long USSFUtilityBase::ParseData(CString csToken, CString &csTarget, bool bForceStore)
{
	trace(L7, _T("ParseData(<%s>, <%s>)"), csToken, csTarget);

	long rc = 0;

	CString csSourceData, csTargetData, csVariable;

	rc = ResolveSymbol(csToken, csSourceData);
	// Now Parse the Source Data.
	rc = ParsePOSData(csSourceData, csTargetData);
	rc = SaveData(csTargetData, csTarget, bForceStore);

	trace(L7, _T("ParseData() Returns <%d>."), rc);

	return rc;
}

long USSFUtilityBase::ClearData(CString &csTarget, bool bForceStore)
{
	trace(L7, _T("ClearData(<%s>)"), csTarget);

	long rc = 0;

	rc = SaveData(_T(""), csTarget, bForceStore);

	trace(L7, _T("ClearData() Returns <%d>."), rc);

	return rc;
}

// This one is likely to be overridden by different POS implementations.
long USSFUtilityBase::ParsePOSData(CString csSource, CString &csTarget)
{
  trace(L7, _T("ParsePOSData(<%s>, <%s>)"), csSource, csTarget);

  long rc = 0;

  csTarget = _T("");

  for(int i = 0; i < csSource.GetLength(); i++)
  {
	// If we hit a '[', then we need to convert character.
	if (csSource[i] == _T('['))
	{
	  // Need to find "]".
	  CString csTempstr = _T("");
	  i++;
	  while(i < csSource.GetLength())
	  {
		if (csSource[i] == _T(']'))
		{
		  break;
		}
		csTempstr += csSource[i++]; 
	  }

	  if (csTempstr.GetLength() > 0)
	  {
		unsigned char tempchar = _ttoi(csTempstr);
		CString csTemp = tempchar;
		csTarget += csTemp;
	  }
	}
	else
	{
	  TCHAR tempchar = csSource[i];
	  CString csTemp = tempchar;
	  csTarget += csTemp;
	}
  }

  trace(L7, _T("ParsePOSData() Returns <%d>."), rc);

  return rc;
}

int USSFUtilityBase::DoTranslationFromWCToMB( char *ch, const wchar_t *wChar, size_t count,
											 bool bIgnoreNull )
{
	int i=0;
	int noOfChar = 0;	
	
	for( int k=0;k<count;k++ )
	{
		i = wctomb(ch, *wChar);		

		printf("Multibyte Result : %x\n", *ch);
		if( i < 0 )
			return noOfChar;
		else
			noOfChar += i;

		// check if it's null terminator
		if( *ch == '\x0' )
		{
			// if it doesn't ignore the null terminator then returns the number of
			// translated character
			if( !bIgnoreNull )
			{
				return noOfChar;
			}			
		}	
		ch += i;
		
		wChar++;					
		
		printf("NoOf Multibyte Character : %d\n", noOfChar);
	}	
	return noOfChar;
}

void USSFUtilityBase::SetMetaData(CString data)
{
  trace(L7, _T("MetaData: %s."), data);

  //Deprecated!
  SetTransactionVariable(_T("USSF_MetaDataRawElement"), data);
  ///////////////
  SetTransactionVariable(_T("USSF_METAELEMENT"), data);

  //Deprecated!
  {
  CString csTempstr;
  GetTransactionVariable(_T("USSF_MetaDataRawBuffer"), csTempstr);
  csTempstr += data;
  SetTransactionVariable(_T("USSF_MetaDataRawBuffer"), csTempstr);
  }
  ///////////////
  CString csTempstr;
  GetTransactionVariable(_T("USSF_METABUFFER"), csTempstr);
  csTempstr += data;
  SetTransactionVariable(_T("USSF_METABUFFER"), csTempstr);
}

//****************************************************************
long USSFUtilityBase::SendScotMsg(TBDEVICEID nDevice,
                             UINT nMsgLen, // size in bytes, not chars
                             LPCVOID pMsgData,
                             LPARAM lParam)
{
	// this message structure will be deleted by TB's ReleaseMessage 
	PSMessageElement pScotMsg = new SMessageElement;

	UCHAR* pNewMsgData = new UCHAR[nMsgLen+sizeof(_TCHAR)];
	memcpy(pNewMsgData, pMsgData, nMsgLen); 
	for (int n = 0; n < sizeof(_TCHAR); n++)
	{
		pNewMsgData[nMsgLen+n] = 0x00;
	}	

	pScotMsg->id        = ID_TRANSBROKER;
	pScotMsg->nDeviceID = nDevice;
	pScotMsg->nLength   = nMsgLen + sizeof(_TCHAR);
	pScotMsg->pData     = pNewMsgData;
	pScotMsg->lParam    = lParam;  

	if (!hw) return TB_E_INVALIDWINDOW;

	// post to main SCOT thread
	PostMessage(hw,SCOT_MESSAGE,0,(LPARAM) pScotMsg);
	trace(L5,_T("PostScotMsg() msgaddr=0x%08x"), pScotMsg);

	return TB_SUCCESS;
}
// TAR #429050 START
long USSFUtilityBase::BuildUICommand(CString &csCommand, CString csInput, CString csControl, CString csID, CString csData)
{
	//	Build PSXCommand String to contain the following information:
	//
	//	1. Input Type
	//	2. Control Name
	//	3. ID (of Button)
	//	4. Data Name
	//
	//	INPUT={SELECT}	CONTROL={XXXXXXXX}	ID={YYYYYYYY}	DATA={ZZZZZZZZ}
	csCommand.Format(_T("INPUT={%s};CONTROL={%s};ID={%s};DATA={%s};"), csInput, csControl, csID, csData);
	return 0;
}
long USSFUtilityBase::ParseUICommand(CString csCommand, CString &csInput, CString &csControl, CString &csID, CString &csData)
{
	long rc = 0;
	CString csTemp;
	csInput = csControl = csID = csData = _T("");
	rc = csCommand.Find(_T("INPUT={"));
	if (rc > -1)
	{
		csInput = _T("");
		for(int i = rc + 7; i < csCommand.GetLength(); i++)
		{
			if (csCommand[i] == _T('}')||csCommand[i] == _T(';'))
			{
				break;
			}
			csInput += csCommand[i];
		}
	}
	rc = csCommand.Find(_T("CONTROL={"));
	if (rc > -1)
	{
		csControl = _T("");
		for(int i = rc + 9; i < csCommand.GetLength(); i++)
		{
			if (csCommand[i] == _T('}')||csCommand[i] == _T(';'))
			{
				break;
			}
			csControl += csCommand[i];
		}
	}
	rc = csCommand.Find(_T("ID={"));
	if (rc > -1)
	{
		csID = _T("");
		for(int i = rc + 4; i < csCommand.GetLength(); i++)
		{
			if (csCommand[i] == _T('}')||csCommand[i] == _T(';'))
			{
				break;
			}
			csID += csCommand[i];
		}
	}
	rc = csCommand.Find(_T("DATA={"));
	if (rc > -1)
	{
		csData = _T("");
		for(int i = rc + 6; i < csCommand.GetLength(); i++)
		{
			if (csCommand[i] == _T('}')||csCommand[i] == _T(';'))
			{
				break;
			}
			csData += csCommand[i];
		}
	}
	return rc;
}
long USSFUtilityBase::Select(CString csCommand, CString &csReplace)
{
	long rc = 0;
	CString csInput, csControl, csID, csData;
	ParseUICommand(csCommand, csInput, csControl, csID, csData);
	USSFdisp.Select(csControl, csID, csReplace);
	return USSFdisp.Refresh();
}
long USSFUtilityBase::DeSelect(CString csCommand)
{
	CString csInput, csControl, csID, csData;
	ParseUICommand(csCommand, csInput, csControl, csID, csData);
	USSFdisp.DeSelect(csControl, csID);
	return USSFdisp.Refresh();
}
// TAR #429050 STOP

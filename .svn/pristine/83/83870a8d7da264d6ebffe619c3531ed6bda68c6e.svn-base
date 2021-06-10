//
// FILE:  USSFUtilityBase.h
//
// TITLE: 
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
//////////////////////////////////////////////////////////////////////////////////////////////////
//  
// 
#ifndef _USSFUTILITYBASE
#define _USSFUTILITYBASE

#pragma warning(disable: 4786)

// TAR #429050 START
#include "USSFProcedures.h"
// TAR #429050 STOP
#include "DllDefine.h"
#include "ScotMsg.h"
#include <vector>
#include <map>
using namespace std;

//Begin SWR740 - HHRAP performance improvement
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 USSFMetaData
{
public:
	long		lWriteCount;
	long		lReadCount;
	long		lSerial;
	long		lElementID;
	COleVariant	vPreviousValue;
	CString		csReadTime;
	CString		csWriteTime;
	USSFMetaData();
	USSFMetaData(const USSFMetaData &source);
	USSFMetaData& operator=(const USSFMetaData &source);
};
//End SWR740 - HHRAP performance improvement


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  USSFUtilityBase
{
public:
  USSFUtilityBase();
  ~USSFUtilityBase();
  virtual bool TestPSX(void);
  virtual long SetTransactionVariable(CString name, COleVariant value = _T(""));
  virtual long GetTransactionVariable(CString name, CString &value);
  virtual long GetTransactionVariable(CString name, long &value);
  virtual long GetTransactionVariable(CString name, double &value);
  virtual long GetTransactionVariable(CString name, COleVariant &value);
  virtual long ResolveSymbol(CString csToken, CString &csValue, CString csDefault = _T(""));
  virtual long ResolveSymbol(CString csToken, long &lValue, long lDefault = -1);
  virtual long ResolveSymbol(CString csToken, double &lValue, double lDefault = -1);
  virtual CString ResolveSymbol2(CString csToken, CString csDefault = _T(""));
  virtual long FindSymbolValue(CString csToken, CString &csValue, CString csDefault);
  virtual CString GetUserTokenValue(CString csToken, CString csDefault = _T(""));
  virtual long ResetData(CString scope = _T("ALL"));
  virtual long ConcatenateData(CString csToken, CString &csTarget, bool bForceStore = true);
  virtual long StoreData(CString csToken, CString &csTarget, bool bForceStore = true);
  virtual long SaveData(CString csTargetData, CString &csTarget, bool bForceStore = true);
  virtual long StoreParsedData(CString csToken, CString &csTarget, bool bForceStore = true);
  virtual long ParseData(CString csToken, CString &csTarget, bool bForceStore = true);
  virtual long ClearData(CString &csTarget, bool bForceStore = true);
  virtual long ParsePOSData(CString csSource, CString &csTarget);
  virtual void SetMetaData(CString);
  // TAR #429050 START
  virtual long BuildUICommand(CString &csCommand, CString csInput, CString csControl, CString csID, CString csData = _T(""));
  virtual long ParseUICommand(CString csCommand, CString &csInput, CString &csControl, CString &csID, CString &csData);
  virtual long Select(CString csCommand, CString &csReplace);
  virtual long DeSelect(CString csCommand);
  virtual void ScrollUp(CString csCommand){USSFdisp.ScrollUp(csCommand);}
  virtual void ScrollDown(CString csCommand){USSFdisp.ScrollDown(csCommand);}
  virtual void Display(){USSFdisp.Display();}
  virtual void Refresh(){USSFdisp.Refresh();}
  // TAR #429050 STOP

  //Begin SWR740 - HHRAP performance improvement
  virtual long DumpSymbols(bool bMeta = true);
  virtual long DumpSymbolsBySerial(bool bMeta = true);
  virtual long DumpSymbolsByElementID(bool bMeta = true);
  //End SWR740 - HHRAP performance improvement

  virtual int DoTranslationFromWCToMB( char *ch, const wchar_t *wChar, size_t count,
								 bool bIgnoreNull );
  virtual long SendScotMsg(TBDEVICEID nDevice,UINT nMsgLen,LPCVOID pMsgData,LPARAM lParam = NULL);

protected:
  static long lSymbolRecursionDepth;
  static long lSymbolRecursionLimit;

  //Begin SWR740 - HHRAP performance improvement
  map<CString, COleVariant> m_USSF_Symbols;
  map<CString, USSFMetaData> m_USSF_MetaData;
  static long lUSSFVariableSerial;
  static long lUSSFVariableElementID;
  //End SWR740 - HHRAP performance improvement

};
#endif	

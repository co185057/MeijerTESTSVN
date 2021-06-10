//////////////////////////////////////////////////////////////////////////////////////////////////
//  
// 
#ifndef _USSFUTILITY
#define _USSFUTILITY

#include "USSFUtilityBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 USSFUtility : public USSFUtilityBase
{
public:
    USSFUtility();
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
    virtual long BuildUICommand(CString &csCommand, CString csInput, CString csControl, CString csID, CString csData = _T(""));
    virtual long ParseUICommand(CString csCommand, CString &csInput, CString &csControl, CString &csID, CString &csData);
    virtual long Select(CString csCommand, CString &csReplace);
    virtual long DeSelect(CString csCommand);
    virtual void ScrollUp(CString csCommand);
    virtual void ScrollDown(CString csCommand);
    virtual void Display();
    virtual void Refresh();
    virtual long DumpSymbols(bool bMeta = true);
    virtual long DumpSymbolsBySerial(bool bMeta = true);
    virtual long DumpSymbolsByElementID(bool bMeta = true);
    virtual int DoTranslationFromWCToMB( char *ch, const wchar_t *wChar, size_t count, bool bIgnoreNull );
    virtual long SendScotMsg(TBDEVICEID nDevice,UINT nMsgLen,LPCVOID pMsgData,LPARAM lParam = NULL);

};
#endif	
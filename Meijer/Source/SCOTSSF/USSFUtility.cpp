#include "stdafx.h"                 // MS MFC always needed first
//                     // MS MFC template header
#include <afxpriv.h>                // for USES_CONVERSION
#include <afxdisp.h>
// TAR #429050 START
#include "Common.h"
#include "USSFCommon.h"
// TAR #425090 STOP

USSFUtility::USSFUtility()
{
}

bool USSFUtility::TestPSX(void)
{
    return USSFUtilityBase::TestPSX();
}

long USSFUtility::SetTransactionVariable(CString name, COleVariant value)
{
    return USSFUtilityBase::SetTransactionVariable(name, value );
}

long USSFUtility::GetTransactionVariable(CString name, CString &value)
{
    return USSFUtilityBase::GetTransactionVariable(name, value);
}

long USSFUtility::GetTransactionVariable(CString name, long &value)
{
    return USSFUtilityBase::GetTransactionVariable(name, value);
}

long USSFUtility::GetTransactionVariable(CString name, double &value)
{
    return USSFUtilityBase::GetTransactionVariable(name, value);
}

long USSFUtility::GetTransactionVariable(CString name, COleVariant &value)
{
    return USSFUtilityBase::GetTransactionVariable(name, value);
}

long USSFUtility::ResolveSymbol(CString csToken, CString &csValue, CString csDefault )
{
    return USSFUtilityBase::ResolveSymbol(csToken, csValue, csDefault );
}

long USSFUtility::ResolveSymbol(CString csToken, long &lValue, long lDefault )
{
    return USSFUtilityBase::ResolveSymbol(csToken, lValue, lDefault );
}

long USSFUtility::ResolveSymbol(CString csToken, double &lValue, double lDefault )
{
    return USSFUtilityBase::ResolveSymbol(csToken, lValue, lDefault );
}

CString USSFUtility::ResolveSymbol2(CString csToken, CString csDefault)
{
    return USSFUtilityBase::ResolveSymbol2(csToken, csDefault );
}

long USSFUtility::FindSymbolValue(CString csToken, CString &csValue, CString csDefault)
{
    return USSFUtilityBase::FindSymbolValue(csToken, csValue, csDefault);
}

CString USSFUtility::GetUserTokenValue(CString csToken, CString csDefault )
{
    return USSFUtilityBase::GetUserTokenValue(csToken, csDefault );
}

long USSFUtility::ResetData(CString scope)
{
    return USSFUtilityBase::ResetData(scope );
}

long USSFUtility::ConcatenateData(CString csToken, CString &csTarget, bool bForceStore )
{
    return USSFUtilityBase::ConcatenateData(csToken, csTarget, bForceStore );
}

long USSFUtility::StoreData(CString csToken, CString &csTarget, bool bForceStore )
{
    return USSFUtilityBase::StoreData(csToken, csTarget, bForceStore );
}

long USSFUtility::SaveData(CString csTargetData, CString &csTarget, bool bForceStore )
{
    return USSFUtilityBase::SaveData(csTargetData, csTarget, bForceStore );
}

long USSFUtility::StoreParsedData(CString csToken, CString &csTarget, bool bForceStore )
{
    return USSFUtilityBase::StoreParsedData(csToken, csTarget, bForceStore );
}

long USSFUtility::ParseData(CString csToken, CString &csTarget, bool bForceStore )
{
    return USSFUtilityBase::ParseData(csToken, csTarget, bForceStore );
}

long USSFUtility::ClearData(CString &csTarget, bool bForceStore )
{
    return USSFUtilityBase::ClearData(csTarget, bForceStore );
}

long USSFUtility::ParsePOSData(CString csSource, CString &csTarget)
{
    return USSFUtilityBase::ParsePOSData(csSource, csTarget);
}

void USSFUtility::SetMetaData(CString data)
{
    USSFUtilityBase::SetMetaData(data);
}

long USSFUtility::BuildUICommand(CString &csCommand, CString csInput, CString csControl, CString csID, CString csData )
{
    return USSFUtilityBase::BuildUICommand(csCommand, csInput, csControl, csID, csData );
}

long USSFUtility::ParseUICommand(CString csCommand, CString &csInput, CString &csControl, CString &csID, CString &csData)
{
    return USSFUtilityBase::ParseUICommand(csCommand, csInput, csControl, csID, csData);
}

long USSFUtility::Select(CString csCommand, CString &csReplace)
{
    return USSFUtilityBase::Select(csCommand, csReplace);
}

long USSFUtility::DeSelect(CString csCommand)
{
    return USSFUtilityBase::DeSelect(csCommand);
}

void USSFUtility::ScrollUp(CString csCommand)
{
    USSFUtilityBase::ScrollUp(csCommand);
}

void USSFUtility::ScrollDown(CString csCommand)
{
    USSFUtilityBase::ScrollDown(csCommand);
}

void USSFUtility::Display()
{
    USSFUtilityBase::Display();
}

void USSFUtility::Refresh()
{
    USSFUtilityBase::Refresh();
}

long USSFUtility::DumpSymbols(bool bMeta)
{
    return USSFUtilityBase::DumpSymbols(bMeta );
}

long USSFUtility::DumpSymbolsBySerial(bool bMeta)
{
    return USSFUtilityBase::DumpSymbolsBySerial(bMeta );
}

long USSFUtility::DumpSymbolsByElementID(bool bMeta )
{
    return USSFUtilityBase::DumpSymbolsByElementID(bMeta );
}

int USSFUtility::DoTranslationFromWCToMB( char *ch, const wchar_t *wChar, size_t count, bool bIgnoreNull )
{
    return USSFUtilityBase::DoTranslationFromWCToMB( ch, wChar, count, bIgnoreNull );
}

long USSFUtility::SendScotMsg(TBDEVICEID nDevice,UINT nMsgLen,LPCVOID pMsgData,LPARAM lParam)
{
    return USSFUtilityBase::SendScotMsg(nDevice, nMsgLen, pMsgData, lParam );
}
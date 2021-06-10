// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayAgeRestrictionInstructionText.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayAgeRestrictionInstructionText.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
 *
 * Copyright 2004 by NCR Corporation,
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayAgeRestrictionInstructionText.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 9     6/05/08 8:28a Bs185067
 * TAR 376639
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     3/24/05 7:20p Dm185016
 * UNICODE
 * 
 * 3     3/21/05 4:41p Dm185016
 * Added more trace.
 * 
 * 2     2/28/05 4:10p Dm185016
 * lint
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 2     1/18/05 3:44p Dm185016
// UNICODE support
// 
// 1     1/07/05 2:27p Dm185016
// TAR 287222, TAR 288789
*/
#include "StdAfx.h"


#include "DisplayAgeRestrictionInstructionText.h"
#include "RemoteAPMgr.h"
#include "RCMKeyValue.h"
#include "Message.h"
#include "LocaleEx.h"

using namespace rap;
using namespace fastlane;
using namespace utils;


CDisplayAgeRestrictionInstructionText::CDisplayAgeRestrictionInstructionText()
{
    CRCMActionBase::SetName(_T("CDisplayAgeRestrictionInstructionText"));
}


CDisplayAgeRestrictionInstructionText::~CDisplayAgeRestrictionInstructionText()
{
}


HRESULT CDisplayAgeRestrictionInstructionText::FormatInstructionText(StringType &sOutput, const _TCHAR *pszText)
{
    IMODTRACE_DEVELOP();

    HRESULT hr = S_OK;

    IString sDateFormat = (const _TCHAR *)CLocale::GetDateFormat(GetTraceHandle());
    IString sText;
    sText.format(pszText, (const _TCHAR *)sDateFormat);

    sOutput = (const _TCHAR *)sText;

    ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Date format:  ") + sDateFormat);
    ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Text:  ") + sText);

    return hr;
}


HRESULT CDisplayAgeRestrictionInstructionText::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

    return DisplayInstructionText();
}


template <>
IAction* CCreateable<CDisplayAgeRestrictionInstructionText, IAction>::NewInstance()
{
    IAction *pAction = new CDisplayAgeRestrictionInstructionText;

    return pAction;
}


template <>
void CCreateable<CDisplayAgeRestrictionInstructionText, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(DisplayAgeRestrictionInstructionText)
DEFINE_ACTION_RELEASE_FXN(DisplayAgeRestrictionInstructionText)

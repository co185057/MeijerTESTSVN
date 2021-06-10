// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/MediaCommand.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)MediaCommand.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/MediaCommand.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     3/31/05 3:38p Tp151000
 * TAR 297101
 * 
 * 3     3/31/05 2:25p Dm185016
 * TAR 297138
 * 
 * 2     3/22/05 4:07p Dm185016
 * UNICODE
 * 
 * 1     3/11/05 2:44p Dm185016
 * 
 * 2     2/28/05 4:38p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 9     1/18/05 3:51p Dm185016
// UNICODE support
// 
// 8     1/12/05 6:01p Dm185016
// Cleaned up trace log.
// 
// 7     12/16/04 10:07a Dm185016
// Problem with iterator copy ctor.
// 
// 6     12/14/04 11:26a Dm185016
// 
// 5     12/07/04 4:22p Dm185016
// Fixed for lane status window
// 
// 4     12/02/04 3:43p Dm185016
// Change SetConfigProperty to SetControlProperty for redraw per Dan.
// 
// 3     11/30/04 4:30p Dm185016
// Fixed receipt source for media messages.
// 
// 2     11/02/04 3:10p Dm185016
// More updates
// 
// 1     10/29/04 9:04a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "MediaCommand.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "PSXMfc.h"
#include "KVParmList.h"
#include "TopListItem.h"
#include "SMError.h"
#include "verifypsx.h"


BEGIN_KV_HANDLER_CLASS(CMediaCommand::MCParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CMediaCommand::MCParms, KEY_EV_DATA_SOURCE, DataSource)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CMediaCommand::MCParms, DataSource)


void CMediaCommand::MCParms::Clear()
{
    m_DataSource.Empty();
}


CMediaCommand::CMediaCommand()
{
    CRCMActionBase::SetName(_T("CMediaCommand"));
}


CMediaCommand::~CMediaCommand()
{
}


// Get all control names from the xml
HRESULT CMediaCommand::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();
    CParameterList::iterator end    = parms.end();
    
    m_parms.Clear();
    hr = m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    hr = CPSXAction::NextOperands(iter, end);

    return hr;
}


HRESULT CMediaCommand::Execute(bool *pResult, IMessage &message)
{
	HRESULT hr = ParseOperands();
    VERIFY_PSX_AND_LOG(psx().ClearReceipt( GetDataSource() ));

    VERIFY_PSX_AND_LOG(psx().SetControlProperty(UI::CONTROLDISPLAY, UI::PROPERTYREDRAW, VARIANT_FALSE));

    _ASSERTE( _CrtCheckMemory() );

    IString msgData((const _TCHAR *)message.GetData());
    hr = ProcessMediaOutput((_TCHAR *)msgData);

    _ASSERTE( _CrtCheckMemory() );

    VERIFY_PSX_AND_LOG(psx().SetControlProperty(UI::CONTROLDISPLAY, UI::PROPERTYREDRAW, VARIANT_TRUE));
    VERIFY_PSX_AND_LOG(psx().UpdateReceiptControls( GetDataSource() ));
    *pResult = true;

    return hr;
}


const size_t KEY_EV_TYPE_ADD_OTHER_EVENT_MEDIA_LENGTH = _tcslen(KEY_EV_TYPE_ADD_OTHER_EVENT_MEDIA);


HRESULT CMediaCommand::ProcessMediaOutput(const _TCHAR* pMsgData)
{
    HRESULT hr = S_OK;

	CString cdata(pMsgData);
	cdata = cdata.Mid((int)KEY_EV_TYPE_ADD_OTHER_EVENT_MEDIA_LENGTH);
	IString msgdata((const _TCHAR *)cdata, cdata.GetLength());

    _ASSERTE( _CrtCheckMemory() );
	IString::token_iterator &iter = msgdata.tokens(_T(";"));

    _ASSERTE( _CrtCheckMemory() );
    for(;iter != msgdata.last_token(); iter++)
    {
        CTopListItem item(GetTraceHandle(), (*iter).second);

        hr = AddReceiptLine(item, false);

        _ASSERTE( _CrtCheckMemory() );
    }

    return hr;
}


template <>
IAction* CCreateable<CMediaCommand, IAction>::NewInstance()
{
    IAction *pAction = new CMediaCommand;

    return pAction;
}


template <>
void CCreateable<CMediaCommand, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(MediaCommand)
DEFINE_ACTION_RELEASE_FXN(MediaCommand)

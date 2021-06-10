//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMExtensionState.CPP
//
// TITLE: Class implementation for customer mode Data Entry state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 

#include "SMState.h"                
#include "SMExtensionState.h"	    

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SMExtensionState")

IMPLEMENT_DYNCREATE(SMExtensionState, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMExtensionState::SMExtensionState()
{
  IMPLEMENT_TIMESTAMP
}

SMExtensionState::SMExtensionState(const int nStateMode, const bool bHoldSecWeight,const int nTimeOutSec)
{
  SMExtensionStateBase::Deliver(nStateMode, bHoldSecWeight, nTimeOutSec);
}

SMStateBase  *SMExtensionState::Initialize(void)
{
    return SMExtensionStateBase::Initialize();
}

void SMExtensionState::UnInitialize(void)
{
    SMExtensionStateBase::UnInitialize();
}

bool SMExtensionState::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMExtensionStateBase::DeliverDescriptorList(dlParameterList);
}

bool SMExtensionState::ExecuteExtensionUpdate(psPropChangeRequestList pRequestListFromTB)
{
    return SMExtensionStateBase::ExecuteExtensionUpdate(pRequestListFromTB);
}

int SMExtensionState::GetDevPropertyID(const CString& csPropName)
{
    return SMExtensionStateBase::GetDevPropertyID(csPropName);
}

int SMExtensionState::GetIndexFromList(const CString& csPropName, int nListSize, sPropIDNode* pList)
{
    return SMExtensionStateBase::GetIndexFromList(csPropName, nListSize, pList);
}

bool SMExtensionState::DispatchExtenstionUpdateRequest(const CString& csPropName, const CString& csSubpropName, const CString& csSubpropValue)
{
    return SMExtensionStateBase::DispatchExtenstionUpdateRequest(csPropName, csSubpropName, csSubpropValue);
}

bool SMExtensionState::storeMode()
{
    return SMExtensionStateBase::storeMode();
}

bool SMExtensionState::helpMode()
{
    return SMExtensionStateBase::helpMode();
}

bool SMExtensionState::CustomerMode()
{
    return SMExtensionStateBase::CustomerMode();
}

SMStateBase  *SMExtensionState::PSNumericKey(const long lKey)
{
    return SMExtensionStateBase::PSNumericKey(lKey);
}

SMStateBase  *SMExtensionState::PSOtherSpecialKeys(const long ch)
{
    return SMExtensionStateBase::PSOtherSpecialKeys(ch);
}

SMStateBase  *SMExtensionState::PSEnterKey(void)
{
    return SMExtensionStateBase::PSEnterKey();
}

SMStateBase  *SMExtensionState::PSDecimalKey(const long ch)
{
    return SMExtensionStateBase::PSDecimalKey(ch);
}

SMStateBase  *SMExtensionState::PSBackSpaceKey(void)
{
    return SMExtensionStateBase::PSBackSpaceKey();
}

SMStateBase  *SMExtensionState::PSShiftKey(void)
{
    return SMExtensionStateBase::PSShiftKey();
}

SMStateBase  *SMExtensionState::PSAlphaKey(const long lKey)
{
    return SMExtensionStateBase::PSAlphaKey(lKey);
}

SMStateBase  *SMExtensionState::PSAlphaNumericKey(const long lKey)
{
    return SMExtensionStateBase::PSAlphaNumericKey(lKey);
}

SMStateBase  *SMExtensionState::PSButtonGoBack(void)
{
    return SMExtensionStateBase::PSButtonGoBack();
}

SMStateBase  *SMExtensionState::PSClearKey(void)
{
    return SMExtensionStateBase::PSClearKey();
}

SMStateBase  *SMExtensionState::PSSpaceKey(void)
{
    return SMExtensionStateBase::PSSpaceKey();
}

SMStateBase  *SMExtensionState::DMScanner()
{
    return SMExtensionStateBase::DMScanner();
}

SMStateBase  *SMExtensionState::DMCardReader()
{
    return SMExtensionStateBase::DMCardReader();
}

SMStateBase  *SMExtensionState::DMAcceptor()
{
    return SMExtensionStateBase::DMAcceptor();
}

SMStateBase  *SMExtensionState::TimedOut(void)
{
    return SMExtensionStateBase::TimedOut();
}

SMStateBase  *SMExtensionState::OnWtIncreaseNotAllowed(void)
{
    return SMExtensionStateBase::OnWtIncreaseNotAllowed();
}

SMStateBase  *SMExtensionState::OnWtDecrease(void)
{
    return SMExtensionStateBase::OnWtDecrease();
}

SMStateBase  *SMExtensionState::OnMatchedWt(void)
{
    return SMExtensionStateBase::OnMatchedWt();
}

bool SMExtensionState::ExecuteDeviceUnknownPoperty(const CString &csPropertyName, key_value_pair_t &ItemPairList)
{
    return SMExtensionStateBase::ExecuteDeviceUnknownPoperty(csPropertyName, ItemPairList);
}

bool SMExtensionState::ExecuteDeviceUnknownSubPoperty(const CString &csPropertyName, const CString &csKey, const CString &csValue)
{
    return SMExtensionStateBase::ExecuteDeviceUnknownSubPoperty(csPropertyName, csKey, csValue);
}

bool SMExtensionState::ExecuteUnknownRequestType(int nRequestType, const CString& csPropName, const CString &csSubPropName, const CString &csSubPropValue)
{
    return SMExtensionStateBase::ExecuteUnknownRequestType(nRequestType, csPropName, csSubPropName, csSubPropValue);
}

bool SMExtensionState::ExecuteUIPropertyChangeRequest(const CString& csControlName, const CString &csKey, const CString &csValue)
{
    return SMExtensionStateBase::ExecuteUIPropertyChangeRequest(csControlName, csKey, csValue);
}

bool SMExtensionState::ExecuteDeviceCashAcceptorSubProperty(const CString &csKey, const CString &csValue)
{
    return SMExtensionStateBase::ExecuteDeviceCashAcceptorSubProperty(csKey, csValue);
}

bool SMExtensionState::ExecuteDeviceCashAcceptorProperty(const CString &csPropertyName, key_value_pair_t& ItemPairList)
{
    return SMExtensionStateBase::ExecuteDeviceCashAcceptorProperty(csPropertyName, ItemPairList);
}

bool SMExtensionState::ExecuteDeviceSecuritySubProperty(const CString &csKey, const CString &csValue)
{
    return SMExtensionStateBase::ExecuteDeviceSecuritySubProperty(csKey, csValue);
}

bool SMExtensionState::ExecuteDeviceSecurityProperty(const CString &csPropertyName, key_value_pair_t& ItemPairList)
{
    return SMExtensionStateBase::ExecuteDeviceSecurityProperty(csPropertyName, ItemPairList);
}

bool SMExtensionState::ExecuteDeviceScannerSubProperty(const CString& csName, const CString& csValue)
{
    return SMExtensionStateBase::ExecuteDeviceScannerSubProperty(csName, csValue);
}

bool SMExtensionState::ExecuteDeviceScannerProperty(const CString &csPropertyName, key_value_pair_t& ItemPairList)
{
    return SMExtensionStateBase::ExecuteDeviceScannerProperty(csPropertyName, ItemPairList);
}

bool SMExtensionState::ExecuteDeviceMSRSubProperty(const CString& csKey, const CString& csValue)
{
    return SMExtensionStateBase::ExecuteDeviceMSRSubProperty(csKey, csValue);
}

bool SMExtensionState::ExecuteDeviceMSRProperty(const CString &csPropertyName, key_value_pair_t& ItemPairList)
{
    return SMExtensionStateBase::ExecuteDeviceMSRProperty(csPropertyName, ItemPairList);
}

bool SMExtensionState::ExecuteExtensionStateSubProperty(const CString &csKey, const CString &csValue)
{
    return SMExtensionStateBase::ExecuteExtensionStateSubProperty(csKey, csValue);
}

bool SMExtensionState::ExecuteExtensionStateProperty(const CString &csPropertyName, key_value_pair_t &ItemPairList)
{
    return SMExtensionStateBase::ExecuteExtensionStateProperty(csPropertyName, ItemPairList);
}

bool SMExtensionState::ExecuteDeviceContext(const CString& csCtrxbame)
{
    return SMExtensionStateBase::ExecuteDeviceContext(csCtrxbame);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMExtensionState.h
//
// TITLE: Class header for customer mode Data Entry state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMEXTENSIONSTATE_H
#define _SMEXTENSIONSTATE_H

#include "SMExtensionStateBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMExtensionState : public SMExtensionStateBase
{
public:
    SMExtensionState();
    ~SMExtensionState(){};
    SMExtensionState(const int nStateMode, const bool bHoldSecWeight,const int nTimeOutSec);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual bool ExecuteExtensionUpdate(psPropChangeRequestList pRequestListFromTB);
    virtual SMStateBase* Initialize();
    virtual void UnInitialize();
    virtual int  GetDevPropertyID(const CString& csPropName);
    virtual int  GetIndexFromList(const CString& csPropName, int nListSize, sPropIDNode* pList);
    virtual bool DispatchExtenstionUpdateRequest(const CString& csPropName, const CString& csSubpropName, const CString& csSubpropValue);
    virtual bool storeMode();
    virtual bool helpMode();
    virtual bool CustomerMode();

protected:
    virtual SMStateBase  *PSNumericKey(const long);
    virtual SMStateBase  *PSEnterKey(void);
    virtual SMStateBase  *PSDecimalKey(const long ch);
    virtual SMStateBase  *PSOtherSpecialKeys(const long);
    virtual SMStateBase  *PSBackSpaceKey(void);
    virtual SMStateBase  *PSShiftKey(void);
    virtual SMStateBase  *PSAlphaKey(const long lKey);
    virtual SMStateBase  *PSAlphaNumericKey(const long lKey);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *PSSpaceKey(void);
    virtual SMStateBase  *DMScanner();
    virtual SMStateBase  *DMCardReader();
    virtual SMStateBase  *DMAcceptor();
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnMatchedWt(void);
    virtual bool ExecuteDeviceUnknownPoperty(const CString &csPropertyName, key_value_pair_t &ItemPairList);
    virtual bool ExecuteDeviceUnknownSubPoperty(const CString &csPropertyName, const CString &csKey, const CString &csValue);
    virtual bool ExecuteUnknownRequestType(int nRequestType, const CString& csPropName, const CString &csSubPropName, const CString &csSubPropValue);
    virtual bool ExecuteUIPropertyChangeRequest(const CString& csControlName, const CString &csKey, const CString &csValue);
    virtual bool ExecuteDeviceCashAcceptorSubProperty(const CString &csKey, const CString &csValue);
    virtual bool ExecuteDeviceCashAcceptorProperty(const CString &csPropertyName, key_value_pair_t& ItemPairList);
    virtual bool ExecuteDeviceSecuritySubProperty(const CString &csKey, const CString &csValue);
    virtual bool ExecuteDeviceSecurityProperty(const CString &csPropertyName, key_value_pair_t& ItemPairList);
    virtual bool ExecuteDeviceScannerSubProperty(const CString& csName, const CString& csValue);
    virtual bool ExecuteDeviceScannerProperty(const CString &csPropertyName, key_value_pair_t& ItemPairList);
    virtual bool ExecuteDeviceMSRSubProperty(const CString& csKey, const CString& csValue);
    virtual bool ExecuteDeviceMSRProperty(const CString &csPropertyName, key_value_pair_t& ItemPairList);
    virtual bool ExecuteExtensionStateSubProperty(const CString &csKey, const CString &csValue);
    virtual bool ExecuteExtensionStateProperty(const CString &csPropertyName, key_value_pair_t &ItemPairList);
    virtual bool ExecuteDeviceContext(const CString& csCtrxbame);

    DECLARE_DYNCREATE(SMExtensionState)	// MFC Runtime class/object information
};

#endif   // _SMEXTENSIONSTATE_H

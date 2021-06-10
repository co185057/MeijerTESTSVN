// SMExtensionStateBase.h: interface for the SMExtensionStateBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMEXTENSIONSTATEBASE_H_)
#define AFX_SMEXTENSIONSTATEBASE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SMState.h"
#include "SMCustomerModeState.h"

//#include "SCUDefs.h"

typedef struct sPropIDNodeT {
   long key;
   LPCTSTR szname;
} sPropIDNode;

typedef enum KnownProperty_t
{
	DEVPROP_MSR1 = 1,  
	DEVPROP_SCANNER1 = 2,
	DEVPROP_CASHACCEPTOR = 3,
	DEVPROP_SECURITY = 4,
	DEVPROP_PRODUCESCALE = 5,
	DEVPROP_EXSTATE = 6,
	DEVPROP_UNKNOWN = 200,  // default property parser
	DEVPROP_LAST
} DEVPropertyID;

typedef enum SupportedExStateCommandList_t
{
	EXSTATE_CMD_PLAYWAVEFILE = 1,
	EXSTATE_CMD_PLAYWAVEFROMID = 2,
	EXSTATE_CMD_SETTIMEOUT = 3,
	EXSTATE_CMD_LAST
} EXSTATE_CMDLIST_TYPE;






#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMExtensionStateBase : public SMCustomerModeState
{
public:
	SMExtensionStateBase(); 
	//For nStateMode,  1 = customer, 2 = help, 3 = store, other/default = customer
	SMExtensionStateBase(const int nStateMode, const bool bHoldSecWeight, const int nTimeOutSec); 
	SMStateBase* Deliver(const int nStateMode, const bool bHoldSecWeight, const int nTimeOutSec);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
	virtual ~SMExtensionStateBase();
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
    virtual SMStateBase  *PSNumericKey(const long);	// a key 0-9 was pressed
    virtual SMStateBase  *PSEnterKey(void);        // override for enter key
    virtual SMStateBase  *PSDecimalKey(const long ch);
    virtual SMStateBase  *PSOtherSpecialKeys(const long);
	virtual SMStateBase  *PSBackSpaceKey(void);
	virtual SMStateBase  *PSShiftKey(void);
    virtual SMStateBase  *PSAlphaKey(const long lKey);
	virtual SMStateBase  *PSAlphaNumericKey(const long lKey);
    virtual SMStateBase  *PSButtonGoBack(void);        // override for enter key
    virtual SMStateBase  *PSClearKey(void);        // override for clear key
    virtual SMStateBase  *PSSpaceKey(void);        // override for space key
    virtual SMStateBase  *DMScanner();             // on barcode scan
    virtual SMStateBase  *DMCardReader();          // on MSR read
    virtual SMStateBase  *DMAcceptor();            // on cash or coin insert

    virtual SMStateBase  *TimedOut(void);          // override for time out 
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnMatchedWt(void);		// Smart Scale reported a matched weight



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

	DECLARE_DYNCREATE(SMExtensionStateBase)// MFC Runtime class/object information

	// data members
	bool fCapsInput;
	bool bShiftOn;
	int  m_nStateMode; 
	bool m_bHoldSecWt;

};

#endif // !defined(AFX_SMEXTENSIONSTATEBASE_H_)

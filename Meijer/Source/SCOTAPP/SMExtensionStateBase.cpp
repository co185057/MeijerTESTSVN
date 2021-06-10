// SMExtensionStateBase.cpp: implementation of the SMExtensionStateBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"
//#include "PSProcedures.h"

#include "SMExtensionStateBase.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



#define COMP_ID ID_SM              
#define T_ID _T("SMExtensionStateBase")

IMPLEMENT_DYNCREATE(SMExtensionStateBase, CObject)
const sPropIDNode ExtStateCmdList[]=
{
	{0, EMPTY_STRING},
   {EXSTATE_CMD_PLAYWAVEFILE,		 _T("PLAYWAVE")},
   {EXSTATE_CMD_PLAYWAVEFROMID,      _T("PLAYWAVEFROMID")},
   {EXSTATE_CMD_SETTIMEOUT,		     _T("SETTIMEOUT")}
};

const sPropIDNode ExtDevicePropertyList[]= 
{
	{0, EMPTY_STRING},
   {DEVPROP_MSR1,            _T("MSR1")},
   {DEVPROP_SCANNER1,        _T("SCANNER")},
   {DEVPROP_CASHACCEPTOR,    _T("CASHACCEPTOR")},
   {DEVPROP_SECURITY,        _T("SECURITY")},
   {DEVPROP_PRODUCESCALE,    _T("PRODUCESCALE")},
   {DEVPROP_EXSTATE,         _T("EXTENSIONSTATE")},
   {DEVPROP_UNKNOWN,         _T("ANYOTHERTHING")},
   {DEVPROP_LAST,            _T("LAST")}
};

int EXPROPERTYLIST_SIZE = ((sizeof(ExtDevicePropertyList))/(sizeof(sPropIDNode)));

int EXTSTATECMDLIST_SIZE = ((sizeof(ExtStateCmdList))/(sizeof(sPropIDNode)));




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// initialize the file name before using it if needs a change
#define DEVICECTX_FILENAME _T("%APP_DRIVE%\\scot\\config\\DeviceContexts.xml")
CDeviceCtxStoreXml gXMLDeviceCTXObj; 


SMExtensionStateBase::SMExtensionStateBase(const int nStateMode, const bool bHoldSecWeight, const int nTimeOutSec)
{
	// nStateMode decides customer mode or store mode or helpmode
	//// 1 = customer, 2 = help, 3 = store, other/default = customer
	// bHoldSecWeight flag requests to hold security weight or not for this TB and trx specific state
	m_nStateMode = nStateMode;
	m_bHoldSecWt = bHoldSecWeight;
	nTimeOut = nTimeOutSec;
	gXMLDeviceCTXObj.SetFileName(DEVICECTX_FILENAME);
}

SMExtensionStateBase::SMExtensionStateBase()
{
	m_nStateMode = 1;
	m_bHoldSecWt = false;
	gXMLDeviceCTXObj.SetFileName(DEVICECTX_FILENAME);
}


SMStateBase  *SMExtensionStateBase::Deliver(const int nStateMode, const bool bHoldSecWeight, const int nTimeOutSec)
{
	m_nStateMode = nStateMode;
	m_bHoldSecWt = bHoldSecWeight;
	nTimeOut = nTimeOutSec;

	return this;
}
/////////////////////////////////////////
bool SMExtensionStateBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    
    // There should be three parameters, two bools passed as word and an int
    if (dlParameterList.GetDescriptorCount() == 3)
    {
        trace(L6, _T("+SMExtensionStateBase::DeliverDescriptorList() - Parameter Count of 3"));
        Deliver((int)DLParamDWORD(dlParameterList),    // nStateMode
                (bool)DLParamWORD(dlParameterList),    // bHoldSecWeight
                (int)DLParamDWORD(dlParameterList));   // nTimeOutSec
        return true;
    }
    else
    {
        trace(L2, _T("+SMExtensionStateBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}



SMExtensionStateBase::~SMExtensionStateBase()
{

}

SMStateBase* SMExtensionStateBase::Initialize()
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ExtensionState") );	// RFC 330050
	SMStateBase* pRetState = STATE_NULL;
	if (m_bHoldSecWt)
	{
        CKeyValueParmList parmList;
        CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_HOLD);
        _bstr_t bstrParmList((const _TCHAR *)parmList);
        BSTR bstrResult;
        bstrResult = NULL;
        SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
        SysFreeString(bstrResult);
	}

	//TAR375770+
	BEST preAnchorState = getAnchorState();
	//TAR375770-
	setAnchorState(BES_EXTENSIONSTATE);
	
	// get UI context from TB and set it
	// get device context specified in UI context/customdata tag or from TB
	sPropChangeRequestList ExtRuntimeChanges;
	ExtRuntimeChanges.InitStruct();
	_TCHAR szUIContext[300]     =  {NULL};
	_TCHAR szDeviceContext[300] =  {NULL};
	TBRC nTBRetCode = tb.GetExtensionStateSetup(szUIContext, szDeviceContext, &ExtRuntimeChanges);
	if (nTBRetCode == TB_E_FUNCTIONNOTSUPPORTED)
	{
		//TAR375770+
		trace(L5, _T("TB_E_FUNCTIONNOTSUPPORTED, go to previous anchor state"));
		return setAndCreateAnchorState(preAnchorState);
		//TAR375770-
	}
    
    //TAR372975+
    trace(L5, _T("ExtensionState UIcontext is %s"), szUIContext);
    if (!szUIContext|| !_tcslen(szUIContext)) //no context for this state
    {
	   //TAR375770+
       trace(L5, _T("ExtensionState context is empty, go to anchor state"));
       return setAndCreateAnchorState(preAnchorState);
		//TAR375770-
	}
    //TAR372975-

	if (!szDeviceContext || !_tcslen(szDeviceContext))
	{
		// retrieve device context from UI context specification
		//CString strValue( _T("") );
		CString strValue;
		static const TCHAR s_XMLSTRINGID[] = _T("DeviceContextName");
		static const TCHAR s_XMLCONTROLID[] = _T("CustomData");
		if( PSX_SUCCESS == (ps.GetPSXObject())->GetCustomDataVar( s_XMLSTRINGID, strValue, s_XMLCONTROLID,  szUIContext) )
		{
			if( strValue.GetLength() )
			{
				 _tcscpy(szDeviceContext, strValue);
			}

		}

	}
	ps.SetFrame(szUIContext);					// setup UI
	ExecuteDeviceContext(szDeviceContext);		// setup device context
	ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
	ps.ShowFrame(false);

	// call ExecuteExtensionUpdate() after showframe
	// Below call makes temporary changes to UI and device context
	ExecuteExtensionUpdate(&ExtRuntimeChanges); // apply changes to device or UI context as specified by TB

	return pRetState;
}

void SMExtensionStateBase::UnInitialize()
{
	if (m_bHoldSecWt)
	{
        CKeyValueParmList parmList;
        CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_RESUME);
        _bstr_t bstrParmList((const _TCHAR *)parmList);
        BSTR bstrResult;
        bstrResult = NULL;
        SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
        SysFreeString(bstrResult);
	}
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ExtensionState") );	// RFC 330050
}


bool SMExtensionStateBase::ExecuteDeviceContext(const CString &csCtrxbame)
{
	bool bRetValue = false;
	if (!csCtrxbame.GetLength())
	{
		return bRetValue;
	}
	// load device context and execute the context by getting property list and iterating on properties
	gXMLDeviceCTXObj.SetCurrentContext(csCtrxbame);
	CStringArray csDPropertyList;
	gXMLDeviceCTXObj.GetAllDevicePropertyNames(csDPropertyList);
	int nCount = csDPropertyList.GetSize();
	key_value_pair_t propPairsList;
	for(int index = 0; (index < nCount); ++ index)
	{
		// retrieve property name
		CString csPropName = csDPropertyList[index];
		gXMLDeviceCTXObj.SetCurrentDeviceProperty(csPropName);
		gXMLDeviceCTXObj.GetAllItemsForCurrentDevicePropetry(propPairsList);
		int nCurrentID = GetDevPropertyID(csPropName);
		switch (nCurrentID)
		{
			case DEVPROP_MSR1:
				ExecuteDeviceMSRProperty(csPropName, propPairsList);
				break;
			case DEVPROP_SCANNER1:
				ExecuteDeviceScannerProperty(csPropName, propPairsList);
				break;
			case DEVPROP_CASHACCEPTOR:
				ExecuteDeviceCashAcceptorProperty(csPropName, propPairsList);
				break;
			case DEVPROP_SECURITY:
				ExecuteDeviceSecurityProperty(csPropName, propPairsList);
				break;
			case DEVPROP_PRODUCESCALE:
				//ExecuteDeviceProduceScaleProperty(csPropName, propPairsList); // for future addition
				break;
			case DEVPROP_EXSTATE:
				ExecuteExtensionStateProperty(csPropName, propPairsList);
				break;

			default:
			case DEVPROP_UNKNOWN:
				ExecuteDeviceUnknownPoperty(csPropName, propPairsList);
				break;
		}
		propPairsList.RemoveAll();
	}
	csDPropertyList.RemoveAll();
	return bRetValue;
}

bool SMExtensionStateBase::ExecuteDeviceMSRProperty(const CString &csPropertyName, key_value_pair_t &ItemPairList)
{
	bool bRetValue = false;
	// int nCount = ItemPairList.GetCount(); // only for tracing purpose
	POSITION nPos = ItemPairList.GetStartPosition();
	while (nPos != NULL)
	{
		CString csKey, csValue;
		ItemPairList.GetNextAssoc(nPos, csKey, csValue);
		bRetValue = ExecuteDeviceMSRSubProperty(csKey, csValue);
	}
	return bRetValue;
}


bool SMExtensionStateBase::ExecuteDeviceMSRSubProperty(const CString &csKey, const CString &csValue)
{
	bool bRetValue = false;

	return bRetValue;
}


bool SMExtensionStateBase::ExecuteDeviceScannerProperty(const CString &csPropertyName, key_value_pair_t &ItemPairList)
{
	bool bRetValue = false;
	// int nCount = ItemPairList.GetCount(); // only for tracing purpose
	POSITION nPos = ItemPairList.GetStartPosition();
	while (nPos != NULL)
	{
		CString csKey, csValue;
		ItemPairList.GetNextAssoc(nPos, csKey, csValue);
		bRetValue = ExecuteDeviceScannerSubProperty(csKey, csValue);
	}
	return bRetValue;
}


bool SMExtensionStateBase::ExecuteDeviceScannerSubProperty(const CString &csName, const CString &csValue)
{
	bool bRetValue = false;

	return bRetValue;
}


bool SMExtensionStateBase::ExecuteDeviceSecurityProperty(const CString &csPropertyName, key_value_pair_t &ItemPairList)
{
	bool bRetValue = false;
	// int nCount = ItemPairList.GetCount(); // only for tracing purpose
	POSITION nPos = ItemPairList.GetStartPosition();
	while (nPos != NULL)
	{
		CString csKey, csValue;
		ItemPairList.GetNextAssoc(nPos, csKey, csValue);
		bRetValue = ExecuteDeviceSecuritySubProperty(csKey, csValue);
	}
	return bRetValue;
}


bool SMExtensionStateBase::ExecuteDeviceSecuritySubProperty(const CString &csKey, const CString &csValue)
{
	bool bRetValue = false;

	if (csKey == _T("HOLD_WEIGHT"))
	{
		CString csValueLocal = csValue;
		csValueLocal.MakeUpper();
		if  ( (csValueLocal == _T("TRUE")) ||
		      (csValueLocal == _T("Y")) ||
		      (csValueLocal == _T("YES")) ||
		      (csValueLocal == _T("1"))
			)
		{
			m_bHoldSecWt = true;
            CKeyValueParmList parmList;
            CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_HOLD);
            _bstr_t bstrParmList((const _TCHAR *)parmList);
            BSTR bstrResult;
            bstrResult = NULL;
            SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
            SysFreeString(bstrResult);
		}
		else
		{
			m_bHoldSecWt = false;
            CKeyValueParmList parmList;
            CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_RESUME);
            _bstr_t bstrParmList((const _TCHAR *)parmList);
            BSTR bstrResult;
            bstrResult = NULL;
            SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
            SysFreeString(bstrResult);
		}
		bRetValue = true;	
	}
	//else if // something else

	return bRetValue;
}


bool SMExtensionStateBase::ExecuteExtensionStateProperty(const CString &csPropertyName, key_value_pair_t &ItemPairList)
{
	bool bRetValue = false;
	POSITION nPos = ItemPairList.GetStartPosition();
	while (nPos != NULL)
	{
		CString csKey, csValue;
		ItemPairList.GetNextAssoc(nPos, csKey, csValue);
		bRetValue = ExecuteExtensionStateSubProperty(csKey, csValue);
	}
	return bRetValue;
}

int  SMExtensionStateBase::GetIndexFromList(const CString& csPropName, int nListSize, sPropIDNode* pList)
{
	CString csName = csPropName;
	csName.MakeUpper();
	for(int i = 0; (i < nListSize); ++i)
	{
		if (pList[i].szname == csName)
		{
			break;
		}
	}
	return ExtDevicePropertyList[i].key;
}


bool SMExtensionStateBase::ExecuteExtensionStateSubProperty(const CString &csKey, const CString &csValue)
{
	bool bRetValue = false;

	int nPropIndex = GetIndexFromList(csKey, EXTSTATECMDLIST_SIZE, (sPropIDNodeT *) ExtStateCmdList);
	switch(	nPropIndex )
	{
		case EXSTATE_CMD_PLAYWAVEFILE:
			{
				DMSayPhrase(csValue);
				bRetValue = true;
			}
			break;
		case EXSTATE_CMD_PLAYWAVEFROMID:
			{
				int nWaveID = _ttol(csValue);
				DMSayPhrase(nWaveID);
				bRetValue = true;
			}
			break;
		case EXSTATE_CMD_SETTIMEOUT:
			{
				int nTimeOutSec = _ttol(csValue);
				nTimeOut = nTimeOutSec;
				bRetValue = true;
			}
			break;
		default:
		break;
	}
	return bRetValue;
}




bool SMExtensionStateBase::ExecuteDeviceCashAcceptorProperty(const CString &csPropertyName, key_value_pair_t &ItemPairList)
{
	bool bRetValue = false;
	// int nCount = ItemPairList.GetCount(); // only for tracing purpose
	POSITION nPos = ItemPairList.GetStartPosition();
	while (nPos != NULL)
	{
		CString csKey, csValue;
		ItemPairList.GetNextAssoc(nPos, csKey, csValue);
		bRetValue = ExecuteDeviceCashAcceptorSubProperty(csKey, csValue);
	}
	return bRetValue;
}


bool SMExtensionStateBase::ExecuteDeviceCashAcceptorSubProperty(const CString &csKey, const CString &csValue)
{
	bool bRetValue = false;

	return bRetValue;
}


bool SMExtensionStateBase::ExecuteUIPropertyChangeRequest(const CString& csControlName, const CString &csKey, const CString &csValue)
{
	bool bRetValue = false;
	CString csOrigValue = csValue;
	int nSCOTMsgIDLoc = csValue.Find(_T("MSGID:"));
	if (-1 != nSCOTMsgIDLoc)
	{
		int nIDLen = (csValue.GetLength() - nSCOTMsgIDLoc - _tcslen(_T("MSGID:")));
		if (nIDLen > 0)
		{
			CString csMsgID = csValue.Right(nIDLen);
			int nMsgID = _ttoi(csMsgID);
			csOrigValue = ps.GetPSText(nMsgID);
		}
	}
	m_pAppPSX->SetControlProperty(csControlName, csKey, (LPCTSTR) csOrigValue);
	return bRetValue;
}



bool SMExtensionStateBase::ExecuteDeviceUnknownPoperty(const CString &csPropertyName, key_value_pair_t &ItemPairList)
{
	bool bRetValue = false;
	// int nCount = ItemPairList.GetCount(); // only for tracing purpose
	POSITION nPos = ItemPairList.GetStartPosition();
	while (nPos != NULL)
	{
		CString csKey, csValue;
		ItemPairList.GetNextAssoc(nPos, csKey, csValue);
		bRetValue = ExecuteDeviceUnknownSubPoperty(csPropertyName, csKey, csValue);
	}
	return bRetValue;
}


bool SMExtensionStateBase::ExecuteDeviceUnknownSubPoperty(const CString &csPropertyName, const CString &csKey, const CString &csValue)
{
	bool bRetValue = false;

	return bRetValue;
}




bool SMExtensionStateBase::ExecuteExtensionUpdate(psPropChangeRequestList pRequestListFromTB)
{
	bool bRetValue = false;
	if (!pRequestListFromTB)
	{
		return bRetValue;
	}
	int nRCount = pRequestListFromTB->GetRequestCount();
	bool bCallReDrawDisable = false;
	for (int i = 0; (i < nRCount); ++i)
	{
		psDevicePropChangeRequest pRequest = pRequestListFromTB->GetRequest(i);
		CString csPropName     = pRequest->m_propName;
		CString csSubpropName  = pRequest->m_propSubItemName;
		CString csSubpropValue = pRequest->m_propSubItemValue;
		if (CHANGEPROP_REQUEST_TYPE_UI == pRequest->m_lChangeRequestType)
		{
			if (!bCallReDrawDisable)
			{
				bCallReDrawDisable = true;
				// disable display redraw until the last request is served
				m_pAppPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_FALSE );
				trace( L6, _T("SMExtensionStateBase - REDRAW is set to FALSE") );            
			}
			bRetValue = ExecuteUIPropertyChangeRequest(csPropName, csSubpropName, csSubpropValue);
		}
		else if (CHANGEPROP_REQUEST_TYPE_DEVICE == pRequest->m_lChangeRequestType)
		{
			bRetValue = DispatchExtenstionUpdateRequest(csPropName, csSubpropName, csSubpropValue);
		}
		else
		{
			bRetValue = ExecuteUnknownRequestType(pRequest->m_lChangeRequestType, csPropName, csSubpropName, csSubpropValue);
		}
	}

	if ( bCallReDrawDisable )
	{
		// enable redraw now
		m_pAppPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_TRUE );
		trace( L6, _T("SMExtensionStateBase - REDRAW is set to TRUE.") );            
		bCallReDrawDisable = false;
	}
	pRequestListFromTB->CleanUp();


	return bRetValue;
}



int  SMExtensionStateBase::GetDevPropertyID(const CString& csPropName)
{
	return GetIndexFromList(csPropName, EXPROPERTYLIST_SIZE, (sPropIDNodeT *) ExtDevicePropertyList);

}

bool SMExtensionStateBase::DispatchExtenstionUpdateRequest(const CString& csPropName, const CString& csSubpropName, const CString& csSubpropValue)
{
	bool bRetValue = false;

	// find the appropriate method based on propertyname
	int nCurrentID = GetDevPropertyID(csPropName);

	switch (nCurrentID)
	{
		case DEVPROP_MSR1:
			break;
		case DEVPROP_SCANNER1:
			bRetValue = ExecuteDeviceScannerSubProperty(csSubpropName, csSubpropValue);
			break;
		case DEVPROP_CASHACCEPTOR:
			bRetValue = ExecuteDeviceCashAcceptorSubProperty(csSubpropName, csSubpropValue);
			break;
		case DEVPROP_SECURITY:
			bRetValue = ExecuteDeviceSecuritySubProperty(csSubpropName, csSubpropValue);
			break;
		case DEVPROP_EXSTATE:
			bRetValue = ExecuteExtensionStateSubProperty(csSubpropName, csSubpropValue);
			break;
		case DEVPROP_PRODUCESCALE:
			// for future implementation
			break;
		case DEVPROP_UNKNOWN:
		default:
			bRetValue = ExecuteDeviceUnknownSubPoperty(csPropName, csSubpropName, csSubpropValue);
			break;
	}
	return bRetValue;
}

// TB sent an unknown extension request
// may be intended to fulfil in customer SSF
// base functionality just ignores it
bool SMExtensionStateBase::ExecuteUnknownRequestType(int nRequestType, const CString& csPropName, const CString &csSubPropName, const CString &csSubPropValue)
{
	bool bRetValue = false;

	return bRetValue;
}



SMStateBase  *SMExtensionStateBase::DMCardReader(void)
{
	SMStateBase* pRetState = STATE_NULL;
	return pRetState;
}

SMStateBase  *SMExtensionStateBase::DMAcceptor(void)
{
	SMStateBase* pRetState = STATE_NULL;
	return pRetState;
}


SMStateBase  *SMExtensionStateBase::PSBackSpaceKey(void)
{
 
  PSClearInput();
  
  CString csValue;   // TAR 227930
  long lLen = ps.GetInput(csValue);	
  if (lLen == 0) 
     ps.ClearInput();


  return STATE_NULL;  	    //remain in same state
}



SMStateBase  *SMExtensionStateBase::PSAlphaNumericKey(const long lKey)
{
	long ch = lKey;
	if(fCapsInput)  // if input field formatting is 'all caps' 
	{
	  ch = toupper(lKey); 	
	}
	else if(bShiftOn) // if input field formatting is 'small caps' but shift is On
	{
	  ch = toupper(lKey);
	}
	ps.EchoInput((_TCHAR)ch, true);	// allow alpha chars in input
	long lLen = ps.GetInput(csItemSent);
    return STATE_NULL;		  	    //remain in same state
}



//////////////////////////////////////////////////////
// PSAlphaKey
//////////////////////////////////////////////////
SMStateBase  *SMExtensionStateBase::PSAlphaKey(const long lKey)
{
	ps.EchoInput((_TCHAR)lKey, true);	// allow alpha chars in input
	long lLen = ps.GetInput(csItemSent);
    return STATE_NULL;		  	    //remain in same state
}
SMStateBase  *SMExtensionStateBase::PSShiftKey(void)
{

	if(bShiftOn)
	{
		bShiftOn = false;
	}
	else
	{
		bShiftOn = true;
	}
	
    return STATE_NULL;		  	    //remain in same state
}


/////////////////////////////////////////
// PSDecimalKey
/////////////////////////////////////////
SMStateBase  *SMExtensionStateBase::PSDecimalKey(const long ch)
{
    ps.EchoDecimalInput((_TCHAR)(ch), true);
	long lLen = ps.GetInput(csItemSent);
	return STATE_NULL;
}


//////////////////////////////////////////////////////
SMStateBase  *SMExtensionStateBase::PSNumericKey(const long lKey)
{
  PSEchoInput(lKey);
  return STATE_NULL;
}


/////////////////////////////////////////
SMStateBase  *SMExtensionStateBase::PSClearKey(void)
{

    PSClearAllInput(true);

  return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase  *SMExtensionStateBase::PSEnterKey(void)// enter key pressed, sell this item
{
  CString csTmp;
  ps.GetInput(csTmp);
  if (!csTmp.GetLength())
  {
    // Nothing entered - leave and continue to prompt for operator
    DMSayPhrase(KEYPADINVALID);
  } 
  return STATE_NULL;
}

/////////////////////////////////////////
// PSSpaceKey
/////////////////////////////////////////
SMStateBase  *SMExtensionStateBase::PSSpaceKey(void)
{
 //   CString csEntryData;
    ps.EchoInput((_TCHAR)(0x20), true);
//	long lLen = ps.GetInput(csEntryData);
	return STATE_NULL;
}


SMStateBase  *SMExtensionStateBase::PSOtherSpecialKeys(const long ch)
{
      ps.EchoDecimalInput((_TCHAR)(ch), true); // EchoDecimalInput used because EchoInput 
      long lLen = ps.GetInput(csItemSent);         // won't allow/display non-alphabetic and non-numeric chars 
      return STATE_NULL;
}


///////////////////////////////////
SMStateBase  *SMExtensionStateBase::DMScanner()// scanned item
{
	return STATE_NULL;
}



inline bool SMExtensionStateBase::CustomerMode()
{
	return (m_nStateMode == 1);
}


inline bool SMExtensionStateBase::storeMode()
{
	return (m_nStateMode == 2);
}


inline bool SMExtensionStateBase::helpMode()
{
	return (m_nStateMode == 3);
}


//////////////////////////////////
// TimedOut
//////////////////////////////////
SMStateBase  *SMExtensionStateBase::TimedOut(void)
{
	// notify the event to TB and request direction on timeout event
	sExtStateEventNotifyIn  sNotifyIn;
	sExtStateEventNotifyOut sNotifyOut;
	sNotifyIn.InitStruct();
	sNotifyOut.InitStruct();
	_TCHAR szTBState[25];
	_ltot( nTimeOut, szTBState, 10 );    
	sNotifyIn.SetEventData(_T("TIME_OUT"),  szTBState, _tcslen(szTBState));
	return PreNotifySCOTEvent2TB(NULL, sNotifyIn, sNotifyOut);
}


//////////////////////////////////////////////////////////////
SMStateBase *SMExtensionStateBase::OnWtIncreaseNotAllowed() //tar 197857
{
	if(fItemIsWaiting)			// Tar 211054 RJF 081402
		return STATE_NULL;		// ignore weight increase if 1st item scanned during attract
	else
		return VoidingWtIncrease(BES_CMDATAENTRY, BES_CMDATAENTRY);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMExtensionStateBase::OnWtDecrease() //tar 197857
{
	if (bIsInFinalization)   //We are at finialization stage
	{
		FinalizationWtDecrease();
		return STATE_NULL;
	}
	else
		return ThresholdViolWtDecrease(BES_CMDATAENTRY, BES_CMDATAENTRY);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMExtensionStateBase::OnMatchedWt()
{
	return STATE_NULL;
}

////////////////////////////////////////
SMStateBase  *SMExtensionStateBase::PSButtonGoBack(void)
{
	return STATE_RETURN;
}



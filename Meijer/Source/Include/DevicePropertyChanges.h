///////////////////////////////////////////////////////////////////////////////////////
// Declaration of Device Property Change Requests
// Requests are prepared by TB and executed by SCOTApp
//
// 
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _DEVICEPROPERTYCHANGE_REQUEST_SPEC_
#define _DEVICEPROPERTYCHANGE_REQUEST_SPEC_

#include "SCOTUICMD.h"
#include "ScotMsg.h"


#define RETURN_PARAM_NAME(a) ((LPCTSTR) (##a))

typedef enum tRequestType
{
	CHANGEPROP_REQUEST_TYPE_DEVICE = 1,
	CHANGEPROP_REQUEST_TYPE_UI = 2
} CHANGEDEVICEPROP_REQUEST_TYPE;

/*  !!!WARNING!!! - Scotapp/TB backward compatability dependancy. Do not change/reorder structure */
typedef struct sDevicePropChangeNode
{
	long   m_lVersionID; // set to current version starts from 1
	// 1= device property(CHANGEPROP_REQUEST_TYPE_DEVICE), 2 = UI context change(CHANGEPROP_REQUEST_TYPE_UI)
	short int m_lChangeRequestType;	
	LPTSTR m_propName;
	LPTSTR m_propSubItemName;
	LPTSTR m_propSubItemValue;
	long   nReserved1;
	long   nReserved2;
	sDevicePropChangeNode::sDevicePropChangeNode()
	{
		m_lVersionID = 0;
	}

	bool SetNodeContent(short int nType, LPCTSTR sPropName, LPCTSTR sSubPropName, LPCTSTR sSubPropValue)
	{
		m_lChangeRequestType = nType;
		CopySCOTString(m_propName, sPropName);
		CopySCOTString(m_propSubItemName, sSubPropName);
		CopySCOTString(m_propSubItemValue, sSubPropValue);
		return true;
	}
	void InitStruct()
	{
		m_propName = NULL;
		m_propSubItemName = NULL;
		m_propSubItemValue = NULL;
		m_lVersionID = 1; // defaults to version 1, user of this can change it later as needed
	}
	void CleanUp()
	{
		DELETE_UIPTR(m_propName);
		DELETE_UIPTR(m_propSubItemName);
		DELETE_UIPTR(m_propSubItemValue);
	}
} sDevicePropChangeRequest, *psDevicePropChangeRequest;

// Usage: 
//		InitStruct()
//      set version id
//      call AddRequest(s)
//      call GetRequest
// There is a limitation of having only QUERY_MAX of requests with this implementation(version:1)
// AddRequest takes 4 parameters : request type, property name, 
//                                 subproperty name, subproperty value
// GetRequest
//		 returns a pointer to node of type sDevicePropChangeRequest
//       takes index of the request to read as argument
// 
/*  !!!WARNING!!! - Scotapp/TB backward compatability dependancy. Do not change/reorder structure */
typedef struct stDevicePropChangeRequestList
{
	long   m_lVersionID; // set to current version starts from 1

	psDevicePropChangeRequest  m_pRequestList[QUERY_MAX]; 
	int m_nNumberOfUsedRequests; // init to zero
	long   m_nReserved1; 
	void InitStruct()
	{
		m_nNumberOfUsedRequests = 0;
		m_lVersionID = 1;
		for(int i=0; (i < QUERY_MAX);++i)
		{
			m_pRequestList[i] = NULL;
		}
	}
	psDevicePropChangeRequest AddRequest(short int nType, LPCTSTR sPropName, LPCTSTR sSubPropName, LPCTSTR sSubPropValue)
	{
		psDevicePropChangeRequest pTmp = NULL;
		if (m_nNumberOfUsedRequests >= QUERY_MAX)
		{
			return pTmp;
		}
		pTmp =  (psDevicePropChangeRequest) new sDevicePropChangeRequest;
		if (pTmp)
		{
			pTmp->InitStruct();
			pTmp->SetNodeContent(nType, sPropName, sSubPropName, sSubPropValue);
			m_pRequestList[m_nNumberOfUsedRequests] = pTmp;
			++m_nNumberOfUsedRequests;
		}
		return pTmp;
	}
	int GetRequestCount()
	{
		return (m_nNumberOfUsedRequests);
	}
	psDevicePropChangeRequest GetRequest(int nIndex)
	{
		psDevicePropChangeRequest pTmp = NULL;
		if ((m_nNumberOfUsedRequests >= QUERY_MAX) && m_nNumberOfUsedRequests )
		{
			return pTmp;
		}
		pTmp = m_pRequestList[nIndex];
		return pTmp;
	}
	void CleanUp()
	{
		psDevicePropChangeRequest pTmp = NULL;
		for(int i=0; (i < QUERY_MAX);++i)
		{
			pTmp = m_pRequestList[i];
			if (pTmp)
			{
				try
				{
					pTmp->CleanUp();
					delete pTmp;
				}
				catch(...){};
			}
			m_pRequestList[i] = NULL;
		}
		InitStruct();
	}
}  sPropChangeRequestList, *psPropChangeRequestList;

/*  !!!WARNING!!! - Scotapp/TB backward compatability dependancy. Do not change/reorder structure */
typedef struct ExtStateEventNotifyInType
{
	long m_lVersionID;
	long nEventID;
	const SMessageElement* pSCOTEvent;
	LPCTSTR szEventName;
	LPCTSTR szUIContextName;
	LPCTSTR szDeviceContextName;
	LPTSTR szEventValue;
	int nEventValueStrLen;
	long nReserved1;
	ExtStateEventNotifyInType()
	{
		pSCOTEvent = NULL;
	};
	void InitStruct()
	{
		m_lVersionID = 1;
		szEventName = NULL;
		szEventValue = NULL;
		nEventValueStrLen = 0;
		szUIContextName = NULL;
		szDeviceContextName = NULL;
	}
	void CleanUp()
	{
		try
		{
			delete[] szEventValue;
			szEventValue = NULL;
		}
		catch(...){};
		InitStruct();
	}
	bool SetContextInfo(LPCTSTR szUICtx, LPCTSTR szDevCtx)
	{
		szUIContextName = szUICtx;
		szDeviceContextName = szDevCtx;
	}
	bool SetEventData(LPCTSTR szEventStr, LPCTSTR szEventData, int nDataLen)
	{
		bool bRetValue = false;
		szEventName = szEventStr;
		if (!szEventName || !_tcsclen(szEventName)) 
			return bRetValue;
		try
		{
			delete[] szEventValue;
			szEventValue = NULL;
		}
		catch(...){};
		szEventValue = new _TCHAR[nDataLen+1];
		if (szEventValue)
		{
			if (memcpy(szEventValue, szEventData, nDataLen))
			{
				nEventValueStrLen = nDataLen;
				szEventValue[nDataLen] = '\0';  // safety terminator
				bRetValue = true;
			}
		}
		return bRetValue;
	}
} sExtStateEventNotifyIn, *psExtStateEventNotifyIn;

/*  !!!WARNING!!! - Scotapp/TB backward compatability dependancy. Do not change/reorder structure */
typedef struct sExtStateEventNotifyOutType
{
	long m_lVersionID;
	bool bAllowDefaultProcessing;
	bool bNeedContextChange;
	bool bGoToInProgress; // Leave this state and go to inprogress to disable all user input
	int  nInProgressTimeOut; // in seconds
	long nReserved1;
	void InitStruct()
	{
		m_lVersionID = 1;
		bAllowDefaultProcessing = true;
		bNeedContextChange = false;
		bGoToInProgress = false;
		nInProgressTimeOut = 60;
	}
} sExtStateEventNotifyOut, *psExtStateEventNotifyOut;


#endif // _DEVICEPROPERTYCHANGE_REQUEST_SPEC_
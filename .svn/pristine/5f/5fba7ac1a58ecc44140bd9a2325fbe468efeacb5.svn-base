#if !defined(AFX_SCOTDEVFACTORY_H__8039CE81_D291_11D3_BBAD_00A0C9EDD523__INCLUDED_)
#define AFX_SCOTDEVFACTORY_H__8039CE81_D291_11D3_BBAD_00A0C9EDD523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SCOTDevFactory.h : header file
//

//
#include "SCOTTakeawayBelt.h"

#include "SCOTDevFactoryConstants.h"
#include "SCOTSigcap.h"

/////////////////////////////////////////////////////////////////////////////
// CSCOTDevFactory window

class CSCOTDevFactory : public CWnd
{
// Construction
public:
	CSCOTDevFactory();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCOTDevFactory)
	//}}AFX_VIRTUAL

// Implementation
public:
  typedef struct
  {
    long         lDevClass;
    UINT         nControlID;
    LPCTSTR      szDevClass;
    CString      csProfile;
    CSCOTDevice* pDevice;
  } SCOTDEVMAPINFO;

	long InitDevice(CSCOTEncryptor**		ppDevice);
	long InitDevice(CSCOTSigCap**			ppDevice);
	long InitDevice(CSCOTAuxDisp**			ppDevice);
   long InitDevice(CSCOTTakeawayBelt**     ppDevice);

  CString& GetDevicesInError() { return m_csDevicesInError; };
  //SR672+
  CString& GetSigCapModel() { return m_csSigCapModel;};
  CString& GetEncryptorModel() { return m_csEncryptorModel;};
  CString& GetAuxDispModel() { return m_csAuxDispModel;};
  //SR672-

	long Initialize(CWnd* pWnd = NULL);
	long UnInitialize();
	virtual ~CSCOTDevFactory();

	// Generated message map functions
protected:
  long CommonInitDevice(SCOTDEVMAPINFO* pDevMapInfo);
	SCOTDEVMAPINFO* NewDevInfo(long lDevClass);
  CString ReadRegString(HKEY hKey, LPCTSTR pszValue);
	BOOL GetDevProfile(SCOTDEVMAPINFO* pDevInfo);
	CWnd* m_pParentWnd;
  CMap <long, long, SCOTDEVMAPINFO*, SCOTDEVMAPINFO*> m_DevMap;

  CString m_csDevicesInError;
  BOOL m_bInitialized;
  //SR672+
  CString m_csSigCapModel;
  CString m_csEncryptorModel;
  CString m_csAuxDispModel;
  //SR672-

 	afx_msg void OnDataEvent(long lDevClass, long Status);
  afx_msg void OnErrorEvent(long lDevClass,
                            LONG ResultCode, 
                            LONG ResultCodeExtended, 
                            LONG ErrorLocus, 
                            LONG* pErrorResponse);

	//{{AFX_MSG(CSCOTDevFactory)
		// NOTE - the ClassWizard will add and remove member functions here.
 	  afx_msg void OnDataEventPinpad1(long Status);
    afx_msg void OnErrorEventPinpad1(LONG ResultCode, 
                                     LONG ResultCodeExtended, 
                                     LONG ErrorLocus, 
                                     LONG* pErrorResponse);
 	  afx_msg void OnDataEventSigcap1(long Status);
    afx_msg void OnErrorEventSigcap1(LONG ResultCode, 
                                     LONG ResultCodeExtended, 
                                     LONG ErrorLocus, 
                                     LONG* pErrorResponse);
 	  afx_msg void OnDataEventForms1(long Status);
    afx_msg void OnErrorEventForms1(LONG ResultCode, 
                                     LONG ResultCodeExtended, 
                                     LONG ErrorLocus, 
                                     LONG* pErrorResponse);
 	  afx_msg void OnDataEventEASDeact1(long Status);
      afx_msg void OnDataEventTakeawayBelt(long Status);

	  DECLARE_EVENTSINK_MAP()
	  DECLARE_MESSAGE_MAP()
	//}}AFX_MSG
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCOTDEVFACTORY_H__8039CE81_D291_11D3_BBAD_00A0C9EDD523__INCLUDED_)

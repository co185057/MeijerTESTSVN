// MsrProperties.h: interface for the CMsrProperties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSRPROPERTIES_H__FDDA5FA9_C592_468F_A2FC_49C42B2122F2__INCLUDED_)
#define AFX_MSRPROPERTIES_H__FDDA5FA9_C592_468F_A2FC_49C42B2122F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MSRMsg.h"

class CMsrProperties  
{
public:
	void SetPropertyNumber( long lPropIndex, long lProperty);
	void ParseMsrData(void *pRawData);
   void ParseMsrData( CMSRMsg &pMsg);
 	void Initialize(CmDataCapture* cmDc);
	long GetPropertyNumber( long lPropIndex );
	CString GetPropertyString( long lPropIndex );
	CMsrProperties();
	virtual ~CMsrProperties();

   CString m_csAccountNumber;
   CString m_csExpirationDate;
   CString m_csFirstName;
   CString m_csMiddleInitial;
   CString m_csServiceCode;
   CString m_csSuffix;
   CString m_csSurname;
   CString m_csTitle;
   CString m_csTrack1Data;
   CString m_csTrack1DiscretionaryData;
   CString m_csTrack2Data;
   CString m_csTrack2DiscretionaryData;
   CString m_csTrack3Data;

   long m_DecodeData;
   long m_ParseDecodeData;
   long m_TracksToRead;
   long m_ErrorReportingType;
   long m_CapISO;
   long m_CapJISOne;
   long m_CapJISTwo;

private:
	void ResetValues();
	void ParseTrack1();
	void ParseTrack2();
	CmDataCapture* m_cmDc;
};

#endif // !defined(AFX_MSRPROPERTIES_H__FDDA5FA9_C592_468F_A2FC_49C42B2122F2__INCLUDED_)

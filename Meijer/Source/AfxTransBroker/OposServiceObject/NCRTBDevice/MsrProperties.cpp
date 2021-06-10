// MsrProperties.cpp: implementation of the CMsrProperties class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tboposio.h"
#include "MsrProperties.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsrProperties::CMsrProperties()
:  m_cmDc(NULL),
m_csAccountNumber(""),
m_csExpirationDate(""),
m_csFirstName(""),
m_csMiddleInitial(""),
m_csServiceCode(""),
m_csSuffix(""),
m_csSurname(""),
m_csTitle(""),
m_csTrack1Data(""),
m_csTrack1DiscretionaryData(""),
m_csTrack2Data(""),
m_csTrack2DiscretionaryData(""),
m_csTrack3Data(""),
m_DecodeData(1),
m_ParseDecodeData(1),
m_TracksToRead(MSR_TR_1_2_3),
m_ErrorReportingType(MSR_ERT_CARD),
m_CapISO(TRUE),
m_CapJISOne(TRUE),
m_CapJISTwo(TRUE)
{

}

CMsrProperties::~CMsrProperties()
{

}

CString CMsrProperties::GetPropertyString(long lPropIndex)
{
    switch (lPropIndex) {
    case PIDXMsr_AccountNumber:
       return m_csAccountNumber;
    case PIDXMsr_ExpirationDate:
       return m_csExpirationDate;
    case PIDXMsr_FirstName:
       return m_csFirstName;
    case PIDXMsr_MiddleInitial:
       return m_csMiddleInitial;
    case PIDXMsr_ServiceCode:
       return m_csServiceCode;
    case PIDXMsr_Suffix:
       return m_csSuffix;
    case PIDXMsr_Surname:
       return m_csSurname;
    case PIDXMsr_Title:
       return m_csTitle;
    case PIDXMsr_Track1Data:
       return m_csTrack1Data;
    case PIDXMsr_Track1DiscretionaryData:
       return m_csTrack1DiscretionaryData;
    case PIDXMsr_Track2Data:
       return m_csTrack2Data;
    case PIDXMsr_Track2DiscretionaryData:
       return m_csTrack2DiscretionaryData;
    case PIDXMsr_Track3Data:
      return m_csTrack3Data;
    default:
       break;
    }
   return "";
}

long CMsrProperties::GetPropertyNumber(long lPropIndex)
{
    switch(lPropIndex) {
    case PIDXMsr_DecodeData:
       return m_DecodeData;
       break;
    case PIDXMsr_ParseDecodeData:  // same as PIDXMsr_ParseDecodedData:
       return m_ParseDecodeData;
       break;
    case PIDXMsr_TracksToRead:
       return m_TracksToRead;
       break;
    case PIDXMsr_ErrorReportingType:
       return m_ErrorReportingType;
       break;
    case PIDXMsr_CapISO:
       return m_CapISO;
       break;
    case PIDXMsr_CapJISOne:
       return m_CapJISOne;
       break;
    case PIDXMsr_CapJISTwo:
       return m_CapJISTwo;
       break;
    default:
       break;
    }
    return 0;
}

void CMsrProperties::Initialize(CmDataCapture *cmDc)
{
	m_cmDc = cmDc;   // Set trace object
}


void CMsrProperties::SetPropertyNumber(long lPropIndex, long lProperty)
{
  //test
//  AfxMessageBox("Setting Properties");
  //test end
    switch(lPropIndex) {
    case PIDXMsr_DecodeData:
       m_DecodeData= lProperty;
       break;
    case PIDXMsr_ParseDecodeData:  // same as PIDXMsr_ParseDecodedData:
       m_ParseDecodeData= lProperty;
       break;
    case PIDXMsr_TracksToRead:
       m_TracksToRead= lProperty;
       break;
    case PIDXMsr_ErrorReportingType:
       m_ErrorReportingType= lProperty;
       break;
    case PIDXMsr_CapISO:
       m_CapISO= lProperty;
       break;
    case PIDXMsr_CapJISOne:
       m_CapJISOne= lProperty;
       break;
    case PIDXMsr_CapJISTwo:
       m_CapJISTwo= lProperty;
       break;
    default:
       break;
    }
}

void CMsrProperties::ParseMsrData( void *ptr)
{
   ResetValues();
   OPOSIO_MSRTRACKS   *pData= (OPOSIO_MSRTRACKS *) ptr;
   // raw data starts with 6 bytes of track1len, track2len, track3len; 2 bytes each
   const int base=6;
   m_csTrack1Data= CString((LPCSTR)&pData->Data[base], pData->track1Len);
   m_csTrack2Data= CString((LPCSTR)&pData->Data[base+ pData->track1Len], pData->track2Len);
   m_csTrack3Data= CString((LPCSTR)&pData->Data[base+ pData->track1Len+ pData->track2Len], pData->track3Len);
   
   ParseTrack1();
   ParseTrack2();
}

void CMsrProperties::ParseMsrData( CMSRMsg &pMsg)
{
   ResetValues();
   m_csTrack1Data= pMsg.Track1();
   m_csTrack2Data= pMsg.Track2();
   m_csTrack3Data= pMsg.Track3();
   
   ParseTrack1();
   ParseTrack2();
}

void CMsrProperties::ParseTrack1()
{
   int pos=-1;
   CString csRemaining;
   if (m_csTrack1Data.IsEmpty())
      return;
   m_csAccountNumber="";
   if (m_csTrack1Data[0]== 'B') { // required for account number
      pos= m_csTrack1Data.Find('^');
      if (pos>=0)
         m_csAccountNumber = m_csTrack1Data.Mid(1, pos-1);
   }
   csRemaining= m_csTrack1Data.Mid(pos+1);
   pos= csRemaining.Find('/');
   int posEON= csRemaining.Find('^');
   if (pos < 0) {
      if (posEON < 0)
        m_csSurname = csRemaining;
      else
        m_csSurname = csRemaining.Mid(0, posEON);
   }
   else {
      m_csSurname   = csRemaining.Mid(0, pos);
      m_csSurname.TrimRight();
      if (posEON < 0)
          m_csFirstName = csRemaining.Mid(pos+1);
      else
          m_csFirstName = csRemaining.Mid(pos+1, posEON -pos -1);
      m_csFirstName.TrimRight();
      pos = m_csFirstName.Find(' ');
      if (pos >= 0) {
         m_csMiddleInitial = m_csFirstName.Mid(pos+1);
         m_csFirstName     = m_csFirstName.Mid(0, pos);
         pos = m_csMiddleInitial.Find('.');
         if (pos >= 0) {
             m_csTitle = m_csMiddleInitial.Mid(pos + 1);
             m_csMiddleInitial = m_csMiddleInitial.Mid(0, pos);
         }
      }
   }
   if (posEON >=0) {
      csRemaining= csRemaining.Mid(posEON+1);
      m_csExpirationDate = csRemaining.Mid(0, 4);
      if (csRemaining.GetLength() > 4) {
         m_csServiceCode = csRemaining.Mid(4, 3);
         if (csRemaining.GetLength() > 7) {
            m_csTrack1DiscretionaryData = csRemaining.Mid(7, 8);
            pos= m_csTrack1DiscretionaryData.Find('?');
            if (pos >= 0)
              m_csTrack1DiscretionaryData = m_csTrack1DiscretionaryData.Left(pos);
         }
      }
   }
// NO LOGGING card numbers
//   m_cmDc->DCPrintf( 0x01, _T("+-CMsrProperties::ParseMsrData, T1:[%s], T2:[%s], T3:[%s]"), 
//                     m_csTrack1Data, m_csTrack2Data, m_csTrack3Data);
}

void CMsrProperties::ParseTrack2()
{
    int pos = m_csTrack2Data.Find('=');
    if (pos >= 0) {
        if (isdigit(m_csTrack2Data[0])) 
           m_csAccountNumber = m_csTrack2Data.Mid(0, pos);

        CString csRemaining = m_csTrack2Data.Mid(pos + 1);
        if (csRemaining.GetLength() >= 4) {
            m_csExpirationDate = csRemaining.Left(4);
            if (csRemaining.GetLength() >= 7) {
                m_csServiceCode = csRemaining.Mid(4, 3);
                if (csRemaining.GetLength() >= 8) {
                   m_csTrack2DiscretionaryData = csRemaining.Mid(7);
                   pos = m_csTrack2DiscretionaryData.Find('?');
                   if (pos >= 0) 
                      m_csTrack2DiscretionaryData = m_csTrack2DiscretionaryData.Left(pos);
                }
            }
        }
    }
}

void CMsrProperties::ResetValues()
{
  m_csAccountNumber="";
  m_csExpirationDate="";
  m_csFirstName="";
  m_csMiddleInitial="";
  m_csServiceCode="";
  m_csSuffix="";
  m_csSurname="";
  m_csTitle="";
  m_csTrack1Data="";
  m_csTrack1DiscretionaryData="";
  m_csTrack2Data="";
  m_csTrack2DiscretionaryData="";
  m_csTrack3Data="";

}

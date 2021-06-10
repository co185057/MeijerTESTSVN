// TBMsgObjBase.cpp: implementation of the CTBMsgObjBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TBMsgObj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTBMsgObj::CTBMsgObj(TBDEVICEID devId)
: m_nOffset(0),
  m_pBuf(NULL),
  m_TBMsgId(0),
  m_nExtension(0),
  m_csUserString(""),
  m_pExtensionBuf(NULL)
{
   SetDeviceId(devId);
}

CTBMsgObj::~CTBMsgObj()
{
	delete[] m_pExtensionBuf; // no need to check for null. Deleting NULL ptr is valid.
}

void CTBMsgObj::SetDeviceId(TBDEVICEID device)
{
    m_dwDeviceId = device;
    switch(m_dwDeviceId) {
      case TBDEV_MICR:
         m_TBMsgId= TBMSG_MICR;
         break;
      case TBDEV_PINPAD:
         m_TBMsgId= TBMSG_PINPAD;
         break;
      case TBDEV_ITEM_DETAILS:
         m_TBMsgId= TBMSG_ITEM;
         break;
      case TBDEV_REWARD_DETAILS:
         m_TBMsgId= TBMSG_REWARD;
         break;
      case TBDEV_TOTAL_DETAILS:
         m_TBMsgId= TBMSG_TOTAL;
         break;
      case TBDEV_TENDER_DETAILS:
         m_TBMsgId= TBMSG_TENDER;
         break;
      case TBDEV_CUSTOM:
         m_TBMsgId= TBMSG_CUSTOM;
         break;
      case TB_DEV_CMOS:
         m_TBMsgId= TBMSG_CMOS;
         break;
      case TB_DEV_DISPLAYPRIMARY:
      case TB_DEV_DISPLAYIMMEDIATE:
         m_TBMsgId= TBMSG_DISPLAY1;
         break;
      case TB_DEV_DISPLAYSECONDARY:
      case TB_DEV_DISPLAYINTERMEDIATE:
         m_TBMsgId= TBMSG_DISPLAY2;
         break;
      case TB_DEV_DRAWER1:
      case TB_DEV_DRAWER2:
         m_TBMsgId= TBMSG_CASH_DRAWER;
         break;
      case TB_DEV_KEYBOARD:
         m_TBMsgId= TBMSG_KEYBOARD;
         break;
      case TB_DEV_KEYLOCK:
         m_TBMsgId= TBMSG_KEYLOCK;
         break;
      case TB_DEV_MSR:
         m_TBMsgId= TBMSG_MSR;
         break;
      case TB_DEV_SCANNER:
         m_TBMsgId= TBMSG_SCANNER;
         break;
      case TB_DEV_SCALE:
         m_TBMsgId= TBMSG_SCALE;
         break;
      case TB_DEV_PRINTERRECEIPT:
      case TB_DEV_PRINTERJOURNAL:
      case TB_DEV_PRINTERSLIP:
         m_TBMsgId= TBMSG_PRINTER;
         break;
         // +Sinan
      case 0xB1:
      case 0xB2:
      case 0xB3:
      case 0xB4:
      case 0xB5:
      case 0xB6:
      case 0xB7:
      case 0xB8:
      case 0xB9:
      case 0xBA:
      case 0xBB:
      case 0xBC:
      case 0xBD:
      case 0xBE:
      case 0xBF:
         m_TBMsgId= TBMSG_CUSTOM;
         break;
      case TB_DEV_POSSTATE: // posstate
         m_TBMsgId = TBMSG_POSSTATE;
         // -Sinan
		 break;
      default:
         // TB MsgId is unknown
         m_TBMsgId= 0;
//         ASSERT(0);
    }
}

TBDEVICEID CTBMsgObj::GetDeviceId()
{
   return m_dwDeviceId;
}

BYTE CTBMsgObj::GetTBMsgId()
{
   return m_TBMsgId;
}

void CTBMsgObj::SetTBMsgId(BYTE tbMsgId)
{
   m_TBMsgId=tbMsgId;
   if (m_dwDeviceId == 0) {
      switch(m_TBMsgId) {
      case TBMSG_MICR:
         m_dwDeviceId=TBDEV_MICR;
         break;
      case TBMSG_PINPAD:
         m_dwDeviceId= TBDEV_PINPAD;
         break;
      case TBMSG_ITEM:
         m_dwDeviceId=TBDEV_ITEM_DETAILS ;
         break;
      case TBMSG_REWARD:
         m_dwDeviceId=TBDEV_REWARD_DETAILS ;
         break;
      case TBMSG_TOTAL:
         m_dwDeviceId=TBDEV_TOTAL_DETAILS ;
         break;
      case TBMSG_TENDER:
         m_dwDeviceId=TBDEV_TENDER_DETAILS ;
         break;
      case TBMSG_CUSTOM:
         m_dwDeviceId=TBDEV_CUSTOM ;
         break;
      case TBMSG_CMOS:
         m_dwDeviceId=TB_DEV_CMOS ;
         break;
      case TBMSG_DISPLAY1:
         m_dwDeviceId=TB_DEV_DISPLAYPRIMARY ;
         break;
      case TBMSG_DISPLAY2:
         m_dwDeviceId=TB_DEV_DISPLAYSECONDARY ;
         break;
      case TBMSG_CASH_DRAWER:
         m_dwDeviceId=TB_DEV_DRAWER1 ;
         break;
      case TBMSG_KEYBOARD:
         m_dwDeviceId=TB_DEV_KEYBOARD ;
         break;
      case TBMSG_KEYLOCK:
         m_dwDeviceId=TB_DEV_KEYLOCK ;
         break;
      case TBMSG_MSR:
         m_dwDeviceId=TB_DEV_MSR ;
         break;
      case TBMSG_SCANNER:
         m_dwDeviceId=TB_DEV_SCANNER ;
         break;
      case TBMSG_SCALE:
         m_dwDeviceId=TB_DEV_SCALE ;
         break;
      case TBMSG_PRINTER:
         m_dwDeviceId=TB_DEV_PRINTERRECEIPT ;
         break;
         // +Sinan
      case 0xB1:
      case 0xB2:
      case 0xB3:
      case 0xB4:
      case 0xB5:
      case 0xB6:
      case 0xB7:
      case 0xB8:
      case 0xB9:
      case 0xBA:
      case 0xBB:
      case 0xBC:
      case 0xBD:
      case 0xBE:
      case 0xBF:
         m_dwDeviceId = TBDEV_CUSTOM;
         break;
      case 0xFE: // posstate
         m_dwDeviceId = TB_DEV_POSSTATE;
         // -Sinan
      }
   }
}

int CTBMsgObj::GetMsgVersion()
{
   return m_Hdr.nVersion;
}

void CTBMsgObj::SetMsgVersion(int nVersion)
{
   m_Hdr.nVersion= nVersion;
}

long CTBMsgObj::DecStringToLong(LPCTSTR szVal)
{
   CString cs(szVal);
   cs.Remove(_T('.'));
   return _ttol((LPCTSTR) cs);
}

CString CTBMsgObj::DblToDecString(double dbl)
{
   char buf[40];

   _gcvt(dbl, sizeof(buf)-1, buf);
   // may have a decimal point with no decimal digits
   // remove the dot in this case.
   int len=(int)strlen(buf);
   if (buf[len] == '.')
      buf[len]= '\0';
   return CString(buf);
}

///////////////////////////
// Parsing functions
///////////////////////////
void CTBMsgObj::ParseHeader(BYTE *pBuf)
{
   m_pBuf= pBuf;
   m_nOffset= sizeof(tRouteHeader);
   memcpy( &m_Hdr, pBuf, sizeof (tRouteHeader));
   SetTBMsgId(m_Hdr.messageID);
   return;
}

BYTE CTBMsgObj::ParseByte()
{
   _ASSERT((m_nOffset + sizeof(BYTE)) <= m_Hdr.nLen);
   BYTE byteVal = m_pBuf[m_nOffset];
   m_nOffset+= sizeof(BYTE);
   return byteVal;
}

USHORT CTBMsgObj::ParseUShort()
{
   _ASSERT((m_nOffset + sizeof(USHORT)) <= m_Hdr.nLen);
   USHORT usVal= *((USHORT *) &m_pBuf[m_nOffset]);
   m_nOffset+= sizeof(USHORT);
   return usVal;
}


ULONG CTBMsgObj::ParseULong()
{
   _ASSERT((m_nOffset + sizeof(ULONG)) <= m_Hdr.nLen);
   ULONG ulVal= *((ULONG *) &m_pBuf[m_nOffset]);
   m_nOffset+= sizeof(ULONG);
   return ulVal;
}

CString CTBMsgObj::ParseString(UINT nlen)
{
   _ASSERT((m_nOffset + nlen * sizeof(TCHAR)) <= m_Hdr.nLen);
   CString cs((LPCTSTR) &m_pBuf[m_nOffset], nlen);
   m_nOffset+= (nlen * sizeof(TCHAR));
   return cs;
}

CString CTBMsgObj::ParseL1String()
{
   UINT nlen = ParseByte();
   _ASSERT((m_nOffset + nlen * sizeof(TCHAR)) <= m_Hdr.nLen);
   return ParseString(nlen);
}

CString CTBMsgObj::ParseL2String()
{
   UINT nlen = ParseUShort();
   _ASSERT((m_nOffset + nlen * sizeof(TCHAR)) <= m_Hdr.nLen);
   return ParseString(nlen);
}

CString CTBMsgObj::ParseL4String()
{
   UINT nlen = ParseULong();
   _ASSERT((m_nOffset + nlen * sizeof(TCHAR)) <= m_Hdr.nLen);
   return ParseString(nlen);
}

// do not allocate memory for the new buffer.
// caller is responsible for copying from this ptr.
BYTE* CTBMsgObj::ParseBinaryBuffer(UINT nlen)
{
   _ASSERT((m_nOffset + nlen) <= m_Hdr.nLen);
   BYTE *p= &m_pBuf[m_nOffset];
   m_nOffset += nlen;
   return p;
}


////////////////////////
// Serializing functions
////////////////////////
void CTBMsgObj::SerializeHeader(int &byteLength)
{
   byteLength= getLength();
   m_pBuf= new BYTE[byteLength];
   ((tRouteHeader *) m_pBuf)->messageID= GetTBMsgId();
   ((tRouteHeader *) m_pBuf)->nLen= byteLength;
   ((tRouteHeader *) m_pBuf)->nVersion= GetMsgVersion();
   m_nOffset= sizeof(tRouteHeader);
}

void CTBMsgObj::SerializeByte(BYTE byteVal)
{
   m_pBuf[m_nOffset]= byteVal;
   m_nOffset+= sizeof(BYTE);
}

void CTBMsgObj::SerializeByte(bool bVal)
{
   m_pBuf[m_nOffset]= bVal ? 1:0;
   m_nOffset+= sizeof(BYTE);
}

void CTBMsgObj::SerializeUShort(USHORT usVal)
{
   *((USHORT *) &m_pBuf[m_nOffset])= usVal;
   m_nOffset+= sizeof(USHORT);
}

void CTBMsgObj::SerializeULong(ULONG ulVal)
{
   *((ULONG *) &m_pBuf[m_nOffset])= ulVal;
   m_nOffset+= sizeof(ULONG);
}

void CTBMsgObj::SerializeString(const CString & csVal)
{
   UINT nlen= csVal.GetLength();
   _tcsncpy((LPTSTR) &m_pBuf[m_nOffset], (LPCTSTR)csVal, nlen);
   m_nOffset+= (nlen * sizeof(TCHAR));
}

void CTBMsgObj::SerializeL1String(const CString &csVal)
{
   SerializeByte((BYTE)csVal.GetLength());
   SerializeString(csVal);
}

void CTBMsgObj::SerializeL2String(const CString &csVal)
{
   SerializeUShort(csVal.GetLength());
   SerializeString(csVal);
}

void CTBMsgObj::SerializeL4String(const CString &csVal)
{
   SerializeULong(csVal.GetLength());
   SerializeString(csVal);
}

void CTBMsgObj::SerializeBinaryBuffer(BYTE *pBuf, UINT nlen)
{
   memcpy(&m_pBuf[m_nOffset], pBuf, nlen);
   m_nOffset += nlen;
}
////////////////////////////////////////////////////////////////////////

int CTBMsgObj::GetLengthExtensionData()
{
	return sizeof(m_nExtension) +
		   2+ m_csUserString.GetLength() *sizeof(TCHAR);
}

void CTBMsgObj::ParseExtensionData()
{
	UserLong((int)ParseULong());

	UserString(ParseL2String());	
}

void CTBMsgObj::SerializeExtensionData()
{
	SerializeULong(m_nExtension);
	
	SerializeL2String(m_csUserString);
	
	// replaced by an L2String
	//SerializeUShort(m_nExtensionBufLength);
	//SerializeBinaryBuffer(m_pExtensionBuf, m_nExtensionBufLength);
}

// returns the extension buffer and forgets it
// caller must now free that buffer!
BYTE* CTBMsgObj::GetAndForgetExtensionData()
{
	// obsolete
	return NULL;
}

int CTBMsgObj::UserLong()
{
	   return m_nExtension;
}

void CTBMsgObj::UserLong(int nNewVal)
{
	   m_nExtension = nNewVal;
}

CString CTBMsgObj::UserString()
{
	   return m_csUserString;
}

void CTBMsgObj::UserString(const CString &cs)
{
	m_csUserString= cs;
}


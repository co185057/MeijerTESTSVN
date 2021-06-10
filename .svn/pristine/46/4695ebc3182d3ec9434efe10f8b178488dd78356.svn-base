// DisplayMsg.h: interface for the CDisplayMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYMSG_H__1A884145_661E_4F50_8052_E825843D8426__INCLUDED_)
#define AFX_DISPLAYMSG_H__1A884145_661E_4F50_8052_E825843D8426__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"

class CDisplayMsg : public CTBMsgObj
{
public:
   typedef enum { NORMAL        = 0x00,
                  DROPDOWN_LIST = 0x01,
                  COMBO_BOX     = 0x02,
                  BUTTON        = 0x03,
                  LABEL         = 0x04,
                  CHECK_BOX     = 0x05,
                  RADIO_BUTTON  = 0x06
   } WindowType;

public:
	CDisplayMsg(TBDEVICEID devId =TB_DEV_DISPLAYPRIMARY);
	virtual ~CDisplayMsg();

   virtual BYTE* Serialize(int &byteLength);
   virtual void Parse(BYTE *pBuf);
   virtual int getLength(void);

   inline unsigned int WindowId()
   {
      return m_nWindowId;
   }

   inline void WindowId(unsigned int nId)
   {
      m_nWindowId = nId;
   }

   inline unsigned short TextRow()
   {
      return m_unTextRow;
   }

   inline void TextRow(unsigned short nRow)
   {
      m_unTextRow = nRow;
   }

   inline unsigned short TextColumn()
   {
      return m_unTextColumn;
   }

   inline void TextColumn(unsigned short nCol)
   {
      m_unTextColumn = nCol;
   }

   inline WindowType Type()
   {
      return m_Type;
   }

   inline void Type(WindowType nType)
   {
      m_Type = nType;
   }

   inline CString DisplayData()
   {
      return m_csDisplayData;
   }

   inline void DisplayData(const CString &csData)
   {
      m_csDisplayData = csData;
   }

protected:
   int m_nWindowId;
   unsigned short m_unTextRow;
   unsigned short m_unTextColumn;
   WindowType m_Type;
   CString m_csDisplayData;
};

#endif // !defined(AFX_DISPLAYMSG_H__1A884145_661E_4F50_8052_E825843D8426__INCLUDED_)

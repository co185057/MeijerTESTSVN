// KeylockMsg.h: interface for the CKeylockMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYLOCKMSG_H__D7A1871F_1B21_444F_98A8_201F6A3E349C__INCLUDED_)
#define AFX_KEYLOCKMSG_H__D7A1871F_1B21_444F_98A8_201F6A3E349C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"

class CKeylockMsg : public CTBMsgObj
{
public:
	CKeylockMsg();
	CKeylockMsg(BYTE cPos);
	virtual ~CKeylockMsg();

   inline BYTE Position()
   {
	   return m_cPosition;
   }

   inline void Position(BYTE cPos)
   {
	   m_cPosition = cPos;
   }

protected:
	BYTE m_cPosition;
protected:
   virtual int getLength(void);
public:
   virtual void Parse(BYTE *pBuf);
   virtual BYTE* Serialize(int &byteLength);

};

#endif // !defined(AFX_KEYLOCKMSG_H__D7A1871F_1B21_444F_98A8_201F6A3E349C__INCLUDED_)

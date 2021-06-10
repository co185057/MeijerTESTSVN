// KeyboardMsg.h: interface for the CKeyboardMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYBOARDMSG_H__98C59863_1DD2_47C3_B4ED_04334DFD00AE__INCLUDED_)
#define AFX_KEYBOARDMSG_H__98C59863_1DD2_47C3_B4ED_04334DFD00AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"
#include "windows.h"
class CKeyboardMsg : public CTBMsgObj
{
public:
	typedef enum { KEY_PRESSED_NONE  = 0x00,
                  SHIFT_KEY_PESSED  = 0x01,
		            CTRL_KEY_PRESSED  = 0X02,
		            ALT_KEY_PRESSED   = 0X04
	} CombinationKeyFlags;

//	typedef enum { KEY_DOWN  = 0x001,
//		           KEY_UP    = 0x100,
//				   KEY_PRESS = 0x101
//	} KeyEventType;

	typedef enum {
		KEYDOWN     = 0x0000,
		EXTENDEDKEY = 0x0001,
		KEYUP       = 0x0002
	} KeyEventType;

public:
	CKeyboardMsg(USHORT usKeyCode, KeyEventType dwEvent, CombinationKeyFlags dwFlags);
	CKeyboardMsg();
	virtual ~CKeyboardMsg();

   inline void KeyCode(USHORT usKeyCode)
   {
	   m_usKeyCode = usKeyCode;
   }

   inline USHORT KeyCode()
   {
	   return m_usKeyCode;
   }

   inline void KeyEvent(KeyEventType dwEvent)
   {
	   m_KeyEvent = dwEvent;
   }

   inline KeyEventType KeyEvent()
   {
	   return m_KeyEvent;
   }

   inline void CombinationKeys(CombinationKeyFlags dwFlags)
   {
	   m_ComboFlags = dwFlags;
   }

   inline CombinationKeyFlags CombinationKeys()
   {
	   return m_ComboFlags;
   }


protected:
	USHORT m_usKeyCode;
	KeyEventType m_KeyEvent;
	CombinationKeyFlags m_ComboFlags;
protected:
   virtual int getLength(void);
public:
   virtual void Parse(BYTE *pBuf);
   virtual BYTE* Serialize(int &byteLength);
};

#endif // !defined(AFX_KEYBOARDMSG_H__98C59863_1DD2_47C3_B4ED_04334DFD00AE__INCLUDED_)

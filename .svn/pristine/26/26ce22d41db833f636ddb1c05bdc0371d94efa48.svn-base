// ashDrawerMsg.h: interface for the CashDrawerMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASHDRAWERMSG_H__4F3A3F73_6440_401E_BD71_E538A39A13DA__INCLUDED_)
#define AFX_ASHDRAWERMSG_H__4F3A3F73_6440_401E_BD71_E538A39A13DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"

class CCashDrawerMsg : public CTBMsgObj  
{
public:
	typedef enum { DRAWER_A         = 0x01,
				      DRAWER_B         = 0x02,
                  DRAWER_UNDEFINED = 0x99
	} CashDrawerStations;

	typedef enum { CD_OPEN    = 0x01,
				      CD_CLOSED  = 0x02,
                  CD_UNKNOWN = 0x99
	} CashDrawerStates;

public:
	CCashDrawerMsg(TBDEVICEID devId=TB_DEV_DRAWER1);
	virtual ~CCashDrawerMsg();

   inline CashDrawerStations Station()
   {
	   return m_Station;
   }

   inline void Station(CashDrawerStations dwStation)
   {
	   m_Station = dwStation;
      SetDeviceId((m_Station== DRAWER_A) ? TB_DEV_DRAWER1 : TB_DEV_DRAWER2);
   }

   inline CashDrawerStates Status()
   {
	   return m_Status;
   }

   inline void Status(CashDrawerStates dwStatus)
   {
	   m_Status = dwStatus;
   }

protected:
	CashDrawerStations m_Station;
	CashDrawerStates m_Status;
protected:
   virtual int getLength(void);
public:
   virtual void Parse(BYTE *pBuf);
   virtual BYTE* Serialize(int &byteLength);

};

#endif // !defined(AFX_ASHDRAWERMSG_H__4F3A3F73_6440_401E_BD71_E538A39A13DA__INCLUDED_)

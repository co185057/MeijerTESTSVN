// ScaleMsg.h: interface for the CScaleMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALEMSG_H__098EF665_ED0C_4D3C_92E0_651FB2203DFF__INCLUDED_)
#define AFX_SCALEMSG_H__098EF665_ED0C_4D3C_92E0_651FB2203DFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"

class CScaleMsg : public CTBMsgObj
{
public:
	CScaleMsg();
	CScaleMsg(unsigned int unWeight);

	virtual ~CScaleMsg();

   inline unsigned int Weight()
   {
	   return m_unWeight;
   }

   inline void Weight(unsigned int unWeight)
   {
	   m_unWeight = unWeight;
   }


protected:
	int m_unWeight;
protected:
   virtual int getLength(void);
public:
   virtual void Parse(BYTE *pBuf);
   virtual BYTE* Serialize(int &byteLength);

};

#endif // !defined(AFX_SCALEMSG_H__098EF665_ED0C_4D3C_92E0_651FB2203DFF__INCLUDED_)

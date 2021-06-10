// MSRMsg.h: interface for the CMSRMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSRMSG_H__8DFD8CC9_91C7_4912_B503_E01EAD80EE4A__INCLUDED_)
#define AFX_MSRMSG_H__8DFD8CC9_91C7_4912_B503_E01EAD80EE4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"

class CMSRMsg : public CTBMsgObj
{
public:
	CMSRMsg();
	virtual ~CMSRMsg();


   inline CString Track1()
   {
      return m_csTracks.Left(m_nTrack1Len);
   }

   inline CString Track2()
   {
      return m_csTracks.Mid(m_nTrack1Len, m_nTrack2Len);
   }

   inline CString Track3()
   {
      return m_csTracks.Mid(m_nTrack1Len + m_nTrack2Len, m_nTrack3Len);
   }
   inline int Track1Len()
   {
      return m_nTrack1Len;
   }
   inline int Track2Len()
   {
      return m_nTrack2Len;
   }
   inline int Track3Len()
   {
      return m_nTrack3Len;
   }
   inline void SetTrack123(BYTE l1, BYTE l2, BYTE l3, const CString &track123)
   {
      m_nTrack1Len=l1;
      m_nTrack2Len=l2;
      m_nTrack3Len=l3;
      m_csTracks= track123;
   }
protected:
   CString m_csTracks;
   int m_nTrack1Len;
   int m_nTrack2Len;
   int m_nTrack3Len;
protected:
   virtual int getLength(void);
public:
   virtual void Parse(BYTE *pBuf);
   virtual BYTE* Serialize(int &byteLength);
};

#endif // !defined(AFX_MSRMSG_H__8DFD8CC9_91C7_4912_B503_E01EAD80EE4A__INCLUDED_)

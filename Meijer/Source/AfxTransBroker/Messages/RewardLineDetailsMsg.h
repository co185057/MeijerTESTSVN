// RewardLineDetailsMsg.h: interface for the CRewardLineDetailsMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REWARDLINEDETAILSMSG_H__2D22A0B6_EDF7_4C4A_A45F_FDC4CDE62DC2__INCLUDED_)
#define AFX_REWARDLINEDETAILSMSG_H__2D22A0B6_EDF7_4C4A_A45F_FDC4CDE62DC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"

class CRewardLineDetailsMsg : public CTBMsgObj
{
public:

	typedef enum
	{
		BOTTOM_OF_RECEIPT = 1,
		ABOVE_REF_ENTRYID,
		BELOW_REF_ENTRYID,
		POS_NONE
	} tRewardLinePosType;

	CRewardLineDetailsMsg();
	virtual ~CRewardLineDetailsMsg();

   inline CString RewardDescription()
   {
	   return m_csRewardDescription;
   }

   inline void RewardDescription(const CString &csDesc)
   {
	   m_csRewardDescription = csDesc;
   }

   inline unsigned int RewardValue()
   {
	   return m_unRewardValue;
   }

   inline void RewardValue(unsigned int unValue)
   {
	   m_unRewardValue = unValue;
   }

   inline bool ShowRewardPoints()
   {
	   return m_bShowRewardPoints;
   }

   inline void ShowRewardPoints(bool bVal)
   {
	   m_bShowRewardPoints = bVal;
   }

   inline unsigned int EntryID()
   {
	   return m_unEntryID;
   }

   inline void EntryID(unsigned int unID)
   {
	   m_unEntryID = unID;
   }

   inline unsigned int AssociatedEntryID()
   {
	   return m_unAssociatedEntryID;
   }

   inline void AssociatedEntryID(unsigned int unID)
   {
	   m_unAssociatedEntryID = unID;
   }

   inline bool UpdateRewardTotal()
   {
	   return m_bUpdateRewardTotal;
   }

   inline void UpdateRewardTotal(bool bVal)
   {
	   m_bUpdateRewardTotal = bVal;
   }

   inline unsigned int RewardPoints()
   {
	   return m_unRewardPoints;
   }

   inline void RewardPoints(unsigned int unPoints)
   {
	   m_unRewardPoints = unPoints;
   }

   inline bool IsVoided()
   {
	   return m_bIsVoided;
   }

   inline void IsVoided(bool bVal)
   {
	   m_bIsVoided = bVal;
   }

   inline tRewardLinePosType RewardLocation()
   {
	   return m_cRewardLocation;
   }

   inline void RewardLocation(tRewardLinePosType cLoc)
   {
	   m_cRewardLocation = cLoc;
   }

protected:
	tRewardLinePosType m_cRewardLocation;
	bool m_bIsVoided;
	unsigned int m_unRewardPoints;
	bool m_bUpdateRewardTotal;
	unsigned int m_unAssociatedEntryID;
	unsigned int m_unEntryID;
	bool m_bShowRewardPoints;
	unsigned int m_unRewardValue;
	CString m_csRewardDescription;
protected:
   virtual int getLength(void);
public:
   virtual void Parse(BYTE *pBuf);
   virtual BYTE* Serialize(int &byteLength);
};

#endif // !defined(AFX_REWARDLINEDETAILSMSG_H__2D22A0B6_EDF7_4C4A_A45F_FDC4CDE62DC2__INCLUDED_)

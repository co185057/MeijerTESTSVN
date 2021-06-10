// SCOTTakeawayBelt.h: interface for the CSCOTTakeawayBelt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOTTAKEAWAYBELT_H__F224C713_0B6D_11D6_AF88_00A0249BB4CF__INCLUDED_)
#define AFX_SCOTTAKEAWAYBELT_H__F224C713_0B6D_11D6_AF88_00A0249BB4CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SCOTDevice.h"
#include "TakeawayBeltControl.h"
#include "TakeawayBeltEvents.h"

class CSCOTTakeawayBelt : public CSCOTDevice  
{
public:
	CSCOTTakeawayBelt();
	virtual ~CSCOTTakeawayBelt();

	virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID);
	virtual long UnInitialize();
	virtual void DataEvent(long Status);

    virtual void OnAppEvent(long Event);

private:
    CTakeawayBeltControl m_beltControl;
};

#endif // !defined(AFX_SCOTTAKEAWAYBELT_H__F224C713_0B6D_11D6_AF88_00A0249BB4CF__INCLUDED_)

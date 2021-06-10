// SCOTTakeawayBelt.cpp: implementation of the CSCOTTakeawayBelt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"                 // common includes
#include "SCOTTakeawayBelt.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSCOTTakeawayBelt::CSCOTTakeawayBelt()
{
}

CSCOTTakeawayBelt::~CSCOTTakeawayBelt()
{
}

//////////////////////////////////////////////////////////////////////
// CSCOTTakeawayBelt methods
//////////////////////////////////////////////////////////////////////

long CSCOTTakeawayBelt::Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID)
{
    long rc = 0;

    m_csProfile = profile;
    m_pMainWnd = pMainWnd;

    if (!m_beltControl.Create(NULL, 0, CRect(0, 0, 0, 0), pParentWnd, nID))
    {
        // TODO: Error handling for TakeawayBelt control not created.
        /*
        CString csErr = m_EASDeactivator.GetErrorFunc();
        ScotError(RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_OPOS_CTRL_INIT_FAILURE, "%s|%s", 
              profile, csErr);
        */
        rc = -1;
    }
    else if (!m_beltControl.Open(profile))
    {
        // TODO: Error handling for TakeawayBelt control could not be opened.
        rc = -1;
    }
  
    return rc;
}

long CSCOTTakeawayBelt::UnInitialize()
{
    long rc = 0;
  
    m_beltControl.Close();
    return rc;
}

void CSCOTTakeawayBelt::DataEvent(long Status)
{
    mo.PostDM(DM_DATA, DMCLASS_TAKEAWAYBELT, 0, sizeof(VT_I4), (void*)&Status);
}

void CSCOTTakeawayBelt::OnAppEvent(long Event)
{
    m_beltControl.OnAppEvent(Event);
}

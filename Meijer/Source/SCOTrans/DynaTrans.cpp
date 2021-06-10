// DynaTrans.cpp: implementation of the CDynaTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dftrans.h"
#include "DynaTrans.h"
#include "DFStatsDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern int __cdecl dfCompare(const void *vp1, const void *vp2);

static char unknown[] = "Unknown";

CDynaTrans::CDynaTrans()
{
  m_bTransactionStart = FALSE;
  m_bTransactionEnd   = FALSE;
  m_bLoadState        = FALSE;
  m_bIsSeparator      = FALSE;
  m_bLaneClosed       = FALSE;
  m_bShutdownState    = FALSE;
  m_dfTime = 0;
  m_pTBSInfo = NULL;
  m_Index  = 0;
  m_pSeparatorName = NULL;
}

CDynaTrans::~CDynaTrans()
{

}
	
BOOL CDynaTrans::InitSeparator(CString& desc)
{
  m_bIsSeparator = TRUE;
  m_pSeparatorName = &desc;
  m_StateName.Empty();
  return TRUE;
}

BOOL CDynaTrans::Initialize(int index, CString& data, CDynaTrans* pPreviousTrans)
{

  // first, let's make sure the data looks somewhat normal.  If it doesn't have
  // 2 comma's in it (3 fields) it is bogus, so just return FALSE.

  int len = data.GetLength();
  int numCommas = 0;
  int i = 0;

  while (i < len)
  {
    if (data[i] == ',')
      numCommas++;
    i++;
  }

  if (numCommas != 2)
  {
    TRACE0("Found a bogus line");
    return FALSE;
  }


  sscanf(data, "%d,%[^,],%d",
         &m_dfTime, m_StateName.GetBuffer(50), &m_LastTBState); 

  m_StateName.ReleaseBuffer();
  if ((m_dfTime      <= 0) ||
      (m_LastTBState <= 0))
  {
    return FALSE;
  }

  m_Index = index;
  m_bIsSeparator = FALSE;

  TBSLISTTYPE tbInfo;
  tbInfo.id = m_LastTBState;

  m_pTBSInfo = (TBSLISTTYPE *) bsearch(&tbInfo, &tbsListArray, tbsListArraySize, sizeof(TBSLISTTYPE), dfCompare);

  if (m_StateName.Right(8).Compare("ThankYou") == 0)
  {
    m_bTransactionEnd = TRUE;
  }
  else if (m_StateName.Right(13).Compare("OutOfService2") == 0)
  {
    m_bLoadState = TRUE;
  }
  else if (m_StateName.Right(12).Compare("OutOfService") == 0)
  {
    m_bShutdownState = TRUE;
  }
  else if (m_StateName.Right(10).Compare("LaneClosed") == 0)
  {
    m_bLaneClosed = TRUE;
  }

  if (pPreviousTrans)
  {
    if (pPreviousTrans->IsEndOfTransaction())
    {
      m_bTransactionStart = TRUE;
    }
    else
    {
      m_bTransactionStart = FALSE;
    }

    double diffTime = difftime(m_dfTime, pPreviousTrans->GetTime());
    // Since we can now update the last guys elapsed time stuff now that we
    // computed and set it, tell the last guy to do it.

    pPreviousTrans->UpdateDynaStats(int(diffTime));
  }
  else
  {
    m_bTransactionStart = TRUE;
  }

  // Update individual Dynaframe Info fields;
  if (m_pTBSInfo)
    m_pTBSInfo->numTimes++;

  return TRUE;
}

LPCTSTR CDynaTrans::GetDateTime()
{

  static CTime theTime;
  theTime = m_dfTime;

  //return (LPCTSTR) 
  static char timeStr[30];

  struct tm *pTM = localtime(&m_dfTime);
  time_t time2 = mktime(pTM);

  ASSERT (time2 == m_dfTime);

  sprintf(timeStr, "%02d/%02d - %02d:%02d:%02d", 
          pTM->tm_mon+1, pTM->tm_mday, pTM->tm_hour, pTM->tm_min, pTM->tm_sec);
     
  return (LPCTSTR) timeStr;
}




LPCTSTR CDynaTrans::GetDynaframeName()
{
  if (m_bIsSeparator && m_pSeparatorName)
    return (LPCTSTR) *m_pSeparatorName;
  else if (m_StateName.GetLength() > 0)
    return (LPCTSTR) m_StateName;
  else
    return unknown;
}

int CDynaTrans::GetTBState()
{
  return m_LastTBState;
}

LPCTSTR CDynaTrans::GetTBStateName()
{
  if (m_pTBSInfo)
    return m_pTBSInfo->name;
  else
    return unknown;
}

BOOL CDynaTrans::IsEndOfTransaction()
{
  return m_bTransactionEnd;
}
  

BOOL CDynaTrans::IsSeparator()
{
  return m_bIsSeparator;
}

time_t CDynaTrans::GetTime()
{
  return m_dfTime;
}

void CDynaTrans::DisplayDynaframeStats()
{
  CDFStatsDlg dlg;
  DWORD avgTime;

  AfxMessageBox("Delayed Deliverable!");
  return;

  dlg.m_ID       = m_pTBSInfo->id;
  dlg.m_Name     = m_pTBSInfo->name;
  dlg.m_NumTimes = m_pTBSInfo->numTimes;

  if (m_pTBSInfo->numTimes > 0)
  {
    avgTime        = m_pTBSInfo->totalTime / m_pTBSInfo->numTimes;
    dlg.m_AvgTime  = GetHMSString(avgTime);
  }
  else
  {
    dlg.m_AvgTime = "Invalid";
  }

  dlg.m_TotalTime  = GetHMSString(m_pTBSInfo->totalTime);

  if (m_pTBSInfo->minTime != INT_MAX)
    dlg.m_MinTime    = GetHMSString(m_pTBSInfo->minTime);
  else
    dlg.m_MinTime = "Invalid";

  if (m_pTBSInfo->maxTime != INT_MIN)
    dlg.m_MaxTime    = GetHMSString(m_pTBSInfo->maxTime);
  else
    dlg.m_MaxTime = "Invalid";

  dlg.DoModal();
}

CString CDynaTrans::GetHMSString(int numSeconds)
{
  int hours, min, sec;
  CString hms;

  hours = numSeconds / 3600;
  sec   = numSeconds % 3600;

  min = sec / 60;
  sec = sec % 60;

  if (hours > 0)
    hms.Format("%dhr %dmin %dsec", hours, min, sec);
  else if (min > 0)
    hms.Format("%dmin %dsec",min, sec);
  else
    hms.Format("%dsec",sec);

  return hms;
}

int CDynaTrans::GetIndex()
{
  return m_Index;
}

void CDynaTrans::UpdateDynaStats(time_t elapsedTime)
{
  if (m_pTBSInfo)
  {
    m_pTBSInfo->totalTime += elapsedTime;
    m_pTBSInfo->minTime = min(m_pTBSInfo->minTime, elapsedTime);
    m_pTBSInfo->maxTime = max(m_pTBSInfo->maxTime, elapsedTime);
  }
}

BOOL CDynaTrans::IsLoadState()
{
  return m_bLoadState;
}
	

BOOL CDynaTrans::IsLaneClosed()
{
  return m_bLaneClosed;
}


BOOL CDynaTrans::IsShutdownState()
{
  return m_bShutdownState;
}
	

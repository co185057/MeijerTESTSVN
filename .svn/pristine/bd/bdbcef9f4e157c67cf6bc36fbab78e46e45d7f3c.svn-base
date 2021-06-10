// SCOTAuxDisp.cpp: implementation of the CSCOTAuxDisp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"                 // common includes
#include "SCOTAuxDisp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define COMP_ID ID_OD
#define T_ID    _T("SCOTAuxDisplay")


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSCOTAuxDisp::CSCOTAuxDisp()
{
  m_DefaultScreen   = AUX_SCREEN_USE_TOUCHSCREEN;
  m_DefaultLang     = _T("");
  m_nCurrentLangNdx = 0;
  m_pCurrentScreen  = NULL;
  m_CurrentScreen   = AUX_SCREEN_DEFAULT;
  m_bForceRefreshAlways = false;

  for (int i = 0; i < 4; i++)
    m_IntCashbackValues[i] = 0;
}

CSCOTAuxDisp::~CSCOTAuxDisp()
{
}


long CSCOTAuxDisp::SetLanguage(SCOT_LANGUAGE_TYPE eLanguage)
{
  long rc = 0;

  // If dual language support is not currently on, then we will just
  // ignore this call as the Primary language will be set earlier when
  // the INI file was read.  However, we will throw a warning in if
  // dual language is not on, and we don't get SCOT_PRIMARY_LANGUAGE
  if (co.GetfStateDualLanguage())
  {
    CString newLang(co.Getlanguage(eLanguage).szCode);
    SetCurrentLanguage(newLang);
  }
  else
  {
    ASSERT(eLanguage == SCOT_LANGUAGE_PRIMARY);
    if (eLanguage != SCOT_LANGUAGE_PRIMARY)
    {
      ScotError(RETURN, SM_CAT_INTERNAL_INTERFACE, SCOTAPP_AD_INVALID_LANG, _T("%d"), 
            eLanguage);
      rc = -1;
    }
  }
 
  return rc;
}

long CSCOTAuxDisp::SetCurrentLanguage(CString& csLanguage)
{
  long rc = 0;

  if (m_csCurrentLanguage.CompareNoCase(csLanguage) == 0)
    return rc;

  int i;
  int numLangs = m_ScreenLangArray.GetSize();

  bool bFound = false;

  for (i = 0; i < numLangs; i++)
  {
    if ((m_ScreenLangArray[i]->csLanguage).CompareNoCase(csLanguage) == 0)
    {
      m_csCurrentLanguage = m_ScreenLangArray[i]->csLanguage;
      m_nCurrentLangNdx = i;
      bFound = true;
      break;
    }
  }

  if (!bFound)
  {
    for (i = 0; i < numLangs; i++)
    {
      if (m_DefaultLang.CompareNoCase(m_ScreenLangArray[i]->csLanguage) == 0)
      {
        m_csCurrentLanguage = m_DefaultLang;
        m_nCurrentLangNdx = i;
        bFound = true;
        break;
      }
    }

    if (!bFound)
    {
      m_csCurrentLanguage = m_ScreenLangArray[0]->csLanguage;
      m_nCurrentLangNdx = 0;
    }

    ScotError(RETURN, SM_CAT_CONFIGURATION, SCOTAPP_AD_LANG_NOT_FOUND, _T("%s|%s"), 
          csLanguage, m_csCurrentLanguage);

  }

  // Every time we switch languages, update the PIN prompt
  if(gpSCOTEncryptor)// tar 174852
	gpSCOTEncryptor->SetPINPrompt(m_ScreenLangArray[m_nCurrentLangNdx]->csPINPrompt);

  return rc;
}


long CSCOTAuxDisp::ShowScreen(SCOT_AUX_SCREEN eScreen, bool bForceRefresh)
{
  return VarShowScreen(eScreen, bForceRefresh, 0);
}


long CSCOTAuxDisp::VarShowScreen(SCOT_AUX_SCREEN eScreen, bool bForceRefresh, int nArgs, ...)
{
  long rc = 0;
  va_list argptr;
  va_start( argptr, nArgs );

  rc = VarShowScreenV(eScreen, bForceRefresh, nArgs, argptr);

  va_end(argptr);
  return rc;
}

long CSCOTAuxDisp::VarShowScreenV(SCOT_AUX_SCREEN eScreen, bool bForceRefresh, int nArgs, va_list argptr)
{
  long rc = -1;
  trace(L6, _T("+VarShowScreenV"));

  if (eScreen == AUX_SCREEN_DEFAULT)
    eScreen = m_DefaultScreen;

  if ((eScreen != m_CurrentScreen) || bForceRefresh || m_bForceRefreshAlways)
  {
    AUX_SCREEN_INFO* pInfo = m_ScreenLangArray[m_nCurrentLangNdx]->screens[eScreen];
    if (pInfo)
    {
      m_pCurrentScreen = pInfo->pAuxScreen;
      m_CurrentScreen  = eScreen;

      if (m_pCurrentScreen)
        rc = m_pCurrentScreen->DisplayV(nArgs, argptr);
      else
      {
				rc = -2; // TAR 222179 invalid screen to display - comm error CMal/FFR 120902
        CString csTmp;
        csTmp.Format(_T("The request to display Aux Screen %d (Missing Handle to Aux)"), eScreen); // CMal/FFR 120902
        ScotError(RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_AD_CURRENT_SCREEN_NULL, _T("%s"), 
              csTmp);
      }
    }
    else
    {
			rc = -3; // TAR 222179 no data, can't display new screen - comm error CMal/FFR 120902
      CString csTmp;
      csTmp.Format(_T("The request to display Aux Screen %d (No Data - Unknown screen, loading or range problem?)"), eScreen);
      ScotError(RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_AD_CURRENT_SCREEN_NULL, _T("%s"), 
            csTmp);
    }

  }
  trace(L6, _T("Request to Display Aux screen:%d was made"), eScreen);
	trace(L6, _T("-VarShowScreenV rc:%d"), rc);
	
  return rc;
}


long CSCOTAuxDisp::ReadCashbackInfo(RSection* pSection)
{
  long rc = 0;

  m_CashbackValues.SetSize(4);

  CString csCashBack;

  bool bUseScotOptsCashbackValues = false;
  REntry* pEntry = pSection->GetEntry(_T("UseScotOptsCashbackValues"));
  if (pEntry)
  {
    if (((pEntry->GetValue()).Left(1)).CompareNoCase(_T("y")) == 0)
      bUseScotOptsCashbackValues = true;
  }      


  for (int i = 0; i < 4; i++)
  {
    csCashBack.Format(_T("CashBack%d"), i+1);
    REntry* pEntry = pSection->GetEntry(csCashBack);
    if (pEntry)
    {
      m_IntCashbackValues[i] = _ttoi(pEntry->GetValue());
    }
    else
      m_IntCashbackValues[i] = 0;

    if (bUseScotOptsCashbackValues)
      if (m_IntCashbackValues[i] < MAX_TENDER_CASHBACK_VALUES)
        m_IntCashbackValues[i] = co.nTenderCashBack[m_IntCashbackValues[i] - 1];

    m_CashbackValues[i] = ps.FmtDollar(m_IntCashbackValues[i] * co.nLocalePriceCentUnit);
  }

  return rc;
}


long CSCOTAuxDisp::UnloadScreensInfo()
{
  int numElements = m_ScreenLangArray.GetSize();

  for (int i = 0; i < numElements; i++)
  {
    int numScreens = m_ScreenLangArray[i]->screens.GetSize();
    for (int j = 0; j < numScreens; j++)
    {
      if (m_ScreenLangArray[i]->screens[j])
      {
        if (m_ScreenLangArray[i]->screens[j]->pAuxScreen)
          delete m_ScreenLangArray[i]->screens[j]->pAuxScreen;

        delete m_ScreenLangArray[i]->screens[j];
      }
    }

    m_ScreenLangArray[i]->screens.RemoveAll();
    delete m_ScreenLangArray[i];
  }

  m_ScreenLangArray.RemoveAll();
  return 0;
}


// SCOTSCOTPromptDisp.cpp: implementation of the CSCOTPromptDisp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"                 // common includes
#include "SCOTAuxDisp.h"
#include "SCOTPromptDisp.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define COMP_ID ID_OD
#define T_ID    _T("SCOTPromptDisp")

#define PROMPTS_REG_PATH         _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP")
#define PROMPTS_CFG_FILE_NAME    _T("PromptsInfoFile")
#define PROMPTS_CFG_FILE_DEFAULT _T("SCOTPinpadPrompts.ini")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

  
CSCOTPromptScreen::CSCOTPromptScreen()
{
  m_bBlankFollowupPrompt = true;
}


CSCOTPromptScreen::~CSCOTPromptScreen()
{
}

long CSCOTPromptScreen::Initialize(CString& csPromptInfo, int nColumns)
{
  long rc = 0;

  int len = csPromptInfo.GetLength();
  int ndx = csPromptInfo.Find(_T(':'));
  CString csNum = csPromptInfo.Left(ndx);
  m_lOPOSPromptNum = _ttoi(csNum);

  if ((abs(m_lOPOSPromptNum) >= PPAD_MAX_PROMPTS) || (abs(m_lOPOSPromptNum) == 0))
  {
    ScotError(RETURN, SM_CAT_CONFIGURATION, SCOTAPP_AD_OPOSPROMPT_OUTOFRANGE, _T("%d|%s"), 
          m_lOPOSPromptNum, csPromptInfo);
    m_lOPOSPromptNum = 0;
    rc = -1;
  }
  else
  {
    if (m_lOPOSPromptNum < 0)
    {
      m_lOPOSPromptNum = abs(m_lOPOSPromptNum);
      m_bBlankFollowupPrompt = false;
    }

    // The following code centers each line of the prompt
    // on the display.  It also wraps the lines when it 
    // finds a "\n".
  
    CString csPrompt = csPromptInfo.Right(len - ndx - 1);

    // look for a text "\n", not a real \n char.
    int nLineSep = csPrompt.Find(_T("\\n"));
    if ((nLineSep > -1) && (nLineSep <= nColumns))
    {
      // Center first line in width of display.
      int nLineLen = nLineSep;
      CString csFormat;

      m_csPrompt = CString(_T(' '), ((nColumns - nLineLen) / 2)) + csPrompt.Left(nLineLen);
      m_csPrompt += CString(_T(' '), nColumns - m_csPrompt.GetLength());

      // Now do second line
      nLineLen = csPrompt.GetLength() - nLineSep - 2;
      m_csPrompt += CString(_T(' '), ((nColumns - nLineLen) / 2)) + csPrompt.Right(nLineLen);
    }
    else if (csPrompt.GetLength() < nColumns)
    {
      // Center first line in width of display.
      int nLineLen = csPrompt.GetLength();
      m_csPrompt = CString(_T(' '), ((nColumns - nLineLen) / 2)) + csPrompt.Left(nLineLen);
    }
    else
    {
      m_csPrompt = csPrompt;
    }
  }
  return rc;
}

long CSCOTPromptScreen::DisplayV(int nArgs, va_list argptr)
{
  // we don't support variable data yet, so we will ignore the
  // arguments for the time being.

  ASSERT(gpSCOTEncryptor != NULL);

  // If SetPromptText fails for some weird reason, we will just
  // ignore it.  We don't want to stop the app because a prompt
  // displays incorrectly.  SetPromptText should log an error to 
  // the event log if it fails internally.

  long rc = gpSCOTEncryptor->SetPromptText(m_lOPOSPromptNum, m_csPrompt, m_bBlankFollowupPrompt);
  rc = gpSCOTEncryptor->DisplayPrompt(m_lOPOSPromptNum);
  return rc;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSCOTPromptDisp::CSCOTPromptDisp()
{
  m_PromptsCfgFile    = co.csConfigFilePath + _T("\\") + PROMPTS_CFG_FILE_DEFAULT;
  m_nColumns        = 20; // sensible default
}

CSCOTPromptDisp::~CSCOTPromptDisp()
{
  UnInitialize();
}


long CSCOTPromptDisp::Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID)
{
  long rc = 0;

  m_csProfile = profile;

  if (gpSCOTEncryptor == NULL)
  {
    ScotError(RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_OPOS_CTRL_INIT_FAILURE, _T("%s|%s"), 
          profile, _T("gpSCOTEncryptor==NULL"));
    rc = -1;
  }
  else
  {
    ReadRegistry();
    rc = ReadPromptsInfo(m_PromptsCfgFile);

    if (rc == 0)
    {
      SetCurrentLanguage(m_DefaultLang);
    }
    else
    {
      ScotError(RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_OPOS_CTRL_INIT_FAILURE, _T("%s|%s"), 
            profile, _T("Unable to read Prompts Information"));
    }
  }
  
  return rc;
}


long CSCOTPromptDisp::UnInitialize()
{
  long rc = 0;

  if (m_pCurrentScreen)
  {
    m_pCurrentScreen = NULL;
  }

  UnloadScreensInfo();

  return rc;
}



long CSCOTPromptDisp::ReadRegistry()
{
	LONG     lRC;
	HKEY     hKey;
	_TCHAR  szFilePath[_MAX_PATH+1];
	DWORD dwDataSize=_MAX_PATH;

	lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, PROMPTS_REG_PATH, 
						 0, KEY_QUERY_VALUE, &hKey);

  if (lRC != ERROR_SUCCESS)
  {
    ScotError(RETURN, SM_CAT_CONFIGURATION, SCOTAPP_REG_ACCESS_ERROR, _T("%d|%s|%s"), 
          lRC, _T("opening"), PROMPTS_REG_PATH);
  }

	// Get the Forms Config File
	lRC = RegQueryValueEx(hKey, PROMPTS_CFG_FILE_NAME, NULL, NULL, 
						  (LPBYTE)szFilePath, &dwDataSize);

  if (lRC == ERROR_SUCCESS)
	  m_PromptsCfgFile = szFilePath;

  // Crude test for absolute vs. relative path.  If we find an API later, we will
  // use it.  Anyway, if the file is relative, prepend the forms dir to it.

  if ((m_PromptsCfgFile[0] != _T('\\')) && (m_PromptsCfgFile[1] != _T(':')))
    m_PromptsCfgFile = co.csConfigFilePath + _T("\\") + m_PromptsCfgFile;

	::RegCloseKey(hKey);


  return lRC;
}


long CSCOTPromptDisp::ReadPromptsInfo(CString& cfgFile)
{
  // clear out old data first.
  UnloadScreensInfo();

  long rc = -1;
  RConfigFile* pCfgFile = new RConfigFile;
  if (pCfgFile->LoadFile(cfgFile) == SU_SUCCESS)
  {
    RSection *pSection = pCfgFile->GetFirstSection();
	  while (pSection)
    {
      if (pSection->GetName().Compare(_T("Defaults")) == 0)
      {
        // Do stuff for [Defaults] section
        REntry* pEntry = pSection->GetEntry(_T("DefaultLanguage"));
        if (pEntry)
        {
          m_DefaultLang = pEntry->GetValue();
        }
        pEntry = pSection->GetEntry(_T("DefaultScreen"));
        if (pEntry)
        {
          int nScreen = _ttoi(pEntry->GetValue());

          // Default can't point to default, so don't allow 0
          if ((nScreen > 0) && (nScreen < AUX_SCREENS_PER_LANG))
          {
            m_DefaultScreen = (SCOT_AUX_SCREEN) nScreen;
          }
        }
        pEntry = pSection->GetEntry(_T("ForceRefreshAlways"));
        if (pEntry)
        {
          if (((pEntry->GetValue()).Left(1)).CompareNoCase(_T("y")) == 0)
            m_bForceRefreshAlways = true;
        }      

        ReadCashbackInfo(pSection);
      }
      else if (pSection->GetName().Compare(_T("NumColumns")) == 0)
      {
        // Do stuff for [NumColumns] section
        REntry* pEntry = NULL;
        if (gpSCOTEncryptor)
          pEntry = pSection->GetEntry(gpSCOTEncryptor->GetProfileName());
        if (pEntry == NULL)
          pEntry = pSection->GetEntry(_T("Default"));

        if (pEntry)
        {
          int nColumns = _ttoi(pEntry->GetValue());

          if ((nColumns > 0) && (nColumns < 255))
          {
            m_nColumns = nColumns;
          }
        }
      }
      else if ((pSection->GetName()).GetLength() == 2) // We are looking for ISO639 codes
      {
        AUX_SCREEN_LANG_INFO* pScreenInfo = new AUX_SCREEN_LANG_INFO;

        AUX_SCREEN_INFO* pScreen = new AUX_SCREEN_INFO;
        pScreen->csScreen.Empty();
        pScreen->pAuxScreen = NULL;
        pScreenInfo->screens.SetAtGrow(0, pScreen);

        pScreenInfo->csLanguage = pSection->GetName(); 
        REntry *pEntry = pSection->GetFirstEntry();
        while (pEntry)
        {
          if ((pEntry->GetName()).CompareNoCase(_T("PINPrompt")) == 0)
          {
            pScreenInfo->csPINPrompt = pEntry->GetValue();
          }
          else
          {
            int num = _ttoi(pEntry->GetName());
            if ((num >= 0) && (num < (AUX_SCREEN_MAX+1)))
            {
              CString csPromptInfo = pEntry->GetValue();
              if (csPromptInfo.GetLength() > 0)
              {
                pScreen = new AUX_SCREEN_INFO;
                pScreen->csScreen = csPromptInfo;
                pScreen->pAuxScreen = FormFactory((SCOT_AUX_SCREEN) num, csPromptInfo);
                if (pScreen->pAuxScreen == NULL)
                {
                  ScotError(RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_AD_SCREEN_CREATE_ERROR, _T("%s|%d"), 
                        csPromptInfo, num);
                }

                pScreenInfo->screens.SetAtGrow(num, pScreen);
              }
              else
              {
                ScotError(RETURN, SM_CAT_CONFIGURATION, SCOTAPP_AD_SCREEN_EMPTY_ENTRY, 
                      _T("%d|%s|%s"), num, pScreenInfo->csLanguage, cfgFile);
              }
            }
          }

          pEntry = pSection->GetNextEntry();
        }

        m_ScreenLangArray.Add(pScreenInfo);
        // we will set a good return code if we read at least one
        // language section
        rc = 0;
      }

      pSection = pCfgFile->GetNextSection();
    }
  }

  delete pCfgFile;

  // Look for NULL entries.If we run across a NULL entry, we will set it up with
  // an entry pointing to "Prompt Missing" We will hard code the text
  // because the purpose of this is to draw attention to a problem,
  // and if it is in the wrong language, it will draw even more
  // attention.

  int numLangs = m_ScreenLangArray.GetSize();
  for (int i = 0; i < numLangs; i++)
  {
    AUX_SCREEN_LANG_INFO* pInfo = m_ScreenLangArray[i];
    int numScreens = pInfo->screens.GetSize();

    for (int j = 0; j < (AUX_SCREEN_MAX+1); j++)
    {
      if ((j >= numScreens) || (pInfo->screens[j] == NULL))
      {
        AUX_SCREEN_INFO* pScreen = new AUX_SCREEN_INFO;
        pScreen->csScreen.Format(_T("11:Prompt %d\\nMissing!"), j);
        pScreen->pAuxScreen = FormFactory((SCOT_AUX_SCREEN) j, pScreen->csScreen);
        pInfo->screens.SetAtGrow(j, pScreen);
      }
    }
  }
  return rc;
}

CSCOTAuxScreen* CSCOTPromptDisp::FormFactory(SCOT_AUX_SCREEN eScreen, CString& csScreen)
{
  CSCOTPromptScreen* pScreen = NULL;

  if (!csScreen.IsEmpty())
  {
    pScreen = new CSCOTPromptScreen;
    ASSERT(pScreen != NULL);

    pScreen->Initialize(csScreen, m_nColumns);
  }

  return pScreen;
}

// SCOTIVIForms.cpp: implementation of the CSCOTIVIForms class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"                 // common includes
#include "SCOTAuxDisp.h"
#include "SCOTIVIForms.h"
#include "SCOTForm.h"
#include "SCOTDerivedForm.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define COMP_ID ID_OD
#define T_ID    _T("SCOTIVIForms")

#define FORMS_SIGDW_REG_PATH   _T("SOFTWARE\\OleForRetail\\ServiceOPOS\\SignatureCapture\\NCR5992SigCap.1")
#define FORMS_REG_PATH         _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP")
#define FORMS_DIR_NAME         _T("FormsDirectory")
#define FORMS_DIR_DEFAULT      _T("%APP_DRIVE%\\scot\\forms")
#define FORMS_CFG_FILE_NAME    _T("FormsInfoFile")
#define FORMS_CFG_FILE_DEFAULT _T("SCOTForms.ini")
#define FORMS_FORCE_ONE_RELOAD _T("FormsForceOneReload")

#define FORM_STORE_RETRY_COUNT  3
#define FORM_CLEAR_BIT          0x80000000
#define MAX_BMP_FORMS           10

#define DIO_CLEAR_SCREEN     0
#define DIO_COMPRESS_MEM     1
#define DIO_RESET            2
#define DIO_CLEAR_MEM        3
#define DIO_MAX              (DIO_CLEAR_MEM + 1)

BYTE IVIDirectIOS[DIO_MAX][6] = 
{
  0x05, 0x04, 0x31, 0x30, 0x0,  0x0 ,                       // Clear Screen
  0x05, 0x05, 0xFF, 0x01, 0xFE, 0x0,                        // Compress Mem
  0x05, 0x04, 0x09, 0x08, 0x0,  0x0,                        // Reset Device
  0x05, 0x05, 0x25, 0xFF, (0x05 ^ 0x05 ^ 0x25 ^ 0xFF), 0x0, // Clear Mem
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSCOTIVIForms::CSCOTIVIForms()
{
  // setup sensible defaults if reg. reading fails.
  m_FormsDir        = FORMS_DIR_DEFAULT;
  GET_PARTITION_PATH(FORMS_DIR_DEFAULT, m_FormsDir.GetBuffer(_MAX_PATH));
  m_FormsDir.ReleaseBuffer();

  m_FormsCfgFile    = m_FormsDir + _T("\\") + FORMS_CFG_FILE_DEFAULT;
  m_bReloadFormsAlways = false;
  m_bForceOneReload = false;
}

CSCOTIVIForms::~CSCOTIVIForms()
{
  UnInitialize();
}


long CSCOTIVIForms::Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID)
{
  long rc = 0;
  m_csProfile = profile;
  m_pMainWnd = pMainWnd;

  if (!m_Form.Initialize(profile, pParentWnd, nID))
  {
    CString csErr = m_Form.GetErrorFunc();
    ScotError(RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_OPOS_CTRL_INIT_FAILURE, _T("%s|%s"), 
          profile, csErr);
    rc = -1;
  }
  else
  {
    ReadRegistry();
    rc = ReadFormsInfo(m_FormsCfgFile);

    if (rc == 0)
    {

      SetCurrentLanguage(m_DefaultLang);

      // Try to display the "Loading" form.  If it fails, then
      // we assume we need a complete re-load of the forms, so
      // we will clear mem and reload the forms.  

      rc = Enable();
	   if (rc == 0)
      {
        if (m_bReloadFormsAlways || m_bForceOneReload || ((rc = ShowScreen(AUX_SCREEN_LOADING)) != 0))
        {
			ClearMemory();
			//TAR 184846
			Sleep(30000);
			rc = ResetDevice();
			if (rc == OPOS_SUCCESS)
			{
              rc = LoadMemory();
			}
			//TAR 184846
			if (rc == OPOS_SUCCESS)
				UpdateRegistryAfterLoad();
			else
			{
			ScotError(RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_OPOS_CTRL_INIT_FAILURE, _T("%s|%s"), 
            profile, _T("Unable to read Forms Information"));
			}
		}	
	  }
	}
  }
  return rc;
}


long CSCOTIVIForms::UnInitialize()
{
  long rc = 0;

  if (m_pCurrentScreen)
  {
    m_pCurrentScreen = NULL;
  }

  m_BitmapFormsArray.RemoveAll();
  UnloadScreensInfo();
  m_Form.UnInitialize();

  return rc;
}


long CSCOTIVIForms::Enable()
{
  long rc = 0;
  
  m_Form.SetDeviceEnabled(TRUE);
  rc = m_Form.GetResultCode();
  return TranslateOPOSRetToSCOTRet(rc);
}

long CSCOTIVIForms::Disable()
{
  long rc = 0;
  
  m_Form.SetDeviceEnabled(FALSE);
  rc = m_Form.GetResultCode();
  return TranslateOPOSRetToSCOTRet(rc);
}

long CSCOTIVIForms::DirectIO(long lCommand)
{
  long myLong = 0, rc;
  CString string = IVIDirectIOS[lCommand];
  BSTR bstr = string.AllocSysString();
  rc = m_Form.DirectIO(0, &myLong, &bstr);
  SysFreeString(bstr);	
  return rc;
}


long CSCOTIVIForms::ClearScreen()
{
  return DirectIO(DIO_CLEAR_SCREEN);
}

BOOL CSCOTIVIForms::HasMemory()
{
  return TRUE; // 5992 has memory
}


long CSCOTIVIForms::ClearMemory()
{
  return DirectIO(DIO_CLEAR_MEM);
}

long CSCOTIVIForms::LoadMemory()
{
  long rc = 0;
  long myrc = OPOS_E_FAILURE;

  int numLangs = m_ScreenLangArray.GetSize();

  CString csFmt = ps.GetPSText(SF_LOADING);
  CString csWork;

  for (int i = 0; i < m_BitmapFormsArray.GetSize(); i++)
  {
    if (!(m_BitmapFormsArray[i].IsEmpty()))
    {
      csWork.Format(csFmt, i);
      ps.Echo(csWork);
      myrc = m_Form.StoreFormOnDevice(i, m_BitmapFormsArray[i]);

      if (myrc != OPOS_SUCCESS)
      {
        csWork.Format(ps.GetPSText(SF_LOAD_FAIL), i);
        ps.Echo(csWork);
        ScotError(RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_AD_FORM_LOAD_FAILURE, _T("%s|%d|%d"), 
              m_BitmapFormsArray[i], i, myrc);
      }
    }
  }

  for (i = 0; i < numLangs; i++)
  {
    AUX_SCREEN_LANG_INFO* pInfo = m_ScreenLangArray[i];
    int numScreens = pInfo->screens.GetSize();

    for (int j = 0; j < numScreens; j++)
    {
      AUX_SCREEN_INFO* pScrInfo = pInfo->screens[j];
      if (pScrInfo != NULL)
      {
        CString& csScreen = pScrInfo->csScreen;
        if (!csScreen.IsEmpty())
        {
          // Try to store the screen.
          int retryNdx = 0;

          int nCurrentForm = (i * numScreens) + j;
          if ((nCurrentForm % 5) == 0)
          {
            csWork.Format(csFmt, nCurrentForm);
            ps.Echo(csWork);
          }

          // if this form does not use a bitmap, then set it to 
          // clear before display.  Otherwise it won't clear so the
          // bitmap will stay on screen when this form is displayed.

          if (pScrInfo->nBMPSlot <= 0)
          {
            nCurrentForm |= FORM_CLEAR_BIT;
          }

          myrc = OPOS_E_FAILURE;
          while ((retryNdx < FORM_STORE_RETRY_COUNT) && (myrc != OPOS_SUCCESS))
          {
            //myrc = m_Form.StoreFormOnDevice(((i * AUX_SCREENS_PER_LANG) + j), csScreen);
            myrc = m_Form.StoreFormOnDevice(nCurrentForm, csScreen);
            retryNdx++;
          }


          if (myrc != OPOS_SUCCESS)
          {
            csWork.Format(ps.GetPSText(SF_LOAD_FAIL), nCurrentForm & 0x0000FFFF);
            ps.Echo(csWork);
            ScotError(RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_AD_FORM_LOAD_FAILURE, _T("%s|%d|%d"), 
                  csScreen, nCurrentForm & 0x0000FFFF, myrc);
          }

        }
      }
    }
  }
  return myrc;
}


long CSCOTIVIForms::CompressMemory()
{
  return DirectIO(DIO_COMPRESS_MEM);
}

#define NUM_ALIVE_CHECKS 15

long CSCOTIVIForms::ResetDevice()
{
  ps.Echo(SF_RESET_DEVICE);

  bool bAlive = true;
  long rc = DirectIO(DIO_RESET);
  if (rc == OPOS_SUCCESS)
  {
    bAlive = false;
    for (int i = 0; i < NUM_ALIVE_CHECKS; i++)
    {
      Sleep(2000);
      if (m_Form.CheckHealth(1) == OPOS_SUCCESS)
      {
        bAlive = true;
        break;
      }
    }
    if (!bAlive)
      rc = -1;
  }

  if (rc != 0)
  {
    ScotError(RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_AD_RESET_DEVICE_FAILURE, _T("%s"), 
          bAlive ? _T("true") : _T("false"));
  }

  return rc;
}

long CSCOTIVIForms::UpdateRegistryAfterLoad()//added for TAR 184846
{
  LONG     lRC;
  HKEY     hKey;
  _TCHAR  szTmpVar[_MAX_PATH+1];
  DWORD dwDataSize=_MAX_PATH;

  lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, FORMS_REG_PATH, 
						 0, KEY_ALL_ACCESS, &hKey);
  if (lRC != ERROR_SUCCESS)
  {
    ScotError(RETURN, SM_CAT_CONFIGURATION, SCOTAPP_REG_ACCESS_ERROR, _T("%d|%s|%s"), 
          lRC, _T("opening"), FORMS_REG_PATH);
  }

  if ((lRC == ERROR_SUCCESS) && (m_bForceOneReload==true))
  {	
      _tcscpy(szTmpVar, _T("No"));
      RegSetValueEx(hKey, FORMS_FORCE_ONE_RELOAD, 0, REG_SZ, (CONST BYTE*)szTmpVar, _tcsclen(_T("No"))); 
  }

  ::RegCloseKey(hKey);
  m_bForceOneReload = false;

	return lRC;
}

long CSCOTIVIForms::ReadRegistry()
{
	LONG     lRC;
	HKEY     hKey;
	_TCHAR  szTmpVar[_MAX_PATH+1];
	DWORD dwDataSize=_MAX_PATH;

	lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, FORMS_REG_PATH, 
						 0, KEY_ALL_ACCESS, &hKey);

  if (lRC != ERROR_SUCCESS)
  {
    ScotError(RETURN, SM_CAT_CONFIGURATION, SCOTAPP_REG_ACCESS_ERROR, _T("%d|%s|%s"), 
          lRC, _T("opening"), FORMS_REG_PATH);
  }

	// Get the Forms Directory
	lRC = RegQueryValueEx(hKey, FORMS_DIR_NAME, NULL, NULL, 
						  (LPBYTE)szTmpVar, &dwDataSize);

  if (lRC == ERROR_SUCCESS)
  {
	  m_FormsDir = szTmpVar;
	  GET_PARTITION_PATH(m_FormsDir, m_FormsDir.GetBuffer(_MAX_PATH));
	  m_FormsDir.ReleaseBuffer();
  }

	// Get the Forms Config File
  dwDataSize=_MAX_PATH;
	lRC = RegQueryValueEx(hKey, FORMS_CFG_FILE_NAME, NULL, NULL, 
						  (LPBYTE)szTmpVar, &dwDataSize);

  if (lRC == ERROR_SUCCESS)
	  m_FormsCfgFile = szTmpVar;

  // Crude test for absolute vs. relative path.  If we find an API later, we will
  // use it.  Anyway, if the file is relative, prepend the forms dir to it.

  if ((m_FormsCfgFile[0] != _T('\\')) && (m_FormsCfgFile[1] != _T(':')))
    m_FormsCfgFile = m_FormsDir + _T("\\") + m_FormsCfgFile;

	// See if we are forcing one reload this time
  dwDataSize=_MAX_PATH;
	lRC = RegQueryValueEx(hKey, FORMS_FORCE_ONE_RELOAD, NULL, NULL, 
						  (LPBYTE)szTmpVar, &dwDataSize);

	 //TAR 184846 Currently updated registy key in UpdateRegistryAfterLoad()
	 if (lRC == ERROR_SUCCESS)
	{
	  if (_tcsnicmp(szTmpVar, _T("y"), 1) == 0) // Yes we are forcing a load
    {
      m_bForceOneReload = true;
      // Reset it back to "No"
      //strcpy(szTmpVar, "No");
      //RegSetValueEx(hKey, FORMS_FORCE_ONE_RELOAD, 0, REG_SZ, (CONST BYTE*)szTmpVar, strlen("No"));
    }
  }

	::RegCloseKey(hKey);


  return lRC;
}


long CSCOTIVIForms::ReadFormsInfo(CString& cfgFile)
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

          // The default screen can't point to 0, since that is already
          // the placeholder value for AUX_SCREEN_DEFAULT that we use to 
          // trigger showing a different default screen whose value is 
          // specified here.
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
        pEntry = pSection->GetEntry(_T("ReloadFormsAlways"));
        if (pEntry)
        {
          if (((pEntry->GetValue()).Left(1)).CompareNoCase(_T("y")) == 0)
            m_bReloadFormsAlways = true;
        }      

        ReadCashbackInfo(pSection);
      }
      else if (pSection->GetName().Compare(_T("Bitmap Forms")) == 0)
      {
        REntry *pEntry = pSection->GetFirstEntry();
        while (pEntry)
        {
          int num = _ttoi(pEntry->GetName());
          if ((num >= 0) && (num <= MAX_BMP_FORMS))
          {
            CString csBmpFormName = m_FormsDir + _T("\\") + pEntry->GetValue();

            m_BitmapFormsArray.SetAtGrow(num, csBmpFormName);
          }

          pEntry = pSection->GetNextEntry();
        }
      }
      else if ((pSection->GetName()).GetLength() == 2) // We are looking for ISO639 codes
      {
        AUX_SCREEN_LANG_INFO* pScreenInfo = new AUX_SCREEN_LANG_INFO;

        // Create an empty entry for index 0 since it is not used.
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
            int nBMPSlot = -1;
            int num = _ttoi(pEntry->GetName());
            if ((num >= 0) && (num < (AUX_SCREEN_MAX+1)))
            {
              if (!(pEntry->GetValue().IsEmpty()))
              {
                CString formName = m_FormsDir              + _T("\\") + 
                                   pScreenInfo->csLanguage + _T("\\") + 
                                   pEntry->GetValue();

                // Get BMP slot num from formname.  This function will 
                // also update formName to not have the slot# info in it.

                nBMPSlot = GetBitmapSlot(formName);

                CFileStatus fileStatus;
                if (CFile::GetStatus(formName, fileStatus))
                {
                  pScreen = new AUX_SCREEN_INFO;
                  pScreen->csScreen = formName;
                  pScreen->nBMPSlot = nBMPSlot;
                  pScreen->pAuxScreen = FormFactory((SCOT_AUX_SCREEN) num, formName, nBMPSlot);
                  if (pScreen->pAuxScreen == NULL)
                  {
                    ScotError(RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_AD_SCREEN_CREATE_ERROR, _T("%s|%d"), 
                          formName, num);
                  }
                  pScreenInfo->screens.SetAtGrow(num, pScreen);
                }
                else
                {
                  ScotError(RETURN, SM_CAT_CONFIGURATION, SCOTAPP_AD_SCREEN_FILE_NOT_EXIST, 
                        _T("%s|%d|%s|%s"), formName, num, pScreenInfo->csLanguage, cfgFile);
                }
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
  else
  {
    ScotError(RETURN, SM_CAT_CONFIGURATION, SCOTAPP_AD_LOAD_CFGFILE_FAILURE, _T("%s"), 
          cfgFile);
  }

  delete pCfgFile;

  // Set the slot # that the form is stored on the device as.  This way,
  // if we don't have to load memory, the CSCOTForm object will still know
  // which slot it is.  If we run across a NULL entry, we will set it up with
  // an entry pointing to BAD_FORM_STRING, and then just set it's form slot also.

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
        pScreen->csScreen = BAD_FORM_STRING;
        pScreen->nBMPSlot = -1;
        pScreen->pAuxScreen = FormFactory((SCOT_AUX_SCREEN) j, pScreen->csScreen, -1);
        pInfo->screens.SetAtGrow(j, pScreen);
      }

      if ((pInfo->screens[j] != NULL) && (!(pInfo->screens[j]->csScreen.IsEmpty())))
      {
        CSCOTForm* pForm = (CSCOTForm*) pInfo->screens[j]->pAuxScreen;
        if (pForm)
          pForm->SetFormSlot((i * numScreens) + j);
      }
    }
  }

  return rc;
}


void CSCOTIVIForms::DataEvent(long Status)
{
  if (m_pCurrentScreen)
    m_pCurrentScreen->DataEvent(Status);
  else
  {
    ScotError(RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_AD_CURRENT_SCREEN_NULL, 
          _T("CSCOTIVIForms Data Event")); 
  }
  m_Form.SetDataEventEnabled(TRUE);
}


void CSCOTIVIForms::ErrorEvent(LONG ResultCode, 
                               LONG ResultCodeExtended, 
                               LONG ErrorLocus, 
                               LONG* pErrorResponse)
{
}


long CSCOTIVIForms::SetCurrentLanguage(CString& csLanguage)
{
  long rc = CSCOTAuxDisp::SetCurrentLanguage(csLanguage);

  // Now that we know the current language index, we have to update
  // the registry for NCR5992SigCap.1 to point to the correct slot num
  // for signature capture.  This is really ugly, but that is the way it works.

  UpdateSigcapFormSlot(m_nCurrentLangNdx, AUX_SCREEN_SIGNATURE_NEEDED);

  return rc;
}
  

void CSCOTIVIForms::UpdateSigcapFormSlot(int nCurrentLangNdx, SCOT_AUX_SCREEN eSigScreen)
{
  int countForms = 0, i = 0;

  // count up number of forms that are in all the languages earlier
  // in the array than our current language.
  while (i < nCurrentLangNdx)
  {
    countForms += m_ScreenLangArray[i]->screens.GetSize();
    i++;
  }

  // Now that we know how many forms there are, we know which slot the sig form
  // for this language should be in.

  int newSigcapSlot = countForms + eSigScreen;

	LONG     lRC;
	HKEY     hKey;
	DWORD dwDataSize=sizeof(DWORD);

	lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, FORMS_SIGDW_REG_PATH, 
						 0, KEY_ALL_ACCESS, &hKey);

  if (lRC == ERROR_SUCCESS)
  {
    lRC =::RegSetValueEx(hKey, SCOT_DEFAULT_SIGFORM, 0, REG_DWORD, (CONST BYTE *)&newSigcapSlot, dwDataSize);
    if (lRC != ERROR_SUCCESS)
    {
      ScotError(RETURN, SM_CAT_CONFIGURATION, SCOTAPP_REG_ACCESS_ERROR, _T("%d|%s|%s"), 
            lRC, _T("setting"), SCOT_DEFAULT_SIGFORM);
    }
	  ::RegCloseKey(hKey);
  }
  else
  {
    ScotError(RETURN, SM_CAT_CONFIGURATION, SCOTAPP_REG_ACCESS_ERROR, _T("%d|%s|%s"), 
          lRC, _T("opening"), FORMS_SIGDW_REG_PATH);
  }
}

CSCOTAuxScreen* CSCOTIVIForms::FormFactory(SCOT_AUX_SCREEN eScreen, CString& csScreen, int nBMPSlot)
{
  CSCOTForm* pForm = NULL;
  if (!csScreen.IsEmpty())
  {
    switch(eScreen)
    {
      case AUX_SCREEN_CARD_TYPE:
      case AUX_SCREEN_CARD_TYPE_NO_EBT:
      case AUX_SCREEN_CARD_TYPE_NOCASH:
      case AUX_SCREEN_CARD_TYPE_NO_CASH_NO_EBT:
        pForm = new CSCOTSelectCardForm;
        break;

      case AUX_SCREEN_CONFIRM_DEBIT_AMT:
        pForm = new CSCOTConfirmDebitAmountForm;
        break;

      case AUX_SCREEN_CONFIRM_EBT_AMT:
      case AUX_SCREEN_CONFIRM_FS_AMT:  
      case AUX_SCREEN_CONFIRM_FS_BAL:   
        pForm = new CSCOTConfirmEBTAmountForm;
        break;

      case AUX_SCREEN_EBT_TYPE:
      case AUX_SCREEN_EBT_NO_FS_TYPE:
        pForm = new CSCOTEBTForm();
        break;

      case AUX_SCREEN_FINISH_PAY:
      case AUX_SCREEN_FINISH_PAY_CARD:
        pForm = new CSCOTFinishForm;
        break;

      case AUX_SCREEN_SELECT_AMOUNT:
        pForm = new CSCOTSelectAmountForm;
        ((CSCOTSelectAmountForm*) pForm)->SetCashbackAmounts(m_IntCashbackValues, &m_CashbackValues);
        break;

      case AUX_SCREEN_ATTRACT_LANG:
        pForm = new CSCOTAttractLangForm;
        break;

      case AUX_SCREEN_SSF1:
      case AUX_SCREEN_SSF2:
      case AUX_SCREEN_SSF3:
        pForm = new CSCOTSSFForm;
        break;

      default:
        pForm = new CSCOTForm;
        break;
    }

    pForm->Initialize(eScreen,  nBMPSlot, csScreen, &m_Form);
  }
  return pForm;
}

int CSCOTIVIForms::GetBitmapSlot(CString& csFormName)
{
  int rcSlot = -1;

  int nCommaNdx = csFormName.ReverseFind(_T(','));
  if (nCommaNdx != -1)
  {
    // We found a comma in the form name, now let's make sure there
    // is a number after it.
    int len = csFormName.GetLength();

    if (nCommaNdx < (len - 1))
    {
      // there is a comma, and there is enough data left
      // over for some #, so find out what it is

      int nSlot = _ttoi(csFormName.Right(len - nCommaNdx - 1));

      if ((nSlot > 0) && (nSlot <= MAX_BMP_FORMS))
      {
        // We have found a reasonable looking bitmap slot #, so use it.
        rcSlot = nSlot;
      }
    }

    // if we get here, regardless of what slot# we found, there is a comma
    // in the form name, so we need to delete it off the end of the string

    csFormName = csFormName.Left(nCommaNdx);
  }

  return rcSlot;
}

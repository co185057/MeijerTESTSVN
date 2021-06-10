// SCOTForm.cpp: implementation of the CSCOTForm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "civicmform.h"
#include "SCOTForm.h"
#include "SCOTDerivedForm.h"
#include "DPSElements.h"
#include "common.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define COMP_ID ID_OD
#define T_ID    _T("CSCOTForm")


long CSCOTAuxScreen::Display(int nArgs, ...)
{
  va_list argptr;
  va_start( argptr, nArgs );

  long rc = DisplayV(nArgs, argptr);
  va_end(argptr);
  return rc;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSCOTForm::CSCOTForm()
{
  // This default array gives the event info for the default form
  // that just displays text but doesn't fire events.
  m_EventInfo.SetSize(MAX_CONTROLS_PER_FORM); 
  m_nBMPSlot = -1;
  m_bValidForm = false;
}


long CSCOTForm::Initialize(SCOT_AUX_SCREEN eScreen, int nBMPSlot, CString& csFormFile, CIVICMForm* pForm)
{
  m_nBMPSlot   = nBMPSlot;
  m_eScreen    = eScreen;
  m_pForm      = pForm;

  if (csFormFile.Compare(BAD_FORM_STRING) != 0)
  {
    m_csFormFile = csFormFile;
    // Set up the m_EventInfo array for any classes that override this.
    MessageInit();
    m_bValidForm = true;
  }
  return 0;
}

CSCOTForm::~CSCOTForm()
{
}


void CSCOTForm::DataEvent(long Status)
{
  if ((Status >= 0) && (Status <= MAX_CONTROLS_PER_FORM))
  {
    if (m_EventInfo[Status].Msg > -1)
      CallBack_Handler(m_EventInfo[Status].Msg,
                       m_EventInfo[Status].wParam,
                       m_EventInfo[Status].lParam);
  }
  else
  {
    ScotError(RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_AD_SCREEN_UNHANDLED_CONTROL_DATA, 
          _T("%d|%s|%d"), Status, m_csFormFile, m_eScreen);
  }
}

long CSCOTForm::DisplayV(int nArgs, va_list argptr)
{
  long rc = 0;
  trace(L6, _T("+DisplayV"));

  if (m_bValidForm)
  {
    // If this form has a bitmap, display it first.
    if (m_nBMPSlot > 0)
      DisplayBitmap(m_nBMPSlot);
    trace(L6, _T("DisplayFormOnDevice call inside DisplayV"), rc);
    rc = m_pForm->DisplayFormOnDevice(m_nFormSlot, m_csFormFile);
  }
  else
  {
    // Clear Screen
    long myLong = 0;
    BYTE ClearCmd[] = { 0x05, 0x04, 0x31, 0x30, 0x0,  0x0 };
    CString string = ClearCmd;
    BSTR bstr = string.AllocSysString();
    m_pForm->DirectIO(0, &myLong, &bstr);
    SysFreeString(bstr);	

    CString csTmp;
    csTmp.Format(_T("Form %d missing!"), m_eScreen);
    m_pForm->SetFontTypeface(FONT_16x16);
    m_pForm->DisplayTextAt(11, 1, csTmp, 0);
  }
  trace(L6, _T("-DisplayV rc:%d"), rc);
  return rc;
}


//
// This function can be used by derived forms if they are just
// setting their event info to contiguous IDC_BUTTONX ranges and/or
// IDC_BUTTON_GOBACK

void CSCOTForm::SetEventButtonInfo(int nFirstRegFormButton,
                                   int nFirstRegDPSButton,
                                   int nNumRegButtons,
                                   int nGoBackButton)
{
  if (nNumRegButtons > 0)
  {
    for (int i = 0; i < nNumRegButtons; i++)
    {
      m_EventInfo[nFirstRegFormButton+i].Msg    = DPSN_BUTTON_RELEASED;
      m_EventInfo[nFirstRegFormButton+i].wParam = nFirstRegDPSButton + i;
      m_EventInfo[nFirstRegFormButton+i].lParam = 0;
    }
  }

  if (nGoBackButton > 0)
  {
    m_EventInfo[nGoBackButton].Msg    = DPSN_BUTTON_RELEASED;
    m_EventInfo[nGoBackButton].wParam = IDC_BUTTONGOBACK;
    m_EventInfo[nGoBackButton].lParam = 0;
  }
}

long CSCOTForm::DisplayBitmap(int nBMPSlot)
{
  long myLong = 0;
  _TCHAR DispBmpCommand[] = {0x05, 0x05, 0x21, 0x01, (0x05 ^ 0x05 ^ 0x21 ^ 0x01), 0x0 };

  CString string = DispBmpCommand;
  BSTR bstr = string.AllocSysString();

  long rc = m_pForm->DirectIO(0, &myLong, &bstr);

  SysFreeString(bstr);

  return rc;
}

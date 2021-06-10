// SCOTDerivedForm.cpp: implementation of the derived-from-CSCOTForm classes.
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
#define T_ID    _T("SCOTDerivedForms")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CVarTextBox::CVarTextBox()
{
  m_pForm       = NULL;
  m_bHasVarData = true;
  m_bCenter     = false;
  m_nXpos       = 0;
  m_nYpos       = 0;
  m_nFont       = 0;
  m_nStyle      = 0;
  m_nDevWidth   = 0;
}

void CVarTextBox::Initialize(CIVICMForm* pForm, CString& csFmtText, int nDevWidth, int x, int y, int font, int style, bool bCenter)
{
  m_pForm = pForm;
  m_csFmtText = csFmtText;
  m_nXpos = x;
  m_nYpos = y;
  m_nFont = font;
  m_nStyle = style;
  m_nDevWidth = nDevWidth;
  m_bCenter = bCenter;
}

long CVarTextBox::Format(LPCTSTR szVarArg)
{
  int rc = -1;

  // Try to format the variable data into the printf specifier.  If it
  // doesn't work, make sure we catch any exceptions and log an error.

  try
  {
    m_csDisplayText.Format(m_csFmtText, szVarArg);
    rc = 0;
  }
  catch(...)
  {
    // if we catch an exception here, we know something we wrong in ::Format (sprintf)
    // so we probably don't want to try to display this later.

    m_bHasVarData   = false;
    m_csDisplayText = m_csFmtText; // Just display the format-specifier in this case

    // Also, when we log the error, we must be careful.  If the above Format() call
    // failed, most likely it is because szVarArg is invalid.  If this is the case,
    // we can't really pass szVarArg as a %s parameter to the error logging function,
    // or it will probably die also.  So I guess we will just log it as a pointer.

    ScotError(RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_AD_VARTEXT_FORMAT_ERROR, 
          _T("%p|%s"), szVarArg, m_csFmtText);

  }

  return rc;
}

long CVarTextBox::Display()
{
  int rc = -1;

  if (m_bHasVarData)
  {
    m_pForm->SetFontTypeface(m_nFont);
    m_pForm->SetFontStyle(m_nStyle);

    if (m_bCenter)
    {
      int len = m_csDisplayText.GetLength();
      if (m_nFont == FONT_16x16)
        len *= 2;

      m_nXpos = max(0, ((m_nDevWidth - len) / 2));
    }

    rc = m_pForm->DisplayTextAt(m_nYpos, m_nXpos, m_csDisplayText, 0);
   }

  return rc;
}


CSCOTVarDataForm::CSCOTVarDataForm()
{
}


CSCOTVarDataForm::~CSCOTVarDataForm()
{
  for (int i = 0; i < m_TextBoxArray.GetSize(); i++)
  {
    delete m_TextBoxArray[i];
  }

  m_TextBoxArray.RemoveAll();
}


long CSCOTVarDataForm::Initialize(SCOT_AUX_SCREEN eScreen, int nBMPSlot, CString& csFormFile, CIVICMForm* pForm)
{
  CSCOTForm::Initialize(eScreen, nBMPSlot, csFormFile, pForm);

  long rc = ParseForm();

  return rc;
}


long CSCOTVarDataForm::ParseForm()
{
  long rc = 0;
  int devWidth = m_pForm->GetColumns();
  CStdioFile fFormFile;

  if (fFormFile.Open(m_csFormFile, CFile::modeRead, NULL))
  {
    CString csLine, csTextVal, csVarTest;
    int x, y, font, italics, underline, reverse;

    while (fFormFile.ReadString(csLine))
    {
      bool bCenter = false, bHasVarData = false;

      if (csLine.Find(_T("Text->")) != -1)
      {
        LPCTSTR    str = (LPCTSTR) csLine;

        _stscanf( _tcsstr(str, _T("[x:"))         + 3, _T("%d"), &x );
        _stscanf( _tcsstr(str, _T("[y:"))         + 3, _T("%d"), &y );
        _stscanf( _tcsstr(str, _T("[font:"))      + 6, _T("%d"), &font );
        _stscanf( _tcsstr(str, _T("[italics:"))   + 9, _T("%d"), &italics );
        _stscanf( _tcsstr(str, _T("[underline:")) +11, _T("%d"), &underline );
        _stscanf( _tcsstr(str, _T("[reverse:"))   + 9, _T("%d"), &reverse );

        csTextVal = _tcsstr(str, _T("[text:")) +6;

        int pos = csTextVal.ReverseFind(_T(']'));
        if (pos > 0)
          csTextVal.SetAt(pos, NULL);

        // %M is a special value meaning "center the text in the whole line"
        // We will set the center flag, and change the specifier to a %s.

        pos = csTextVal.Find(_T("%M"));
        if (pos >= 0)
        {
          csTextVal.SetAt(pos+1,_T('s'));
          bCenter = TRUE;
        }

        try
        {
          csVarTest.Format(csTextVal, _T("Test"));
          if (csVarTest.Compare(csTextVal) != 0)
            bHasVarData = true;
        }
        catch(...)
        {
          // obviously had a problem, so maybe more than one % specifier.
          // in this case we will say it has no Var Data, since we can
          // only handle one % specifier per CTextBox object.
          bHasVarData = false;

          ScotError(RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_AD_VARTEXT_VALIDATE_ERROR, 
                _T("%s|%s"), m_csFormFile, csTextVal);
        }
         
        if (bHasVarData)
        {
          CVarTextBox* pText = new CVarTextBox;
          pText->Initialize(m_pForm, csTextVal, devWidth, x, y, font, 
                            MAKE_STYLE(italics, underline, reverse), bCenter);
          m_TextBoxArray.Add(pText);
        }
      }
    }

    fFormFile.Close();  
  }
  else
  {
    // if we make it to here, the form file should be valid since ReadFormsInfo
    // validates the existence of form files.  Something is very weird.
    ScotError(RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_AD_VARTEXT_PARSE_OPEN_ERROR, 
          _T("%s"), m_csFormFile);
  }
  return rc;
}

long CSCOTVarDataForm::DisplayV(int nArgs, va_list argptr)
{
  long rc;
  int i = 0, nCurrentArg = 0;

  while ((i < m_TextBoxArray.GetSize()) && (nCurrentArg < nArgs))
  {
    rc = m_TextBoxArray[i]->Format(va_arg(argptr, LPCTSTR));
    if (rc == 0)
      nCurrentArg++;
    i++;
  }

  rc = CSCOTForm::DisplayV(nArgs, argptr);

  for (i = 0; i < m_TextBoxArray.GetSize(); i++)
    m_TextBoxArray[i]->Display();
  
  return rc;
}


CSCOTSelectCardForm::CSCOTSelectCardForm()
{
}

void CSCOTSelectCardForm::MessageInit()
{
  SetEventButtonInfo(3, IDC_BUTTON1, 4, 7);
}


CSCOTSelectAmountForm::CSCOTSelectAmountForm()
{
}

void CSCOTSelectAmountForm::SetCashbackAmounts(int* pIntCashbackValues, CArray<CString, CString&> *pCashbackValues)
{
  m_pCashbackValues = pCashbackValues;

  // Put the cash back values into our lParam values
  // and we will spoof PSNumericKey back to the core.
  m_EventInfo[3].Msg    = DPSN_AUXDISP_SPOOF_NUMERIC;
  m_EventInfo[3].wParam = 0;
  m_EventInfo[3].lParam = 0;

  for (int i = 0; i < 4; i++)
  {
    m_EventInfo[i+4].Msg    = DPSN_AUXDISP_SPOOF_NUMERIC;
    m_EventInfo[i+4].wParam = 0;
    m_EventInfo[i+4].lParam = pIntCashbackValues[i];
  }

  m_EventInfo[8].Msg    = DPSN_BUTTON_RELEASED;
  m_EventInfo[8].wParam = IDC_BUTTONGOBACK;
  m_EventInfo[8].lParam = 0;
}


long CSCOTSelectAmountForm::DisplayV(int nArgs, va_list argptr)
{
  // We should never be called with any args - we generate our own!
  ASSERT(nArgs == 0);

  long rc;
  int i = 0, nCurrentArg = 0;

  while ((i < m_TextBoxArray.GetSize()) && (nCurrentArg < m_pCashbackValues->GetSize()))
  {
    rc = m_TextBoxArray[i]->Format(m_pCashbackValues->GetAt(nCurrentArg));
    if (rc == 0)
      nCurrentArg++;
    i++;
  }

  rc = CSCOTForm::DisplayV(nArgs, argptr);

  for (i = 0; i < m_TextBoxArray.GetSize(); i++)
    m_TextBoxArray[i]->Display();
  
  return rc;
}

CSCOTConfirmEBTAmountForm::CSCOTConfirmEBTAmountForm()
{
}

void CSCOTConfirmEBTAmountForm::MessageInit()
{
  SetEventButtonInfo(3, IDC_BUTTON1, 2, 0);
}


CSCOTConfirmDebitAmountForm::CSCOTConfirmDebitAmountForm()
{
}

void CSCOTConfirmDebitAmountForm::MessageInit()
{
  SetEventButtonInfo(3, IDC_BUTTON1, 1, 4);
}

CSCOTFinishForm::CSCOTFinishForm()
{
}

void CSCOTFinishForm::MessageInit()
{
  SetEventButtonInfo(3, IDC_BUTTON1, 2, 0);
}


CSCOTEBTForm::CSCOTEBTForm()
{
}

void CSCOTEBTForm::MessageInit()
{
  SetEventButtonInfo(3, IDC_BUTTON1, 2, 5);
}


CSCOTAttractLangForm::CSCOTAttractLangForm()
{
}

void CSCOTAttractLangForm::MessageInit()
{
  SetEventButtonInfo(3, IDC_BUTTON1, 2, 0);
}

CSCOTSSFForm::CSCOTSSFForm()
{
}

void CSCOTSSFForm::MessageInit()
{
  SetEventButtonInfo(3, IDC_BUTTON1, 4, 7);
}


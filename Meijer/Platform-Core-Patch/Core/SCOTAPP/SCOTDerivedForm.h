// SCOTDerivedForm.h: interface for the derived-from-CSCOTForm classes.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOTDERIVEDFORM_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)
#define AFX_SCOTDERIVEDFORM_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// These are the Fonts the 5992 supports.

#define FONT_8x8         0
#define FONT_16x16       1
#define FONT_8x16        2
#define FONT_8x8_BOLD    3

// These are the Styles the 5992 supports

#define STYLE_NORMAL     0
#define STYLE_ITALICS    1
#define SYTLE_UNDERLINE  2
#define STYLE_REVERSE    4

#define STYLE_MASK       0x07

#define SHIFT_ITALICS    0
#define SHIFT_UNDERLINE  1
#define SHIFT_REVERSE    2

// Styles are combined into one style mask

#define MAKE_STYLE(italics, underline, reverse)  \
        (((italics   << SHIFT_ITALICS)   |       \
          (underline << SHIFT_UNDERLINE) |       \
          (reverse   << SHIFT_REVERSE))  & STYLE_MASK)

class CVarTextBox
{
public:
  CVarTextBox();
  void Initialize(CIVICMForm* pForm, CString& csFmtText, int nDevWidth, int x, int y, int font, int style, bool bCenter);
  long Display();
  long Format(LPCTSTR szVarArg);

private:
  CIVICMForm* m_pForm;
  bool    m_bHasVarData;
  bool    m_bCenter;

  CString m_csFmtText;
  CString m_csDisplayText;

  int     m_nXpos;
  int     m_nYpos;
  int     m_nFont;
  int     m_nStyle;
  int     m_nDevWidth;
};


class CSCOTVarDataForm : public CSCOTForm
{
public:
  CSCOTVarDataForm();
  virtual ~CSCOTVarDataForm();
  virtual long Initialize(SCOT_AUX_SCREEN eScreen, int nBMPSlot, CString& csFormFile, CIVICMForm* pForm);
  virtual long DisplayV(int nArgs, va_list argptr);

protected:
  long ParseForm();

  CArray<CVarTextBox*, CVarTextBox*> m_TextBoxArray;
};

class CSCOTSelectCardForm: public CSCOTForm
{
public:

  CSCOTSelectCardForm();
  virtual void MessageInit();
};


class CSCOTSelectAmountForm: public CSCOTVarDataForm
{
public:

  CSCOTSelectAmountForm();
  void SetCashbackAmounts(int* pIntCashbackValues, CArray<CString, CString&> *pCashbackValues);
  virtual long DisplayV(int nArgs, va_list argptr);
private:
  CArray<CString, CString&> *m_pCashbackValues;
};


class CSCOTFinishForm: public CSCOTForm
{
public:

  CSCOTFinishForm();
  virtual void MessageInit();
};

class CSCOTConfirmDebitAmountForm: public CSCOTVarDataForm
{
public:

  CSCOTConfirmDebitAmountForm();
  virtual void MessageInit();
};

class CSCOTConfirmEBTAmountForm: public CSCOTVarDataForm
{
public:

  CSCOTConfirmEBTAmountForm();
  virtual void MessageInit();
};

class CSCOTEBTForm: public CSCOTForm
{
public:

  CSCOTEBTForm();
  virtual void MessageInit();
};

class CSCOTAttractLangForm: public CSCOTVarDataForm
{
public:

  CSCOTAttractLangForm();
  virtual void MessageInit();
};

class CSCOTSSFForm: public CSCOTForm
{
public:

  CSCOTSSFForm();
  virtual void MessageInit();
};

#endif
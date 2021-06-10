// SCOTForm.h: interface for the CSCOTForm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOTFORM_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)
#define AFX_SCOTFORM_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
#include "SCOTAuxDisp.h"

#define MAX_CONTROLS_PER_FORM        10

// We will define a string that CSCOTIVIForms::ReadFormsInof() can use
// for the pScreen->csScreen value if there is a problem with the form (it
// either is not specified or does not exist).  This string will contain
// invalid file chars so we know it will never collide with a valid ICF file.
// 
// When CSCOTForm goes to display the form, it will check if this string is the file
// name, and if so, it will display a message "Form %d missing".
#define BAD_FORM_STRING _T(",,,BADFORM,,,")

class CSCOTForm : public CSCOTAuxScreen
{
public:

  CSCOTForm();
  virtual ~CSCOTForm();

  virtual long Initialize(SCOT_AUX_SCREEN eScreen, int nBMPSlot, CString& csFormFile, CIVICMForm* pForm);
  virtual void SetFormSlot(int nFormSlot) {m_nFormSlot = nFormSlot;}
  virtual void MessageInit() {};
  virtual void DataEvent(long Status);
  virtual long DisplayV(int nArgs, va_list argptr);

protected:

  bool               m_bValidForm;
  int                m_nFormSlot;
  int                m_nBMPSlot;
  CString            m_csFormFile;
  SCOT_AUX_SCREEN    m_eScreen;
  CIVICMForm*        m_pForm;

  CArray<AUX_DISP_DPS_EVENT, AUX_DISP_DPS_EVENT&> m_EventInfo;
  void SetEventButtonInfo(int nFirstRegFormButton,
                          int nFirstRegDPSButton,
                          int nNumRegButtons,
                          int nGoBackButton);

  long DisplayBitmap(int nBMPSlot);
};



#endif
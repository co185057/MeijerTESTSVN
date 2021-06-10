// SCOTIVIForms.h: interface for the CSCOTIVIForms class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOTIVIForms_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)
#define AFX_SCOTIVIForms_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_

#include "civicmform.h"	// Added by ClassView
#include "SCOTAuxDisp.h"
#include "SCOTForm.h"
#include "SCOTUtil.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//
// This class is for use when there is an IVI-Form's-compatible device
// such as the NCR 5992 or IVI's signature capture form device.
//

class CSCOTIVIForms : public CSCOTAuxDisp
{
public:
  virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID);
	virtual long UnInitialize();
	virtual long Disable();
	virtual long Enable();

  CSCOTIVIForms();
  virtual ~CSCOTIVIForms();
  virtual void DataEvent(long Status);
  virtual void ErrorEvent(LONG ResultCode, 
                          LONG ResultCodeExtended, 
                          LONG ErrorLocus, 
                          LONG* pErrorResponse);

  virtual long ClearScreen();
  virtual BOOL HasMemory();
  virtual long ClearMemory();
  virtual long LoadMemory();
  virtual long CompressMemory();

protected:
  long SetCurrentLanguage(CString& csLanguage);
  long DirectIO(long lCommand);
  long ReadFormsInfo(CString& cfgFile);
  long ReadRegistry();
  long UpdateRegistryAfterLoad();
  virtual CSCOTAuxScreen* FormFactory(SCOT_AUX_SCREEN eScreen, CString& csScreen, int nBMPSlot);
  void UpdateSigcapFormSlot(int m_nCurrentLangNdx, SCOT_AUX_SCREEN eScreen);
  virtual long ResetDevice();
  int GetBitmapSlot(CString& csFormName);

  CIVICMForm m_Form;
  CString    m_FormsDir;
  CString    m_FormsCfgFile;

  CArray<CString, CString&> m_BitmapFormsArray;

  bool       m_bReloadFormsAlways;
  bool		 m_bForceOneReload;
};

#endif
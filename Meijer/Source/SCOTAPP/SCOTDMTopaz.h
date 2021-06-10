#if !defined(AFX_SCOTDMTOPAZ_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)
#define AFX_SCOTDMTOPAZ_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SCOTSigCap.h"

class CSCOTDMTopaz : public CSCOTSigCap
{
public:
#ifndef _CPPUNIT
	virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID);
	virtual long UnInitialize();
	virtual long Disable();
	virtual long Enable();
	CSCOTDMTopaz();
	virtual ~CSCOTDMTopaz();


  virtual long BeginCapture(LPCTSTR szCaptureForm,
                            HWND hDrawWnd = NULL);
  virtual long EndCapture();
  virtual long Get5991Format(BSTR* pData);
#endif //_CPPUNIT
};
#endif // !defined(AFX_SCOTDMTOPAZ_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)

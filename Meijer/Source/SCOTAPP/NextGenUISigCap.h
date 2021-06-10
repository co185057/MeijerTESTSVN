// NextGenUISigCap.h.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEXTGENUISIGCAP_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)
#define AFX_NEXTGENUISIGCAP_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "SCOTSigCap.h"


class CNextGenUISigCap : public CSCOTSigCap
{
public:
	virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID);
	virtual long UnInitialize();
	virtual long Disable();
	virtual long Enable();
	CNextGenUISigCap();
	virtual ~CNextGenUISigCap();


  virtual long BeginCapture(LPCTSTR szCaptureForm,
                            HWND hDrawWnd = NULL);
  virtual long EndCapture();
  virtual long Get5991Format(BSTR* pData);
  virtual CString GetcsSigData() const;
  virtual void SetcsSigData(const CString &csData);
  virtual bool IsOnScreenCapture() const {return true;};

  virtual long GetWidth() const { return m_lWidth;};
  virtual long GetHeight() const { return m_lHeight;};


protected:
    CString m_csSigData;
    long m_lWidth;
    long m_lHeight;
	int ParseStroke(const CString &csStroke, short PointArray[]);
	int ParseWPFSigFormat(const CString &csData, short PointArray[]);
    void SetWidth(const long lValue);
    void SetHeight(const long lVaue);
    CString AdjustStroke(const CString &csStroke);

};



#endif // !defined(AFX_NEXTGENUISIGCAP_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)

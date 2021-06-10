#if !defined(AFX_SCOTOPOSSIGCAP_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)
#define AFX_SCOTOPOSSIGCAP_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SCOTSigCap.h"

class CSCOTOPOSSigCap : public CSCOTSigCap
{
public:
#ifndef _CPPUNIT
    virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID);
	virtual long UnInitialize();
	virtual long Disable();
	virtual long Enable();
	CSCOTOPOSSigCap();
	virtual ~CSCOTOPOSSigCap();
  virtual void DataEvent(long Status);
  virtual void ErrorEvent(LONG ResultCode, 
                          LONG ResultCodeExtended, 
                          LONG ErrorLocus, 
                          LONG* pErrorResponse);

  virtual long BeginCapture(LPCTSTR szCaptureForm,
                            HWND hDrawWnd = NULL);
  virtual long EndCapture();
  virtual long Get5991Format(BSTR* pData);
#endif //_CPPUNIT

protected:
#ifndef _CPPUNIT
	CRect m_SigRect;
	CSigCap m_Sigcap;
	WNDPROC m_lpfnOldWndProc;
  static CSCOTOPOSSigCap* m_pThis;
	CPoint m_WindowExtent;
	CPoint m_ViewportExtent;
	CPoint m_ViewportOrigin;
  long m_lTotalPoints;
  unsigned char PA[16000]; // Room for 4000 points
  bool m_bInSigcap;

	void FindNewOrigin(CRect &wnd, CRect &dev, CPoint& newOrigin);
	void FindMaxMins(CRect& cornerRect);
	void CalcNewCoordinates(CRect& rDisp);
  void DrawSignature(long lTotalPoints, LPCTSTR pRawData);

	void DetachDispWnd();
	void AttachDispWnd(HWND hWnd);
	void OnDraw( void );

	LRESULT CallWindowProc(UINT Msg, WPARAM wParam, LPARAM lParam ) const;
  static LRESULT CALLBACK SubClassSigCapWindowFunc(HWND hWnd, 
                                                   UINT wMessage,
	                                                 WPARAM wParam, 
                                                   LPARAM lParam);

  virtual long DisplaySigOnWindow();
  CWnd*   m_pMainWnd; 

#endif //_CPPUNIT
};

#endif // !defined(AFX_SCOTOPOSSIGCAP_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)

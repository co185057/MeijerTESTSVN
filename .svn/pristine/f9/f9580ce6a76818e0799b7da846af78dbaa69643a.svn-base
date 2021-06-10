// SCOTSigCap.h: interface for the CSCOTSigCap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOTSIGCAP_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)
#define AFX_SCOTSIGCAP_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SigCap.h"

#define SCOT_DEFAULT_SIGFORM      _T("SCOT.SigcapForm")

class CSCOTSigCap : public CSCOTDevice
{
public:
	BOOL m_bHaveValidSig;
	virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID) = 0;
	virtual long UnInitialize() = 0;
	virtual long Disable() = 0;
	virtual long Enable() = 0;
	CSCOTSigCap();
	virtual ~CSCOTSigCap();
  virtual void DataEvent(long Status) {};
  virtual void ErrorEvent(LONG ResultCode, 
                          LONG ResultCodeExtended, 
                          LONG ErrorLocus, 
                          LONG* pErrorResponse) {};

  virtual long BeginCapture(LPCTSTR szCaptureForm,
                            HWND hDrawWnd = NULL) = 0;
  virtual long EndCapture() = 0;
  virtual long Get5991Format(BSTR* pData) = 0;
  virtual long ClearDisplay() { return 0; };

protected:
	HWND m_hDispWnd;
};


class CSCOTOPOSSigCap : public CSCOTSigCap
{
public:
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

protected:
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
};


class CSCOTDMTopaz : public CSCOTSigCap
{
public:
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
};

class CSCOT5992SigCap : public CSCOTOPOSSigCap
{
public:
	CSCOT5992SigCap();
	virtual ~CSCOT5992SigCap();
  virtual long ClearDisplay();
};


#endif // !defined(AFX_SCOTSIGCAP_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)

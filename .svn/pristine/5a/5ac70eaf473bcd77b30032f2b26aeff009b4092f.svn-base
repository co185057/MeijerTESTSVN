// SCOTAuxDisp.h: interface for the CSCOTAuxDisp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOTAUXDISP_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)
#define AFX_SCOTAUXDISP_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConfigObject.h"
#include "SCOTDevice.h"
#include "SCOTUtil.h"
#include "SCOTAuxScreenList.h"

// This value is used in the DPS event handler Callback_Handler to allow
// CSCOTAuxScreen objects to spoof numeric data back thru the DPS event
// handling scheme.  This is currently used for cashback values.

#define DPSN_AUXDISP_SPOOF_NUMERIC   777

typedef struct AuxDispDpsEvent
{
  AuxDispDpsEvent(long msg = -1, WPARAM wparam = 0, LPARAM lparam = 0) : 
                 Msg(msg), wParam(wparam), lParam(lparam) {};
  long           Msg;
  WPARAM         wParam;
  LPARAM         lParam;
} AUX_DISP_DPS_EVENT;


class CSCOTAuxScreen
{
public:
  CSCOTAuxScreen() {};
  virtual ~CSCOTAuxScreen() {};

  virtual void MessageInit() {};
  virtual void DataEvent(long Status) {};
  long Display(int nArgs, ...);
  virtual long DisplayV(int nArgs, va_list argptr) = 0;
};


typedef struct
{
  CString csScreen;
  int nBMPSlot;    // This is only used for 5992.
  CSCOTAuxScreen* pAuxScreen;
} AUX_SCREEN_INFO;


typedef CArray<AUX_SCREEN_INFO*, AUX_SCREEN_INFO*> AUX_SCREEN_ARRAY;

typedef struct
{
  CString csLanguage;
  CString csPINPrompt; // Used for prompting for PIN - kept per language
  AUX_SCREEN_ARRAY screens;
} AUX_SCREEN_LANG_INFO;


//
// This is the DPS callback handler - we will call it 
// from the forms event handler to spoof DPS Events.
//

extern BOOL CALLBACK CallBack_Handler(const long Msg,
                                      const WPARAM wParam,
                                      const LPARAM lParam);


class CSCOTAuxDisp : public CSCOTDevice
{
public:
	virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID) = 0;
	virtual long UnInitialize() = 0;
	virtual long Disable() = 0;
	virtual long Enable() = 0;

  CSCOTAuxDisp();
	virtual ~CSCOTAuxDisp();
  virtual void DataEvent(long Status) {};
  virtual void ErrorEvent(LONG ResultCode, 
                          LONG ResultCodeExtended, 
                          LONG ErrorLocus, 
                          LONG* pErrorResponse) {};

  virtual long ClearScreen() = 0;
  virtual BOOL HasMemory() = 0;
  virtual long ClearMemory() = 0;
  virtual long LoadMemory() = 0;
  virtual long CompressMemory() = 0;
  virtual long SetLanguage(SCOT_LANGUAGE_TYPE eLanguage);

  virtual long ShowScreen(SCOT_AUX_SCREEN eScreen, bool bForceRefresh = false);
  virtual long VarShowScreen(SCOT_AUX_SCREEN eScreen, bool bForceRefresh, int nArgs, ...);
  virtual long VarShowScreenV(SCOT_AUX_SCREEN eScreen, bool bForceRefresh, int nArgs, va_list argptr);

protected:
  virtual long SetCurrentLanguage(CString& csLanguage);
  virtual long ReadCashbackInfo(RSection* pSection);
  virtual long UnloadScreensInfo();

  CString    m_DefaultLang;
  CSCOTAuxScreen* m_pCurrentScreen;

  int        m_nCurrentLangNdx;
  CString    m_csCurrentLanguage;
  bool       m_bForceRefreshAlways;

  SCOT_AUX_SCREEN m_DefaultScreen;
  SCOT_AUX_SCREEN m_CurrentScreen;
  CArray<AUX_SCREEN_LANG_INFO*, AUX_SCREEN_LANG_INFO*> m_ScreenLangArray;
  CArray<CString, CString&> m_CashbackValues;
  int                       m_IntCashbackValues[4];
};


//
// This class is for use by 1.1-type platforms where there is no
// concept of an Auxilary Display device.
//
class CSCOTNullAuxDisp : public CSCOTAuxDisp
{
public:
  virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID) { return 0;}
	virtual long UnInitialize() { return 0;}
	virtual long Disable() { return 0;}
	virtual long Enable() { return 0;}

  CSCOTNullAuxDisp() {};
  virtual ~CSCOTNullAuxDisp() {};
  virtual void DataEvent(long Status) {};
  virtual void ErrorEvent(LONG ResultCode, 
                          LONG ResultCodeExtended, 
                          LONG ErrorLocus, 
                          LONG* pErrorResponse) {};

  virtual long ClearScreen() { return 0;}
  virtual BOOL HasMemory() { return FALSE;}
  virtual long ClearMemory() { return 0;}
  virtual long LoadMemory() { return 0;}
  virtual long CompressMemory() { return 0;}
  virtual long SetLanguage(SCOT_LANGUAGE_TYPE eLanguage) {return 0;}

  virtual long ShowScreen(SCOT_AUX_SCREEN eScreen, bool bForceRefresh = false) { return 0;}
  virtual long VarShowScreen(SCOT_AUX_SCREEN eScreen, bool bForceRefresh, int nArgs, ...) { return 0;}
  virtual long VarShowScreenV(SCOT_AUX_SCREEN eScreen, bool bForceRefresh, int nArgs, va_list argptr) { return 0;}
};


#endif
// SCOTPromptDisp.h: interface for the CSCOTPromptDisp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOTPromptDisp_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)
#define AFX_SCOTPromptDisp_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_

#include "SCOTAuxDisp.h"
#include "SCOTUtil.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CSCOTPromptScreen : public CSCOTAuxScreen
{
public:
  CSCOTPromptScreen();
  virtual ~CSCOTPromptScreen();

  virtual long Initialize(CString& csPromptInfo, int nColumns);
  virtual void MessageInit() {};
  virtual void DataEvent(long Status) {};
  virtual long DisplayV(int nArgs, va_list argptr);

protected:
  long    m_lOPOSPromptNum;
  CString m_csPrompt;
  bool    m_bBlankFollowupPrompt;
};


class CSCOTPromptDisp : public CSCOTAuxDisp
{
public:
  virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID);
	virtual long UnInitialize();
	virtual long Disable(){return 0;}
	virtual long Enable(){return 0;}

  CSCOTPromptDisp();
  virtual ~CSCOTPromptDisp();
  virtual void DataEvent(long Status){}
  virtual void ErrorEvent(LONG ResultCode, 
                          LONG ResultCodeExtended, 
                          LONG ErrorLocus, 
                          LONG* pErrorResponse){}

  virtual long ClearScreen() {return 0;}
  virtual BOOL HasMemory(){return FALSE;}
  virtual long ClearMemory(){return 0;}
  virtual long LoadMemory(){return 0;}
  virtual long CompressMemory(){return 0;}

protected:
  long ReadPromptsInfo(CString& cfgFile);
  long ReadRegistry();
  virtual CSCOTAuxScreen* FormFactory(SCOT_AUX_SCREEN eScreen, CString& csScreen);

  CString m_PromptsCfgFile;
  int m_nColumns;

};

#endif
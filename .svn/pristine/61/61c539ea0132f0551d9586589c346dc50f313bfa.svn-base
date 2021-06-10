// SCOTEncryptor.h: interface for the CSCOTEncryptor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOTENCRYPTOR_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)
#define AFX_SCOTENCRYPTOR_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "opospinpad.h"	// Added by ClassView
#include "oposppad.h"
#include "TransBroker.h"

// OPOS defines PPAD_MSG_SELECTCARDTYPE as the last valid PINPad prompt

#define PPAD_MAX_PROMPTS  PPAD_MSG_SELECTCARDTYPE+1


class CSCOTEncryptor : public CSCOTDevice
{
  friend class CSCOTPromptScreen;
public:
	CSCOTEncryptor();
	virtual ~CSCOTEncryptor();

	virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID) = 0;
	virtual long UnInitialize() = 0;
	virtual long Disable() = 0;
	virtual long Enable() = 0;
  virtual void DataEvent(long Status) {};
  virtual void ErrorEvent(LONG ResultCode, 
                          LONG ResultCodeExtended, 
                          LONG ErrorLocus, 
                          LONG* pErrorResponse) {};

	virtual long ValidateMasterKey() = 0;
	virtual long StoreMasterKeyFromPinpad() = 0;
  virtual bool CanStoreMasterKeyFromPinpad() = 0;
  virtual long StoreWorkingKey(long lKeyNum, LPCTSTR szWorkingKey) = 0;
	virtual long DoPINTransaction(CString& csAcctNum,
                                LPCTSTR szWorkingKey, 
                                TBPINBLOCKFORMAT nPinBlockFormat, 
                                TBPINPADSYSTEM   nPinpadSystem,
                                TBPINENCODING    nPinBlockEncoding) = 0;
	virtual long CancelPINEntry() = 0;
	virtual CString GetEncryptedPIN() = 0;

  virtual long SetPINPrompt(CString& csPrompt) { return 0; };
  virtual void SetMSREnabled(bool bEnabled) { m_bMSREnabled = bEnabled; }

protected:
  virtual long GetWorkingKeySlotForPinTransaction() = 0;
  virtual long DisplayPrompt(const LONG lOPOSPromptNum) {return 0;}
  virtual long SetPromptText(const LONG lOPOSPromptNum, CString& csText, bool bBlankFollowupPrompt) {return 0;}

  bool m_bMSREnabled;
	TBPINPADSYSTEM m_nLastPinpadSystem;
  TBPINENCODING  m_nPinBlockEncoding;
};


class CSCOTOPOSPinpad : public CSCOTEncryptor
{
public:
	virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID);
	virtual long UnInitialize();
	virtual long CancelPINEntry();
	virtual long Disable();
	virtual long Enable();
	virtual CString GetEncryptedPIN();
	virtual long ValidateMasterKey();
	virtual long StoreMasterKeyFromPinpad();
  virtual bool CanStoreMasterKeyFromPinpad() { return false;};;
  virtual long StoreWorkingKey(long lKeyNum, LPCTSTR szWorkingKey);
	virtual long DoPINTransaction(CString& csAcctNum,
                                LPCTSTR szWorkingKey, 
                                TBPINBLOCKFORMAT nPinBlockFormat, 
                                TBPINPADSYSTEM   nPinpadSystem,
                                TBPINENCODING    nPinBlockEncoding);

  CSCOTOPOSPinpad();
	virtual ~CSCOTOPOSPinpad();
  virtual void DataEvent(long Status);
  virtual void ErrorEvent(LONG ResultCode, 
                          LONG ResultCodeExtended, 
                          LONG ErrorLocus, 
                          LONG* pErrorResponse);
  virtual long SetPINPrompt(CString& csPrompt);

protected:
  virtual long GetWorkingKeySlotForPinTransaction();
  virtual long DisplayPrompt(const LONG lOPOSPromptNum);
  virtual long SetPromptText(const LONG lOPOSPromptNum, CString& csText, bool bBlankFollowupPrompt);

  virtual CString TranslatePromptText(CString& csText) { return csText; }

  virtual long SetPromptTextThruDirectIO(const long lDIOPromptNum, 
                                         CString& csText,
                                         bool bIsFollowupPrompt,
                                         bool bNeedToStore);
  virtual long GetFollowupPrompt(const long lDIOPromptNum) { return -1; }

  virtual bool NeedToStorePrompt(const long lDIOPromptNum, CString& csPrompt);

  long m_lDirectIOPrompts[PPAD_MAX_PROMPTS];
  long m_CapDisplay;
  CString m_csPINPrompt;

  CMap<long, long, CString, CString&> m_LastPromptTextMap;

	COPOSPINPad m_Pinpad;
};


class CSCOTBAPE : public CSCOTEncryptor
{
public:
	virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID);
	virtual long UnInitialize();
	virtual long CancelPINEntry();
	virtual long Disable();
	virtual long Enable();
	virtual CString GetEncryptedPIN();
	virtual long ValidateMasterKey();
	virtual long StoreMasterKeyFromPinpad();
  virtual bool CanStoreMasterKeyFromPinpad() { return true;};
  virtual long StoreWorkingKey(long lKeyNum, LPCTSTR szWorkingKey);
	CSCOTBAPE();
	virtual ~CSCOTBAPE();
	virtual long DoPINTransaction(CString& csAcctNum,
                                LPCTSTR szWorkingKey, 
                                TBPINBLOCKFORMAT nPinBlockFormat, 
                                TBPINPADSYSTEM   nPinpadSystem,
                                TBPINENCODING    nPinBlockEncoding);

protected:
  virtual long GetWorkingKeySlotForPinTransaction();
};


class CSCOT5992Pinpad : public CSCOTOPOSPinpad
{
public:
  CSCOT5992Pinpad() {};
  virtual ~CSCOT5992Pinpad() {};

protected:
  virtual long SetPINPrompt(CString& csPrompt);
};



class CSCOT5945Pinpad : public CSCOTOPOSPinpad
{
public:
  CSCOT5945Pinpad();
  virtual ~CSCOT5945Pinpad() {};

protected:
  virtual CString TranslatePromptText(CString& csText);
  virtual long SetPromptTextThruDirectIO(const long lDIOPromptNum, 
                                         CString& csText,
                                         bool bIsFollowupPrompt,
                                         bool bNeedToStore);
  virtual long GetFollowupPrompt(const long lDIOPromptNum);
  virtual bool NeedToStorePrompt(const long lDIOPromptNum, CString& csPrompt);
  char* ConvertLPWSTRToLPSTR (LPWSTR lpwszStrIn);

  BYTE ComputeBCC(BYTE *pCmd, int nCmdLen);
  long DirectIO(BYTE *pCommand, int nCmdLen);
};



class CSCOTC2000Pinpad : public CSCOTOPOSPinpad
{
public:
  CSCOTC2000Pinpad() {};
  virtual ~CSCOTC2000Pinpad() {};

protected:
  virtual long SetPromptTextThruDirectIO(const long lDIOPromptNum, 
                                         CString& csText,
                                         bool bIsFollowupPrompt,
                                         bool bNeedToStore);
};


class CSCOTEmulatorPinpad : public CSCOTC2000Pinpad
{
public:
  CSCOTEmulatorPinpad() {};
  virtual ~CSCOTEmulatorPinpad() {};
};

#endif // !defined(AFX_SCOTENCRYPTOR_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)

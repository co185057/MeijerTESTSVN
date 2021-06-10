// SCOTEncryptor.cpp: implementation of the CSCOTEncryptor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"                 // common includes
#include "SCOTEncryptor.h"
#include "ScotPPad.h"
#include "DMProcedures.h"           // DM procedures

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define COMP_ID ID_OD
#define T_ID    _T("SCOTEncryptor")


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSCOTEncryptor::CSCOTEncryptor()
{
  m_bMSREnabled = false;
}

CSCOTEncryptor::~CSCOTEncryptor()
{
  m_nLastPinpadSystem = TB_PINSYSTEM_MS;
  m_nPinBlockEncoding = TB_PINENCODING_COMPATIBLE;
}


//////////////////////////////////////////////////////////////////////
// CSCOTOPOSPinpad methods - these use an OPOS Pinpad control
//////////////////////////////////////////////////////////////////////


CSCOTOPOSPinpad::CSCOTOPOSPinpad()
{
  // Default will set the value of the DirectIO Prompt# equal to the
  // value of the OPOS prompt number

  for (int i=0; i < PPAD_MAX_PROMPTS; i++)
    m_lDirectIOPrompts[i] = i;

}


CSCOTOPOSPinpad::~CSCOTOPOSPinpad()
{
  m_LastPromptTextMap.RemoveAll();
}

long CSCOTOPOSPinpad::GetWorkingKeySlotForPinTransaction()
{
  return 1;  // OPOS always uses 1 for key slot in SCOT transactions
}


long CSCOTOPOSPinpad::DoPINTransaction(CString& csAcctNum, 
                                      LPCTSTR szWorkingKey, 
                                      TBPINBLOCKFORMAT nPinBlockFormat, 
                                      TBPINPADSYSTEM   nPinpadSystem,
                                      TBPINENCODING    nPinBlockEncoding)
{
  long rc = 0;

  // Most of the values that BeginEFTTransaction() requires can be 
  // anything, as long as it is syntactically valid.
  //
  // Account#, TransactionType, and encryption type must be real values

  CY amount;
  amount.Hi = 0;
  amount.Lo = 100;
  m_Pinpad.SetAccountNumber(csAcctNum);
  m_Pinpad.SetMerchantID(_T("1"));
  m_Pinpad.SetTerminalID(_T("1"));
  m_Pinpad.SetTrack1Data(_T(""));
  m_Pinpad.SetTrack2Data(_T(""));
  m_Pinpad.SetTrack3Data(_T(""));
  m_Pinpad.SetTransactionType(PPAD_TRANS_DEBIT);

  CString csEncryptionType = _T("Unknown");
  if (nPinpadSystem == TB_PINSYSTEM_MS)
    csEncryptionType = _T("M/S");
  else if (nPinpadSystem == TB_PINSYSTEM_DUKPT)
    csEncryptionType = _T("DUKPT");
  else
    ASSERT(FALSE);

  SetPromptText(PPAD_MSG_ENTERPIN, m_csPINPrompt, true);

  CString func = _T("BeginEFTTransaction");

  rc = m_Pinpad.BeginEFTTransaction(csEncryptionType, GetWorkingKeySlotForPinTransaction());
  if (rc == OPOS_SUCCESS)
  {
    // Only store a new working key if it is non-NULL - this will
    // allow the core app to re-use the last working key by passing NULL
    // in this function
    if (szWorkingKey && (_tcsclen(szWorkingKey) > 0))
    {
      func = _T("StoreWorkingKey");
      rc = StoreWorkingKey(GetWorkingKeySlotForPinTransaction(), szWorkingKey);  
    }

    if (rc == 0)
    {
      func = _T("EnablePINEntry");
      rc = m_Pinpad.EnablePINEntry();
      if (rc == OPOS_SUCCESS)
      {
        // Set which type of encryption this is:
        m_nLastPinpadSystem = nPinpadSystem;
        m_nPinBlockEncoding = nPinBlockEncoding;

        // Core app is expecting an event when we go into secure mode.
        // The OPOS model doesn't do this, so let's fake one up!

        long Status = DM_ENCRYPT_SECURE_MODE;
        mo.PostDM(DM_DATA, DMCLASS_ENCRYPTOR, 0, sizeof(VT_I4), (void*)&Status);
      }
    }

    if (rc != OPOS_SUCCESS)
    {
      m_Pinpad.EndEFTTransaction(PPAD_EFT_ABNORMAL);
    }
  }

  if (rc != OPOS_SUCCESS)
  {
	// TAR 320164 Don't display customer data
    ScotError(RETURN, SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_SE_PIN_TRANS_FAILED, 
          _T("%s|%d|%d|%s|%d|%d|%d"),
          func, rc, m_Pinpad.GetResultCodeExtended(),
          szWorkingKey ? szWorkingKey : _T("NULL"),
          nPinBlockFormat, nPinpadSystem,
          GetWorkingKeySlotForPinTransaction());
  }

  return TranslateOPOSRetToSCOTRet(rc);
}

long CSCOTOPOSPinpad::StoreMasterKeyFromPinpad()
{
  // Unsupported!
  return -1;
}


long CSCOTOPOSPinpad::StoreWorkingKey(long lKeyNum, LPCTSTR szWorkingKey)
{
  // let's try a work-around for the 5945 problem where
  // our first store key after MSR swipe fails for some weird reason;
  //
  // Note that this is a BIG kludge that should be fixed by an OPOS fix, and
  // this code should go away!
  static CString csLastWorkingKey = _T("");
  long rc = OPOS_SUCCESS;
  if (csLastWorkingKey != szWorkingKey)
  {
	  rc = m_Pinpad.UpdateKey(lKeyNum, szWorkingKey);
	  if (rc != OPOS_SUCCESS)
	  {
		Sleep(250);
		rc = m_Pinpad.UpdateKey(lKeyNum, szWorkingKey);
	  }
	  csLastWorkingKey = szWorkingKey;
  }
  return rc;
}


long CSCOTOPOSPinpad::ValidateMasterKey()
{
  long rc = 0;
  // Unsupported - but we will assume success for the sake of
  // the SCOT app
  return rc;
}

#define VAL_FROM_char(ch) (((ch >= _T('0')) && (ch <= _T('9'))) ? (ch - _T('0')) : (ch - _T('A') + 10))

CString CSCOTOPOSPinpad::GetEncryptedPIN()
{
  CString csEncryptedPIN = m_Pinpad.GetEncryptedPIN();

  if (m_nPinBlockEncoding == TB_PINENCODING_COMPATIBLE)
  {
    // Need to convert back to Binary to work like SCOT 1.1 did.  Doesn't this
    // just absolutely bite?  The sins of the parents really ARE visted on the children
    // unto the third and fourth generation.
  
    // First capitalize any hex chars so our macro above works correctly
    csEncryptedPIN.MakeUpper();

    int lPinLen = csEncryptedPIN.GetLength();

    CString csBinaryPIN;

    // binary version of PIN takes up 1/2 the number of bytes as ASCII version
    int lPinBufLen = (lPinLen / 2);


    LPTSTR pchar = csBinaryPIN.GetBuffer(lPinBufLen + 1);

    int i = 0;
    while (i < lPinLen)
    {
      *pchar = (VAL_FROM_char(csEncryptedPIN.GetAt(i)) << 4) + VAL_FROM_char(csEncryptedPIN.GetAt(i+1));
      i += 2;
      pchar++;
    }
    *pchar = 0; // Null Terminate

    csBinaryPIN.ReleaseBuffer(lPinBufLen); // Set string's length to 1/2 of ASCII length 
    csEncryptedPIN = csBinaryPIN;
  }

  if (m_nLastPinpadSystem == TB_PINSYSTEM_DUKPT)
  {
    csEncryptedPIN += m_Pinpad.GetAdditionalSecurityInformation();
    ASSERT(csEncryptedPIN.GetLength() == (m_nPinBlockEncoding == TB_PINENCODING_ASCII ? 36 : 28));
  }
  else
  {
    ASSERT(csEncryptedPIN.GetLength() == (m_nPinBlockEncoding == TB_PINENCODING_ASCII ? 16 : 8));
  }

  return csEncryptedPIN;
}

long CSCOTOPOSPinpad::Enable()
{
  long rc = 0;
  
  m_Pinpad.SetDeviceEnabled(TRUE);
  rc = m_Pinpad.GetResultCode();
  return TranslateOPOSRetToSCOTRet(rc);
}

long CSCOTOPOSPinpad::Disable()
{
  long rc = 0;
  
  m_Pinpad.SetDeviceEnabled(FALSE);
  rc = m_Pinpad.GetResultCode();
  return TranslateOPOSRetToSCOTRet(rc);
}

long CSCOTOPOSPinpad::CancelPINEntry()
{
  long rc;;
  rc = m_Pinpad.EndEFTTransaction(PPAD_EFT_ABNORMAL);
  return TranslateOPOSRetToSCOTRet(rc);
}

long CSCOTOPOSPinpad::Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID)
{
  long rc = 0;

  m_csProfile = profile;

  m_pMainWnd = pMainWnd;

  if (!m_Pinpad.Initialize(profile, pParentWnd, nID))
  {
    CString csErr = m_Pinpad.GetErrorFunc();
    ScotError(RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_OPOS_CTRL_INIT_FAILURE, _T("%s|%s"), 
          profile, csErr);
    rc = -1;
  }
  else
  {
    m_CapDisplay = m_Pinpad.GetCapDisplay();
  }
  
  return rc;
}

long CSCOTOPOSPinpad::UnInitialize()
{
  long rc = 0;
  
  m_Pinpad.UnInitialize();
  return rc;
}

void CSCOTOPOSPinpad::DataEvent(long Status)
{
  long DMStatus;
  if (Status == PPAD_SUCCESS)
  {
    m_Pinpad.EndEFTTransaction(PPAD_EFT_NORMAL);
    DMStatus = DM_ENCRYPT_COMPLETE;
  }
  else if (Status == PPAD_CANCEL)
  {
    DMStatus = DM_ENCRYPT_CANCELED;
  }
  else
    ASSERT(FALSE);

  mo.PostDM(DM_DATA, DMCLASS_ENCRYPTOR, 0, sizeof(VT_I4), (void*)&DMStatus);

  m_Pinpad.SetDataEventEnabled(TRUE);
}


void CSCOTOPOSPinpad::ErrorEvent(LONG ResultCode, 
                                 LONG ResultCodeExtended, 
                                 LONG ErrorLocus, 
                                 LONG* pErrorResponse)
{
}


long CSCOTOPOSPinpad::DisplayPrompt(const LONG lOPOSPromptNum)
{
  ASSERT(lOPOSPromptNum != 0);

  long rc = 0;
  if ((m_CapDisplay == PPAD_DISP_RESTRICTED_LIST) || (m_CapDisplay == PPAD_DISP_RESTRICTED_ORDER))
  {
    m_Pinpad.SetPrompt(lOPOSPromptNum);
    rc = m_Pinpad.GetResultCode();
  }

  return rc;
}

long CSCOTOPOSPinpad::SetPINPrompt(CString& csPrompt)
{
  m_csPINPrompt = csPrompt;
  return 0;
}


bool CSCOTOPOSPinpad::NeedToStorePrompt(const long lDIOPromptNum, CString& csPrompt)
{
  bool rc = true;
  
  CString oldPrompt;
  if ((m_LastPromptTextMap.Lookup(lDIOPromptNum, oldPrompt)) && 
      (oldPrompt.Compare(csPrompt) == 0))
  {
      rc = false;
  }
  else
  {
    rc = true;
    m_LastPromptTextMap.SetAt(lDIOPromptNum, csPrompt);
  }
  
  return rc;
}


long CSCOTOPOSPinpad::SetPromptText(const LONG lOPOSPromptNum, CString& csText, bool bBlankFollowupPrompt)
{
  ASSERT(lOPOSPromptNum != 0);
  long rc = 0;
  bool bNeedToStore;

  long lDIOPromptNum = m_lDirectIOPrompts[lOPOSPromptNum];

  // Only set the prompt text if the new one is different from the old

  CString csTranslatedText = TranslatePromptText(csText);

  CString csPrompt = csTranslatedText;

  bNeedToStore = NeedToStorePrompt(lDIOPromptNum, csPrompt);
  rc = SetPromptTextThruDirectIO(lDIOPromptNum, csPrompt, false, bNeedToStore);

  if (rc == 0)
  {
    lDIOPromptNum = GetFollowupPrompt(m_lDirectIOPrompts[lOPOSPromptNum]);

    if (lDIOPromptNum >= 0)
    {
      csPrompt = bBlankFollowupPrompt ? CString(_T(" ")) : csTranslatedText;

      bNeedToStore = NeedToStorePrompt(lDIOPromptNum, csPrompt);
      rc = SetPromptTextThruDirectIO(lDIOPromptNum, csPrompt, true, bNeedToStore);
    }
  }

  if (rc != 0)
  {
    ScotError(RETURN, SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_SE_SET_PROMPT_FAILED, 
          _T("%s|%d|%d|%d"), csPrompt, rc, lOPOSPromptNum, lDIOPromptNum);
  }

  return rc;
}


// TAR 193729 - Removed #if 0
//////////////////////////////////////////////////////////////////////
// CSCOTBAPE methods - these use the SCOT 1.1 DM BAPE interface
//////////////////////////////////////////////////////////////////////


CSCOTBAPE::CSCOTBAPE()
{

}


CSCOTBAPE::~CSCOTBAPE()
{

}

long CSCOTBAPE::GetWorkingKeySlotForPinTransaction()
{
  return 2;  // BAPE always uses 2 for key slot in SCOT transactions
}


long CSCOTBAPE::DoPINTransaction(CString& csAcctNum, 
                                  LPCTSTR szWorkingKey, 
                                  TBPINBLOCKFORMAT nPinBlockFormat, 
                                  TBPINPADSYSTEM   nPinpadSystem,
                                  TBPINENCODING    nPinBlockEncoding)
{
  long rc = 0;
  trace(L6,_T("+CSCOTBAPE::DoPINTransaction"));

  // Only store a new working key if it is non-NULL - this will
  // allow the core app to re-use the last working key by passing NULL
  // in this function

  if (szWorkingKey && (_tcsclen(szWorkingKey) > 0))
    rc = StoreWorkingKey(GetWorkingKeySlotForPinTransaction(), szWorkingKey);  

  if (rc == 0)
  {
    long lDMPinBlockFormat;
    if (nPinBlockFormat == TB_DIEBOLD)
      lDMPinBlockFormat = DM_ENCRYPTOR_PINBLOCK_FORMAT_DIEBOLD;
    else // We only support the 2 right now.
      lDMPinBlockFormat = DM_ENCRYPTOR_PINBLOCK_FORMAT_ANSI;

    rc = -1; // dm.SetEncryptorPINBlockFormat(lDMPinBlockFormat);
	  if (rc == 0)
    {
      m_nLastPinpadSystem = nPinpadSystem;
      m_nPinBlockEncoding = nPinBlockEncoding;

	    CString csA = csAcctNum;
	    csA = csA.Right(13);
	    csA = csA.Left(12);
		  rc = -1; // dm.EncryptorGetPinBlock(GetWorkingKeySlotForPinTransaction(), csA);
    }
  }

  trace(L6,_T("-CSCOTBAPE::DoPINTransaction %d"),rc);
  return rc;
}


long CSCOTBAPE::StoreMasterKeyFromPinpad()
{
  long rc = 0;
  trace(L6,_T("+CSCOTBAPE::StoreMasterKeyFromPinpad"));
	
	// see comments in DMEncryptorSetup(void)
	rc = -1; 
#if 0
	dm.EncryptorStoreKey(1,
		                        NULL,
		                        DM_ENCRYPTOR_FROM_PINPAD,
		                        DM_ENCRYPTOR_NO_EXCHANGE_KEY);
#endif	
  trace(L6,_T("-CSCOTBAPE::StoreMasterKeyFromPinpad %d"),rc);
  return rc;
}


long CSCOTBAPE::StoreWorkingKey(long lKeyNum, LPCTSTR szWorkingKey)
{
  return -1; // dm.EncryptorStoreKey(lKeyNum, szWorkingKey, 0, 1);
}

long CSCOTBAPE::ValidateMasterKey()
{
  long rc = 0;
  trace(L6,_T("+CSCOTBAPE::ValidateMasterKey"));
  rc = -1; // dm.EncryptorKeyTest();
  trace(L6,_T("-CSCOTBAPE::ValidateMasterKey %d"),rc);
  return rc;
}

CString CSCOTBAPE::GetEncryptedPIN()
{
  CString csRet;
  trace(L6,_T("+CSCOTBAPE::GetEncryptedPIN"));
	
  csRet = _T("-1"); // dm.GetEncryptorEncryptedPIN();

  // If TB wants the PIN in ASCII, we have to convert for BAPE,
  // since it always returns a binary PIN block natively.

  if (m_nPinBlockEncoding == TB_PINENCODING_ASCII)
  {
    CString ascii;
    LPTSTR p1;
    _TUCHAR *b1;
    p1 = ascii.GetBuffer(32);;
    b1 = (_TUCHAR *)(LPCTSTR) csRet;
		
    for (int i1=0;i1<8;i1++)
    {
      _stprintf(p1,_T("%02x"),*b1++);
      p1 += 2;
    }

    ascii.ReleaseBuffer();
    ascii.MakeUpper();
    csRet = ascii;
  }
	
  trace(L6,_T("-CSCOTBAPE::GetEncryptedPIN <%s>"),csRet);
  return csRet;
}

long CSCOTBAPE::Enable()
{
  long rc = 0;
  trace(L6,_T("+CSCOTBAPE::Enable"));
	
  rc = -1; // dm.EncryptorEnable();			  // enable the encryptor
	
  trace(L6,_T("-CSCOTBAPE::Enable %d"),rc);
  return rc;
}

long CSCOTBAPE::Disable()
{
  long rc = 0;
  trace(L6,_T("+CSCOTBAPE::Disable"));
	
  rc = -1; // dm.EncryptorDisable();			  // disable the encryptor
	
  trace(L6,_T("-CSCOTBAPE::Disable %d"),rc);
  return rc;
}

long CSCOTBAPE::CancelPINEntry()
{
  long rc = 0;
  trace(L6,_T("+CSCOTBAPE::CancelPINEntry"));
	
  rc = -1; // dm.EncryptorDisable();			  // disable the encryptor
  rc = -1; // dm.EncryptorEnable();			  // re-enable the encryptor
	
  trace(L6,_T("-CSCOTBAPE::CancelPINEntry %d"),rc);
  return rc;
}

long CSCOTBAPE::Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID)
{
  long rc = 0;
  
  m_csProfile = profile;

  // All of our initialization is done by DMInterface, so this
  // is a no-brainer.

  return rc;
}

long CSCOTBAPE::UnInitialize()
{
  long rc = 0;
  
  // All of our uninitialization is done by DMInterface, so this
  // is a no-brainer.

  return rc;
}

// TAR 193729 Removed #endif

long CSCOT5992Pinpad::SetPINPrompt(CString& csPrompt)
{
  LONG myLong;

  BSTR bPrompt = csPrompt.AllocSysString();
  long rc = m_Pinpad.DirectIO(1, &myLong, &bPrompt);
  SysFreeString(bPrompt);
  return rc;
}



CSCOT5945Pinpad::CSCOT5945Pinpad()
{
  // Map the pre-defined OPOS prompts to the
  // prompt values the NCR 5945 uses for its
  // various states.
  // See the 5945 Developer's Guide for more info

  m_lDirectIOPrompts[0]                        = 0;
  m_lDirectIOPrompts[PPAD_MSG_ENTERPIN]        = 6;
  m_lDirectIOPrompts[PPAD_MSG_PLEASEWAIT]      = 3;
  m_lDirectIOPrompts[PPAD_MSG_ENTERVALIDPIN]   = 6;
  m_lDirectIOPrompts[PPAD_MSG_RETRIESEXCEEDED] = 3;
  m_lDirectIOPrompts[PPAD_MSG_APPROVED]        = 0;
  m_lDirectIOPrompts[PPAD_MSG_DECLINED]        = 0;
  m_lDirectIOPrompts[PPAD_MSG_CANCELED]        = 3;
  m_lDirectIOPrompts[PPAD_MSG_AMOUNTOK]        = 15;
  m_lDirectIOPrompts[PPAD_MSG_NOTREADY]        = 0;
  m_lDirectIOPrompts[PPAD_MSG_IDLE]            = 3;
  m_lDirectIOPrompts[PPAD_MSG_SLIDE_CARD]      = 2;
  m_lDirectIOPrompts[PPAD_MSG_INSERTCARD]      = 2;
  m_lDirectIOPrompts[PPAD_MSG_SELECTCARDTYPE]  = 15;

}

bool CSCOT5945Pinpad::NeedToStorePrompt(const long lDIOPromptNum, CString& csPrompt)
{
  bool rc;

  // If the prompt we are going to use is actually a 5945 prompt (as
  // opposed to just displaying text), then call the base class
  // NeedToStore() function.  Otherwise, just return false
  // because we aren't storing anything, just displaying.

  if ((((m_bMSREnabled) && (lDIOPromptNum  == 2)) || 
       (lDIOPromptNum  == 6) || 
       (lDIOPromptNum == 3)))
  {
    rc = CSCOTOPOSPinpad::NeedToStorePrompt(lDIOPromptNum, csPrompt);
  }
  else
    rc = false;

  return rc;

}


_TUCHAR cTranslateWinCharTo5945(_TUCHAR cCurrent)
{ 
  static TCHAR translate[224] = 
  { 
    (_TCHAR) 0x20, (_TCHAR) 0x21, (_TCHAR) 0x22, (_TCHAR) 0x23,  //0x20
    (_TCHAR) 0x24, (_TCHAR) 0x25, (_TCHAR) 0x26, (_TCHAR) 0x27, 
    (_TCHAR) 0x28, (_TCHAR) 0x29, (_TCHAR) 0x2A, (_TCHAR) 0x2B, 
    (_TCHAR) 0x2C, (_TCHAR) 0x2D, (_TCHAR) 0x2E, (_TCHAR) 0x2F, 

    (_TCHAR) 0x30, (_TCHAR) 0x31, (_TCHAR) 0x32, (_TCHAR) 0x33,  //0x30
    (_TCHAR) 0x34, (_TCHAR) 0x35, (_TCHAR) 0x36, (_TCHAR) 0x37, 
    (_TCHAR) 0x38, (_TCHAR) 0x39, (_TCHAR) 0x3A, (_TCHAR) 0x3B, 
    (_TCHAR) 0x3C, (_TCHAR) 0x3D, (_TCHAR) 0x3E, (_TCHAR) 0x3F, 

    (_TCHAR) 0x40, (_TCHAR) 0x41, (_TCHAR) 0x42, (_TCHAR) 0x43,  //0x40
    (_TCHAR) 0x44, (_TCHAR) 0x45, (_TCHAR) 0x46, (_TCHAR) 0x47, 
    (_TCHAR) 0x48, (_TCHAR) 0x49, (_TCHAR) 0x4A, (_TCHAR) 0x4B, 
    (_TCHAR) 0x4C, (_TCHAR) 0x4D, (_TCHAR) 0x4E, (_TCHAR) 0x4F, 

    (_TCHAR) 0x50, (_TCHAR) 0x51, (_TCHAR) 0x52, (_TCHAR) 0x53,  //0x50
    (_TCHAR) 0x54, (_TCHAR) 0x55, (_TCHAR) 0x56, (_TCHAR) 0x57, 
    (_TCHAR) 0x58, (_TCHAR) 0x59, (_TCHAR) 0x5A, (_TCHAR) 0x5B, 
    (_TCHAR) 0xDA, (_TCHAR) 0x5D, (_TCHAR) 0x5E, (_TCHAR) 0x5F, 

    (_TCHAR) 0x60, (_TCHAR) 0x61, (_TCHAR) 0x62, (_TCHAR) 0x63,  //0x60
    (_TCHAR) 0x64, (_TCHAR) 0x65, (_TCHAR) 0x66, (_TCHAR) 0x67, 
    (_TCHAR) 0x68, (_TCHAR) 0x69, (_TCHAR) 0x6A, (_TCHAR) 0x6B, 
    (_TCHAR) 0x6C, (_TCHAR) 0x6D, (_TCHAR) 0x6E, (_TCHAR) 0x6F, 

    (_TCHAR) 0x70, (_TCHAR) 0x71, (_TCHAR) 0x72, (_TCHAR) 0x73,  //0x70
    (_TCHAR) 0x74, (_TCHAR) 0x75, (_TCHAR) 0x76, (_TCHAR) 0x77, 
    (_TCHAR) 0x78, (_TCHAR) 0x79, (_TCHAR) 0x7A, (_TCHAR) 0x7B, 
    (_TCHAR) 0x7C, (_TCHAR) 0x7D, (_TCHAR) 0xD5, (_TCHAR) 0x20, 
 
    (_TCHAR) 0x20, (_TCHAR) 0x20, (_TCHAR) 0x2C, (_TCHAR) 0xB6,  //0x80
    (_TCHAR) 0x22, (_TCHAR) 0xD1, (_TCHAR) 0x20, (_TCHAR) 0x20, 
    (_TCHAR) 0x20, (_TCHAR) 0x20, (_TCHAR) 0x53, (_TCHAR) 0x3C, 
    (_TCHAR) 0x20, (_TCHAR) 0x20, (_TCHAR) 0x20, (_TCHAR) 0x20,

    (_TCHAR) 0x20, (_TCHAR) 0x27, (_TCHAR) 0x27, (_TCHAR) 0x22,  //0x90
    (_TCHAR) 0x22, (_TCHAR) 0xB2, (_TCHAR) 0x20, (_TCHAR) 0x20, 
    (_TCHAR) 0xD5, (_TCHAR) 0x20, (_TCHAR) 0x73, (_TCHAR) 0x3E, 
    (_TCHAR) 0x20, (_TCHAR) 0x20, (_TCHAR) 0x20, (_TCHAR) 0xCA,

    (_TCHAR) 0x20, (_TCHAR) 0xA5, (_TCHAR) 0xCC, (_TCHAR) 0xB7,  //0xA0
    (_TCHAR) 0xC6, (_TCHAR) 0x5C, (_TCHAR) 0x7C, (_TCHAR) 0xB0, 
    (_TCHAR) 0xD1, (_TCHAR) 0xCD, (_TCHAR) 0xA0, (_TCHAR) 0xD7, 
    (_TCHAR) 0x20, (_TCHAR) 0x20, (_TCHAR) 0xC5, (_TCHAR) 0x20,

    (_TCHAR) 0x00, (_TCHAR) 0xB1, (_TCHAR) 0x32, (_TCHAR) 0x33,  //0xB0
    (_TCHAR) 0xD4, (_TCHAR) 0xC8, (_TCHAR) 0xBC, (_TCHAR) 0xD0, 
    (_TCHAR) 0xD3, (_TCHAR) 0x31, (_TCHAR) 0xF4, (_TCHAR) 0xD8, 
    (_TCHAR) 0xE3, (_TCHAR) 0xE2, (_TCHAR) 0xF5, (_TCHAR) 0x8F,

    (_TCHAR) 0x9C, (_TCHAR) 0x9D, (_TCHAR) 0x9B, (_TCHAR) 0xCB,  //0xC0
    (_TCHAR) 0x99, (_TCHAR) 0x98, (_TCHAR) 0xAF, (_TCHAR) 0xC4, 
    (_TCHAR) 0x91, (_TCHAR) 0x92, (_TCHAR) 0x90, (_TCHAR) 0x93, 
    (_TCHAR) 0xA2, (_TCHAR) 0xA3, (_TCHAR) 0xA1, (_TCHAR) 0xA4,

    (_TCHAR) 0xC2, (_TCHAR) 0xA6, (_TCHAR) 0x8C, (_TCHAR) 0x8D,  //0xD0
    (_TCHAR) 0x8B, (_TCHAR) 0xCE, (_TCHAR) 0x8E, (_TCHAR) 0xDB, 
    (_TCHAR) 0xC0, (_TCHAR) 0x81, (_TCHAR) 0x82, (_TCHAR) 0x80, 
    (_TCHAR) 0x83, (_TCHAR) 0x59, (_TCHAR) 0xF0, (_TCHAR) 0xF5,

    (_TCHAR) 0x9C, (_TCHAR) 0x9D, (_TCHAR) 0x9B, (_TCHAR) 0xCD,  //0xE0
    (_TCHAR) 0x9E, (_TCHAR) 0x9A, (_TCHAR) 0xA0, (_TCHAR) 0xC4, 
    (_TCHAR) 0x95, (_TCHAR) 0x96, (_TCHAR) 0x94, (_TCHAR) 0x97, 
    (_TCHAR) 0xA2, (_TCHAR) 0xA3, (_TCHAR) 0xA1, (_TCHAR) 0xA4,

    (_TCHAR) 0x20, (_TCHAR) 0xA7, (_TCHAR) 0x88, (_TCHAR) 0x89,  //0xF0
    (_TCHAR) 0x87, (_TCHAR) 0xCF, (_TCHAR) 0x8E, (_TCHAR) 0xD6, 
    (_TCHAR) 0xC0, (_TCHAR) 0x85, (_TCHAR) 0x86, (_TCHAR) 0x84, 
    (_TCHAR) 0x83, (_TCHAR) 0xC0, (_TCHAR) 0xF0, (_TCHAR) 0xCA  
  };

  if ( cCurrent >= 0x20)  // Translate after 0x20
  {
    return translate[  cCurrent - (_TUCHAR) 0x20 ];
  }

  return cCurrent;
}


CString CSCOT5945Pinpad::TranslatePromptText(CString& csText)
{
  CString csRet;
  LPTSTR szBuf = csRet.GetBuffer(csText.GetLength() + 1);

  for (int i = 0; i < csText.GetLength(); i++)
  {
    szBuf[i] = cTranslateWinCharTo5945(csText[i]);
  }
  szBuf[i] = 0;
  csRet.ReleaseBuffer();
  return csRet;
}
char* CSCOT5945Pinpad::ConvertLPWSTRToLPSTR (LPWSTR lpwszStrIn)
{
  LPSTR pszOut = NULL;
  if (lpwszStrIn != NULL)
  {
	int nInputStrLen = wcslen (lpwszStrIn);

	// Double NULL Termination
	int nOutputStrLen = WideCharToMultiByte (CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
	pszOut = new char [nOutputStrLen];

	if (pszOut)
	{
	  memset (pszOut, 0x00, nOutputStrLen);
	  WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
	}
  }
  return pszOut;
}

long CSCOT5945Pinpad::SetPromptTextThruDirectIO(const long lDIOPromptNum, 
                                                 CString& csText,
                                                 bool bIsFollowupPrompt,
                                                 bool bNeedToStore)
{
  ASSERT(lDIOPromptNum != 0);
  long rc = 0;

  BYTE cmd[37];

  // On the 5945, if we are in a state where a builtin prompt will be displayed,
  // then update that prompt in the 5945's table.  Otherwise, we will just
  // use a DirectIO to display text on the disply since that is MUCH quicker.

  // If we are changing stored prompt text, then look at value of bNeedToStore
  if ((((m_bMSREnabled) && (lDIOPromptNum  == 2)) || 
       (lDIOPromptNum  == 6) || 
       ((lDIOPromptNum == 3) && bIsFollowupPrompt)))
  {
    if (bNeedToStore)
    {
      cmd[0] = 36;  // VLI
      cmd[1] = 0xA4;  // Command code - Remote Display Table 1 Load
      cmd[2] = (BYTE) lDIOPromptNum;  // Message number to update

#ifdef _UNICODE

	int nInputStrLen = wcslen (csText);
	LPTSTR lpwszStrIn = csText.GetBuffer(nInputStrLen);
	LPSTR pszOut = ConvertLPWSTRToLPSTR(lpwszStrIn);
	csText.ReleaseBuffer();

	memset(&(cmd[3]), 0, 32);
	memcpy(&(cmd[3]), pszOut, strlen(pszOut));
	
	delete [] pszOut;

#else
	_stprintf((LPTSTR) &(cmd[3]), _T("%-32.32s"), csText);
#endif

      cmd[35] = ComputeBCC(cmd, 35);
      cmd[36] = 0;
      rc = DirectIO(cmd, 36);
    }
  }
  else // we always display, regardless of value of bNeedToStore
  {
    cmd[0] = 35;  // VLI
    cmd[1] = 0x23;  // Command code - Write on 2x16 display


#ifdef _UNICODE

	int nInputStrLen = wcslen (csText);
	LPTSTR lpwszStrIn = csText.GetBuffer(nInputStrLen);
	LPSTR pszOut = ConvertLPWSTRToLPSTR(lpwszStrIn);
	csText.ReleaseBuffer();

	memset(&(cmd[2]), 0, 32);
	memcpy(&(cmd[2]), pszOut, strlen(pszOut));
	delete [] pszOut;

#else
	_stprintf((LPTSTR) &(cmd[2]), _T("%-32.32s"), csText);
#endif

    cmd[34] = ComputeBCC(cmd, 34);
    cmd[35] = 0;
    rc = DirectIO(cmd, 35);

  }
  return rc;
}


BYTE CSCOT5945Pinpad::ComputeBCC(BYTE *pCmd, int nCmdLen)
{
  BYTE bcc = 0;

  for (int i = 0; i < nCmdLen; i++)
    bcc ^= pCmd[i];

  return bcc;
}


long CSCOT5945Pinpad::DirectIO(BYTE *pCommand, int nCmdLen)
{
  LONG myLong = 0;

  WCHAR wCmd[256];

  int nWideCmdLen = MultiByteToWideChar(CP_ACP, 
                                        MB_PRECOMPOSED, 
                                        (LPCSTR) pCommand, 
                                        nCmdLen, 
                                        wCmd, 
                                        256);

  BSTR bstr = SysAllocStringLen(wCmd, nWideCmdLen);

  long rc = m_Pinpad.DirectIO(0, &myLong, &bstr);

  SysFreeString(bstr);
  return rc;
}

long CSCOT5945Pinpad::GetFollowupPrompt(const long lDIOPromptNum)
{ 
  long rc = -1;

  if ((lDIOPromptNum == 2) || (lDIOPromptNum == 6))
    rc = m_lDirectIOPrompts[PPAD_MSG_PLEASEWAIT];

  return rc;
}


long CSCOTC2000Pinpad::SetPromptTextThruDirectIO(const long lDIOPromptNum, 
                                                 CString& csText,
                                                 bool bIsFollowupPrompt,
                                                 bool bNeedToStore)
{
  long rc = 0;
  if (bNeedToStore)
  {
    ASSERT(lDIOPromptNum != 0);

    long lDirectIOPrompt = lDIOPromptNum;

    BSTR bPrompt = csText.AllocSysString();
    long rc = m_Pinpad.DirectIO(NCRDIO_SCOT_PPAD_SETPROMPT, &lDirectIOPrompt, &bPrompt);
    SysFreeString(bPrompt);
  }
  return rc;
}

long CSCOTOPOSPinpad::SetPromptTextThruDirectIO(const long lDIOPromptNum, 
                                                 CString& csText,
                                                 bool bIsFollowupPrompt,
                                                 bool bNeedToStore)
{
  long rc = 0;
  if (bNeedToStore)
  {
    ASSERT(lDIOPromptNum != 0);

    long lDirectIOPrompt = lDIOPromptNum;

    BSTR bPrompt = csText.AllocSysString();
    long rc = m_Pinpad.DirectIO(NCRDIO_SCOT_PPAD_SETPROMPT, &lDirectIOPrompt, &bPrompt);
    SysFreeString(bPrompt);
  }
  return rc;
}

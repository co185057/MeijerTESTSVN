//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage.h
//
// Description: Base class for system message classes.  Differences between
//   subclasses are what combination of Continue and Cancel buttons they have.
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSTEMMESSAGEBASE
#define _SYSTEMMESSAGEBASE

#ifndef _CPPUNIT
#include "SMHelpModeState.h"
#else
#include "FakeSMHelpModeStateBase.h"
#endif //_CPPUNIT

class SMSystemMessageOpPassBase;        // forward declare for friend
class SMSystemMessageStartupBase;       // TAR 452451

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSystemMessageBase : public STATE(HelpModeState)
{
public:
#ifndef _CPPUNIT
           SMSystemMessageBase(BMPS   bitmap,
                           PSTEXT title,
                           bool   continueButton,
                           bool   cancelButton,
		                   LPCTSTR szMessage,  // char message text or NULL
		                   PSTEXT psMessage,  // message text
		                   PSTEXT promptText, // prompt text or NULL
		                   long   promptValue,
                           long   deviceClass,
                           bool   checkHealth,
                           bool showTotal = true,
                           bool showRetractedTotal = false,
                           bool okButton = false, //button 1
                           //Tar 121312
                           //bool flashBtn1 = false);
                           bool flashBtn1 = true,
						   bool fHTML = false); //SR672
	SMSystemMessageBase();
	SMStateBase *Deliver(BMPS   bitmap,
                           PSTEXT title,
                           bool   continueButton,
                           bool   cancelButton,
		                   LPCTSTR szMessage,  // char message text or NULL
		                   PSTEXT psMessage,  // message text
		                   PSTEXT promptText, // prompt text or NULL
		                   long   promptValue,
                           long   deviceClass,
                           bool   checkHealth,
                           bool showTotal = true,
                           bool showRetractedTotal = false,
                           bool okButton = false, //button 1
                           bool flashBtn1 = true,
						   bool argDisplayHTML = true); //SR672

    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase * Initialize(void);
  virtual void UnInitialize();      // TAR203166
  virtual SMStateBase  *OnMatchedWt(void);		// Smart Scale reported a matched weight TAR203166
  virtual bool PSRemoteLegalForState(); //207300 //HDMerge-062502
  virtual long GetDeviceClass(); //307389
  virtual SMStateBase* CkStateStackAndReturn(void);
#endif
  virtual SMStateBase * ShowDeviceErrorNotFixedScreen(void);
#ifndef _CPPUNIT
  /**
   * RFC 445940
   * \return true if this error must be fixed from the cash/paper status
   *         (a.k.a. media status screen).  false if this is a normal device
   *         error handled by checkhealth etc.
   */
  virtual bool IsMediaStatusError(void);
#endif
  static CString g_csButtonText;
protected:
#ifndef _CPPUNIT
  // a kindred spirit - allow access to these protected functions
  friend SMSystemMessageOpPassBase;
  friend SMSystemMessageStartupBase;   // TAR 452451     

  virtual void     showScreen(bool showInfo = true);
  virtual void     showRemoteScreen(bool showInfo); // not to be called externally but overwritable //HDMerge-062502
  virtual CString  getWaveFile() {return _T("");}
  virtual int      getTimeout() { return 0; }
#endif
  virtual SMStateBase * PSButton1(void);
  virtual SMStateBase * PSButton2(void);
#ifndef _CPPUNIT
  virtual SMStateBase * PSButton3(void);
  virtual SMStateBase * PSButton8(void); //HDMerge-062502
  virtual SMStateBase * PSCancelKey(void);
  virtual SMStateBase * OnBackToLGW(void);				// Smart Scale reported that the unmatched weight was removed
  virtual void    LogDeviceError(void);    //Log device error to reporing server 

  // TAR 321887 - Provide SSF access to the existing bInSystemMessage global
  //			  variable declared in SMSystemMessageBase.cpp
  virtual bool getInSystemMessage(void);
  virtual void setInSystemMessage(bool b);
  // TAR 321887

//private:
  BMPS    bitmap;
#endif
  bool    continueButton;
  bool    okButton;
  long    deviceClass;
  bool    checkHealth;
#ifndef _CPPUNIT
  bool    cancelButton;
  PSTEXT  title;
  PSTEXT  psMessage;
  PSTEXT  promptText;
  long    promptValue;

  bool	  m_bDisplayHTML; //SR672

  //Added to support TAR 116892
  bool    showTotal;
  bool    showRetractedTotal;
  bool    flashButton1;
  PSTEXTWAVE m_nWave;    //DeviceError
  CString m_csMovie;
  bool    m_bUseBitmap;
  bool    m_bCashChangerHopperDefault;
  bool	  m_bBillDispenserError;
  bool	  m_bCashChangerError;
  CString m_csTitle1;
  CString m_csTitle2;
  bool    m_bDeviceErrorNotFixed;
#endif
  bool	  m_bForceFixDeviceError;	//SR742
  CString csMessage;
  CString csCurContext;    // Tar 238987
#ifndef _CPPUNIT
  bool    m_bDeviceErrorLogged;   //Only log the device error once. Don't log it after pressing Continued button and the error still exists.
   bool m_bSavedSafetyCoverClosed; //TAR 298987-II
   bool m_bDegradedTextAppended;

	
  DECLARE_DYNCREATE(SMSystemMessageBase)// MFC Runtime class/object information
#endif
};

#endif

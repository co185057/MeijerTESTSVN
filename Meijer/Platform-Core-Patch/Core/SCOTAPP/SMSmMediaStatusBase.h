//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmMediaStatusBase.h
//
// TITLE: Class header for store mode MediaStatus state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmMediaStatusBaseSTATE
#define _SmMediaStatusBaseSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmMediaStatusBase : public STATE(StoreModeState)
{
public:
	SMSmMediaStatusBase();
  virtual SMStateBase  *Initialize(void);
  static bool m_bIsButtonFourPressed;		//4.2 - Card Dispenser
  virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *PSButton3(void);

  virtual SMStateBase  *PSButton4(void);
  virtual SMStateBase  *PSButton5(void);		//Security Maintenance Mode
  //Begin_Suspend
  virtual SMStateBase  *PSButton6(void);
  //End_Suspend
  virtual SMStateBase  *PSButton7(void);
  virtual SMStateBase  *PSButton8(void);
  virtual SMStateBase  *DMCashChangerInOperable(void); //TAR225069 CHU/DALE
  // NewUI
  virtual SMStateBase *PSReceiptUp(void);   // to disable and enable scroll buttons when 
  virtual SMStateBase *PSReceiptDown(void);   // to disable and enable scroll buttons when 

  // NewUI	
//private:
  virtual void initializeHopperList();
  virtual void initializeCurrencyList();
  virtual void initializePrinterList();
  virtual void initializeCashAcceptorStatusLine();
  virtual void initializeCoinAcceptorStatusLine();
  virtual void UpdateDeviceStatus(); 
  virtual long DMCheckHealth(long nDevClass);

  /*
  virtual bool getNextHopperStatusLine(CString& currencyMap,
                                       CString& hopperStatusList,
                                       CString& hopperStatusLine);
  virtual void getHopperStatus(const CString denominationCode,
                               CString& denominationStatus);
							   */

  virtual CString DisplayCoinDispenserStatus(void);     // TAR 140515
  virtual void InitializeCoinDispenserStatus(void);     // TAR 140515
  virtual void initializeCardDispenserStatus(void);		//4.2 - Card Dispenser
  bool    bReadDataFromRegistry;
  bool    bCashDispenserOnly;
  bool    bIsButtonOnePressed;                          //TAR225069 CHU/DALE        
  virtual SMStateBase  *DisplayCashDeviceError(void);  // SR742 
  bool    bCashDeviceErrorToDispaly;  //415172
 
   DECLARE_DYNCREATE(SMSmMediaStatusBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmMediaStatus(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmMediaStatusBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmMediaStatus objects.  This function is called by
// RETURNSTATE(SmMediaStatus) instead of calling new STATE(SmMediaStatus) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmMediaStatusBase.cpp and SMSmMediaStatus.cpp
// by using the macro IMPLEMENT_STATE(SmMediaStatus)

//DECLARE_STATE(SmMediaStatus)

#endif

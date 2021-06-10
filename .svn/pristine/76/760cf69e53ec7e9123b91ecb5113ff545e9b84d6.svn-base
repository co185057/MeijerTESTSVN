//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCashManagementBase.h
//
// TITLE: Class header for store mode CashManagement state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmCashManagementBaseSTATE
#define _SmCashManagementBaseSTATE

#include "SMStoreModeState.h"


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif


SMSmCashManagementBase : public STATE(StoreModeState)
{
private:
	bool m_bExcessLoadAmtEntered;   //TAR323136
	int m_nMaxAmtNeeded;             //TAR323136
public:
	SMSmCashManagementBase();							  // constructor
  virtual SMStateBase  *Initialize(void);        // required override
  virtual void UnInitialize();
  virtual SMStateBase  *TimedOut(void);
  bool DataInitialize(void);
  void GenericDataInitialize();
  void ParseCashCount(CString csCashCounts );

  void showScreen();

  static bool m_bBill;
  static bool m_bGlory;
  CString csSavedCashCountsString;
  bool m_bCoinHasMadeChanges;
  bool m_bBillHasMadeChanges;
  bool m_bSavedBill;

  static bool m_bfFirstEntry;     // flag to clear the deposit and dispense list when exiting this state
  static bool m_bfAuthenticated;     // flag to clear the deposit and dispense list when exiting this state
  static CArray<int,int> caValueList;
  static CArray<CString,CString> caDispenserDenomList;
  static CArray<int,int> caCountList;
  CArray<int,int> caLastCountList;  // used to eliminate unnecessary repainting

  // count arrays
  static CArray<int,int> m_caCashAcceptorCountList;
  static CArray<int,int> m_caCoinAcceptorCountList;
  static CArray<int,int> m_caCashAcceptorChangeInCountList;
  static CArray<int,int> m_caCoinAcceptorChangeInCountList;

  int nCashAcceptorTotalCount;
  int nCashAcceptorTotalValue;
  int nCashAcceptorTotalChangeInCount;
  int nCashAcceptorTotalChangeInValue; //tar 299529
  int nCoinAcceptorTotalValue;
  int nCoinAcceptorTotalCount;
  int nCoinAcceptorTotalChangeInCount;
  int nCoinAcceptorTotalChangeInValue; //tar 299529

  int nCashDispenserTotalCount;
  int nCoinDispenserTotalCount;
  int nCashDispenserTotalValue;
  int nCoinDispenserTotalValue;

  int nLoadIndex;


  //denom arrays
  static CArray<CString,CString> m_caCashAcceptorDenomList;
  static CArray<CString,CString> m_caCoinAcceptorDenomList;
  static int nCoinCashSeparator; //value - index of first Bill in caValueList 
  int nPurgeCnt;        // let's just separate the purge counts
  static int nSavedPurgeCnt;

  // this member not used. maybe we can use for future use. just keep it.
  // previous use of this function was to compute the change in count, current count 
  // minus this saved count. Now, we use the m_caChangeInCount instead.
  static CArray<int,int> m_caSavedOriginalCountList; 
                                                      
  static CArray<int,int> m_caChangeInCount;         // the change in count
  static CStringArray m_caSavedOriginalColumnPositions;
protected :
  virtual SMStateBase  *PSButton1(void); // let's first assume this to be the "Switch to Coin/Cash"
  virtual SMStateBase  *PSButton2(void); 
  virtual SMStateBase  *PSButton3(void); // acceptor pickup
  virtual SMStateBase  *PSButton4(void); // dispenser pickup
  virtual SMStateBase  *PSButton8(void); // Go Back button
  virtual SMStateBase *PSKeystroke(const long wParam, const long lParam); 
  virtual SMStateBase *DMAcceptor(void); //when it accepts coin/cash, do processing here
 // virtual long DMCashAcceptorAccept(void);    
 // virtual long DMCoinAcceptorAccept(void); 
  virtual void ShowAcceptorAndGenericData();

  virtual void ShowAcceptorColumns( long nColumnNumber, 
        VARIANT_BOOL vColumnVisible, 
        CString vCountOrValue,
        CString vDenomination
        );
  virtual void ShowAcceptorCylinder(  
        CString csAcceptorTotal,
        CString csChangeInCount,
        CString CylinderImagePath,
        CString CylinderPosition );
  virtual void ShowTotals(  
        CString csAcceptorTotal,
        CString csDispenserTotal,
        CString csGrandTotal );
  
  virtual void ShowColumn( long nColumnNumber, 
        VARIANT_BOOL vColumnVisible, 
        CString vCountOrValue,
        CString vChangeInCount, 
        CString CylinderImagePath,
        CString CylinderPosition,
        CString vDenomination,
        long vUnloadButtonEnable, 
        long vRemoveButtonEnable,
        int nRemoveAmount,
        long vLoadButtonfEnable,
        VARIANT_BOOL vUnloadButtonVisible = VARIANT_TRUE,
        VARIANT_BOOL vRemoveButtonVisible = VARIANT_TRUE,
        VARIANT_BOOL vLoadButtonfVisible = VARIANT_TRUE
        );

  virtual CString GetNewCylinderPosition( int i, COleVariant vColumnVisible, int nCoinOrBill, VARIANT_BOOL fVisible, CString& csImage );
  virtual void SaveOriginalCylinderPosition();

  virtual void ComputeTotals();
  virtual void DisposeAllData();
  virtual bool LoadAmount( int nLoadIndex, int nDeltaOrFullChange, bool bFullReplace = false );
  virtual void IgnoreChanges();
  virtual void ChangeAndShowContext( CString csContext = EMPTY_STRING );
  virtual bool SetDevices();

  // load functionalities
  virtual SMStateBase  *PSEnterKey(void);        // override for enter key
  virtual SMStateBase  *PSClearKey(void);
  virtual SMStateBase  *PSNumericKey(const long lKey);
  virtual SMStateBase *PSOtherSpecialKeys(const long); // minus key
  virtual void SendTBCashManagementData();

  DECLARE_DYNCREATE(SMSmCashManagementBase)// MFC Runtime class/object information
};


#endif

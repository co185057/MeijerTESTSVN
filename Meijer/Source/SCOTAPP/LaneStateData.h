#ifndef _LANESTATEDATA_H_
#define _LANESTATEDATA_H_

#ifndef _CPPUNIT
#include "PSXInterface.h"
#else
#include "FakeCommon.h"
#include "FakeSMStateBase.h"
#endif

class CLibraryHelper;

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
CLaneStateData
{
protected:
   CString m_csLaneState;
   CString m_csOverrideLaneState;
   static bool g_bOverride;
   CString m_csCurrentLaneState;
   CString CreateLaneChangeStateData(bool bStoreMode);
   CPSX *m_pPSX;
   CLibraryHelper *m_pLibraryHelper;

   
public:
    static LPCTSTR LOADING;
    static LPCTSTR OUTOFTRANSACTION;
    static LPCTSTR INTRANSACTION;
	static LPCTSTR INTRANSACTIONVOID;
    static LPCTSTR STORE;
    static LPCTSTR ENDOFTRANSACTION;
    static CString g_csCurrentNextGenUIAppState;
    static CString g_csPreviousNextGenUIAppState;

   CLaneStateData(CPSX *pPSX);
   CLaneStateData(CPSX *pPSX, CLibraryHelper &helper);
   virtual ~CLaneStateData();
   void SendLaneStateData(CString csLaneState, CString csOverrideLaneState, bool bOverride, bool bStoreMode);
   void SendNextGenUIData(CString csTransactionVariableName, COleVariant vValue);
   void SendNextGenUIData(CString csTransactionVariableName, bool bValue);
   void SendNextGenUIData(CString csTransactionVariableName, CString csValue);
   void SendNextGenUIData(CString csTransactionVariableName, long lValue);

   void SendNextGenUIAppState();

   CString CreateNextGenUIModeData(bool bAttendantMode, bool bTrainingMode, long lDegradedMode);
   CString CreateNextGenUIHotKeyState(LPCTSTR HKState, bool bShiftOn);

   void ResetNextGenDataTrxnVariables(void);

};
#endif // _LANESTATEDATA_H_


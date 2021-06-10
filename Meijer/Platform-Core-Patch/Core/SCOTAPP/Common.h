//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  Common.h
//
// TITLE:   Header file to inlcude most common headers.
//
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/SCOTAPP/Common.h 1     6/23/08 4:55p Sa250050 $
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _COMMONMAIN
#define _COMMONMAIN
#pragma warning(disable:4786)

#ifndef StringType
	#include <string>
	#ifdef _UNICODE
		#define StringType std::wstring
	#else
		#define StringType std::string
	#endif
#endif
#ifndef __ATLBASE_H__
   #include <atlbase.h>
#endif

#pragma warning( disable : 4800 )

// NOTE:  THIS CODE IS COPIED FROM MFC AND IS ESSENTIALLY REDEFINING DECLARE_DYNCREATE
// TO BE THE SAME AS DECLARE_SERIAL TO AVOID HAVING TO CHANGE ALL THE STATES IN THE BASE
// AND SSF FOR THE STATE CHANGE FEATURE.  THIS CODE SHOULD BE REVIEWED WHEN GOING TO
// VISUAL STUDIO.NET.  I THINK IT WILL STILL BE REQUIRED BUT IT SHOULD BE REVIEWED.
#undef DECLARE_DYNCREATE
#undef IMPLEMENT_DYNCREATE 
#define DECLARE_DYNCREATE(class_name) \
	_DECLARE_DYNCREATE(class_name) \
	AFX_API friend CArchive& AFXAPI operator>>(CArchive& ar, class_name* &pOb);
#define IMPLEMENT_DYNCREATE(class_name, base_class_name) \
	CObject* PASCAL class_name::CreateObject() \
		{ return new class_name; } \
	_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0x0001, \
		class_name::CreateObject) \
	AFX_CLASSINIT _init_##class_name(RUNTIME_CLASS(class_name)); \
	CArchive& AFXAPI operator>>(CArchive& ar, class_name* &pOb) \
		{ pOb = (class_name*) ar.ReadObject(RUNTIME_CLASS(class_name)); \
			return ar; } \


#include "CommonNCR.h"              // common NCR includes from ADK
#include "CommonServices.h"         // All services headers
#include "ItemObject.h"             // Item Object
#include "PSProcedures.h"           // PS procedures
#include "CTransBroker.h"           // TB 
//#include "receiptqueue.h"
//#include "journalqueue.h"

#include "InstoreBarcodes.h"		// for custom barcodes


#include "DMTriColorLight.h"        // DM TriColor Light class
//#include "DMInterface.h"            // MGV DM low level interface
#include "DMProcedures.h"           // DM procedures
#include "SAInterface.h"            // SA interface
//#include "SAProcedures.h"           // SA procedures
#include "ConfigObject.h"           // Configuration Object
#include "AppRestartCount.h"        // no instance required
#include "RAProcedures.h"			// Remote Approval
#include "HTProcedures.h"           // Hard Totals
#include "SCOTDevice.h"
#include "SCOTEncryptor.h"
#include "SCOTSigCap.h"
#include "SCOTAuxDisp.h"
#include "SCOTDevFactory.h"
#include "TrainingMode.h"
#include "PSIntList.h"

#include "SecMgrInterface.h"         // Security Manager Interface
#include "SecMgrProcedures.h"        // Security Manager Interface
#include "scotdataentry.h"
#include "DevicePropertyChanges.h"
#include "DeviceCtxStoreXML.h"
#include "dataentry.h"
#include "Reporting.h"
#include "FastlaneUtils.h"
#undef error
#include "KeyValueHandler.h"	// NewSec
#include "KVParmList.h"			// NewSec - KeyValue parser class
#include "SmKeyValue.h"
#include "SmConfigDefaults.h"
#include "CustomerInfo.h"

#define	 ScotError eo.Error

#ifndef	 _SCOT
#define  error eo.Error
#endif  

#ifdef _BUILDLIB_
#ifdef _SCOTSSF_
DLLIMPORT extern PSProceduresBase &ps;			// global PS object used by many
DLLIMPORT extern CCustomerInfoBase &custInfo;			// global Personalization object
#else 
DLLEXPORT extern PSProceduresBase &ps;           // global PS object used by many
DLLEXPORT extern CCustomerInfoBase &custInfo;           // global Personalization object
#endif

#else

#ifdef _SCOTSSF_
DLLEXPORT extern PSProcedures &ps;			// global PS object used by many
DLLEXPORT extern CCustomerInfo &custInfo;			// global Personalization object
#else 
DLLIMPORT extern PSProcedures &ps;           // global PS object used by many
DLLIMPORT extern CCustomerInfo &custInfo;           // global Personalization object
#endif

#endif

#ifdef _SCOTSSF_
DLLIMPORT extern ConfigObject co;			// global configuration Object
DLLIMPORT extern TraceObject  to;			// global trace Object
DLLIMPORT extern TraceObject  misMatchTrace;// global WLDB mismatch trace Object RFC 210177
DLLIMPORT extern ItemObject   io;			// global current item in progress
DLLIMPORT extern DMProcedures dm;			// global Device Mananger object
DLLIMPORT extern CTransBroker tb;			// global transaction broker object
DLLIMPORT extern RAProcedures ra;			// global remote approval object
// Please don't merge back in code .................Tina - Jan 20, 2002
//DLLIMPORT extern SAProcedures sa;           // global security agent object
// NewSec
DLLIMPORT extern SecMgrProcedures SecMgr;   // global Security Manager control
DLLIMPORT extern CSCOTDataEntry cDataEntry;
DLLIMPORT extern HTProcedures ht;           // global hard totals object
DLLIMPORT extern CInstoreCode cstoreCode;			//global In-Store barcode
DLLIMPORT extern CConfigMultiLabelBarcode g_MultiLabelBarcodes;
DLLIMPORT extern CustomMessage CustomMessageObj;    // global current custom message - TAR127247
DLLIMPORT extern CSCOTTrainingMode TrainingModeObj; // global training mode object
DLLIMPORT extern CReporting rp; // global logging object
DLLIMPORT extern HDEVCONTEXT hDC;                   // the device context from PSThread()
DLLIMPORT extern CSCOTSigCap*			gpSCOTSigCap;

#else
 
DLLEXPORT extern ConfigObject co;			// global configuration Object
DLLEXPORT extern TraceObject  to;			// global trace Object
DLLEXPORT extern TraceObject  misMatchTrace;// global WLDB mismatch trace Object RFC 210177
DLLEXPORT extern ItemObject   io;			// global current item in progress
DLLEXPORT extern DMProcedures dm;			// global Device Mananger object
DLLEXPORT extern CTransBroker tb;			// global transaction broker object
DLLEXPORT extern RAProcedures ra;			// global remote approval object
// Please don't merge back in code .................Tina - Jan 20, 2002
//DLLEXPORT extern SAProcedures sa;           // global security agent objec
// NewSec
DLLEXPORT extern SecMgrProcedures SecMgr;   // global Security Manager object

DLLEXPORT extern CSCOTDataEntry cDataEntry;
DLLEXPORT extern HTProcedures ht;           // global hard totals object
DLLEXPORT extern CInstoreCode cstoreCode;
DLLEXPORT extern CConfigMultiLabelBarcode g_MultiLabelBarcodes;
DLLEXPORT extern CustomMessage CustomMessageObj;    // global current custom message - TAR127247
DLLEXPORT extern CReporting rp;    // global logging object
DLLEXPORT extern CSCOTTrainingMode TrainingModeObj; // global training mode object
DLLEXPORT extern HDEVCONTEXT hDC;   
DLLEXPORT extern CSCOTSigCap*			gpSCOTSigCap;                // the device context from PSThread()

#endif

extern CSCOTDevFactory		gSCOTDevFactory;
extern CSCOTEncryptor*		gpSCOTEncryptor;
extern CSCOTAuxDisp*		gpSCOTAuxDisp;
extern CSCOTTakeawayBelt*   gpSCOTTakeawayBelt;

// APIExpose   PSRC        PSEnableNotification(HDEVCONTEXT hDC, BOOL fEnable, PSELEMENTTYPE lElementTypes);
#define PSEnableNotification(a,b,c) {trace(L0,_T("++++++TOUCH :%s"), (##b)? _T("ENABLE") : _T("DISABLE"));\
                                        PSEnableNotification(a,b,c); \
									 trace(L0,_T("------TOUCH")); }   
#ifdef strace
#undef strace
#endif
#define strace misMatchTrace.Trace              // set to the global trace object for security log RFC 210177

//RFC263887+
class CDeferredMessage
   {
   public:
      CDeferredMessage(UINT m = 0, WPARAM wp = 0, LPARAM lp = 0) : 
         message(m), wParam(wp), lParam(lp){}
      CDeferredMessage(const CDeferredMessage& ref)
         {message=ref.message;wParam=ref.wParam;lParam=ref.lParam;}
      ~CDeferredMessage(){}
      CDeferredMessage& operator =(const CDeferredMessage& ref)
         {message=ref.message;wParam=ref.wParam;lParam=ref.lParam;return *this;}
      UINT message;
      WPARAM wParam;
      LPARAM lParam;
   };


// Little list of Window messages we have deferred.  When we return from
// processing a message, we check this list to see if any came in while
// we were busy.
extern CList<CDeferredMessage, CDeferredMessage&> s_qDeferredMessages;

//RFC263887-

#endif  //_COMMOMMAIN



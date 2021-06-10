// TraceSystem.h: interface for the TraceSystem class.
//
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///
/// \file TraceSystem.h
/// \brief interface for the TraceSystem class.
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////////
#if !defined(AFX_TRACESYSTEM_H__1120104F_5D38_11D6_B79F_00A0C9EDD45C__INCLUDED_)
#define AFX_TRACESYSTEM_H__1120104F_5D38_11D6_B79F_00A0C9EDD45C__INCLUDED_

//#if _MFC_VER < 0x0700
	#import "_traceExe.tlb"
//#else
//	#import "progid:TraceExe.TraceWriter"
//#endif

using namespace traceExe;
  
#include <tchar.h>
#include "safearrayVariant.h"
#include "LogFile.h"

#include <queue>
#include <map>

/// \def TRACE_MASK_ERROR
///	\brief Mask for ERROR Message 0x01
#define TRACE_MASK_ERROR    0x01 

/// \def TRACE_MASK_WARNING
///	\brief Mask for WARNING Message 0x02
#define TRACE_MASK_WARNING  0x02 

/// \def TRACE_MASK_INFO
///	\brief Mask for INFO Message 0x04
#define TRACE_MASK_INFO     0x04 

/// \def TRACE_MASK_DEBUG
///	\brief Mask for DEBUG Message 0x08
#define TRACE_MASK_DEBUG    0x08

/// \def TRACE_MASK_ENTRY
///	\brief Mask for ENTRY Message 0x10
#define TRACE_MASK_ENTRY    0x10

/// \def TRACE_MASK_IN
///	\brief Mask for IN Message 0x20
#define TRACE_MASK_IN       0x20

/// \def TRACE_MASK_OUT
///	\brief Mask for OUT Message 0x40
#define TRACE_MASK_OUT	    0x40

/// \def TM_MODULE_1
///	\brief Define the Module 1 0x0100
#define TM_MODULE_1 0x0100

/// \def TM_MODULE_2
///	\brief Define the Module 2 0x0200
#define TM_MODULE_2 0x0200

/// \def TM_MODULE_3
///	\brief Define the Module 3 0x0400
#define TM_MODULE_3 0x0400

/// \def TM_MODULE_4
///	\brief Define the Module 4 0x0800
#define TM_MODULE_4 0x0800

/// \def TM_MODULE_5
///	\brief Define the Module 5 0x1000
#define TM_MODULE_5 0x1000

/// \def TM_MODULE_6
///	\brief Define the Module 6 0x2000
#define TM_MODULE_6 0x2000

/// \def TM_MODULE_7
///	\brief Define the Module 7 0x4000
#define TM_MODULE_7 0x4000

/// \def TM_MODULE_8
///	\brief Define the Module 8 0x8000
#define TM_MODULE_8 0x8000

#ifndef TM_MODULE_ID 
		/// \def TM_MODULE_ID
		///	\brief Define the Module ID default to TM_MODULE_1
    #define TM_MODULE_ID TM_MODULE_1
#endif

/// \def TM_ERROR
///	\brief Define Macro for Error Message
#define TM_ERROR       TM_MODULE_ID | TRACE_MASK_ERROR,    T_ID,__LINE__

/// \def TM_WARNING
///	\brief Define Macro for warning Message
#define TM_WARNING     TM_MODULE_ID | TRACE_MASK_WARNING,  T_ID,__LINE__

/// \def TM_INFO
///	\brief Define Macro for info Message
#define TM_INFO        TM_MODULE_ID | TRACE_MASK_INFO,     T_ID,__LINE__

/// \def TM_DEBUG
///	\brief Define Macro for debug Message
#define TM_DEBUG       TM_MODULE_ID | TRACE_MASK_DEBUG,    T_ID,__LINE__

/// \def TM_ENTRY
///	\brief Define Macro for entry Message
#define TM_ENTRY       TM_MODULE_ID | TRACE_MASK_ENTRY,    T_ID,__LINE__

/// \def TM_IN
///	\brief Define Macro for in Message
#define TM_IN          TM_MODULE_ID | TRACE_MASK_IN,       T_ID,__LINE__

/// \def TM_OUT
///	\brief Define Macro for out Message
#define TM_OUT         TM_MODULE_ID | TRACE_MASK_OUT,      T_ID,__LINE__


#if _MSC_VER >= 7000

/// \def TM_ML_ERROR
///	\brief Define Macro for Error Message with Function
#define TM_ML_ERROR     TM_MODULE_ID | TRACE_MASK_ERROR,    __FUNCTION__,__LINE__

/// \def TM_ML_WARNING
///	\brief Define Macro for warning Message with Function
#define TM_ML_WARNING   TM_MODULE_ID | TRACE_MASK_WARNING,  __FUNCTION__,__LINE__

/// \def TM_ML_INFO
///	\brief Define Macro for info Message with Function
#define TM_ML_INFO      TM_MODULE_ID | TRACE_MASK_INFO,     __FUNCTION__,__LINE__

/// \def TM_ML_DEBUG
///	\brief Define Macro for debug Message with Function
#define TM_ML_DEBUG     TM_MODULE_ID | TRACE_MASK_DEBUG,    __FUNCTION__,__LINE__

/// \def TM_ML_ENTRY
///	\brief Define Macro for entry Message with Function
#define TM_ML_ENTRY     TM_MODULE_ID | TRACE_MASK_ENTRY,    __FUNCTION__,__LINE__

/// \def TM_IN
///	\brief Define Macro for in Message with Function
#define TM_IN           TM_MODULE_ID | TRACE_MASK_IN,       __FUNCTION__,__LINE__

/// \def TM_OUT
///	\brief Define Macro for out Message with Function
#define TM_OUT          TM_MODULE_ID | TRACE_MASK_OUT,      __FUNCTION__,__LINE__

#else  // VC6 does not support __FUNCTION__

/// \def TM_ML_ERROR
///	\brief Define Macro for Error Message
#define TM_ML_ERROR     TM_MODULE_ID | TRACE_MASK_ERROR,    T_ID,__LINE__

/// \def TM_ML_WARNING
///	\brief Define Macro for warning Message
#define TM_ML_WARNING   TM_MODULE_ID | TRACE_MASK_WARNING,  T_ID,__LINE__

/// \def TM_ML_INFO
///	\brief Define Macro for info Message
#define TM_ML_INFO      TM_MODULE_ID | TRACE_MASK_INFO,     T_ID,__LINE__

/// \def TM_ML_DEBUG
///	\brief Define Macro for debug Message
#define TM_ML_DEBUG     TM_MODULE_ID | TRACE_MASK_DEBUG,    T_ID,__LINE__

/// \def TM_ML_ENTRY
///	\brief Define Macro for entry Message
#define TM_ML_ENTRY     TM_MODULE_ID | TRACE_MASK_ENTRY,    T_ID,__LINE__

/// \def TM_ML_IN
///	\brief Define Macro for in Message
#define TM_ML_IN        TM_MODULE_ID | TRACE_MASK_IN,       T_ID,__LINE__

/// \def TM_ML_OUT
///	\brief Define Macro for out Message
#define TM_ML_OUT       TM_MODULE_ID | TRACE_MASK_OUT,      T_ID,__LINE__

#endif


//long tbtrace(int traceMask,  LPCTSTR szsource, const int iLineNum, LPCTSTR sFormat, ...);
//if(ptbto) ptbto->Trace              // set to the global trace object

using namespace std ;
//////////////////////////////////////////////////////////////////////
///
/// qData is ...
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////
class qData
{
public:
	//////////////////////////////////////////////////////////////////////
	///
	/// Constructor
	///
	//////////////////////////////////////////////////////////////////////
	qData():m_traceMask(0),m_csTrace(_T("")),m_pData(NULL),m_nDataLen(0)
	{};

	//////////////////////////////////////////////////////////////////////
	///
	/// Destructor
	///
	//////////////////////////////////////////////////////////////////////
	~qData()
	{
		delete [] m_pData;
	};

	ULONG		m_traceMask;	///< trace mask variable
	CString	m_csTrace;		///< Trace String
	BYTE		*m_pData;			///< pointer to BYTE data
	UINT		m_nDataLen;		///< data length
	
	//////////////////////////////////////////////////////////////////////
	///
	/// SetData, ### ADD DESC ###
	///
	///	@param BYTE *
	///	@paramg int
	//////////////////////////////////////////////////////////////////////
	void SetData(BYTE *pd, int len)
	{
		delete [] m_pData;
		m_pData=new BYTE[len];
		memcpy(m_pData, pd, len);
		m_nDataLen= len;
	}
};

typedef queue<qData*> CStringsQueue;
typedef map<int , int > CThreadMap;

//#ifdef _AFXDLL

typedef struct _tdata {
    CStringsQueue *m_pmq; ///< queue for trace string
	 CString m_csTraceName;	///< Trace name            
} tdata;

//////////////////////////////////////////////////////////////////////
///
/// CTraceSystemMgr is ...
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////
class CTraceSystemMgr
{
public:
		//////////////////////////////////////////////////////////////////////
		///
		/// Constructor
		///
		///	@param LPCTSTR
		///	@param ULONG
		//////////////////////////////////////////////////////////////////////
		CTraceSystemMgr(LPCTSTR szTraceName=NULL, ULONG moduleId= TM_MODULE_1);

		//////////////////////////////////////////////////////////////////////
		///
		/// Destructor
		///
		//////////////////////////////////////////////////////////////////////
		virtual ~CTraceSystemMgr();

		//////////////////////////////////////////////////////////////////////
		///
		/// Trace, ### ADD DESC ###
		///
		///	@param ULONG
		///	@param LPCTSTR
		///	@param int
		///	@param LPCTSTR
		///	@return long
		//////////////////////////////////////////////////////////////////////
		long Trace(ULONG traceMask,  LPCTSTR szsource, const int iLineNum, LPCTSTR sFormat, ...);

		//////////////////////////////////////////////////////////////////////
		///
		/// HexTrace, ### ADD DESC ###
		///
		///	@param ULONG
		///	@param LPCTSTR
		///	@param int
		///	@param LPCTSTR
		///	@param UCHAR *
		///	@param ULONG
		///	@return long
		//////////////////////////////////////////////////////////////////////
		long HexTrace(ULONG traceMask, LPCTSTR szsource, const int iLineNum, 
									 LPCTSTR szTitle, UCHAR *phexData, ULONG hexDataLen=0);

		//////////////////////////////////////////////////////////////////////
		///
		/// HexTrace, ### ADD DESC ###
		///
		///	@param ULONG
		/// @param LPCTSTR
		///	@param UCHAR *
		///	@param ULONG
		///	@return long
		//////////////////////////////////////////////////////////////////////
		long HexTrace(ULONG traceMask, LPCTSTR szTitle, UCHAR *phexData, ULONG hexDataLen);

		//////////////////////////////////////////////////////////////////////
		///
		/// internalTrace, ### ADD DESC ###
		///
		///	@param ULONG
		///	@param LPCTSTR
		///	@param LPCTSTR
		///	@param va_list
		///	@return long
		//////////////////////////////////////////////////////////////////////
		long internalTrace(ULONG traceMask,  LPCTSTR szsourceAndLine, LPCTSTR sFormat, va_list aList);

		//////////////////////////////////////////////////////////////////////
		///
		/// StartTrace, ### ADD DESC ###
		///
		//////////////////////////////////////////////////////////////////////
		void StartTrace();

		//////////////////////////////////////////////////////////////////////
		///
		/// EndTrace, ### ADD DESC ###
		///
		//////////////////////////////////////////////////////////////////////
		void EndTrace();

		//////////////////////////////////////////////////////////////////////
		///
		/// createQueue, ### ADD DESC ###
		///
		//////////////////////////////////////////////////////////////////////
		void createQueue() {
			m_tdata.m_pmq= new CStringsQueue();
		}
private:
		CThreadMap m_WriterPtrs;	///< ThreadMap for Writer
		CThreadMap::iterator m_WriterIterator;	///< Iterator for writer

		ULONG m_moduleId;	///< module ID

      tdata m_tdata;

		HANDLE m_hCOMThread; ///< HANDLE for COM thread
		
		//////////////////////////////////////////////////////////////////////
		///
		/// SetupCOMClientThread, ### ADD DESC ###
		///
		//////////////////////////////////////////////////////////////////////
		void SetupCOMClientThread();

		//////////////////////////////////////////////////////////////////////
		///
		/// putMsg, ### ADD DESC ###
		///
		///	@param qData
		//////////////////////////////////////////////////////////////////////
		void putMsg(qData *pqdata);		
};

#ifdef TRACESYSTEM_MAIN
   CTraceSystemMgr *ptbto=NULL;
#else
   extern CTraceSystemMgr *ptbto;//*ptbto;
#endif

/// \def Macro for tbtrace
/// \brief Call Trace method
#define tbtrace      if (ptbto) ptbto->Trace 

/// \def Macro for tbtrace
/// \brief Call Trace method
#define tbtraceIO(a) CTraceEntry tmpTraceVar(TM_ENTRY, a)

/// \def Macro for tbtrace
/// \brief Call Trace method
#define STARTtbtrace if (ptbto) ptbto->StartTrace 

/// \def Macro for tbtrace
/// \brief Call Trace method
#define ENDtbtrace   if (ptbto) ptbto->EndTrace 

/// \def Macro for tbtrace
/// \brief Call Trace method
#define tbhextrace   if (ptbto) ptbto->HexTrace 

//#endif // _AFXDLL

//////////////////////////////////////////////////////////////////////
///
/// CTraceSystem is ...
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////
class CTraceSystem  
{
public:  
    // public functions
		//////////////////////////////////////////////////////////////////////
		///
		/// Constructor
		///
		///	@param LPCTSTR
		///	@param ULONG
		//////////////////////////////////////////////////////////////////////
    CTraceSystem(LPCTSTR szTraceLog = _T("C:\\scot\\bin\\TBTrace.log"), ULONG moduleId=TM_MODULE_1);

		//////////////////////////////////////////////////////////////////////
		///
		/// Destructor
		///
		//////////////////////////////////////////////////////////////////////
    virtual ~CTraceSystem();

		//////////////////////////////////////////////////////////////////////
		///
		/// SetModuleID, ### ADD DESC ###
		///
		///	@param ULONG
		//////////////////////////////////////////////////////////////////////
    void SetModuleID(ULONG moduleId);

		//////////////////////////////////////////////////////////////////////
		///
		/// Trace, ### ADD DESC ###
		///
		///	@param ULONG
		///	@param LPCTSTR
		///	@param int
		///	@param LPCTSTR
		///	@return long
		//////////////////////////////////////////////////////////////////////
    long Trace(ULONG traceMask, LPCTSTR szsource, const int iLineNum, LPCTSTR sFormat, ...);

		//////////////////////////////////////////////////////////////////////
		///
		/// internalTrace,
		///
		///	@param ULONG
		///	@param LPCTSTR
		///	@param LPCTSTR
		///	@param va_list
		///	@return long
		//////////////////////////////////////////////////////////////////////
    long internalTrace(ULONG traceMask,  LPCTSTR szsourceAndLineNum, LPCTSTR sFormat, va_list aList);

		//////////////////////////////////////////////////////////////////////
		///
		/// Trace, ### ADD DESC ###
		///
		///	@param ULONG
		///	@param LPCTSTR
		///	@return long
		//////////////////////////////////////////////////////////////////////
		long Trace(ULONG traceMask, LPCTSTR strTrace);

		//////////////////////////////////////////////////////////////////////
		///
		/// HexTrace, ### ADD DESC ###
		///
		///	@param ULONG
		///	@param LPCTSTR
		///	@param int
		///	@param LPCTSTR
		///	@param UCHAR *
		/// @param ULONG
		///	@return long
		//////////////////////////////////////////////////////////////////////
    long HexTrace(ULONG traceMask, LPCTSTR szsource, const int iLineNum,
                  LPCTSTR szTitle, UCHAR *phexData, ULONG hexDataLen =0);

		//////////////////////////////////////////////////////////////////////
		///
		/// HexTrace, ### ADD DESC ###
		///
		///	@param ULONG
		///	@param LPCTSTR
		///	@param UCHAR *
		///	@param ULONG
		///	@return long
		//////////////////////////////////////////////////////////////////////
		long HexTrace(ULONG traceMask, LPCTSTR szTitle, UCHAR *phexData, ULONG hexDataLen=0);

		//////////////////////////////////////////////////////////////////////
		///
		/// ConvertBufferToVariant, ### ADD DESC ###
		///
		///	@param UCHAR *
		///	@param ULONG
		///	@param VARIANTARG &
		//////////////////////////////////////////////////////////////////////
    void ConvertBufferToVariant(UCHAR* i_pBuffer, ULONG i_ulnBytes, VARIANTARG& o_varBuffer);

		//////////////////////////////////////////////////////////////////////
		///
		/// Create, ### ADD DESC ###
		///
		///	@return bool
		//////////////////////////////////////////////////////////////////////
    bool Create();

		//////////////////////////////////////////////////////////////////////
		///
		/// Release, ### ADD DESC ###
		///
		//////////////////////////////////////////////////////////////////////
    void Release();

    // public data
    ULONG   m_moduleId;	///< ULONG for module ID
    ULONG   m_ncurMask;	///< ULONG for current mask
    DWORD   m_threadId;	///< DWORD for THREAD ID
		int			m_nIndent;	///< int for indent
    CString m_TraceLog;	///< trace log
private:
    // private functions
		//////////////////////////////////////////////////////////////////////
		///
		/// Hide assignment
		///
		//////////////////////////////////////////////////////////////////////
    CTraceSystem & operator = (const CTraceSystem &);

		//////////////////////////////////////////////////////////////////////
		///
		/// Hide copy const
		///
		//////////////////////////////////////////////////////////////////////
    CTraceSystem(CTraceSystem &);

    // private data
    ITraceWriterPtr ptw;	///< Trace Writer object
    bool m_bCreated;			///< flag it trace is created
    bool m_bStop;					///< flag to stop
    CSafeArrayVariant m_VarSA;	///< SafeArrayVariant
    CLogFile *m_pEmergencyLogFile;	///< Emergency log file
    
		//////////////////////////////////////////////////////////////////////
		///
		/// LogEmergencyMessage, ### ADD DESC ###
		///
		///	@param LPCTSTR
		//////////////////////////////////////////////////////////////////////
		void LogEmergencyMessage(LPCTSTR strTemp);	

};


#ifdef _DEBUG
#define     TBASSERT_LINE_FILE(f, nLine, sFile) do \
{ \
  if (!(f)) \
  { \
     DWORD dwStatus= GetLastError(); \
     ASSERT(false); \
     CString str; \
     str.Format(_T("AssertFailed at line %d in file %s GetLastError= %d"), nLine, sFile, dwStatus); \
     tbtrace(TM_WARNING, str); \
  } \
} while(0)
#else
#define     TBASSERT_LINE_FILE(f, nLine, sFile) do \
{ \
    if (!(f)) \
   { \
      DWORD dwStatus= GetLastError(); \
      CString str; \
      str.Format(_T("AssertFailed at line %d in file %s GetLastError= %d"), nLine, sFile, dwStatus); \
      tbtrace(TM_WARNING, str); \
   } \
} while(0)

#endif
#define TBASSERT(f) TBASSERT_LINE_FILE(f, __LINE__, __FILE__)

//////////////////////////////////////////////////////////////////////
///
/// CTraceEntry is ...
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////
class CTraceEntry {
public:
		//////////////////////////////////////////////////////////////////////
		///
		/// Constructor
		///
		///	@param ULONG
		///	@param LPCTSTR
		///	@param int
		///	@param LPCTSTR
		//////////////////////////////////////////////////////////////////////
		CTraceEntry (ULONG traceMask, LPCTSTR szsource, const int iLineNum, LPCTSTR szEntry);

		//////////////////////////////////////////////////////////////////////
		///
		/// Destructor
		///
		//////////////////////////////////////////////////////////////////////
		~CTraceEntry ();
private:
		ULONG m_traceMask;	///< Mask for trace in ULONG
		CString m_csSource;	///< Source Name
		int m_nLine;				///< Line no
		CString m_csText;		///< Trace text
};

#endif // !defined(AFX_TRACESYSTEM_H__1120104F_5D38_11D6_B79F_00A0C9EDD45C__INCLUDED_)
